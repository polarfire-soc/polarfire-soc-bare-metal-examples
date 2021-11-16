/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file hw_nvm_map.h
 * @author Microchip-FPGA Embedded Systems Solutions
 *
 *
 * Note 1: This file should not be edited. If you need to modify a parameter
 * without going through regenerating using the MSS Configurator Libero flow 
 * or editing the associated xml file
 * the following method is recommended: 

 * 1. edit the following file 
 * boards/your_board/platform_config/mpfs_hal_config/mss_sw_config.h

 * 2. define the value you want to override there.
 * (Note: There is a commented example in the platform directory)

 * Note 2: The definition in mss_sw_config.h takes precedence, as
 * mss_sw_config.h is included prior to the generated header files located in
 * boards/your_board/fpga_design_config
 *
 */

#ifndef HW_NVM_MAP_H_
#define HW_NVM_MAP_H_


#ifdef __cplusplus
extern  "C" {
#endif

#if !defined (LIBERO_SETTING_SNVM_MSS_START_PAGE)
/*Page offset to start page of sNVM available to MSS. Each SNVM module may be
stored in any of the following formats, Non-authenticated plaintext,
Authenticated plaintext, Authenticated ciphertext. When the data is
authenticated 236 bytes of storage per page is available. When the data is not
authenticated 252 bytes may be stored. (Note: Value in decimal) */
#define LIBERO_SETTING_SNVM_MSS_START_PAGE    0
    /* START_PAGE_OFFSET                 [0:32]  RW value= 0 */
#endif
#if !defined (LIBERO_SETTING_SNVM_MSS_END_PAGE)
/*Page offset to end page of sNVM available to MSS (Note: Value in decimal) */
#define LIBERO_SETTING_SNVM_MSS_END_PAGE    220
    /* END_PAGE_OFFSET                   [0:32]  RW value= 220 */
#endif
#if !defined (LIBERO_SETTING_ENVM_MSS_START_PAGE)
/*Page offset to start page of sNVM available to MSS (Note: Value in decimal)
*/
#define LIBERO_SETTING_ENVM_MSS_START_PAGE    0
    /* START_PAGE_OFFSET                 [0:32]  RW value= 0 */
#endif
#if !defined (LIBERO_SETTING_ENVM_MSS_END_PAGE)
/*Page offset to end page of sNVM available to MSS (Note: Value in decimal) */
#define LIBERO_SETTING_ENVM_MSS_END_PAGE    511
    /* END_PAGE_OFFSET                   [0:32]  RW value= 511 */
#endif

#ifdef __cplusplus
}
#endif


#endif /* #ifdef HW_NVM_MAP_H_ */

