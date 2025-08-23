#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO    GPIO_NUM_19  // SCL pin
#define I2C_MASTER_SDA_IO    GPIO_NUM_18  // SDA pin
#define I2C_MASTER_NUM       I2C_NUM_0    // I2C port number
#define I2C_MASTER_FREQ_HZ   100000       // I2C clock frequency (e.g., 100 kHz)
#define I2C_MASTER_TX_BUF_LEN    0        // I2C master buffer size for TX
#define I2C_MASTER_RX_BUF_LEN    0        // I2C master buffer size for RX