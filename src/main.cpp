#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "./libs/util.h"

#define RXp2 16
#define TXp2 17

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
#define TELA_COFRE 4
#define TELA_COFRE_DESTRAVA 5

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

#define getALL 86
#define setReleH 82
#define setReleL 114
#define setCCH 67
#define setCCL 99
#define setIgnicaoH 73
#define setIgnicaoL 105
#define setMotorH 77
#define setMotorL 109
#define setGasH 71
#define setGasL 103
#define setAguaH 65
#define setAguaL 97
#define setPositH 80
#define setPositL 112

int stTanqueA = LOW;
int stTanqueB = LOW;
int stRele200a = LOW;
int stReleCC = LOW;
int stGas = HIGH;
int stAgua = LOW;
int stIgnicao = LOW;
int stMotor = LOW;
int stAuxPositron = LOW;

int bt1last = HIGH;
int bt2last = HIGH;

int bt1mili = 0;
int bt2mili = 0;

int loopLed = 0;
int loopBtn = 0;
int loopSetSerial = 0;

int screen = -1;
int trava = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, MOSI, SCLK, RST);

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

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
  tft.setTextColor(ST7735_ORANGE);
  tft.setTextSize(2);
  tft.println("El Patron");
}

void telaPrincipal(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(3,5);
    tft.print("Agua Mot Pas");
    tft.setCursor(3, 20);
    tft.print("200a Corta");
    tft.setCursor(3, 35);
    tft.print("Bomba Gas");
    tft.setCursor(25, 40);
    tft.println("El Patron");
    tft.setCursor(3, 50);
    tft.print("Motor");

  } else {
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
  }
}

void telaSoleTanque(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(3,20);
    tft.println("Tanque aberto");
    tft.setCursor(30,55);
    tft.println("Motorista");
    tft.setCursor(25,55);
    tft.println("Passageiro");
    tft.setCursor(45,55);
    tft.println("Nenhum");
    tft.setCursor(55,55);
    tft.println("Ambos");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(3,20);
    tft.println("Tanque aberto");

    tft.setTextColor(ST77XX_BLACK);
    tft.setCursor(30,55);
    tft.println("Motorista");
    tft.setCursor(25,55);
    tft.println("Passageiro");
    tft.setCursor(45,55);
    tft.println("Nenhum");
    tft.setCursor(55,55);
    tft.println("Ambos");

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
  }
}

void telaRele200A(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(10,20);
    tft.println("Casa<->Carro");
    tft.setCursor(40,55);
    tft.println("Deligado");
    tft.setCursor(55,55);
    tft.println("Ligado");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(10,20);
    tft.println("Casa<->Carro");

    if (stRele200a == HIGH) {
      tft.setCursor(40,55);
      tft.setTextColor(ST77XX_BLACK);
      tft.println("Deligado");

      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Ligado");
    } else {
      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_BLACK);
      tft.println("Ligado");

      tft.setCursor(40,55);
      tft.setTextColor(ST77XX_RED);
      tft.println("Deligado");
    }
  }
}

void telaCC(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(80,20);
    tft.println("CC");
    tft.setCursor(40,55);
    tft.println("Deligado");
    tft.setCursor(55,55);
    tft.println("Ligado");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(80,20);
    tft.println("CC");

    if (stReleCC == HIGH) {
      tft.setCursor(40,55);
      tft.setTextColor(ST77XX_BLACK);
      tft.println("Deligado");

      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_GREEN);
      tft.println("Ligado");
    } else {
      tft.setCursor(55,55);
      tft.setTextColor(ST77XX_BLACK);
      tft.println("Ligado");

      tft.setCursor(40,55);
      tft.setTextColor(ST77XX_RED);
      tft.println("Deligado");
    }
  }
}

void telaCofre(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(40,20);
    tft.println("Travas");
    tft.setCursor(1,50);
    tft.print("Banco Armario");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(40,20);
    tft.println("Travas");
    tft.setCursor(1,50);
    tft.setTextColor(trava == 0 ? ST77XX_ORANGE : ST77XX_WHITE);
    tft.print("Banco ");
    tft.setTextColor(trava == 1 ? ST77XX_ORANGE : ST77XX_WHITE);
    tft.println("Armario");
  }
}

void telaCofreDestrava(bool cls) {
  if (cls) {
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(40,20);
    tft.println("Travas");
    tft.setCursor(1,50);
    tft.print("Banco Armario");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(40,20);
    tft.println("Travas");
    tft.setCursor(1,50);
    tft.setTextColor(trava == 0 ? ST77XX_GREEN : ST77XX_WHITE);
    tft.print("Banco ");
    tft.setTextColor(trava == 1 ? ST77XX_GREEN : ST77XX_WHITE);
    tft.println("Armario");
  }
}

void _tela(int screen, bool clean) {
  tft.setTextSize(2);

  switch (screen) {
    case TELA_SOLE_TANQUE:
      telaSoleTanque(clean);
      break;

    case TELA_RELE_200A:
      telaRele200A(clean);
      break;

    case TELA_CC:
      telaCC(clean);
      break;

    case TELA_COFRE:
      telaCofre(clean);
      break;

    case TELA_COFRE_DESTRAVA:
      telaCofreDestrava(clean);
      break;

    default:
    case TELA_PRINCIPAL:
      telaPrincipal(clean);
  }
}

void tela(int newScreen, bool clean) {
  int oldScreen = screen;

  if (clean) {
    tft.fillScreen(ST7735_BLACK);
  }

  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(oldScreen);
  tft.setCursor(0,0);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print(newScreen);

  if (newScreen > TELA_COFRE_DESTRAVA || newScreen < TELA_PRINCIPAL) {
    screen = TELA_PRINCIPAL;
  } else {
    screen = newScreen;
  }

  if (!clean && screen != oldScreen) {
    _tela(oldScreen, true);
  }

  _tela(screen, false);
}

void botoes(int bt1, int bt2) {
  if (bt1 > 5000) {
    tela(TELA_CC, false);
  } else if (bt1 > 0) {
    if (screen >= TELA_RELE_200A) {
      tela(0, false);
    } else {
      tela(screen + 1, false);
    }
  } else if (bt2 > 5000) {
    tela(TELA_COFRE, false);
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

      tela(TELA_SOLE_TANQUE, false);
    } else if (screen == TELA_RELE_200A) {
      if (stRele200a == HIGH) {
        stRele200a = LOW;
      } else {
        stRele200a = HIGH;
      }

      tela(TELA_RELE_200A, false);
    } else if (screen == TELA_CC) {
      if (bt2 > 2000 && stReleCC == LOW) {
        stReleCC = HIGH;
      } else {
        stReleCC = LOW;
      }

      tela(TELA_CC, false);
    } else if (screen == TELA_COFRE) {
        if (bt2 > 2000) {
          digitalWrite(trava == 0 ? pTravaA : pTravaB, HIGH);
          tela(TELA_COFRE_DESTRAVA, false);
          delay(1500);
          digitalWrite(pTravaA, LOW);
          digitalWrite(pTravaB, LOW);
          tela(TELA_COFRE, false);
        } else {
          trava = trava == 1 ? 0 : 1;

          tela(TELA_COFRE, false);
        }
    } else {
      tela(screen - 1, false);
    }
  }
}

void processarSerial() {
  int serialByte = 0;

  while (Serial2.available() > 0) {
    serialByte = Serial2.read();

    switch (serialByte) {
      case getALL:
        Serial2.write(stRele200a ? setReleH : setReleL);
        Serial2.write(stReleCC ? setCCH : setCCL);
        Serial2.write(stGas ? setGasH : setGasL);
        Serial2.write(stAgua ? setAguaH : setAguaL);
        break;

      case setReleH:
        stRele200a = HIGH;
        break;

      case setReleL:
        stRele200a = LOW;
        break;

      case setCCH:
        stReleCC = HIGH;
        break;

      case setCCL:
        stReleCC = LOW;
        break;

      case setIgnicaoH:
        stIgnicao = HIGH;
        break;

      case setIgnicaoL:
        stIgnicao = LOW;
        break;

      case setMotorH:
        stMotor = HIGH;
        break;

      case setMotorL:
        stMotor = LOW;
        break;

      case setPositH:
        stAuxPositron = HIGH;
        break;

      case setPositL:
        stAuxPositron = LOW;
        break;
    }
  }

  if (millis() - loopSetSerial > 751) {
    loopSetSerial = millis();
  }
}


void loop() {
  int tempoBt1 = 0;
  int tempoBt2 = 0;

  if (millis() - loopBtn > 50) {
    tempoBt1 = pressBotao(pBt1, HIGH, &bt1last, &bt1mili);
    tempoBt2 = pressBotao(pBt2, HIGH, &bt2last, &bt2mili);
    loopBtn = millis();
  }

  if ((atualizaRele(pAgua, &stAgua) || atualizaRele(pGas, &stGas)) && screen == TELA_PRINCIPAL) {
    tela(TELA_PRINCIPAL, false);
  }

  botoes(tempoBt1, tempoBt2);
  processarSerial();

  digitalWrite(pTanqueA, stTanqueA);
  digitalWrite(pTanqueB, stTanqueB);
  digitalWrite(pRele200a, stRele200a);
  digitalWrite(pReleCC, stReleCC);
}
