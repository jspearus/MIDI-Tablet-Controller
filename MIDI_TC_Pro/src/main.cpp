


#include <Arduino.h>
#include <MIDI.h>
#include <Wire.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// MIDI Channel and Note Setup ##############
const int Channel = 2;
const int Full_note = 75;
const int Worship_note = 65;
const int All_Off = 70;
const int Click = 61;
const int Velocity_thresh = 90;
//###########################################

//Variables
int type, note, velocity, channel;

String Serial_Com = "";

bool BluMode = true;
//#######################################

// Function Declarations #############################
void printData();
void BluetoothOut(int x, int y, int xStep, int yStep, String click, int hStep);
void BluOut(String command);
void home();
void moveCursor(int x, int y, int v);
void leftClick();
void printData();
void serialEvent();
// #######################################

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Wire.begin(1);
  Wire.setClock(400000);
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
    if (BluMode == false){
      moveCursor(15, 45, 3);  // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(2, 2, 20, 53, "true", 8);  // Bluetooth(x, y, xStep, yStep, click, hStep)
      //BluOut("full");
    }
    Serial.println("Full");
    note = 0;
  }

  //note = command for "Worship"
  else if(channel == Channel && note == Worship_note && velocity >= Velocity_thresh){            
    if (BluMode == false){
      moveCursor(63, 47, 3); // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(2, 2, 68, 47, "false", 8);  // Bluetooth(x, y, xStep, yStep, hStep)
      //BluOut("worship");
    }
    Serial.println("Worship");
    note = 0;
  }

  //note = command for "All Off"
  else if(channel == Channel && note == All_Off && velocity >= Velocity_thresh){            
    if (BluMode == false){
      moveCursor(63, 23, 3); // move(x,y,v)
    }
    else if(BluMode == true){
      BluetoothOut(2, 2, 63, 19, "true", 8);  // Bluetooth(x, y, xStep, yStep, hStep)
      //BluOut("alloff");
    }
    Serial.println("All Off");
    note = 0;
  }

  //note = command for "Left Click"
  else if(channel == Channel && note == Click && velocity >= Velocity_thresh){            
    if (BluMode == false){
      moveCursor(63, 23, 3); // move(x,y,v)
    }
    else if(BluMode == true){
      BluOut("click");
    }
    Serial.println("Left Click");
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

void BluetoothOut(int x, int y, int xStep, int yStep, String click, int hStep){
  Wire.beginTransmission(9);
  Wire.print(x); Wire.print("@");
  Wire.print(y); Wire.print("-");
  Wire.print(xStep); Wire.print("&");
  Wire.print(yStep); Wire.print("^");
  Wire.print(hStep); Wire.print("~");
  Wire.print(click); Wire.print("#");
  Wire.endTransmission();
}
void BluOut(String command){
  Wire.beginTransmission(9);
  Wire.print(command);
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
  delay(10);
  for (int i=0; i<x; i++){    // move Horizontal - X AXIS
    Mouse.move(v, 0);
  } 
  for (int i=0; i<y; i++) {   // Move Verticl - Y AXIS
    Mouse.move(0, v);
  }
  delay(10);
  leftClick();
  note = 0;
}

void leftClick(){
  Mouse.press();
  delay(50);
  Mouse.release();
}


