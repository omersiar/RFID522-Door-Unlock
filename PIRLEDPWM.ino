/* 
 * Led Strip PWM Fade In and Out with PIR sensor
 * Original PIR Code from Kristian Gohlke
 * and Fading code from By Tom Igoe
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 3;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 7;    //the digital pin connected to the PIR sensor's output
int ledPin = 9;


/////////////////////////////
//SETUP

void setup(){
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  //calibration
      for(int i = 0; i < calibrationTime; i++){
      delay(1000);
      }
    delay(50);
  }
  
////////////////////////////  
//LOOP

void loop(){
     if(digitalRead(pirPin) == HIGH){
        if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;          
         delay(50);
         for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
         analogWrite(ledPin, fadeValue);
         delay(100);
         }
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           delay(50);
           for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=1) { 
            // sets the value (range from 0 to 255):
          analogWrite(ledPin, fadeValue);         
          // wait for 30 milliseconds to see the dimming effect    
          delay(30);
        }
           }
       }
  }

