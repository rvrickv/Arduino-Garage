#include <Arduino.h>
#include <elapsedMillis.h>
#include <EEPROM.h>

#include "FastLED.h"
#include "NewPing.h"
//#include "QuickStats.h"

#define DATA_PIN D3       // Data pin for FastLED LED control
#define TRIGGER D5        // Pin used to trigger HC-SR04
#define ECHO D0           // Pin that receives unltrasound ping
#define LED_TYPE NEOPIXEL // Type of LED pixels for FastLED
#define NUM_LEDS 16       // Number of LEDs being used
#define BRIGHTNESS 180    // How brignt will LESs appear.
#define PARKING_DISTANCE 100  // Default parking distance
#define MAX_DISTANCE 350      // Sensor is less precise at longer distances
#define RESET_DISTANCE 5      // Distance that will trigger parking distance setting
#define SAMPLE_READINGS 5     // Number of readings to increase precision
#define TIME_TO_OFF 10000     // How long before LEDs go off

CRGB leds[NUM_LEDS];
elapsedMillis timeElapsed;
//QuickStats stats;

bool parked = false;
NewPing sensor(TRIGGER, ECHO, MAX_DISTANCE);

uint16_t parking_distance = PARKING_DISTANCE;
float readings[SAMPLE_READINGS];

void setup() {
  // Small delay to let eveerything get started
  delay(1000);

  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  EEPROM.begin(2);
  Serial.begin(9600);
  Serial.println("Started");
  led_off();

  // Check to see if we have parking data in the eeprom
  uint16_t ee_distance = 0;

  // Query the parking distance from flash. If not 0, set
  // parking_distance to stored value.
  EEPROM.get(0,ee_distance);
  if (ee_distance){ parking_distance = ee_distance; Serial.println(parking_distance);}
}


void loop() {

  uint16_t distance = 0;

  // Mode sampling technique - slower but reduces outliers
  // Take n readings, store them in the array
  //for (int i=0; i<SAMPLE_READINGS; i++){
    //readings[i]  = round_to_base(sensor.convert_cm(sensor.ping_median()));
    /*Serial.print("Reading: ");
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(readings[i]);
    */
  //}
  // now extract the mode to eliminate outliers
  //distance =  uint16_t(stats.mode(readings,SAMPLE_READINGS));
  //Serial.print("Mode result is: ");

  // No mode sampling approach
  distance = round_to_base(sensor.convert_cm(sensor.ping_median()));

  //Serial.println(distance);

  if (distance <= 1)  // Nothing going on. Show nothing
  {
    led_off();
    timeElapsed = 0;
    parked = false;
  }
  else if (distance <= RESET_DISTANCE)  //Change parking distance
  {
    led_off();
    timeElapsed = 0;
    parked = false;
    led_enter_setup();
    //check to see if we're still within setup distance to make sure
    // user didn't change their mind
    distance = round_to_base(sensor.convert_cm(sensor.ping_median()));
    if (distance > 0 && distance <=RESET_DISTANCE)
    {
      parking_distance = reset_parking_distance();
      //Serial.print("New Parking distance is: ");
      //Serial.println(parking_distance);
    }
  }
  else if (distance <= parking_distance)
  {
      if ((!parked) || (timeElapsed < TIME_TO_OFF)){
        led_stop();
        parked = true;
      }
      else if(parked && (timeElapsed >= TIME_TO_OFF) && leds[0].getLuma())
      {
        led_fade_off(); //If lEDs are on after being parked for a few secs turn off
      }
  }
  else if (distance <= MAX_DISTANCE){
      led_show_value(distance);
      parked = false;
      timeElapsed = 0;
  }
  delay(10);
}

uint16_t reset_parking_distance(){

  led_countdown();
  uint16_t new_distance = round_to_base(sensor.convert_cm(sensor.ping_median(25)));
  new_distance += 5;  // a little buffer to ensure we're "Red"
  timeElapsed = 0;

  EEPROM.put(0,new_distance);  // write the new distance to Flash
  EEPROM.commit();

  return (new_distance);
}

uint16_t round_to_base(uint16_t value)
{
  uint16_t base = 5;
  return uint16_t(base * round(float(value)/base));
}
