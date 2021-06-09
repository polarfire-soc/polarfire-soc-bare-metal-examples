#include <stdint.h>
#include "mpfs_hal/mss_hal.h"

#if __has_include("coremark.h")
#include "coremark.h"
#else
#include "add_coremark.h"
#endif


volatile uint64_t dummy_h1 = 0;

#ifndef REQUIRES_COREMARK_FILES
extern MAIN_RETURN_TYPE main(void);
#endif

#ifdef TEST_CORE_E51
void e51(void) {
    volatile int i;
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 100000;
    uint32_t hartid = read_csr(mhartid);

    //write_csr(0x7C0, 0x0);

    main();

    while(1) {

        mcycle_start = readmcycle();

        for(i = 0; i < num_loops; i++) {
            dummy_h1 = i;
        }

        mcycle_end = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;

    }
    // never return
};
#endif

#ifdef TEST_CORE_U54_1
void u54_1(void) {
    volatile int i;
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 100000;
    uint32_t hartid = read_csr(mhartid);

    write_csr(0x7C0, 0x0);

#ifndef REQUIRES_COREMARK_FILES
    main();
#endif

    while(1) {

        mcycle_start = readmcycle();

        for(i = 0; i < num_loops; i++) {
            dummy_h1 = i;
        }

        mcycle_end = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;

    }
    // never return
};
#endif

#ifdef TEST_CORE_U54_2
void u54_2(void) {
    volatile int i;
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 100000;
    uint32_t hartid = read_csr(mhartid);

    write_csr(0x7C0, 0x0);

    main();

    while(1) {

        mcycle_start = readmcycle();

        for(i = 0; i < num_loops; i++) {
            dummy_h1 = i;
        }

        mcycle_end = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;

    }
    // never return
};
#endif

#ifdef TEST_CORE_U54_3
void u54_3(void) {
    volatile int i;
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 100000;
    uint32_t hartid = read_csr(mhartid);

    write_csr(0x7C0, 0x0);

    main();

    while(1) {

        mcycle_start = readmcycle();

        for(i = 0; i < num_loops; i++) {
            dummy_h1 = i;
        }

        mcycle_end = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;

    }
    // never return
};
#endif

#ifdef TEST_CORE_U54_4
void u54_4(void) {
    volatile int i;
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 100000;
    uint32_t hartid = read_csr(mhartid);
  
    write_csr(0x7C0, 0x0);

    main();

    while(1) {

        mcycle_start = readmcycle();

        for(i = 0; i < num_loops; i++) {
            dummy_h1 = i;
        }

        mcycle_end = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;

    }
    // never return
};
#endif


int main_other_hart(void)
{
    extern char __app_stack_top_h0;
    extern char __app_stack_top_h1;
    extern char __app_stack_top_h2;
    extern char __app_stack_top_h3;
    extern char __app_stack_top_h4;
#ifndef COREMARK_RUN_FROM_DDR
    extern char __cm_lim_stack_top;
    extern char __cm_scratch_stack_top;
    extern char __cm_dtm_stack_top;
#endif

    const uint64_t app_stack_top_h0 = (const uint64_t)&__app_stack_top_h0;

#ifndef TEST_CORE_U54_1
    const uint64_t app_stack_top_h1 = (const uint64_t)&__app_stack_top_h1;
#else
#ifdef COREMARK_RUN_FROM_LIM
    const uint64_t app_stack_top_h1 = (const uint64_t)&__cm_lim_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_SCRATCHPAD
    const uint64_t app_stack_top_h1 = (const uint64_t)&__cm_scratch_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_DDR
    const uint64_t app_stack_top_h1 = (const uint64_t)&__app_stack_top_h1;
#endif
#endif

#ifndef TEST_CORE_U54_2
    const uint64_t app_stack_top_h2 = (const uint64_t)&__app_stack_top_h2;
#else
#ifdef COREMARK_RUN_FROM_LIM
    const uint64_t app_stack_top_h2 = (const uint64_t)&__cm_lim_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_SCRATCHPAD

    const uint64_t app_stack_top_h2 = (const uint64_t)&__cm_scratch_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_DDR
    const uint64_t app_stack_top_h2 = (const uint64_t)&__app_stack_top_h2;
#endif
#endif

#ifndef TEST_CORE_U54_3
    const uint64_t app_stack_top_h3 = (const uint64_t)&__app_stack_top_h3;
#else
#ifdef COREMARK_RUN_FROM_LIM
    const uint64_t app_stack_top_h3 = (const uint64_t)&__cm_lim_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_SCRATCHPAD
    const uint64_t app_stack_top_h3 = (const uint64_t)&__cm_scratch_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_DDR
    const uint64_t app_stack_top_h3 = (const uint64_t)&__app_stack_top_h3;
#endif
#endif

#ifndef TEST_CORE_U54_4
    const uint64_t app_stack_top_h4 = (const uint64_t)&__app_stack_top_h4;
#else
#ifdef COREMARK_RUN_FROM_LIM
    const uint64_t app_stack_top_h4 = (const uint64_t)&__cm_lim_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_SCRATCHPAD
    const uint64_t app_stack_top_h4 = (const uint64_t)&__cm_scratch_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_ITIM
    const uint64_t app_stack_top_h4 = (const uint64_t)0x01822000; /* 8k itim */
#endif
#ifdef COREMARK_RUN_FROM_DTIM
    const uint64_t app_stack_top_h4 = (const uint64_t)&__cm_dtm_stack_top;
#endif
#ifdef COREMARK_RUN_FROM_DDR
    const uint64_t app_stack_top_h4 = (const uint64_t)&__app_stack_top_h4;
#endif
#endif

    uint64_t hartid = read_csr(mhartid);

    volatile uint64_t dummy;

    switch(hartid)
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

    return (0);

}




