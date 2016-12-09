#define FREQUENCY 60
#define INNERACTUATORMASK  0b00000001
#define OUTERACTUATORMASK  0b00000010
//62.5kHz 1
//#define SCALER 0x1; 
//7.8kHz 8
#define SCALER 0x2; 
//1.95kHz 64
//#define SCALER 0x3;
//977Hz 256
//#define SCALER 0x4;

//sin data curent index for channels
byte indexA,indexB,indexC = 0;
int OFFSETA = 0;
int OFFSETB = 86;
int OFFSETC = 171;
//0 to 255 sin curve of the frist 256 points of a 267 point sin curve

const PROGMEM byte  SIN_DATA[] = {
  128,  131,  134,  137,  140,  143,  146,  149,  152,  155,  158,  162,  165,  167,  170,  173,  
  176,  179,  182,  185,  188,  190,  193,  196,  198,  201,  203,  206,  208,  211,  213,  215,  
  218,  220,  222,  224,  226,  228,  230,  232,  234,  235,  237,  238,  240,  241,  243,  244,  
  245,  246,  248,  249,  250,  250,  251,  252,  253,  253,  254,  254,  254,  255,  255,  255,  
  255,  255,  255,  255,  254,  254,  254,  253,  253,  252,  251,  250,  250,  249,  248,  246,  
  245,  244,  243,  241,  240,  238,  237,  235,  234,  232,  230,  228,  226,  224,  222,  220,  
  218,  215,  213,  211,  208,  206,  203,  201,  198,  196,  193,  190,  188,  185,  182,  179,  
  176,  173,  170,  167,  165,  162,  158,  155,  152,  149,  146,  143,  140,  137,  134,  131,  
  128,  124,  121,  118,  115,  112,  109,  106,  103,  100,  97,   93,   90,   88,   85,   82, 
  79,   76,   73,   70,   67,   65,   62,   59,   57,   54,   52,   49,   47,   44,   42,   40, 
  37,   35,   33,   31,   29,   27,   25,   23,   21,   20,   18,   17,   15,   14,   12,   11, 
  10,   9,    7,    6,    5,    5,    4,    3,    2,    2,    1,    1,    1,    0,    0,    0,  
  0,    0,    0,    0,    1,    1,    1,    2,    2,    3,    4,    5,    5,    6,    7,    9,  
  10,   11,   12,   14,   15,   17,   18,   20,   21,   23,   25,   27,   29,   31,   33,   35, 
  37,   40,   42,   44,   47,   49,   52,   54,   57,   59,   62,   65,   67,   70,   73,   76, 
  79,   82,   85,   88,   90,   93,   97,   100,  103,  106,  109,  112,  115,  118,  121,  124};
/*
const byte SIN_DATA[] = {115, 118, 120, 123, 126, 129, 132, 134, 137, 140, 143, 145, 148, 151, 153,
156, 159, 161, 164, 166, 169, 171, 174, 176, 179, 181, 183, 185, 188, 190, 192, 194, 196, 198, 200,
202, 203, 205, 207, 209, 210, 212, 213, 215, 216, 217, 218, 220, 221, 222, 223, 224, 225, 225, 226,
227, 227, 228, 228, 229, 229, 229, 229, 229, 230, 229, 229, 229, 229, 229, 228, 228, 227, 227, 226,
225, 225, 224, 223, 222, 221, 220, 218, 217, 216, 215, 213, 212, 210, 209, 207, 205, 203, 202, 200,
198, 196, 194, 192, 190, 188, 185, 183, 181, 179, 176, 174, 171, 169, 166, 164, 161, 159, 156, 153,
151, 148, 145, 143, 140, 137, 134, 132, 129, 126, 123, 120, 118, 115, 112, 109, 106, 104, 101, 98,
95, 92, 90, 87, 84, 81, 79, 76, 73, 71, 68, 66, 63, 61, 58, 56, 53, 51, 49, 46, 44, 42, 40, 38, 36,
34, 32, 30, 28, 26, 24, 23, 21, 19, 18, 16, 15, 14, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 18, 19, 21,
23, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 49, 51, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76,
79, 81, 84, 87, 90, 92, 95, 98, 101, 104, 106, 109, 112};
*/

byte sinAlpha1[256],sinBeta1[256],*sinon1 = sinAlpha1,*sinoff1 = sinBeta1;
byte sinAlpha2[256],sinBeta2[256],*sinon2 = sinAlpha2,*sinoff2 = sinBeta2;

void init_SinWAVE() {
  
 pinMode(2,OUTPUT);  //PE4-OC3B,OUTER2
 pinMode(3,OUTPUT);  //PE5-OC3C,OUTER3
 pinMode(5,OUTPUT);  //PE3-OC3A,OUTER1
 pinMode(6,OUTPUT);  //PH3-OC4A,INNER1
 pinMode(7,OUTPUT);  //PH4-OC4B,INNER2
 pinMode(8,OUTPUT);  //PH5-OC4C,INNER3
  
noInterrupts();

memcpy_P(sinon1,SIN_DATA,256);
memcpy_P(sinon2,SIN_DATA,256);

//set to output
//DDRE |= 0x38;

//turn off timers
TCCR3B = 0x88; //noice Canceled, falling edge, WMG mode 5, timer stopped
TCCR4B = 0x88;
/*TCCRxB*/
/*  7    6     5     4     3    2    1    0 */
/* ICNC ICES   -   WGM3  WGM2  CS2  CS1  CS0 */
TCCR2B = 0x0;//non-forcing, CTC mode,timer stopped
TCCR5B = 0x0;//non-forcing, CTC mode,timer stopped
//timer 3 pwm,pins 5,3,2
TCCR3A = 0xA9; //timer 3, WMG mode 5, all channels fast PWM non-inverting
TCCR4A = 0xA9;
/*TCCRxA*/
/*  7      6     5     4     3     2    1    0 */
/* COMA1 COMA0 COMB1 COMB0 COMC1 COMC0 WGM1 WGM0*/
/* WGM5 FASTPWM 8bit, MAX 0xff, update OCR at bottom, TOV at top*/
TIMSK3 = 0x00;//no timer 3 interupts
TIMSK4 = 0x00;
//timer 2 interupt
TCCR2A = 0x2;//OC2 A and B discomnected, CTC mode
TCCR5A = 0x2;//OC2 A and B discomnected, CTC mode
OCR2A = (byte)round(2.0 * 16000000 / FREQUENCY / 64 / 256- 1);//set compair register A to match at 256 times FREQUENCY
OCR5A = (byte)round(2.0 * 16000000 / FREQUENCY / 64 / 256- 1);//set compair register A to match at 256 times FREQUENCY
TIMSK2 = 0x2;//fire inturupt on compair match A
TIMSK5 = 0x2;//fire inturupt on compair match A

interrupts();




}

void startSinWAVE(unsigned char target){

  if(target&&OUTERACTUATORMASK){
      TCCR3A = 0xA9; //timer 3, WMG mode 5, all channels fast PWM non-inverting
      
      TCNT2 = 0;//clear timer2
      TCNT3L = 0x0; // clear timer3
      
      OCR3AL = SIN_DATA[indexA = OFFSETA];//compair reg A
      OCR3BL = SIN_DATA[indexB = OFFSETB];//compair reg B
      OCR3CL = SIN_DATA[indexC = OFFSETC];//compair reg C
      
      TCCR3B = 0x88 | SCALER; //noice Canceled, falling edge, WMG mode 5, timer running with pre-scaler
      TCCR2B = 0x3;//non-forcing, CTC mode,timer running with 64 prescaler
  }
  if(target&&INNERACTUATORMASK){
      TCCR4A = 0xA9; //timer 3, WMG mode 5, all channels fast PWM non-inverting
      
      TCNT5 = 0;//clear timer2
      TCNT4L = 0x0; // clear timer3
      
      OCR4AL = SIN_DATA[indexA = OFFSETA];//compair reg A
      OCR4BL = SIN_DATA[indexB = OFFSETB];//compair reg B
      OCR4CL = SIN_DATA[indexC = OFFSETC];//compair reg C
      
      TCCR4B = 0x88 | SCALER; //noice Canceled, falling edge, WMG mode 5, timer running with pre-scaler
      TCCR5B = 0x3;//non-forcing, CTC mode,timer running with 64 prescaler
  }

}

void setFrequencySinWAVE(float f, unsigned char target){
  if(target&&OUTERACTUATORMASK){
    OCR2A = (byte)round(2 * 16000000 / f / 64 / 256- 1) ;//set compair register A to match at 256 times FREQUENCY    
  }
  if(target&&INNERACTUATORMASK){
    OCR5A = (byte)round(2 * 16000000 / f / 64 / 256- 1) ;//set compair register A to match at 256 times FREQUENCY    
  }

}

void setAmplitudeSinWAVE(float amp,unsigned char target){
  if(target&&OUTERACTUATORMASK){
    /* Outer Actuator*/
    memcpy_P(sinoff1,SIN_DATA,256);
    for(int i = 0; i < 256; ++i){
      sinoff1[i] = (byte)round(amp * sinoff1[i]);
    }
    byte* tempSwitch = sinon1;
    noInterrupts();
    sinon1 = sinoff1;
    interrupts();
    sinoff1 = tempSwitch;
  }
  if(target&&INNERACTUATORMASK){
    /* Inner Actuator*/
    memcpy_P(sinoff2,SIN_DATA,256);
    for(int i = 0; i < 256; ++i){
      sinoff2[i] = (byte)round(amp * sinoff2[i]);
    }
    byte* tempSwitch = sinon2;
    noInterrupts();
    sinon2 = sinoff2;
    interrupts();
    sinoff2 = tempSwitch;
  }

}

bool isSinWAVE(){
  return TCCR3B & 0x7;
}

void stopSinWAVE(unsigned char target){
      noInterrupts();
  if(target&&OUTERACTUATORMASK){
      PORTE &= 0xC7;
      TCCR3B = 0x88; //noice Canceled, falling edge, WMG mode 5, timer stopped
      TCCR2B = 0x3;//non-forcing, CTC mode,timer stopped
  }
  if(target&&INNERACTUATORMASK){
      PORTH &= 0xC7;
      TCCR4B = 0x88; //noice Canceled, falling edge, WMG mode 5, timer stopped
      TCCR5B = 0x3;//non-forcing, CTC mode,timer stopped
  }
      interrupts();
  
}

void plug(unsigned char target){
  byte temp=OFFSETB;
  noInterrupts();
  OFFSETB=OFFSETC;
  OFFSETC=temp;
  interrupts();
}

ISR(TIMER2_COMPA_vect){

  OCR3AL = sinon1[indexA++];//advance compair reg A
  OCR3BL = sinon1[indexB++];//advance compair reg B
  OCR3CL = sinon1[indexC++];//advance compair reg C
}

ISR(TIMER5_COMPA_vect){

  OCR4AL = sinon2[indexA++];//advance compair reg A
  OCR4BL = sinon2[indexB++];//advance compair reg B
  OCR4CL = sinon2[indexC++];//advance compair reg C
}