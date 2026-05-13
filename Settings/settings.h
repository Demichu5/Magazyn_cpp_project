#pragma once

// ----- GUI -----
#define G_SCREEN_WIDTH 1280
#define G_SCREEN_HEIGHT 720
#define G_X_PADDING 2
#define G_Y_PADDING 2
#define G_BUTTON_WIDTH 96
#define G_BUTTON_HEIGHT 60

// ----- COLORS -----
#define C_COLOR_GRAY1   0xC2C2C2FF
#define C_COLOR_GRAY2   0xABADB2FF
#define C_COLOR_GRAY3   0x8F9AA2FF
#define C_COLOR_GRAY4   0x72898FFF
#define C_COLOR_WHITE1  0xFAF7FFFF
#define C_COLOR_GREEN1  0x00FF00FF
#define C_COLOR_RED1    0xFF0000FF
#define C_COLOR_TYPE_SOLID   0x95A5A6FF
#define C_COLOR_TYPE_LIQUID  0x3498DBFF
#define C_COLOR_TYPE_FRAGILE 0xF1C40FFF
#define C_COLOR_HIGHLIGHT    0xE74C3CFF

// ----- STATE MACHINE -----
#define SM_None         0x00
#define SM_User         0x01
#define SM_Admin        0x02

// ----- LOGIKA MAGAZYNU -----
#define MAX_SECTIONS    6
#define MAX_SLOTS       6
#define MAX_POOL        36