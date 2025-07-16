# PolarFire&reg; SoC platform source code

## Repo organization

```text
<platform>
  |
  |-- drivers
  |     |- fpga_ip
  |     |     | .
  |     |     | .  
  |     |-  mss
  |     |     |- mss-ethernet-mac
  |     |     |- mss-can
  |     |     |- mss-gpio
  |     |     |     .
  |     |     |     .
  |     |     |     .
  |     |     |- mss-mmc
  |     |     |- mss-watchdog  
  |     |           |- mss-watchdog.c
  |     |           |- mss-watchdog.h
  |     |
  |     |- off_chip
  |     |     |  .
  |     |     |  .
  |     |
  |-- hal
  |     |
  |-- mpfs_hal
  |     |
  |-- platform_config_reference
  |     |     |- linker
  |     |     |     |- mpfs-ddr-loaded-by-boot-loader.ld
  |     |     |     |- mpfs-envm-lma-scratchpad-vma.ld
  |     |     |     |- mpfs-envm.ld
  |     |     |     |- mpfs-lim-lma-scratchpad-vma.ld
  |     |     |     |- mpfs-lim.ld
  |     |     |
  |     |     |
  |     |     |- mpfs_hal_config
  |     |           |- mss_sw_config.h
  |     |
  |-- soc_config_generator
  |           |
  |           |- mpfs_configuration_generator.py

```

This repository will always contain the latest and greatest of the platform contents. Some of the example projects under [polarfire-soc-bare-metal-examples](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)
may not contain all the contents or latest versions of of the contents from this repository. In such cases, please download this repository and replace the src/platform repository in the project with it.

When you update the _platform_ repository in your project, you must make sure that the reference design (and the xml configuration) is compatible with it.

For detailed description about the contents of the platform sub-directories and the overall bare metal software project folder structure, please refer to the
[Bare Metal Software Projects Structure](https://mi-v-ecosystem.github.io/redirects/bare-metal-project-structure_bare-metal-software-project-structure)
