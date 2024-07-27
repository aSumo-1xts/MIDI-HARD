/**
 * @file        heartLand_daughter.ino
 * @author      aSumo (1xtelescope@gmail.com)
 * @brief       a sketch for Mega2560 ProMini in USB-MIDI Controller "HeartLand"
 * @version     1.0
 * @date        2024-06-26
 * @copyright   GPL-3.0
 * @details     Just send serial MIDI message from Serial1 to "mother"
 */

#include <Control_Surface.h> //!< https://github.com/tttapa/Control-Surface.git

//! 単純なシリアル通信のためのインスタンス
SerialMIDI_Interface<decltype(Serial1)> midi_ser {Serial1, MIDI_BAUD};

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ variable to change ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

uint8_t ch[3]       = {2, 3, 4};    //!< MIDIチャンネルの管理
uint8_t BPM_multi   = 5;            //!< BPM用エンコーダの係数
uint8_t enc_multi   = 5;            //!< 汎用エンコーダの係数

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ variable to change ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

uint8_t util01 = 2;     //!< 余りその1
uint8_t util02 = 3;     //!< 余りその2



CCAbsoluteEncoder enc[] = {
    {{26, 28}, {MIDI_CC::Sound_Controller_1, Channel::createChannel(ch[0])}, BPM_multi, }, // enc01
    {{29, 31}, {MIDI_CC::Sound_Controller_2, Channel::createChannel(ch[0])}, enc_multi, }, // enc02
    {{32, 34}, {MIDI_CC::Sound_Controller_3, Channel::createChannel(ch[0])}, enc_multi, }, // enc03
    {{35, 37}, {MIDI_CC::Sound_Controller_4, Channel::createChannel(ch[0])}, enc_multi, }, // enc04
};



CCButton sw[] = {
    {27, {MIDI_CC::Sound_Controller_5, Channel::createChannel(ch[0])}, }, // enc_sw01
    {30, {MIDI_CC::Sound_Controller_6, Channel::createChannel(ch[0])}, }, // enc_sw02
    {33, {MIDI_CC::Sound_Controller_7, Channel::createChannel(ch[0])}, }, // enc_sw03
    {36, {MIDI_CC::Sound_Controller_8, Channel::createChannel(ch[0])}, }, // enc_sw04

    {4,  {MIDI_CC::General_Purpose_Controller_1, Channel::createChannel(ch[0])}, }, // ary01
    {5,  {MIDI_CC::General_Purpose_Controller_2, Channel::createChannel(ch[0])}, }, // ary02
    {6,  {MIDI_CC::General_Purpose_Controller_3, Channel::createChannel(ch[0])}, }, // ary03
    {7,  {MIDI_CC::General_Purpose_Controller_4, Channel::createChannel(ch[0])}, }, // ary04
    {8,  {MIDI_CC::General_Purpose_Controller_5, Channel::createChannel(ch[0])}, }, // ary05
    {9,  {MIDI_CC::General_Purpose_Controller_6, Channel::createChannel(ch[0])}, }, // ary06
    {10, {MIDI_CC::General_Purpose_Controller_7, Channel::createChannel(ch[0])}, }, // ary07
    {11, {MIDI_CC::General_Purpose_Controller_8, Channel::createChannel(ch[0])}, }, // ary08

    {22, {MIDI_CC::Sound_Controller_5, Channel::createChannel(ch[1])}, }, // Sw01
    {23, {MIDI_CC::Sound_Controller_6, Channel::createChannel(ch[1])}, }, // Sw02
    {24, {MIDI_CC::Sound_Controller_5, Channel::createChannel(ch[2])}, }, // Sw03
    {25, {MIDI_CC::Sound_Controller_6, Channel::createChannel(ch[2])}, }, // Sw04
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



//! @brief setup関数
void setup() {
    pinMode(util01, INPUT_PULLUP);
    pinMode(util02, INPUT_PULLUP);

    Control_Surface.begin();
}

//! @brief loop関数
void loop() {
    Control_Surface.loop();
}