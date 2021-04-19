#include "HDC1080.hpp"

#include <Arduino.h>

#include <Wire.h>

HDC1080::HDC1080(uint16_t i2c_clock, uint16_t i2c_data)
{
	
}

HDC1080::HDC1080()
{

}

bool HDC1080::begin(uint8_t i2c_adress)
{
    this->address = i2c_adress;
	Wire.begin();

    uint8_t readByte[2] = {0};

    // Check if Manufacturer and Device ID match
    if( read2Bytes(address, HDC1080_ADRESS_MANUFACTURER_ID) == 0x5449 &&
        read2Bytes(address, HDC1080_ADRESS_DEVICE_ID) == 0x1050)
    {
        config = read2Bytes(address, HDC1080_ADRESS_CONFIG);
        return true;
    }   
    else
        return false;
}

uint16_t HDC1080::readTemperature()
{

}

uint16_t HDC1080::readHumidity()
{

}

HDC1080::measurement HDC1080::readData()
{
    uint8_t data[4] = {0};
    readBytes(HDC1080_DEVICE, HDC1080_ADRESS_TEMPERATURE, data, 4);
    
    uint16_t temp = (((uint16_t) data[0] << 8 | data[1]));
    uint16_t humidity = (((uint16_t) data[2] << 8 | data[3]));

    return {
        (double)(temp)/(65536)*165-40, 
        (double)(humidity)/(65536)*100
    };
}

void HDC1080::configure(uint8_t configBytes) 
{
    this->config = configBytes;
    Wire.beginTransmission(address);
    Wire.write(HDC1080_ADRESS_CONFIG);
    Wire.write(configBytes);
    Wire.write(0x00);
    Wire.endTransmission();
}

uint16_t HDC1080::read2Bytes(uint8_t dev_adress, uint8_t reg_address)
{
    uint8_t bytes[2] = {0};
    readBytes(dev_adress, reg_address, bytes, 2);
    return (uint16_t) bytes[0] << 8 | bytes[1];
}

bool HDC1080::readBytes(uint8_t dev_adress, uint8_t reg_address, uint8_t* bytes, uint8_t numBytes)
{
    uint8_t readByte[2] = {0};

    // Request Device ID
    Wire.beginTransmission(dev_adress);
    Wire.write(reg_address);
    Wire.endTransmission();

    delay(20); // Wait for the device to process the request
    Wire.requestFrom(dev_adress, numBytes);
    delay(1);
    if(Wire.available() <= numBytes)
    {
        for(int i=0; i<numBytes; i++)
            bytes[i] = Wire.read();
    }
}
