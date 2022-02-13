#include <max6675.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


// config display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//
  
/* Definições: GPIOs do Arduino utilizado na comunicação com o 
   MAX6675 */
#define GPIO_SO       8
#define GPIO_CS       9
#define GPIO_CLK      10
 
/* Definição: baudrate da comunicação com Serial Monitor */
#define BAUDRATE_SERIAL_MONITOR    9600
 
/* Definição: tempo entre leituras do MAX6675 */
#define TEMPO_ENTRE_LEITURAS             1000 //ms

// LEDS
const int LED_HOT = 5;
const int LED_COLD = 4;
const int LED_OK = 3;

// TERM
const int HEAT_BOTTOM_TEMP_ALERT = 85;
const int HEAT_TOP_TEMP_ALERT = 95;

// BEEP
const bool BEEP_ALERT = true;
const int BEEP_PORT = 6;
const int BEEP_TOTAL_ITERATIONS = 3;
const int BEEP_DURATION = 20;

const int BEEP_TIME_BETWEEN_ULTRA_HOT = 150;
const int BEEP_TIME_BETWEEN_HOT = 300;
const int BEEP_TIME_BETWEEN_COOL = 500;

// KEYBOARD
const byte KEYBOARD_ROWS = 4;
const byte KEYBOARD_COLS = 4;
const byte KEYBOARD_ROWS_PINS [KEYBOARD_ROWS] = {30, 31 ,32, 33};
const byte KEYBOARD_COLS_PINS [KEYBOARD_COLS] = {34, 35 ,36, 37};
const char KEYBOARD_KEYS [KEYBOARD_ROWS][KEYBOARD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad myKeyboard = Keypad(
  makeKeymap(KEYBOARD_KEYS),
  KEYBOARD_ROWS_PINS,
  KEYBOARD_COLS_PINS,
  KEYBOARD_ROWS,
  KEYBOARD_COLS
);


//Quantity of times that a temperature need to be counted to trigger a beep
const int TIMES_VERIFICATION = 3;
 
/* Criação de objeto para comunicação com termopar */
MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);
  
void setup()
{
    Serial.begin(BAUDRATE_SERIAL_MONITOR);

    pinMode(LED_HOT, OUTPUT);
    pinMode(LED_COLD, OUTPUT);
    pinMode(LED_OK, OUTPUT);

    pinMode(BEEP_PORT, OUTPUT);

    lcd.init();                      // initialize the lcd 
    setDisplayOn();

}
 
/* Programa principal */
void loop() 
{  
  int temperature = termopar.readCelsius();
  int intTemperature = (int)temperature;
  digitalWrite(LED_HOT, LOW);
  digitalWrite(LED_COLD, LOW);
  digitalWrite(LED_OK, LOW);
  delay(500);

  Serial.println("Antes");
  if (keyboardAction()){
      return;
  }

  return;

  if (intTemperature < HEAT_BOTTOM_TEMP_ALERT){
    digitalWrite(LED_COLD, HIGH);
     beepAlert(BEEP_TIME_BETWEEN_COOL);
    Serial.println("Cold");
    
  } else if (intTemperature > HEAT_BOTTOM_TEMP_ALERT && intTemperature < HEAT_TOP_TEMP_ALERT) {
    Serial.println("Temp ok Cold");
    digitalWrite(LED_OK, HIGH);
    
  } else {
    Serial.println("Too hot");
    digitalWrite(LED_HOT, HIGH);
    beepAlert(BEEP_TIME_BETWEEN_ULTRA_HOT);
  }

  Serial.print("Temperatura: "); 
  Serial.print(temperature);
  Serial.println("C"); 

//  displayText("Temperatura: " + (String) temperature);
  displayText("Temperatura", 0, 0, 1000);
  displayText((String) temperature, 1, 0, 5000);
  
  delay(TEMPO_ENTRE_LEITURAS);
}

bool keyboardAction() {
  Serial.println("Lendo teclas");
  char pressedKey = myKeyboard.getKey(); // Atribui a variavel a leitura do teclado

  if (pressedKey) { // Se alguma tecla foi pressionada
    Serial.println(pressedKey); // Imprime a tecla pressionada na porta serial
    return true;
  }
  return false;
}

void setDisplayOn() {
   lcd.setBacklight(HIGH);
}

void setDisplayOff() {
   lcd.setBacklight(LOW);
}


void displayText(String text, int line, int column, int delayTime) {
  for(int i =0; i < text.length(); i++ ) {
    lcd.setCursor(column, line);
    lcd.print(text[i]);
    column = column + 1;
  }
  delay(delayTime);
}

void beepAlert(int delayBetweenBeeps) {
  if (! BEEP_ALERT) {
      Serial.println("Beep turned off");
  }

  for (int i = 0; i < BEEP_TOTAL_ITERATIONS; i++)
  {
      beep();
      delay(delayBetweenBeeps);
  }
}

void beep() {
    Serial.println("beep");
    digitalWrite(BEEP_PORT, HIGH);
    delay(BEEP_DURATION);
    digitalWrite(BEEP_PORT, LOW);
}

