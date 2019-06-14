/*******************************************************************************
*
*    Projeto Eletrônica – Termômetro com leds e sensor de temperatura com alarme e lcd
*    Gabriel Nicolau, Daniel Froes, Guilherme e Mash Bragança
*
*******************************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 
#define LM35 1
#define Buzzer 2
#define adder 0.70
#define lowestTemp 23.0
#define buttonPin 3
const int LED[] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
float temperatura = 0;
float tempAux = 0;

void setup() {
  lcd.begin (16,2); //Inicializa o lcd
  Serial.begin(9600); //Mostrar a temperatura no log do arduino
  //Pinagens
  pinMode(Buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  for(int x = 0; x < 10; x++){
    pinMode(LED[x], OUTPUT);
  }
}

void loop() {
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01; //Celsius
  Serial.println(temperatura); //Printar no log as temperaturas lidas

  int buttonNotPressed = digitalRead(buttonPin); // Verifica se o botão foi pressionado
  delay(250);
  
  
  if (buttonNotPressed) {
//    tempAux = (float(analogRead(LM35))*5/(1023))/0.01; //Celsius
    printCelsius((float(analogRead(LM35))*5/(1023))/0.01);
  } else {
//    tempAux = ((temperatura) * 1.8)+32; //Farenheit
    printFarenheit(((temperatura) * 1.8)+32);
  }
  
  delay(1000);
  
  for (int i=0; i<9; i++) {
    if(temperatura > lowestTemp + i*adder){
      digitalWrite(LED[i], HIGH);
    }
    else{
      digitalWrite(LED[i], LOW);
    }
  }
  if(temperatura > lowestTemp + 9*adder){
    digitalWrite(LED[9], HIGH);
    digitalWrite(Buzzer,HIGH);
  }
  else{
    digitalWrite(LED[9], LOW);
    digitalWrite(Buzzer,LOW);
  }
}

void printCelsius(float tempToShow) {
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0,1);
  lcd.print(tempToShow);
  lcd.write(B11011111); // Símbolo de grau
  lcd.print("C ");
}

void printFarenheit(float tempToShow) {
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0,1);
  lcd.print(tempToShow);
  lcd.write(B11011111); // Símbolo de grau
  lcd.print("F ");
}
