#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

void TaskSendData( void *pvParameters );
void TaskReadSensor( void *pvParameters );
void TaskMonitor( void *pvParameters );

LiquidCrystal_I2C lcd(0x3F,16,2); 

const int trig = 13;
const int d1 = 12;
const int d2 = 11;
const int r1 = 10;
const int r2 = 9;
const int cao = 8;

long duration, chieudai, chieurong, chieucao;

#define Rx 3
#define Tx 2
SoftwareSerial UnoSerial(Rx, Tx);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

void setup()
{
    //===============//
  pinMode(trig, OUTPUT);
  pinMode(d1, INPUT);
  pinMode(d2, INPUT);
  pinMode(r1, INPUT);
  pinMode(r2, INPUT);
  pinMode(cao, INPUT);
  pinMode(Rx, INPUT);
  pinMode(Tx, OUTPUT);
    //===============//
  Serial.begin(115200);
  UnoSerial.begin(57600);
  lcd.init();                    
  lcd.backlight();
  lcd.createChar(0, heart);
  lcd.setCursor(0,0);
  lcd.print("Nhom 11");
  lcd.setCursor(7,0);
  lcd.write((byte)0);
  lcd.setCursor(0,1);
  lcd.print("Xin chao cac ban");
    //===============//
  while (!Serial) {
    ;
  }

  xTaskCreate(TaskReadSensor, "ReadSensor", 128, NULL, 1, NULL);
  xTaskCreate(TaskMonitor, "Monitor", 128, NULL, 1, NULL);
  xTaskCreate(TaskSendData, "SendData", 128, NULL, 1, NULL);
}

void loop() {}

void TaskReadSensor(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task ReadSensor is working");

    //===============================//
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(d1, HIGH);
    chieudai += (duration / 29) / 2;
    //===============================//
    Serial.println(chieudai);
    //===============================//
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(d2, HIGH);
    chieudai += int((duration / 29) / 2);
    //===============================//
    //===============================//
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(r1, HIGH);
    chieurong += int((duration / 29) / 2);
    //===============================//
    //===============================//
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(r2, HIGH);
    chieurong += int((duration / 29) / 2);
    //===============================//
    //===============================//
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(cao, HIGH);
    chieucao = int((duration / 29) / 2);
    /*/===============================//
    Serial.print("Chieu dai: ");
    Serial.println(chieudai);
    Serial.print("Chieu rong: ");
    Serial.println(chieurong);
    Serial.print("Chieu cao: ");
    Serial.println(chieucao);
    //===============================/*/
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void TaskMonitor(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task Monitor is working");
    Serial.print("Sensor's data : ");
    char buffer[40];
    sprintf(buffer, "%d,%d,%d", chieudai, chieurong, chieucao);
    Serial.println(buffer);
    Serial.print("Chiều dài : ");
    Serial.print(chieudai);
    Serial.println(" cm");
    Serial.print("Chiều rộng : ");
    Serial.print(chieurong);
    Serial.println(" cm");
    Serial.print("Chiều cao : ");
    Serial.print(chieucao); 
    Serial.println(" cm");
    vTaskDelay(3000 / portTICK_PERIOD_MS);

  }
}

void TaskSendData(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task SendData is working");
    Serial.print("send >> ");
    char buffer[40];
    sprintf(buffer, "%d,%d,%d", chieudai, chieurong, chieucao);
    UnoSerial.print(buffer);
    UnoSerial.print("\n");
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
