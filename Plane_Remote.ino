#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define joyRightUpDown A0
#define joyRightRotate A1
#define joyLeftY A2
#define joyLeftX A3
#define potentiometer A4
#define joyLeftButton 9
#define MENUPOSMAX 3
#define LOW_TRIGGER 200
#define HIGH_TRIGGER 800
#define MICRO_SLEEP 15
#define QUICK_SLEEP 300
#define NORMAL_SLEEP 500


RF24 radio(7, 8);
const uint64_t address = 1234;

void setup() {
  Serial.begin(9600);
  radio.begin();
  //radio.setRetries(15, 15);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  pinMode(joyLeftButton, INPUT);
  digitalWrite(joyLeftButton, HIGH);
}

void loop() {

  int RxValue = analogRead(joyRightUpDown);
  int RyValue = analogRead(joyRightRotate);
  int LyValue = analogRead(joyLeftY);
  int LxValue = analogRead(joyLeftX);
  int pot = analogRead(potentiometer);
  bool buttonLeft = digitalRead(joyLeftButton);

  //  int msg[3];
  //  msg[0] = RxValue;
  //  msg[1] = RyValue;
  //  msg[2] = LyValue;
  //  msg[3] = pot;

  // radio.write(&msg, sizeof(msg));
  // const char text[] = "sygnal";
  // radio.write(&text, sizeof(text));
  delay(100); //tmp delay
  //  Serial.println(RxValue);
  //  Serial.println(RyValue);
  Serial.println(LyValue);
  Serial.println(LxValue);
  Serial.println(buttonLeft);

  if (!buttonLeft)
  {
    menu();
  }

}

void menu()
{
  delay(600);
  bool button = true;
  int upDown = analogRead(joyLeftY);
  int leftRight = analogRead(joyLeftX);
  int menuPos = 1;

  displayMenu(menuPos);

  while (button)
  {
    upDown = analogRead(joyLeftY);
    leftRight = analogRead(joyLeftX);

    while (LOW_TRIGGER < upDown && upDown < HIGH_TRIGGER && button)
    {
      delay(MICRO_SLEEP);
      upDown = analogRead(joyLeftY);
      leftRight = analogRead(joyLeftX);
      button = digitalRead(joyLeftButton);

      if (LOW_TRIGGER > leftRight || HIGH_TRIGGER < leftRight)
      {
        if (1 == menuPos)
        {
          switch_RF_PA_LvL();
          displayMenu(menuPos);
        }
      }
    }

    if (HIGH_TRIGGER < upDown) //change option Down
    {
      if (menuPos == 1) menuPos = MENUPOSMAX;
      else --menuPos;
      displayMenu(menuPos);
    }
    else if (LOW_TRIGGER > upDown) //change option Up
    {
      if (menuPos == MENUPOSMAX) menuPos = 1;
      else ++menuPos;
      displayMenu(menuPos);
    }
  }
  delay(1000);
  button = digitalRead(joyLeftButton);

}

void displayMenu(int x)
{
  switch (x)
  {
    case 1:
      {
        Serial.println("SET_RF_POWER");
        delay(QUICK_SLEEP);
        break;
      }
    case 2:
      {
        Serial.println("Opcja 2");
        delay(QUICK_SLEEP);
        break;
      }
    case 3:
      {
        Serial.println("Opcja 3");
        delay(QUICK_SLEEP);
        break;
      }
  }
}

void switch_RF_PA_LvL()
{

  uint8_t pick = radio.getPALevel();
  Serial.print("Current RF_PA_LVL: ");
  Serial.println(pick);
  delay(NORMAL_SLEEP);

  bool button = digitalRead(joyLeftButton);
  int rightLeft = analogRead(joyLeftX);

  while (button)
  {
    while (LOW_TRIGGER < rightLeft && rightLeft < HIGH_TRIGGER && button)
    {
      rightLeft = analogRead(joyLeftX);
      button = digitalRead(joyLeftButton);

      if (LOW_TRIGGER > analogRead(joyLeftY)) //up
      {
        if (menuPos == MENUPOSMAX) menuPos = 1;
        else ++menuPos;
      }
      else if (HIGH_TRIGGER < analogRead(joyLeftY))
      {

      }
      delay(15);
    }
    if (LOW_TRIGGER > rightLeft) // Up ++
    {
      if (3 <= pick) pick = RF24_PA_MIN;
      else ++pick;
      Serial.print("RF_PA_LVL: ");
      Serial.println(pick);
    }
    else if (HIGH_TRIGGER < rightLeft) //Down --
    {
      if (0 == pick) pick = RF24_PA_MAX;
      else --pick;
      Serial.print("RF_PA_LVL: ");
      Serial.println(pick);
    }
    delay(QUICK_SLEEP);
    rightLeft = analogRead(joyLeftX);
  }
  radio.setPALevel(pick);
  Serial.print("RF_PA SET TO: ");
  Serial.println(radio.getPALevel());
  delay(NORMAL_SLEEP);
}
