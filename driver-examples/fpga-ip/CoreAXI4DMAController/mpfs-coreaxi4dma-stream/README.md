# CoreAXI4DMAController Streaming Example

- [CoreAXI4DMAController Streaming Example](#coreaxi4dmacontroller-streaming-example)
  - [Introduction](#introduction)
  - [Hardware Dependencies](#hardware-dependencies)
  - [How to Use This Example](#how-to-use-this-example)

## Introduction

This example project demonstrates the use of the CoreAXI4DMAController IP for data stream transfers.

The following document explains how to use the project: [AXI4_STREAM_DEMO](https://mi-v-ecosystem.github.io/redirects/demo-guides_mpfs-axi4-stream-demo).

This design includes a stream generator master module at address 0x40000F00.

The stream generator has a `PATTERN COUNT` register at offset address 0x0u.
Configure the size of the AXI4 stream transfer by writing the number of 64-bit words to be transferred
into this register.

The stream generator will transfer 64-bit data with an incrementing pattern up to the value that is
programmed into the `PATTERN COUNT` register.
CoreAXI4DMAController can accept transfers of up to 1,048,576 64-bit words (8 MiB).

## Hardware Dependencies

To run this project the Icicle Kit Reference Design is required.
It is available in the following repository:
[PolarFire® SoC Icicle Kit Reference Design Generation Tcl Scripts](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design).

The "AXI4 Stream Demo" configuration of the Icicle Kit Reference design must be built and programmed
onto the PolarFire Soc Icicle Kit.

This configuration can be achieved by passing the `AXI4_STREAM_DEMO` build argument when generating
the Icicle Kit Reference Design.

## How to Use This Example

This project is tested on PolarFire SoC Icicle Kit.

On connecting Icicle Kit J11 to the host PC, you should see 4 COM port interfaces connected.
To use this project configure COM port **interface1** as below:

- Baud Rate: 115200
- Data bits: 8
- Stop bits: 1
- Parity: no

The example project will display messages over the COM port.
The example can be run by pressing the `SPACE` key.
