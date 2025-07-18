/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file system_startup.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief first C code called on startup. Will call user code created outside
 * the HAL.
 *
 */

#include <stddef.h>
#include <stdbool.h>
#include "mpfs_hal/mss_hal.h"
#ifdef  MPFS_HAL_HW_CONFIG
#include "../common/nwc/mss_nwc_init.h"
#endif
#include "system_startup_defs.h"


static uint32_t parked_harts = 0U;
void* __dso_handle = (void*) &__dso_handle;
static void init_global_constructors(void);

/*==============================================================================
 * This function is called by the lowest enabled hart (MPFS_HAL_FIRST_HART) in
 * the configuration file :
 * (src/boards/<my-board/platform_config/mpfs_hal_config/mss_sw_config.h )
 *
 * The other harts up to MPFS_HAL_LAST_HART are placed in wfi at this point.
 * This function brings them out of wfi in sequence.
 * If you need to modify this function, create your own one in a user directory
 * space
 * e.g. /hart0/e51.c
 */
#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
__attribute__((weak)) int main_first_hart(HLS_DATA* hls)
{
    uint64_t hartid = read_csr(mhartid);

    if(hartid == MPFS_HAL_FIRST_HART)
    {
        uint8_t hart_id;
        ptrdiff_t stack_top;

        /*
         * We only use code within the conditional compile
         * #ifdef MPFS_HAL_HW_CONFIG
         * if this program is used as part of the initial board bring-up
         * Please comment/uncomment MPFS_HAL_HW_CONFIG define in
         * src/boards/<my-board/platform_config/mpfs_hal_config/sw_config.h
         * as required.
         */
        init_memory();
#ifndef MPFS_HAL_HW_CONFIG
        hls->my_hart_id = MPFS_HAL_FIRST_HART;
#endif
#ifdef  MPFS_HAL_HW_CONFIG
        load_virtual_rom();
        (void)init_bus_error_unit();
        (void)init_mem_protection_unit();
        (void)init_pmp((uint8_t)MPFS_HAL_FIRST_HART);
        (void)mss_set_apb_bus_cr((uint32_t)LIBERO_SETTING_APBBUS_CR);
        (void)mss_set_gpio_interrupt_fab_cr((uint32_t)LIBERO_SETTING_GPIO_INTERRUPT_FAB_CR);
#endif  /* MPFS_HAL_HW_CONFIG */
        /*
         * Initialise NWC
         *      Clocks
         *      SGMII
         *      DDR
         *      IOMUX
         */
#ifdef  MPFS_HAL_HW_CONFIG
        (void)mss_nwc_init();
        (void)mss_nwc_init_ddr();
        init_global_constructors();

        /* main hart init's the PLIC */
        PLIC_init_on_reset();
        /*
         * Start the other harts. They are put in wfi in entry.S
         * When debugging, harts are released from reset separately,
         * so we need to make sure hart is in wfi before we try and release.
        */
        stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h0$);
        hls = (HLS_DATA*)(stack_top - HLS_DEBUG_AREA_SIZE);
        hls->in_wfi_indicator = HLS_MAIN_HART_STARTED;
        hls->my_hart_id = MPFS_HAL_FIRST_HART;
        WFI_SM sm_check_thread = INIT_THREAD_PR;
        hart_id = MPFS_HAL_FIRST_HART + 1U;
        while( hart_id <= MPFS_HAL_LAST_HART)
        {
            uint32_t wait_count = 0U;

            switch(sm_check_thread)
            {
                default:
                case INIT_THREAD_PR:

                    switch (hart_id)
                    {
                        case 1:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h1$);
                            break;
                        case 2:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h2$);
                            break;
                        case 3:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h3$);
                            break;
                        case 4:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h4$);
                            break;
                    }
                    hls = (HLS_DATA*)(stack_top - HLS_DEBUG_AREA_SIZE);
                    sm_check_thread = CHECK_WFI;
                    wait_count = 0U;
                    break;

                case CHECK_WFI:
                    if( hls->in_wfi_indicator == HLS_OTHER_HART_IN_WFI )
                    {
                        /* Separate state- to add a little delay */
                        sm_check_thread = SEND_WFI;
                    }
                    break;

                case SEND_WFI:
                    hls->my_hart_id = hart_id; /* record hartid locally */
                    raise_soft_interrupt(hart_id);
                    sm_check_thread = CHECK_WAKE;
                    wait_count = 0UL;
                    break;

                case CHECK_WAKE:
                    if( hls->in_wfi_indicator == HLS_OTHER_HART_PASSED_WFI )
                    {
                        sm_check_thread = INIT_THREAD_PR;
                        hart_id++;
                        wait_count = 0UL;
                    }
                    else
                    {
                        wait_count++;
                        if(wait_count > 0x10U)
                        {
                            if( hls->in_wfi_indicator == HLS_OTHER_HART_IN_WFI )
                            {
                                hls->my_hart_id = hart_id; /* record hartid locally */
                                raise_soft_interrupt(hart_id);
                                wait_count = 0UL;
                            }
                        }
                    }
                    break;
            }
        }
        stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h0$);
        hls = (HLS_DATA*)(stack_top - HLS_DEBUG_AREA_SIZE);
        hls->in_wfi_indicator = HLS_MAIN_HART_FIN_INIT;
        
        /* Turn off peripheral RAM that is not being used */
        mss_turn_off_unused_ram_clks();
        /*
         * Turn on fic interfaces by default. Drivers will turn on/off other MSS
         * peripherals as required.
         */
        (void)mss_config_clk_rst(MSS_PERIPH_FIC0, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
        (void)mss_config_clk_rst(MSS_PERIPH_FIC1, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
        (void)mss_config_clk_rst(MSS_PERIPH_FIC2, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
        (void)mss_config_clk_rst(MSS_PERIPH_FIC3, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
        /* enable the fabric */
        mss_enable_fabric();

#endif /* MPFS_HAL_HW_CONFIG */
        (void)main_other_hart(hls);
    }

    __builtin_unreachable();
    /* should never get here */
    while(true)
    {
       static volatile uint64_t counter = 0U;
       /* Added some code as debugger hangs if in loop doing nothing */
       counter = counter + 1U;
    }

    return (0);
}
#endif

/*==============================================================================
 * This function is called by the lowest enabled hart (MPFS_HAL_FIRST_HART) in
 * the configuration file :
 * (src/boards/<my-board/platform_config/mpfs_hal_config/mss_sw_config.h )
 *
 * The other harts up to MPFS_HAL_LAST_HART are placed in wfi at this point.
 * This function brings them out of wfi in sequence.
 * If you need to modify this function, create your own one in a user directory
 * space
 * e.g. /hart0/e51.c
 */
#if (IMAGE_LOADED_BY_BOOTLOADER == 1)

__attribute__((weak)) int main_first_hart_app(HLS_DATA* hls)
{
    uint64_t hartid = read_csr(mhartid);

    if(hartid == MPFS_HAL_FIRST_HART)
    {
        uint8_t hart_id;
        ptrdiff_t stack_top;

        init_memory();
        init_global_constructors();

        hls->my_hart_id = MPFS_HAL_FIRST_HART;
        hls->in_wfi_indicator = HLS_MAIN_HART_STARTED;
        WFI_SM sm_check_thread = INIT_THREAD_PR;
        hart_id = MPFS_HAL_FIRST_HART + 1U;
        while( hart_id <= MPFS_HAL_LAST_HART)
        {
            uint32_t wait_count = 0U;

            switch(sm_check_thread)
            {
                default:
                case INIT_THREAD_PR:

                    switch (hart_id)
                    {
                        case 1:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h1$);
                            break;
                        case 2:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h2$);
                            break;
                        case 3:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h3$);
                            break;
                        case 4:
                            stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h4$);
                            break;
                    }
                    hls = (HLS_DATA*)(stack_top - HLS_DEBUG_AREA_SIZE);
                    sm_check_thread = CHECK_WFI;
                    wait_count = 0U;
                    break;

                case CHECK_WFI:
                    if( hls->in_wfi_indicator == HLS_OTHER_HART_IN_WFI )
                    {
                        /* Separate state- to add a little delay */
                        sm_check_thread = SEND_WFI;
                    }
                    break;

                case SEND_WFI:
                    hls->my_hart_id = hart_id; /* record hartid locally */
                    raise_soft_interrupt(hart_id);
                    sm_check_thread = CHECK_WAKE;
                    wait_count = 0UL;
                    break;

                case CHECK_WAKE:
                    if( hls->in_wfi_indicator == HLS_OTHER_HART_PASSED_WFI )
                    {
                        sm_check_thread = INIT_THREAD_PR;
                        hart_id++;
                        wait_count = 0UL;
                    }
                    else
                    {
                        wait_count++;
                        if(wait_count > 0x10U)
                        {
                            if( hls->in_wfi_indicator == HLS_OTHER_HART_IN_WFI )
                            {
                                hls->my_hart_id = hart_id; /* record hartid locally */
                                raise_soft_interrupt(hart_id);
                                wait_count = 0UL;
                            }
                        }
                    }
                    break;
            }
        }
        stack_top = (ptrdiff_t)((uint8_t*)&__stack_top_h1$);
        hls = (HLS_DATA*)(stack_top - HLS_DEBUG_AREA_SIZE);
        hls->in_wfi_indicator = HLS_MAIN_HART_FIN_INIT;
        (void)main_other_hart(hls);
    }

    __builtin_unreachable();
    /* should never get here */
    while(true)
    {
       static volatile uint64_t counter = 0U;
       /* Added some code as debugger hangs if in loop doing nothing */
       counter = counter + 1U;
    }

    return (0);
}
#endif

/*==============================================================================
 * U54s startup.
 * This is called from entry.S
 * If you need to modify this function, create your own one in a user directory
 * space.
 *
 * Please note: harts MPFS_HAL_FIRST_FIRST + 1 to MPFS_HAL_FIRST_LAST will wait
 * in startup code in entry.S as they run the wfi (wait for interrupt)
 * instruction.
 * They are woken up as required by the the MPFS_HAL_FIRST_HART, in the function
 * main_first_hart().
 * ( It triggers a software interrupt on the particular hart to be woken up )
 */
__attribute__((weak)) int main_other_hart(HLS_DATA* hls)
{
#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    extern char __app_stack_top_h0;
    extern char __app_stack_top_h1;
    extern char __app_stack_top_h2;
    extern char __app_stack_top_h3;
    extern char __app_stack_top_h4;

    const uint64_t app_stack_top_h0 = (const uint64_t)&__app_stack_top_h0 - (HLS_DEBUG_AREA_SIZE);
    const uint64_t app_stack_top_h1 = (const uint64_t)&__app_stack_top_h1 - (HLS_DEBUG_AREA_SIZE);
    const uint64_t app_stack_top_h2 = (const uint64_t)&__app_stack_top_h2 - (HLS_DEBUG_AREA_SIZE);
    const uint64_t app_stack_top_h3 = (const uint64_t)&__app_stack_top_h3 - (HLS_DEBUG_AREA_SIZE);
    const uint64_t app_stack_top_h4 = (const uint64_t)&__app_stack_top_h4 - (HLS_DEBUG_AREA_SIZE);

#ifdef TURN_OFF_POWER_TO_PARKED_HARTS
    turn_off_power_to_parked_harts_ram();
#endif

#ifdef  MPFS_HAL_HW_CONFIG
    turn_on_fpu((uint32_t)LIBERO_SETTING_TURN_ON_FPU);
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    /*
     * If we are a boot-loader, and shared memory enabled (MPFS_HAL_SHARED_MEM_ENABLED)
     * sets the pointer in each harts HLS to the shared memory.
     * This allows access to this shared memory across all harts.
     */
    const uint64_t app_hart_common_start = (const uint64_t)&__app_hart_common_start;
    hls->shared_mem = (uint64_t *)app_hart_common_start;
    hls->shared_mem_marker = SHARED_MEM_INITALISED_MARKER;
    hls->shared_mem_status = SHARED_MEM_DEFAULT_STATUS;
#endif
#else
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    /* make sure each harts Harts Local Storage has pointer to common memory if enabled */
    /* store the value here received from boot-loader */
    /* a1 will contain pointer to the start of shared memory */
    //hls->shared_mem = (uint64_t *)__uninit_bottom$;
#else
    /*
     * We are not using shared memory
     */
    hls->shared_mem = (uint64_t *)NULL;
#endif
#endif

    volatile uint64_t dummy;

    switch(hls->my_hart_id)
    {

    case 0U:
        __asm volatile ("add sp, x0, %1" : "=r"(dummy) : "r"(app_stack_top_h0));
        e51();
        break;

    case 1U:
        (void)init_pmp((uint8_t)1);
        __asm volatile ("add sp, x0, %1" : "=r"(dummy) : "r"(app_stack_top_h1));
        u54_1();
        break;

    case 2U:
        (void)init_pmp((uint8_t)2);
        __asm volatile ("add sp, x0, %1" : "=r"(dummy) : "r"(app_stack_top_h2));
        u54_2();
        break;

    case 3U:
        (void)init_pmp((uint8_t)3);
        __asm volatile ("add sp, x0, %1" : "=r"(dummy) : "r"(app_stack_top_h3));
        u54_3();
        break;

    case 4U:
        (void)init_pmp((uint8_t)4);
        __asm volatile ("add sp, x0, %1" : "=r"(dummy) : "r"(app_stack_top_h4));
        u54_4();
        break;

    default:
        /* no more harts */
        break;
    }

    /* should never get here */
    while(true)
    {
       static volatile uint64_t counter = 0U;
       /* Added some code as debugger hangs if in loop doing nothing */
       counter = counter + 1U;
    }
#else /* (IMAGE_LOADED_BY_BOOTLOADER == 0) */


    uint64_t hartid = read_csr(mhartid);

    switch(hartid)
    {

    case 0U:
        e51();
        break;

    case 1U:
        u54_1();
        break;

    case 2U:
        u54_2();
        break;

    case 3U:
        u54_3();
        break;

    case 4U:
        u54_4();
        break;

    default:
        /* no more harts */
        break;
    }
#endif

  return (0);

}

/*==============================================================================
 * Load the virtual ROM located at address 0x20003120 within the SCB system
 * registers with an executable allowing to park a hart in an infinite loop.
 */
#ifdef  MPFS_HAL_HW_CONFIG
#define VIRTUAL_BOOTROM_BASE_ADDR   0x20003120UL
#define NB_BOOT_ROM_WORDS           8U
const uint32_t rom[NB_BOOT_ROM_WORDS] =
{
    0x00000513U,    /* li a0, 0 */
    0x34451073U,    /* csrw mip, a0 */
    0x10500073U,    /* wfi */
    0xFF5FF06FU,    /* j 0x20003120 */
    0xFF1FF06FU,    /* j 0x20003120 */
    0xFEDFF06FU,    /* j 0x20003120 */
    0xFE9FF06FU,    /* j 0x20003120 */
    0xFE5FF06FU     /* j 0x20003120 */
};

void load_virtual_rom(void)
{
    volatile uint32_t * p_virtual_bootrom = (uint32_t *)VIRTUAL_BOOTROM_BASE_ADDR;
    config_copy( (void *)p_virtual_bootrom, (void *)rom,sizeof(rom[NB_BOOT_ROM_WORDS]));
}
#endif  /* MPFS_HAL_HW_CONFIG */

/*==============================================================================
 * Initialize the global constructor before using any C++ feature which depends
 * on global constructors.
 */
static void init_global_constructors(void)
{
    void (**fun_ptr)(void) = (void (**)(void))&__init_array_start;
    while (fun_ptr != (void (**)(void))&__init_array_end)
    {
        (*fun_ptr)();
        fun_ptr++;
     }
}

/*==============================================================================
 * Put the hart executing this code into an infinite loop executing from the
 * SCB system register memory space.
 * This allows preventing a hart from accessing memory regardless of memory
 * hierarchy configuration or compiler/linker settings.
 * This function relies on load_virtual_rom() having been called previously to
 * populate the virtual ROM with a suitable executable.
 */
static void park_hart(void)
{
    clear_csr(mstatus, MSTATUS_MIE);
    __asm volatile("fence.i");
    __asm volatile("li ra,0x20003120");
    __asm volatile("ret");
}

/*==============================================================================
 * E51 code executing after system startup.
 * In absence of an application function of this name with strong linkage, this
 * function will get linked.
 * This default implementation is for illustration purpose only. If you need to
 * modify this function, create your own one in an application directory space.
 */
__attribute__((weak)) void e51(void)
{
    parked_harts |= (1U << 0U);
    /* Put hart in safe infinite WFI loop. */
     park_hart();
}

/*==============================================================================
 * First U54.
 * In absence of an application function of this name with strong linkage, this
 * function will get linked.
 * This default implementation is for illustration purpose only. If you need to
 * modify this function, create your own one in an application directory space.
  */
__attribute__((weak)) void u54_1(void)
{
    parked_harts |= (1U << 1U);
    /* Put hart in safe infinite WFI loop. */
     park_hart();
}


/*==============================================================================
 * Second U54.
 * In absence of an application function of this name with strong linkage, this
 * function will get linked.
 * This default implementation is for illustration purpose only. If you need to
 * modify this function, create your own one in an application directory space.
 */
__attribute__((weak)) void u54_2(void)
{
    parked_harts |= (1U << 2U);
    /* Put hart in safe infinite WFI loop. */
    park_hart();
}

/*==============================================================================
 * Third U54.
 * In absence of an application function of this name with strong linkage, this
 * function will get linked.
 * This default implementation is for illustration purpose only. If you need to
 * modify this function, create your own one in an application directory space.
 */
__attribute__((weak)) void u54_3(void)
{
    parked_harts |= (1U << 3U);
    /* Put hart in safe infinite WFI loop. */
     park_hart();
}

/*==============================================================================
 * Fourth U54.
 * In absence of an application function of this name with strong linkage, this
 * function will get linked.
 * This default implementation is for illustration purpose only. If you need to
 * modify this function, create your own one in an application directory space.
 */
__attribute__((weak)) void u54_4(void)
{
    parked_harts |= (1U << 4U);
    /* Put hart in safe infinite WFI loop. */
    park_hart();
}

 /*-----------------------------------------------------------------------------
  * _start() function called invoked
  * This function is called on power up and warm reset.
  */
 __attribute__((weak)) void init_memory( void)
 {
    copy_section(&__text_load, &__text_start, &__text_end);
    copy_section(&__sdata_load, &__sdata_start, &__sdata_end);
    copy_section(&__data_load, &__data_start, &__data_end);

    zero_section(&__sbss_start, &__sbss_end);
    zero_section(&__bss_start, &__bss_end);

    __disable_all_irqs();      /* disables local and global interrupt enable */
 }

 /*-----------------------------------------------------------------------------
   * _start() function called invoked
   * This function is called on power up and warm reset.
   */
  __attribute__((weak)) void init_ddr(void)
  {
    if ((LIBERO_SETTING_DDRPHY_MODE & DDRPHY_MODE_MASK) != DDR_OFF_MODE) {
#ifdef DDR_SUPPORT
        uint64_t end_address;

#if 0 /* enable to init cache to zero using 64 bit writes */
        end_address = LIBERO_SETTING_DDR_64_NON_CACHE + LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_0 + LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_1;
        zero_section((uint64_t *)LIBERO_SETTING_DDR_64_NON_CACHE, (uint64_t *)end_address);
#endif

        end_address = LIBERO_SETTING_DDR_64_CACHE + LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI1_0 + LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI1_1;
        zero_section((uint64_t *)LIBERO_SETTING_DDR_64_CACHE, (uint64_t *)end_address);
#endif
    }
  }

/**
 * init_mem_protection_unit(void)
 * add this function to you code and configure as required
 * @return
 */
__attribute__((weak)) uint8_t init_mem_protection_unit(void)
{
    mpu_configure();
    return (0U);
}

/**
 * init_pmp(void)
 * add this function to you code and configure as required
 * @return
 */
__attribute__((weak)) uint8_t init_pmp(uint8_t hart_id)
{
    pmp_configure(hart_id);
    return (0U);
}

/**
 * turn_off_power_to_parked_harts(void)
 *
 * Turns off power to harts that have been parked.
 *
 */
__attribute__((weak)) void turn_off_power_to_parked_harts_ram(void)
{
    uint32_t hart_id = MPFS_HAL_FIRST_HART;

    while( hart_id <= MPFS_HAL_LAST_HART)
    {
        if ((parked_harts & (1U << hart_id)) !=0)
        {
            SYSREG->RAM_SHUTDOWN_CR |=  ((1U << 8U)<< hart_id);
        }
        hart_id++;
    }
}

/**
 * turn_on_power_to_hart_ram(void)
 *
 * Turns on power to a hart that was previously turned off
 *
 */
__attribute__((weak)) void turn_on_power_to_hart_ram(uint32_t hart_id)
{
    SYSREG->RAM_SHUTDOWN_CR &=  ~((1U << 8U)<< hart_id);
}

