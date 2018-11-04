/*
AC Light Control
 
 Updated by Robert Twomey 
 
 Changed zero-crossing detection to look for RISING edge rather
 than falling.  (originally it was only chopping the negative half
 of the AC wave form). 
 
 Also changed the dim_check() to turn on the Triac, leaving it on 
 until the zero_cross_detect() turn's it off.
 
 Adapted from sketch by Ryan McLaughlin 
 <a href="http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1230333861/30" rel="nofollow"> <a rel="nofollow"> http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1...</a>>
(now here: <a rel="nofollow"> http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1...</a>
 
 */

#include  <TimerOne.h>          // Avaiable from <a href="http://www.arduino.cc/playground/Code/Timer1" rel="nofollow"> <a href="http://www.arduino.cc/playground/Code/Timer1" rel="nofollow"> http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1...</a>
volatile int i=0;               // Variable to use as a counter
volatile int i2=0;               // Variable to use as a counter
volatile int i3=0;               // Variable to use as a counter
volatile int i4=0;               // Variable to use as a counter
volatile int i5=0;               // Variable to use as a counter


volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin = 3;                // Output to Opto Triac
int AC_pin2 = 4;                // Output to Opto Triac
int AC_pin3 = 7;                // Output to Opto Triac
int AC_pin4 = 8;                // Output to Opto Triac
int AC_pin5 = 12;                // Output to Opto Triac
int dim = 60;                    // Dimming level (0-128)  0 = on, 128 = 0ff
int dim2 = 60;
int dim3 = 60;
int dim4 = 60;
int dim5 = 60;
int inc=1;       // counting up or down, 1=up, -1=down
int id;
int message;
int maxData[] = {0, 0, 0, 0, 0};
int freqStep = 65;    // This is the delay-per-brightness step in microseconds.
                      // For 60 Hz it should be 65
// It is calculated based on the frequency of your voltage supply (50Hz or 60Hz)
// and the number of brightness steps you want. 
// 
// Realize that there are 2 zerocrossing per cycle. This means
// zero crossing happens at 120Hz for a 60Hz supply or 100Hz for a 50Hz supply. 

// To calculate freqStep divide the length of one full half-wave of the power
// cycle (in microseconds) by the number of brightness steps. 
//
// (120 Hz=8333uS) / 128 brightness steps = 65 uS / brightness step
// (100Hz=10000uS) / 128 steps = 75uS/step

int brightnessPin = 5;   
int flickerPin = 4;       
int brightnessVal = 0;   
int flickerVal = 0;       


void setup() {                                      // Begin setup
  Serial.begin(9600);
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  pinMode(AC_pin2, OUTPUT);                          // Set the Triac pin as output
  pinMode(AC_pin3, OUTPUT);                          // Set the Triac pin as output
  pinMode(AC_pin4, OUTPUT);                          // Set the Triac pin as output
  pinMode(AC_pin5, OUTPUT);                          // Set the Triac pin as output
  
  pinMode(13, OUTPUT); // make pin 13 an output
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      
  // Use the TimerOne Library to attach an interrupt
  // to the function we use to check to see if it is 
  // the right time to fire the triac.  This function 
  // will now run every freqStep in microseconds.                                            
}

void zero_cross_detect() {    
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  i2=0;
  i3=0;
  i4=0;
  i5=0;
  digitalWrite(AC_pin, LOW);       // turn off TRIAC (and AC)
  digitalWrite(AC_pin2, LOW);       // turn off TRIAC (and AC)
  digitalWrite(AC_pin3, LOW);       // turn off TRIAC (and AC)
  digitalWrite(AC_pin4, LOW);       // turn off TRIAC (and AC)
  digitalWrite(AC_pin5, LOW);       // turn off TRIAC (and AC)
}                                 


// Turn on the TRIAC at the appropriate time
void dim_check() {                   
  //if(zero_cross == true) {              
    if(i>=dim) {                     
      digitalWrite(AC_pin, HIGH); // turn on light       
      i=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i++; // increment time step counter                     
    } 

    if(i2>=dim2) {                     
      digitalWrite(AC_pin2, HIGH); // turn on light       
      i2=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i2++; // increment time step counter                     
    }  

    if(i3>=dim3) {                     
      digitalWrite(AC_pin3, HIGH); // turn on light       
      i3=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i3++; // increment time step counter                     
    } 

    if(i4>=dim4) {                     
      digitalWrite(AC_pin4, HIGH); // turn on light       
      i4=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i4++; // increment time step counter                     
    }  

    if(i5>=dim5) {                     
      digitalWrite(AC_pin5, HIGH); // turn on light       
      i5=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i5++; // increment time step counter                     
    }        
  //}                                  
}                                   

void loop() {                            
 digitalWrite(13,  LOW);
//  int d1 = map(analogRead(0), 0, 1023, 0, 128);
//  int d2 = map(analogRead(1), 0, 1023, 0, 128);
//
//  dim = d1;
//  dim2 = d2;
//  dim3 = 99;
//  Serial.println(d1);

//  dim = 127;
//  delay(flickerVal);
//  dim = brightnessVal;
//  delay(flickerVal);
  read_max_data();
  write_max_data();

  
  
}

void write_max_data() {  
  digitalWrite(13,  LOW);
  
  dim = maxData[0];
  dim2 = maxData[1];
  dim3 = maxData[2];
  dim4 = maxData[3];
  dim5 = maxData[4];  
}


void read_max_data() {
  while(Serial.available() > 0) { 
    digitalWrite(13,  HIGH);
    message = Serial.read();
    if (message == 250 || message == 251 || message == 252 || message == 253 || message == 254) {
      switch (message) {
        case 250:
          id = 0;
          break;
        case 251:
          id = 1;
          break;      
        case 252:
          id = 2;
          break;
        case 253:
          id = 3;
          break;  
        case 254:
          id = 4;
          break;
        default:
          break;        
      } 
    } else {
      maxData[id] = message;
    } 
    
    Serial.print("1: ");
    Serial.println(maxData[0], DEC);
    Serial.print("2: ");
    Serial.println(maxData[1], DEC);
    Serial.print("3: ");
    Serial.println(maxData[2], DEC);
    Serial.print("4: ");
    Serial.println(maxData[3], DEC);
    Serial.print("5: ");
    Serial.println(maxData[4], DEC);        
  } 
}


