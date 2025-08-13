#include <DHT.h>

// Pin definitions
#define DHT11_PIN 4
#define LDR_PIN 5
#define BUZZER_PIN 8
#define LED_PIN_1 A2
#define LED_PIN_2 A3
#define LED_PIN_3 A4
#define LED_PIN_4 A5
#define GAS_SENSOR_PIN A0

// DHT sensor object
DHT dht(DHT11_PIN, DHT11);

// Variables to store sensor data
float temperature;
float humidity;
int ldrValue;
int gasValue;
bool led1State = false;
bool led2State = false;
bool led3State = false;
bool led4State = false;
bool buzzerState = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Set pin modes
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  
  // Initialize outputs to OFF
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_4, LOW);
  
  Serial.println("Arduino Sensor Dashboard Ready");
}

void loop() {
  // Read DHT sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  // Read analog sensors
  ldrValue = analogRead(LDR_PIN);
  gasValue = analogRead(GAS_SENSOR_PIN);
  
  // Send sensor data as JSON
  Serial.print("{\"temperature\":");
  Serial.print(isnan(temperature) ? 0 : temperature);
  Serial.print(",\"humidity\":");
  Serial.print(isnan(humidity) ? 0 : humidity);
  Serial.print(",\"ldr\":");
  Serial.print(ldrValue);
  Serial.print(",\"gas\":");
  Serial.print(gasValue);
  Serial.print(",\"led1\":");
  Serial.print(led1State ? 1 : 0);
  Serial.print(",\"led2\":");
  Serial.print(led2State ? 1 : 0);
  Serial.print(",\"led3\":");
  Serial.print(led3State ? 1 : 0);
  Serial.print(",\"led4\":");
  Serial.print(led4State ? 1 : 0);
  Serial.print(",\"buzzer\":");
  Serial.print(buzzerState ? 1 : 0);
  Serial.println("}");
  
  // Check for incoming commands
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    
    // Parse commands
    if (command == "LED1_ON") {
      led1State = true;
      digitalWrite(LED_PIN_1, HIGH);
      Serial.println("LED1 ON");
    }
    else if (command == "LED1_OFF") {
      led1State = false;
      digitalWrite(LED_PIN_1, LOW);
      Serial.println("LED1 OFF");
    }
    else if (command == "LED2_ON") {
      led2State = true;
      digitalWrite(LED_PIN_2, HIGH);
      Serial.println("LED2 ON");
    }
    else if (command == "LED2_OFF") {
      led2State = false;
      digitalWrite(LED_PIN_2, LOW);
      Serial.println("LED2 OFF");
    }
    else if (command == "LED3_ON") {
      led3State = true;
      digitalWrite(LED_PIN_3, HIGH);
      Serial.println("LED3 ON");
    }
    else if (command == "LED3_OFF") {
      led3State = false;
      digitalWrite(LED_PIN_3, LOW);
      Serial.println("LED3 OFF");
    }
    else if (command == "LED4_ON") {
      led4State = true;
      digitalWrite(LED_PIN_4, HIGH);
      Serial.println("LED4 ON");
    }
    else if (command == "LED4_OFF") {
      led4State = false;
      digitalWrite(LED_PIN_4, LOW);
      Serial.println("LED4 OFF");
    }
    else if (command == "BUZZER_ON") {
      buzzerState = true;
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("BUZZER ON");
    }
    else if (command == "BUZZER_OFF") {
      buzzerState = false;
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("BUZZER OFF");
    }
    else if (command == "ALL_OFF") {
      led1State = led2State = led3State = led4State = buzzerState = false;
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_3, LOW);
      digitalWrite(LED_PIN_4, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("ALL OFF");
    }
  }
  
  delay(1000); // Send data every second
}