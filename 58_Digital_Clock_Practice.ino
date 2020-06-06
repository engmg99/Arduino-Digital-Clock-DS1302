#include <virtuabotixRTC.h> //RTC Library
virtuabotixRTC myRTC(A0,A1,A2); // CLK,DATA,RST

const int dataPin  = 12;  // 74HC595 pin 8 DS
const int latchPin = 11;  // 74HC595 pin 9 STCP
const int clockPin = 9;   // 74HC595 pin 10 SHCP
const int digit0   = 7;   // 7-Segment pin D4 d1 to 7
const int digit1   = 6;   // 7-Segment pin D3 d2
const int digit2   = 5;   // 7-Segment pin D2 d3
const int digit3   = 4;   // 7-Segment pin D1 d4

int dp=3;//decimal pointer to pin 3 of arduino

int hour, minute;
int fnHour, snHour; 
int fnMinute,snMinute;

int period=1000;
unsigned long stime=0;

byte table[]= 
    {   0xbF,  // = 0
        0x86,  // = 1
        0xdb,  // = 2
        0xcF,  // = 3
        0xe6,  // = 4
        0xed,  // = 5
        0xfd,  // = 6
        0x87,  // = 7
        0xff,  // = 8
        0xe7,  // = 9
        0xf7,  // = A
        0xfc,  // = b
        0xb9,  // = C
        0xde,  // = d
        0xf9,  // = E
        0xf1,  // = F
        0x80   // blank
    };

byte controlDigits[] = { digit0, digit1, digit2, digit3 };  // pins to turn off & on digits
byte displayDigits[] = {0};// lsb to msb

void setup() {
  Serial.begin(9600);
// seconds, minutes, hours, day of the week, day of the month, month, year
myRTC.setDS1302Time(44, 07, 22, 3,03,6, 2020);  //Here you write your actual time/date as shown above 
//but remember to "comment/remove" this function once you're done
//The setup is done only one time and the module will continue counting it automatically
   pinMode(dp,OUTPUT); //Decimal Pointer
   pinMode(latchPin,OUTPUT);
   pinMode(clockPin,OUTPUT);
   pinMode(dataPin,OUTPUT);
   
   for (int x=0; x<4; x++)
   {
   pinMode(controlDigits[x],OUTPUT);
   digitalWrite(controlDigits[x],HIGH);  // Turns off the digit  
   }
}
void calTime()
{
myRTC.updateTime();
hour = myRTC.hours;                           // Multiplex hour from RTC
minute =myRTC.minutes;                       // Multiplex Minute from RTC
fnHour = hour / 10;      // First Number Hour
snHour = hour % 10;       // Second Number Hour
fnMinute = minute / 10;   // First Number Minute
snMinute = minute % 10; // Second Number Minute
}

void blink()
{
    if(digitalRead(dp)==0)
    {
      digitalWrite(dp,1);
    }
    else
    {
      digitalWrite(dp,0);
    }
}

void loop() {

for (int x=0; x<=4; x++)
{
        for (int j=0; j<4; j++)
        {
            digitalWrite(controlDigits[j],HIGH);    // turn off digits
        }
        digitalWrite(latchPin,LOW);
        if (bitRead(displayDigits[4],x)==1)
        {
            // raw byte value is sent to shift register
            shiftOut(dataPin,clockPin,MSBFIRST,displayDigits[x]);
        } else 
        {
            // table array value is sent to the shift register
            shiftOut(dataPin,clockPin,MSBFIRST,table[displayDigits[x]]);
        }
        
        digitalWrite(latchPin,HIGH);
        digitalWrite(controlDigits[x],LOW);   // turn on one digit
        delay(1);                              // 1 or 2 is ok
}

    if(millis()>period+stime)
    {
        stime=millis();
        blink();
    }
    
    calTime();
    displayDigits[0]=snMinute;
    displayDigits[1]=fnMinute;
    displayDigits[2]=snHour;  
    displayDigits[3]=fnHour;
  
}
