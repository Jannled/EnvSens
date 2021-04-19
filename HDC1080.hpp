#ifndef J3_HDC1080_H
#define J3_HDC1080_H

#include <stdint.h>

#define HDC1080_DEVICE 0x40
#define HDC1080_ADRESS_MANUFACTURER_ID 0xFE
#define HDC1080_ADRESS_DEVICE_ID 0xFF
#define HDC1080_ADRESS_TEMPERATURE 0x00
#define HDC1080_ADRESS_HUMIDITY 0x01
#define HDC1080_ADRESS_CONFIG 0x02

#define HDC1080_CONF_HEAT_ENABLE 		0x20
#define HDC1080_CONF_HEAT_DISABLE 		0x00

#define HDC1080_CONF_MODE_SEQUENTIAL	0x10
#define HDC1080_CONF_MODE_INDIVIDUAL	0x00

#define HDC1080_CONF_TRES_14BIT			0x00
#define HDC1080_CONF_TRES_11BIT			0x04

#define HDC1080_CONF_HRES_14BIT			0x00
#define HDC1080_CONF_HRES_11BIT			0x01
#define HDC1080_CONF_HRES_8BIT			0x02

class HDC1080 
{
	public: 
		/** TODO: Add possibility for Software I2C */
		HDC1080(uint16_t i2c_clock, uint16_t i2c_data);

		/** Connect to the HDC1080 using Hardware I2C */
		HDC1080();

		bool begin(uint8_t i2c_adress = HDC1080_DEVICE);

		typedef struct measurement {
			double temperature;
			double humidity;
		};

		measurement readData();
		uint16_t readTemperature();
		uint16_t readHumidity();

		void configure(uint8_t configBytes);

	private:
		uint16_t read2Bytes(uint8_t dev_adress, uint8_t reg_address);
		bool readBytes(uint8_t dev_adress, uint8_t reg_address, uint8_t* bytes, uint8_t numBytes);

		uint16_t config = 0x1000;
		uint8_t address = HDC1080_DEVICE;
};

#endif
