

// # Connection:
// #        M1 pin  -> Digital pin 4
// #        E1 pin  -> Digital pin 5
// #        M2 pin  -> Digital pin 7
// #        E2 pin  -> Digital pin 6
// #        Motor Power Supply -> Centor blue screw connector(5.08mm 3p connector)
// #        Motor A  ->  Screw terminal close to E1 driver pin
// #        Motor B  ->  Screw terminal close to E2 driver pin
// #


int E1 = 6;
int M1 = 7;
int E2 = 5;
int M2 = 4;

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    
}

void loop()
{
  int value=255;
    digitalWrite(M1,HIGH);
    digitalWrite(M2,LOW); 
    analogWrite(E1, value);   //PWM Speed Control
    analogWrite(E2, value);   //PWM Speed Control
    delay(3000);
    analogWrite(E1, 0);   //PWM Speed Control
    analogWrite(E2, 0);
    delay(3000);
}
