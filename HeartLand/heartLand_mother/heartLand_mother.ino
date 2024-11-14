/**
 * @file        heartLand_mother.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @version     1.0
 * @date        2024-06-26
 * @copyright   GPL-3.0
 * @brief       a sketch for ProMicro in USB-MIDI Controller "HeartLand"
 * @details     Merge MIDI message; "daughter"s one and mine, and TX/RX with USB
 */

//! https://github.com/tttapa/Control-Surface
#include <Control_Surface.h>
//! Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
//! Instantiate a 5-pin DIN MIDI interface
HardwareSerialMIDI_Interface midi_ser {Serial1, MIDI_BAUD};
//! Create a MIDI pipe factory with 3 pipes
BidirectionalMIDI_PipeFactory<3> pipes;

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ variable to change ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

uint8_t ch[3]   = {2, 3, 4};    //!< MIDIチャンネルの管理
uint8_t minPWM  = 0;            //!< LED最暗値
uint8_t maxPWM  = 255;          //!< LED最明値

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ variable to change ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

uint8_t LEDpin[2] = {3, 5};     //!< indicator



/**
 * @struct      CCButton
 * @brief       CCボタンの構造体
 */
CCButton sw[] = {
    {10,  {1, Channel::createChannel(ch[1])}, }, // Solo01
    {14,  {2, Channel::createChannel(ch[1])}, }, // Solo02

    {15,  {1, Channel::createChannel(ch[2])}, }, // Solo03
    {16,  {2, Channel::createChannel(ch[2])}, }, // Solo04
};



/**
 * @struct      CCPotentiometer
 * @brief       CCポテンショメータの構造体
 */
CCPotentiometer pot[] = {
    {A8, {1, Channel::createChannel(ch[0])}, }, // A x B
    {A9, {2, Channel::createChannel(ch[0])}, }, // Master

    {A0, {3, Channel::createChannel(ch[1])}, }, // Vol01
    {A1, {4, Channel::createChannel(ch[1])}, }, // Vol02
    {A6, {5, Channel::createChannel(ch[1])}, }, // FX_A

    {A2, {3, Channel::createChannel(ch[2])}, }, // Vol03
    {A3, {4, Channel::createChannel(ch[2])}, }, // Vol04
    {A7, {5, Channel::createChannel(ch[2])}, }, // FX_B
};



/**
 * @fn      void setup()
 * @brief   セットアップ関数
 */
void setup() {
    Control_Surface | pipes | midi_usb;
    Control_Surface | pipes | midi_ser;
    
    midi_ser | pipes | midi_usb;    // Manually route Serial to USB / USB to Serial
    MIDI_Interface::beginAll();     // Initialize the MIDI interfaces

    Control_Surface.begin();

    Control_Surface.setMIDIInputCallbacks(nullptr, nullptr, nullptr, realTimeMessageCallback);
}

/**
 * @fn      void loop()
 * @brief   ループ関数
 */
void loop() {
    Control_Surface.loop();
}



float     BPM       = 0;  //!< グローバルBPM
uint8_t   ppqn      = 0;  //!< 24 Pulses Per Quarter Note
uint32_t  startTime = 0;  //!< カウント開始時刻
/**
 *  @fn          bool realTimeMessageCallback(RealTimeMessage rt)
 *  @param rt    謎パラメータ
 *  @brief       なんかよく解らんけどsetHandleClockの代わり！使用中はシリアルモニタを絶対に開かないこと
 *  @details     詳細：https://zenn.dev/asumo_1xts/articles/612f2ece9f22c9
 */
bool realTimeMessageCallback(RealTimeMessage rt) {
    float preBPM = 0;   // 一旦の計算結果としてのBPM 

    if(ppqn == 0) {
        startTime = micros();
        analogWrite(LEDpin[0], minPWM);
    }
    ppqn++;             // カウントアップ

    if(ppqn > 24) {     // 24回＝1拍
        preBPM  = 6.0e+07 / float(micros() - startTime); 

        if(20 <= preBPM && preBPM <= 999) {
            BPM = preBPM;
        }               // preBPMが有効な数字であるならば、BPMとして採用
        
        analogWrite(LEDpin[0], maxPWM);
        ppqn = 0;       // カウントリセット
    }

    return true;
}