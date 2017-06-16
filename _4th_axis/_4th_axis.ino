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
#define STEP_PIN 2
#define DIR_PIN 3
#define ENA_PIN 8

int potValue, oldPot, degVal;
int stepping = 8;
int reduction = 100;
int rpmVal;
long degMov;
bool mode;
String btnStr;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
int hertz = 8000;
int micStep = 8;


void setup(void)
{
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  pinMode(PIN_B3, INPUT_PULLUP);
  pinMode(PIN_SW1, INPUT_PULLUP); // DPDT switch
  Timer1.initialize(1000000 / hertz);
  Timer1.attachInterrupt(stepperRun); // stepper to run at 'hertz'
  stepper1.setPinsInverted(false, false, true);
  stepper1.setMaxSpeed(5000.0);
  stepper1.setAcceleration(5000.0);
  stepper1.setEnablePin(ENA_PIN);

  //stepping = stepping * reduction;
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.println("CNC 4TH AXIS");
  delay(1000);
}

void stepperRun() {
  if (mode == 0) {
    stepper1.run();
  }
  else {
    stepper1.runSpeed();
  }
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
        stepper1.enableOutputs();
        btnStr = degVal;
        btnStr.concat(" ANTICLOCKWISE");
        degMov = moveDegrees(-degVal);
        stepper1.move(degMov);
        Serial.println(degMov);
        break;
      case 2:
        stepper1.disableOutputs();
        Serial.println("STOP");
        btnStr = "STOP";
        break;
      case 3:
        stepper1.enableOutputs();
        btnStr = degVal;
        btnStr.concat(" CLOCKWISE");
        degMov = moveDegrees(degVal);
        stepper1.move(degMov);
        Serial.println(degMov);
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
        stepper1.enableOutputs();
        btnStr = rpmVal;
        btnStr.concat(" ANTICLOCKWISE");
        moveSpeed(-(rpmVal));
        break;
      case 2:
        stepper1.disableOutputs();
        Serial.println("STOP");
        btnStr = "STOP";
        break;
      case 3:
        stepper1.enableOutputs();
        btnStr = rpmVal;
        btnStr.concat(" CLOCKWISE");
        moveSpeed(rpmVal);
        break;
      default:
        break;
    }
    lcd.setCursor(0, 1);
    lcd.println(btnStr);
  }
}
long moveDegrees(long d) {
  d = d * (reduction * stepping);
  return d;
}
void moveSpeed(int s) {
  s = s * reduction;
  stepper1.setSpeed(s);
  Serial.println(s);
}

AdvButton but1 = AdvButton(PIN_B1,  NULL, OnKeyDownBut1, NULL, btn_Digital);
AdvButton but2 = AdvButton(PIN_B2,  NULL, OnKeyDownBut2, NULL, btn_Digital);
AdvButton but3 = AdvButton(PIN_B3,  NULL, OnKeyDownBut3, NULL, btn_Digital);

void loop() {
  ButtonManager::instance()->checkButtons();
  potValue = analogRead(PIN_POT);
  if (stepper1.distanceToGo() == 0) {
    stepper1.disableOutputs();
  }
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
    rpmVal = map(potValue, 0, 1024, 50, 0);
    if (rpmVal != oldPot) {
      lcdDraw(0);
      oldPot = rpmVal;
    }
  }
}



