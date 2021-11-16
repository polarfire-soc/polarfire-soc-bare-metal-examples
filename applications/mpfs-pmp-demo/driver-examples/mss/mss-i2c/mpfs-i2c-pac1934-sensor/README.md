# PolarFire SoC MSS I2C sensor interface example
This example project demonstrates reading voltage and current data from PAC1934
sensor on the Icicle kit using I2C interface.

## How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces connected. To use this project configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This project interfaces with the PAC1934 sensor on the Icicle kit over I2C bus 
using MSS I2C1 and displays the voltage and current values over the serial terminal.

This project can be tested with default [reference Libero design](https://github.com/polarfire-soc/icicle-kit-reference-design/releases).

This project provides build configurations and debug launchers as described [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)

