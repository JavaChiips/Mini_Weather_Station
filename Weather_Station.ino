#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <dht.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define dht_apin A0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
dht DHT;
Adafruit_BMP085 bmp;

int pressure, calculate_pressure;
int pressure_store[200];
int count=1; 
int count_time=0;
int mode=0;
int i,z;
int pressure_mode = 0;

void setup() 
{
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("Weather Station Accessed.....\n\n");
  Serial.print("**************************************\n");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 32);
  display.println(F("Display Accessed..."));
  display.display();
  delay(1000);//Wait before accessing Sensor
  if (!bmp.begin()) 
  {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}

void loop() 
{
  //Humidity
  DHT.read11(dht_apin);
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity);
  Serial.println("%  ");
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Humidity =");
  display.setCursor(65,0);
  display.print(DHT.humidity);
  display.setCursor(100,0);
  display.println("%  ");

  //Air Pressure
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  //Sea Level Pressure
  pressure = bmp.readSealevelPressure();
  pressure = pressure/100000*1000; //Convert Pa to mbar
  
  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  display.setCursor(0,10);
  display.print("Pressure = ");
  display.setCursor(65,10);
  display.print(bmp.readSealevelPressure());
  display.setCursor(100,10);
  display.println(" Pa");

  //Temperature
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  display.setCursor(0,20);
  display.print("Temp     = ");
  display.setCursor(65,20);
  display.print(bmp.readTemperature());
  display.setCursor(95,20);
  display.println(" *C");
  
  Serial.print("Temperature 2 = ");
  Serial.print(DHT.temperature);
  Serial.println(" *C");
  Serial.print("**************************************\n");
  display.display();
  //===================Storing Pressure in 3 hour period===============================
  if(count_time == 60)
  {
    count_time = 0 ;
    pressure_store[count]=pressure;
    count = count+1;

     if(count >= 181)
     {
        count = 1;
        mode = 1;
     }
      
     if(mode == 1) //Checking pressure
     {
        for(i=1;i<=180;i++)
        {
          calculate_pressure = pressure - pressure_store[i];
          if(calculate_pressure >= 1.6 & pressure > 947 & pressure < 1030) //Pressure Raising
          {
            pressure_mode = 1;
          }
          else if(calculate_pressure <= -1.6 & pressure > 985 & pressure < 1050) //Pressure Falling
          {
            pressure_mode = 2;
          }
          else if(calculate_pressure < 1.6 & calculate_pressure > -1.6 & pressure > 960 & pressure < 1033) //Pressure in Steady
          {
            pressure_mode = 3;
          }
        }
     }
     else if(mode == 0)
     {
        for(i=1;i<=count;i++)
        {
          calculate_pressure = pressure - pressure_store[i];
          if(calculate_pressure >= 1.6 & pressure > 947 & pressure < 1030) //Pressure Raising
          {
            pressure_mode = 1;
          }
          else if(calculate_pressure <= -1.6 & pressure > 985 & pressure < 1050) //Pressure Falling
          {
            pressure_mode = 2;
          }
          else if(calculate_pressure < 1.6 & calculate_pressure > -1.6 & pressure > 960 & pressure < 1033) //Pressure in Steady
          {
            pressure_mode = 3;
          }
        }
     }
  }
  //==================================Finish Storing and Checking=====================================================
  
  //********************************Start Calculation****************************************************
  if(pressure_mode == 1) //Pressure Raising
  {
    z = 185 - (0.16*pressure);
  }
  else if(pressure_mode == 2) //Pressure Falling
  {
    z = 127 - (0.12*pressure);
  }
  else if(pressure_mode == 3) //Pressure Steady
  {
    z = 144 - (0.13*pressure);
  }

 

  display.setCursor(0,50);
  switch(z)
  {
    case 1:
      display.print("Settled Fine");
      break;
    case 2:
      display.print("Fine Weather");
      break;
    case 3:
      display.print("Fine, Becoming Less Settled");
      break;
    case 4:
      display.print("Fairly Fine, Showery Later");
      break;
    case 5:
      display.print("Showery, Becoming More Unsettled");
      break;
    case 6:
      display.print("Unsettled, Rain Later");
      break;
    case 7:
      display.print("Rain at Times, Worse Later");
      break;
    case 8:
      display.print("Rain at Times, Becoming Very Unsettled");
      break;
    case 9:
      display.print("Very Unsettled, Rain");
      break;
    case 10:
      display.print("Settled Fine");
      break;
    case 11:
      display.print("Fine Weather");
      break;
    case 12:
      display.print("Fine, Possibly Showers");
      break;
    case 13:
      display.print("Fairly Fine, Showers Likely");
      break;
    case 14:
      display.print("Showery, Bright Intervals");
      break;
    case 15:
      display.print("Changeable, Some Rain");
      break;
    case 16:
      display.print("Unsettled, Rain at Times");
      break;
    case 17:
      display.print("Rain at Frequent Intervals");
      break;
    case 18:
      display.print("Very Unsettled, Rain");
      break;
    case 19:
      display.print("Stormy, Much Rain");
      break;
    case 20:
      display.print("Settled Fine");
      break;
    case 21:
      display.print("Fine Weather");
      break;
    case 22:
      display.print("Becoming Fine");
      break;
    case 23:
      display.print("Fairly Fine, Improving");
      break;
    case 24:
      display.print("Fairly Fine, Possibly Showers Early");
      break;
    case 25:
      display.print("Showery Early, Improving");
      break;
    case 26:
      display.print("Changeable, Mending");
      break;
    case 27:
      display.print("Rather Unsettled, Clearing Later");
      break;
    case 28:
      display.print("Unsettled, Probably Improving");
      break;
    case 29:
      display.print("Unsettled, Short Fine Intervals");
      break;
    case 30:
      display.print("Very Unsettled, Finer at Times");
      break;
    case 31:
      display.print("Stormy, Possibly Improving");
      break;
    case 32:
      display.print("Stormy, Much Rain");
      break;
    default:
      display.print("Invalid...");
      break;
  }

  display.display();

  delay(2000);
  count_time = count_time+2;
  
}
