# 1 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
# 1 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
# 2 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 2
# 3 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 2
# 4 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 2
# 5 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 2

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

static const char *connectionString = "HostName=VRATelemetryIoTHubDev.azure-devices.net;DeviceId=DarkKnight;SharedAccessKey=NOGzhBvSBtls4xVyVgbq5x1DhSln8CXfvBkQ7N07pKc=";
const char *messageDataHigh = "{\"value\":\"Racing_Trough_The_Night\"}";
const char *messageDataLow = "{\"value\":\"Racing_Trough_The_Night\"}";
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
  if (temp == 
# 125 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
             __null
# 125 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
                 )
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
  do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 138 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
 __null
# 138 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
 ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 138, 0x01, "Try to invoke method %s", methodName); }; }while((void)0,0);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 144 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 144 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 144, 0x01, "Start sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = true;
  }
  if (strcmp(methodName, "moveForward") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 149 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 149 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 149, 0x01, "HappyBot is moving forward..."); }; }while((void)0,0);
    MoveForward();
  }
  if (strcmp(methodName, "moveBackward") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 154 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 154 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 154, 0x01, "HappyBot is moving backwards..."); }; }while((void)0,0);
    MoveBackward();
  }
  if (strcmp(methodName, "stopMoving") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 159 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 159 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 159, 0x01, "HappyBot is stopped moving..."); }; }while((void)0,0);
    StopMoving();
  }
  if (strcmp(methodName, "turnLeft") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 164 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 164 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 164, 0x01, "HappyBot is turning left..."); }; }while((void)0,0);
    TurnLeft();
  }
  if (strcmp(methodName, "burstLeft") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 169 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 169 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 169, 0x01, "HappyBot is bursting left..."); }; }while((void)0,0);

    TurnLeft();
    delay(250);
    StopMoving();
  }
  if (strcmp(methodName, "burstRight") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 177 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 177 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 177, 0x01, "HappyBot is bursting left..."); }; }while((void)0,0);

    TurnRight();
    delay(250);
    StopMoving();
  }
  if (strcmp(methodName, "turnRight") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 185 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 185 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 185, 0x01, "HappyBot is turning right..."); }; }while((void)0,0);
    TurnRight();
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 190 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 190 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 190, 0x01, "Stop sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = false;
  }
  else if (strcmp(methodName, "high") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 195 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 195 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 195, 0x01, "Stop sending temperature and humidity data"); }; }while((void)0,0);
    digitalWrite(ledPin, 0x1);
  }
  else if (strcmp(methodName, "soundOn") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 200 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 200 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 200, 0x01, "Sound ON"); }; }while((void)0,0);

    firstSection();
    firstSection();
    secondSection();
    thirdSection();
    thirdSection();
  }
  else if (strcmp(methodName, "optitionalHigh") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 210 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 210 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 210, 0x01, "Pin 12 HIGH"); }; }while((void)0,0);
    digitalWrite(optitionalPin, 0x1);
  }
  else if (strcmp(methodName, "optitionalLow") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 215 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 215 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 215, 0x01, "Pin 12 LOW"); }; }while((void)0,0);
    digitalWrite(optitionalPin, 0x0);
  }
  else if (strcmp(methodName, "soundOff") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 220 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 220 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 220, 0x01, "Sound OFF"); }; }while((void)0,0);
    ledcWrite(channel, 0);
    ledcWriteTone(channel, 0);
  }
  else if (strcmp(methodName, "low") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 226 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 226 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 226, 0x01, "Stop sending temperature and humidity data"); }; }while((void)0,0);
    digitalWrite(ledPin, 0x0);
  }
  else
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 231 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino" 3 4
   __null
# 231 "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\nicolas.mertens\\source\\repos\\VRA.SneakPeek2019\\DarkKnight\\Wifi\\Device\\device.ino", __func__, 231, 0x01, "No method %s found", methodName); }; }while((void)0,0);
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
  L_MOTOR->run(4);

  R_MOTOR->setSpeed(0);
  R_MOTOR->run(4);

  Serial.begin(115200);

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(soundPin, channel);

  pinMode(ledPin, 0x02);
  pinMode(optitionalPin, 0x02);

  Serial.println("ESP32 Device");
  Serial.println("Initializing...");
  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");

  delay(10);
  WiFi.mode(WIFI_MODE_AP);
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
  L_MOTOR->run(1);
  R_MOTOR->run(1);
  maxspeed = 110;
  Serial.println("Forward");
  SpeedUp();
}

void MoveBackward()
{
  isMoving = true;
  L_MOTOR->run(2);
  R_MOTOR->run(2);
  maxspeed = 110;
  Serial.println("Backward");
  SpeedUp();
}

void TurnLeft(){
  isMoving = true;
        L_MOTOR->run(4);
        R_MOTOR->run(1);
        maxspeed = 90;
        Serial.println("Left");
        SpeedUp();
}

void TurnRight(){
   isMoving = true;
        L_MOTOR->run(1);
        R_MOTOR->run(4);
        maxspeed = 90;
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
      L_MOTOR->run(4);
      R_MOTOR->run(4);
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
    char messagePayload[500];

    //   //   if (digitalRead(pirPin) == HIGH)
    //        Serial.println("Sending message to the IoT Hub.");
    //   //   {
    float sensorValue = 1;
    snprintf(messagePayload, 500, messageDataHigh);
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
