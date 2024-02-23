int led_1 = 13
int led_2 = 12
void setup(){
  pinMode(led_1, OUTPUT)
  pinMode(led_2, OUTPUT)
}

void loop(){
  digitalWrite(led_1, HIGH)
  digitalWrite(led_2, LOW)
  
}
