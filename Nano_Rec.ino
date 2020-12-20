#include <SPI.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define MAIN_WING_ROTATE_SERVO 11
#define RARE_WING_UP_DOWN_SERVO 9
#define RARE_WING_LEFT_RIGHT_SERVO 10
#define ESC_MOTOR 3
#define MIN_PULSE_WIDTH 1000
#define MAX_PULSE_WIDTH 2000
#define SETUP_SLEEP 300
#define SERVO_LOWER_ANGLE 45
#define SERVO_HIGHER_ANGLE 135
#define ANALOG_READ_LOWEST_VAL 0
#define ANALOG_READ_HIGHEST_VAL 1023
#define ESC_MAX 180
#define ESC_MIN 0

struct dataSend
{
  int Rx;
  int Ry;
  int Ly;
  int Lx;
  int pot;

  dataSend(){}
};

RF24 radio(7, 8); //csn, ce
const uint64_t address = 1234;

Servo MainWing;
Servo rareUpDown;
Serwo rareLeftRight;
Servo ESC;

void setup() {
  Serial.begin(115200);
  //RADIO
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  radio.setAutoAck(false);
  Serial.println("START_RADIO");

  //SERVO
  MainWing.attach(MAIN_WING_ROTATE_SERVO);
  rareUpDown.attach(RARE_WING_UP_DOWN_SERVO);
  rareLeftRight.attach(RARE_WING_LEFT_RIGHT_SERVO);
  ESC.attach(ESC_MOTOR, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  MainWing.write(0);
  rareUpDown.write(0);
  rareLeftRight.write(0);
  delay(SETUP_SLEEP);
}

void loop()
{
  if (radio.available())
  {
    /*
        Serial.println("Radio available");
        dataSend d;
        radio.read(&d, sizeof(d));
        Serial.println(d.Rx);
        Serial.println(d.Ry);
        Serial.println(d.Ly);
        Serial.println(d.pot);
    */
        MainWing.write(map(d.Ry, ANALOG_READ_LOWEST_VAL, ANALOG_READ_HIGHEST_VAL, SERVO_LOWER_ANGLE, SERVO_HIGHER_ANGLE));
        rareUpDown.write(map(d.Rx, ANALOG_READ_LOWEST_VAL, ANALOG_READ_HIGHEST_VAL, SERVO_LOWER_ANGLE, SERVO_HIGHER_ANGLE));
        rareLeftRight.write(map(d.Lx, ANALOG_READ_LOWEST_VAL, ANALOG_READ_HIGHEST_VAL, SERVO_LOWER_ANGLE, SERVO_HIGHER_ANGLE));
        ESC.write(map(d.pot, ANALOG_READ_LOWEST_VAL, ANALOG_READ_HIGHEST_VAL, ESC_MIN, ESC_MAX));
        delay(5);
  }
  else
  {
    Serial.println("LOST CONNECTION");
    turnAroundMonuver();
  }
}

void turnAroundMonuver();
{
  ESC.write(0);
  //rest implement when gyroscope installed
}
