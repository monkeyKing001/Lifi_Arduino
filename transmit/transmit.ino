#define LED_PIN1 4
#define LED_PIN2 10
#define BUTTON_PIN A0
#define PERIOD 5
char* string = "I Love Han Jun in Communication Networks\n";
int string_length;
void setup() 
{
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  string_length = strlen(string);
  Serial.begin(9600);
}

void loop() 
{
  Serial.println("sending messages");
  for(int i = 0; i < string_length; i ++)
  {
    send_byte(string[i]);
  }
  Serial.println("sending completed");
  //delay(1000);
}

void send_byte(char my_byte)
{
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  delay(PERIOD);

  //transmission of bits
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(LED_PIN1, (my_byte&(0x01 << i))!=0 );
    digitalWrite(LED_PIN2, (my_byte&(0x01 << i))!=0 );
    delay(PERIOD);
  }

  digitalWrite(LED_PIN1, HIGH);
  digitalWrite(LED_PIN2, HIGH);
  delay(PERIOD);

}