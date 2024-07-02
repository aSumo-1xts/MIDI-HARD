/**
 * @file        heartLand_mother.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @brief       a sketch for Mega2560 ProMini in USB-MIDI Controller "HeartLand"
 * @version     1.0
 * @date        2024-06-26
 * @copyright   GPL-3.0
 * @details     combines MIDI messages from ProMicro and self-generated one, and exchanges them with the PC
 */





#include <Control_Surface.h> //!< https://github.com/tttapa/Control-Surface.git

USBMIDI_Interface midi_usb;                     //!< Instantiate a MIDI over USB interface
HardwareSerialMIDI_Interface midi_ser {Serial}; //!< Instantiate a 5-pin DIN MIDI interface (RX0, TX0)
BidirectionalMIDI_Pipe pipes;                   //!< Instantiate the pipe to connect the two interfaces

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ variable to change ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

uint8_t ch[3]           = {2, 3, 4};                //!< MIDIチャンネルの管理（master, A, B）
uint8_t BPM_multi       = 5;                        //!< BPM用エンコーダの係数
uint8_t enc_multi       = 5;                        //!< 汎用エンコーダの係数
uint8_t nud_multi       = 1;                        //!< ナッジCCボタンの係数
uint8_t defaultEnc[6]   = {64, 64, 64, 64, 64, 64}; //!< 各エンコーダの既定値
uint8_t minPWM          = 0;                        //!< LEDのOFF時の明るさ
uint8_t maxPWM          = 255;                      //!< LEDのON時の明るさ

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ variable to change ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */





uint8_t util01 = 2;     //!< 余りその1
uint8_t util02 = 3;     //!< 余りその2
uint8_t LEDpin = 12;    //!< indicator





CCAbsoluteEncoder enc_rotary[] = {
    {{26, 28}, {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[0])}, BPM_coeff, }, // enc01 (for BPM)
    {{29, 31}, {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[0])}, enc_coeff, }, // enc02
    {{32, 34}, {MIDI_CC::Sound_Controller_3, Channel::createChannel(ch[0])}, enc_coeff, }, // enc03
    {{35, 37}, {MIDI_CC::Sound_Controller_4, Channel::createChannel(ch[0])}, enc_coeff, }, // enc04
};
CCIncrementDecrementButtons enc_nudge[] = {
    {{30, 27}, {MIDI_CC::Sound_Controller_5, Channel::createChannel(ch[0])}, nud_multi, },
    {{36, 33}, {MIDI_CC::Sound_Controller_6, Channel::createChannel(ch[0])}, nud_multi, },
};

//! @brief エンコーダによるCCの値をdefaultEncにリセットする。
void initEnc() {
    for (uint8_t i=0; i<6; i++) {
        midi_usb.sendControlChange({i+70, Channel::createChannel(ch[0])}, defaultEnc[i]);
    }
}





CCButton sw[] = {
    {22, {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[1])}, }, // sw01
    {23, {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[1])}, }, // sw02
    {24, {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[2])}, }, // sw03
    {25, {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[2])}, }, // sw04
    {4,  {MIDI_CC::General_Purpose_Controller_1, Channel::createChannel(ch[0])}, }, // ary01
    {5,  {MIDI_CC::General_Purpose_Controller_2, Channel::createChannel(ch[0])}, }, // ary02
    {6,  {MIDI_CC::General_Purpose_Controller_3, Channel::createChannel(ch[0])}, }, // ary03
    {7,  {MIDI_CC::General_Purpose_Controller_4, Channel::createChannel(ch[0])}, }, // ary04
    {8,  {MIDI_CC::General_Purpose_Controller_5, Channel::createChannel(ch[0])}, }, // ary05
    {9,  {MIDI_CC::General_Purpose_Controller_6, Channel::createChannel(ch[0])}, }, // ary06
    {10, {MIDI_CC::General_Purpose_Controller_7, Channel::createChannel(ch[0])}, }, // ary07
    {11, {MIDI_CC::General_Purpose_Controller_8, Channel::createChannel(ch[0])}, }, // ary08
};





CCPotentiometer pot[] = {
    {A0,  {MIDI_CC::Sound_Controller_3,  Channel::createChannel(ch[1])}, }, // pot01_01
    {A1,  {MIDI_CC::Sound_Controller_4,  Channel::createChannel(ch[1])}, }, // pot01_02
    {A2,  {MIDI_CC::Sound_Controller_5,  Channel::createChannel(ch[1])}, }, // pot01_03
    {A3,  {MIDI_CC::Sound_Controller_6,  Channel::createChannel(ch[1])}, }, // pot01_04
    {A4,  {MIDI_CC::Sound_Controller_7,  Channel::createChannel(ch[1])}, }, // pot02_01
    {A5,  {MIDI_CC::Sound_Controller_8,  Channel::createChannel(ch[1])}, }, // pot02_02
    {A6,  {MIDI_CC::Sound_Controller_9,  Channel::createChannel(ch[1])}, }, // pot02_03
    {A7,  {MIDI_CC::Sound_Controller_10, Channel::createChannel(ch[1])}, }, // pot02_04
    {A8,  {MIDI_CC::Sound_Controller_3,  Channel::createChannel(ch[2])}, }, // pot03_01
    {A9,  {MIDI_CC::Sound_Controller_4,  Channel::createChannel(ch[2])}, }, // pot03_02
    {A10, {MIDI_CC::Sound_Controller_5,  Channel::createChannel(ch[2])}, }, // pot03_03
    {A11, {MIDI_CC::Sound_Controller_6,  Channel::createChannel(ch[2])}, }, // pot03_04
    {A12, {MIDI_CC::Sound_Controller_7,  Channel::createChannel(ch[2])}, }, // pot04_01
    {A13, {MIDI_CC::Sound_Controller_8,  Channel::createChannel(ch[2])}, }, // pot04_02
    {A14, {MIDI_CC::Sound_Controller_9,  Channel::createChannel(ch[2])}, }, // pot04_03
    {A15, {MIDI_CC::Sound_Controller_10, Channel::createChannel(ch[2])}, }, // pot04_04
};

//! @brief 可変抵抗によるCCの値をすべて読み取って送る。
void initPot() {
    float primPotCC;
    for (uint8_t i=0; i<16; i++) {
        primPotCC = analogRead(i+54) * 128 / 1023;
        midi_usb.sendControlChange({(i%8)+72, Channel::createChannel(ch[(i/8)+1])}, round(primPotCC));
    }
}





//! @brief setup関数
void setup() {
    pinMode(util01, INPUT_PULLUP);
    pinMode(util02, INPUT_PULLUP);

    RelativeCCSender::setMode(SIGN_MAGNITUDE);

    Control_Surface.begin();
    Control_Surface.setMIDIInputCallbacks(nullptr, nullptr, nullptr, realTimeMessageCallback);

    midi_ser | pipes | midi_usb;    // Manually route Serial to USB / USB to Serial
    MIDI_Interface::beginAll();     // Initialize the MIDI interfaces

    initEnc();
    initPot();
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
        analogWrite(LEDpin, minPWM);
    }
    ppqn++;             // カウントアップ

    if (ppqn > 24) {    // 24回＝1拍
        preBPM  = 6.0e+07 / float(micros() - startTime); 

        if(20 <= preBPM && preBPM <= 999) {
            BPM = preBPM;
        }               // preBPMが有効な数字であるならば、BPMとして採用
        
        analogWrite(LEDpin, maxPWM);
        ppqn = 0;       // カウントリセット
    }

    return true;
}