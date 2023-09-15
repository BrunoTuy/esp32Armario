#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Pin definitions for ESP32
#define SCLK 22
#define MOSI 23
#define CS 4
#define DC 2
#define RST 15

#define TELA_PRINCIPAL 0
#define TELA_SOLE_TANQUE 1
#define TELA_RELE_200A 2
#define TELA_CC 3

#define pTanqueA 13
#define pRele200a 12
#define pTanqueB 14
#define pReleCC 27
#define pTravaA 26
#define pTravaB 25
#define pBt1 35
#define pBt2 33
#define pAgua 21
#define pGas 19

int stTanqueA = LOW;
int stTanqueB = LOW;
int stRele200a = LOW;
int stReleCC = LOW;
int stGas = HIGH;
int stAgua = LOW;
int stIgnicao = LOW;
int stMotor = LOW;

int bt1last = HIGH;
int bt2last = HIGH;

int bt1mili = 0;
int bt2mili = 0;

int loopLed = 0;
int loopBtn = 0;

int screen = -1;

Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, MOSI, SCLK, RST);

void setup() {
  pinMode(pTanqueA, OUTPUT);
  pinMode(pRele200a, OUTPUT);
  pinMode(pTanqueB, OUTPUT);
  pinMode(pReleCC, OUTPUT);
  pinMode(pTravaA, OUTPUT);
  pinMode(pTravaB, OUTPUT);
  pinMode(pBt1, INPUT);
  pinMode(pBt2, INPUT);
  pinMode(pAgua, INPUT);
  pinMode(pGas, INPUT);

  tft.initR(INITR_MINI160x80_PLUGIN);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(25,40);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(2);
  tft.println("El Patron");
}

int pressBotao(int bt, int *lastStatus, int *btmilis) {
  int status = digitalRead(bt);

  if (status != *lastStatus) {
    *lastStatus = status;
  
    if (status == HIGH) {
      return millis() - *btmilis;
    }

    *btmilis = millis();
  }

  return 0;
}

void leds() {
  digitalWrite(pTanqueA, stTanqueA);
  digitalWrite(pTanqueB, stTanqueB);
  digitalWrite(pRele200a, stRele200a);
  digitalWrite(pReleCC, stReleCC);
}

void tela(int newScreen) {
  if (newScreen > TELA_CC || newScreen < TELA_PRINCIPAL) {
    screen = TELA_PRINCIPAL;
  } else {
    screen = newScreen;
  }

  tft.setTextSize(2);

  if (screen == TELA_PRINCIPAL) {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(3,5);

    tft.setTextColor(ST77XX_WHITE);
    tft.print("Agua ");
    if (stTanqueA == HIGH && stTanqueB == HIGH) {
      tft.setTextColor(ST77XX_ORANGE);
    } else if (stTanqueA == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("Mot ");

    if (stTanqueA == HIGH && stTanqueB == HIGH) {
      tft.setTextColor(ST77XX_ORANGE);
    } else if (stTanqueB == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("Pas ");

    tft.setCursor(3, 20);
    if (stRele200a == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("200a ");

    if (stReleCC == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("Corta");

    tft.setCursor(3, 35);
    if (stAgua == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("Bomba ");

    if (stGas == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else {
      tft.setTextColor(ST77XX_RED);
    }
    tft.print("Gas");

    tft.setCursor(3, 50);
    if (stMotor == HIGH) {
      tft.setTextColor(ST77XX_GREEN);
    } else if (stIgnicao) {
      tft.setTextColor(ST77XX_ORANGE);
    } else {
      tft.setTextColor(ST77XX_WHITE);
    }
    tft.print("Motor");

  } else if (screen == TELA_SOLE_TANQUE) {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(3,20);
    tft.println("Tanque aberto");

    if (stTanqueA == HIGH && stTanqueB == HIGH) {
      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_ORANGE);
      tft.println("Ambos");
    } else if (stTanqueA == HIGH) {
      tft.setCursor(30,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Motorista");
    } else if (stTanqueB == HIGH) {
      tft.setCursor(25,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Passageiro");
    } else {
      tft.setCursor(45,55);
      tft.setTextColor(ST77XX_RED);
      tft.println("Nenhum");
    }
    
  } else if (screen == TELA_RELE_200A) {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(10,20);
    tft.println("Casa<->Carro");

    if (stRele200a == HIGH) {
      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Ligado");
    } else {
    tft.setCursor(40,55);
      tft.setTextColor(ST77XX_RED);
      tft.println("Deligado");
    }
    
  } else if (screen == TELA_CC) {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(80,20);
    tft.println("CC");

    if (stReleCC == HIGH) {
      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Ligado");
    } else {
    tft.setCursor(40,55);
      tft.setTextColor(ST77XX_RED);
      tft.println("Deligado");
    }
  }

  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.setTextColor(ST77XX_CYAN);
  tft.println(screen);
}

void botoes(int bt1, int bt2) {
  if (bt1 > 0) {
    if (bt1 > 5000) {
      tela(TELA_CC);
    } else if (screen == TELA_RELE_200A) {
      tela(0);
    } else {
      tela(screen + 1);
    }
  } else if (bt2 > 0) {
    if (screen == TELA_SOLE_TANQUE) {
      if (stTanqueA == LOW && stTanqueB == LOW) {
        stTanqueA = HIGH;
        stTanqueB = LOW;
      } else if (stTanqueA == HIGH && stTanqueB == HIGH) {
        stTanqueA = LOW;
        stTanqueB = LOW;
      } else if (stTanqueA == HIGH) {
        stTanqueA = LOW;
        stTanqueB = HIGH;
      } else {
        stTanqueA = HIGH;
        stTanqueB = HIGH;        
      }

      tela(TELA_SOLE_TANQUE);
    } else if (screen == TELA_RELE_200A) {
      if (stRele200a == HIGH) {
        stRele200a = LOW;
      } else {
        stRele200a = HIGH;
      }

      tela(TELA_RELE_200A);
    } else if (screen == TELA_CC) {
      if (bt2 > 2000 && stReleCC == LOW) {
        stReleCC = HIGH;
      } else {
        stReleCC = LOW;
      }

      tela(TELA_CC);
    } else {
      tela(screen - 1);
    }
  }
}

bool atualizaRele(int pin, int *status) {
  int newStatus = digitalRead(pin);

  if (newStatus != *status) {
    *status = newStatus;

    return true;
  }

  return false;
} 

void loop() {
  int tempoBt1 = 0;
  int tempoBt2 = 0;

  if (millis() - loopBtn > 50) {
    tempoBt1 = pressBotao(pBt1, &bt1last, &bt1mili);
    tempoBt2 = pressBotao(pBt2, &bt2last, &bt2mili);
    loopBtn = millis();
  }

  if ((atualizaRele(pAgua, &stAgua) || atualizaRele(pGas, &stGas)) && screen == TELA_PRINCIPAL) {
    tela(TELA_PRINCIPAL);
  }

  botoes(tempoBt1, tempoBt2);
  leds();
}
