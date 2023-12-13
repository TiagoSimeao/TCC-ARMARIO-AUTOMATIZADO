#include <Keypad.h> 
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"

int pinoSS = 53;

RTC_DS3231 rtc;
DateTime now;

#define FECHADURA_PIN     28
#define LED_VERDE_PIN     32
#define LED_VERMELHO_PIN  34
#define BUZZER_PIN        26
#define GRAVOU            30

int senhaErrada = 0;
int segundos = 30;
bool contador = false;

const byte ROWS = 4;
const byte COLS = 4;

bool acessoLiberado = false;
int usuarioIndex = -1;

const int FERRAMENTA_1 = 36;
const int FERRAMENTA_2 = 38;
const int FERRAMENTA_3 = 40;
const int PORTAA = 44;

String ultimaMensagem1;
String ultimaMensagem2;
String ultimaMensagem3;

bool varredura = false;

bool devolveu1 = false;
bool devolveu2 = false;
bool devolveu3 = false;

bool invasor = false;

bool menu = false;
bool menuErro = false;

String nomezinho = "";


char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {A7, A6, A5, A4};
byte colPins[COLS] = {A3, A2, A1, A0};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Correspondência correta entre senhas e nomes de usuário
struct User {
  char senha[5];
  char nome[16];
};

User users[] = {
  {"1104", "Luan"},
  {"ABCD", "Tiago"},
  {"5678", "Samuel"},
  {"0000", "Otavio"},
  {"1234", "Rafael"}
};


LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  Serial.begin(9600);
  //rtc.adjust(DateTime(2023, 10, 24, 10, 37, 0)); //ajuste hora
  lcd.begin(20, 4);
  lcd.clear();
  pinMode(FECHADURA_PIN, OUTPUT);
  pinMode(LED_VERDE_PIN, OUTPUT);
  pinMode(LED_VERMELHO_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(pinoSS, OUTPUT);
  digitalWrite(FECHADURA_PIN, HIGH);
  pinMode(FERRAMENTA_1, INPUT_PULLUP);
  pinMode(FERRAMENTA_2, INPUT_PULLUP);
  pinMode(FERRAMENTA_3, INPUT_PULLUP);
  pinMode(PORTAA, INPUT_PULLUP);
  pinMode(GRAVOU, OUTPUT);


  if (SD.begin(pinoSS)) { // Inicializa o SD Card
    Serial.println("SD Card pronto para uso"); // Imprime na tela
  }
  else {
    Serial.println("Falha na inicialização do SD Card");
    return;
  }
}


void loop() {

  if (senhaErrada >= 3 && menuErro == false) {
    contador = true;
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Muitas tentativas!");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Aguarde:");

  }

  while (contador == true) {
    segundos--;
    lcd.setCursor(9, 1);
    lcd.print("00:");
    if (segundos < 10) {
      lcd.print("0");
    }
    lcd.print(segundos);
    delay(1000);

    if (segundos == 0) {
      lcd.clear();
      contador = false;
      segundos = 30;
    }
  }

  if (acessoLiberado == true) {
    senhaErrada = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Porta aberta!");
    digitalWrite(LED_VERDE_PIN, HIGH);
    digitalWrite(FECHADURA_PIN, LOW);
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Retire!");
    delay(1000);
    acessoLiberado = false;
    usuarioIndex = -1;
  }

  while (varredura == true) {
    int estadoBotao1 = digitalRead(FERRAMENTA_1);
    int estadoBotao2 = digitalRead(FERRAMENTA_2);
    int estadoBotao3 = digitalRead(FERRAMENTA_3);
    int estadoBotao4 = digitalRead(PORTAA);

    if (estadoBotao1 == HIGH) {
      if (devolveu1 == true) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("Retirou 1!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem1 = " Retirou 1";
        ultimaMensagem1 = nomezinho + mensagem1;
        File file = SD.open("Tool1.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem1);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu1 = false;
        }
      }
    }

    if (estadoBotao1 == LOW) {
      if (devolveu1 == false) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("devolveu 1!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem1 = " Devolveu 1";
        ultimaMensagem1 = nomezinho + mensagem1;
        File file = SD.open("Tool1.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem1);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu1 = true;
        }
      }
    }

    if (estadoBotao2 == HIGH) {
      if (devolveu2 == true) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("Retirou 2!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem2 = " Retirou 2";
        ultimaMensagem2 = nomezinho + mensagem2;
        File file = SD.open("Tool2.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem2);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu2 = false;
        }
      }
    }

    if (estadoBotao2 == LOW) {
      if (devolveu2 == false) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("devolveu 2!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem2 = " Devolveu 2";
        ultimaMensagem2 = nomezinho + mensagem2;
        File file = SD.open("Tool2.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem2);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu2 = true;
        }
      }
    }

    if (estadoBotao3 == HIGH) {
      if (devolveu3 == true) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("Retirou 3!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem3 = " Retirou 3";
        ultimaMensagem3 = nomezinho + mensagem3;
        File file = SD.open("Tool3.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem3);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu3 = false;
        }
      }
    }

    if (estadoBotao3 == LOW) {
      if (devolveu3 == false) {
        digitalWrite(GRAVOU, HIGH);
        delay(500);
        Serial.println("devolveu 3!");
        delay(500);
        digitalWrite(GRAVOU, LOW);
        String mensagem3 = " Devolveu 3";
        ultimaMensagem3 = nomezinho + mensagem3;
        File file = SD.open("Tool3.txt", FILE_WRITE);
        now = rtc.now();
        if (file) {
          file.println(nomezinho + mensagem3);
          file.print("Data: ");
          file.print(now.day(), DEC);
          file.print("/");
          file.print(now.month(), DEC);
          file.println(" ");
          file.print("Hora: ");
          file.print(now.hour(), DEC);
          file.print(":");
          file.print(now.minute(), DEC);
          file.println(" ");
          file.println(" ");
          file.close();
          devolveu3 = true;
        }
      }
    }

    if (estadoBotao4 == LOW) {
      digitalWrite(LED_VERDE_PIN, LOW);
      digitalWrite(FECHADURA_PIN, HIGH);
      lcd.clear();
      varredura = false;
    }
  }


  if (checkSenha()) {
    usuarioIndex = obterUsuarioIndex();
  }
}

bool checkSenha() {

  char senha[5];
  int senhaIndex = 0;
  char customKey;
  bool registro = true;
  String ferramentasSelecionadas = "";  // String para armazenar as teclas pressionadas


  while (invasor == true && acessoLiberado == false) {   // caso haja um invasor (ainda não terminei)
    lcd.setCursor(6, 0);
    lcd.print("Aviso!!!");
    lcd.setCursor(2, 1);
    lcd.print("Sem Autorizar!!!");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_VERMELHO_PIN, HIGH);
  }
  if (invasor == false) {
    lcd.clear();
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_VERMELHO_PIN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Digite a senha:");
    lcd.setCursor(0, 3);
    lcd.print("* - MENU");
    lcd.setCursor(0, 1);
  }


  while (senhaIndex < 4) {
    customKey = myKeypad.getKey();

    if (registro == true) {
      if (customKey) {
        senha[senhaIndex] = customKey;
        senhaIndex++;
        lcd.print("*");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
      }
    }

    if (customKey == '*') {
      registro = false;
      menuErro = true;
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Qual Ferramenta?");
      lcd.setCursor(4, 1);
      lcd.print("(1) (2) (3)");
      lcd.setCursor(10, 3);
      lcd.print("# - VOLTAR");

      while (true) {
        customKey = myKeypad.getKey();
        if (customKey && customKey != '*') {
          ferramentasSelecionadas += customKey;
        }


        if (customKey == '1') {
          lcd.setCursor(4, 1);
          if (menu == false) {
            lcd.print("(X) (2) (3)");
          }
          delay(1000);
          menu = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Registro atual:");
          lcd.setCursor(0, 1);
          lcd.print(ultimaMensagem1);
          lcd.setCursor(10, 3);
          lcd.print("# - VOLTAR");
          lcd.setCursor(0, 3);
          lcd.print("(2) (3)");

        }
        else if (customKey == '2') {
          lcd.setCursor(4, 1);
          if (menu == false) {
            lcd.print("(1) (X) (3)");
          }
          delay(1000);
          menu = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Registro atual:");
          lcd.setCursor(0, 1);
          lcd.print(ultimaMensagem2);
          lcd.setCursor(10, 3);
          lcd.print("# - VOLTAR");
          lcd.setCursor(0, 3);
          lcd.print("(1) (3)");
        }
        else if (customKey == '3') {
          lcd.setCursor(4, 1);
          if (menu == false) {
            lcd.print("(1) (2) (X)");
          }
          delay(1000);
          menu = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Registro atual:");
          lcd.setCursor(0, 1);
          lcd.print(ultimaMensagem3);
          lcd.setCursor(10, 3);
          lcd.print("# - VOLTAR");
          lcd.setCursor(0, 3);
          lcd.print("(1) (2)");
        }
        else if (customKey == '#') {
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          menu = false;
          lcd.print("Digite sua senha:");
          return true;
        }
      }
    }
  }

  senha[4] = '\0';

  for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) {
    if (strcmp(senha, users[i].senha) == 0) {
      displayWelcomeMessage(i);
      acessoLiberado = true;
      senhaErrada = 0;
      varredura = true;
      return true;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Senha incorreta");
  delay(500);
  digitalWrite(LED_VERMELHO_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_VERMELHO_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  acessoLiberado = false;
  senhaErrada += 1;
  menuErro = false;
  return false;
}

int obterUsuarioIndex() {
  for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) {
    if (strcmp(users[i].senha, users[usuarioIndex].senha) == 0) {
      return i;
    }
  }
  return -1;
}

void displayWelcomeMessage(int userID) {
  File dataFile = SD.open("LOG.txt", FILE_WRITE);
  now = rtc.now();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem-vindo, ");
  lcd.setCursor(0, 1);
  lcd.print("Usuario: " + String(users[userID].nome));
  nomezinho = (String(users[userID].nome));

  if (dataFile) {
    dataFile.print("Usuario: " + String(users[userID].nome));
    dataFile.println(" ");
    dataFile.print("Data: ");
    dataFile.print(now.day(), DEC);
    dataFile.print("/");
    dataFile.print(now.month(), DEC);
    dataFile.print("/");
    dataFile.print(now.year(), DEC);
    dataFile.println(" ");
    dataFile.print("Hora: ");
    dataFile.print(now.hour(), DEC);
    dataFile.print(":");
    dataFile.print(now.minute(), DEC);
    dataFile.println(" ");
    dataFile.println(" ");
    dataFile.close();
  }
  else {
    Serial.println("Erro ao abrir o arquivo");
  }
  displayDateTime();
  delay(4000);
  lcd.clear();
}

void displayDateTime() {
  now = rtc.now(); // Obtenha a data e a hora atual do RTC
  lcd.setCursor(0, 2); // Defina a posição para a quarta linha do LCD
  lcd.print("Data: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 3);
  lcd.print("Hora: ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
}