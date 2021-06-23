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






