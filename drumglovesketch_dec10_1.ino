
/***********HOBGOB ECLECTRONICS BeatSneaks code**************/

/*This code sends MIDI messages using electrical pulses from FSRs (Force Sensitive Resistors) placed in the heels and toes of a pair of sneakers.
 It has been adapted from code by todbot, ladyada, spikenzielabs, and mschaff (ardrumo) - special thanks to X (wxs.ca)
 
 You'll need to grab the Serial-MIDI Converter from spikenzielabs.com */
/************************************************************/

#define THRESHOLD 300
#define LOWERTHRESHOLD 1    // We're setting both a lower and an upper threshold for the FSRs, so that they will only trigger a MIDI message when we actually want them too.
#define PALMPIN 0 
#define THUMBPIN 1  
#define INDEXPIN 2 
#define MIDDLEPIN 3
#define RINGPIN 4  
#define PINKYPIN 5            // Define which analog in pins on the arduino we'll be using
int LEDpin = 13;                           // Connect LED to pin 13
int pinNote[6] = {35,39,40,41,42,43}; // This array stores the different MIDI notes that each pin is assigned 
boolean palm = false;
boolean thumb = false;
boolean index = false;
boolean middle = false;
boolean ring = false;
boolean pinky = false;  //These variables will help us to make sure we don't send multiple MIDI messages per single hit        

void setup(void) {
  Serial.begin(115200);                     // We'll send debugging information to the Serial monitor
  pinMode(LEDpin, OUTPUT);
}

void loop(void) {                                                                                                                  // This is the main function that executes all the others.
  palm = sender(analogRead(PALMPIN), PALMPIN, pinNote[PALMPIN], "PALM = ", palm, THRESHOLD, LOWERTHRESHOLD);

  thumb = sender(analogRead(THUMBPIN), THUMBPIN, pinNote[THUMBPIN], "THUMB = ", thumb, THRESHOLD, LOWERTHRESHOLD);

  index = sender(analogRead(INDEXPIN), INDEXPIN, pinNote[INDEXPIN], "INDEX = ", index, THRESHOLD, LOWERTHRESHOLD);

  middle = sender(analogRead(MIDDLEPIN), MIDDLEPIN, pinNote[MIDDLEPIN], "MIDDLE = ", middle, THRESHOLD, LOWERTHRESHOLD);

  ring = sender(analogRead(RINGPIN), RINGPIN, pinNote[RINGPIN], "RING = ", ring, THRESHOLD, LOWERTHRESHOLD);

  pinky = sender(analogRead(PINKYPIN), PINKYPIN, pinNote[PINKYPIN], "PINKY = ", pinky, THRESHOLD, LOWERTHRESHOLD);  // We add some extra punch to the toe readings so that they'll sound about as loud as the heel
}

void midimsg(unsigned char message, unsigned char pitch, unsigned char velocity) {                                // This function sends a MIDI message with a message, pitch, and velocity
  Serial.write(message);
  Serial.write(pitch);
  Serial.write(velocity);
}

boolean sender(int reading, int pin, int note, char msg[], boolean press, int threshold, int lowerthreshold ) {  // This function is what does all the work
  if(reading >= threshold) {                                                                                      // If the user stomps harder than a certain threshold...
    if(!press) {                                                                                                  // and if the sensor is not already in 'pressed' mode...
      reading = map (reading, 0,800,0,127); // using a map function to crop the range to a more meaningful set of data (0-400 instead of 0-1024)
      digitalWrite(LEDpin, HIGH);      
      midimsg(144, note, reading);                                                                              // Send a MIDI message
            delay(30);
      midimsg(0x80, note, reading);
      digitalWrite(LEDpin, LOW);
    }  
    press = true;                                                                                             // The sensor is now in 'pressed' mode,
  } 
  else if(reading <= lowerthreshold) {                                                                         // once the applied force sinks below the lower threshold...
    press = false;                                                                                    // the sensor is no longer in 'pressed' mode
  }
  return press;
}


