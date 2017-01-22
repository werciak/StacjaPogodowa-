

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#include <Wire.h>  // Comes with Arduino IDE
#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#include <DS3231.h>
#include <SFE_BMP180.h>
DS3231 clock;
RTCDateTime dt;


#define DHTTYPE           DHT11     // DHT 11 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

DHT_Unified dht(DHTPIN, DHTTYPE);
SFE_BMP180 pressure;

double baseline;

uint32_t delayMS;

void setup() {
 lcd.begin(20,4);  
  Serial.begin(9600); 
  dht.begin();
  clock.begin();
clock.setDateTime(__DATE__, __TIME__);

pressure.begin();



  
    
  lcd.print("Stacja Pogodowa  :)");
  lcd.setCursor(0,1);
  lcd.print("Wykonali:");
  lcd.setCursor(0,2);
  lcd.print("Weronika Kowalska");
  lcd.setCursor(0,3);
  lcd.print("Grupa 31INF-PSI-SP");
  delay(7000);
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  dht.humidity().getSensor(&sensor);
 
  delayMS = sensor.min_delay / 1000;

 
  
 

}

void loop() {
dt = clock.getDateTime();
  delay(delayMS);
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  lcd.setCursor(0,0);
  if (isnan(event.temperature)) {
    lcd.print("YY");
  }
  else {
   lcd.setCursor(0,0);
    lcd.print("Temperatura: ");
  lcd.print(event.temperature);
  
     lcd.print("*C");
     
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    lcd.setCursor(0,1);
    lcd.print("xx");
  }
  else {
 
   lcd.setCursor(0,1);
     lcd.print("Wilgotnosc: ");
     lcd.print(event.relative_humidity);
     lcd.print("%");
  }
  lcd.setCursor(0,4);
  lcd.print(dt.year);
  lcd.print("/");
  lcd.print(dt.month);
  lcd.print("/");
  lcd.print(dt.day);
  
  lcd.print("  ");
  lcd.print(dt.hour); 
  lcd.print(":");
  lcd.print(dt.minute);
   lcd.print(":");
  lcd.print(dt.second);
   baseline = getPressure();

  lcd.setCursor(0,2);
  lcd.print("Cisnienie:");
  lcd.print(baseline);
  lcd.print("hPa");
 
 delay(7);
}
double getPressure()
{
  char status;
  double T,P,p0,a;



  status = pressure.startTemperature();
  if (status != 0)
  {
  

    delay(status);

    

    status = pressure.getTemperature(T);
    if (status != 0)
    {
    

      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);



        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
