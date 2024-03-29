/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software
 *
 */
/*******************************************************************************
 * 
 * file name : mpfs-lim-lma-scratchpad-vma.ld
 * Code starts from LIM and relocates itself to an L2 cache scratchpad mapped in
 * the Zero Device address range.
 * Used when debugging start-up code.
 */
 
OUTPUT_ARCH( "riscv" )
ENTRY(_start)


/*-----------------------------------------------------------------------------

  -- PolarFire SoC Memorp map


     0x0800_0000  +-----------------------+
                  |   DTIM  Len=8K        |
                  +-----------------------+

     0x0180_0000  +-----------------------+
                  |   ITIM Hart 0         |
                  +-----------------------+
                                                        eNVM detail
     0x0180_8000  +-----------------------+        +-------------------+   -
                  |   ITIM Hart 1 Len=28K |        |Sector 2           |   ^
                  +-----------------------+        |0x2022_0000 8K     |   |
                                                   +-------------------+   |
     0x0181_0000  +-----------------------+        |Sector 0           |   1
                  |   ITIM Hart 2 Len=28K |        |0x2022_2000 56K    |   2
                  +-----------------------+        +-------------------+   8
                                                   |Sector 1           |   k
     0x0181_8000  +-----------------------+        |0x2022_3000 56K    |   |
                  |   ITIM Hart 3 Len=28K |        +-------------------+   |
                  +-----------------------+        |Sector 3           |   |
                                                   |0x2022_3E00 8K     |   v
     0x0182_0000  +-----------------------+        +-------------------+   -
                  |   ITIM Hart 4 Len=28K |
                  +-----------------------+

     0x0800_0000  +-----------------------+
                  |   LIM  Len max=1920K  |  +------>+------+
                  +-----------------------+  |       |      |
                                             |       |      |
     0x2022_0000  +-----------------------+  |       |      |
                  |   eNVM 128K           |  |       |      |
                  +-----------------------+  |       |      |
                                             |       |      |
     0x8000_0000  +-----------------------+--+       |  DDR |
  0x10_0000_0000  | DDR cached            |          |      |
                  |   SEG0                |     +--->|      |
                  +-----------------------+     |    |      |
                                                |    |      |
     0xC000_0000  +-----------------------+-----+    |      |
  0x14_0000_0000  | DDR non-cached        |          |      |
                  |    SEG1               |          |      |
                  +-----------------------+      +-->+      |
                                                 |   |      |
     0xD000_0000  +-----------------------+------+   |      |
  0x18_0000_0000  | Write Combine buffer  |          +------+
                  |    SEG1               |
                  +-----------------------+
  ----------------------------------------------------------------------------*/
  
/*-----------------------------------------------------------------------------

-- MSS hart Reset vector

The MSS reset vector for each hart is configured using the PolarFire SoC MSS
Configurator and stored in the device's secure NVM.
The most common usage will be where the reset vector for each hart will be set
to the start of the eNVM at address 0x2022_0000, giving 128K of contiguous
non-volatile storage. Normally this is where the initial boot-loader will 
reside.
The MSS Configurator outputs the configured reset vector address to an xml file,
see LIBERO_SETTING_RESET_VECTOR_HART0 etc in <hw_memory.h>

When debugging a bare metal program that is run out of reset from eNVM, a linker 
script will be used whereby the program will run from LIM instead of eNVM.
In this case, set the reset vector in the linker script to 0x0800_0000.
This means you are not continually programming the eNVM each time you load a 
program and there is no limitation with hardware break points when debugging.
See mpfs-lim.ld example linker script when runing from LIM.

------------------------------------------------------------------------------*/

MEMORY
{
    eNVM_SEC_2_0_1_3 (rx) : ORIGIN = 0x20220000, LENGTH = 128k
    l2_lim (rwx) :          ORIGIN = 0x08000000, LENGTH = 256k
    scratchpad(rwx):        ORIGIN = 0x0A000000, LENGTH = 256k
    ram_DTIM (rwx) :        ORIGIN = 0x01000000, LENGTH = 7k
    /* This 1k of DTIM is used to run code when switching the eNVM clock */
    switch_code (rx) :      ORIGIN = 0x01001c00, LENGTH = 1k     
}

HEAP_SIZE           = 8k;               /* needs to be calculated for your application if using */

/* STACK_SIZE_PER_HART needs to be calculated for your */
/* application. Must be aligned */
/* Also Thread local storage (AKA hart local storage) allocated for each hart */
/* as part of the stack
/* So memory map will look like once apportion in startup code: */
/*   */
/* stack hart0  Actual Stack size = (STACK_SIZE_PER_HART - HLS_DEBUG_AREA_SIZE) */
/* TLS hart 0   */
/* stack hart1  */
/* TLS hart 1   */
/* etc */
/* note: HLS_DEBUG_AREA_SIZE is defined in mss_sw_config.h */

/*
 * Stack size for each hart's application.
 * These are the stack sizes that will be allocated to each hart before starting
 * each hart's application function, e51(), u54_1(), u54_2(), u54_3(), u54_4().
 */
STACK_SIZE_E51_APPLICATION = 4k;
STACK_SIZE_U54_1_APPLICATION = 4k;
STACK_SIZE_U54_2_APPLICATION = 4k;
STACK_SIZE_U54_3_APPLICATION = 4k;
STACK_SIZE_U54_4_APPLICATION = 4k;


SECTIONS
{
  . = 0x08000000;
  .text_init : ALIGN(0x10)
  {
     *(.text.init)
     *system_startup.o (.text .text* .rodata .rodata* .srodata*)
     *mtrap.o (.text .text* .rodata .rodata* .srodata*)
     *mss_h2f.o (.text .text* .rodata .rodata* .srodata*)
     *mss_l2_cache.o (.text .text* .rodata .rodata* .srodata*)
     *mss_mpu.o (.text .text* .rodata .rodata* .srodata*)
     *mss_pmp.o (.text .text* .rodata .rodata* .srodata*)
     *mss_mutex.o (.text .text* .rodata .rodata* .srodata*)
     *mss_stubs.o (.text .text* .rodata .rodata* .srodata*)
     *mss_util.o (.text .text* .rodata .rodata* .srodata*)
     *mss_nwc_init.o (.text .text* .rodata .rodata* .srodata*)
     *mss_pll.o (.text .text* .rodata .rodata* .srodata*)
     *mss_ddr.o (.text .text* .rodata .rodata* .srodata*)
     *mss_io.o (.text .text* .rodata .rodata* .srodata*)
     *mss_sgmii.o (.text .text* .rodata .rodata* .srodata*)
     *memcpy.o (.text .text* .rodata .rodata* .srodata*)
     *memset.o (.text .text* .rodata .rodata* .srodata*)
     *u54_1.o (.text .text* .rodata .rodata* .srodata*) 
    . = ALIGN(0x10);
  } >l2_lim 
  
  .text : ALIGN(0x10)
  {
    __text_load = LOADADDR(.text);
    . = ALIGN(0x10);
    __text_start = .;
    __l2_scratchpad_vma_start = .; 
    *(.text .text.* .gnu.linkonce.t.*)
    *(.plt)
    . = ALIGN(0x10);
    
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))
    
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
    *(.gcc_except_table) 
    *(.eh_frame_hdr)
    *(.eh_frame)
    
    KEEP (*(.init))
    KEEP (*(.fini))

    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
    
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
        
    . = ALIGN(0x10);
    __text_end = .;
  } >scratchpad AT> l2_lim
 
   
  .l2_scratchpad : ALIGN(0x10)
  { 
    __l2_scratchpad_load = LOADADDR(.l2_scratchpad);
    __l2_scratchpad_start = .; 
    *(.l2_scratchpad)
    . = ALIGN(0x10);
    __l2_scratchpad_end = .;
  } >scratchpad AT> l2_lim
  
  /* short/global data section */
  .sdata : ALIGN(0x10)
  {
    __sdata_load = LOADADDR(.sdata);
    __sdata_start = .; 
    /* offset used with gp(gloabl pointer) are +/- 12 bits, so set point to middle of expected sdata range */
    /* If sdata more than 4K, linker used direct addressing. Perhaps we should add check/warning to linker script if sdata is > 4k */
    __global_pointer$ = . + 0x800;
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    . = ALIGN(0x10);
    __sdata_end = .;
  } >scratchpad AT> l2_lim
  
  /* data section */
  .data : ALIGN(0x10)
  { 
    __data_load = LOADADDR(.data);
    __data_start = .; 
    *(.got.plt) *(.got)
    *(.shdata)
    *(.data .data.* .gnu.linkonce.d.*)
    . = ALIGN(0x10);
    __data_end = .;
  } > scratchpad AT> l2_lim

  /* sbss section */
  .sbss : ALIGN(0x10)
  {
    __sbss_start = .;
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    . = ALIGN(0x10);
    __sbss_end = .;
  } > scratchpad
  
  /* sbss section */
  .bss : ALIGN(0x10)
  { 
    __bss_start = .;
    *(.shbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(0x10);
    __bss_end = .;
  } > scratchpad

  /* End of uninitialized data segment */
  _end = .;
  
  .heap : ALIGN(0x10)
  {
    __heap_start = .;
    . += HEAP_SIZE;
    __heap_end = .;
    . = ALIGN(0x10);
    _heap_end = __heap_end;
    __l2_scratchpad_vma_end = .;
  } > scratchpad
  
    /* must be on 4k boundary- corresponds to page size */
    .stack_e51 : /* ALIGN(0x1000) */
    {
        PROVIDE(__stack_bottom_h0$ = .);
        PROVIDE(__app_stack_bottom_h0 = .);
        . += STACK_SIZE_E51_APPLICATION;
        PROVIDE(__app_stack_top_h0 = .);
        PROVIDE(__stack_top_h0$ = .);
    } > l2_lim
  
    /* must be on 4k boundary- corresponds to page size */
    .stack_u54_1 : /* ALIGN(0x1000) */
    {
        PROVIDE(__stack_bottom_h1$ = .);
        PROVIDE(__app_stack_bottom_h1$ = .);
        . += STACK_SIZE_U54_1_APPLICATION;
        PROVIDE(__app_stack_top_h1 = .);
        PROVIDE(__stack_top_h1$ = .);
    } > l2_lim
  
  
    /* must be on 4k boundary- corresponds to page size */
    .stack_u54_2 : /* ALIGN(0x1000) */
    {
        PROVIDE(__stack_bottom_h2$ = .);
        . += STACK_SIZE_U54_2_APPLICATION;
        PROVIDE(__stack_top_h2$ = .);
    } > l2_lim
    
    /*  */
    .stack_app_u54_2 : /* ALIGN(0x1000) */
    {
        PROVIDE(__app_stack_bottom_h2 = .);
        . += STACK_SIZE_U54_2_APPLICATION;
        PROVIDE(__app_stack_top_h2 = .);
    } > scratchpad
  
    /*  */
    .stack_u54_3 : /* ALIGN(0x1000) */
    {
        PROVIDE(__stack_bottom_h3$ = .);
        PROVIDE(__app_stack_bottom_h3 = .);
        . += STACK_SIZE_U54_3_APPLICATION;
        PROVIDE(__app_stack_top_h3 = .);
        PROVIDE(__stack_top_h3$ = .);
    } > l2_lim
    
    /*  */
    .stack_u54_4 : /* ALIGN(0x1000) */
    {
        PROVIDE(__stack_bottom_h4$ = .);
        PROVIDE(__app_stack_bottom_h4 = .);
        . += STACK_SIZE_U54_4_APPLICATION;
        PROVIDE(__app_stack_top_h4 = .);
        PROVIDE(__stack_top_h4$ = .);
    } > l2_lim
    
    /*  */
    .stack_coremark_lim : /* ALIGN(0x1000) */
    {
        PROVIDE(__cm_lim_stack_bottom = .);
        . += STACK_SIZE_U54_4_APPLICATION;
        PROVIDE(__cm_lim_stack_top = .);
    } > l2_lim
    
    /*  */
    .stack_coremark_scratchpad : /* ALIGN(0x1000) */
    {
        PROVIDE(__cm_scratch_stack_bottom = .);
        . += STACK_SIZE_U54_4_APPLICATION;
        PROVIDE(__cm_scratch_stack_top = .);
    } > scratchpad
    
    /*  */
    .stack_coremark_dtm : /* ALIGN(0x1000) */
    {
        PROVIDE(__cm_dtm_stack_bottom = .);
        . += STACK_SIZE_U54_4_APPLICATION;
        PROVIDE(__cm_dtm_stack_top = .);
    } > ram_DTIM

  /* 
   *   The .ram_code section will contain the code That is run from RAM.
   *   We are using this code to switch the clocks including eNVM clock.
   *   This can not be done when running from eNVM
   *   This will need to be copied to ram, before any of this code is run.
   */
  .ram_code : ALIGN(0x10)
  {
    . = ALIGN (4);
    __sc_load = LOADADDR (.ram_code);
    __sc_start = .;
    *(.ram_codetext)        /* .ram_codetext sections (code) */
    *(.ram_codetext*)       /* .ram_codetext* sections (code)  */
    *(.ram_coderodata)      /* read-only data (constants) */
    *(.ram_coderodata*)
    . = ALIGN (4);
    __sc_end = .;
    /* place __start_of_free_lim$ after last allocation of l2_lim */
    PROVIDE(__start_of_free_lim$ = .);
  } >switch_code AT> l2_lim     /* On the MPFS for startup code use,  >switch_code AT>eNVM */
  
}

 
