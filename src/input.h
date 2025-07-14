
// @TODO: need to map these to the keys in raylib

enum InputDeviceType {
  InputDeviceType_Invalid,
  InputDeviceType_Keyboard,
  InputDeviceType_Mouse,
  InputDeviceType_Gamepad,
};

struct InputDevice {
  InputDeviceType type;

  int32 discreteCount;
    
  int32 *framesHeld;
  bool *released;
  bool *pressed;
  float32 *timePressed;

  int32 analogueCount;
    
  float32 *prevAnalogue;
  float32 *analogue;
};

enum InputKeyType : uint32 {
  Input_A,
  Input_B,
  Input_C,
  Input_D,
  Input_E,
  Input_F,
  Input_G,
  Input_H,
  Input_I,
  Input_J,
  Input_K,
  Input_L,
  Input_M,
  Input_N,
  Input_O,
  Input_P,
  Input_Q,
  Input_R,
  Input_S,
  Input_T,
  Input_U,
  Input_V,
  Input_W,
  Input_X,
  Input_Y,
  Input_Z,
  Input_0,
  Input_1,
  Input_2,
  Input_3,
  Input_4,
  Input_5,
  Input_6,
  Input_7,
  Input_8,
  Input_9,
  Input_Equal,
  Input_Minus,
  Input_RightBracket,
  Input_LeftBracket,
  Input_Quote,
  Input_Semicolon,
  Input_Backslash,
  Input_Comma,
  Input_Slash,
  Input_Period,
  Input_Return,
  Input_Tab,
  Input_Space,
  Input_Backspace,
  Input_Escape,
  Input_Tick,
  Input_Win,
  Input_Shift,
  Input_CapsLock,
  Input_Alt,
  Input_Control,
  Input_RightWin,
  Input_RightShift,
  Input_RightAlt,
  Input_RightControl,
  Input_Function,
  Input_F1,
  Input_F2,
  Input_F3,
  Input_F4,
  Input_F5,
  Input_F6,
  Input_F7,
  Input_F8,
  Input_F9,
  Input_F10,
  Input_F11,
  Input_F12,
  Input_F13,
  Input_F14,
  Input_F15,
  Input_F16,
  Input_F17,
  Input_F18,
  Input_F19,
  Input_F20,
  Input_F21,
  Input_F22,
  Input_F23,
  Input_F24,
  Input_Help,
  Input_Home,
  Input_Insert,
  Input_PageUp,
  Input_ForwardDelete,
  Input_End,
  Input_PageDown,
  Input_LeftArrow,
  Input_RightArrow,
  Input_DownArrow,
  Input_UpArrow,

  Input_KeyCount,
};

#if 1
enum InputMouseButton : uint32 {
  Input_MouseLeft,
  Input_MouseRight,
  Input_MouseMiddle,

  Input_MouseDiscreteCount,
};


enum InputMouseAnalogue : uint32 {
  Input_MousePositionX,
  Input_MousePositionY,

  MouseAnalogue_Scroll,

  Input_MouseAnalogueCount,
};
#endif

// This is mapping
// Wait this is supposed to map opposite: take the value we get from raylib and map to our key
#if 0
uint32 KeyTypeMap [] = {
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,

  KEY_ONE,
  KEY_TWO,
  KEY_THREE,
  KEY_FOUR,
  KEY_FIVE,
  KEY_SIX,
  KEY_SEVEN,
  KEY_EIGHT,
  KEY_NINE,
  KEY_ZERO,
};
#else
static int32 KeyTypeMap[] = {
    -1,
    -1, // VK_LBUTTON
    -1, // VK_RBUTTON
    -1, // VK_CANCEL
    -1, // VK_MBUTTON
    -1, // VK_XBUTTON1
    -1, // VK_XBUTTON2

    -1, // Undefined
    Input_Backspace, // VK_BACK
    Input_Tab, // VK_TAB

    // Reserved
    -1, -1,
    -1, // VK_CLEAR

    Input_Return, // VK_RETURN

    // Undefined
    -1, -1, 

    Input_Shift, // VK_SHIFT
    Input_Control, // VK_CONTROL
    Input_Alt, // VK_MENU

    -1, // VK_PAUSE
    -1, // VK_CAPITAL
    -1, // VK_KANA, VK_HANGUEL VK_HANGUL
    -1, // Undefined
    -1, // VK_JUNJA
    -1, // VK_FINAL
    -1, // VK_HANJA, VK_KANJI

    -1, // Undefined
    Input_Escape, // VK_ESCAPE

    -1, // VK_CONVERT
    -1, //VK_NONCONVERT
    -1, //VK_ACCEPT
    -1, //VK_MODECHANGE

    Input_Space, // VK_SPACE
    -1, // VK_PRIOR
    -1, // VK_NEXT
    Input_End, // VK_END
    Input_Home, // VK_HOME
    Input_LeftArrow, // VK_LEFT
    Input_UpArrow, // VK_UP
    Input_RightArrow, // VK_RIGHT
    Input_DownArrow, // VK_DOWN
    -1, // VK_SELECT
    -1, // VK_PRINT
    -1, // VK_EXECUTE
    -1, // VK_SNAPSHOT
    Input_Insert, // VK_INSERT
    Input_ForwardDelete, // VK_DELETE
    Input_Help, // VK_HELP
    Input_0,
    Input_1,
    Input_2,
    Input_3,
    Input_4,
    Input_5,
    Input_6,
    Input_7,
    Input_8,
    Input_9,

    // Undefined
    -1, -1, -1, -1, -1, -1, -1,

    Input_A,
    Input_B,
    Input_C,
    Input_D,
    Input_E,
    Input_F,
    Input_G,
    Input_H,
    Input_I,
    Input_J,
    Input_K,
    Input_L,
    Input_M,
    Input_N,
    Input_O,
    Input_P,
    Input_Q,
    Input_R,
    Input_S,
    Input_T,
    Input_U,
    Input_V,
    Input_W,
    Input_X,
    Input_Y,
    Input_Z,
    Input_Win, // VK_LWIN
    Input_RightWin, // VK_RWIN
    -1, // VK_APPS
    -1, // Reserved
    -1, // VK_SLEEP
    -1, // VK_NUMPAD0
    -1, // VK_NUMPAD1
    -1, // VK_NUMPAD2
    -1, // VK_NUMPAD3
    -1, // VK_NUMPAD4
    -1, // VK_NUMPAD5
    -1, // VK_NUMPAD6
    -1, // VK_NUMPAD7
    -1, // VK_NUMPAD8
    -1, // VK_NUMPAD9
    -1, // VK_MULTIPLY
    -1, // VK_ADD
    -1, // VK_SEPARATOR
    -1, // VK_SUBTRACT
    -1, // VK_DECIMAL
    -1, // VK_DIVIDE
    Input_F1,
    Input_F2,
    Input_F3,
    Input_F4,
    Input_F5,
    Input_F6,
    Input_F7,
    Input_F8,
    Input_F9,
    Input_F10,
    Input_F11,
    Input_F12,
    Input_F13,
    Input_F14,
    Input_F15,
    Input_F16,
    Input_F17,
    Input_F18,
    Input_F19,
    Input_F20,
    Input_F21,
    Input_F22,
    Input_F23,
    Input_F24,

    // Unassigned
    -1, -1, -1, -1, -1, -1, -1, -1,

    -1, // VK_NUMLOCK
    -1, // VK_SCROLL

    // OEM specific
    -1, -1, -1, -1, -1,

    // Unassigned
    -1, -1, -1, -1, -1, -1, -1, -1, -1,

    Input_Shift, // VK_LSHIFT
    Input_RightShift, // VK_RSHIFT
    Input_Control, // VK_LCONTROL
    Input_RightControl, // VK_RCONTROL

    Input_Alt, // VK_LMENU
    Input_RightAlt, // VK_RMENU
    
    -1, // VK_BROWSER_BACK
    -1, // VK_BROWSER_FORWARD
    -1, // VK_BROWSER_REFRESH
    -1, // VK_BROWSER_STOP
    -1, // VK_BROWSER_SEARCH
    -1, // VK_BROWSER_FAVORITES
    -1, // VK_BROWSER_HOME
    -1, // VK_VOLUME_MUTE
    -1, // VK_VOLUME_DOWN
    -1, // VK_VOLUME_UP
    -1, // VK_MEDIA_NEXT_TRACK
    -1, // VK_MEDIA_PREV_TRACK
    -1, // VK_MEDIA_STOP
    -1, // VK_MEDIA_PLAY_PAUSE
    -1, // VK_LAUNCH_MAIL
    -1, // VK_LAUNCH_MEDIA_SELECT
    -1, // VK_LAUNCH_APP1
    -1, // VK_LAUNCH_APP2

    // Reserved
    -1, -1,

    Input_Semicolon, // VK_OEM_1
    Input_Equal, // VK_OEM_PLUS
    Input_Comma, // VK_OEM_COMMA
    Input_Minus, // VK_OEM_MINUS
    Input_Period, // VK_OEM_PERIOD
    Input_Slash, // VK_OEM_2
    Input_Tick, // VK_OEM_3

    // Reserved
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1,
    
    // Unassigned
    -1, -1, -1,

    Input_LeftBracket, // VK_OEM_4
    Input_Backslash, // VK_OEM_5
    Input_RightBracket, // VK_OEM_6
    Input_Quote, // VK_OEM_7

    // VK_OEM_8...
};
#endif

static int32 MouseButtonMap[] = {
  Input_MouseLeft,
  Input_MouseRight,
  Input_MouseMiddle,
  -1, -1, -1, -1
};


struct InputEvent {
    InputDevice *device;
    int32 index;
    bool discreteValue;
    float32 value;
};


struct InputManager {
  uint32 capacity;
  uint32 count;
  uint32 size;

  DynamicArray<InputEvent> events;

  int32 deviceCount;
  InputDevice *devices;

  uint32 charSize;
  uint32 charCount;
  char *inputChars;

  vec2i mousePos;
  vec2 mousePosNorm;
  vec2 mousePosNormSigned;
};


void AllocateInputManager(InputManager *input, MemoryArena *arena, int32 capacity, int32 deviceCapacity);

void AllocateInputDevice(InputDevice *device, InputDeviceType type, int32 discreteCount, int32 analogueCount);
