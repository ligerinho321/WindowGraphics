#ifndef WG_SCANCODES_H_
#define WG_SCANCODES_H_

typedef enum WG_ScanCode{
    WG_SCANCODE_UNKNOWN,

    WG_SCANCODE_A,
    WG_SCANCODE_B,
    WG_SCANCODE_C,
    WG_SCANCODE_D,
    WG_SCANCODE_E,
    WG_SCANCODE_F,
    WG_SCANCODE_G,
    WG_SCANCODE_H,
    WG_SCANCODE_I,
    WG_SCANCODE_J,
    WG_SCANCODE_K,
    WG_SCANCODE_L,
    WG_SCANCODE_M,
    WG_SCANCODE_N,
    WG_SCANCODE_O,
    WG_SCANCODE_P,
    WG_SCANCODE_Q,
    WG_SCANCODE_R,
    WG_SCANCODE_S,
    WG_SCANCODE_T,
    WG_SCANCODE_U,
    WG_SCANCODE_V,
    WG_SCANCODE_W,
    WG_SCANCODE_X,
    WG_SCANCODE_Y,
    WG_SCANCODE_Z,

    WG_SCANCODE_1,
    WG_SCANCODE_2,
    WG_SCANCODE_3,
    WG_SCANCODE_4,
    WG_SCANCODE_5,
    WG_SCANCODE_6,
    WG_SCANCODE_7,
    WG_SCANCODE_8,
    WG_SCANCODE_9,
    WG_SCANCODE_0,

    WG_SCANCODE_RETURN,
    WG_SCANCODE_ESCAPE,
    WG_SCANCODE_BACKSPACE,
    WG_SCANCODE_TAB,
    WG_SCANCODE_SPACE,

    WG_SCANCODE_DASH_UNDERSCORE,
    WG_SCANCODE_EQUALS_PLUS,
    WG_SCANCODE_LEFTBRACE,
    WG_SCANCODE_RIGHTBRACE,
    WG_SCANCODE_BACKSLASH,
    WG_SCANCODE_NONUSHASH,
    WG_SCANCODE_SEMICOLON,
    WG_SCANCODE_ASPOSTROPHE,
    WG_SCANCODE_GRAVE,
    WG_SCANCODE_COMMA,
    WG_SCANCODE_PERIOD,
    WG_SCANCODE_QUESTIONMARK,
    WG_SCANCODE_CAPSLOCK,

    WG_SCANCODE_F1,
    WG_SCANCODE_F2,
    WG_SCANCODE_F3,
    WG_SCANCODE_F4,
    WG_SCANCODE_F5,
    WG_SCANCODE_F6,
    WG_SCANCODE_F7,
    WG_SCANCODE_F8,
    WG_SCANCODE_F9,
    WG_SCANCODE_F10,
    WG_SCANCODE_F11,
    WG_SCANCODE_F12,
    
    WG_SCANCODE_PRINTSCREEN,
    WG_SCANCODE_SCROLLLOCK,
    WG_SCANCODE_PAUSE,
    WG_SCANCODE_INSERT,

    WG_SCANCODE_HOME,
    WG_SCANCODE_PAGEUP,
    WG_SCANCODE_DELETE,
    WG_SCANCODE_END,
    WG_SCANCODE_PAGEDOWN,
    WG_SCANCODE_RIGHT,
    WG_SCANCODE_LEFT,
    WG_SCANCODE_DOWN,
    WG_SCANCODE_UP,

    WG_SCANCODE_NUMLOCK,

    WG_SCANCODE_KP_DIVIDE,
    WG_SCANCODE_KP_MULTIPLY,
    WG_SCANCODE_KP_MINUS,
    WG_SCANCODE_KP_PLUS,
    WG_SCANCODE_KP_ENTER,
    WG_SCANCODE_KP_1,
    WG_SCANCODE_KP_2,
    WG_SCANCODE_KP_3,
    WG_SCANCODE_KP_4,
    WG_SCANCODE_KP_5,
    WG_SCANCODE_KP_6,
    WG_SCANCODE_KP_7,
    WG_SCANCODE_KP_8,
    WG_SCANCODE_KP_9,
    WG_SCANCODE_KP_0, 
    WG_SCANCODE_KP_PERIOD,

    WG_SCANCODE_NONUSBACKSLASH,

    WG_SCANCODE_APPLICATION,
    WG_SCANCODE_POWER,
    WG_SCANCODE_SLEEP,
    WG_SCANCODE_WAKEUP,

    WG_SCANCODE_KP_EQUALS,

    WG_SCANCODE_F13,
    WG_SCANCODE_F14,
    WG_SCANCODE_F15,
    WG_SCANCODE_F16,
    WG_SCANCODE_F17,
    WG_SCANCODE_F18,
    WG_SCANCODE_F19,
    WG_SCANCODE_F20,
    WG_SCANCODE_F21,
    WG_SCANCODE_F22,
    WG_SCANCODE_F23,
    WG_SCANCODE_F24,

    WG_SCANCODE_KP_COMMA,

    WG_SCANCODE_INTERNATIONAL1,
    WG_SCANCODE_INTERNATIONAL2,
    WG_SCANCODE_INTERNATIONAL3,
    WG_SCANCODE_INTERNATIONAL4,
    WG_SCANCODE_INTERNATIONAL5,
    WG_SCANCODE_INTERNATIONAL6,

    WG_SCANCODE_LANG1,
    WG_SCANCODE_LANG2,
    WG_SCANCODE_LANG3,
    WG_SCANCODE_LANG4,
    WG_SCANCODE_LANG5,

    WG_SCANCODE_LCTRL,
    WG_SCANCODE_RCTRL,

    WG_SCANCODE_LSHIFT,
    WG_SCANCODE_RSHIFT,

    WG_SCANCODE_LALT,
    WG_SCANCODE_RALT, 

    WG_SCANCODE_LGUI,
    WG_SCANCODE_RGUI,

    WG_SCANCODE_AUDIONEXT,
    WG_SCANCODE_AUDIOPREV,
    WG_SCANCODE_AUDIOSTOP,
    WG_SCANCODE_AUDIOPLAY,
    WG_SCANCODE_AUDIOMUTE,
    WG_SCANCODE_VOLUMEUP,
    WG_SCANCODE_VOLUMEDOWN,
    WG_SCANCODE_MEDIACONTROL,
    WG_SCANCODE_EMAILREADER,
    WG_SCANCODE_CALCULATOR,
    WG_SCANCODE_COMPUTER,
    WG_SCANCODE_AC_SEARCH,
    WG_SCANCODE_AC_HOME,
    WG_SCANCODE_AC_BACK,
    WG_SCANCODE_AC_FORWARD,
    WG_SCANCODE_AC_STOP,
    WG_SCANCODE_AC_REFRESH,
    WG_SCANCODE_AC_BOOKMARKS,

    WG_NUM_SCANCODES
}WG_ScanCode;

static const unsigned char window_scancode_table[256] = {
/* 0x0000 */ WG_SCANCODE_UNKNOWN,
/* 0x0001 */ WG_SCANCODE_ESCAPE,
/* 0x0002 */ WG_SCANCODE_1,
/* 0x0003 */ WG_SCANCODE_2,
/* 0x0004 */ WG_SCANCODE_3,
/* 0x0005 */ WG_SCANCODE_4,
/* 0x0006 */ WG_SCANCODE_5,
/* 0x0007 */ WG_SCANCODE_6,
/* 0x0008 */ WG_SCANCODE_7,
/* 0x0009 */ WG_SCANCODE_8,
/* 0x000A */ WG_SCANCODE_9,
/* 0x000B */ WG_SCANCODE_0,
/* 0x000C */ WG_SCANCODE_DASH_UNDERSCORE,
/* 0x000D */ WG_SCANCODE_EQUALS_PLUS,
/* 0x000E */ WG_SCANCODE_BACKSPACE,
/* 0x000F */ WG_SCANCODE_TAB,
/* 0x0010 */ WG_SCANCODE_Q,
/* 0x0011 */ WG_SCANCODE_W,
/* 0x0012 */ WG_SCANCODE_E,
/* 0x0013 */ WG_SCANCODE_R,
/* 0x0014 */ WG_SCANCODE_T,
/* 0x0015 */ WG_SCANCODE_Y,
/* 0x0016 */ WG_SCANCODE_U,
/* 0x0017 */ WG_SCANCODE_I,
/* 0x0018 */ WG_SCANCODE_O,
/* 0x0019 */ WG_SCANCODE_P,
/* 0x001A */ WG_SCANCODE_LEFTBRACE,
/* 0x001B */ WG_SCANCODE_RIGHTBRACE,
/* 0x001C */ WG_SCANCODE_RETURN,
/* 0x001D */ WG_SCANCODE_LCTRL,
/* 0x001E */ WG_SCANCODE_A,
/* 0x001F */ WG_SCANCODE_S,
/* 0x0020 */ WG_SCANCODE_D,
/* 0x0021 */ WG_SCANCODE_F,
/* 0x0022 */ WG_SCANCODE_G,
/* 0x0023 */ WG_SCANCODE_H,
/* 0x0024 */ WG_SCANCODE_J,
/* 0x0025 */ WG_SCANCODE_K,
/* 0x0026 */ WG_SCANCODE_L,
/* 0x0027 */ WG_SCANCODE_SEMICOLON,
/* 0x0028 */ WG_SCANCODE_ASPOSTROPHE,
/* 0x0029 */ WG_SCANCODE_GRAVE,
/* 0x002A */ WG_SCANCODE_LSHIFT,
/* 0x002B */ WG_SCANCODE_BACKSLASH,
/* 0x002C */ WG_SCANCODE_Z,
/* 0x002D */ WG_SCANCODE_X,
/* 0x002E */ WG_SCANCODE_C,
/* 0x002F */ WG_SCANCODE_V,
/* 0x0030 */ WG_SCANCODE_B,
/* 0x0031 */ WG_SCANCODE_N,
/* 0x0032 */ WG_SCANCODE_M,
/* 0x0033 */ WG_SCANCODE_COMMA,
/* 0x0034 */ WG_SCANCODE_PERIOD,
/* 0x0035 */ WG_SCANCODE_QUESTIONMARK,
/* 0x0036 */ WG_SCANCODE_RSHIFT,
/* 0x0037 */ WG_SCANCODE_KP_MULTIPLY,
/* 0x0038 */ WG_SCANCODE_LALT,
/* 0x0039 */ WG_SCANCODE_SPACE,
/* 0x003A */ WG_SCANCODE_CAPSLOCK,
/* 0x003B */ WG_SCANCODE_F1,
/* 0x003C */ WG_SCANCODE_F2,
/* 0x003D */ WG_SCANCODE_F3,
/* 0x003E */ WG_SCANCODE_F4,
/* 0x003F */ WG_SCANCODE_F5,
/* 0x0040 */ WG_SCANCODE_F6,
/* 0x0041 */ WG_SCANCODE_F7,
/* 0x0042 */ WG_SCANCODE_F8,
/* 0x0043 */ WG_SCANCODE_F9,
/* 0x0044 */ WG_SCANCODE_F10,
/* 0x0045 */ WG_SCANCODE_NUMLOCK,
/* 0x0046 */ WG_SCANCODE_SCROLLLOCK,
/* 0x0047 */ WG_SCANCODE_KP_7,
/* 0x0048 */ WG_SCANCODE_KP_8,
/* 0x0049 */ WG_SCANCODE_KP_9,
/* 0x004A */ WG_SCANCODE_KP_MINUS,
/* 0x004B */ WG_SCANCODE_KP_4,
/* 0x004C */ WG_SCANCODE_KP_5,
/* 0x004D */ WG_SCANCODE_KP_6,
/* 0x004E */ WG_SCANCODE_KP_PLUS,
/* 0x004F */ WG_SCANCODE_KP_1,
/* 0x0050 */ WG_SCANCODE_KP_2,
/* 0x0051 */ WG_SCANCODE_KP_3,
/* 0x0052 */ WG_SCANCODE_KP_0,
/* 0x0053 */ WG_SCANCODE_KP_PERIOD,
/* 0x0054 */ WG_SCANCODE_UNKNOWN,
/* 0x0055 */ WG_SCANCODE_UNKNOWN,
/* 0x0056 */ WG_SCANCODE_NONUSBACKSLASH,
/* 0x0057 */ WG_SCANCODE_F11,
/* 0x0058 */ WG_SCANCODE_F12,
/* 0x0059 */ WG_SCANCODE_KP_EQUALS,
/* 0x005A */ WG_SCANCODE_UNKNOWN,
/* 0x005B */ WG_SCANCODE_UNKNOWN,
/* 0x005C */ WG_SCANCODE_INTERNATIONAL6,
/* 0x005D */ WG_SCANCODE_UNKNOWN,
/* 0x005E */ WG_SCANCODE_UNKNOWN,
/* 0x005F */ WG_SCANCODE_UNKNOWN,
/* 0x0060 */ WG_SCANCODE_UNKNOWN,
/* 0x0061 */ WG_SCANCODE_UNKNOWN,
/* 0x0062 */ WG_SCANCODE_UNKNOWN,
/* 0x0063 */ WG_SCANCODE_UNKNOWN,
/* 0x0064 */ WG_SCANCODE_F13,
/* 0x0065 */ WG_SCANCODE_F14,
/* 0x0066 */ WG_SCANCODE_F15,
/* 0x0067 */ WG_SCANCODE_F16,
/* 0x0068 */ WG_SCANCODE_F17,
/* 0x0069 */ WG_SCANCODE_F18,
/* 0x006A */ WG_SCANCODE_F19,
/* 0x006B */ WG_SCANCODE_F20,
/* 0x006C */ WG_SCANCODE_F21,
/* 0x006D */ WG_SCANCODE_F22,
/* 0x006E */ WG_SCANCODE_F23,
/* 0x006F */ WG_SCANCODE_UNKNOWN,
/* 0x0070 */ WG_SCANCODE_INTERNATIONAL2,
/* 0x0071 */ WG_SCANCODE_LANG2,
/* 0x0072 */ WG_SCANCODE_LANG1,
/* 0x0073 */ WG_SCANCODE_INTERNATIONAL1,
/* 0x0074 */ WG_SCANCODE_UNKNOWN,
/* 0x0075 */ WG_SCANCODE_UNKNOWN,
/* 0x0076 */ WG_SCANCODE_F24,
/* 0x0077 */ WG_SCANCODE_LANG4,
/* 0x0078 */ WG_SCANCODE_LANG3,
/* 0x0079 */ WG_SCANCODE_INTERNATIONAL4,
/* 0x007A */ WG_SCANCODE_UNKNOWN,
/* 0x007B */ WG_SCANCODE_INTERNATIONAL5,
/* 0x007C */ WG_SCANCODE_UNKNOWN,
/* 0x007D */ WG_SCANCODE_INTERNATIONAL3,
/* 0x007E */ WG_SCANCODE_KP_COMMA,
/* 0x007F */ WG_SCANCODE_UNKNOWN,
/* 0xE000 */ WG_SCANCODE_UNKNOWN,
/* 0xE001 */ WG_SCANCODE_UNKNOWN,
/* 0xE002 */ WG_SCANCODE_UNKNOWN,
/* 0xE003 */ WG_SCANCODE_UNKNOWN,
/* 0xE004 */ WG_SCANCODE_UNKNOWN,
/* 0xE005 */ WG_SCANCODE_UNKNOWN,
/* 0xE006 */ WG_SCANCODE_UNKNOWN,
/* 0xE007 */ WG_SCANCODE_UNKNOWN,
/* 0xE008 */ WG_SCANCODE_UNKNOWN,
/* 0xE009 */ WG_SCANCODE_UNKNOWN,
/* 0xE00A */ WG_SCANCODE_UNKNOWN,
/* 0xE00B */ WG_SCANCODE_UNKNOWN,
/* 0xE00C */ WG_SCANCODE_UNKNOWN,
/* 0xE00D */ WG_SCANCODE_UNKNOWN,
/* 0xE00E */ WG_SCANCODE_UNKNOWN,
/* 0xE00F */ WG_SCANCODE_UNKNOWN,
/* 0xE010 */ WG_SCANCODE_AUDIOPREV,
/* 0xE011 */ WG_SCANCODE_UNKNOWN,
/* 0xE012 */ WG_SCANCODE_UNKNOWN,
/* 0xE013 */ WG_SCANCODE_UNKNOWN,
/* 0xE014 */ WG_SCANCODE_UNKNOWN,
/* 0xE015 */ WG_SCANCODE_UNKNOWN,
/* 0xE016 */ WG_SCANCODE_UNKNOWN,
/* 0xE017 */ WG_SCANCODE_UNKNOWN,
/* 0xE018 */ WG_SCANCODE_UNKNOWN,
/* 0xE019 */ WG_SCANCODE_AUDIONEXT,
/* 0xE01A */ WG_SCANCODE_UNKNOWN,
/* 0xE01B */ WG_SCANCODE_UNKNOWN,
/* 0xE01C */ WG_SCANCODE_KP_ENTER,
/* 0xE01D */ WG_SCANCODE_RCTRL,
/* 0xE01E */ WG_SCANCODE_UNKNOWN,
/* 0xE01F */ WG_SCANCODE_UNKNOWN,
/* 0xE020 */ WG_SCANCODE_AUDIOMUTE,
/* 0xE021 */ WG_SCANCODE_CALCULATOR,
/* 0xE022 */ WG_SCANCODE_AUDIOPLAY,
/* 0xE023 */ WG_SCANCODE_UNKNOWN,
/* 0xE024 */ WG_SCANCODE_AC_STOP,
/* 0xE025 */ WG_SCANCODE_UNKNOWN,
/* 0xE026 */ WG_SCANCODE_UNKNOWN,
/* 0xE027 */ WG_SCANCODE_UNKNOWN,
/* 0xE028 */ WG_SCANCODE_UNKNOWN,
/* 0xE029 */ WG_SCANCODE_UNKNOWN,
/* 0xE02A */ WG_SCANCODE_UNKNOWN,
/* 0xE02B */ WG_SCANCODE_UNKNOWN,
/* 0xE02C */ WG_SCANCODE_UNKNOWN,
/* 0xE02D */ WG_SCANCODE_UNKNOWN,
/* 0xE02E */ WG_SCANCODE_VOLUMEDOWN,
/* 0xE02F */ WG_SCANCODE_UNKNOWN,
/* 0xE030 */ WG_SCANCODE_VOLUMEUP,
/* 0xE031 */ WG_SCANCODE_UNKNOWN,
/* 0xE032 */ WG_SCANCODE_AC_HOME,
/* 0xE033 */ WG_SCANCODE_UNKNOWN,
/* 0xE034 */ WG_SCANCODE_UNKNOWN,
/* 0xE035 */ WG_SCANCODE_KP_DIVIDE,
/* 0xE036 */ WG_SCANCODE_UNKNOWN,
/* 0xE037 */ WG_SCANCODE_PRINTSCREEN,
/* 0xE038 */ WG_SCANCODE_RALT,
/* 0xE039 */ WG_SCANCODE_UNKNOWN,
/* 0xE03A */ WG_SCANCODE_UNKNOWN,
/* 0xE03B */ WG_SCANCODE_UNKNOWN,
/* 0xE03C */ WG_SCANCODE_UNKNOWN,
/* 0xE03D */ WG_SCANCODE_UNKNOWN,
/* 0xE03E */ WG_SCANCODE_UNKNOWN,
/* 0xE03F */ WG_SCANCODE_UNKNOWN,
/* 0xE040 */ WG_SCANCODE_UNKNOWN,
/* 0xE041 */ WG_SCANCODE_UNKNOWN,
/* 0xE042 */ WG_SCANCODE_UNKNOWN,
/* 0xE043 */ WG_SCANCODE_UNKNOWN,
/* 0xE044 */ WG_SCANCODE_UNKNOWN,
/* 0xE045 */ WG_SCANCODE_NUMLOCK,
/* 0xE046 */ WG_SCANCODE_PAUSE,
/* 0xE047 */ WG_SCANCODE_HOME,
/* 0xE048 */ WG_SCANCODE_UP,
/* 0xE049 */ WG_SCANCODE_PAGEUP,
/* 0xE04A */ WG_SCANCODE_UNKNOWN,
/* 0xE04B */ WG_SCANCODE_LEFT,
/* 0xE04C */ WG_SCANCODE_UNKNOWN,
/* 0xE04D */ WG_SCANCODE_RIGHT,
/* 0xE04E */ WG_SCANCODE_UNKNOWN,
/* 0xE04F */ WG_SCANCODE_END,
/* 0xE050 */ WG_SCANCODE_DOWN,
/* 0xE051 */ WG_SCANCODE_PAGEDOWN,
/* 0xE052 */ WG_SCANCODE_INSERT,
/* 0xE053 */ WG_SCANCODE_DELETE,
/* 0xE054 */ WG_SCANCODE_UNKNOWN,
/* 0xE055 */ WG_SCANCODE_UNKNOWN,
/* 0xE056 */ WG_SCANCODE_UNKNOWN,
/* 0xE057 */ WG_SCANCODE_UNKNOWN,
/* 0xE058 */ WG_SCANCODE_UNKNOWN,
/* 0xE059 */ WG_SCANCODE_UNKNOWN,
/* 0xE05A */ WG_SCANCODE_UNKNOWN,
/* 0xE05B */ WG_SCANCODE_LGUI,
/* 0xE05C */ WG_SCANCODE_RGUI,
/* 0xE05D */ WG_SCANCODE_APPLICATION,
/* 0xE05E */ WG_SCANCODE_POWER,
/* 0xE05F */ WG_SCANCODE_SLEEP,
/* 0xE060 */ WG_SCANCODE_UNKNOWN,
/* 0xE061 */ WG_SCANCODE_UNKNOWN,
/* 0xE062 */ WG_SCANCODE_UNKNOWN,
/* 0xE063 */ WG_SCANCODE_WAKEUP,
/* 0xE064 */ WG_SCANCODE_UNKNOWN,
/* 0xE065 */ WG_SCANCODE_AC_SEARCH,
/* 0xE066 */ WG_SCANCODE_AC_BOOKMARKS,
/* 0xE067 */ WG_SCANCODE_AC_REFRESH,
/* 0xE068 */ WG_SCANCODE_AC_STOP,
/* 0xE069 */ WG_SCANCODE_AC_FORWARD,
/* 0xE06A */ WG_SCANCODE_AC_BACK,
/* 0xE06B */ WG_SCANCODE_COMPUTER,
/* 0xE06C */ WG_SCANCODE_EMAILREADER,
/* 0xE06D */ WG_SCANCODE_MEDIACONTROL,
/* 0xE06E */ WG_SCANCODE_UNKNOWN,
/* 0xE06F */ WG_SCANCODE_UNKNOWN,
/* 0xE070 */ WG_SCANCODE_UNKNOWN,
/* 0xE071 */ WG_SCANCODE_UNKNOWN,
/* 0xE072 */ WG_SCANCODE_UNKNOWN,
/* 0xE073 */ WG_SCANCODE_UNKNOWN,
/* 0xE074 */ WG_SCANCODE_UNKNOWN,
/* 0xE075 */ WG_SCANCODE_UNKNOWN,
/* 0xE076 */ WG_SCANCODE_UNKNOWN,
/* 0xE077 */ WG_SCANCODE_UNKNOWN,
/* 0xE078 */ WG_SCANCODE_UNKNOWN,
/* 0xE079 */ WG_SCANCODE_UNKNOWN,
/* 0xE07A */ WG_SCANCODE_UNKNOWN,
/* 0xE07B */ WG_SCANCODE_UNKNOWN,
/* 0xE07C */ WG_SCANCODE_UNKNOWN,
/* 0xE07D */ WG_SCANCODE_UNKNOWN,
/* 0xE07E */ WG_SCANCODE_UNKNOWN,
/* 0xE07F */ WG_SCANCODE_UNKNOWN,
};

#endif