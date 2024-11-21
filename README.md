# FIT_FC11_firmware
## CLI
All the commands from the Fan Control Board Manual (FCB Manual 2022-02-15-2-1.pdf) had been recreated. 

### CLI Normal Mode
In the normal mode all the commands should be ended with `<CR>`
- `xxR` – read device status. Response “xx rpm1 rpm2 rpm3 rpm4 rpm5 field1 field2 err1 err2 L M” where:
    - `xx` – device address
    - `rpm1` .. `rpm5` fan rotation speeds, roots per minutes
    - `field1` – magnetic field, measured by 1-st sensor, mT
    - `field2` – magnetic field, measured by 2-nd sensor, mT
    - `err1` – 1-st magnetic sensor error code, normally must be 0. If err1 ≠ 0, field1 is invalid.
    - `err2` – 2-nd magnetic sensor error code, normally must be 0. If err2 ≠ 0, field2 is invalid.
    - `L` – L relay status: 0 – OFF, 1 – ON
    - `M` – M relay status: 0 – OFF, 1 – ON
- `xxL0` – turn relay L OFF. Response “xx OK”
- `xxL1` - turn relay L ON. Response “xx OK”
- `xxM0` – turn relay M OFF. Response “xx OK”
- `xxM1` - turn relay M ON. Response “xx OK”
- `xxversion` – reads HW and FW versions
- `xxdiagnostic_mode_on` – enters diagnostic mode
- `xxboard_reset` – resets board MCU
- `xxremote_upgrade_mode` [WIP]

### CLI Diagnostic Mode
In the diagnostic mode all commands are initiated by simply pressing a key.
- ` ` – SPACE toggle fans ON/OFF
- `0` to `9` – set rotation speed from 0% to 90% from full range (PWM)
- `f` – set full rotation speed
- `l` – turn OFF relay L
- `L` – turn ON relay L
- `m` – turn OFF relay M
- `M` – turn ON relay M
- `x` – exit diagnostic mode
- `z` – reset microcontroller and exit diagnostic mode

## Remarks
### PCB
The version of the original PCB is `MMXXI-V11` and the version of the recreated PCB is `FIT v1.1 2024`, should be `v1.2` probably. 
The original PCB firmware responds with hw version `1.1`
responds
### CLI
THe commands mentioned in the FCB Manual are not all available on the original PCB firmware. 
The manual is probably newer than the original PCB firmware (which is 1.0) The differences are:
1. Unrecognized command `xxboard_reset`
2. Command `xxdiagnostic_mode_on` working differently. After displaying status several times within a few seconds, the firmware goes back to the normal mode.
Additional commands mentioned in the manual seem not to be working.
3. The original firmware responds are ended with `<CR><LF>` while the manual mentions only `<CR>`
### FANS
For now it's unknown what is the default fan speed and how it is adjusted. The only way to change them seems to be through *diagnostic mode* but it's not clear
whether changes made while being within this mode are permament.

## To Do List
- [] Remote upgrade
- [] Read fans RPMs
- [] Find out initial fans RPM
- [] Find out how fans RPM is changed during operation
- [] Add MLX90393 readout