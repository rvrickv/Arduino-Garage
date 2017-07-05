#include <Arduino.h>

void led_stop(){
  //FastLED.showColor(CRGB::Red);
  fill_solid(&leds[0], 16,CRGB::Red);
  FastLED.show();
}

void led_off(){
  //FastLED.showColor(CRGB::Black);
  fill_solid(&leds[0], 16,CRGB::Black);
  FastLED.show();
}

void led_show_value(uint16_t value){
  uint8_t no_of_leds = map(value,parking_distance,MAX_DISTANCE,16,0);

  for (uint8_t led = 0; led<=no_of_leds; led++){leds[led] = CRGB::Green;}
  for (uint8_t led = NUM_LEDS - 1; led > no_of_leds; led--){leds[led] = CRGB::Black;}

  FastLED.show();
}

void led_enter_setup()
{
    // Blink 3 times
    for (int hue = 90; hue<255; hue++){
    fill_rainbow( leds, NUM_LEDS, hue, 5);
    FastLED.show();
    delay(20);
    }
}

void led_countdown()
{
    fill_rainbow( leds, NUM_LEDS, 255, 5);
    FastLED.show();
     delay(300);

    for (int i = NUM_LEDS-1; i>=0; i--)
    {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(300);
    }
}

void led_fade_off(){
  for (int i = 0; i < 255; i++){
    for (int j = 0 ; j < NUM_LEDS; j++){leds[j].fadeToBlackBy(20);}
    FastLED.show();
    if (!leds[0].getLuma()){
      led_off();
      break;}
    FastLED.delay(30);
  }

}
