/* MIDI Tablet Controller 
 *  reads midi data and acts as a mouse to control
 *  a tablet
 *  
 *  6/6/2021
 *  verison 1.1
*/
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

const int Channel = 2;
const int Full_note = 75;
const int Worship_note = 65;
const int Velocity_thresh = 90;


int type, note, velocity, channel, d1, d2;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(57600);
  delay(500);
  home();
  //Serial.println("MIDI Input Test");
  } 
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
  
  if(channel == Channel && note == Full_note && velocity >= Velocity_thresh ){            //note = command for "full"
    //Serial.println("test");
    moveCursor(15, 45, 3);
  }
  
   else if(channel == Channel && note == Worship_note && velocity >= Velocity_thresh){            //note = command for "Worship"
    //Serial.println("test");
    moveCursor(63, 47, 3);
   }
}

void home(){
  int i;
    for (i=0; i<50; i++) {
     Mouse.move(-127, -127);
   }
}

void moveCursor(int x, int y, int v){
  home();
  for (int i=0; i<x; i++){    // move Horizontal
    Mouse.move(v, 0);
  }
  
  for (int i=0; i<y; i++) {   // Move Verticl
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
