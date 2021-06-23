# Creating SmartDesign LPDDR4_TOP
set sd_name {LPDDR4_TOP}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {RESET_N} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {ODT} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {CKE} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {CS} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {CK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {CK_N} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {MMUART_0_RXD_F2M} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {MSSUART0_TX} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {MSS_RESET_N_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {PLL_CPU_LOCK_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {PLL_DDR_LOCK_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {REFCLK} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {REFCLK_N} -port_direction {IN}

sd_create_bus_port -sd_name ${sd_name} -port_name {CA} -port_direction {OUT} -port_range {[5:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {DQ} -port_direction {INOUT} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {DQS} -port_direction {INOUT} -port_range {[3:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {DQS_N} -port_direction {INOUT} -port_range {[3:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {DM} -port_direction {OUT} -port_range {[3:0]}

# Add PF_CCC_C0_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {PF_CCC_C0} -instance_name {PF_CCC_C0_0}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PF_CCC_C0_0:PLL_POWERDOWN_N_0} -value {VCC}



# Add PF_OSC_C0_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {PF_OSC_C0} -instance_name {PF_OSC_C0_0}



# Add PFSOC_MSS_INT_C0_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {PFSOC_MSS_C0} -instance_name {PFSOC_MSS_INT_C0_0}
sd_create_pin_group -sd_name ${sd_name} -group_name {CLOCK_IN_0} -instance_name {PFSOC_MSS_INT_C0_0} -pin_names {"REFCLK" "REFCLK_N" }



# Add TRIBUFF_0 instance
sd_instantiate_macro -sd_name ${sd_name} -macro_name {TRIBUFF} -instance_name {TRIBUFF_0}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:CK" "CK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:CK_N" "CK_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:CKE" "CKE" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:CS" "CS" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:MMUART_0_RXD_F2M" "MMUART_0_RXD_F2M" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:MSS_RESET_N_M2F" "MSS_RESET_N_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MSSUART0_TX" "TRIBUFF_0:PAD" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:ODT" "ODT" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:MSS_RESET_N_F2M" "PF_CCC_C0_0:PLL_LOCK_0" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PF_OSC_C0_0:RCOSC_160MHZ_GL" "PF_CCC_C0_0:REF_CLK_0" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:MMUART_0_TXD_M2F" "TRIBUFF_0:D" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:MMUART_0_TXD_OE_M2F" "TRIBUFF_0:E" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:PLL_CPU_LOCK_M2F" "PLL_CPU_LOCK_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:PLL_DDR_LOCK_M2F" "PLL_DDR_LOCK_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:REFCLK" "REFCLK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:REFCLK_N" "REFCLK_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:RESET_N" "RESET_N" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"CA" "PFSOC_MSS_INT_C0_0:CA" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"DM" "PFSOC_MSS_INT_C0_0:DM" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:DQ" "DQ" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:DQS" "DQS" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PFSOC_MSS_INT_C0_0:DQS_N" "DQS_N" }


# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the smartDesign
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign LPDDR4_TOP
generate_component -component_name ${sd_name}
