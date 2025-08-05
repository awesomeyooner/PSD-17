#ifndef BNO055_HPP
#define BNO055_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <string.h>
#include <functional>

#include "math/vector3d.hpp"
#include "math/math_helper.hpp"

#include "util.hpp"
#include "status.hpp"
#include <unistd.h>
#include <thread>

// Accelerometer
constexpr uint8_t REG_ACCEL_LSB_X = 0x08;
constexpr uint8_t REG_ACCEL_MSB_X = 0x09;

constexpr uint8_t REG_ACCEL_LSB_Y = 0x0A;
constexpr uint8_t REG_ACCEL_MSB_Y = 0x0B;

constexpr uint8_t REG_ACCEL_LSB_Z = 0x0C;
constexpr uint8_t REG_ACCEL_MSB_Z = 0x0D;

// Magnetometer
constexpr uint8_t REG_MAG_LSB_X = 0x0E;
constexpr uint8_t REG_MAG_MSB_X = 0x0F;

constexpr uint8_t REG_MAG_LSB_Y = 0x10;
constexpr uint8_t REG_MAG_MSB_Y = 0x11;

constexpr uint8_t REG_MAG_LSB_Z = 0x12;
constexpr uint8_t REG_MAG_MSB_Z = 0x13;

// Gyroscope
constexpr uint8_t REG_GYRO_LSB_X = 0x14;
constexpr uint8_t REG_GYRO_MSB_X = 0x15;

constexpr uint8_t REG_GYRO_LSB_Y = 0x16;
constexpr uint8_t REG_GYRO_MSB_Y = 0x17;

constexpr uint8_t REG_GYRO_LSB_Z = 0x18;
constexpr uint8_t REG_GYRO_MSB_Z = 0x19;

// Others
constexpr uint8_t REG_MODE = 0x3D;
constexpr uint8_t CONF_MODE = 0x00;
constexpr uint8_t AMG_MODE = 0x07;

using namespace math_util;
class BNO055{

    public:

        BNO055(){}

        StatusCode run_ack(std::function<StatusCode(void)> runnable, int attempts, std::string prefix = "", double delay = 1.0){

            for(int i = 1; i < attempts + 1; i++){
                StatusCode status = runnable();
                usleep(conversions::seconds_to_micros(delay));

                if(status == StatusCode::OK){

                    if(prefix.length() != 0)
                        print(prefix + " OK!");
                        
                    return StatusCode::OK;
                }
                else if(status == StatusCode::FAILED && i != attempts){

                    if(prefix.length() != 0)
                        print(prefix + " FAILED! Retrying...");

                }
                else{

                    if(prefix.length() != 0)
                        print(prefix + " FAILED! Ending...");

                    return StatusCode::FAILED;
                }
            }
        }

        StatusCode init(const char* name = "/dev/i2c-9", unsigned short address = 0x28){
            device.addr = address;

            bus = i2c_open(name);

            device.addr = address;
            device.bus = bus;
            device.iaddr_bytes = 1;
            device.page_bytes = 1;

            if(bus == -1)
                return StatusCode::FAILED;
            else
                return StatusCode::OK;
        }

        StatusCode set_mode_AMG(){
            write_bus(REG_MODE, CONF_MODE);
            usleep(conversions::seconds_to_micros(0.5));
            return write_bus(REG_MODE, AMG_MODE);
        }


        void init_threads(){
            // std::thread accel_thread(
            //     [this](){
            //         while(true){
            //             accel = get_accel();
            //         }
            //     }
            // );

            // std::thread gyro_thread(
            //     [this](){
            //         while(true){
            //             gyro = get_gyro();
            //         }
            //     }
            // );

            // std::thread mag_thread(
            //     [this](){
            //         while(true){
            //             mag = get_mag();
            //         }
            //     }
            // );

            std::thread refresh_thread(
                [this](){
                    while(true){
                        if(refresh_all() != StatusCode::OK)
                            print("FAILED!");
                    }
                }
            );

            std::thread print_thread(
                [this](){
                    while(true){
                        std::cout << "Accel: \t" + accel.to_string() + "\t";
                        std::cout << "Gyro: \t" + gyro.to_string() + "\t";
                        std::cout << "Mag: \t" + mag.to_string() << std::endl;
                    }
                }
            );
                
            

            // accel_thread.join();
            // gyro_thread.join();
            // mag_thread.join();
            refresh_thread.join();
            print_thread.join();
        }

        void close(){
            i2c_close(bus);
        }

        StatusCode refresh_all(){
            uint8_t data[18];

            StatusCode status = i2c_read(&device, 0x08, data, sizeof(data)) == sizeof(data) ? StatusCode::OK : StatusCode::FAILED;

            int16_t raw_accel_x = (data[1] << 8) | data[0];
            int16_t raw_accel_y = (data[3] << 8) | data[2];
            int16_t raw_accel_z = (data[5] << 8) | data[4];

            int16_t raw_mag_x = (data[7] << 8) | data[6];
            int16_t raw_mag_y = (data[9] << 8) | data[8];
            int16_t raw_mag_z = (data[11] << 8) | data[10];

            int16_t raw_gyro_x = (data[13] << 8) | data[12];
            int16_t raw_gyro_y = (data[15] << 8) | data[14];
            int16_t raw_gyro_z = (data[17] << 8) | data[16];

            double accel_x = conversions::mg_to_mps2((double)(raw_accel_x));
            double accel_y = conversions::mg_to_mps2((double)(raw_accel_y));
            double accel_z = conversions::mg_to_mps2((double)(raw_accel_z));

            accel.refresh(accel_x, accel_y, accel_z);

            double mag_x = (double)(raw_mag_x) / 1000.0;
            double mag_y = (double)(raw_mag_y) / 1000.0;
            double mag_z = (double)(raw_mag_z) / 1000.0;

            mag.refresh(mag_x, mag_y, mag_z);

            double gyro_x = conversions::degrees_to_radians((double)(raw_gyro_x));
            double gyro_y = conversions::degrees_to_radians((double)(raw_gyro_y));
            double gyro_z = conversions::degrees_to_radians((double)(raw_gyro_z));

            gyro.refresh(gyro_x, gyro_y, gyro_z);

            return status;
        }

        StatusCode refresh_accel(){
            StatusedValue<double> x = get_reading(REG_ACCEL_LSB_X, REG_ACCEL_MSB_X);
            StatusedValue<double> y = get_reading(REG_ACCEL_LSB_Y, REG_ACCEL_MSB_Y);
            StatusedValue<double> z = get_reading(REG_ACCEL_LSB_Z, REG_ACCEL_MSB_Z);

            accel.refresh(
                conversions::mg_to_mps2(x.value),
                conversions::mg_to_mps2(y.value),
                conversions::mg_to_mps2(z.value)
            );

            return statuses_OK({x.status, y.status, z.status}) ? StatusCode::OK : StatusCode::FAILED;
        }

        StatusCode refresh_mag(){
            StatusedValue<double> x = get_reading(REG_MAG_LSB_X, REG_MAG_MSB_X);
            StatusedValue<double> y = get_reading(REG_MAG_LSB_Y, REG_MAG_MSB_Y);
            StatusedValue<double> z = get_reading(REG_MAG_LSB_Z, REG_MAG_MSB_Z);

            mag.refresh(
                conversions::mg_to_mps2(x.value),
                conversions::mg_to_mps2(y.value),
                conversions::mg_to_mps2(z.value)
            );

            return statuses_OK({x.status, y.status, z.status}) ? StatusCode::OK : StatusCode::FAILED;
        }

        StatusCode refresh_gyro(){
            StatusedValue<double> x = get_reading(REG_GYRO_LSB_X, REG_GYRO_MSB_X);
            StatusedValue<double> y = get_reading(REG_GYRO_LSB_Y, REG_GYRO_MSB_Y);
            StatusedValue<double> z = get_reading(REG_GYRO_LSB_Z, REG_GYRO_MSB_Z);

            gyro.refresh(
                conversions::mg_to_mps2(x.value),
                conversions::mg_to_mps2(y.value),
                conversions::mg_to_mps2(z.value)
            );

            return statuses_OK({x.status, y.status, z.status}) ? StatusCode::OK : StatusCode::FAILED;
        }
        
        Vector3D get_accel(){
            return accel;
        }

        Vector3D get_mag(){
            return mag;
        }
        
        Vector3D get_gyro(){
            return gyro;
        }
        
    private:
        i2c_device device;
        int bus;

        Vector3D accel;
        Vector3D gyro;
        Vector3D mag;

        StatusedValue<double> get_reading(uint8_t reg_lsb, uint8_t reg_msg){
            StatusedValue<uint16_t> raw = read_bus(reg_lsb, reg_msg);

            return StatusedValue((double)((int16_t)raw.value), raw.status);
        }

        StatusedValue<uint16_t> read_bus(uint8_t reg_lsb, uint8_t reg_msb){

            StatusedValue<uint8_t> lsb = read_bus(reg_lsb);
            StatusedValue<uint8_t> msb = read_bus(reg_msb);

            StatusCode status  = lsb.status == StatusCode::OK && msb.status == StatusCode::OK ? StatusCode::OK : StatusCode::FAILED;
            uint16_t value = (msb.value << 8 | lsb.value); // little endian

            return StatusedValue(value, status); 
        }

        StatusedValue<uint8_t> read_bus(uint8_t reg){
            uint8_t buffer[1];

            StatusCode status = i2c_read(&device, reg, buffer, sizeof(buffer)) == sizeof(buffer) ? StatusCode::OK : StatusCode::FAILED;
        
            return StatusedValue<uint8_t>(buffer[0], status);
        }

        StatusCode write_bus(uint8_t reg, uint8_t write){
            uint8_t data[] = {write};
            return write_bus(reg, data);
        }

        StatusCode write_bus(uint8_t reg, uint8_t write[]){
            return i2c_write(&device, reg, write, sizeof(write)) == sizeof(write) ? StatusCode::OK : StatusCode::FAILED;
        }


}; // class BNO055

#endif