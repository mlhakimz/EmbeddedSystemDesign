#include <Wire.h>                                           //include I2C library for LCD
#include <LiquidCrystal_I2C.h>                              //include I2C LCD library
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);     //define an object named lcd using I2C LCD library

int button1pin = 2;                                         //Eat button pin
int button2pin = 3;                                         //Next round button pin
int ledgpin = 5;                                            //Green LED (lvl 1) pin
int ledypin = 6;                                            //Yellow LED (lvl 2) pin
int ledrpin = 9;                                            //Red LED (lvl 3) pin
int potpin = A0;                                            //potentiometer pin
int pwmg;                                                   //pwm values based on score or progress to next level
int pwmy;
int pwmr;

unsigned long currentmillis;                                //timing variables to refresh the LCD at refreshrate value
unsigned long prevmillis = 0;
unsigned long last_interrupt0_time = 0;                     //timing variables to debounce buttons
unsigned long last_interrupt1_time = 0;
int refreshrate = 100;                                      //refresh LCD approximately every 100ms

int charpos = 0;                                            //position variable for food and character
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int type1 = 0;                                              //type variable for food
int type2 = 0;
int type3 = 0;
int type4 = 0;
bool flag1 = 0;                                             //flag variable for food either eaten or not
bool flag2 = 0;
bool flag3 = 0;
bool flag4 = 0;
bool flag5 = 0;                                             //flag variable to generate new food positions and types
bool flag6 = 0;                                             //flag variable to display level introduction
bool flag7 = 0;                                             //flag for level 2
bool flag8 = 0;                                             // flag for level 3

int score = 0;
int count = 0;                                              //count for eaten food every round, refreshes every round.
int gameround = 1;                                          //number of game round
int level = 1;


void setup() {
  pinMode(button1pin, INPUT_PULLUP);
  pinMode(button2pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button1pin), ISR0, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2pin), ISR1, FALLING);
  pinMode(potpin, INPUT);
  pinMode(ledgpin, OUTPUT);
  pinMode(ledypin, OUTPUT);
  pinMode(ledrpin, OUTPUT);
  lcd.init();
  lcd.backlight();

  displaystart();   //Display welcome, game intro, controls, and type of foods.
}

void ISR0()         //interrupt function for eat button
{
  unsigned long interrupt0_time = millis();
  if (interrupt0_time - last_interrupt0_time > 300)     //ignore interrupts faster than 300ms from previous interrupt
  {
    if (charpos == pos1)                  //if the character position is the same as food,
    {
      if (flag1 == 0)                     //to avoid counting adding up score more than once
        score += type1;                   //add to score based on type
      count++;                            //count eaten food
      flag1 = 1;                          //set flag for food eaten
    }
    if (charpos == pos2)
    {
      if (flag2 == 0)
      score += type2;
      count++;
      flag2 = 1;
    }
    if (charpos == pos3)
    {
      if (flag3 == 0)
      score += type3;
      count++;
      flag3 = 1;
    }
    if (charpos == pos4)
    {
      if (flag4 == 0)
      score += type4;
      count++;
      flag4 = 1;
    }
  }
  last_interrupt0_time = interrupt0_time;
}

void ISR1()                     //interrupt function for next round
{
  unsigned long interrupt1_time = millis();
  if (interrupt1_time - last_interrupt1_time > 300)     //ignore interrupts faster than 300ms from previous interrupt
  {
    if (level == 1)                   //in level 1,
    {
      if (count > 2)                  //need to eat at least 3 food to go next round
      {
        gameround++;
        flag1 = flag2 = flag3 = flag4 = flag5 = 0;
        count = 0;
      }
    }
    else if (level == 2)              //in level 2,
    {
      if (count > 1)                  //need to eat at least 2 food to go next round
      {
        gameround++;
        flag1 = flag2 = flag3 = flag4 = flag5 = 0;
        count = 0;
      }
    }
    else
    {
      if (count > 0)                  //need to eat at least 1 food to go next round (level 3)
      {
        gameround++;
        flag1 = flag2 = flag3 = flag4 = flag5 = 0;
        count = 0;
      }
    }
  }
  last_interrupt1_time = interrupt1_time;
}

void displaystart()         //function for welcome messages, controls and food types
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eat as much as");
  lcd.setCursor(0, 1);
  lcd.print("you can b4 time!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Knob = move");
  lcd.setCursor(0, 1);
  lcd.print("Button = eat");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X=3pts");
  lcd.setCursor(9, 0);
  lcd.print("x=2pts");
  lcd.setCursor(0, 1);
  lcd.print("O=1pts");
  lcd.setCursor(9, 1);
  lcd.print("o=0pts");
  delay(2000);
}

void generatefood()                   //generate random food positions and types
{
  pos1 = random(0, 4);
  pos2 = random(4, 8);
  pos3 = random(8, 12);
  pos4 = random(12, 16);
  type1 = random(0, 4);
  type2 = random(0, 4);
  type3 = random(0, 4);
  type4 = random(0, 4);
}

void levelintro()                         // display level introduction when leveling up
{
  if (level == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LEVEL 1");
    lcd.setCursor(0, 1);
    lcd.print("Slow eater");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Can skip round");
    lcd.setCursor(0, 1);
    lcd.print("after 3 eats");
    delay(2000);
  }
  else if (level == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LEVEL UP! (2)");
    lcd.setCursor(0, 1);
    lcd.print("Can eat faster");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Can skip round");
    lcd.setCursor(0, 1);
    lcd.print("after 2 eats");
    delay(2000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LEVEL UP! (3)");
    lcd.setCursor(0, 1);
    lcd.print("Eat even faster");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Can skip round");
    lcd.setCursor(0, 1);
    lcd.print("after 1 eat");
    delay(2000);
  }
}

void displayitem()                    //display food and character
{
  lcd.clear();
  lcd.setCursor(pos1, 0);
  if (flag1)
    lcd.print(".");
  else
    displayfood(type1);

  lcd.setCursor(pos2, 0);
  if (flag2)
    lcd.print(".");
  else
    displayfood(type2);

  lcd.setCursor(pos3, 0);
  if (flag3)
    lcd.print(".");
  else
    displayfood(type3);

  lcd.setCursor(pos4, 0);
  if (flag4)
    lcd.print(".");
  else
    displayfood(type4);

  lcd.setCursor(charpos, 1);
  if (level == 1)
  {
    lcd.print(".");
  }
  else if (level == 2)
  {
    lcd.print("x");
  }
  else
    lcd.print("X");
}

void displayfood(int type)
{
  if (type == 0)
    lcd.print("o");
  if (type == 1)
    lcd.print("O");
  if (type == 2)
    lcd.print("x");
  if (type == 3)
    lcd.print("X");
}

void displayscore()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TIME'S UP!");
  lcd.setCursor(0, 1);
  lcd.print("SCORE: ");
  lcd.setCursor(9, 1);
  lcd.print(score);
}

void loop() {
  charpos = map(analogRead(potpin), 0, 1023, 0, 16);            //map potentiometer value from 0 to 15
  currentmillis = millis();
  if (currentmillis - prevmillis >= refreshrate) {              //refresh every 100ms(refreshrate value)
    if (millis() < 90000)                                       //if time still does not run out, keep playing
    {
      if (flag5 == 0)                                           //to avoid generating food position and types every cycle.
      {
        generatefood();
        flag5 = 1;
      }
      if (flag6 == 0)                                           //to avoid looping level introduction display messages
      {
        levelintro();
        flag6 = 1;
      }
      if (flag1 && flag2 && flag3 && flag4)                     //automatically move on to next round when all food is eaten in that round
      {
        gameround++;
        flag1 = flag2 = flag3 = flag4 = flag5 = 0;
      }
      if (score >= 20)                                          //if score is more than 20, level up to level 2
      {
        level = 2;
        if (flag7 == 0)                                         //to avoid resetting the other flags every cycle after score is more than 20
        {
          flag1 = flag2 = flag3 = flag4 = flag5 = flag6 = 0;
          flag7 = 1;
          count = 0;
        }
      }

      if (score >= 40)                                          //if score is more than 40, level up to level 3
      {
        level = 3;
        if (flag8 == 0)                                         //to avoid resetting the other flags every cycle after score is more than 40
        {
          flag1 = flag2 = flag3 = flag4 = flag5 = flag6 = 0;
          flag8 = 1;
          count = 0;
        }
      }
      displayitem();
      pwmg = map(score, 0, 20, 50, 255);                        //map green LED pwm with level 1 progress
      pwmy = map(score, 20, 40, 50, 255);                       //map yellow LED pwm with level 2 progress
      pwmr = map(score, 40, 60, 50, 255);                       //map red LED pwm with level 3 progress
      if (level == 1)
      {
        analogWrite(ledgpin, pwmg);                             //light up green led, off others
        digitalWrite(ledypin, LOW);
        digitalWrite(ledrpin, LOW);
      }
      if (level == 2)
      {
        digitalWrite(ledgpin, LOW);
        analogWrite(ledypin, pwmy);                             //light up yellow led, off others
        digitalWrite(ledrpin, LOW);
      }
      if (level == 3)
      {
        digitalWrite(ledgpin, LOW);
        digitalWrite(ledypin, LOW);
        analogWrite(ledrpin, pwmr);                             //light up red led, off others
      }
    }
    else
      displayscore();                                           //display score after time runs out.
    prevmillis = currentmillis;
  }
}
