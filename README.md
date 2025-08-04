[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

# PSD-17

The PSD-21 is an FOC controller designed for stepper motors, specifically for NEMA 17 stepper motors.

## Folder Tree

```bash
.
├── esp_test_firmware # Test firmware on ESP32
│   └── firmware # PlatformIO Project
├── firmware # Main firmware (SAMD21)
├── i2c_test # I2C Master Communication Test
│   └── project  # CMake Project
├── pcb # KiCad Project
├── samd21_test_firmware # Test firmware on SAMD21
│   └── firmware # PlatformIO Project
└── README.md
```

## Hardware Details

- [NEMA 17 Motor](https://www.omc-stepperonline.com/nema-17-stepper-motor)
    - [STEPPERONLINE 23mm](https://www.amazon.com/dp/B0B93PNYCP?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1)
    - [STEPPERONLINE 38mm](https://www.amazon.com/dp/B0B38GX54H?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1)
- [AS5047P Magnetic Encoder](https://www.digikey.com/en/products/detail/ams-osram-usa-inc/AS5047P-ADAPTERBOARD/5452344?s=N4IgTCBcDaIIYGcCsAGALAdgA4gLoF8g)
    - [Diametric Magnet](https://www.digikey.com/en/products/detail/radial-magnets-inc/9049/6030786) (6mm x 2.5mm )
- [DRV8874 DC Motor Driver](https://www.pololu.com/product/4035) (2x)
- [SAMD21 Seeed Studio Xiao](https://www.amazon.com/Seeeduino-Smallest-Microcontroller-Interfaces-Compatible/dp/B08745JBRP?th=1)

### Passive Components 

- [5V Voltage Regulator](https://www.pololu.com/product/5433?gQT=1)

## Datasheets

### AS5047P
- [Board](https://www.mouser.com/datasheet/2/588/AS5047P-TS_EK_AB_Operation-Manual_Rev.1.0-775823.pdf)
- [Chip](https://look.ams-osram.com/m/d05ee39221f9857/original/AS5047P-DS000324.pdf)

### DRV8874
- [Board](https://www.pololu.com/product/4035)
- [Chip](https://www.ti.com/lit/ds/symlink/drv8874.pdf)

### SAMD21
- [Seeed Studio Xiao](https://wiki.seeedstudio.com/Seeeduino-XIAO/)
- [SAMD21 Chip](https://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf)

### NEMA 17
- [23mm](https://www.omc-stepperonline.com/e-series-nema-17-bipolar-1-8deg-17ncm-24-07oz-in-1a-42x42x23mm-4-wires-17he08-1004s)
- [38mm](https://www.omc-stepperonline.com/e-series-nema-17-bipolar-42ncm-59-49oz-in-1-5a-42x42x38mm-4-wires-w-1m-cable-connector-17he15-1504s)

## Contact
Feel free to reach out!
- awesomeyooner@gmail.com