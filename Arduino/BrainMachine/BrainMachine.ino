/***************************************************
Sketch: Sound & Light Machine for Arduino
Author: Chris Sparnicht - http://low.li
Creation Date: 2011.01.31
Last Modification Date: 2011.02.12
License: Creative Commons 2.5 Attrib. & Share Alike

Modified from Chris Sparnicht's sketch by Mitch Altman
11-Jan-2025

Derivation and Notes:
Chris used a stereo 10K Ohm potentiometer to reduce
the volume of the audio for the ears. 
[Mitch:  I used a fixed 4.7K resistor for each ear, 
 which works well with the earbuds that come with the kit I designed.]

This arduino sketch is based on the original Sound & Light Machine 
by - Mitch Altman - 19-Mar-07 as featured in Make Magazine 10.
https://makezine.com/article/home/fun-games/the-brain-machine/
and then modified for Arduino by Chris Sparnicht.
See notes in code below for how Chris adapted Mitch Altman's version for Arduino.
Mitch then modified Chris' sketch to its present form.

The sleep coding comes partially from here:
http://www.arduino.cc/playground/Learning/ArduinoSleepCode
[Mitch note:  the above link is no longer available, but here is another link with good info:
 https://docs.arduino.cc/learn/electronics/low-power/]
***************************************************/


/***************************************************
SOME INFORMATION ABOUT PROGMEM:
First, you have to use #include <avr/pgmspace.h> for table arrays - PROGMEM
The Arduino compiler creates code that will transfer all constants into RAM when the microcontroller
resets.  This hardward could probably hold all this data in memory, but since the original system
used a table (brainwaveTab) that is was too large to transfer into RAM in the original microcontroller,
we're taking the same approach.  This is accomplished by using the library for PROGMEM. 
(This is used below in the definition for the brainwaveTab).  Since the
C compiler assumes that constants are in RAM, rather than in program memory, when accessing
the brainwaveTab, we need to use the pgm_read_byte() and pgm_read_dword() macros, and we need
to use the brainwveTab as an address, i.e., precede it with "&".  For example, to access
brainwaveTab[3].bwType, which is a byte, this is how to do it:
     pgm_read_byte( &brainwaveTab[3].bwType );
And to access brainwaveTab[3].bwDuration, which is a double-word, this is how to do it:
     pgm_read_dword( &brainwaveTab[3].bwDuration );
 ***************************************************/


/***************************************************
LIBRARIES - Define necessary libraries here.
***************************************************/
#include <avr/pgmspace.h> // for arrays - PROGMEM 
#include <Tone.h>         // Include the arduino tone library -- used for generating sound at any desired integer frequency
#include <avr/sleep.h>    // A library to control the sleep mode -- used to put the microcont
#include <avr/power.h>    // A library to control power

/***************************************************
GLOBALS
We use these globals to define the pin numbers.
This makes it easier to change pin numbers later
without needing to search and replace them in many places.
***************************************************/
#define rightEye 9      // Define pinout for right eye
#define leftEye 10      // Define pinout for left eye
#define leftEarLow 5    // Define pinout for right ear
#define rightEarLow 6   // Define pinout for left ear
//#define wakePin 3     // the input pin where the pushbutton is connected.
// [Mitch:  I got rid of the Push-Button on/off switch, 
//  and replaced it with a slide switch for the new Brain Machine kit]


/***************************************************
BRAINWAVE TABLE
See 'Some information about PROGMEM' above.
Table of values for meditation start with 
lots of Beta (awake / conscious)
add Alpha (dreamy / trancy to connect with 
      subconscious Theta that'll be coming up)
reduce Beta (less conscious)
start adding Theta (more subconscious)
pulse in some Delta (creativity)
and then reverse the above to come up refreshed
***************************************************/
struct brainwaveElement {
  char bwType;  // 'a' for Alpha, 'b' for Beta, 't' for Theta,'d' for Delta or 'g' for gamma ('0' signifies last entry in table)
// A, B, T, D and G offer alternating flash instead of concurrent flash (which aren't used in the following table, but are available for experiementing)
// {0,0} must be the last element of the Brainwave table -- it signifies the end of the Brainwave table
  unsigned long int bwDuration;  // Duration of each Brainwave element (divide by 10,000 to get seconds)
                                 // For example:
                                 //    { 'b', 600000 }   plays a beta wave for 60 seconds
// The following table adds up to 856 seconds, which is a nice 14 minutes, 16 second meditation sequence.  
} const brainwaveTab[] PROGMEM = { 
{ 'b', 600000 },
{ 'a', 100000 },
{ 'b', 200000 },
{ 'a', 150000 },
{ 'b', 150000 },
{ 'a', 200000 },
{ 'b', 100000 },
{ 'a', 300000 },
{ 'b',  50000 },
{ 'a', 600000 },
{ 't', 100000 },
{ 'a', 300000 },
{ 't', 200000 },
{ 'a', 300000 },
{ 't', 300000 },
{ 'a', 150000 },
{ 't', 600000 },
{ 'a', 150000 },
{ 'b',  10000 },
{ 'a', 150000 },
{ 't', 600000 },
{ 'd',  10000 },
{ 't', 100000 },
{ 'd',  10000 },
{ 't', 100000 },
{ 'd',  10000 },
{ 't', 300000 },
{ 'a', 150000 },
{ 'b',  10000 },
{ 'a', 150000 },
{ 't', 300000 },
{ 'a', 150000 },
{ 'b',  10000 },
{ 'a', 200000 },
{ 'b',  50000 },
{ 'a', 200000 },
{ 'b', 150000 },
{ 'a', 150000 },
{ 'b', 200000 },
{ 'a', 100000 },
{ 'b', 250000 },
{ 'a',  50000 },
{ 'b', 600000 },
{ '0', 0 }
};


/***************************************************
VARIABLES for the Tone generator
We use the Tone libary to generate the desired sounds for the Brain Machine.

When there are two different pitches generated in a physical space 
the difference in Hz between two pitches causes a physical "beat frequency" which everyone can hear. 
When the two different pitches are presented separately to each ear, 
the difference in the two pitches is (oddly) perceived in most peoples' brains 
as an imaginary beat frequency, which is called a "binaural beat".
To create the binaural beat frequencies that we desire, 
we choose a central pitch, and generate pitches for each ear, as needed (using the Tone library),
which are half of the desired difference up and down from the central pitch.
If we choose a central pitch of 200 Hz, 
we can generate the following pitches for each ear 
to create the desired binaural beat for our brains to synch up with
BTW, when a brain synchs up to an external frequency, it is called "entrainment".
For the Brain Machine, we entrain our brains using both sound (ears) and light (eyes).
     Brainwave type      R Ear    L Ear    Beat
            Gama:        280.00   220.00   40.0 Hz
            Beta:        192.80   207.20   14.4 Hz
            Alpha:       194.45   205.55   11.1 Hz
            Theta:       197.00   203.00    6.0 Hz
            Delta:       198.90   201.10    2.2 Hz

You can use any central pitch you like. 
For meditation and relaxation, 
I think a lower tone between 100 and 220 is easier on the ears.
Others prefer something around 440 (A above Middle C).
Isolating the central pitch makes it easy for
the user to choose a preferred frequency base.

***************************************************/
float binauralBeat[] = { 40.0, 14.4, 11.1, 6.0, 2.2 }; // For gama, beta, alpha, theta, and delta beat differences.
Tone rightEar;              // For using the Tone library for generating a pitch for the right ear
Tone leftEar;               // For using the Tone library for generating a pitch for the left ear
float centralTone = 200.0;  // We starting at this central frequency and spread the pitches for each ear from there to create the desired beat frequencies.

// Definitions for LED blink timings for the 'blink_LEDs' and 'alt_blink_LEDS' functions
unsigned long int duration = 0;
unsigned long int onTime = 0;
unsigned long int offTime = 0;


/***************************************************
  SETUP initializes our Arduino pins for blinking the LEDs for each eye and sending sound to the speakers for each ear.
  Arduino pins we'll use:
  pin  7  - on/off switch  -- [Mitch:  this switch is no longer used]
  pin  5  - Right ear
  pin  6  - Left ear
  pin  9  - Left eye LED1
  pin 10  - Right eye LED2
  pin 5V  - Power for everything, including the common anode on LED's
  pin GND - Ground for power, LEDs, and speakers
*/
void setup()  { 
  rightEar.begin(rightEarLow);      // Enables the Tone library to play a pitch on the rightEarLow output pin
  leftEar.begin(leftEarLow);        // Enables the Tone library to play a pitch on the rightEarLow output pin
  pinMode(rightEye, OUTPUT);        // Output pin for rightEye LED
  pinMode(leftEye, OUTPUT);         // Output pin for LeftEye LED
  //pinMode(wakePin, INPUT_PULLUP); // Pin input at wakePin  -- [Mitch:  no longer used]

  // The following lines are for using the myTimer() function to empirically determine the value for DelayCount.
  // Display (in the Serial Monitor) the number of milliseconds before playing the Brainwave table (which should be '0'):
  Serial.begin(9600);
  Serial.print("Start");
  myTimer();                        // Print out the time value from the millisec() function
}


/***************************************************
   MAIN LOOP - tells our program what to do.

   It plays each element in the brainwaveTab,
   and then puts the microcontroller into lower-power mode (sleep) to save battery.
***************************************************/

void loop() {
  int j = 0;
  while (pgm_read_byte(&brainwaveTab[j].bwType) != '0') {  // '0' signifies end of table
    do_brainwave_element(j);                               // Play one element of the Brainwave Table
    j++;
  }
 
  // Shut down everything and put the microcontroller to sleep (low-power mode)
  analogWrite(rightEye, 255);  // common anode - HIGH (255) means 'Off'
  analogWrite(leftEye, 255);   // common anode - HIGH (255) means 'Off'
  rightEar.stop();
  leftEar.stop();
  // The following lines are for using the myTimer() function to empirically determine the value for DelayCount.
  // Display (in the Serial Monitor) the number of milliseconds used to reach the end of the Brainwave table:
  Serial.print("End");
  myTimer();                   // Print out the time value from the millisec() function
  delay(500);                  // Give the serial port time to finish displaying the time value before putting the microcontroller to sleep
  sleepNow();                  // Put the microcontroller into sleep mode

  // The microcontroller should never wake up, 
  // since we did not enable any interrupts to do so, 
  // but just in case, we will just loop here till the unit is turned Off with the slide switch.
  Serial.println("Hey!  The microcontroller woke up (even though it shouldn't have!");
  while(1){}
}


/***************************************************
This function delays the specified number of 1/10 milliseconds
***************************************************/

void delay_one_tenth_ms(unsigned long int ms) {
  unsigned long int timer;
  const unsigned long int DelayCount=196;  // This value was determined by trial and error
                                           //   (for more info please see the notes in the myTimer() function at the bottom of this sketch) 
 
  while (ms != 0) {
    // Toggling PD0 is done here to force the compiler to do this loop, rather than optimize it away
    for (timer=0; timer <= DelayCount; timer++) {PIND |= 0b0000001;};
    ms--;
  }
}
 
/***************************************************
This function blinks the LEDs 
(connected to Arduino Pins 9 & 10 - 
for Right eye, Left eye, respectively)
and keeps them blinking for the Duration specified 
(Duration given in 1/10 millisecs).
This function also acts as a delay for the Duration specified.
In this particular instance, digitalWrites are set 
for common anode, so LED "On" = LOW )0) and LED "Off" = HIGH (255).
***************************************************/

// Blink both LEDs (left eye and right eye) On and Off together
void blink_LEDs( unsigned long int duration, unsigned long int onTime, unsigned long int offTime) {
  for (int i=0; i<(duration/(onTime+offTime)); i++) {   
    analogWrite(rightEye, 0);     // common anode - LOW (0) means 'On'
    analogWrite(leftEye, 0);      // common anode - LOW (0) means 'On'
                                  // Both LEDs are now turned On     
    delay_one_tenth_ms(onTime);   //   Keep them On for a duration of onTime     
    
    analogWrite(rightEye, 255);   // common anode - HIGH (255) means 'Off'
    analogWrite(leftEye, 255);    // common anode - HIGH (255) means 'Off'
                                  // Both LEDs are now turned Off
    delay_one_tenth_ms(offTime);  //   Keep them Off for a duration of offTime   
  } 
} 

// NOTE: The Brainwave Table in this sketch does not use alternate blinking LEDs -- this is here for you to experiment with.
// Blink both LEDs (left eye and right eye) in an alternating sense -- one is On when the other is Off
void alt_blink_LEDs( unsigned long int duration, unsigned long int onTime, unsigned long int offTime) {
  for (int i=0; i<(duration/(onTime+offTime)); i++) {   
    analogWrite(rightEye, 0);     // common anode - LOW (0) means 'On'
    analogWrite(leftEye, 255);    // common anode - HIGH (255) means 'Off'
                                  // Both LEDs are appropriately On / Off
    delay_one_tenth_ms(onTime);   //   Keep them in that state for a duration of onTime
    
    analogWrite(rightEye, 255);   // common anode - HIGH (255) means 'Off'
    analogWrite(leftEye, 0);      // common anode - LOW (0) means 'On'
                                  // Both LEDs are appropriately On / Off
    delay_one_tenth_ms(offTime);  //   Keep them in that state for a duration of offTime
  } 
} 


/***************************************************
This function starts with a central audio frequency and
splits the difference between two pitches
to create a binaural beat (between Left and Right ears) 
for a Brainwave Element.
(See notes above for beat creation method.)

The blink_LEDs() function has 3 values:
   duration
   onTime
   offTime
The onTime+offTime are calculated so that the LED blink-rate
is the same as the binaural beat frequency for a given brainwave value.
For example, for "a" or "A" (for Alpha), the binaural beat frequency is 11.1Hz.
To get a blink-rate of as close to 11.1Hz as possible, onTime=451 and offTime=450:
     10,000/(451+450) = 11.0988Hz

For each Brainwave type given below, there are two cases (one with lower-case letter, and one with upper-case letter):
* one with both LEDs blinking the same (lower-case letter, such as 'b')
* one with the LEDs blinking alternately (upper-case letter, such as 'B').
The Brainwave table included near the top of this sketch does not make use of alternating blinking.  They are included here in case you would like to experiment.

The same is true for including 'g' and 'G' (Gamma), which is not used in the Brainwave table used in this sketch.  These are included here for you to experiment with.
***************************************************/

void do_brainwave_element(int index) {     
  char brainChr = pgm_read_byte(&brainwaveTab[index].bwType);     
    
  switch (brainChr) {
    case 'g':
      // Gamma
      // Generate a binaural beat of 40.0Hz
      rightEar.play(centralTone - (binauralBeat[0]/2));
      leftEar.play(centralTone + (binauralBeat[0]/2));  
      // delay for the time specified in the table while blinking the LEDs at 40.0Hz
      blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 125, 125 );
      return;   // Gamma
 
    case 'G':
      // Gamma - with alternating blinks
      // Generate a binaural beat of 40.0Hz
      rightEar.play(centralTone - (binauralBeat[0]/2));
      leftEar.play(centralTone + (binauralBeat[0]/2));  
      // delay for the time specified in the table while blinking the LEDs at 40.0Hz
      alt_blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 125, 125 );
      return;   // Gamma -- with alternating blinks

    case 'b':         
      // Beta       
      // Generate binaural beat of 14.4Hz       
      rightEar.play(centralTone - (binauralBeat[1]/2));
      leftEar.play(centralTone + (binauralBeat[1]/2));  
      // delay for the time specified in the table while blinking the LEDs at 14.4Hz
      blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 347, 347 );
      return;   // Beta
 
    case 'B':         
      // Beta - with alternating blinks
      // Generate binaural beat of 14.4Hz       
      rightEar.play(centralTone - (binauralBeat[1]/2));
      leftEar.play(centralTone + (binauralBeat[1]/2));  
      // delay for the time specified in the table while blinking the LEDs at 14.4Hz
      alt_blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 347, 347 );
      return;   // Beta - with alternating blinks
 
    case 'a':
      // Alpha
      // Generate a binaural beat of 11.1Hz
      rightEar.play(centralTone - (binauralBeat[2]/2));
      leftEar.play(centralTone + (binauralBeat[2]/2));  
      // delay for the time specified in the table while blinking the LEDs at 11.1Hz
      blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 451, 450 );
      return;   // Alpha
 
    case 'A':
      // Alpha - with alternating blinks
      // Generate a binaural beat of 11.1Hz
      rightEar.play(centralTone - (binauralBeat[2]/2));
      leftEar.play(centralTone + (binauralBeat[2]/2));  
      // delay for the time specified in the table while blinking the LEDs at 11.1Hz
      alt_blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 451, 450 );
      return;   // Alpha - with alternating blinks
 
    case 't':
      // Theta
      // Generate a binaural beat of 6.0Hz
      rightEar.play(centralTone - (binauralBeat[3]/2));
      leftEar.play(centralTone + (binauralBeat[3]/2));  
      // delay for the time specified in the table while blinking the LEDs at 6.0Hz
      blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 833, 833 );
      return;   // Theta
 
    case 'T':
      // Theta - with alternating blinks
      // Generate a binaural beat of 6.0Hz
      rightEar.play(centralTone - (binauralBeat[3]/2));
      leftEar.play(centralTone + (binauralBeat[3]/2));  
      // delay for the time specified in the table while blinking the LEDs at 6.0Hz
      alt_blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 833, 833 );
      return;   // Theta - with alternating blinks
 
    case 'd':
      // Delta
      // Generate a binaural beat of 2.2Hz
      rightEar.play(centralTone - (binauralBeat[4]/2));
      leftEar.play(centralTone + (binauralBeat[4]/2));  
      // delay for the time specified in the table while blinking the LEDs at 2.2Hz
      blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 2253, 2253 );
      return;   // Delta
 
    case 'D':
      // Delta - with alternating blinks
      // Generate a binaural beat of 2.2Hz
      rightEar.play(centralTone - (binauralBeat[4]/2));
      leftEar.play(centralTone + (binauralBeat[4]/2));  
      // delay for the time specified in the table while blinking the LEDs at 2.2Hz
      alt_blink_LEDs( pgm_read_dword(&brainwaveTab[index].bwDuration), 2272, 2273 );
      return;   // Delta - with alternating blinks
 
    // this should never be executed, since we catch the end of table in the main loop
    default:
      return;      // end of table
  }
}


/***************************************************
SLEEP function

After playing the entire Brainwave table
we can put the microcontroller into a low-power state (sleep)
to increase the life of the battery.
***************************************************/
void sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_datasheet.pdf 
     * on page 35 there is a list of sleep modes 
     * which explains which clocks and wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we 
     * choose the according sleep mode: SLEEP_MODE_PWR_DOWN
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 

    /* If you want to wake the microcontroller up from sleep, then you need to enable an interrupt to do it. 
     * We do it here so that an accidentally pushed button doesn't cause an interrupt
     * in our running program. 
     * If you want to be able to run interrupt code besides the sleep function, place it in 
     * setup() , for example.
     * 
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
     * B   Name of a function you want to execute at interrupt for A.
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *     In all but the IDLE sleep modes only LOW can be used.
     */

    //attachInterrupt(1,wakeUpNow, LOW); // use interrupt 1 (pin 3) and run function
                                       // wakeUpNow when pin 3 goes LOW 
    // MITCH:  For the Brain Machine kit, I replaced the push-button with a SPST slide switch,
    //            so there is no more a need to wake up the microcontoller.  So, no wakeup interrupt.
    //            Once we put it to sleep, it stays asleep, until we turn the board Off and On again with the slide switch.
    //         So, the above line is commented out.

    sleep_mode();            // here the device is actually put to sleep!!
                             // IF WE ENABLED AN INTERRUPT, THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

/*
// Since we do not enable an interrupt to wake up the microcontroller from sleep mode, 
// the microcontroller should never wake up.  So, the following lines of code are commented out:
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(1);      // disables interrupt 1 on pin 3 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.
*/
}


/***************************************************
WAKE function
***************************************************/
void wakeUpNow()        // here the interrupt is handled after wakeup (if we enabled an interrupt)
{
  // Execute code here after wake-up before returning to the loop() function.
  // Timers, and code using timers (serial.print and more...), will not work here.
  // We don't really need to execute any special functions here, since we just want the thing to wake up 
  // (if we enabled an interrupt for doing so)
}


/***************************************************
Timer function - millisecond count is sent to serial monitor at start of the show 
and again just before putting the microcontroller to sleep at the end. 
We want to make the milliseconds match, as closely as possible, 
to the sum of bwDuration column in the BrainWave Table given near the top of this sketch. 
This is achieved, through trial and error, by by changing the value of the DelayCount variable (defined above). 
DelayCount from Mitch Altman's original sketch was at 87. 
But we're using a faster chip, so I've determined for the arduino DelayCount should be 196.
   Mitch note:
      Using this myTimer() function, I empirically confirmed that 196 is the best value for DelayCount:
         For the Brainwave Table given in this sketch
         the ideal calculated length of the entire brainwave sequence is: 856.000 seconds (14 minutes, 16 seconds).
            When DelayCount=196, the measured (repeatable) duration to play the entire BrainWave table is: 854.338 seconds (error of 1.662 seconds too short)
            When DelayCount=197, the measured (repeatable) duration to play the entire BrainWave table is: 858.654 seconds (error of 2.654 seconds too long)

***************************************************/
void myTimer(){
  Serial.println("  ");
  Serial.print("Time: ");
  long int time = millis();
  //prints time since program started
  Serial.print(time);
  Serial.println("  ");
  // This function is a little quirky, but it works.
}
