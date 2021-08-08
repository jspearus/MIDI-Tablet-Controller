


#include <Arduino.h>
#include <MIDI.h>
#include <Wire.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// MIDI Channel and Note Setup ##############
const int Channel = 2;
const int Full_note = 75;
const int Worship_note = 65;
const int All_Off = 70;
const int Velocity_thresh = 90;
//###########################################

//Variables
int type, note, velocity, channel;

bool BluMode = true;
//#######################################

// Function Declarations #############################
void printData();
void BluetoothOut(int x, int y, int xStep, int yStep, int hStep);
void home();
void moveCursor(int x, int y, int v);
void leftClick();
void printData();
// #######################################

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Wire.begin(1);
  Serial.begin(57600);
  delay(3500);
  Serial.println("MIDI-TC pro Ready...");
  if(BluMode == true){
    Serial.println("Mode = Blutooth");
  }
  else if(BluMode == false){
    Serial.println("Mode = USB");
  }
}

// Main Program Loop ########################################
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
          //printData();  //Uncoment to see debug data
        break;
    }
  }
  //note = command for "full"
  if(channel == Channel && note == Full_note && velocity >= Velocity_thresh ){            
    Serial.println("Full");
    if (BluMode == false){
      moveCursor(15, 45, 3);  // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(5, 15, 9, 9, 10);  // Bluetooth(x, y, xStep, yStep, hStep)
    }
    note = 0;
  }
  //note = command for "Worship"
  else if(channel == Channel && note == Worship_note && velocity >= Velocity_thresh){            
    Serial.println("Worship");
    if (BluMode == false){
      moveCursor(63, 47, 3); // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(21, 16, 9, 9, 10);  // Bluetooth(x, y, xStep, yStep, hStep)
    }
    note = 0;
  }
  else if(channel == Channel && note == All_Off && velocity >= Velocity_thresh){            
    Serial.println("All Off");
    if (BluMode == false){
      moveCursor(63, 32, 3); // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(21, 7, 9, 9, 10);  // Bluetooth(x, y, xStep, yStep, hStep)
    }
    note = 0;
  }
}
// END MAIN LOOP #############################################



// FUNCTIONS ########################################################################################


void printData(){
  if (velocity > 0) {
    Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
  } else {
    Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
  }
}

void BluetoothOut(int x, int y, int xStep, int yStep, int hStep){
  Wire.beginTransmission(9);
  Wire.print(x);
  Wire.print("@");
  Wire.print(y);
  Wire.print("-");
  Wire.print(xStep);
  Wire.print("&");
  Wire.print(yStep);
  Wire.print("^");
  Wire.print(hStep);
  Wire.print("#");
  Wire.endTransmission();
}

void home(){
  int i;
    for (i=0; i<15; i++) {
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
  leftClick();
  note = 0;
}

void leftClick(){
  Mouse.press();
  delay(100);
  Mouse.release();
}