#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define addr  0x27
#define cols   20
#define lines    4

LiquidCrystal_I2C lcd(addr, cols, lines);

const byte r = 4;
const byte c = 4;

byte PinsR[r] = {2, 3, 4, 5};
byte PinsC[c] = {6, 7, 8, 9};

char keys[r][c] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), PinsR, PinsC, r, c);

const int numAlunos = 13;

String alunos[numAlunos][2] = {
  {"1", "Ana Carolina"},
  {"2", "Arthur"},
  {"3", "Bárbara"},
  {"4", "Bruno"},
  {"5", "Carol"},
  {"6", "Flavia"},
  {"7", "Gabriel"},
  {"8", "Giovana F."},
  {"9", "Giovana S."},
  {"10", "Giovana V."},
  {"11", "Guilherme O."},
  {"A", "Prof. Antonio"},
  {"B", "Prof. Bruno"},
};

bool status = false;
int temp = 0;
String accessCode = "";

bool buttonState = false;

void setup() {
  lcd.init();
  
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Banheiro");
  lcd.setCursor(0, 1);
  lcd.print("Liberado");
  Serial.begin(9600);
  Serial.println("Acesso Liberado: nenhum aluno no banheiro.");
  
  pinMode(12, OUTPUT);
  pinMode(10, INPUT_PULLUP);
}

void loop() {
  char key = keypad.getKey();
  buttonState = digitalRead(10);

  if (key != NO_KEY) {
    if (key == '*') {
      if (status) {
        status = false;
        resetDisplay();
      } else {
        accessCode = "";
        resetDisplay();
      }
    } else if (!status) {
      if (key == '#') {
        if (accessCode.length() > 0) {
          Serial.println("Acesso Verificado...");
          verificarAcesso(accessCode);
        }
      } else {
        accessCode += key;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Liberando:");
        lcd.setCursor(0, 1);
        lcd.print(accessCode);
        Serial.println("Aguardando: Aguardando o aluno confirmar o número (#).");
      }
    }
  }

  if (status) {
    lcd.setCursor(0, 1);
    if (temp < 60) {
      lcd.print("Tempo: 00:" + zero(temp));
      digitalWrite(12, LOW);
    } else {
      int minutes = temp / 60;
      int seconds = temp % 60;
      lcd.print("Tempo: " + zero(minutes) + ":" + zero(seconds));

      if(temp > 20){
        digitalWrite(12, HIGH);
      }
    }
    delay(1000);
    temp += 1;
  }

  if (buttonState == LOW) {
    acionarBotao();
  }
}

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Banheiro:");
  lcd.setCursor(0, 1);
  lcd.print("Liberado");
  Serial.println("Acesso Liberado: nenhum aluno no banheiro.");
  digitalWrite(12, LOW);
}

String zero(int num) {
  if (num < 10) {
    return "0" + (String)num;
  } else {
    return (String)num;
  }
}

void verificarAcesso(String code) {
  for (int i = 0; i < numAlunos; i++) {
    if (alunos[i][0] == code) {
      status = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aguarde:");
      lcd.setCursor(9, 0);
      lcd.print(alunos[i][1]);
      Serial.println("Acesso bloqueado: há um aluno no banheiro - " + alunos[i][1]);
      temp = 0;
      accessCode = "";
      return;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Código:");
  lcd.setCursor(0, 1);
  lcd.print("Inválido");
  Serial.println("Código inválido: o código inserido não está cadastrado em nosso sistema.");
  delay(3000);
  resetDisplay();
  accessCode = "";
}

void acionarBotao() {
  if (status) {
    status = false;
    resetDisplay();
    Serial.println("Botão pressionado: Acesso resetado.");
  } else {
    Serial.println("Botão pressionado: Nenhum aluno no banheiro.");
  }
}
