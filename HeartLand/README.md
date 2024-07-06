# HeartLand

Ableton LiveでDJをするためのUSB-MIDIコントローラ

## 概要

```text:Diagram
    many INPUTs         some INPUTs
        ↓                   ↓
        ↓                   ↓
Mega 2560 Pro Mini  →   Pro Micro       →   PC
    [daughter]      |    [mother]        |
                    |                    |
        (UART Serial Communication)   (USB-C)
```

## HeartLand_mother

## HeartLand_daughter

CH340などを搭載している場合、どう頑張ってもPC側から
USBデバイスとして認識されないためmotherには不適であることに注意。
