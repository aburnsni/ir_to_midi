#include <IRremote.h>
#include <MIDIUSB.h>

const bool MYDEBUG = 1;

//  Setup IR Receiver
const int RECV_PIN = 9;
const int BLINK_PIN = 3;

IRrecv irrecv(RECV_PIN, BLINK_PIN);
decode_results results;

// Setup MIDI output
const uint8_t inputs = 12;
byte notes[inputs] = {60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79};
unsigned long lasttrig[inputs];
uint8_t channel = 0;
byte vol = 127;
uint16_t debounce = 100;

// MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

unsigned long key_value = 0;

void keyPress(int key);
void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);

void setup(){

  // MIDI.begin();
  if (MYDEBUG) {
    Serial.begin(115200);
  }

  irrecv.enableIRIn();
  irrecv.blink13(true);

  for (uint8_t i = 0; i < inputs; i++) {
    lasttrig[i] = millis();
  }
}

void loop(){
  if (irrecv.decode(&results)){
 
        // if (results.value == 0XFFFFFFFF)
        //   results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
          keyPress(0);
          // Serial.println("1");
          break;
          case 0xFF629D:
          keyPress(1);
          // Serial.println("2");
          break;
          case 0xFFE21D:
          keyPress(2);
          // Serial.println("3");
          break;
          case 0xFF22DD:
          keyPress(3);
          // Serial.println("4");
          break;
          case 0xFF02FD:
          keyPress(4);
          // Serial.println("5");
          break ;  
          case 0xFFC23D:
          keyPress(5);
          // Serial.println("6");
          break ;               
          case 0xFFE01F:
          keyPress(6);
          // Serial.println("7");
          break ;  
          case 0xFFA857:
          keyPress(7);
          // Serial.println("8");
          break ;  
          case 0xFF906F:
          keyPress(8);
          // Serial.println("9");
          break ;  
          case 0xFF6897:
          keyPress(9);
          // Serial.println("*");
          break ;  
          case 0xFF9867:
          keyPress(10);
          // Serial.println("0");
          break ;
          case 0xFFB04F:
          keyPress(11);
          // Serial.println("#");
          break ;
          case 0xFF30CF:
          Serial.println("Not Used");
          break ;
          case 0xFF18E7:
          Serial.println("Up");
          break ;
          case 0xFF7A85:
          Serial.println("Not Used");
          break ;
          case 0xFF10EF:
          Serial.println("Left");
          break ;
          case 0xFF38C7:
          Serial.println("OK");
          break ;
          case 0xFF5AA5:
          Serial.println("Right");
          break ;
          case 0xFF42BD:
          Serial.println("Not Used");
          break ;
          case 0xFF4AB5:
          Serial.println("Down");
          break ;
          case 0xFF52AD:
          Serial.println("Not Used");
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }
}

void keyPress(int key) {
  if ((millis() - lasttrig[key]) > debounce) {
    noteOn(channel, notes[key], vol);
    delay(20);
    noteOff(channel, notes[key], vol);
    lasttrig[key] = millis();
    MidiUSB.flush();
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
