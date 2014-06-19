//Serial RGB led controller
//Author: Trevor Shannon
//see http://trevorshp.com/creations/android_led.htm

//pin definitions.  must be PWM-capable pins!
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

//maximum duty cycle to be used on each led for color balancing.  
//if "white" (R=255, G=255, B=255) doesn't look white, reduce the red, green, or blue max value.
const int max_red = 255;
const int max_green = 90;
const int max_blue = 100;

byte colors[3] = {0, 0, 0}; //array to store led brightness values
byte lineEnding = 0x0A; //10 in decimal, ASCII newline character

void setup(){
  //set all three of our led pins to output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  //start the Serial connection
  Serial.begin(115200);
}

void loop(){
  //check that at least 3 bytes are available on the Serial port
  if (Serial.available() > 2){
     //store data up until lineEnding (0x0A) in the bytesRead array
     int bytesRead = Serial.readBytesUntil(lineEnding, (char*)colors, 3);
  }  
  //set the three PWM pins according to the data read from the Serial port
  //we also scale the values with map() so that the R, G, and B brightnesses are balanced.
  analogWrite(redPin, map(colors[0], 0, 255, 0, max_red));
  analogWrite(greenPin, map(colors[1], 0, 255, 0, max_green));
  analogWrite(bluePin, map(colors[2], 0, 255, 0, max_blue));
}
