#include <SPI.h>
 
// SCK  : pin 13 (Serial ClocK)
// MISO : pin 12 (Master In Slave Out)
// MOSI : pin 11 (Master Out Slave In)
// SS   : pin 10 (Slave Select pin)
 
void setup() {
  Serial.begin(9600); 
    
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
    
  // 書き込みモードの設定
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
 
  // アドレス0番地から5番地までデータを書き込む
  uint8_t count =1;
  for (int i=0; i< 6; i++){
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
 
  // アドレス0番地から5番地までデータを読み込む
  for (int i=0; i< 6; i++){
    digitalWrite(SS, HIGH); 
      // READ命令　+　アドレス
      SPI.transfer(0b00000001);  
      SPI.transfer(0b10000000 | i);         
      delay(5);    
 
      // ビットがずれているので3byteを取得する
      uint8_t b1 =  SPI.transfer(0x00);
      uint8_t b2 =  SPI.transfer(0x00);
      uint8_t b3 =  SPI.transfer(0x00); 
 
      // ビットを修正後、シリアルモニタへ出力
      Serial.print(i); Serial.print("番地(H) = ");
      Serial.println((b1 << 1) | (b2 >> 7));
      Serial.print(i); Serial.print("番地(L) = ");      
      Serial.println(((b2 & 0b01111111) << 1) | (b3 >> 7));
    digitalWrite(SS, LOW); 
  }  
}
 
void loop() {
}
 
 
