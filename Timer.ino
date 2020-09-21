
const int  HeadPin = 6; 
int HeadState = 0;         // current state of the button
//int lastHeadState = 1;     // previous state of the button
//int HeadCounter = 0;

unsigned int tick = 0;
unsigned int condition = 0;
long hours;
long minutes;
long seconds;
unsigned long setTime;
unsigned long lcdtime;

long sec;

const int  MainPin = 7; 
int MainState = 0;         // current state of the button
//int lastMainState = 0;     // previous state of the button
//int MainCounter = 0;


const int  buttonPin = 13;   // the pin that the pushbutton is attached to
const int  UpDownPin = 8;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int UpDownState = 0;   
int lastButtonState = 0;     // previous state of the button
int lastUpDownnState = 0;

int PiezoPin = 9;

unsigned long previousMillis = 0; 
unsigned long currentMillis = 0;
unsigned long diffMillis = 0;

unsigned long debounceDelay = 200;
unsigned long lastDebounceTime = 0; 
unsigned long lastDebounceHeadTime = 0;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// set up a constant for the tilt switch pin
const int switchPin = 6;

// variable to hold the value of the switch pin
int switchState = 0;

// variable to hold previous value of the switch pin
int prevSwitchState = 0;

// a variable to choose which reply from the crystal ball
int reply;


void setup() {
  // put your setup code here, to run once:
  pinMode(MainPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(UpDownPin, INPUT);
  pinMode(HeadPin, INPUT);
  
  // initialize serial communication:
  Serial.begin(9600);
  
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Press to Start");
}

void loop() {
  //Serial.print("tick ");
  //Serial.println(tick);
  //Serial.print("condition ");
  //Serial.println(condition);
  //maintainenance code
  MainState = digitalRead(MainPin);

  if (MainState == HIGH ) {
   if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();  // reset the debouncing timer
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset");
    condition = 0;
    } 
  } 

  HeadState = digitalRead(HeadPin);
  if (HeadState == HIGH ) {
    if ((millis() - lastDebounceHeadTime) > debounceDelay) {
      lastDebounceHeadTime = millis();
      condition++;
      if (condition == 5){
        condition = 4;
      }
    }
  }

  switch (condition) {
  
    case 0:
      //Reset variables
      tick = 0;
      buttonPushCounter = 0;
      condition = 1;
      break;
  
    case 1:
      // Set time
      lcdDisplay ();
      break;
  
    case 2:
      // calculations
      pressbutton();
      break;
  
    case 3:
      ///conversion
      sec = buttonPushCounter*1000;
    
      if (tick == 0) {
        hours = 3600*sec;
        condition = 1;
        tick++;
        buttonPushCounter = 0;
         //Serial.print("Set Hours equals ");
         //Serial.println(hours);
      } else if (tick == 1){
        minutes = 60*sec;
        condition = 1;
        tick++;
        buttonPushCounter = 0;
         //Serial.print("Set Mins equals ");
         //Serial.println(minutes);
        
      } else if (tick == 2) {
          if (sec > 0) {
            seconds = sec;  
          
            //Serial.print("Set Secs equals ");
            //Serial.println(seconds);
            
            previousMillis = millis();
            diffMillis = millis();
          
            setTime = hours + minutes + seconds;
            lcdtime = setTime/1000;
            //Serial.print("Set LCd conter time equals ");
            //Serial.println(lcdtime);
            condition = 4 ;

            // First print
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Timer On");
            lcd.setCursor(0, 1);
            lcd.print(lcdtime);

      } else {
        // If seconds is less than or equal to zero
        condition = 1;
        tick = 2;
      }  
    }
    break;    // "break" for case 3
    
    case 4:
      // LCD display
      if (setTime > 0){
        Serial.print("Set time");
        Serial.println(setTime);
        
        remaining ();
   
        if (currentMillis - previousMillis  >= setTime) {
          lcdtime = 0;
          lcd.setCursor(0, 1);
          lcd.print(lcdtime);
          lcd.setCursor(0, 0);
          lcd.print("Timer Off");
          lcd.setCursor(0, 1);
          lcd.print("0");
  
          alarm ();
  
          condition = 0 ;
        }
      }
      break;
  }
}   // end of the "loop" function

///===================================================================

void lcdDisplay () {
  switch (tick) {
    case 0:
      // Hours
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Hours");
//      Serial.println("hours");
      
      condition = 2;
      break;
    
    case 1:
      // min
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Mins");
//      Serial.println("min");
      
      condition = 2;
      break;
    
    case 2:
      // sec
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Sec");
      
      condition = 2;
      break;
  }
}

///// -----------------------------------------------------
    
 void remaining() {
  currentMillis = millis();

  if (currentMillis - diffMillis >= 1000) {
    lcd.setCursor(0, 1);
    lcdtime--;
    lcd.print(lcdtime);
    diffMillis = currentMillis;
  }
  // end of the function "remaining"          
}   

   
void alarm (){      
  digitalWrite(PiezoPin, HIGH);
  Serial.println("buzzer ringing");
  tone(9, 2000, 2000);
  
  delay (1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Reset");

  delay (1000);
}
 
///////////////////////////////-----------------------------------
void pressbutton () {
  //Serial.println ("In the Press button loop");
  buttonState = digitalRead(buttonPin);
  UpDownState = digitalRead(UpDownPin);
      
 
  // compare the buttonState to its previous state
      if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
      if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
      
    } 
    // Delay a little bit to avoid bouncing
    //delay(25);
  }
    lastButtonState = buttonState;

 if (UpDownState !=lastUpDownnState) {
    // if the state has changed, increment the counter
    if (UpDownState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      
        if (buttonPushCounter > 0){
          buttonPushCounter--;
        }
        else {
          buttonPushCounter =0;
        }

       // buttonPushCounter = (buttonPushCounter > 0) ? buttonPushCounter-1 : 0;
        
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
      
    } 
    // Delay a little bit to avoid bouncing
    //delay(10);
   }
  lastUpDownnState = UpDownState;
  
  lcd.setCursor(0, 1);
  lcd.print(buttonPushCounter);
}
