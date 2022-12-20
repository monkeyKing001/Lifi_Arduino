#define LED_PIN 3
#define LDR_PIN A1
#define THRESHOLD 100 //IN DARKER ENVIRONMENT, HIGHER THRESHOLS (IN BOX -> 150, light off room -> 120)
#define BOX_THRESHOLD 120
#define INDOOR_THRESHOLD 100
#define SAMPLING_TIME 5

/*********************/
/***   global var  ***/
/*********************/
bool led_state = false;
bool previous_state = true;
bool current_state = true;
char buff[64];
char nl = '\n';
int threshold = INDOOR_THRESHOLD;


/*****************************************************************************/
/***************************        main        ******************************/
/*****************************************************************************/
void setup() 
{
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  //change_threshold(BOX_THRESHOLD);
  change_threshold(INDOOR_THRESHOLD);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, get_ldr());
  current_state = get_ldr(); 
  if(!current_state && previous_state)
  {
    sprintf(buff, "%c", get_byte());
    Serial.print(buff);
  }
  digitalWrite(LED_PIN, current_state);
  previous_state = current_state;
}

void  change_threshold(int ENV) { threshold = ENV; }

bool get_ldr()
{
  bool val = analogRead(LDR_PIN) < threshold ? true : false;
  digitalWrite(LED_PIN, val);
  return val;
  //int voltage = analogRead(LDR_PIN);
  //return voltage  > THRESHOLD ? 8 : 0;
}

char get_byte()
{
  char data_byte = 0;
  delay(SAMPLING_TIME * 1.5); //1.5 work well checked.
  for(int i = 0; i < 8; i++)
  {
    data_byte = data_byte | (char)get_ldr() << i;
    delay(SAMPLING_TIME);
  }
  return data_byte;
}