#include <RF24Network.h>
#include <RF24.h>
#include <EEPROM.h>
#include <SPI.h>
#include <DHT.h>
#include <printf.h>

#define PIR_PIN 3
#define DHT_PIN A0

RF24 radio(8, 7);
RF24Network network(radio);

DHT dht(DHT_PIN, DHT11);

// constraints
const uint16_t node = 02;
const uint16_t server = 00;

// vars
unsigned long lastSent;
unsigned long lastMotion;

struct config {
  unsigned long motionInterval = 10000;
  unsigned long interval = 60000;
  boolean autoSend = true;
  uint8_t channel = 90;
} config;

enum payloadTypes {
  payloadCommand,
  payloadReadingChar,
  payloadReadingInteger,
  payloadReadingFloat,
  payloadReadingDouble
};

struct command { char message[32]; };
struct readingInt { char name[16]; int value; };
struct readingFloat { char name[16]; float value; };
struct readingChar { char name[16]; char value[16]; };
struct readingDouble { char name[16]; double value; };

void setup(void) {
  Serial.begin(57600);
  printf_begin();
  printf("starting node...\n");
  updateConfig();
  SPI.begin();
  dht.begin();
  radio.begin();
  network.begin(config.channel, node);
  radio.printDetails();

  // Setup the PIR sensor
  pinMode(PIR_PIN, INPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
}

void loop() {
  network.update();

  unsigned long now = millis();

  if ((now - lastSent) >= config.interval) {
    while (!sendFloat("VOLTAGE", readVoltage())) {
      prioratize(); 
      delay(125);
    }
    while (!sendFloat("TEMP", dht.readTemperature())) {
      prioratize();
      delay(125);
    }
    while (!sendFloat("HUMID", dht.readHumidity())) {
      delay(125);
      prioratize();
    }

    lastSent = now;
  }

  while (network.available()) {
    RF24NetworkHeader header;
    network.peek(header);
    if (header.type == payloadCommand) {
      command cmd;
      network.read(header, &cmd, sizeof(cmd));
      String msg = cmd.message;
      String val = part(msg.substring(4), '=', 1);
      if (msg.substring(0, 3) == "SET") {
        char key[32];
        part(msg.substring(4), '=', 0).toCharArray(key, 32);
        if (key == "MOTION_INT") EEPROM.write(3, msg.toInt());
        if (key == "CHANNEL") EEPROM.write(2, msg.toInt());
        if (key == "AUTOSEND") EEPROM.write(1, msg.toInt());
        if (key == "INTERVAL") EEPROM.write(0, msg.toInt());
        updateConfig();
      }
    }
  }

  prioratize();
}

void prioratize()
{
  unsigned long now = millis();
  if (digitalRead(PIR_PIN) == HIGH && (now - lastMotion) >= config.motionInterval) {
    printf("Motion detected!\n");
    lastMotion = now;
    bool done = false;
    while ( ! done) {
      done = sendFloat("MOTION", 1);
    }
  }
}

void updateConfig() {
  int c_interval = EEPROM.read(0);
  int c_autosend = EEPROM.read(1);
  int c_channel = EEPROM.read(2);
  int c_motioninterval = EEPROM.read(3);

  delay(30);

  config.interval = (long) (c_interval < 255) ? (c_interval * 1000) : config.interval;
  config.motionInterval = (long) (c_motioninterval < 255) ? (c_motioninterval * 1000) : config.motionInterval;
  config.autoSend = (c_autosend != 255);
  config.channel = (c_channel < 127) ? c_channel : config.channel;

  printf("config {\n\tinterval=%i\n\tautoSend=%i\n\tchannel=%i\n}\n", config.interval, config.autoSend, config.channel);
}

boolean sendFloat(char* param, float value) {
  RF24NetworkHeader header(server, payloadReadingFloat);
  readingFloat reading;
  reading.value = value;
  strncpy(reading.name, param, sizeof(reading.name));
  return network.write(header, &reading, sizeof(reading));
}

float readVoltage() {
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(75);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  uint8_t low  = ADCL;
  uint8_t high = ADCH;
  long result = (high << 8) | low;
  result = 1125300L / result;
  return result;
}

String part(String data, char separator, int index)
{
  int found = 0;
  int buff[] = { 0, -1 };
  int _max = data.length() - 1;
  for (int i = 0; i <= _max && found <= index; i++) {
    if (data.charAt(i) == separator || i == _max) {
      found++;
      buff[0] = buff[1]+1;
      buff[1] = (i == _max) ? i+1 : i;
    }
  }
  return (found > index) ? data.substring(buff[0], buff[1]) : "";
}
