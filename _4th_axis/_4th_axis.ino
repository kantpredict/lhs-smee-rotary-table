#include <AdvButton.h>
#include <ButtonManager.h>
#include <TimerOne.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <LiquidCrystal_I2C.h>

#define PIN_B1 4 //3 buttons
#define PIN_B2 5
#define PIN_B3 6
#define PIN_SW1 7 //two-position switch
#define PIN_POT A0 //setting pot

int potValue;
int oldPot;
int degVal;
int rpmVal;
bool mode;
String btnStr;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
AccelStepper stepper1(AccelStepper::DRIVER, 2, 3);
int hertz = 1;
float micStep = 8;


void setup(void)
{
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  pinMode(PIN_B3, INPUT_PULLUP);
  pinMode(PIN_SW1, INPUT_PULLUP); // DPDT switch

  Timer1.initialize(1000000 / hertz);
  Timer1.attachInterrupt(stepperRun); // stepper to run at 'hertz'
  stepper1.setMaxSpeed(10000.0);
  stepper1.setAcceleration(10000.0);
  stepper1.setEnablePin(4);
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.println("CNC 4TH AXIS");
  delay(1000);
}

void stepperRun() {
  stepper1.run();
}

void OnKeyDownBut1(AdvButton* but) {
  btnStr = "";
  lcdDraw(1);
}

void OnKeyDownBut2(AdvButton* but)  {
  btnStr = "";
  lcdDraw(2);
}

void OnKeyDownBut3(AdvButton* but) {
  btnStr = "";
  lcdDraw(3);
}

void lcdDraw(int but) {
  lcd.clear();

  if (mode == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Degrees: ");
    lcd.println(degVal);
    lcd.setCursor(0, 1);
    lcd.println(btnStr);
    switch (but) {
      case 1:
        btnStr = degVal;
        btnStr.concat(" ANTICLOCKWISE");
        break;
      case 2:
        btnStr = degVal;
        btnStr = "STOP";
        break;
      case 3:
        btnStr = degVal;
        btnStr.concat(" CLOCKWISE");
        break;
      default:
        break;
    }
    lcd.setCursor(0, 1);
    lcd.println(btnStr);
  }
  if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Set RPM = ");
    lcd.println(rpmVal);

    switch (but) {
      case 1:
        btnStr = rpmVal;
        btnStr.concat(" ANTICLOCKWISE");
        break;
      case 2:
        btnStr = "STOP";
        break;
      case 3:
        btnStr = rpmVal;
        btnStr.concat(" CLOCKWISE");
        break;
      default:
        break;
    }
    lcd.setCursor(0, 1);
    lcd.println(btnStr);
  }
}
AdvButton but1 = AdvButton(PIN_B1,  NULL, OnKeyDownBut1, NULL, btn_Digital);
AdvButton but2 = AdvButton(PIN_B2,  NULL, OnKeyDownBut2, NULL, btn_Digital);
AdvButton but3 = AdvButton(PIN_B3,  NULL, OnKeyDownBut3, NULL, btn_Digital);

void loop() {

  ButtonManager::instance()->checkButtons();
  potValue = analogRead(PIN_POT);

  if (digitalRead(PIN_SW1) == LOW) { //STEP MODE
    mode = 0;
    degVal = map(potValue, 0, 1024, 90, 0);
    if (degVal != oldPot) {
      lcdDraw(0);
      oldPot = degVal;
    }
  }

  else { //CONTINUOUS MODE
    mode = 1;
    rpmVal = map(potValue, 0, 1024, 60, 0);
    if (rpmVal != oldPot) {
      lcdDraw(0);
      oldPot = rpmVal;
    }
  }

}



