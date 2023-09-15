#include <NecDecoder.h>
uint8_t r_pwm;
uint8_t g_pwm;
uint8_t b_pwm;
NecDecoder ir;
uint8_t color = 128;
unsigned long time;
unsigned long time_wave;
unsigned long change_time;
float k = 0.5;
void setup() {
  
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  attachInterrupt(0, irIsr, FALLING);
}

void loop() {

    if (ir.available()){
      switch (ir.readCommand()){
        case 0x10: 
          color++;
          makeColor(color);
          makeBright(k);
          break;
        case 0x5A: 
          color--; 
          makeColor(color);
          makeBright(k);
          break;
        case 0x18:
          k = k + 0.05;
          if (k > 1.0){
            k = 0.05;
          }
          makeBright(k);   
          break;
        case 0x4A:
          k = k - 0.05;
          if (k < 0.05){
            k = 1.0;
          }
          makeBright(k); 
          break;
        case 0xA2: r_pwm = 0; g_pwm = 0; b_pwm = 255;makeBright(k);break;
        case 0x62: r_pwm = 0; g_pwm = 255; b_pwm = 0;makeBright(k);break;
        case 0xE2: r_pwm = 255; g_pwm = 0; b_pwm = 0;makeBright(k);break;
        case 0x22: r_pwm = 255; g_pwm = 255; b_pwm = 255;makeBright(k);break;
        case 0x2: r_pwm = 0; g_pwm = 255; b_pwm = 255;makeBright(k);break;
        case 0xC2: r_pwm = 255; g_pwm = 0; b_pwm = 255;makeBright(k) ;break;
        case 0xE0: r_pwm = 255; g_pwm = 255; b_pwm = 0; makeBright(k);break; 
        case 0xB0: r_pwm = 0; g_pwm = 0; b_pwm = 0;makeBright(k);break; 
        case 0xA8: wave(); break;
        case 0x90: flashing(); break;
        case 0x98: change_color(color, k); break;
      }
    }
    
}
void irIsr() {
  ir.tick();
}
void makeColor(uint8_t color){
  uint8_t shift = 0; 
  checkColor(color);                             
  if (color > 170) {                              
    shift = (color - 170) * 3;                    
    r_pwm = shift, g_pwm = 0, b_pwm = ~shift;     
  } else if (color > 85) {                       
    shift = (color - 85) * 3;                     
    r_pwm = 0, g_pwm = ~shift, b_pwm = shift;     
  } else {                                        
    shift = (color - 0) * 3;                      
    r_pwm = ~shift, g_pwm = shift, b_pwm = 0;     
  }
  analogWrite(3, r_pwm);
  analogWrite(5, g_pwm);
  analogWrite(6, b_pwm);
}
void checkColor(uint8_t color){
  if (color > 255){
    color = 1;
  }
  else if (color < 1){
    color = 255;
  }
}
void makeBright(float brig) {                   
  uint8_t r_bright = r_pwm*brig;
  uint8_t g_bright = g_pwm*brig;
  uint8_t b_bright = b_pwm*brig;
  analogWrite(3, r_bright);
  analogWrite(5, g_bright);
  analogWrite(6, b_bright);
}
void wave(){
  Serial.println("wave");
  byte i;
  while (true){
    if (ir.available()){
      if (ir.readCommand() == 0xA8){
          uint8_t r_bright = r_pwm*k;
          uint8_t g_bright = g_pwm*k;
          uint8_t b_bright = b_pwm*k;
          analogWrite(3, r_bright);
          analogWrite(5, g_bright);
          analogWrite(6, b_bright);
          break;        
      }
    }
    for (float i = 0; i <=100; i = i + 5){
        Serial.println(i);
        uint8_t r_bright = r_pwm*(i/100);
        uint8_t g_bright = g_pwm*(i/100);
        uint8_t b_bright = b_pwm*(i/100);
        analogWrite(3, r_bright);
        analogWrite(5, g_bright);
        analogWrite(6, b_bright);
    }
    for (float i = 100; i >=0; i = i - 5){
        Serial.println(i);

        uint8_t r_bright = r_pwm*(i/100);
        uint8_t g_bright = g_pwm*(i/100);
        uint8_t b_bright = b_pwm*(i/100);
        analogWrite(3, r_bright);
        analogWrite(5, g_bright);
        analogWrite(6, b_bright);
    }
  }
}
void flashing(){
  bool led = true;
  while (true){

    if (ir.available()){
      if (ir.readCommand() == 0x90){
          uint8_t r_bright = r_pwm*k;
          uint8_t g_bright = g_pwm*k;
          uint8_t b_bright = b_pwm*k;
          analogWrite(3, r_bright);
          analogWrite(5, g_bright);
          analogWrite(6, b_bright);
          break;        
      }
    }
    if (millis() - time > 500){
      
      time = millis();
      led = !led;
      if (led){
        uint8_t r_bright = r_pwm*k;
        uint8_t g_bright = g_pwm*k;
        uint8_t b_bright = b_pwm*k;
        analogWrite(3, r_bright);
        analogWrite(5, g_bright);
        analogWrite(6, b_bright);
      }
      else{
        analogWrite(3, 0);
        analogWrite(5, 0);
        analogWrite(6, 0);
      }
    }
  }  
}
void change_color(uint8_t color, float k){
  uint8_t shift = 0;  
  while (true){

    if (ir.available()){
      if (ir.readCommand() == 0x98){
          uint8_t r_bright = r_pwm*k;
          uint8_t g_bright = g_pwm*k;
          uint8_t b_bright = b_pwm*k;
          analogWrite(3, r_bright);   
          analogWrite(5, g_bright);
          analogWrite(6, b_bright);
          break;        
      }
    }
    if (millis() - change_time > 50){
      color++;
      change_time = millis();
    }                           
    if (color > 170) {                              
      shift = (color - 170) * 3;                    
      r_pwm = shift-, g_pwm = 0, b_pwm = ~shift;     
    } else if (color > 85) {                        
      shift = (color - 85) * 3;                     
      r_pwm = 0, g_pwm = ~shift, b_pwm = shift;     
    } else {                                        
      shift = (color - 0) * 3;                      
      r_pwm = ~shift, g_pwm = shift, b_pwm = 0;     // Получаем компоненты RGB
    }
    uint8_t r_bright = r_pwm*k;
    uint8_t g_bright = g_pwm*k;
    uint8_t b_bright = b_pwm*k;
    analogWrite(3, r_bright);
    analogWrite(5, g_bright);
    analogWrite(6, b_bright);
    
  }
}
