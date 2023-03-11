#include <Keypad.h>
#include <Encoder.h>
#include <Bounce2.h>  
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "HID-Project.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);
byte UpArrow[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte DownArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte LeftArrow[] = {
  B00000,
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000
};
byte RightArrow[] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};
byte Folder[] = {
  B00000,
  B00000,
  B11000,
  B11111,
  B10001,
  B10001,
  B11111,
  B00000
};

byte coffeLeft[8] = {
  0b01001,
  0b00100,
  0b00000,
  0b11111,
  0b10000,
  0b10000,
  0b01000,
  0b00111
};

byte coffeRight[8] = {
  0b00000,
  0b10000,
  0b00000,
  0b11110,
  0b00101,
  0b00110,
  0b01000,
  0b10000
};
//Keypad buttons
int R1 = 5;
int R2 = 6;
int R3 = 7;
int R4 = 8;
int C1 = 15;
int C2 = 9;
int C3 = 4;
const byte ROWS = 4;
const byte COLS = 3;
char keys[COLS][ROWS] = {
  {'X','7','4','1'},
  {'*','8','5','2'},
  {'-','9','6','3'}
};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};
Keypad kpd = Keypad( makeKeymap(keys), colPins, rowPins, COLS, ROWS);

//State LED pins
int S1 = 21;
int S2 = 20;

const int numStates = 3;
const int States[numStates] = {S1, S2};
int currentState = 0;

int lastDebounceTime = 0;
const int debounceTime = 50;

// Variables Cronometro
const int INTERVALO_REST = 50; 
const int Hasta_REST = 60; 
unsigned long tiempo_inicial = 0; 
unsigned long tiempo_actual = 0; 
unsigned long tiempo_transcurrido = 0;
unsigned int horas = 0;
unsigned int minutos = 0;
unsigned int segundos = 0;
unsigned int centesimos =0;
unsigned int milisegundos;
bool cronometro_corriendo = false; 
bool mostrar_resto = false;

//Encoder
int SW = 14;
int DT = 16;
int CLK = 10;
Encoder Knob(DT,CLK);

Bounce encoderButton = Bounce(SW,14);
int timeLimit = 500;
long oldPosition = -999;
long newPosition;

void setup() {
  Serial.begin(9600);
  
  lcd.init();  //initialize lcd screen
  lcd.backlight();  // turn on the backlight

  lcd.createChar(0, UpArrow); // Up arrow create
  lcd.createChar(1, DownArrow); // Down arrow create
  lcd.createChar(2, LeftArrow); // Left arrow create
  lcd.createChar(3, RightArrow); // Right arrow create
  lcd.createChar(4, Folder); // Folder create
  lcd.createChar(5, coffeLeft);
  lcd.createChar(6, coffeRight);

  pinMode(CLK, INPUT_PULLUP);
  
  Keyboard.begin();
  //Consumer.begin();
  
  Serial.print("Ready");
  newPosition = Knob.read();
}

void LcdPrint(int key){
      //lcd.clear();
      //lcd.setCursor(0,0); //posicionar linea 1
      //lcd.print("Key :");
      //lcd.print(key);
      lcd.setCursor(0,1);  //posicionar linea 2
      lcd.print("Write :");
      lcd.print(key);
}

void LayoutPrint(){
  lcd.setCursor(13,1);
  lcd.print("[");
  lcd.setCursor(14,1);
  lcd.print(currentState);
  lcd.setCursor(15,1);
  lcd.print("]");
}

void LcdPrint2(int key){
  lcd.clear();

  switch(key){
    case 1:
      lcd.setCursor(0,1);
      lcd.print("Left ");
      lcd.write(byte(2));
      break;
    case 2:
      lcd.setCursor(0,1);
      lcd.print("Down ");
      lcd.write(byte(1));
      break;
    case 3:
      lcd.setCursor(0,1);
      lcd.print("Right ");
      lcd.write(byte(3));
      break;
    case 4:
      lcd.setCursor(0,1);
      lcd.print("Delete");
      break;
    case 5:
      lcd.setCursor(0,1);
      lcd.print("Up ");
      lcd.write(byte(0));
      break;
    case 6:
      lcd.setCursor(0,1);
      lcd.print("Supr");
      break;
    case 7:
      lcd.setCursor(0,1);
      lcd.print("Esc");
      break;
    case 8:
      
      break;
    case 9:
      
      break;
  }
}

void ChangeState(){
  digitalWrite(States[currentState], LOW);
  currentState++;
  if (currentState == numStates){
    currentState = 0;
  }
  digitalWrite(States[currentState], HIGH);
  //Serial.print("State Changed. Current State: "); Serial.println(currentState);
  delay(100);
  return;
}

void Layout1(char button){
  switch(button){
    case '1':
      Keyboard.print('1');
      LcdPrint(1);
      break;
    case '2':
      Keyboard.print('2');
      LcdPrint(2);
      break;
    case '3':
      Keyboard.print('3');
      LcdPrint(3);
      break;
    case '4':
      Keyboard.print('4');
      LcdPrint(4);
      break;
    case '5':
      Keyboard.print('5');
      LcdPrint(5);
      break;
    case '6':
      Keyboard.print('6');
      LcdPrint(6);
      break;
    case '7':
      Keyboard.print('7');
      LcdPrint(7);
      break;
    case '8':
      Keyboard.print('8');
      LcdPrint(8);
      break;
    case '9':
      Keyboard.print('9');
      LcdPrint(9);
      break;
  };
}//
void Layout2(char button){
  lcd.clear();
  switch(button){
    case '1':
      Keyboard.press(KEY_LEFT);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Left ");
      lcd.write(byte(2));
      break;
    case '2':
      Keyboard.press(KEY_DOWN);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Down ");
      lcd.write(byte(1));
      break;
    case '3':
      Keyboard.press(KEY_RIGHT);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Right ");
      lcd.write(byte(3));
      LcdPrint2(3);
      break;
    case '4':
      Keyboard.press(KEY_BACKSPACE);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Delete");
      break;
    case '5':
      Keyboard.press(KEY_UP);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Up ");
      lcd.write(byte(0));
      break;
    case '6'://
      Keyboard.press(KEY_DELETE);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Supr");
      break;
    case '7'://
      Keyboard.press(KEY_ESC);
      Keyboard.releaseAll();
      lcd.setCursor(0,1);
      lcd.print("Esc");
      break;
    case '8':
      Keyboard.press(KEY_RIGHT_CTRL);
      Keyboard.press(KEY_RIGHT_SHIFT);
      Keyboard.press(KEY_LEFT);
      lcd.setCursor(0,1);
      lcd.print("Left Select");
      Keyboard.releaseAll();
      break;
    case '9':
    Keyboard.press(KEY_RIGHT_CTRL);
    Keyboard.press(KEY_RIGHT_SHIFT);
    Keyboard.press(KEY_RIGHT);
    Keyboard.releaseAll();
    lcd.setCursor(0,1);
    lcd.print("Right Select");
      break;
  };
}

void Layout3(char button){
  lcd.clear();
  switch(button) {
    case '1':
      tiempo_inicial = millis(); 
      cronometro_corriendo = true; 
      break;

    case '2': 
      tiempo_inicial = 0; 
      tiempo_transcurrido = 0; 
      cronometro_corriendo = false; 
      mostrar_resto = false; 
      //lcd.clear();
      break;
  }
}

void Layout4(char button){
  
}

void PrintCronometro(){
    tiempo_actual = millis(); 
    tiempo_transcurrido = tiempo_actual - tiempo_inicial;
    
    horas = tiempo_transcurrido / 3600000; 
    minutos = (tiempo_transcurrido / 60000) % 60;
    segundos = (tiempo_transcurrido / 1000) % 60;
    
      if(tiempo_actual % 10 == 0){
      //lcd.clear();
      centesimos++;
      if(centesimos == 100){
      centesimos = 0;
      segundos++;
      }
      if(segundos == 60){
      segundos = 0;
      minutos++;
      }
      if(minutos == 60){
      minutos = 0;
      horas++;
      }
      lcd.setCursor(0,0);
      if(horas < 10){
      lcd.print("0");
      }
      lcd.print(horas);
      lcd.print(":");

      if(minutos < 10){
      lcd.print("0");
      }
      lcd.print(minutos);
      lcd.print(":");
      
      if(segundos < 10){
      lcd.print("0");
      }
      lcd.print(segundos);}

    if (minutos >= INTERVALO_REST && minutos < Hasta_REST) {
      lcd.setCursor(0, 1);  // linea 2
      lcd.print("Descanso");
      //imprimir el caffe :)
      lcd.setCursor(11,0);
      lcd.write(byte(5));
      lcd.setCursor(12,0);
      lcd.write(byte(6));
    }

}



void loop() {
  char key = kpd.getKey();
  LayoutPrint();
  if(key) {
    switch(key){
      case '*':
        ChangeState();
        break;
      case '-':
        Keyboard.press(KEY_RIGHT_CTRL);
        Keyboard.press('s');
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Saved ");
        lcd.write(byte(4));
        delay(10);
        Keyboard.releaseAll();
        break;
      default:
        switch(currentState){
          case 0:
            //lcd.setCursor(0,1);  // linea 2
            Layout1(key);
            break;
          case 1:
            Layout2(key);
            break;
          case 2:
            Layout3(key);
            break;
          case 3: 
            Layout4(key);
            break;
        }
    }
  }

  //cronometro del layout3
  if(cronometro_corriendo) {
    PrintCronometro();
  }

  //check the encoder button
  if(encoderButton.update()) {
    if(encoderButton.fallingEdge()) {
      int fall = millis();
      while(!encoderButton.update()){}
      if(encoderButton.risingEdge()){
        int rise = millis();
        //Serial.println(rise - fall);
        if(rise - fall > timeLimit){
          Consumer.write(MEDIA_NEXT);
          Serial.print("Next");
        } else {
          Consumer.write(MEDIA_PLAY_PAUSE);
          Serial.print("Play/Pause");
        }
      }
      Keyboard.releaseAll();
    }
  }

   //check encoder rotation
  newPosition = Knob.read();
  if(newPosition != oldPosition){
    Serial.print(newPosition);

    if((newPosition - oldPosition) > 0) {
      //volumeup
      Serial.println("volume up");
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      //volumedown
      Serial.println("volume down");
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    oldPosition = newPosition;
    Keyboard.releaseAll();
    delay(200); //a delay of 200 seems to be the sweet spot for this encoder.
  }  
}
