# CorePWM Duty Cycle Control Example Project

This example project demonstrates how to utilize the CorePWM IP to control the
duty cycle of individual PWM outputs. It slowly flashes a single LED.The LED
slowly becomes brighter as the PWM duty cycle increases and slowly dims as the
duty cycle decreases.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. To use this project configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

Make sure to set up following hardware configuration for running the current
example project:
   - One hardware instance of CorePWM should be created with 1 PWM Channel.
   - Connect an external LED to the PWM pin of the microbus socket.
   - Take the LED, connect the LED's longer pin (anode/positive) to the PWM pin
     on the microbus socket.
   - Connect the LED's shorter pin (cathode/negative) to an appropriate ground.
   - Use a resistor to control the current, its value depends on your LED's
     specifications.

**Note:** You can use a logic analyzer or oscilloscope to monitor the changing
duty cycle of the PWM pin.

The example project will display a greeting message over the UART terminal. User
can notice the LED's brightness changing.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/polarfire-soc-documentation-master_README)

This example is tested on the PolarFire SoC Icicle kit with the v2023.06
reference design.It is available in the following repository:
[PolarFire® SoC Icicle Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design).
