# PSD-17
The PSD-21 is an FOC controller designed for stepper motors, specifically for NEMA 17 stepper motors.

## Folder Directory

```bash
.
├── esp_test_firmware
│   └── firmware
├── firmware # Bob
├── i2c_test
├── pcb
├── samd21_test_firmware 
└── README.md
```

## Hardware Details

- [STEPPERONLINE NEMA 17 Motor](https://www.amazon.com/dp/B0B93PNYCP?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) (23mm)
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