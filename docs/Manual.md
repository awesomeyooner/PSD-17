# Manual for PSD 17

## I2C Communication

| Byte 1 | Byte 2... Byte N |
| --- | --- |
| Data Type | Data |

> **NOTE** Data Length is the number of bytes for the `data` section, so it does not include the first byte

### Commands 
| Data Types | Byte Value | Data Length |
| :---: | :---: | :---: |
| Disable Motor Command | 0 | 0 |
| Enable Motor Command | 1 | 0 |
| Closed Loop Velocity Command | 1 | 4 (float) |  
| Closed Loop Position Command | 2 | 4 (float) |
| Closed Loop Torque Command | 3 | 4 (float) |
| Open Loop Velocity Command | 4 | 4 (float) |
| ... | ... | ... |

### Requests

> **NOTE** Data Length is the number of bytes for the `data` section to be recieved, it will send back the first byte as an acknowledgement check

| Data Types | Byte Value | Data Length |
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
