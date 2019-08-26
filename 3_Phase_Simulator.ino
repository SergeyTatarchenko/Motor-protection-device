
/* Arduino UNO  3 phase source simulator (inverse) */

#define VOLTAGE_A 1
#define CURRENT_A 2

#define VOLTAGE_B 3
#define CURRENT_B 4

#define VOLTAGE_C 5
#define CURRENT_C 6


#define SetHigh(pin)   digitalWrite(pin,LOW);
#define SetLow(pin)    digitalWrite(pin,HIGH);

void loop() 

{
  /*phase A - current shift 1 ms ~ 18 %*/
  SetHigh(VOLTAGE_A);
  delay(1);
  SetHigh(CURRENT_A);
  delay(5);

  /*phase B - current shift 2 ms ~ 36 %*/
  SetHigh(VOLTAGE_B);
  delay(2);
  SetHigh(CURRENT_B);
  delay(4);

  /*phase C - current shift 1 ms ~ 18 %*/
  SetHigh(VOLTAGE_C);
  delay(1);
  SetHigh(CURRENT_C);

  /*restart sequence*/
  delay(1);
  restart();
  delay(4);
  
}

void restart(void);

void restart()
{
  SetLow(VOLTAGE_A);
  SetLow(CURRENT_A);

  SetLow(VOLTAGE_B);
  SetLow(CURRENT_B);
    
  SetLow(VOLTAGE_C);
  SetLow(CURRENT_C);
  
}

void setup() {
  
  pinMode(VOLTAGE_A,OUTPUT);
  pinMode(CURRENT_A,OUTPUT);
  
  pinMode(VOLTAGE_B,OUTPUT);
  pinMode(CURRENT_B,OUTPUT);
  
  pinMode(VOLTAGE_C,OUTPUT);
  pinMode(CURRENT_C,OUTPUT);

  SetLow(VOLTAGE_A);
  SetLow(CURRENT_A);

  SetLow(VOLTAGE_B);
  SetLow(CURRENT_B);
    
  SetLow(VOLTAGE_C);
  SetLow(CURRENT_C);
  
}
