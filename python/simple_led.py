import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD)

GPIO.setup(11, GPIO.OUT)

GPIO.output(11, GPIO.HIGH)

time.sleep(100)

GPIO.cleanup()
