#include <U8g2lib.h>

#include <Adafruit_CCS811.h>
#include <Adafruit_BMP280.h>

#include "HDC1080.hpp"

#include "secret.h"

// U8GLIB_SSD1306_128X32 Addr: 0x78

//CCS811      Addr: 0x5A (evtl 0x5B)  MOX-Sensor
//HDC1080     Addr: 0x40              Luftfeuchtigkeit und Temperatur
//BMP280      Addr: 0x77 (evtl 0x76)  Luftdruck und Temperatur?


/* Enable and disable certain parts of the software */
#define ENABLE_DISPLAY
#define ENABLE_CCS811
#define ENABLE_HDC1080
#define ENABLE_BMP280

// Constructors
#ifdef ENABLE_DISPLAY
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0);
#endif

#ifdef ENABLE_CCS811
Adafruit_CCS811 ccs;
#endif

#ifdef ENABLE_HDC1080
HDC1080 hdc;
#endif

#ifdef ENABLE_BMP280
Adafruit_BMP280 bmp; // I2C
#endif

void setup(void) 
{
	//Init console
	Serial.begin(115200);

	#ifdef ENABLE_DISPLAY
	u8g2.begin();
	Serial.print("Display Adress: ");
	Serial.println(u8g2.getU8x8()->i2c_address);
	#endif

	#ifdef ENABLE_CCS811
	if(!ccs.begin(0x5A))
		Serial.println("[CCS811] Failed to start sensor! Please check your wiring.");
	#endif

	#ifdef ENABLE_HDC1080
	if(!hdc.begin())
		Serial.println("[CCS811] Failed to find the HDC1080 sensor. Please check your wiring.");
	hdc.configure(HDC1080_CONF_HEAT_ENABLE | HDC1080_CONF_MODE_SEQUENTIAL);
	#endif
	
	#ifdef ENABLE_BMP280
	if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID))
		Serial.println("[BMP280] Could not find a valid BMP280 sensor, check wiring!");
	else 
	{
		bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,	/* Operating Mode. */
				Adafruit_BMP280::SAMPLING_X2,			/* Temp. oversampling */
				Adafruit_BMP280::SAMPLING_X16,			/* Pressure oversampling */
				Adafruit_BMP280::FILTER_X16,			/* Filtering. */
				Adafruit_BMP280::STANDBY_MS_500);		/* Standby time. */

	}
	/* Default settings from datasheet. */
  	#endif
}

void loop(void) 
{
	#ifdef ENABLE_DISPLAY
	u8g2.firstPage();
	do 
	{
		u8g2.setFont(u8g_font_unifont);
		u8g2.drawStr(0, 10, "Lorem Ipsum");
	} while ( u8g2.nextPage() );
	//delay(1000);
	#endif

	#ifdef ENABLE_CCS811
	if(ccs.available())
	{
		if(!ccs.readData())
		{
			Serial.print("[CCS811] eCO2: ");
			Serial.print(ccs.geteCO2());
			Serial.print("ppm, TVOC: ");
			Serial.println(ccs.getTVOC());
		}
		else
			Serial.println("[CCS811] ERROR!");
	}
	#endif

	#ifdef ENABLE_HDC1080
	HDC1080::measurement hdc1080data = hdc.readData();

	Serial.print("[HDC1080] ");
	Serial.print(hdc1080data.temperature);
	Serial.println(" °C");

	Serial.print("[HDC1080] ");
	Serial.print(hdc1080data.humidity);
	Serial.println(" %");
	#endif
	
	#ifdef ENABLE_BMP280
	Serial.print("[BMP280] Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" °C");

    Serial.print("[BMP280] Pressure = ");
    Serial.print(bmp.readPressure()/100);
    Serial.println(" hPa");

    Serial.print("[BMP280] Approx altitude = ");
    Serial.print(bmp.readAltitude(1025)); // Adjust to local forecast!
    Serial.println(" m");
	#endif

	Serial.println();
	delay(2000);
}