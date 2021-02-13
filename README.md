# Explorer
Software, firmware, CAD models for a 12" round differential drive robot

## Documentation
HTML files are located in the doc directory

## Firmware Build
* install platformio for your system https://platformio.org/
* in the firmware/motor_controller directory run 'pio run' to build the firmware
* with an appropriate device connected run 'pio run -t upload' to upload the firmware

## R/C Server
The robot can be remote controlled via any web browser. This is done with a node.js server that runs on the Raspberry Pi on the robot which both serves a control console web page and implements a REST API which provides basic motion control.
* install nodejs on the raspi on the robot
* in the software/node_rc directory run 'node rc_server.js'

## HW Details
The hardware directory contains openscad and other drawings

