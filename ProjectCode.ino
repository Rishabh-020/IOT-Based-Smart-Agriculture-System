#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include "thingProperties.h"

#include <DHT.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ===== LCD =====
hd44780_I2Cexp lcd;

// ===== PINS =====
#define DHTPIN 2
#define DHTTYPE DHT22

#define SOIL_PIN A2
#define LDR_PIN  A0
#define RAIN_PIN A1

#define RELAY_FAN  7
#define RELAY_PUMP 8

#define SERVO_PIN    6
#define ONE_WIRE_BUS 3

// ===== OBJECTS =====
DHT dht(DHTPIN, DHTTYPE);
Servo flapServo;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// ===== THRESHOLDS =====
float tempThreshold  = 25;
int   soilThreshold  = 550;
int   lightThreshold = 300;
int   rainThreshold  = 600;
float humidityHigh   = 80;
int   lastCropType   = -1;   // int, same as cropType in thingProperties.h

// ===== TIMING =====
unsigned long lastUpdate = 0;
const long interval = 2000;

// ===== FUNCTION DECLARATIONS =====
int    readStable(int pin);
void   readSensors();
void   applyLogic();
void   setCropThresholds(int type);
String getCropName(int type);

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  dht.begin();
  ds18b20.begin();

  // LCD splash screen — shown ONCE at boot
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Agriculture");
  lcd.setCursor(0, 1);
  lcd.print("   System v1.0  ");
  delay(3000);
  lcd.clear();

  flapServo.attach(SERVO_PIN);
  flapServo.write(180); // open initially

  pinMode(RELAY_FAN,  OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);

  digitalWrite(RELAY_FAN,  HIGH);
  digitalWrite(RELAY_PUMP, HIGH);
}

// ===== LOOP =====
void loop() {
  ArduinoCloud.update();

  if (millis() - lastUpdate < interval) return;
  lastUpdate = millis();

  if (cropType != lastCropType) {
    setCropThresholds(cropType);
    lastCropType = cropType;
  }

  readSensors();
  applyLogic();
}

// ===== CROP NAME =====
String getCropName(int type) {
  switch (type) {
    case 1:  return "Wheat";
    case 2:  return "Rice";
    case 3:  return "Tomato";
    case 4:  return "Potato";
    default: return "Unknown";
  }
}

// ===== SENSOR READ =====
void readSensors() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if (!isnan(temp)) airTemp      = temp;
  if (!isnan(hum))  humidity     = hum;

  soilMoisture = readStable(SOIL_PIN);

  ds18b20.requestTemperatures();
}

// ===== MAIN LOGIC =====
void applyLogic() {

  // ----- Local variables (NOT cloud) -----
  int   lightIntensity = readStable(LDR_PIN);
  int   rainRaw        = readStable(RAIN_PIN);
  bool  rainDetected   = (rainRaw < rainThreshold);
  float soilTemp       = ds18b20.getTempCByIndex(0);
  bool  pumpStatus     = false;
  bool  servoStatus    = false;

  // ===== CONDITIONS =====
  bool isHot    = (airTemp        > tempThreshold);
  bool isHumid  = (humidity       > humidityHigh);
  bool isDry    = (soilMoisture   > soilThreshold);
  bool isBright = (lightIntensity < lightThreshold);

  // ===== FAN & PUMP =====
  bool systemON = isHot || isHumid || isDry;

  if (systemON) {
    digitalWrite(RELAY_FAN,  LOW);
    digitalWrite(RELAY_PUMP, LOW);
    fanStatus  = true;
    pumpStatus = true;
  } else {
    digitalWrite(RELAY_FAN,  HIGH);
    digitalWrite(RELAY_PUMP, HIGH);
    fanStatus  = false;
    pumpStatus = false;
  }

  // ===== SERVO (BRIGHT LIGHT OR RAIN) =====
  bool servoActive = isBright || rainDetected;

  if (servoActive) {
    flapServo.write(0);    // close flap
    servoStatus = true;
  } else {
    flapServo.write(180);  // open flap
    servoStatus = false;
  }

  // ===== LCD — always shows live data =====
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(airTemp, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(soilMoisture);
  lcd.print(" ");
  lcd.print(getCropName(cropType));
  lcd.print("   ");

  // ===== DEBUG =====
  Serial.print("Temp: ");    Serial.print(airTemp);
  Serial.print(" | Hum: ");  Serial.print(humidity);
  Serial.print(" | Soil: "); Serial.print(soilMoisture);
  Serial.print(" | Light: ");Serial.print(lightIntensity);
  Serial.print(" | SoilT: ");Serial.print(soilTemp);
  Serial.print(" | Rain: "); Serial.print(rainDetected);
  Serial.print(" | Servo: ");Serial.print(servoStatus);
  Serial.print(" | Pump: "); Serial.print(pumpStatus);
  Serial.print(" | Fan: ");  Serial.print(fanStatus);
  Serial.print(" | Crop: "); Serial.println(getCropName(cropType));
}

// ===== CROP THRESHOLDS =====
void setCropThresholds(int type) {
  switch (type) {
    case 1: // Wheat
      tempThreshold  = 26;  humidityHigh   = 75;
      soilThreshold  = 590; lightThreshold = 350;
      rainThreshold  = 600;
      break;
    case 2: // Rice
      tempThreshold  = 30;  humidityHigh   = 85;
      soilThreshold  = 290; lightThreshold = 300;
      rainThreshold  = 650;
      break;
    case 3: // Tomato
      tempThreshold  = 28;  humidityHigh   = 75;
      soilThreshold  = 560; lightThreshold = 320;
      rainThreshold  = 600;
      break;
    case 4: // Potato
      tempThreshold  = 30;  humidityHigh   = 80;
      soilThreshold  = 530; lightThreshold = 330;
      rainThreshold  = 620;
      break;
    default:
      tempThreshold  = 25;  humidityHigh   = 80;
      soilThreshold  = 500; lightThreshold = 300;
      rainThreshold  = 600;
      break;
  }
}

// ===== STABLE READ =====
int readStable(int pin) {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return sum / 5;
}

// ===== CLOUD CALLBACKS =====
void onCropTypeChange() {
  setCropThresholds(cropType);
  lastCropType = cropType;
}

void onAirTempChange()     {}
void onHumidityChange()    {}
void onSoilMoistureChange(){}
void onFanStatusChange()   {}
