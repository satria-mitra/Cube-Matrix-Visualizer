/*
Modified Adafruit Arduino - Lesson 3. RGB LED
to select options a, b, c, d, e with a push button, change colors, and print debug messages to the Serial Monitor.
*/

int redPin = 10;
int greenPin = 11;
int bluePin = 12;
int buttonPin = 13;
int buttonState = 0;
int lastButtonState = 0;
int option = 0; // Variable to track the current option

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP); // Set the button as an input
  Serial.begin(9600); // Initialize Serial communication at 9600 baud rate
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Check if button is pressed
      option++; // Increment option
      if (option > 4) option = 0; // Cycle back to the first option after the last one
      
      // Change color based on the current option and print a message to the Serial Monitor
      switch (option) {
        case 0:
          setColor(255, 0, 0); // red
          Serial.println("Option a selected: Red");
          break;
        case 1:
          setColor(0, 255, 0); // green
          Serial.println("Option b selected: Green");
          break;
        case 2:
          setColor(0, 0, 255); // blue
          Serial.println("Option c selected: Blue");
          break;
        case 3:
          setColor(252, 226, 0); // yellow
          Serial.println("Option d selected: Yellow");
          break;
        case 4:
          setColor(160, 32, 240); // purple
          Serial.println("Option e selected: Purple");
          break;
      }
    }
    delay(150); // Debounce delay
  }
  lastButtonState = buttonState; // Save the last button state
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
