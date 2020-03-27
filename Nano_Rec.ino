#include <SPI.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

struct dataSend
{
  int Rx;
  int Ry;
  int Ly;
  int Lx;
  int pot;

  dataSend(int rx, int ry, int lx, int ly, int p)
  :Rx(rx),
  Ry(ry),
  Lx(lx),
  Ly(ly),
  pot(p)
  {}
  dataSend(){}
};

RF24 radio(7, 8); //csn, ce
const uint64_t address = 1234;

Servo servoMainWing;

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
  //servoMainWing.attach(5);

}
int i = 1;
void loop()
{
  //  delay(2000);
  //  if (i%4 == 0)  servoMainWing.write(80);
  //  //else if (i%3 ==0) servoMainWing.writeMicroseconds(2000);
  //  else servoMainWing.write(0);
  //  delay(15);
  // ++i;


  if (radio.available())
  {
         Serial.println("Radio available");
        dataSend d;
       // int msg[3];
        radio.read(&d, sizeof(d));
        Serial.println(d.Rx);
        Serial.println(d.Ry);
        Serial.println(d.Ly);
        Serial.println(d.pot);


//    char text[32] = {0};
//    radio.read(&text, sizeof(text));
//    Serial.println(text);
   delay(5);
  }
}
