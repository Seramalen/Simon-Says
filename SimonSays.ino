//Last modified August 9, 2024, Adalin Zimmermann
//Light order on the panel is GBYR
int redLed = 13;
int greenLed = 12;
int yellowLed = 11;
int blueLed = 10;
int redButton = 9;
int greenButton = 8;
int yellowButton = 7;
int blueButton = 6;
int ledOne = 4;
int ledTwo = 3;
int ledThree = 2;
int timer = 0;
int phase = 1;
int activeLight = 0;
int maglock = 5;
int initialKey = 0;

void setup()
{  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(maglock, OUTPUT);
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
  pinMode(redButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);
  pinMode(yellowButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
}


//Helper function to toggle all color lights on or off. True means ON
void toggleLights(bool onOff)
{
  if(onOff)
  {
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(yellowLed, HIGH);
  digitalWrite(redLed, HIGH);
  }
  else
  {
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  }
}

//Correct code entered light sequence
//Should light lights in order from left to right to left
void correctLightSequence()
{
  toggleLights(false);
  for(int i = 0; i<=2; i++)
  {
    digitalWrite(greenLed, HIGH);
    delay(250);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, HIGH);
    delay(250);
    digitalWrite(blueLed, LOW);
    digitalWrite(yellowLed, HIGH);
    delay(250);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
    delay(250);
    digitalWrite(redLed, LOW);
    delay(250);
    digitalWrite(yellowLed, HIGH);
    delay(250);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, HIGH);
    delay(250);
    digitalWrite(blueLed, LOW);
    digitalWrite(greenLed, HIGH);
    delay(250);
    digitalWrite(greenLed, LOW);
    delay(250);
  }
}

//Sequence for an incorrect button press
void incorrectLightSequence()
{
  for(int i = 0; i<=5; i++)
  {
    toggleLights(true);
    delay(500);
    toggleLights(false);
    delay(500);
  }
}


//helper function that activates a specific light based on input int and entered code type
//0 lights was added to allow the program to still check for button input while the lights were off
//Because this method automatically turns the last light off at the end, the light 0 function works as intended
bool nextLight(int light, int codeNum)
{
  if(light == 1)
  {
    //Green
    activeLight = greenLed;
  }
  else if(light == 2) 
  {
    //Blue
    activeLight = blueLed;
  }
  else if(light == 3) 
  {
    //Yellow
    activeLight = yellowLed;    
  }
  else if(light == 4) 
  {
    //Red    
    activeLight = redLed;
  }
  else if(light == 0)
  {
    //All off
    activeLight = 1;
  }
    timer = 0;
  digitalWrite(activeLight, HIGH);
  //This section runs a periodic delay of 10ms, 100 times to add to a full second of each light lit
  //During each of these cycles, the program determines whether or not a button was being pressed at that time
  while(timer <= 100) {
    //Check if a key was pressed
    if(checkKeyPress() != 0)
    {
      initialKey = checkKeyPress();
      //This delay has been entered since human reflexes are way slower than 10 ms so any button pressed would cause a button to register twice
      delay(300);
      //Test the code usin the button that was pressed and the version of the code to test against
      if(testCodes(initialKey, codeNum))
      {
        correctLightSequence();
        phase += 1;
        return true;
      } else 
      {
        incorrectLightSequence();
        return false;
      }
    }
    delay(10);
    timer += 1;
  }
  //If no button was pressed we simply return false
  digitalWrite(activeLight, LOW); 
  return false; 
}



int checkKeyPress() {
  //Here we want to check the key that is being pressed
  if(digitalRead(greenButton)==LOW) {
    return 1;    
    }
  else if(digitalRead(blueButton)==LOW) {
    return 2;
    }
  else if(digitalRead(yellowButton)==LOW) {
    return 3;    
    } 
  else if(digitalRead(redButton)==LOW) {
    return 4;    
    } else {return 0;}
 
  }



//this method is called to detect which key we want pressed and wait for that key to be unpressed before returning true
//Cannibalized from another project I made using piano keys, hence variable names
static bool waitForKey(int wantedKey) {
    //weve added a delay here to account for some input issues
    delay(50);
    int key = checkKeyPress();
    //This pressedKey variable is in place since the arduino processes too fast, and will always read that no button was pressed upon return
    int pressedKey;
    while(key == 0) {
      //key has not been pressed
      key = checkKeyPress();     
      }
      //Here we say what key has been pressed
      pressedKey = key;
      //Now we want to wait until the user lets go of the key
      while(true){
      while(key == pressedKey){
        key = checkKeyPress();  
        //so again we do nothing
        }
        delay(50);
        key = checkKeyPress();
        if(key == pressedKey){
          //we do nothing here and let it loop again
        } else{
          break;
          }
      }
        //and finally we return to the main loop, but we tell it whether or not the button that was pressed was correct
        Serial.println(pressedKey);
        if(pressedKey == wantedKey){
          return true;
          }
        if(pressedKey != wantedKey){
          return false;
          }   
}

//This method tests the input keys versus the order we want them placed in, return true if sequence completed correctly
bool testCodes(int key, int code)
{
  //Immediately set all lights to on
  toggleLights(true);
  if(code == 1)
  {
    //Check which key triggered this test
    if(key != 1)
    {
      return false;
    }
    if(!waitForKey(2))
    {
      return false;
    }
    if(!waitForKey(3))
    {
      return false;
    }
    if(!waitForKey(4))
    {
      return false;
    }
    return true;
  }
  //*****************
  if(code == 2)
  {
    //Check which key triggered this test
    if(key != 1)
    {
      return false;
    }
    if(!waitForKey(3))
    {
      return false;
    }
    if(!waitForKey(4))
    {
      return false;
    }
    if(!waitForKey(2))
    {
      return false;
    }
    return true;
  }
  //***************
  if(code == 3)
  {
    //Check which key triggered this test
    if(key != 4)
    {
      return false;
    }
    if(!waitForKey(2))
    {
      return false;
    }
    if(!waitForKey(3))
    {
      return false;
    }
    if(!waitForKey(1))
    {
      return false;
    }
    return true;
  }
  //failsafe, should not return from here
  return false;
}

//GBYR
void displayCodeOne()
{
    if(nextLight(1,1))
    {
      return;
    }
      if(nextLight(2,1))
    {
      return;
    }
      if(nextLight(3,1))
    {
      return;
    }
      if(nextLight(4,1))
    {
      return;
    }
    if(nextLight(0,1))
    {
      return;
    }
    if(nextLight(0,1))
    {
      return;
    }
}

//GYRB
void displayCodeTwo()
{
    if(nextLight(1,2))
    {
      return;
    }
    if(nextLight(3,2))
    {
      return;
    }
    if(nextLight(4,2))
    {
      return;
    }
    if(nextLight(2,2))
    {
      return;
    }
    if(nextLight(0,2))
    {
      return;
    }
    if(nextLight(0,2))
    {
      return;
    }
}

//RBYG
bool displayCodeThree()
{
    if(nextLight(4,3))
    {
      return true;
    }
    if(nextLight(2,3))
    {
      return true;
    }
    if(nextLight(3,3))
    {
      return true;
    }
    if(nextLight(1,3))
    {
      return true;
    }
    if(nextLight(0,3))
    {
      return true;
    }
    if(nextLight(0,3))
    {
      return true;
    }
    return false;
}


void loop()
{
  if(phase == 1)
  {
    displayCodeOne();
  }
  if(phase == 2)
  {
    displayCodeTwo();
  }
  if(phase == 3)
  {
    if(displayCodeThree())
    {
      digitalWrite(maglock, HIGH);
      delay(3000);
      digitalWrite(maglock, LOW);
      phase=1;
    }
  }
}
