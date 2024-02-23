#define LED_01 10;
const int LED_02 = 12; 

void setup(){
  pinMode(LED_01, OUTPUT);
  pinMode(LED_02, OUTPUT);
}

void ligarLed(tmp){
  digitalWrite(LED_01, HIGH);
  digitalWrite(LED_02, LOW);
  delay(tmp*1000);
}

void loop(){
  ligarLed(1)
}
