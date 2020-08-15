#include <SPI.h>

//int MISO = 12;
//int MOSI = 11;
//int SCK = 13;
//int SS = 10;

//int MPH = 2000000;
int LED = 5;


float data_in[5]; 
float data_out[5]; // encoder positions

int pos = 0;
int i = 0;


bool rec_dat = false;

bool full_dat = false;

byte test[] = {'s', 't', 'a', 'r'};
byte test2[] = {'d', 'o', 'n', 'e'};


SPISettings settings(1000000, LSBFIRST, SPI_MODE0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  SPCR |=bit (SPE);
  pinMode(MISO,OUTPUT);

  Serial.print("Arduino is Online\n");
  SPI.attachInterrupt();
  pinMode(LED,OUTPUT);
}

//
union u_tag{
  byte b[4];
  float fval;

}u;


ISR(SPI_STC_vect) {
  u.b[i] = SPDR;
  i++;

  if(i>3){
    //if(u.b[0] == test[0] && u.b[1] == test[1] && u.b[2] == test[2] && u.b[3] == test[3]){
    if (check(u.b,test) == true) {
      rec_dat = true;
      i = 0;
    }
    //else if (u.b[0] == test2[0] && u.b[1] == test2[1] && u.b[2] == test2[2] && u.b[3] == test2[3]){
    else if (check(u.b, test2) ==true){
      rec_dat = false;
      full_dat = true;
      i = 0;
    }
    else if (rec_dat == true){
      data_in[pos] = u.fval;
      pos++;
      i = 0;
    }
    i = 0;
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(full_dat == true){
    Serial.print("Data: [");
    for(int j = 0; j<pos; j++){
          Serial.print(data_in[j]);
          Serial.print(" ") ;
    }
    
    Serial.print("]\n");
    if (data_in[0] > 3.0){
      digitalWrite(LED,HIGH);
    }
    else digitalWrite(LED,LOW);
    full_dat = false;
    pos = 0;
  }
}

boolean check(byte *a, byte *b){
     for (int n=0;n<4;n++){
      if (a[n]!=b[n]) return false;
     }
     return true;
}
