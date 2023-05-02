//******************************************************************************** PQ Training Kit, ORION Space *********************************************************************************************
//******************************************************************************** Morse Using RFM26W           *********************************************************************************************
// This program is an example to test morse using RFM26W transceiver module
// This program is based on the example to encode/decode morse created by: Matthias Esterl.

#include <SPI.h>
#include<EEPROM.h>
#include <RH_RF24.h>

RH_RF24 rf24;
#define UNIT_LENGTH  60

RH_RF24::ModemConfig OOKAsync = { 
0x89, 0x00, 0xc3, 0x50, 0x01, 0x00, 0x00, 0x00, 0x00, 0x34,
0x10, 0x00, 0x3f, 0x08, 0x31, 0x27, 0x04, 0x10, 0x02, 0x12,
0x00, 0x2c, 0x03, 0xf9, 0x62, 0x11, 0x0e, 0x0e, 0x00, 0x02,
0xff, 0xff, 0x00, 0x27, 0x00, 0x00, 0x07, 0xff, 0x40, 0xcc,
0xa1, 0x30, 0xa0, 0x21, 0xd1, 0xb9, 0xc9, 0xea, 0x05, 0x12,
0x11, 0x0a, 0x04, 0x15, 0xfc, 0x03, 0x00, 0xcc, 0xa1, 0x30,
0xa0, 0x21, 0xd1, 0xb9, 0xc9, 0xea, 0x05, 0x12, 0x11, 0x0a,
0x04, 0x15, 0xfc, 0x03, 0x00, 0x3f, 0x2c, 0x0e, 0x04, 0x0c,
0x73 };


int GPIO0=7;
int SDN_Pin=9;

static const struct {const byte mapBit, *code;} MorseMapModified[]=
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};

static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};
String morseWord;

void setup() 
{
  pinMode(GPIO0,OUTPUT);
  pinMode(SDN_Pin,OUTPUT);
 
  Serial.begin(9600);
 
  
  if (!rf24.init())
  {
    Serial.println("init failed");
   
    rf24.init();
  }
  rf24.setFrequency(434);
  rf24.setModemRegisters(&OOKAsync);
  rf24.setModeTx();   
}
void loop(){
  morseWord=encode("9N1JT"); 

  for(int i=0; i<=morseWord.length(); i++)
  {
    switch( morseWord[i] )
    {
      case '.': //dit
        
        Serial.println("Dit");
        digitalWrite(GPIO0,HIGH);
        rf24.setModeTx();
        delay( UNIT_LENGTH );
        digitalWrite(GPIO0,LOW);
        rf24.setModeTx();
        delay( UNIT_LENGTH );
        break;

      case '-': //dah
        digitalWrite(GPIO0,HIGH);
        rf24.setModeTx();
        Serial.println("Dah");
        delay( UNIT_LENGTH*3 );
        digitalWrite(GPIO0,LOW);
        rf24.setModeTx();
        delay( UNIT_LENGTH );
        break;

      case ' ': //gap
        delay( UNIT_LENGTH );
    }
  }
}

String encode(const char *string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }

  return morseWord;  
}

String decode(String morse)
{
  String msg = "";
  
  int lastPos = 0;
  int pos = morse.indexOf(' ');
  while( lastPos <= morse.lastIndexOf(' ') )
  {    
    for( int i = 0; i < sizeof MorseMap / sizeof *MorseMap; ++i )
    {
      if( morse.substring(lastPos, pos) == MorseMap[i].code )
      {
        msg += MorseMap[i].letter;
      }
    }

    lastPos = pos+1;
    pos = morse.indexOf(' ', lastPos);
    
    // Handle white-spaces between words (7 spaces)
    while( morse[lastPos] == ' ' && morse[pos+1] == ' ' )
    {
      pos ++;
    }
  }

  return msg;
}
