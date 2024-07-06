/**
 * @file        heartLand_mother.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @brief       a sketch for ProMicro in USB-MIDI Controller "HeartLand"
 * @version     1.0
 * @date        2024-06-26
 * @copyright   GPL-3.0
 * @details     Merge MIDI message; "daughter"s one and mine, and TX/RX with USB
 */

#include <Control_Surface.h>    //!< https://github.com/tttapa/Control-Surface.git

USBMIDI_Interface midi_usb;                     //!< Instantiate a MIDI over USB interface
HardwareSerialMIDI_Interface midi_ser {Serial1};//!< Instantiate a 5-pin DIN MIDI interface
BidirectionalMIDI_Pipe pipes;                   //!< Instantiate the pipe to connect the two interfaces

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ variable to change ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

uint8_t ch[3]   = {2, 3, 4};    //!< MIDIチャンネルの管理
uint8_t minPWM  = 0;            //!< LED最暗値
uint8_t maxPWM  = 255;          //!< LED最明値

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ variable to change ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

uint8_t LEDpin[2] = {3, 5};     //!< indicator





CCButton sw[] = {
    {10,  {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[1])}, }, // Solo01
    {14,  {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[1])}, }, // Solo02
    {15,  {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[2])}, }, // Solo03
    {16,  {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[2])}, }, // Solo04
};





CCPotentiometer pot[] = {
    {A0, {MIDI_CC::Sound_Controller_3,  Channel::createChannel(ch[1])}, }, // Vol01
    {A1, {MIDI_CC::Sound_Controller_4,  Channel::createChannel(ch[1])}, }, // Vol02
    {A2, {MIDI_CC::Sound_Controller_3,  Channel::createChannel(ch[2])}, }, // Vol03
    {A3, {MIDI_CC::Sound_Controller_4,  Channel::createChannel(ch[2])}, }, // Vol04

    {A8, {MIDI_CC::Balance,         Channel::createChannel(ch[0])}, }, // A x B
    {A9, {MIDI_CC::Channel_Volume,  Channel::createChannel(ch[0])}, }, // Master
};





//! @brief setup関数
void setup() {
    Control_Surface.begin();

    midi_ser | pipes | midi_usb;    // Manually route Serial to USB / USB to Serial
    MIDI_Interface::beginAll();     // Initialize the MIDI interfaces

    Control_Surface.setMIDIInputCallbacks(nullptr, nullptr, nullptr, realTimeMessageCallback);
}

//! @brief loop関数
void loop() {
    Control_Surface.loop();
}





float     BPM       = 0;  //!< グローバルBPM
uint8_t   ppqn      = 0;  //!< 24 Pulses Per Quarter Note
uint32_t  startTime = 0;  //!< カウント開始時刻
/**
 * @brief         なんかよく解らんけどsetHandleClockの代わり！使用中はシリアルモニタを絶対に開かないこと
 * @param rt      これも謎パラメータ
 */
bool realTimeMessageCallback(RealTimeMessage rt) {
    float   preBPM = 0;   // 一旦の計算結果としてのBPM 

    if (ppqn == 0) {
        startTime = micros();
        analogWrite(LEDpin[0], minPWM);
    }
    ppqn++;             // カウントアップ

    if (ppqn > 24) {    // 24回＝1拍
        preBPM  = 6.0e+07 / float(micros() - startTime); 

        if(20 <= preBPM && preBPM <= 999) {
            BPM = preBPM;
        }               // preBPMが有効な数字であるならば、BPMとして採用
        
        analogWrite(LEDpin[0], maxPWM);
        ppqn = 0;       // カウントリセット
    }

    return true;
}