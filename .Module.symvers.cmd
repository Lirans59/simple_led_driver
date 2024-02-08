cmd_/home/pi/dev/simple_led/Module.symvers := sed 's/ko$$/o/' /home/pi/dev/simple_led/modules.order | scripts/mod/modpost -m -a   -o /home/pi/dev/simple_led/Module.symvers -e -i Module.symvers   -T -
