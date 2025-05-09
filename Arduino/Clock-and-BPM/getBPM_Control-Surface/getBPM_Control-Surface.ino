/**
 * @file        getBPM-Control_Surface.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @version     1.0
 * @date        2024-06-16
 * @copyright   GPL-3.0
 * @brief       rx MIDI from DAW → detect Clock → convert to BPM
 * @details     This sketch can get BPM from Clock sent by DAW, with "Control_Surface.h".
 * @n           I use Arduino ProMicro and have verified it with AbletonLive 12 and StudioOne 6.
 * @n           more info: https://zenn.dev/asumo_1xts/articles/612f2ece9f22c9
 */

#include <Control_Surface.h>    //!< https://github.com/tttapa/Control-Surface
USBMIDI_Interface midi_usb;



uint16_t  BPM       = 0;  //!< global BPM
uint8_t   ppqn      = 0;  //!< 24 Pulses Per Quarter Note
uint32_t  startTime = 0;  //!< for Timer
/**
 * @fn            bool realTimeMessageCallback(RealTimeMessage rt)
 * @brief         I don't know the details, but it is an alternative to "setHandleClock". 
 * @n             Closing the serial monitor may stop the program.
 * @param rt      a mystery parameter
 * @return true   When you indicate that handling is done, and Control_Surface shouldn't handle it anymore.
 * @return false  When you want Control_Surface to handle it as well.
 */
bool realTimeMessageCallback(RealTimeMessage rt) {
    float preBPM = 0;                   // temporary BPM as a result of a single calculation

    if (ppqn == 0) {                    // the first Clock
        startTime = micros();           // start Timer
    }
    ppqn++;                             // count up Clock

    if (ppqn > 24) {                    // 24 Clocks = 1 bar
        preBPM  = 6.0e+07 / float(micros() - startTime);    // stop Timer, calculate BPM
        if(20 <= preBPM && preBPM <= 999) {                 // adopt if reasonable
            BPM = round(preBPM);
        }
        Serial.println(BPM);            // or just "Serial.println(preBPM)"
        ppqn = 0;                       // reset Clock
    }

    return true;
}



/**
 * @fn      void setup()
 * @brief   setup function
 */
void setup() {
    Control_Surface.begin();
    Control_Surface.setMIDIInputCallbacks(nullptr, nullptr, nullptr, realTimeMessageCallback);
}

/**
 * @fn      void loop()
 * @brief   loop function
 */
void loop() {
    Control_Surface.loop();
}