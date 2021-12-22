//=============================================================================================
// is31fl3731.cpp - Implents a two-character display driver for the IS31FL3731 Led Matrix
//=============================================================================================
#include <string.h>
#include "is31fl3731.h"
#include <Wire.h>
#include <arduino.h>

#define FRAME_SELECT_REG  0xFD
#define CONFIG_FRAME      0x0B
#define PWM_BASE_REG      0x24
#define LED_CTRL_BASE_REG 0x00
#define SHUTDOWN_REG      0x0A

const unsigned char font[] PROGMEM = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // ASCII 0
    0x0E, 0x1F, 0x15, 0x1F, 0x1B, 0x11, 0x0E,      // ASCII 1
    0x0E, 0x1F, 0x15, 0x1F, 0x11, 0x1B, 0x0E,      // ASCII 2
    0x00, 0x0A, 0x1F, 0x1F, 0x1F, 0x0E, 0x04,      // ASCII 3
    0x00, 0x04, 0x0E, 0x1F, 0x1F, 0x0E, 0x04,      // ASCII 4
    0x0E, 0x0A, 0x1F, 0x15, 0x1F, 0x04, 0x0E,      // ASCII 5
    0x04, 0x0E, 0x1F, 0x1F, 0x1F, 0x04, 0x0E,      // ASCII 6
    0x00, 0x00, 0x04, 0x0E, 0x0E, 0x04, 0x00,      // ASCII 7
    0x1F, 0x1F, 0x1B, 0x11, 0x11, 0x1B, 0x1F,      // ASCII 8
    0x00, 0x00, 0x04, 0x0A, 0x0A, 0x04, 0x00,      // ASCII 9
    0x1F, 0x1F, 0x1B, 0x15, 0x15, 0x1B, 0x1F,      // ASCII 10
    0x00, 0x07, 0x03, 0x0D, 0x14, 0x14, 0x08,      // ASCII 11
    0x0E, 0x11, 0x11, 0x0E, 0x04, 0x1F, 0x04,      // ASCII 12
    0x0F, 0x09, 0x0F, 0x08, 0x08, 0x08, 0x18,      // ASCII 13
    0x0F, 0x09, 0x0F, 0x09, 0x09, 0x0B, 0x18,      // ASCII 14
    0x04, 0x15, 0x0E, 0x1B, 0x1B, 0x0E, 0x15,      // ASCII 15
    0x10, 0x18, 0x1E, 0x1F, 0x1E, 0x18, 0x10,      // ASCII 16
    0x01, 0x03, 0x0F, 0x1F, 0x0F, 0x03, 0x01,      // ASCII 17
    0x04, 0x0E, 0x15, 0x04, 0x15, 0x0E, 0x04,      // ASCII 18
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00, 0x1B,      // ASCII 19
    0x0F, 0x15, 0x15, 0x0D, 0x05, 0x05, 0x05,      // ASCII 20
    0x06, 0x09, 0x0A, 0x05, 0x02, 0x09, 0x09,      // ASCII 21
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F,      // ASCII 22
    0x04, 0x0E, 0x15, 0x04, 0x15, 0x0E, 0x04,      // ASCII 23
    0x00, 0x04, 0x0E, 0x15, 0x04, 0x04, 0x04,      // ASCII 24
    0x00, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04,      // ASCII 25
    0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00,      // ASCII 26
    0x00, 0x04, 0x08, 0x1F, 0x08, 0x04, 0x00,      // ASCII 27
    0x00, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00,      // ASCII 28
    0x00, 0x0A, 0x1F, 0x1F, 0x0A, 0x00, 0x00,      // ASCII 29
    0x00, 0x04, 0x04, 0x0E, 0x1F, 0x1F, 0x00,      // ASCII 30
    0x00, 0x1F, 0x1F, 0x0E, 0x04, 0x04, 0x00,      // ASCII 31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // ASCII 32
    0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04,      // ASCII 33
    0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00,      // ASCII 34
    0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A,      // ASCII 35
    0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04,      // ASCII 36
    0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03,      // ASCII 37
    0x08, 0x14, 0x14, 0x08, 0x15, 0x12, 0x0D,      // ASCII 38
    0x06, 0x06, 0x04, 0x08, 0x00, 0x00, 0x00,      // ASCII 39
    0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02,      // ASCII 40
    0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08,      // ASCII 41
    0x04, 0x15, 0x0E, 0x1F, 0x0E, 0x15, 0x04,      // ASCII 42
    0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00,      // ASCII 43
    0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04,      // ASCII 44
    0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,      // ASCII 45
    0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06,      // ASCII 46
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00,      // ASCII 47
    0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E,      // ASCII 48
    0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E,      // ASCII 49
    0x0E, 0x11, 0x01, 0x0E, 0x10, 0x10, 0x1F,      // ASCII 50
    0x1F, 0x01, 0x02, 0x06, 0x01, 0x11, 0x0E,      // ASCII 51
    0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02,      // ASCII 52
    0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E,      // ASCII 53
    0x07, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E,      // ASCII 54
    0x1F, 0x01, 0x01, 0x02, 0x04, 0x08, 0x10,      // ASCII 55
    0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E,      // ASCII 56
    0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x1C,      // ASCII 57
    0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00,      // ASCII 58
    0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x08,      // ASCII 59
    0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01,      // ASCII 60
    0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00,      // ASCII 61
    0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08,      // ASCII 62
    0x0E, 0x11, 0x01, 0x06, 0x04, 0x00, 0x04,      // ASCII 63
    0x0E, 0x11, 0x15, 0x17, 0x16, 0x10, 0x0F,      // ASCII 64
    0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11,      // ASCII 65
    0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E,      // ASCII 66
    0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E,      // ASCII 67
    0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E,      // ASCII 68
    0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F,      // ASCII 69
    0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10,      // ASCII 70
    0x0F, 0x11, 0x10, 0x10, 0x13, 0x11, 0x0F,      // ASCII 71
    0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,      // ASCII 72
    0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,      // ASCII 73
    0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C,      // ASCII 74
    0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11,      // ASCII 75
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F,      // ASCII 76
    0x11, 0x1B, 0x15, 0x15, 0x15, 0x11, 0x11,      // ASCII 77
    0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11,      // ASCII 78
    0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,      // ASCII 79
    0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10,      // ASCII 80
    0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D,      // ASCII 81
    0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11,      // ASCII 82
    0x0E, 0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E,      // ASCII 83
    0x1F, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04,      // ASCII 84
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,      // ASCII 85
    0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04,      // ASCII 86
    0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A,      // ASCII 87
    0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11,      // ASCII 88
    0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04,      // ASCII 89
    0x1F, 0x01, 0x02, 0x0E, 0x08, 0x10, 0x1F,      // ASCII 90
    0x0F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F,      // ASCII 91
    0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00,      // ASCII 92
    0x0F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F,      // ASCII 93
    0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00,      // ASCII 94
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,      // ASCII 95
    0x0C, 0x0C, 0x04, 0x02, 0x00, 0x00, 0x00,      // ASCII 96
    0x00, 0x00, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 97
    0x10, 0x10, 0x16, 0x19, 0x11, 0x19, 0x16,      // ASCII 98
    0x00, 0x00, 0x0E, 0x11, 0x10, 0x11, 0x0E,      // ASCII 99
    0x01, 0x01, 0x0D, 0x13, 0x11, 0x13, 0x0D,      // ASCII 100
    0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E,      // ASCII 101
    0x02, 0x05, 0x04, 0x0E, 0x04, 0x04, 0x04,      // ASCII 102
    0x00, 0x00, 0x0E, 0x13, 0x13, 0x0D, 0x01,      // ASCII 103
    0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11,      // ASCII 104
    0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E,      // ASCII 105
    0x02, 0x00, 0x02, 0x02, 0x02, 0x12, 0x0C,      // ASCII 106
    0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12,      // ASCII 107
    0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,      // ASCII 108
    0x00, 0x00, 0x1A, 0x15, 0x15, 0x15, 0x15,      // ASCII 109
    0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11,      // ASCII 110
    0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E,      // ASCII 111
    0x00, 0x00, 0x16, 0x19, 0x19, 0x16, 0x10,      // ASCII 112
    0x00, 0x00, 0x0D, 0x13, 0x13, 0x0D, 0x01,      // ASCII 113
    0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10,      // ASCII 114
    0x00, 0x00, 0x0F, 0x10, 0x0E, 0x01, 0x1E,      // ASCII 115
    0x04, 0x04, 0x1F, 0x04, 0x04, 0x05, 0x02,      // ASCII 116
    0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D,      // ASCII 117
    0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04,      // ASCII 118
    0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A,      // ASCII 119
    0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11,      // ASCII 120
    0x00, 0x00, 0x11, 0x11, 0x0F, 0x01, 0x11,      // ASCII 121
    0x00, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F,      // ASCII 122
    0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02,      // ASCII 123
    0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04,      // ASCII 124
    0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08,      // ASCII 125
    0x08, 0x15, 0x02, 0x00, 0x00, 0x00, 0x00,      // ASCII 126
    0x04, 0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x00,      // ASCII 127
    0x0E, 0x11, 0x10, 0x10, 0x11, 0x0E, 0x02,      // ASCII 128
    0x00, 0x11, 0x00, 0x11, 0x11, 0x13, 0x0D,      // ASCII 129
    0x03, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0F,      // ASCII 130
    0x1F, 0x00, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 131
    0x00, 0x11, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 132
    0x18, 0x00, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 133
    0x06, 0x00, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 134
    0x00, 0x0F, 0x18, 0x18, 0x0F, 0x02, 0x06,      // ASCII 135
    0x1F, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0F,      // ASCII 136
    0x11, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0F,      // ASCII 137
    0x18, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0F,      // ASCII 138
    0x05, 0x00, 0x06, 0x02, 0x02, 0x02, 0x07,      // ASCII 139
    0x06, 0x09, 0x06, 0x02, 0x02, 0x02, 0x07,      // ASCII 140
    0x0C, 0x00, 0x06, 0x02, 0x02, 0x02, 0x07,      // ASCII 141
    0x15, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11,      // ASCII 142
    0x04, 0x00, 0x04, 0x0A, 0x11, 0x1F, 0x11,      // ASCII 143
    0x06, 0x00, 0x1E, 0x10, 0x1C, 0x10, 0x1E,      // ASCII 144
    0x00, 0x00, 0x0F, 0x02, 0x0F, 0x12, 0x0F,      // ASCII 145
    0x07, 0x0A, 0x12, 0x1F, 0x12, 0x12, 0x13,      // ASCII 146
    0x0E, 0x11, 0x00, 0x0E, 0x11, 0x11, 0x0E,      // ASCII 147
    0x00, 0x11, 0x0E, 0x11, 0x11, 0x11, 0x0E,      // ASCII 148
    0x00, 0x18, 0x00, 0x0E, 0x11, 0x11, 0x0E,      // ASCII 149
    0x0E, 0x11, 0x00, 0x11, 0x11, 0x13, 0x0D,      // ASCII 150
    0x00, 0x18, 0x00, 0x11, 0x11, 0x13, 0x0D,      // ASCII 151
    0x09, 0x00, 0x09, 0x09, 0x09, 0x07, 0x01,      // ASCII 152
    0x11, 0x0E, 0x11, 0x11, 0x11, 0x11, 0x0E,      // ASCII 153
    0x11, 0x00, 0x11, 0x11, 0x11, 0x11, 0x0E,      // ASCII 154
    0x04, 0x04, 0x1F, 0x14, 0x14, 0x1F, 0x04,      // ASCII 155
    0x06, 0x0B, 0x09, 0x1C, 0x08, 0x09, 0x1F,      // ASCII 156
    0x1B, 0x1B, 0x0E, 0x1F, 0x04, 0x1F, 0x04,      // ASCII 157
    0x1C, 0x12, 0x12, 0x1C, 0x12, 0x17, 0x12,      // ASCII 158
    0x03, 0x05, 0x04, 0x0E, 0x04, 0x04, 0x14,      // ASCII 159
    0x03, 0x00, 0x0C, 0x02, 0x0E, 0x12, 0x0F,      // ASCII 160
    0x03, 0x00, 0x06, 0x02, 0x02, 0x02, 0x07,      // ASCII 161
    0x00, 0x03, 0x00, 0x0E, 0x11, 0x11, 0x0E,      // ASCII 162
    0x00, 0x03, 0x00, 0x11, 0x11, 0x13, 0x0D,      // ASCII 163
    0x00, 0x0F, 0x00, 0x0E, 0x09, 0x09, 0x09,      // ASCII 164
    0x1F, 0x00, 0x19, 0x1D, 0x17, 0x13, 0x11,      // ASCII 165
    0x0E, 0x12, 0x12, 0x0F, 0x00, 0x1F, 0x00,      // ASCII 166
    0x0E, 0x11, 0x11, 0x0E, 0x00, 0x1F, 0x00,      // ASCII 167
    0x04, 0x00, 0x04, 0x0C, 0x10, 0x11, 0x0E,      // ASCII 168
    0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x00,      // ASCII 169
    0x00, 0x00, 0x00, 0x1F, 0x01, 0x01, 0x00,      // ASCII 170
    0x10, 0x11, 0x12, 0x17, 0x09, 0x13, 0x04,      // ASCII 171
    0x10, 0x11, 0x12, 0x15, 0x0B, 0x17, 0x01,      // ASCII 172
    0x04, 0x04, 0x00, 0x04, 0x04, 0x04, 0x04,      // ASCII 173
    0x00, 0x05, 0x0A, 0x14, 0x0A, 0x05, 0x00,      // ASCII 174
    0x00, 0x14, 0x0A, 0x05, 0x0A, 0x14, 0x00,      // ASCII 175
    0x15, 0x00, 0x15, 0x00, 0x15, 0x00, 0x15,      // ASCII 176
    0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A,      // ASCII 177
    0x1F, 0x15, 0x1F, 0x15, 0x1F, 0x15, 0x1F,      // ASCII 178
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,      // ASCII 179
    0x02, 0x02, 0x02, 0x02, 0x1E, 0x02, 0x02,      // ASCII 180
    0x02, 0x02, 0x1E, 0x02, 0x1E, 0x02, 0x02,      // ASCII 181
    0x05, 0x05, 0x05, 0x05, 0x1D, 0x05, 0x05,      // ASCII 182
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x05, 0x05,      // ASCII 183
    0x00, 0x00, 0x1E, 0x02, 0x1E, 0x02, 0x02,      // ASCII 184
    0x05, 0x05, 0x1D, 0x01, 0x1D, 0x05, 0x05,      // ASCII 185
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,      // ASCII 186
    0x00, 0x00, 0x1F, 0x01, 0x1D, 0x05, 0x05,      // ASCII 187
    0x05, 0x05, 0x1D, 0x01, 0x1F, 0x00, 0x00,      // ASCII 188
    0x05, 0x05, 0x05, 0x05, 0x1F, 0x00, 0x00,      // ASCII 189
    0x02, 0x02, 0x1E, 0x02, 0x1E, 0x00, 0x00,      // ASCII 190
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x02, 0x02,      // ASCII 191
    0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00,      // ASCII 192
    0x02, 0x02, 0x02, 0x02, 0x1F, 0x00, 0x00,      // ASCII 193
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x02, 0x02,      // ASCII 194
    0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02,      // ASCII 195
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00,      // ASCII 196
    0x02, 0x02, 0x02, 0x02, 0x1F, 0x02, 0x02,      // ASCII 197
    0x02, 0x02, 0x03, 0x02, 0x03, 0x02, 0x02,      // ASCII 198
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,      // ASCII 199
    0x05, 0x05, 0x05, 0x04, 0x07, 0x00, 0x00,      // ASCII 200
    0x00, 0x00, 0x07, 0x04, 0x05, 0x05, 0x05,      // ASCII 201
    0x05, 0x05, 0x1D, 0x00, 0x1F, 0x00, 0x00,      // ASCII 202
    0x00, 0x00, 0x1F, 0x00, 0x1D, 0x05, 0x05,      // ASCII 203
    0x05, 0x05, 0x05, 0x04, 0x05, 0x05, 0x05,      // ASCII 204
    0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00,      // ASCII 205
    0x05, 0x05, 0x1D, 0x00, 0x1D, 0x05, 0x05,      // ASCII 206
    0x02, 0x02, 0x1F, 0x00, 0x1F, 0x00, 0x00,      // ASCII 207
    0x05, 0x05, 0x05, 0x05, 0x1F, 0x00, 0x00,      // ASCII 208
    0x00, 0x00, 0x1F, 0x00, 0x1F, 0x02, 0x02,      // ASCII 209
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x05, 0x05,      // ASCII 210
    0x05, 0x05, 0x05, 0x05, 0x07, 0x00, 0x00,      // ASCII 211
    0x02, 0x02, 0x03, 0x02, 0x03, 0x00, 0x00,      // ASCII 212
    0x00, 0x00, 0x03, 0x02, 0x03, 0x02, 0x02,      // ASCII 213
    0x00, 0x00, 0x00, 0x00, 0x07, 0x05, 0x05,      // ASCII 214
    0x05, 0x05, 0x05, 0x05, 0x1F, 0x05, 0x05,      // ASCII 215
    0x02, 0x02, 0x1F, 0x02, 0x1F, 0x02, 0x02,      // ASCII 216
    0x02, 0x02, 0x02, 0x02, 0x1E, 0x00, 0x00,      // ASCII 217
    0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02,      // ASCII 218
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,      // ASCII 219
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,      // ASCII 220
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C,      // ASCII 221
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,      // ASCII 222
    0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00,      // ASCII 223
    0x00, 0x00, 0x0D, 0x12, 0x12, 0x12, 0x0D,      // ASCII 224
    0x00, 0x0E, 0x11, 0x1E, 0x11, 0x11, 0x1E,      // ASCII 225
    0x00, 0x1F, 0x13, 0x10, 0x10, 0x10, 0x10,      // ASCII 226
    0x00, 0x1F, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,      // ASCII 227
    0x1F, 0x11, 0x08, 0x04, 0x08, 0x11, 0x1F,      // ASCII 228
    0x00, 0x00, 0x0F, 0x12, 0x12, 0x12, 0x0C,      // ASCII 229
    0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x0D, 0x18,      // ASCII 230
    0x00, 0x1F, 0x14, 0x04, 0x04, 0x04, 0x04,      // ASCII 231
    0x1F, 0x04, 0x0E, 0x11, 0x11, 0x0E, 0x04,      // ASCII 232
    0x04, 0x0A, 0x11, 0x1F, 0x11, 0x0A, 0x04,      // ASCII 233
    0x04, 0x0A, 0x11, 0x11, 0x0A, 0x0A, 0x1B,      // ASCII 234
    0x06, 0x08, 0x06, 0x0E, 0x11, 0x11, 0x0E,      // ASCII 235
    0x00, 0x00, 0x00, 0x0E, 0x15, 0x15, 0x0E,      // ASCII 236
    0x01, 0x0E, 0x13, 0x15, 0x15, 0x19, 0x0E,      // ASCII 237
    0x0E, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x0E,      // ASCII 238
    0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,      // ASCII 239
    0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00,      // ASCII 240
    0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x1F,      // ASCII 241
    0x08, 0x04, 0x02, 0x04, 0x08, 0x00, 0x1F,      // ASCII 242
    0x02, 0x04, 0x08, 0x04, 0x02, 0x00, 0x1F,      // ASCII 243
    0x07, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04,      // ASCII 244
    0x04, 0x04, 0x04, 0x04, 0x04, 0x14, 0x14,      // ASCII 245
    0x06, 0x06, 0x00, 0x1F, 0x00, 0x06, 0x06,      // ASCII 246
    0x00, 0x1D, 0x17, 0x00, 0x1D, 0x17, 0x00,      // ASCII 247
    0x0E, 0x1B, 0x1B, 0x0E, 0x00, 0x00, 0x00,      // ASCII 248
    0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,      // ASCII 249
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,      // ASCII 250
    0x07, 0x04, 0x04, 0x04, 0x14, 0x14, 0x0C,      // ASCII 251
    0x0E, 0x09, 0x09, 0x09, 0x09, 0x00, 0x00,      // ASCII 252
    0x0E, 0x03, 0x06, 0x0C, 0x0F, 0x00, 0x00,      // ASCII 253
    0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x00,      // ASCII 254
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00       // ASCII 255
};

//=============================================================================================
// Lookup table that maps an LED index to a PWM register.  This table was built with the 
// code below it
//=============================================================================================
static const unsigned char pwm_reg[] PROGMEM = 
{
    43, 59, 75, 91, 107, 123, 139, 155, 156, 140, 124, 108, 92, 76, 60, 44,
    42, 58, 74, 90, 106, 122, 138, 154, 157, 141, 125, 109, 93, 77, 61, 45,
    41, 57, 73, 89, 105, 121, 137, 153, 158, 142, 126, 110, 94, 78, 62, 46,
    40, 56, 72, 88, 104, 120, 136, 152, 159, 143, 127, 111, 95, 79, 63, 47,
    39, 55, 71, 87, 103, 119, 135, 151, 160, 144, 128, 112, 96, 80, 64, 48,
    38, 54, 70, 86, 102, 118, 134, 150, 161, 145, 129, 113, 97, 81, 65, 49,
    37, 53, 69, 85, 101, 117, 133, 149, 162, 146, 130, 114, 98, 82, 66, 50,
    36, 52, 68, 84, 100, 116, 132, 148, 163, 147, 131, 115, 99, 83, 67, 51,
};
//=============================================================================================
/* 
static void IS31FL3731::map_led_to_pwm_reg()
{
    int row, col;
    int starting_reg_l = MAX_ROWS - 1;
    int starting_reg_r = MAX_ROWS + 16*7;
    int index = 0;
    
    // Loop through every row the chip supports
    for (row = 0; row < MAX_ROWS; ++row)
    {
        // For the first 8 columns, register numbers are ascending
        for (col=0; col< MAX_COLS/2; ++col)
        {
            pwm_reg[index++] = PWM_BASE_REG + starting_reg_l + col*16;  
        }

        // For the next 8 columns, register numbers are descending
        for (col=0; col<MAX_COLS/2; ++col)
        {
            pwm_reg[index++] = PWM_BASE_REG + starting_reg_r - col*16;
        }

        --starting_reg_l;
        ++starting_reg_r;
    }
}
//=============================================================================================
*/

//=============================================================================================
// transmit() - Transmits data to the device via I2C
//=============================================================================================
void IS31FL3731::transmit(const uint8_t* data, size_t length)
{
    Wire.flush();
    Wire.beginTransmission(m_i2c_address);
    Wire.write(data, length);
    Wire.endTransmission();
}
//=============================================================================================


//=============================================================================================
// write_reg() - Writes an 8-bit value to an 8-bit address on the device
//=============================================================================================
void IS31FL3731::write_reg(unsigned char address, unsigned char value)
{
    unsigned char buffer[2] = { address, value };
    transmit(buffer, 2);
}
//=============================================================================================



//=============================================================================================
// display_image() - Transmits the PWM values for our entire bitmap to the device
//=============================================================================================
void IS31FL3731::display_image()
{
    // This holds the address of a PWM register plus 1 PWM value
    unsigned char cmd[2];

    // Find out how many columns of LEDs are missing from the right-edge
    const int missing_led_cols = MAX_COLS - PHYS_COLS;

    // Create a bitmask with a 1 in either the left-most or right-most bit 
    // that corresponds to a physical LED
    uint16_t initial_mask = (m_orientation) ? (1 << 15) : (1 << missing_led_cols);

    // Loop through each row of the display from top to bottom
    for (int row = 0; row < PHYS_ROWS; ++row)
    {
        // Find the index of the first LED in this row
        int led_index = row * MAX_COLS;

        // Fetch the LED-is-on bits for the bitmap-row we care about
        uint16_t row_bits = (m_orientation) ? m_bitmap[row] : m_bitmap[PHYS_ROWS - 1 - row];

        // The mask for this row starts with a 1 in either the left-most or right-most bit
        // that coreresponds to a physical LED
        uint16_t mask = initial_mask;

        // Loop through each column, turning a bit into a PWM value
        for (int col = 0; col < PHYS_COLS; ++col)
        {
            // Find out which PWM register corresponds to this LED
            cmd[0] = pgm_read_byte_near(pwm_reg + led_index++);

            // Determine whether we're turning this LED on or off
            cmd[1] = (row_bits & mask) ? m_brightness : 0;
            
            // Transmit the PWM value to the register
            transmit(cmd, 2);

            // And shift to the next (or previous) LED
            if (m_orientation)
                mask >>= 1;
            else
                mask <<= 1;
        }

    }
}
//=============================================================================================



//=============================================================================================
// init() - Call once at startup
//=============================================================================================
void IS31FL3731::init(int i2c_address, unsigned char brightness)
{
    // Save the I2C address for future use
    m_i2c_address = i2c_address;

    // Save the PWM value we want to use to turn an LED on
    m_brightness = brightness;

    // We start out in right-side-up orientation
    m_orientation = true;

    // Clear the bitmap.  This is the equivalent of a "clear the screen"
    memset(m_bitmap, 0, sizeof m_bitmap);

    // Ensure that the Wire library is running
    Wire.begin();

    // Select frame #9, which is actually the control registers
    write_reg(FRAME_SELECT_REG, CONFIG_FRAME);

    // Ensure that the chip isn't in shutdown mode
    write_reg(SHUTDOWN_REG, 1);

    // This selects "picture mode" with frame #0 the frame to be displayed
    write_reg(0,0);

    // Select frame #0
    write_reg(FRAME_SELECT_REG, 0);

    // Send the PWM equivalent of our bitmap to the device
    display_image();

    // This writes a 1-bit to the "enable" bit for every LED
    unsigned char enable_cmd[] =
    {
        LED_CTRL_BASE_REG,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    // Send the "enable all LEDs" command
    transmit(enable_cmd, sizeof enable_cmd);
}
//=============================================================================================




//=============================================================================================
// fill() - Turns on every pixel in the display
//=============================================================================================
void IS31FL3731::fill()
{
    // Fill every bit of the bitmap so that every LED turns on
    memset(m_bitmap, 0, sizeof m_bitmap);

    // And display the image
    display_image();
}
//=============================================================================================




//=============================================================================================
// set_brightness() - Define the PWM value of an "on" pixel and optionally update the display
//=============================================================================================
void IS31FL3731::set_brightness(unsigned char brightness, bool update_image)
{
    // Save the PWM value we want to use for an "on" pixel
    m_brightness = brightness;

    // And optionally update the display with this new brightness
    if (update_image) display_image();
}
//=============================================================================================


//=============================================================================================
// set_orientation() - Sets the orientation of the display and optionally redisplays the
//                     current image
//=============================================================================================
void IS31FL3731::set_orientation(bool orientation, bool update_image)
{
    // Save the specified orientation for future reference
    m_orientation = orientation;
    
    // If the caller wants us to update the image, make it so
    if (update_image) display_image();
}
//=============================================================================================



//=============================================================================================
// vertical_bar() - Draws a vertical bar at the specified column
//=============================================================================================
void IS31FL3731::vertical_bar(int column, bool update_image)
{
    // Ensure that the column number the caller gave us physically exists
    if (column < 0 || column >= PHYS_COLS) return;

    // Turn the column into a bit-mask
    int mask = 1 << (15 - column);

    // Turn that bit on in each row
    for (int row = 0; row < PHYS_ROWS; ++row) m_bitmap[row] |= mask;

    // And optionally update the display with this new vertical bar
    if (update_image) display_image();
}
//=============================================================================================



//=============================================================================================
// print() - Merges the bitmap of a character into our display bitmap.
// 
// This does not transmit the bitmap to the device
//=============================================================================================
void IS31FL3731::print(int display_row, int display_col, uint8_t c)
{
    // These are the dimensions of a character in our font
    const int CHAR_COLS = 5;
    const int CHAR_ROWS = 7;

    // Get the index in the font table where our 5x7 character begins
    int font_index = c * CHAR_ROWS;

    // Determine which bit number the left-most bit of the 5 bit-wide character should reside in
    int shift = (PHYS_COLS - CHAR_COLS + 1) - display_col;

    // Loop through each row of the character...
    for (int char_row = 0; char_row < CHAR_ROWS; ++char_row)
    {
        // If it's off the end of the display, we're done
        if (display_row >= MAX_ROWS) break;
        
        // Fetch the bits for this row of the character
        uint16_t bits = pgm_read_byte_near(font + font_index++);

        // And merge those bits into our display bitmap
        m_bitmap[display_row++] |= (bits << shift);
    }
}
//=============================================================================================



//=============================================================================================
// print() - Displays two characters on the screen
//=============================================================================================
void IS31FL3731::print(char c1, char c2)
{
    // Clear all bits to off
    memset(m_bitmap, 0, sizeof m_bitmap);

    // Print either 1 or 2 characters
    if (c2 == 0)
        print(0, 5, (unsigned char)c1);
    else
    {
        print(0, 2, (unsigned char)c1);
        print(0, 8, (unsigned char)c2);
    }

    // Transmit our bitmap to the device
    display_image();
}
//=============================================================================================


//=============================================================================================
// print() - Print an integer value between -9 and 99
//=============================================================================================
void IS31FL3731::print(int value)
{
    char c1 = '*', c2 = '*';

    // If it's a negative one digit number...
    if (value >= -9 && value <= -1)
    {
        c1 = '-';
        c2 = '0' - value;
    }

    // If it's a positive single-digit number...
    else if (value >= 0 && value <= 9)
    {
        c1 = '0' + value;
        c2 = 0;
    }

    // If it's a two digit number...
    if (value >= 10 && value <= 99)
    {
        c1 = '0' + value / 10;
        c2 = '0' + value % 10;
    }

    // And display the two characters
    print(c1, c2);
}
//=============================================================================================


 

//=============================================================================================
// print() - Print a rounded floating point value between -9 and 99
//=============================================================================================
void IS31FL3731::print(double value)
{
    print((int)(value + .5));
}
//=============================================================================================




//=============================================================================================
// print() - Print a 2 character string
//=============================================================================================
void IS31FL3731::print(const char* str)
{
    char c1 = str[0];
    char c2 = str[1];
    if (c1 == 0) c2 = 0;
    print(c1, c2);
}
//=============================================================================================
