# Manual for PSD 17

## I2C Communication

| Byte 1 | Byte 2... Byte N |
| --- | --- |
| Data Type | Data |

> **NOTE** Data Length is the number of bytes for the `data` section, so it does not include the first byte

### Commands 
| Data Types | Byte Value | Data Length (to send) |
| :---: | :---: | :---: |
| Disable Motor Command | 0 | 0 |
| Enable Motor Command | 1 | 0 |
| Run FOC Init | 2 | 0 |
| Closed Loop Velocity Command | 3 | 4 (float) |  
| Closed Loop Position Command | 4 | 4 (float) |
| Closed Loop Torque Command | 5 | 4 (float) |
| Open Loop Velocity Command | 6 | 4 (float) |
| ... | ... | ... |

### Settings 

> **NOTE** For Torque Modes, 0 is `Voltage`, 1 is `DC Current`, 2 is `FOC Current`

| Data Types | Byte Value | Data Length (to send) |
| :---: | :---: | :---: |
| Change Motor Current Limit | 121 | 4 (float) |
| Change Motor Phase Resistance | 122 | 4 (float) |
| Change Motor Voltage Alignment | 123 | 4 (float) |
| Change Motor Voltage Limit | 124 | 4 (float) |
| Change Driver Voltage Limit | 125 | 4 (float) |
| Change Supply Voltage | 126 | 4 (float) |
| Change Torque Mode | 127 | 1 (uint8_t) | 
| Velocity kP | 128 | 4 (float) |
| Velocity kI | 129 | 4 (float) |
| Velocity kD | 130 | 4 (float) |
| Position kP | 131 | 4 (float) |
| Position kI | 132 | 4 (float) |
| Position kD | 133 | 4 (float) |
| Position kF | 134 | 4 (float) |
| Current q kP | 135 | 4 (float) |
| Current q kI | 136 | 4 (float) |
| Current q kD | 137 | 4 (float) |
| Current d kP | 138 | 4 (float) |
| Current d kI | 139 | 4 (float) |
| Current d kD | 140 | 4 (float) |

### Requests

> **NOTE** Data Length is the number of bytes for the `data` section to be recieved, it will send back the first byte as an acknowledgement check

| Data Types | Byte Value | Data Length (to recieve) |
| :---: | :---: | :---: |
| Velocity Request | 255 | 4 (float) |
| Position Request | 254 |  4 (float) |
| Current Request | 253 | 4 (float) |
| Error Request | 252 | 1 (uint8_t) |
| ... | ... | ... |

#### Error Codes

> **NOTE** None exist yet

| Byte Value | Error Type |
| :---: | :---: |
| ... | ... |
