# CoreAXI4DMAController Block Transfer Example

- [CoreAXI4DMAController Block Transfer Example](#coreaxi4dmacontroller-block-transfer-example)
  - [Introduction](#introduction)
  - [Hardware Dependencies](#hardware-dependencies)
  - [How to Use This Example](#how-to-use-this-example)

## Introduction

This example project demonstrates the use of the CoreAXI4DMAController IP for block transfers.

## Hardware Dependencies

Running this project requires programming the [Icicle Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-Kit-reference-design)
onto the PolarFire SoC Icicle Kit.

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
