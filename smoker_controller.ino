

#include <max6675.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// config display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Thermometer
#define GPIO_SO 8
#define GPIO_CS 9
#define GPIO_CLK 10

#define BAUDRATE_SERIAL_MONITOR 9600

#define TIME_BETWEEN_THERMOMETER_READINGS 1000 // ms

const int LED_HOT = 5;
const int LED_COLD = 4;
const int LED_OK = 3;

const int HEAT_BOTTOM_TEMP_ALERT = 85;
const int HEAT_TOP_TEMP_ALERT = 95;

// Quantity of times that a temperature need to be counted to trigger a sing
const int TIMES_VERIFICATION = 3;

/* THERMOMETER OBJECT */
MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);

void setup()
{
    Serial.begin(BAUDRATE_SERIAL_MONITOR);

    pinMode(LED_HOT, OUTPUT);
    pinMode(LED_COLD, OUTPUT);
    pinMode(LED_OK, OUTPUT);

    lcd.init(); // initialize the lcd
    setDisplayOn();
}

void loop()
{
    int temperature = termopar.readCelsius();
    int intTemperature = (int)temperature;
    digitalWrite(LED_HOT, LOW);
    digitalWrite(LED_COLD, LOW);
    digitalWrite(LED_OK, LOW);
    delay(500);

    if (intTemperature < HEAT_BOTTOM_TEMP_ALERT)
    {
        digitalWrite(LED_COLD, HIGH);
        Serial.println("Cold");
    }
    else if (intTemperature > HEAT_BOTTOM_TEMP_ALERT && intTemperature < HEAT_TOP_TEMP_ALERT)
    {
        Serial.println("Temp ok Cold");
        digitalWrite(LED_OK, HIGH);
    }
    else
    {
        Serial.println("Too hot");
        digitalWrite(LED_HOT, HIGH);
    }

    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println("C");

    //  displayText("Temperatura: " + (String) temperature);
    displayText("Temperatura", 0, 0, 1000);
    displayText((String)temperature, 1, 0, 5000);

    delay(TIME_BETWEEN_THERMOMETER_READINGS);
}

void setDisplayOn()
{
    lcd.setBacklight(HIGH);
}

void setDisplayOff()
{
    lcd.setBacklight(LOW);
}

void displayText(String text, int line, int column, int delayTime)
{
    for (int i = 0; i < text.length(); i++)
    {
        lcd.setCursor(column, line);
        lcd.print(text[i]);
        column = column + 1;
    }
    delay(delayTime);
}