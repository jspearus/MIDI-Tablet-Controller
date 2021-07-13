#include <Arduino.h>
/* MIDI Tablet Controller 
 *  reads midi data and acts as a mouse to control
 *  a tablet
 *  
 *  6/6/2021
 *  verison 1.1
*/
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// MIDI Channel and Note Setup ##############
const int Channel = 2;
const int Full_note = 75;
const int Worship_note = 65;
const int Velocity_thresh = 90;
//###########################################

//Variables
int type, note, velocity, channel, d1, d2;

//Function Declarations #############################
void home();
void moveCursor(int x, int y, int v);
void leftClick();
void printData();
//#######################################


void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(57600);
  delay(500);
  home();
  delay(2000);
  Serial.println("MIDI Tablet Controller Ready...");
  } 

// Main Program Loop########################################
void loop() {
  if (MIDI.read()){
    byte type = MIDI.getType();
    //read MIDI data 
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        channel = channel-1;
        //printData();
        break;
    }
  }
  //note = command for "full"
  if(channel == Channel && note == Full_note && velocity >= Velocity_thresh ){            
    //Serial.println("Full");
    moveCursor(15, 45, 3);  // move(x,y,v)
  }
  //note = command for "Worship"
  else if(channel == Channel && note == Worship_note && velocity >= Velocity_thresh){            
    //Serial.println("Worship");
    moveCursor(63, 47, 3); // move(x,y,v)
  }
}
 //END Main Loop ###########################################


//Functions-------------------------------------------------

void home(){
  int i;
    for (i=0; i<50; i++) {
     Mouse.move(-127, -127);
   }
}

void moveCursor(int x, int y, int v){
  home();
  for (int i=0; i<x; i++){    // move Horizontal - X AXIS
    Mouse.move(v, 0);
  } 
  for (int i=0; i<y; i++) {   // Move Verticl - Y AXIS
    Mouse.move(0, v);
  }
  delay(25);
  leftClick();
  note = 0;
}

void leftClick(){
  Mouse.press();
  delay(100);
  Mouse.release();
  delay(250);
}

void printData(){
  if (velocity > 0) {
    Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
  } else {
    Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
  }
}
