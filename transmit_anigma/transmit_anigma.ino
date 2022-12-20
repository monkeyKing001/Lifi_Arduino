#define LED_PIN1 4
#define LED_PIN2 10
#define BUTTON_PIN A0
#define PERIOD 5

/*********************/
/***   global var  ***/
/*********************/
char* string = "Everybody, shout out his name!! The Master of Network Communication, Han Jun!\n";
int string_length;
unsigned char ascii_anigma[256];
int ascii_num = 256;
//int ascii_num = 128;
char buff[200];


/*****************************************************************************/
/************************     set up ANIGMA        ***************************/
/*****************************************************************************/
char  encrypt_anigram(char input) { return (ascii_anigma[input]); }

void setup_anigma_table(void)
{
  randomSeed(2022);
  int rand_num = ascii_num;
  for (int i = 0; i < ascii_num; i++)
    ascii_anigma[i] = i;
  int temp, swap_temp;
      while (--rand_num >= 0)
  {
    temp = random(0, rand_num);
    swap_temp = ascii_anigma[rand_num];
    ascii_anigma[rand_num] = ascii_anigma[temp];
    ascii_anigma[temp] = swap_temp;
  }
}

void print_anigma_table(void)
{
  int row_num = ascii_num / 4;
  Serial.println("\n\n===============================     ASCII ANIGMA TABLE.    ===============================");
  for (int i = 0; i < row_num; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      sprintf(buff, "| index[%03d] => %03d   |", (j * row_num) + i, ascii_anigma[(j * row_num) + i]);
      Serial.print(buff);
    }
    Serial.println("");
  }
  Serial.println("========================================================================================");
  Serial.println("\n=========  Start sending message! =========\n");
}

/*****************************************************************************/
/***************************        main        ******************************/
/*****************************************************************************/
void setup() 
{
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  string_length = strlen(string);
  Serial.begin(9600);
  setup_anigma_table();
  print_anigma_table();
}

void loop() 
{
  Serial.println("sending messages");
  for(int i = 0; i < string_length; i ++)
  {
    send_byte(encrypt_anigram(string[i]));
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