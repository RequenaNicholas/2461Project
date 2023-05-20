#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
Stepper motor(200, IN1, IN2, IN3, IN4);

// Timer variables
unsigned long coin_tmr = 0;

// Data pins for IR sensors
const int IRpin_1c = 2;
const int IRpin_5c = 3;
const int IRpin_10c = 4;
const int IRpin_25c = 5;
const int IRpin_100c = 6;


// Read values on the IR sensor
int IRvalue_1c = 0;
int IRvalue_5c = 0;
int IRvalue_10c = 0;
int IRvalue_25c = 0;
int IRvalue_100c = 0;

// Coin Counters
int coinCount = 0;
int moneyCount = 0;
int moneyWorth = 0;



void setup() {
  Serial.begin(9600);

  lcd.init();           // Initialize display
  lcd.clear();          // Clear Display
  lcd.backlight();      // Make sure backlight is on

  lcd.setCursor(0, 0);            // Set cursor to character 0 on line 0
  lcd.print("Init. Success");  // Print on char. 0, line 0.
  lcd.setCursor(2, 1);            // Set cursor to character 2 on line 1 

  pinMode(IRpin_1c, INPUT);
  pinMode(IRpin_5c, INPUT);
  pinMode(IRpin_10c, INPUT);
  pinMode(IRpin_25c, INPUT);
  pinMode(IRpin_100c, INPUT);

  digitalWrite(IRpin_1c, HIGH);   //All IR Sensors run in PULL-UP configuration.
  digitalWrite(IRpin_5c, HIGH);
  digitalWrite(IRpin_10c, HIGH);
  digitalWrite(IRpin_25c, HIGH);
  digitalWrite(IRpin_100c, HIGH);
}


void loop() {
  motor.step(200);

  timers();

  IRvalue_1c = digitalRead(IRpin_1c);
  IRvalue_5c = digitalRead(IRpin_5c);
  IRvalue_10c = digitalRead(IRpin_10c);
  IRvalue_25c = digitalRead(IRpin_25c);
  IRvalue_100c = digitalRead(IRpin_100c);


// Check every 300ms if an IR sensor has detected anything
// When the IR sensor detects an obstacle, increase coinCount
// Add the respective IR sensor's value to the moneyWorth
// Print coinCount and moneyWorth on the serial monitor

  if (coin_tmr > 3)       // Runs every 300ms
  {
    
    // IR sensor for Penny
    if (IRvalue_1c == 0)  
    { // IR sensor == 0 when an obstacle is detected
      Serial.println("Penny has been detected");
      coinCount++;               // Increases coin count (in total, not specifically for pennies)
      moneyWorth = moneyWorth + 1;  // Increases the total worth by $0.01
      displayAmountSerial();        // Displays current total on Serial Monitor
      displayAmountLCD(moneyWorth); // Displays current total on LCD screen
      coin_tmr = 0;                 // Resets timer
    }

    else if (IRvalue_5c == 0)
    { // See example on Penny IR Sensor for details.
      Serial.println("Nickel has been detected");
      coinCount++;
      moneyWorth = moneyWorth + 5;
      displayAmountSerial();
      displayAmountLCD(moneyWorth);
      coin_tmr = 0;
    }

    else if (IRvalue_10c == 0)
    { // See example on Penny IR Sensor for details.
      Serial.println("Dime has been detected");
      coinCount++;
      moneyWorth = moneyWorth + 10;
      displayAmountSerial();
      displayAmountLCD(moneyWorth);
      coin_tmr = 0;
    } 
    else if (IRvalue_25c == 0)
    { // See example on Penny IR Sensor for details.
      Serial.println("Quarter has been detected");
      coinCount++;
      moneyWorth = moneyWorth + 25;
      displayAmountSerial();
      displayAmountLCD(moneyWorth);
      coin_tmr = 0;
    } 
    else if (IRvalue_100c == 0)
    { // See example on Penny IR Sensor for details.
      Serial.println("Dollar Coin has been detected");
      coinCount++;
      moneyWorth = moneyWorth + 100;
      displayAmountSerial();
      displayAmountLCD(moneyWorth);
      coin_tmr = 0;
    } 
  }

}


void displayAmountSerial()
{
// Function to display coin info via. Serial Monitor
// Print the value of coinCount--the # of coins that has been detected
// Print the value of moneyWorth--the value of the coins added up
  Serial.print("So that's ");
  Serial.print(coinCount);
  Serial.print(" coins now and the total value counted is ");
  Serial.print(moneyWorth);
  Serial.println("");
  Serial.print("The total amount is: $");
  Serial.println(convertUSD(moneyWorth));
  Serial.println();
}

void displayAmountLCD(float moneyNumber)
{
  int detectionCount = 0;      

  lcd.setCursor(0, 0);  // Set cursor to character 0 on line 0
  lcd.print("Total amount is:");
  lcd.setCursor(2, 1);   // Set cursor to character 2 on line 1 (to put money)
  
  moneyNumber = convertUSD(moneyNumber);
  lcd.print("$");
  lcd.print(moneyNumber);
  detectionCount++;

  if (detectionCount > 2)   // Clears the display starting after the second IR detection.
    lcd.clear();            // This is to 'update' the LCD display by clearing the old                         
                            // total amount, and displaying the new total.          

}

void timers(void)
{
// Create: "unsigned long timer_name = 0" at start of program
// Put 'timer_name++' under "if(one_ms_timer > 99)"
// Declare timers() function at beginning of loop to run every 100ms
  static unsigned long ms_runtime = 0;
  static unsigned long one_ms_timer = 0;

  if (millis() > ms_runtime)
  {
    ms_runtime++;
    one_ms_timer++;
  }

  else if (ms_runtime > millis())
  {
    ms_runtime = millis();
    one_ms_timer++;
  }

  if(one_ms_timer > 99)
  { //runs every 100ms
    //put new timers here:
    coin_tmr++;
    one_ms_timer = 0;
  }
}

float convertUSD(float moneyNumber)
{
// Take the moneyNumber--the total cents detected
// divide that by 100 to convert to USD dollars
// return the dollar count
  float cents = moneyNumber;
  float dollar = cents/100;
  
  return dollar;
}
