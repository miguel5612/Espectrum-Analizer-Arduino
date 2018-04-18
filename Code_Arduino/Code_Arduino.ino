/* 

 for Arduino IDE 1.0.6  http://yulie-wu.blogspot.tw/2017/04/arduino-240930-mhz-spectrum-analyser.html
 2018-02-23 debug and 變更參數範圍 改外接天線,預防漏接 偵測CapTime時間內最大的收訊值 
 2017-03-11 add frequency pos 
 2017-03-09 add memory fn & Battery  volt & cus
 2017-03-04 onCase test OK 
 2016-12-23 by yulie~
 */

#include <EEPROM.h>
#include <SPI.h>
//#include <SD.h>
//const int chipSelect = 17;

#include "U8glib.h"
int incomingByte = 0;  // 用來儲存收進來的 data byte
//U8GLIB_PCD8544 u8g(9, 8, 6, 7, 5);    // CLK ,DIN ,CE ,DC ,RST    //for test
U8GLIB_PCD8544 u8g(5, 6, 8, 7, 9);  // CLK ,DIN ,CE ,DC ,RST    //Case OnBoard

#define LINE_MAX 17 
uint8_t line_buf[LINE_MAX] = "U8GLIB Console";
uint8_t line_pos = 0;
// setup a text screen to support scrolling
#define ROW_MAX 6
uint8_t screen[ROW_MAX][LINE_MAX];
uint8_t rows, cols;
uint8_t rssi,keepRssi;
unsigned long t,u,v,y,x;
// line height, which matches the selected font (5x7)
#define LINE_PIXEL_HEIGHT 7
#include <RF22.h>
// Singleton instance of the radio
RF22 rf22;
byte Spectrum_gain = 4; // 預設訊號位準
byte Spectrum_base = 6; // 預設雜訊位準
byte sigDet_gain = 4;   // 預設訊號位準
byte sigDet_base = 16;  // 預設雜訊位準
float sigDet_Frequency = 432.0;
float sigDet_Frequency_1 = 432.0;


byte cus  = 4; // 預設畫面尺規位置 
byte pos  = 40; // 預設 freq target位置
byte j=0;
byte bar=82;//偵測數
byte barW=1;//長條寬度
byte barG=0;//長條圖間隔
byte barT=30;//偵測間隔mS
byte rsi[84];//陣列數量
//int spt[84];//頻譜陣列數

//volatile int encoderPos = 0; // variables changed within interrupts are volatile

String fnStr="spt";
float fScn  = 432.0;          //Step Frequency /MHz
//float fScn1 = 0;
// float fScn = 0.00f;   //Variable to store data read from EEPROM.
//頻譜參數
float fStep=1.0;          //Step Frequency /MHz
float fSta=400.0;         //Start Frequency  /Mhz
float fEnd= fSta + (84 * fStep); 
byte fn,oldFn,sl,sl_1,oldsl = 0;
word CapTime = 200 ;
byte runOne = 0;
//boolean SwMu,SwDn,SwUp = 1;
boolean SwFn = 1;//　不知為何無法整批指定
boolean SwMu = 1;//　不知為何無法整批指定
boolean SwSl = 1;
boolean SwDn = 1;
boolean SwUp = 1;
word oscill = 500 ;

byte val,old_val,count;
byte readrssi;


//rssi 18--> -118dB//rssi 230 --> -2dB
void setup(void) {
  Serial.begin(115200);
  analogReference(INTERNAL);
  pinMode(A0,INPUT_PULLUP);//14 A0 Fn
  pinMode(A1,INPUT_PULLUP);//15 A1 slect
  pinMode(A2,INPUT_PULLUP);//16 A2 up
  pinMode(A3,INPUT_PULLUP);//17 A3 down
  digitalWrite(9,LOW);  
  // digitalWrite(14,LOW);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  rf22.setFrequency(432.0);  // rf22.setModemConfig(RF22::FSK_PN9_Rb2Fd5);
  rf22.setModeRx();  
  delayMicroseconds(200); // Wait for freq to settle    
  for(int i = 0 ; i <= bar ; i++ ){    
    rsi[i]=2;   
  }

  Serial.println("setup ok");
  //    attachInterrupt(1, irRec, HIGH); // encoder pin on interrupt 0 (pin 2)
  //   u8g.setRot180(); for test
       u8g.firstPage(); do {      
       u8g.setFont(u8g_font_6x10);
       u8g.setPrintPos( 0, 7); 
       u8g.print("Easy Spectrum ");
       u8g.setPrintPos( 0, 17); 
       u8g.print("240~930 MHz");
       u8g.setPrintPos( 0, 25); 
       u8g.print("ver 2018-02-24");
       u8g.setPrintPos( 0, 35); 
       u8g.print(" WU YUN CHANG");
       u8g.setPrintPos( 0, 45); 
       u8g.print("Battery:");
       u8g.print(analogRead(A6) * (1.1 / 1024) * 5.51745);
       u8g.print("V"); } 
      while( u8g.nextPage() );
////取得本地設備電池電壓值
// volt = (analogRead(A0) * (1.1 / 1024) * 5.51745) ; // 220K + 48.7k 分壓 , (220 + 48.7)/48.7=5.51745

 delay(5000);
 fn = EEPROM.read(0); 
 // EEPROM.get( eeAddress, fScn );
}

void loop(void) {
  //  if(digitalRead(A0) == 0 && digitalRead(A0) !=  SwMu ){ //　按住連續跑
  if( digitalRead(A0) !=  SwMu ){      //按住　連續跑
    fn++;  if(fn >= 4)fn=0;   SwMu != SwMu; //true 或 false(A0)
    delay(250);
    if( EEPROM.read(0) != fn ){ EEPROM.write(0, fn);}  //設定值改變才會執行 write 一次
  }

  if(millis() - u >= 5000){
    u=millis(); } 

  switch (fn) {
  case 0:{SwFnSpectrum(); Spectrum();} break;  //頻譜 點
  case 1:{SwFnSpectrum(); Spectrum(); } break;   //頻譜 線
  case 2:{SwFnsigDet(); sigDet(); } break; //顯示脈衝訊號 點 
  case 3:{SwFnsigDet(); sigDet(); } break; //顯示脈衝訊號 線
  case 4:{}
  }
}

//================================================================================================================
void Spectrum(){
      float f = fSta;
      fScn = fSta + ((fEnd-fSta)/2);  //for signal  Detect
      fEnd= fSta + (80 * fStep);
      int i=0;
      while( f < fEnd + 1){
        rf22.setFrequency(f);
        delayMicroseconds(1125);//825~1125
       // rsi[i]=(rf22.rssiRead()/gain) - base;
       // if(rsi[i] <= 1)rsi[i] = 1; //2017-03-06 add  
        rsi[i]=constrain(((rf22.rssiRead()/Spectrum_gain) - Spectrum_base), 1,40);
        //  keepRssi=max(keepRssi,rsi[i]);//取得陣列中最大值
        f += fStep;
        i++;
      }
      u8g.firstPage();  
      do {      
 //      u8g.setRot180();
        u8g.setFont(u8g_font_6x10);
        u8g.setPrintPos( 0, 7); 
        if(sl==0){u8g.print("Freq adj:");}
        if(sl==1){u8g.print("Step adj:");u8g.print(fStep);}
        if(sl==2){u8g.print("Gain adj:");u8g.print(Spectrum_gain);}
        if(sl==3){u8g.print("Base adj:");u8g.print(Spectrum_base);}
        if(sl==4){u8g.print("Cus  adj:");u8g.print(cus);}
        if(sl==5){
        u8g.print("Freq:");
        u8g.print(fSta + (pos * fStep),1); 
        u8g.print("MHz");
        for(int j=16;j<40;j+=2)u8g.drawPixel(pos, j ); // Y 軸虛線
      }
      if(sl != 5){
        u8g.setPrintPos( 0, 15); 
        u8g.print(""); 
        u8g.print(fSta,1); 
        u8g.print("~");
        u8g.print(fEnd,1); 
        u8g.print("MHz");}
        for(int j=0;j<84;j+=20)u8g.drawPixel(j, 18 + cus ); 
        for(int j=0;j<84;j+=10)u8g.drawPixel(j, 19 + cus ); 
       // for(int j=13;j<84;j+=20)u8g.drawPixel(j, 19 + cus );
       // u8g.drawFrame(2, 16 , 1, 32); 
       // u8g.drawPixel(0, 18 );u8g.drawPixel(20, 18 );u8g.drawPixel(40, 18 );u8g.drawPixel(60, 18 );u8g.drawPixel(80, 18 );
       // u8g.drawPixel(0, 19 );u8g.drawPixel(20, 19 );u8g.drawPixel(39, 19 );u8g.drawPixel(60, 19 );u8g.drawPixel(80, 19 );
       // u8g.drawPixel(10, 19 );u8g.drawPixel(30, 19 );u8g.drawPixel(50, 19 );u8g.drawPixel(70, 19 );
        if(fn == 0)for(byte i=0;i <= bar -2; i++)u8g.drawPixel(i, 48 - rsi[i]);  //點  時間軸強度(drawPixel　最快)
        if(fn == 1)for(byte i=0;i <= bar -2; i++)u8g.drawFrame((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);  //線 時間軸強度(drawFrame 快 )
        // u8g.drawBox((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);   //時間軸強度(drawBox 普)
        // u8g.drawLine( i, 48 - rsi[i], i, 48);  //時間軸強度(drawLine 慢)
       } 
      while( u8g.nextPage() );
    }

//================================================================================================================
void sigDet(){
  if(sigDet_Frequency != sigDet_Frequency_1){       //設定值改變才執行一次
    sigDet_Frequency_1 = sigDet_Frequency;
    rf22.setFrequency(sigDet_Frequency);
  }  
  if(millis() - x >= CapTime){
//    Serial.println(millis() - x);  //for show real CapTime 
    x = millis();
//-----------------------------------------------------------------------------------------------------------------
    if(CapTime <= 80)rsi[0]=constrain(((rf22.rssiRead()/sigDet_gain) - sigDet_base), 1,40);
    if(rsi[0] <= 2)rsi[0] = 2; //2017-03-06 add 
    for(int i = 0 ; i <= bar ; i++ ){ rsi[ bar - i] = rsi[bar - (i+1)]; }
      u8g.firstPage();  
      do {      
 //       u8g.setRot180();
        u8g.setFont(u8g_font_6x10);
        u8g.setPrintPos( 0, 7); 
        if(sl_1==0){ u8g.print("Freq adj:"); }
        if(sl_1==1){ u8g.print("Step adj:");u8g.print(fStep); }
        if(sl_1==2){ u8g.print("CapT adj:");u8g.print(CapTime); }
        if(sl_1==3){ u8g.print("Gain adj:");u8g.print(sigDet_gain); }
        if(sl_1==4){ u8g.print("Base adj:");u8g.print(sigDet_base); }
        u8g.setPrintPos( 0, 15); 
        u8g.print(" ");
        u8g.print(sigDet_Frequency,1);
        u8g.print("MHz");
        if(fn == 2)for(byte i=0;i <= bar; i++){u8g.drawPixel(i, 48 - rsi[i]);}  //點 時間軸強度
        if(fn == 3)for(byte i=0;i <= bar; i++){u8g.drawFrame((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);}  //線 時間軸強度
      } 
      while( u8g.nextPage() );
//-----------------------------------------------------------------------------------------------------------------
} 
  else {
    //rsi[0]=constrain(((rf22.rssiRead()/sigDet_gain) - sigDet_base), 1,40);
    readrssi=constrain(((rf22.rssiRead()/sigDet_gain) - sigDet_base), 1,40);  
    rsi[0] = max(rsi[0],readrssi);  //預防漏接 偵測CapTime時間內最大的收訊值 
    //Serial.println(rsi[0]);
  }  
}


//================================================================================================================
void SwFnSpectrum(){
       if( digitalRead(A1) !=  SwSl ){      //按住　連續跑
        sl++;     
        if(sl >= 6)sl=0; 
        SwSl != SwSl;                       //true 或 false
        delay(250);
      } 
      switch (sl) {
       case 0:{ //adj freq range
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fSta -= 10 * fStep; SwDn != SwDn; delay(1); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fSta += 10 * fStep; SwUp != SwUp; delay(1); }
            fSta = constrain(fSta, 240, 930); } break;
        
       case 1:{ //adj freq step
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fStep-=0.1;  SwDn != SwDn; delay(1);}
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fStep+=0.1; SwUp != SwUp; delay(1); }
            fStep = constrain(fStep, 0.1, 5.0);  } break;

       case 2:{//adj GAIN
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            Spectrum_gain++; SwDn != SwDn; delay(1); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            Spectrum_gain--; SwUp != SwUp; delay(1); }
            Spectrum_gain = constrain(Spectrum_gain, 1, 32);}  break;

      case 3:{//adj base
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            Spectrum_base++; SwDn != SwDn; delay(80); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            Spectrum_base--; SwUp != SwUp; delay(80); }
            Spectrum_base = constrain(Spectrum_base, 1, 64); }  break;
          
      case 4:{//adj cus
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            cus++; SwDn != SwDn; delay(5); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            cus--; SwUp != SwUp; delay(5); }
            cus = constrain(cus, 1, 30); }  break;
          
      case 5:{//adj pos
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            pos--; SwDn != SwDn; delay(2);  }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            pos++; SwUp != SwUp; delay(2);  }
            pos = constrain(pos, 0, 81); }  break;
      } 
}

//================================================================================================================
void SwFnsigDet(){
        if( digitalRead(A1) !=  SwSl ){      //按住　連續跑
        sl_1++;     
        if(sl_1 >= 5)sl_1=0; 
        SwSl != SwSl; //true 或 false
        delay(250);
      } 
      switch (sl_1) {
       case 0:{ //adj freq range
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            sigDet_Frequency -=  fStep; SwDn != SwDn; delay(200); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            sigDet_Frequency +=  fStep; SwUp != SwUp; delay(200); }
            sigDet_Frequency = constrain(sigDet_Frequency, 240, 960); } break;

       case 1:{ //adj freq step
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fStep-=0.1; SwDn != SwDn; delay(100);}
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fStep+=0.1; SwUp != SwUp; delay(100);}
            fStep = constrain(fStep, 0.1, 5.0);      }  break;
        
       case 2:{ //adj cap time
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   // 按住連續跑
            if(CapTime <= 10 && CapTime >= 1 ) CapTime-= 1;
            if(CapTime <= 100 && CapTime >= 20 ) CapTime-= 10;
            if(CapTime >= 200 )  CapTime-= 100;
            SwDn != SwDn;  delay(100); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    // 按住連續跑
            if(CapTime >= 100 )  CapTime+= 100;
            if(CapTime <= 100 && CapTime >= 10 ){CapTime+= 10;}
            if(CapTime <= 9 && CapTime >= 1 ) CapTime+= 1;
            SwUp != SwUp; delay(100); }
            CapTime = constrain(CapTime, 1, 5000); }  break;

       case 3:{//adj GAIN
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   // 按住連續跑
            sigDet_gain++; SwDn != SwDn; delay(100); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    // 按住連續跑
            sigDet_gain--; SwUp != SwUp; delay(100); }
            sigDet_gain = constrain(sigDet_gain, 1, 32);  } break;

       case 4:{//adj base
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){    // 按住連續跑+
            sigDet_base++; SwDn != SwDn; delay(80); }               // 連續速率
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    // 按住連續跑-
            sigDet_base--; SwUp != SwUp; delay(80); }               // 連續速率
            sigDet_base = constrain(sigDet_base, 1, 80);  } break;
      } 
}

//-----下列為前一個版本-------

/* 
 for Arduino IDE 1.0.6
 2017-03-11 add frequency pos 
 2017-03-09 add memory fn & Battery  volt & cus
 2017-03-04 onCase test OK 
 2016-12-23 by yulie~
 */
/* 

#include <EEPROM.h>
#include <SPI.h>
#include "U8glib.h"
U8GLIB_PCD8544 u8g(5, 6, 8, 7, 9);  // CLK ,DIN ,CE ,DC ,RST    //Case OnBoard

#define LINE_MAX 17 
uint8_t line_buf[LINE_MAX] = "U8GLIB Console";
uint8_t line_pos = 0;
// setup a text screen to support scrolling
#define ROW_MAX 6
uint8_t screen[ROW_MAX][LINE_MAX];
uint8_t rows, cols;
uint8_t rssi,keepRssi;
unsigned long t,u,v,w,x,y,IrCode;
// line height, which matches the selected font (5x7)
#define LINE_PIXEL_HEIGHT 7
#include <RF22.h>
// Singleton instance of the radio
RF22 rf22;
byte gain = 4; // 預設訊號位準
byte base = 6; // 預設雜訊位準
byte cus  = 4; // 預設畫面尺規位置 
byte pos  = 40; // 預設 freq target位置
byte j=0;
byte bar=82;//偵測數
byte barW=1;//長條寬度
byte barG=0;//長條圖間隔
byte barT=30;//偵測間隔mS
byte rsi[84];//陣列數量

String fnStr="spt";
float fScn  = 434.0;          //Step Frequency /MHz
float fScn1 = 0;
// float fScn = 0.00f;   //Variable to store data read from EEPROM.
//頻譜參數
float fStep=1.0;          //Step Frequency /MHz
float fSta=400.0;         //Start Frequency  /Mhz
float fEnd= fSta + (84 * fStep); 
byte fn,oldFn,sl,sl_1,oldsl = 0;
word CapTime = 50 ;
byte runOne = 0;
boolean SwFn = 1;
boolean SwMu = 1;
boolean SwSl = 1;
boolean SwDn = 1;
boolean SwUp = 1;
word oscill = 500 ;

byte val,old_val,count;

void setup(void) {
  Serial.begin(115200);
  analogReference(INTERNAL);
  pinMode(A0,INPUT_PULLUP);//14 A0 Fn
  pinMode(A1,INPUT_PULLUP);//15 A1 slect
  pinMode(A2,INPUT_PULLUP);//16 A2 up
  pinMode(A3,INPUT_PULLUP);//17 A3 down
  digitalWrite(9,LOW);  
  // digitalWrite(14,LOW);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  rf22.setFrequency(434.0);  // rf22.setModemConfig(RF22::FSK_PN9_Rb2Fd5);
  rf22.setModeRx();  
  delayMicroseconds(200); // Wait for freq to settle    
  for(int i = 0 ; i <= bar ; i++ ){    
    rsi[i]=2;   
  }

  Serial.println("setup ok");
  //    attachInterrupt(1, irRec, HIGH); // encoder pin on interrupt 0 (pin 2)
  //   u8g.setRot180(); for test
       u8g.firstPage(); do {      
       u8g.setFont(u8g_font_6x10);
       u8g.setPrintPos( 0, 7); 
       u8g.print("Easy Spectrum ");
       u8g.setPrintPos( 0, 17); 
       u8g.print("240~930 MHz");
       u8g.setPrintPos( 0, 27); 
       u8g.print("Maker");
       u8g.setPrintPos( 0, 35); 
       u8g.print(" WU YUN CHANG");
       u8g.setPrintPos( 0, 45); 
       u8g.print("Battery:");
       u8g.print(analogRead(A6) * (1.1 / 1024) * 5.51745);
       u8g.print("V"); } 
      while( u8g.nextPage() );
////取得本地設備電池電壓值
// volt = (analogRead(A0) * (1.1 / 1024) * 5.51745) ; // 220K + 48.7k 分壓 , (220 + 48.7)/48.7=5.51745

 delay(5000);
 fn = EEPROM.read(0); 
 // EEPROM.get( eeAddress, fScn );
}



void loop(void) {
  // irRec();
  //  if(digitalRead(A0) == 0 && digitalRead(A0) !=  SwMu ){ //　按住連續跑
  if( digitalRead(A0) !=  SwMu ){      //按住　連續跑
    fn++;  if(fn >= 4)fn=0;   SwMu != SwMu; //true 或 false(A0)
    delay(250);
    if( EEPROM.read(0) != fn ){ EEPROM.write(0, fn);}
  }

  if(millis() - u >= 5000){
    u=millis(); } 

  switch (fn) {
  case 0:{select(); Spectrum();} break; //頻譜
  case 1:{select(); Spectrum();} break; //顯示脈衝訊號  
  case 2:{select_1(); sigDet();  } break;
  case 3:{select_1(); sigDet();  } break;
  case 4:{
      //fSta =10;
      if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
        if(oscill >= 1001 ) oscill-= 100;
        if(oscill <= 1000 && oscill >= 201 ) oscill-= 20;
        if(oscill <= 200 && oscill >= 1 ) oscill-= 2;
        SwDn != SwDn; //true 或 false
        delay(2);
      }
      if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
        if(oscill >= 1001 ) oscill+= 100;
        if(oscill <= 1000 && oscill >= 201 ) oscill+= 20;
        if(oscill <= 200 && oscill >= 1 ) oscill+= 2;
        SwUp != SwUp; //true 或 false
        delay(2);
      }

      float f = fSta;
      fEnd= fSta + (84 * fStep);
      int i=0,j=0;
      while( f < fEnd){
        // rf22.setFrequency(f);
        delayMicroseconds(oscill);//825~1125
        rsi[i]=analogRead(A7)/24; 
        f ++;
        i++;
      }
      u8g.firstPage();  
      do {      
        u8g.setFont(u8g_font_6x10);
        if(y == 1) u8g.setPrintPos( 0, 7); 
        else{
          u8g.drawStr( 0, 7, "oscill");
          u8g.setPrintPos( 0, 15);     
        }
        u8g.print(""); 
        u8g.print(oscill); 
        //  u8g.print("~");
        // u8g.print(fEnd,1); 
        u8g.print(" microSec");
        for(byte i=0;i <= bar; i++){
          u8g.drawPixel(i, 48 - rsi[i]);  //時間軸強度(drawPixel　最快)
          // u8g.drawLine( i, 48 - rsi[i], i, 48);  //時間軸強度(drawLine 慢)
          //u8g.drawFrame((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);  //時間軸強度(drawFrame 快 )
          // u8g.drawBox((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);   //時間軸強度(drawBox 普)
        }
      } 
      while( u8g.nextPage() );
    }
    break;
  default:  delay(5);    //頻譜
  }
}
///*
void Spectrum(){
        float f = fSta;
      fScn = fSta + ((fEnd-fSta)/2);  //for signal  Detect
      fEnd= fSta + (80 * fStep);
      int i=0;
      while( f < fEnd + 1){
        rf22.setFrequency(f);
        delayMicroseconds(1125);//825~1125
       // rsi[i]=(rf22.rssiRead()/gain) - base;
       // if(rsi[i] <= 1)rsi[i] = 1; //2017-03-06 add  
        rsi[i]=constrain(((rf22.rssiRead()/gain) - base), 1,40);
        //  keepRssi=max(keepRssi,rsi[i]);//取得陣列中最大值
        f += fStep;
        i++;
      }
      u8g.firstPage();  
      do {      
 //      u8g.setRot180();
        u8g.setFont(u8g_font_6x10);
        u8g.setPrintPos( 0, 7); 
        if(sl==0){u8g.print("Freq adj:");}
        if(sl==1){u8g.print("Step adj:");u8g.print(fStep);}
        if(sl==2){u8g.print("Gain adj:");u8g.print(gain);}
        if(sl==3){u8g.print("Base adj:");u8g.print(base);}
        if(sl==4){u8g.print("Cus  adj:");u8g.print(cus);}
        if(sl==5){u8g.print("Freq:"); u8g.print(fSta + (pos * fStep),1); u8g.print("MHz");
        for(int j=16;j<40;j+=2)u8g.drawPixel(pos, j );}  // Y 軸虛線
      
      if(sl != 5){
        u8g.setPrintPos( 0, 15); 
        u8g.print("");  u8g.print(fSta,1);  u8g.print("~"); u8g.print(fEnd,1); u8g.print("MHz");}
        for(int j=0;j<84;j+=20)u8g.drawPixel(j, 18 + cus ); 
        for(int j=0;j<84;j+=10)u8g.drawPixel(j, 19 + cus ); 
       // for(int j=13;j<84;j+=20)u8g.drawPixel(j, 19 + cus );
       // u8g.drawFrame(2, 16 , 1, 32); 
       // u8g.drawPixel(0, 18 );u8g.drawPixel(20, 18 );u8g.drawPixel(40, 18 );u8g.drawPixel(60, 18 );u8g.drawPixel(80, 18 );
       // u8g.drawPixel(0, 19 );u8g.drawPixel(20, 19 );u8g.drawPixel(39, 19 );u8g.drawPixel(60, 19 );u8g.drawPixel(80, 19 );
       // u8g.drawPixel(10, 19 );u8g.drawPixel(30, 19 );u8g.drawPixel(50, 19 );u8g.drawPixel(70, 19 );
        if(fn == 0)for(byte i=0;i <= bar -2; i++)u8g.drawPixel(i, 48 - rsi[i]);  //時間軸強度(drawPixel　最快)
        if(fn == 1)for(byte i=0;i <= bar -2; i++)u8g.drawFrame((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);  //時間軸強度(drawFrame 快 )
        // u8g.drawBox((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);   //時間軸強度(drawBox 普)
        // u8g.drawLine( i, 48 - rsi[i], i, 48);  //時間軸強度(drawLine 慢)
       } 
      while( u8g.nextPage() );
    }

//  

void sigDet(){
      if(fScn != fScn1){ fScn1 = fScn;
        rf22.setFrequency(fScn1);
      }
      delay(CapTime);
      rsi[0]=constrain(((rf22.rssiRead()/gain) - base), 1,40);
     // rsi[0]=rf22.rssiRead()/gain - base;
      if(rsi[0] <= 2)rsi[0] = 2; //2017-03-06 add 
      for(int i = 0 ; i <= bar ; i++ ){
        rsi[ bar - i] = rsi[bar - (i+1)];
        }
      u8g.firstPage();  
      do {      
        u8g.setFont(u8g_font_6x10);
        u8g.setPrintPos( 0, 7); 
        if(sl_1==0){u8g.print("Freq adj:");}
        if(sl_1==1){u8g.print("Step adj:");u8g.print(fStep);}
        if(sl_1==2){u8g.print("CapT adj:");u8g.print(CapTime);}
        if(sl_1==3){u8g.print("Gain adj:");u8g.print(gain);}
        if(sl_1==4){u8g.print("Base adj:");u8g.print(base);}
        u8g.setPrintPos( 0, 15); 
        u8g.print(" ");
        u8g.print(fScn,1);
        u8g.print("MHz");
        if(fn == 2)for(byte i=0;i <= bar; i++){u8g.drawPixel(i, 48 - rsi[i]);}  //時間軸強度
        if(fn == 3)for(byte i=0;i <= bar; i++){u8g.drawFrame((barW + barG) * i, 48 - rsi[i], barW, rsi[i]);}  //時間軸強度
      } 
      while( u8g.nextPage() );
}

///*
void select(){
       if( digitalRead(A1) !=  SwSl ){      //按住　連續跑
        sl++;     
        if(sl >= 6)sl=0; 
        SwSl != SwSl; //true 或 false
        delay(250);
      } 
      switch (sl) {
        case 0:{ //adj freq range
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fSta -= 10 * fStep; SwDn != SwDn; delay(1); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fSta += 10 * fStep; SwUp != SwUp; delay(1); }
           fSta = constrain(fSta, 240, 930);        }    break;
        
      case 1:{ //adj freq step
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fStep-=0.1;  SwDn != SwDn; delay(1);}
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fStep+=0.1; SwUp != SwUp; delay(1); }
         fStep = constrain(fStep, 0.1, 5.0);    }        break;

        case 2:{//adj GAIN
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            gain++; SwDn != SwDn; delay(1); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            gain--; SwUp != SwUp; delay(1); }
          gain = constrain(gain, 1, 32);    }        break;

      case 3:{//adj base
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            base++; SwDn != SwDn; delay(5); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            base--; SwUp != SwUp; delay(5); }
          base = constrain(base, 1, 30);    }        break;
          
     case 4:{//adj cus
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            cus++; SwDn != SwDn; delay(5); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            cus--; SwUp != SwUp; delay(5); }
          base = constrain(base, 1, 30);   }        break;
          
    case 5:{//adj pos
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            pos--; SwDn != SwDn; delay(2);  }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            pos++; SwUp != SwUp; delay(2);  }
          pos = constrain(pos, 0, 81);      }        break;
      } 
}

void select_1(){
        if( digitalRead(A1) !=  SwSl ){      //按住　連續跑
        sl_1++;     
        if(sl_1 >= 5)sl_1=0; 
        SwSl != SwSl; //true 或 false
        delay(250);
      } 
      switch (sl_1) {
       case 0:{ //adj freq range
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fScn -=  fStep; SwDn != SwDn; delay(20); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fScn +=  fStep; SwUp != SwUp; delay(20); }
            fScn = constrain(fScn, 240, 960);        }        break;

       case 1:{ //adj freq step
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            fStep-=0.1; SwDn != SwDn; delay(50);}
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            fStep+=0.1; SwUp != SwUp; delay(50);}
          fStep = constrain(fStep, 0.1, 5.0);      }        break;
        
       case 2:{ //adj cap time
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            if(CapTime <= 10 && CapTime >= 1 ) CapTime-= 1;
            if(CapTime <= 100 && CapTime >= 20 ) CapTime-= 10;
            if(CapTime >= 200 )  CapTime-= 100;
            SwDn != SwDn;  delay(100); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            if(CapTime >= 100 )  CapTime+= 100;
            if(CapTime <= 100 && CapTime >= 10 ){CapTime+= 10;}
            if(CapTime <= 9 && CapTime >= 1 ) CapTime+= 1;
            SwUp != SwUp; delay(100); }
           CapTime = constrain(CapTime, 1, 2000); }      break;

       case 3:{//adj GAIN
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            gain++; SwDn != SwDn; delay(100); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            gain--; SwUp != SwUp; delay(100); }
          gain = constrain(gain, 1, 32);       }        break;

       case 4:{//adj base
          if(digitalRead(A3) == 0 && digitalRead(A3) !=  SwDn ){   //　按住連續跑
            base++; SwDn != SwDn; delay(100); }
          if(digitalRead(A2) == 0 && digitalRead(A2) !=  SwUp ){    //　按住連續跑
            base--; SwUp != SwUp; delay(100); }
          base = constrain(base, 1, 30);        }        break;
      } 
}




*/

































