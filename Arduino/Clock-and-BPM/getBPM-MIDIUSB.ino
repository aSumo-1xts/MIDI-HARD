/**
 * @file        getBPM_MIDIUSB.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @brief       rx MIDI from DAW → detect Clock → convert to BPM
 * @version     1.0
 * @date        2024-05-31
 * @copyright   GPL-3.0
 * @details     This sketch can get BPM from Clock sent by DAW, with Library "MIDIUSB.h".
 */

#include <MIDIUSB.h>



/**
 * @brief       get MIDI signals sent serially
 * @param vals  means MIDIvals[3]
 */
void getSerialMIDI(int16_t *vals) {
    int16_t kago[3] = {-1, -1, -1};

    midiEventPacket_t rx;       // Open the mailbox. If empty, header=0.

    do {                        // Retrieve received letters and store them in kago.
        rx = MidiUSB.read();
        if (rx.header != 0) {
            kago[0] = rx.byte1; // read command byte
            kago[1] = rx.byte2; // read next byte
            kago[2] = rx.byte3; // read final byte
        }
    } while (rx.header != 0);   // Repeat until mailbox is empty.

    for (uint8_t i=0; i<3; i++) {
        vals[i] = kago[i];
    }
}



uint16_t    BPM       = 0;  //!< global BPM
uint8_t     ppqn      = 0;  //!< 24 Pulses Per Quarter Note
uint32_t    startTime = 0;  //!< for Timer
/**
 * @brief   クロックを検出して処理する
 */
void clock2BPM() {
    float   preBPM = 0;         // temporary BPM as a result of a single calculation
    int16_t MIDIvals[3];        // means kago

    getSerialMIDI(MIDIvals);    // Bring the letters from the mailbox into the living room.

    if (MIDIvals[0] == 0xF8) {  // System Realtime Message about Clock.
        if (ppqn == 0) {                        // the first Clock
            startTime = micros();               // start Timer
        }
        ppqn++;                                 // count up Clock

        if (ppqn > 24) {                        // 24 Clocks = 1 bar
            preBPM  = 6.0e+07 / float(micros() - startTime);    // stop Timer, calculate BPM
            if(20 <= preBPM && preBPM <= 999) {                 // adopt if reasonable
                BPM = round(preBPM);
            }
            Serial.println(BPM);                // or just "Serial.println(preBPM)"
            ppqn = 0;                           // reset Clock
        }
    }
}



//! @brief setup function
void setup() {
    Serial.begin(115200);
}

//! @grief loop function
void loop() {
    clock2BPM();
}