/***************************************************************************
  Kod: chartoseg;
  Kod maqsadi: Atmega microcontrollerlarda shift register yordamiga segment displayga belgi (matn) yozish;
  Batafsil: https://github.com/con9799/chartoseg/
  Muallif: Manuchehr Usmonov;
  Web sayt: https://devcon.uz
  Telegram https://t.me/devcon

  ***************

  Segment va shift registerni ulash:
  Q0: A(10);
  Q1: B(9);
  Q2: C(7);
  Q3: D(5);
  Q4: E(4);
  Q5: F(2);
  Q6: G(1);
  Q7: DP(6);

***************************************************************************/

const int dataPin = 2; //74hc595 data (DS)
const int latchPin = 3; //74hc595 latch (STCP)
const int clockPin = 4; //74hc595 clock (SHCP)

//Segment diplsay turi
const char common = 'a';    // common anode
//const char common = 'c';    // common cathode

bool decPt = false, ncon = false;

char* alphabet[]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," "};

const byte segmentChars[43] =
{
  B11111100, B01100000, B11011010, B11110010,   // 0123
  B01100110, B10110110, B10111110, B11100000,   // 4567
  B11111110, B11110110, B11101110, B00111110,   // 89AB
  B00011010, B01111010, B10011110, B10001110,   // CDEF
  B10111110, B01101110, B01100000, B01111000,   // GHIJ
  B00011110, B00011100, B10101000, B11101100,   // KLMN
  B11111100, B11001110, B11100110, B11101110,   // OPQR
  B10110110, B01100010, B01111100, B00111000,   // STUV
  B01010100, B10010010, B01110110, B11011010,   // WXYZ
  
  B00000000, B10000000, B01000000, B00100000,   // SPACE, SPIN
  B00010000, B00001000, B00000100,
};

 
void setup() {
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  printToSeg(".");
}

void loop() {
  /* 
  //Serial datadan kerakli ma'lumotni o'qib ekranga chiqarish. 
  char str[4] = {'\0'};

  if (Serial.available()>=3){
    for (byte i=0; i<3; i++){
      str[i] = Serial.read();
    }
  }
  
  printToSeg(str);
  Serial.print(str);
  */
  
  //Tasodifiy belgilar ko'rsatish
  printToSeg(alphabet[random (0, 35)]);
}

void printToSeg(const char prstring[]){
  for(int i =0; i < strlen(prstring); i++ ) {
    if(ncon){decPt = false;ncon=false;continue;}
    if(String(prstring[i+1]) == "."){decPt = true;ncon=true;}
    
    char c[1] = {prstring[i]};
    int cindex =  getAlpIndex(c);
    byte bits = segmentChars[cindex] ;
    
    if (decPt) {
      bits = bits | B00000001;
    }
    
    updateSegment(bits);
    delay(120);
  }
}

void updateSegment(byte eightBits) {
  if (common == 'a') {
    eightBits = eightBits ^ B11111111;
  }
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits);
  digitalWrite(latchPin, HIGH);
}

int getAlpIndex( const char * key )
{
  for ( uint8_t i = 0; i < sizeof( alphabet ) / sizeof( char * ); i++ )
    if ( !strcmp( strupr(key), alphabet[i] ) )
      return i;
  return -1;
}
