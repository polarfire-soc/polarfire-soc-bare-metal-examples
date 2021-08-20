
#                         mpfs-pmp-app-u54-1

This example project creates an appliction which is loaded by hart-software-services(HSS). 
U54_1 executes this application in M-mode.

This appliction provides user options to validate PMP.

#### hss-payload-generator

The payload generator tool is used to create payload.bin from mpfs-pmp-app-u54-1.elf.
HSS ymodem is invoked to copy the payload.bin to SD or eMMC.

**Payload generator** [https://github.com/polarfire-soc/hart-software-services/tree/master/tools/hss-payload-generator](https://github.com/polarfire-soc/hart-software-services/tree/master/tools/hss-payload-generator)

#### Loading application to LPDDR4
On board power-up, the payload.bin will be copied to LPDDR4 from SD or eMMC and executed.
