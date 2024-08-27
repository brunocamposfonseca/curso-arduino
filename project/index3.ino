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

const int numAlunos = 34;

const char* alunos[][2] = {
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
  {"12", "Fernando"},
  {"13", "Alison"},
  {"14", "Angélica"},
  {"15", "Sara"},
  {"16", "Eduardo"},
  {"17", "Miguel"},
  {"18", "Calebe"},
  {"19", "Roberto"},
  {"20", "Rogério"},
  {"21", "Teste 01"},
  {"22", "Teste 02"},
  {"23", "Teste 03"},
  {"24", "Teste 04"},
  {"25", "Teste 05"},
  {"26", "Teste 06"},
  {"27", "Teste 07"},
  {"28", "Teste 08"},
  {"29", "Teste 09"},
  {"30", "Teste 10"},
  {"32", "Teste 11"},
  {"32", "Teste 12"},
  {"A", "Prof. Antonio"},
  {"B", "Prof. Bruno"},
};

bool status = false;
int temp = 0;
char accessCode[5] = "";  // Alterado para armazenar o código de acesso como uma string C

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
        accessCode[0] = '\0';  // Resetando o código de acesso
        resetDisplay();
      }
    } else if (!status) {
      if (key == '#') {
        if (strlen(accessCode) > 0) {
          Serial.println("Acesso Verificado...");
          verificarAcesso(accessCode);
        }
      } else {
        int len = strlen(accessCode);
        if (len < 4) {  // Evita overflow do array
          accessCode[len] = key;
          accessCode[len + 1] = '\0';
        }
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
      lcd.print("Tempo: 00:");
      lcd.print(zero(temp));
      digitalWrite(12, LOW);
    } else {
      int minutes = temp / 60;
      int seconds = temp % 60;
      lcd.print("Tempo: ");
      lcd.print(zero(minutes));
      lcd.print(":");
      lcd.print(zero(seconds));

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

const char* zero(int num) {
  static char str[3];  // Static para manter a string fora da função
  if (num < 10) {
    sprintf(str, "0%d", num);
  } else {
    sprintf(str, "%d", num);
  }
  return str;
}

void verificarAcesso(const char* code) {
  for (int i = 0; i < numAlunos; i++) {
    if (strcmp(alunos[i][0], code) == 0) {
      status = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aguarde:");
      lcd.setCursor(9, 0);
      lcd.print(alunos[i][1]);
      Serial.print("Acesso bloqueado: há um aluno no banheiro - ");
      Serial.println(alunos[i][1]);
      temp = 0;
      accessCode[0] = '\0';
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
  accessCode[0] = '\0';
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
