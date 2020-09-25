#include <SPI.h>

//int MISO = 12;
//int MOSI = 11;
//int SCK = 13;
//int SS = 10;

//int MPH = 2000000;
int LED = 5;

long int data_in[4]; 
long int data_out[] = {25,30,40,50}; // encoder positions
int out_pos = 0;
int j = 0;
int pos = 0;
int i = 0;

bool rec_dat = false;
bool full_dat = false;

byte test[] = {'s', 't', 'a', 'r'};
byte test2[] = {'d', 'o', 'n', 'e'};

union u_tag{
  byte b[4];
  int32_t ival;
}u;

union u_tag v;

SPISettings settings(250000, LSBFIRST, SPI_MODE0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  SPCR |=bit (SPE);
  pinMode(MISO,OUTPUT);

  Serial.print("Arduino is Online\n");
  SPI.attachInterrupt();
  pinMode(LED,OUTPUT);

}


ISR(SPI_STC_vect) {

  u.b[i] = SPDR;
  //Serial.println(SPDR);
  SPDR = v.b[i];
  i++;
  
  if(i>3){
    if (check(u.b,test) == true) {
      rec_dat = true;
      v.ival = data_out[pos];
      i = 0;
    }

    else if (check(u.b, test2) ==true){
      rec_dat = false;
      out_pos = 0;
      pos = 0;
      full_dat = true;
      i = 0;
    }
    else if (rec_dat == true){
      data_in[pos] = u.ival;
      pos++;
      v.ival = data_out[pos];
      i = 0;
    }

  i = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(full_dat == true){
    Serial.print("Data: [");
    for(int j = 0; j<4; j++){
          Serial.print(data_in[j]);
          Serial.print(" ") ;
    }
    
    Serial.print("]\n");
    if (data_in[0] > 100){
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