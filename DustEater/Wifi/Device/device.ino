#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include <Adafruit_MotorShield.h>
#include <Wire.h>

#define INTERVAL 5000
#define MESSAGE_MAX_LEN 500

int freq = 2000;
int channel = 0;
int resolution = 8;

const char *ssid = "Van Roey - Guest";
const char *password = "1a2b3c4d5e";

// const char *ssid = "Mertens 2.4";
// const char *password = "298cff444a";
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *L_MOTOR = AFMS.getMotor(3);
Adafruit_DCMotor *R_MOTOR = AFMS.getMotor(4);

// Set your forward, reverse, and turning speeds
#define ForwardSpeed 110
#define ReverseSpeed 110
#define TurningSpeed 90

const int C2 = 65;
const int Db2 = 69;
const int D2 = 73;
const int Eb2 = 78;
const int E2 = 82;
const int F2 = 87;
const int Gb2 = 93;
const int G2 = 98;
const int Ab2 = 104;
const int AA2 = 110;
const int Bb2 = 117;
const int B2 = 123;
const int C3 = 131;
const int Db3 = 139;
const int D3 = 147;
const int Eb3 = 156;
const int E3 = 165;
const int F3 = 175;
const int Gb3 = 185;
const int G3 = 196;
const int Ab3 = 208;
const int AA3 = 220;
const int Bb3 = 233;
const int B3 = 247;
const int C4 = 262;
const int Db4 = 277;
const int D4 = 294;
const int Eb4 = 311;
const int E4 = 330;
const int F4 = 349;
const int Gb4 = 370;
const int G4 = 392;
const int Ab4 = 415;
const int AA4 = 440;
const int Bb4 = 466;
const int B4 = 494;
const int C5 = 523;
const int Db5 = 554;
const int D5 = 587;
const int Eb5 = 622;
const int E5 = 659;
const int F5 = 698;
const int Gb5 = 740;
const int G5 = 784;
const int Ab5 = 831;
const int AA5 = 880;
const int Bb5 = 932;
const int B5 = 988;
const int C6 = 1047;
const int Db6 = 1109;
const int D6 = 1175;
const int Eb6 = 1245;
const int E6 = 1319;
const int F6 = 1397;
const int Gb6 = 1480;
const int G6 = 1568;
const int Ab6 = 1661;
const int AA6 = 1760;
const int Bb6 = 1865;
const int B6 = 1976;

const int buzzerPin = 8;
const int ledPin1 = 12;
const int ledPin2 = 13;

int counter = 0;

static const char *connectionString = "HostName=VRATelemetryIoTHubDev.azure-devices.net;DeviceId=DustEater;SharedAccessKey=idLbmRzt6wmrK+CCNzHie2rnI6zqI+h9DYsmx83rHBY=";
const char *messageDataHigh = "{\"value\":\"Eating_Dust\"}";
const char *messageDataLow = "{\"value\":\"Eating_Dust\"}";
static bool hasIoTHub = false;
static bool hasWifi = false;
int messageCount = 1;
static bool messageSending = true;
static uint64_t send_interval_ms;

const int ledPin = 21;
const int soundPin = 27;
const int optitionalPin = 13;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char *payLoad, int size)
{
  Serial.println("Message callback:");
  Serial.println(payLoad);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == NULL)
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  // Display Twin message.
  Serial.println(temp);
  free(temp);
}

static int DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  LogInfo("Try to invoke method %s", methodName);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    LogInfo("Start sending temperature and humidity data");
    messageSending = true;
  }
  if (strcmp(methodName, "moveForward") == 0)
  {
    LogInfo("HappyBot is moving forward...");
    MoveForward();
  }
  if (strcmp(methodName, "moveBackward") == 0)
  {
    LogInfo("HappyBot is moving backwards...");
    MoveBackward();
  }
  if (strcmp(methodName, "stopMoving") == 0)
  {
    LogInfo("HappyBot is stopped moving...");
    StopMoving();
  }
  if (strcmp(methodName, "turnLeft") == 0)
  {
    LogInfo("HappyBot is turning left...");
    TurnLeft();
  }
    if (strcmp(methodName, "burstLeft") == 0)
  {
    LogInfo("HappyBot is bursting left...");

    TurnLeft();
    delay(250);
    StopMoving();
  }
  if (strcmp(methodName, "burstRight") == 0)
  {
    LogInfo("HappyBot is bursting left...");

    TurnRight();
    delay(250);
    StopMoving();
  }
  if (strcmp(methodName, "turnRight") == 0)
  {
    LogInfo("HappyBot is turning right...");
    TurnRight();
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    LogInfo("Stop sending temperature and humidity data");
    messageSending = false;
  }
  else if (strcmp(methodName, "high") == 0)
  {
    LogInfo("Stop sending temperature and humidity data");
    digitalWrite(ledPin, HIGH);
  }
  else if (strcmp(methodName, "soundOn") == 0)
  {
    LogInfo("Sound ON");

    firstSection();
    firstSection();
    secondSection();
    thirdSection();
    thirdSection();
  }
  else if (strcmp(methodName, "optitionalHigh") == 0)
  {
    LogInfo("Pin 12 HIGH");
    digitalWrite(optitionalPin, HIGH);
  }
  else if (strcmp(methodName, "optitionalLow") == 0)
  {
    LogInfo("Pin 12 LOW");
    digitalWrite(optitionalPin, LOW);
  }
  else if (strcmp(methodName, "soundOff") == 0)
  {
    LogInfo("Sound OFF");
    ledcWrite(channel, 0);
    ledcWriteTone(channel, 0);
  }
  else if (strcmp(methodName, "low") == 0)
  {
    LogInfo("Stop sending temperature and humidity data");
    digitalWrite(ledPin, LOW);
  }
  else
  {
    LogInfo("No method %s found", methodName);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}

void setup()
{
  Serial.begin(9600);
  AFMS.begin(); // create with the default frequency 1.6KHz

  // turn on motors
  L_MOTOR->setSpeed(0);
  L_MOTOR->run(RELEASE);

  R_MOTOR->setSpeed(0);
  R_MOTOR->run(RELEASE);

  Serial.begin(115200);

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(soundPin, channel);

  pinMode(ledPin, OUTPUT);
  pinMode(optitionalPin, OUTPUT);

  Serial.println("ESP32 Device");
  Serial.println("Initializing...");
  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");

  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    hasWifi = false;
  }
  hasWifi = true;

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" > IoT Hub");
  if (!Esp32MQTTClient_Init((const uint8_t *)connectionString, true))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  Serial.println("Start sending events.");
  randomSeed(analogRead(0));
  send_interval_ms = millis();

  TurnLeft();
  delay(1500);
  StopMoving();
}

bool isMoving = false;
uint8_t maxspeed;

void MoveForward()
{
  isMoving = true;
  L_MOTOR->run(FORWARD);
  R_MOTOR->run(FORWARD);
  maxspeed = ForwardSpeed;
  Serial.println("Forward");
  SpeedUp();
}

void MoveBackward()
{
  isMoving = true;
  L_MOTOR->run(BACKWARD);
  R_MOTOR->run(BACKWARD);
  maxspeed = ReverseSpeed;
  Serial.println("Backward");
  SpeedUp();
}

void TurnLeft(){
  isMoving = true;
        L_MOTOR->run(RELEASE);
        R_MOTOR->run(FORWARD);
        maxspeed = TurningSpeed;
        Serial.println("Left");
        SpeedUp();
}

void TurnRight(){
   isMoving = true;
        L_MOTOR->run(FORWARD);
        R_MOTOR->run(RELEASE);
        maxspeed = TurningSpeed;
        Serial.println("Right"); 
        SpeedUp();
}

void StopMoving(){
   isMoving = false;
      // slow down the motors
      for (int speed = maxspeed; speed >= 0; speed-=5) {
        L_MOTOR->setSpeed(speed);
        R_MOTOR->setSpeed(speed);
        delay(5); // 50ms total to slow down
      }
      L_MOTOR->run(RELEASE);
      R_MOTOR->run(RELEASE);
}

void SpeedUp(){
  for (int speed=0; speed < maxspeed; speed+=5) {
        L_MOTOR->setSpeed(speed);
        R_MOTOR->setSpeed(speed);
        delay(5); // 250ms total to speed up
      }
}

void loop()
{
  // ledcWriteTone(channel, 2000);

  // for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){

  //   Serial.println(dutyCycle);

  //   ledcWrite(channel, dutyCycle);
  //   delay(1000);
  // }

  // ledcWrite(channel, 125);

  //  ledcWriteTone(channel, 4000);
  if (messageSending)
  {
    char messagePayload[MESSAGE_MAX_LEN];

    //   //   if (digitalRead(pirPin) == HIGH)
    //        Serial.println("Sending message to the IoT Hub.");
    //   //   {
    float sensorValue = 1;
    snprintf(messagePayload, MESSAGE_MAX_LEN, messageDataHigh);
    EVENT_INSTANCE *message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
    Esp32MQTTClient_SendEventInstance(message);

    //   //     Serial.println("Motion!");
    // digitalWrite(ledPin, LOW);
    // digitalWrite(optitionalPin, HIGH);

    //   //   }

    //   //   if (digitalRead(pirPin) == LOW)
    //   //   {
    //   //     float sensorValue = 0;
    //   //     snprintf(messagePayload, MESSAGE_MAX_LEN, messageDataLow, sensorValue);
    //   //     EVENT_INSTANCE *message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
    //   //     Esp32MQTTClient_SendEventInstance(message);
    //   //     Serial.println("No motion.");
    //       // digitalWrite(ledPin, HIGH);
    //       // delay(2000);
    //       //   digitalWrite(ledPin, LOW);
    //   //   }
  }
  else
  {
    Esp32MQTTClient_Check();
  }
  // // }

  delay(2000);
  // digitalWrite(ledPin, HIGH);
  
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  // tone(buzzerPin, note, duration);

  ledcWrite(channel, 100);
  ledcWriteTone(channel, note);
  delay(duration);

  // //Play different LED depending on value of 'counter'
  // if(counter % 2 == 0)
  // {
  //   digitalWrite(ledPin1, HIGH);
  //   delay(duration);
  //   digitalWrite(ledPin1, LOW);
  // }else
  // {
  //   digitalWrite(ledPin2, HIGH);
  //   delay(duration);
  //   digitalWrite(ledPin2, LOW);
  // }

  //Stop tone on buzzerPin
  // noTone(buzzerPin);

  ledcWrite(channel, 0);
  ledcWriteTone(channel, 0);

  delay(50);
}
void firstSection()
{
  beep(AA3, 200);
  beep(AA3, 200);
  beep(C4, 200);
  beep(AA3, 200);
  beep(D4, 200);
  beep(AA3, 200);
  beep(E4, 200);
  beep(D4, 200);
  beep(C4, 200);
  beep(C4, 200);
  beep(E4, 200);
  beep(C4, 200);
  beep(G4, 200);
  beep(C4, 200);
  beep(E4, 200);
  beep(C4, 200);
  beep(G3, 200);
  beep(G3, 200);
  beep(B3, 200);
  beep(G3, 200);
  beep(C4, 200);
  beep(G3, 200);
  beep(D4, 200);
  beep(C4, 200);
  beep(F3, 200);
  beep(F3, 200);
  beep(AA3, 200);
  beep(F3, 200);
  beep(C4, 200);
  beep(F3, 200);
  beep(C4, 200);
  beep(B3, 200);
}
void secondSection()
{
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(C4, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(E3, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(G3, 200);
  beep(C4, 200);
  beep(AA3, 325);
  beep(AA3, 325);
  beep(AA3, 200);
  beep(AA3, 75);
  beep(AA3, 325);
  beep(AA3, 450);
}

void thirdSection()
{
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(AA3, 75);
  beep(Bb3, 75);
  beep(G3, 200);
  beep(AA3, 75);
  beep(E4, 200);
  beep(AA3, 75);
  beep(C4, 200);
  beep(G3, 75);
  beep(G3, 200);
  beep(G3, 75);
  beep(AA3, 200);
  beep(AA3, 450);
}