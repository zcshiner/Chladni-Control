// Chladni Control - Penn State, M E 340
// Zachary Shiner
// April 2014

/*
-------------------------------------------------------------------------------------------
Description:
This program powers the Chladni Plate (Dancing Sand) Exhibit built for the Discovery Space
Children's Science Museum, located in Downtown State College.  http://mydiscoveryspace.org

This code is designed and tested to run on an Adafruit Trinket 5V  http://adafruit.com

How it works:
When the button is pressed (pulled LOW) the following occurs.
* Bring high (turn on) the standby and mute lines separated by a delay to reduce
  amplifier pop
* Turn on the light in the button
The Trinket then waits an amount of time as specified by a potentiometer and turns off
(pulled LOW) the standby and mute lines in the reverse order as outlined above.
After the timer has 'expired' the exhibit is in standby and the button blinks.

-------------------------------------------------------------------------------------------
*/


// Define what pins do what
const byte mute = 4;
const byte standby = 1;
const byte buttonBulb = 0;
const byte buttonPin = 3;
const byte potPin = 1; // Analog and digital pins are numbered differently A1 = D2

// Delay between mute and standby to reduce pop
const unsigned int popDelay = 200; //milliseconds

// Delay to blink the button light
const unsigned int bulbBlink = 2000; //milliseconds, 2.5 seconds

// Upper and Lower limits to the exhibit time delay (in milliseconds)
unsigned long lowerDelay = 10;    // 10 seconds
unsigned long upperDelay = 5*60;  // 5 minutes (in seconds)
unsigned long runTime = 1000;     // initial value of 1000

// Other variables needed for later
unsigned long now;
unsigned long timerA = 0;
unsigned long timerB = 0;
boolean lightState = true;
boolean exhibitState = true;


// Some initial commands on power-up
void setup() {
  // Set specific pins as outputs or inputs
  pinMode(mute, OUTPUT);
  pinMode(standby, OUTPUT);
  pinMode(buttonBulb, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(2, HIGH); // Turn on pullup resistor
  //pinMode(potPin, INPUT);  // This is commented out because pins default to inputs
  delay(150);  // wait for things to settle
  
  // Start with the Amplifier disabled, mute and standby LOW
  digitalWrite(mute, LOW);
  delay(popDelay);
  digitalWrite(standby, LOW);
  
  delay(500);
}

// The main program starts here
void loop() {
  // Check if the pushbutton is pressed
  // If the pin is LOW, the buton is pressed (because of the pull-up resistor)
  if (!digitalRead(buttonPin)) {
    timerA = millis(); // this is now our reference time
    delay(10); // delay 10ms to improve performance
  }
  
  // The great divide! Does the exhibit run or not?
  now = millis();
  if(now - timerA >= runTime)
  {
    // When (now - timer) > the run time, the timer has elapsed.  This is the resting state.
    exhibit(false);  // Make sure the amplifier is turned off by turning off the exhibit
    
    // The button light will blink until someone presses it
    if(now - timerB >= bulbBlink)
    {
      // If the button's light is off, turn it on and vice versa.
      if (lightState == false){
        digitalWrite(buttonBulb, HIGH);
      }else{
        digitalWrite(buttonBulb, LOW);
      }
      lightState = !lightState;  // change the state of the light
      timerB = now;  // reset timerB
    }
  }
  else // When (now - timer) < the run time, the timer is running
  {
    // Turn on the button light
    digitalWrite(buttonBulb, HIGH);
    
    // Call the exhibit function, change its state to true
    exhibit(true);
  }
}


// Exhibit Function
void exhibit(boolean run){
  // If the state is changing, not running to running or vice versa
  if(run != exhibitState){
    // If the exhibit is told to run:
    // Determine how long to run
    unsigned int sensorValue = analogRead(potPin);
    runTime = map(sensorValue, 0, 1023, lowerDelay, upperDelay) * 1000;
    
    //Turn on the amplifier, then unmute
    if(run == true){
      // Unmute and enable the amplifier
      digitalWrite(standby, HIGH);
      delay(popDelay);
      digitalWrite(mute, HIGH);
    }
    // If the exhibit is told to turn off, mute then disable the amplifier
    else{  //run == false
      // Disable and Mute the amplifier
      digitalWrite(mute, LOW);
      delay(popDelay);
      digitalWrite(standby, LOW);
    }
    exhibitState = run;
  // else, no change
  }

}

//*/