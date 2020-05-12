/***********************************************************************************************************************/
/* File Name    : 
/* Version      : 
/* Device(s)    :   Ardino nano
/*              :    
/* Description  :   REM >> nano
/* Creation Date: 
/***********************************************************************************************************************/

#include <SPI.h>
 
// SCK  : pin 13 (Serial ClocK)
// MISO : pin 12 (Master In Slave Out)
// MOSI : pin 11 (Master Out Slave In)
// SS   : pin 10 (Slave Select pin)

#define     MOSI    11
#define     MISO    12
#define     SCK     13
#define     SS      10

void    clkWait(void);
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/
void setup() {
  Serial.begin(19200);

#if 1
    pinMode(MOSI, OUTPUT);
    pinMode(SS, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MISO, INPUT);
#endif


   
#if 0    
  // SPIの初期化
  // ※自動的に「SCK、MOSI、SSのピンの動作はOUTPUT」となり「SSはHIGH」となる
  SPI.begin();
  // SPI転送モード
  // クロック位相(CPOL) = 0 クロック極性(CPHA) = 0
  SPI.setDataMode(SPI_MODE0);
  // SPI送受信用のビットオーダー(MSBFIRST)
  SPI.setBitOrder(MSBFIRST);
  // SPIクロック分周器 2MHz = 16MHz/8 
  SPI.setClockDivider(SPI_CLOCK_DIV8);  
  #endif
}
/***********************************************************************************************************************
* Function Name: 
* Description  : // アドレス0番地からnum番地までデータを読み込む
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/   
void    readDeta(uint16_t ax){ 
        
union   {
        uint16_t   comm;         
        
        struct  {
                uint8_t cl;
                uint8_t ch;
        }addr;
}eep;        
        
        uint16_t    cx;
        
        for (int i=0; i< ax; i++){
            digitalWrite(SS, HIGH); 
             
            eep.comm= i;                            // READ命令　+　アドレス
            eep.addr.ch &= 0x03;
            eep.addr.ch |= 0x18;
            
            eep.comm= eep.comm << 3;
                                        
            uint8_t b0 = SPI.transfer(eep.addr.ch);  
            uint8_t b1 = SPI.transfer(eep.addr.cl);
            uint8_t b2 =  SPI.transfer(0x00);
            uint8_t b3 =  SPI.transfer(0x00);         
            delay(5);

                
 
            // ビットがずれているので3byteを取得する
            //uint8_t b1 =  SPI.transfer(0x00);
            //uint8_t b2 =  SPI.transfer(0x00);
            //uint8_t b3 =  SPI.transfer(0x00); 
 
            // ビットを修正後、シリアルモニタへ出力

            
            Serial.print(i); Serial.print("addr(H) = ");
            Serial.println((b1 << 6) | (b2 >> 2));
            Serial.print(i); Serial.print("addr(L) = ");      
            Serial.println((b2 << 6) | (b3 >> 2));
            digitalWrite(SS, LOW); 
        }  
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if 1
void    at93c86read(uint16_t ax){
        
        byte    val;
        uint16_t    i=0;
        uint16_t    bx=0;
        uint32_t    red;
        
        union   {
            uint16_t   comm;         
            struct  {
                    uint8_t cl;
                    uint8_t ch;
            }addr;
        }eep;

                
        for(uint16_t al=0; al < ax ;al++,i++){
            digitalWrite(SS,HIGH);
                
            eep.comm= i;                            // READ命令　+　アドレス
            eep.addr.ch &= 0x03;
            eep.addr.ch |= 0x18;
            eep.comm= eep.comm << 3;
            
            for(byte k=0; k<13; k++){
                val= (eep.comm & 0x8000)? 1:0;
                eep.comm= eep.comm << 1;
                digitalWrite(MOSI,val);
                sck();
            }
            digitalWrite(SCK,LOW);                  //SCK ADJST
            red=0x0000;
            for(byte j=0; j<16;j++){
                sck();
                if(digitalRead(MISO)==1) red |= 0x0001;
                red=red<<1;
                digitalWrite(SCK,LOW);              //SCK ADJST
            }
            digitalWrite(SS,LOW);
            red=red>>1;
            Serial.print(i); Serial.print("addr(H) = ");
            Serial.println(highByte(red),HEX);
            Serial.print(i); Serial.print("addr(L) = ");      
            Serial.println(lowByte(red),HEX);
        }
}
#endif
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/
void    sck(){
        digitalWrite(SCK,HIGH);
        digitalWrite(SCK,LOW);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/
void    clkWait(){
         asm volatile(
        
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
            "nop \n"
        );
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/    
void loop() {

    //ewenComm();
    at93c86read(20);
    //writeDeta(20, 30);
    //readDeta(2);
    delay(500);
    while(1){       
    }
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/   
void    ewenComm(){    // 書き込みモードの設定
                        // ※Instruction Set(命令セット)は2byteで「ビットは右寄せ」です。
    digitalWrite(SS, HIGH); 
    // EWEN命令(書き込みの有効) 
    SPI.transfer(0b00000001);  
    SPI.transfer(0b00110000);    
 
    // EWDS命令(書き込みの無効→読み込み専用)    
    //SPI.transfer(0b00000001);  
    //SPI.transfer(0b00000000);    
    digitalWrite(SS, LOW); 
    delay(10);
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : Code
* Return Value : None
***********************************************************************************************************************/     
void    writeDeta(uint16_t ax, uint16_t bx){ 
    // アドレス0番地から5番地までデータを書き込む
    uint8_t count =1;
        
        for (int i=ax; i< bx; i++){
        digitalWrite(SS, HIGH); 
        // WRITE命令　+　アドレス
        SPI.transfer(0b00000001);  
        SPI.transfer(0b01000000 | i);    
 
        // 2byte書き込む
        SPI.transfer(count++);
        SPI.transfer(count++);    
        digitalWrite(SS, LOW);
        delay(10);  
        }
}
 
