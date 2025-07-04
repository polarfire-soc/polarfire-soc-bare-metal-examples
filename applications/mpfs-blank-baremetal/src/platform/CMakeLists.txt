cmake_minimum_required(VERSION 3.27.1)

set(SYS_PROC rv64imafdc_zicsr)
set(MARCH rv64imafdc_zicsr_zifencei)
set(MABI lp64d)

set(linker_script_path ${CMAKE_CURRENT_LIST_DIR}/platform_config_reference/linker/sdk/)
if(${CONFIG_MEMORY} MATCHES "envm")
    set(linker_script ${linker_script_path}/mpfs-envm.lds)
elseif(${CONFIG_MEMORY} MATCHES "scratchpad")
    set(linker_script ${linker_script_path}/mpfs-lim-lma-scratchpad-vma.lds)
elseif(${CONFIG_MEMORY} MATCHES "lim")
    set(linker_script ${linker_script_path}/mpfs-lim.lds)
elseif(${CONFIG_MEMORY} MATCHES "ddr")
    set(linker_script ${linker_script_path}/mpfs-ddr-loaded-by-boot-loader.lds)
else()
    set(linker_script ${linker_script_path}/mpfs-lim.lds)
endif()

execute_process(
    COMMAND ${PYTHON_EXECUTABLE} 
    ${CMAKE_CURRENT_LIST_DIR}/soc_config_generator/mpfs_configuration_generator.py
    ${CMAKE_BINARY_DIR}/board/fpga_design/design_description/ 
    ${CMAKE_BINARY_DIR}/board
)

list(APPEND PLATFORM_HEADERS
    ${CMAKE_SOURCE_DIR}/src/application/
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/
    ${CMAKE_BINARY_DIR}/board/
    ${CMAKE_BINARY_DIR}/board/fpga_design_config/
    ${CMAKE_BINARY_DIR}/board/platform_config/
    ${CMAKE_CURRENT_LIST_DIR}/platform_config_reference/
    ${CMAKE_BINARY_DIR}
)

if(${CONFIG_MEMORY} MATCHES "ddr")
    list(APPEND LIB_MSS_HEADERS
        ${CMAKE_BINARY_DIR}/board/platform_config_ddr/
        ${CMAKE_BINARY_DIR}/board/platform_config_ddr/mpfs_hal_config/    
    )
else()
    list(APPEND LIB_MSS_HEADERS
        ${CMAKE_BINARY_DIR}/board/platform_config/
        ${CMAKE_BINARY_DIR}/board/platform_config/mpfs_hal_config/    
    )
endif()

list(APPEND MSS_DRIVERS)

if(${CONFIG_MSS_CAN})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_can/mss_can.c
    )
endif()

if(${CONFIG_MSS_ETHERNET_MAC})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/mss_ethernet_mac.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/null_phy.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/ti_dp83867_phy.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/vsc8541_phy.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/vsc8575_phy.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/vsc8575_support.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_ethernet_mac/vsc8662_phy.c
    )
endif()

if(${CONFIG_MSS_GPIO})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_gpio/mss_gpio.c
    )
endif()
    
if(${CONFIG_MSS_I2C})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_i2c/mss_i2c.c
    )
endif()
    
if(${CONFIG_MSS_MMC})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_mmc/mss_mmc_if.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_mmc/mss_mmc.c
    )
endif()
    
if(${CONFIG_MSS_MMUART})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_mmuart/mss_uart.c
    )
endif()

if(${CONFIG_MSS_PDMA})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_pdma/mss_pdma.c
    )
endif()
    
if(${CONFIG_MSS_QSPI})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_qspi/mss_qspi.c
    )
endif()
    
if(${CONFIG_MSS_RTC})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_rtc/mss_rtc.c
    )
endif()

if(${CONFIG_MSS_SPI})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_spi/mss_spi.c
    )
endif()

if(${CONFIG_MSS_SYS_SERVICES})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_sys_services/mss_sys_services.c
    )
endif()

if(${CONFIG_MSS_USB})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_common_cif.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_device_cif.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_device_hid.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_device_msd.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_device_vc.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_device.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_host_cif.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_host_hid.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_host_msc.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_usb/mss_usb_host.c
    )
endif()

if(${CONFIG_MSS_WATCHDOG})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/mss_watchdog/mss_watchdog.c
    )
endif()

if(${CONFIG_PF_PCIE})
    list(APPEND MSS_DRIVERS
        ${CMAKE_CURRENT_LIST_DIR}/drivers/mss/pf_pcie/pf_pcie.c
    )
endif()

list(APPEND MSS_HAL
    ${CMAKE_CURRENT_LIST_DIR}/hal/hal_irq.c
    ${CMAKE_CURRENT_LIST_DIR}/hal/hw_reg_access.S
)

list(APPEND MSS_MPFS_HAL
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_cfm.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_ddr.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_ddr_debug.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_ddr_test_pattern.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_io.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_nwc_init.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_pll.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/nwc/mss_sgmii.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_axiswitch.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_beu.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_clint.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_h2f.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_irq_handler_stubs.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_l2_cache.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_mpu.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_mtrap.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_peripherals.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_plic.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_pmp.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/common/mss_util.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/startup_gcc/newlib_stubs.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/startup_gcc/system_startup.c
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/startup_gcc/mss_entry.S
    ${CMAKE_CURRENT_LIST_DIR}/mpfs_hal/startup_gcc/mss_utils.S
)

list(APPEND PLATFORM_SRC
    ${MSS_DRIVERS}
    ${MSS_HAL}
    ${MSS_MPFS_HAL}
)
