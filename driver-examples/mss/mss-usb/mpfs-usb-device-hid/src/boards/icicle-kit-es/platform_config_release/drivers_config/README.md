# drivers_config folder
The user-configured driver configurations are located in this folder.

If the user needs to modify the default configuration of the driver, then the 
user should copy the configuration file from the platform_config_reference 
folder [ex. *platform/platform_config_reference/drivers_config/mss/mss_usb/
mss_usb_sw_config.h*] and paste it in the platform_config folder [ex. *boards/
icicle-kit-es/platform_config/drivers_config/mss/mss_usb/mss_usb_sw_config.h*].
The user should modify only the configuration files that were copied into the 
platform_config folder, and the user should update the include path from the 
default path platform_config_reference [*$workspace_loc:/$ProjName/src/platform/
platform_config_reference*] to the platform_config path [*$workspace_loc:/
$ProjName/src/boards/icicle-kit-es/platform_config*] in the project build 
settings.
