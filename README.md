# BORC
Battery-Operated Radiator Control

A low-power automatic radiator control that is designed to retrofit on steam radiators that use Honeywell's T104A1040 Zone Valve Operator
uses LowPowerLab's [DualOptiboot](https://github.com/LowPowerLab/DualOptiboot) bootloader for the ATmega1284P.

This repo contains:
- PCB design files and schematic for control board, power board and LED board for the BORC.
- Firmware for a low-power radiator control node
- Logger script to get real-time current data from the BORC

---

## Dependencies

To compile the firmware, [Arduino IDE 1.8.19](https://www.arduino.cc/en/software) is needed. Newer IDE versions are not verfied to work.

The firmware requires the following libraries installed:
- [RFM69](https://github.com/LowPowerLab/RFM69)
- [SPIFlash](https://github.com/LowPowerLab/SPIFlash)
- [LowPower](https://github.com/LowPowerLab/LowPower)
- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

The firmware also requires the Moteino Board definitions installed following [this guide](https://lowpowerlab.com/guide/moteino/programming-libraries/).

---

## Logger script

The [logger script](https://github.com/berg-lab/BORC/blob/master/logger/logger.py) enables capturing current usage data over the serial port on the BORC.

### Prerequisites:
- [pip](https://pip.pypa.io/en/stable/installation/)
- [Pyserial](https://pypi.org/project/pyserial/)

### Run:
Run the script using the following command over a command line interface:

_For Windows:_
```
python logger.py -c COM15 -n filename
```

_For MacOS/Linux:_
```
python logger.py -c /dev/ttyusb0 -n filename
```

### Notes:
- Change `python` to `python3` if using Python version 3.
- Change `filename` to something referencing the test to keep the generated files organized. The current date/time is automatically added to the filename.
- The script automatically adds the `.csv` extension to the filename, so there's no need to include it in the arguments when running it.
- For Windows, go to device manager and find the COM port of the BORC under "Ports" section and replace `COM15` in the command above.
- For MacOS or Linux, use `ls /dev/tty.*` command in terminal to find the COM port of the BORC, and replace `/dev/ttyusb0` above with the full path of the COM port.
- Use `-h` command to bring a help menu (eg. `python logger.py -h`).
- If the servo doesn't move in some areas, you can adjust the default min and max limits to the particular servo installed. The default limits should be set to where the servo is guaranteed to move, and beyond these limits, the servo will not respond. Typical min limit for a 270° servo is 3600 and max limit is 3950. You can increase or decrease these limits to see where the servo being tested begins to move.
- Default baudrate for the BORC is `115200`. If updated, make sure to update the same in the logger script.
