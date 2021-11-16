#This Tcl file sources other Tcl files to build the design(on which recursive export is run) in a bottom-up fashion

#Sourcing the Tcl file in which all the HDL source files used in the design are imported or linked
source hdl_source.tcl
build_design_hierarchy

#Sourcing the Tcl files for creating individual components under the top level
source components/PFSOC_MSS_C0.tcl 
source components/PF_CCC_C0.tcl 
source components/PF_OSC_C0.tcl 
source components/LPDDR4_TOP.tcl 
build_design_hierarchy
