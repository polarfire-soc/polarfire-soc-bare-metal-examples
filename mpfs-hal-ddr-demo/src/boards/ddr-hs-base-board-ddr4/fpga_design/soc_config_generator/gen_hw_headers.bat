@ECHO OFF
ECHO generate header files for Embedded software project
ECHO mpfs_configuration_generator.py
set xml_file=../xml/PFSOC_MSS_C0_0.xml
set output_folder=../../
py -3 mpfs_configuration_generator.py %xml_file% %output_folder%
pause
