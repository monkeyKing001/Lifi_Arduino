#define LED_PIN 3
#define LDR_PIN A1
#define THRESHOLD 100 //IN DARKER ENVIRONMENT, HIGHER THRESHOLS (IN BOX -> 150)
#define BOX_THRESHOLD 120
#define INDOOR_THRESHOLD 110
#define SAMPLING_TIME 5
#define ASCII_NUM 256

/*********************/
/***   global var  ***/
/*********************/
bool led_state = false;
bool previous_state = true;
bool current_state = true;
char buff[200];
char offset = 0;
int threshold = INDOOR_THRESHOLD;
int ascii_num = 256;
//int ascii_num = 128;
unsigned char ascii_anigma[ASCII_NUM];
unsigned char anigma_decoder[ASCII_NUM];


/*****************************************************************************/
/************************     set up ANIGMA        ***************************/
/*****************************************************************************/
void setup_anigma_table(void)
{
  randomSeed(2022);
  for (int i = 0; i < ascii_num; i++)
    ascii_anigma[i] = i;
  int temp, swap_temp;
  int rand_num = ascii_num;
  while (--rand_num >= 0)
  {
    temp = random(0, rand_num);
    swap_temp = ascii_anigma[rand_num];
    ascii_anigma[rand_num] = ascii_anigma[temp];
    ascii_anigma[temp] = swap_temp;
  }
}

void setup_anigma_decoder()
{
  for (int i = 0; i < ascii_num; i++)
    anigma_decoder[ascii_anigma[i]] = i;
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
}

void print_anigma_decoder(void)
{
  int row_num = ascii_num / 4;
  Serial.println("\n\n===============================     ASCII ANIGMA DECODER.    ===============================");
  for (int i = 0; i < row_num; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      sprintf(buff, "| index[%03d] => %03d   |", (j * row_num) + i, anigma_decoder[(j * row_num) + i]);
      Serial.print(buff);
    }
    Serial.println("");
  }
  Serial.println("========================================================================================");
  Serial.println("\n=========  Start receiving message! =========\n");
}

char  decrypt_anigma(unsigned char enc_char) {return (anigma_decoder[enc_char]); }

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
  setup_anigma_table();
  //print_anigma_table();
  setup_anigma_decoder();
  print_anigma_decoder();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, get_ldr());
  current_state = get_ldr(); 
  if(!current_state && previous_state)
  {
    sprintf(buff, "%c", (char)decrypt_anigma((unsigned char)get_byte()));
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

unsigned char get_byte()
{
  unsigned char data_byte = 0;//return byte with unsigned char type as its value will be the index of decoding table(0~255)
  delay(SAMPLING_TIME * 1.5); //1.5 work well checked.
  for(int i = 0; i < 8; i++)
  {
    data_byte = data_byte | (unsigned char)get_ldr() << i;
    delay(SAMPLING_TIME);
  }
  return (unsigned char)data_byte;
}