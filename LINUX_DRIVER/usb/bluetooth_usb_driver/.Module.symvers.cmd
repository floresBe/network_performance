cmd_/home/aale/Downloads/LINUX_DRIVER/usb/bluetooth_usb_driver/Module.symvers := sed 's/ko$$/o/' /home/aale/Downloads/LINUX_DRIVER/usb/bluetooth_usb_driver/modules.order | scripts/mod/modpost  -a   -o /home/aale/Downloads/LINUX_DRIVER/usb/bluetooth_usb_driver/Module.symvers -e -i Module.symvers   -T -