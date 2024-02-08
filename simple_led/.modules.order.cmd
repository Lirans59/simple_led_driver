cmd_/home/pi/dev/simple_led/modules.order := {   echo /home/pi/dev/simple_led/simple_led.ko; :; } | awk '!x[$$0]++' - > /home/pi/dev/simple_led/modules.order
