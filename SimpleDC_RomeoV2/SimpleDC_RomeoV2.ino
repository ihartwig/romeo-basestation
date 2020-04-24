
#define AIN_SWITCHES 0
#define AIN_M1_CUR 3
#define AIN_M2_CUR 1
#define PIN_M1_DIR 4
#define PIN_M1_PWM 5
#define PIN_M2_DIR 6
#define PIN_M2_PWM 7
#define PIN_DEBUG_LED 13
#define CYCLE_DELAY_MS 50
#define SPEED_INC_RATE 15
#define SPEED_MAX 255

#define KEY_UP 0
#define KEY_LEFT 1
#define KEY_DOWN 2
#define KEY_RIGHT 3
#define KEY_SELECT 4

char msgs[5][15] = {
  "Up Key OK    ", 
  "Left Key OK  ", 
  "Down Key OK  ", 
  "Right Key OK ", 
  "Select Key OK" };
char start_msg[15] = {
  "Start loop "};                    
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
int adc_m1_cur;
int adc_m2_cur;

int track_dir = 0;
int track_speed = 0; /* 0-255 */


void setup() { 
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
  Serial.begin(9600);
 
  /* Print that we made it here */
  Serial.println(start_msg); 
}


void loop() 
{
  adc_key_in = analogRead(AIN_SWITCHES);    // read the value from the sensor
  key = get_key(adc_key_in);    // convert into key press

  switch(key) {
    // increase speed
    case KEY_UP:
      if(track_speed <= (SPEED_MAX - SPEED_INC_RATE)) {
        track_speed += SPEED_INC_RATE;
      }
    break;

    // decrease speed
    case KEY_DOWN:
      if(track_speed >= (0 + SPEED_INC_RATE)) {
        track_speed -= SPEED_INC_RATE;
      }
    break;

    // set direction forward
    case KEY_LEFT:
      track_dir = 0;
    break;

    // set direction backwards
    case KEY_RIGHT:
      track_dir = 1;
    break;

    // emergency stop
    case KEY_SELECT:
      track_speed = 0;
    break;

    // ignore when no key pressed
    default:
    break;
  }

  // read raw motor currents
  adc_m1_cur = analogRead(AIN_M1_CUR);
  adc_m2_cur = analogRead(AIN_M2_CUR);

  // physical feedback
  analogWrite(PIN_DEBUG_LED, track_speed);
  // debug feedback
  Serial.print("Dir: ");
  Serial.print(track_dir);
  Serial.print(", Speed: ");
  Serial.print(track_speed);
  Serial.print(", M1: ");
  Serial.print(adc_m1_cur);
  Serial.print(", M2: ");
  Serial.print(adc_m2_cur);
  Serial.print(", Key: ");
  Serial.println(key);
  // set motor output
  digitalWrite(PIN_M1_DIR, track_dir);
  analogWrite(PIN_M1_PWM, track_speed);

  // wait for the next cycle
  delay(CYCLE_DELAY_MS);
}


// Convert ADC value to key number
int get_key(unsigned int input)
{   
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {  
      return k;  
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}
