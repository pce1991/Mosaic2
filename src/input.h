
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
  real32 *timePressed;

  int32 analogueCount;
    
  real32 *prevAnalogue;
  real32 *analogue;
};

enum KeyType : uint32 {
  Key_A,
  Key_B,
  Key_C,
  Key_D,
  Key_E,
  Key_F,
  Key_G,
  Key_H,
  Key_I,
  Key_J,
  Key_K,
  Key_L,
  Key_M,
  Key_N,
  Key_O,
  Key_P,
  Key_Q,
  Key_R,
  Key_S,
  Key_T,
  Key_U,
  Key_V,
  Key_W,
  Key_X,
  Key_Y,
  Key_Z,
  Key_0,
  Key_1,
  Key_2,
  Key_3,
  Key_4,
  Key_5,
  Key_6,
  Key_7,
  Key_8,
  Key_9,
  Key_Equal,
  Key_Minus,
  Key_RightBracket,
  Key_LeftBracket,
  Key_Quote,
  Key_Semicolon,
  Key_Backslash,
  Key_Comma,
  Key_Slash,
  Key_Period,
  Key_Return,
  Key_Tab,
  Key_Space,
  Key_Backspace,
  Key_Escape,
  Key_Tick,
  Key_Win,
  Key_Shift,
  Key_CapsLock,
  Key_Alt,
  Key_Control,
  Key_RightWin,
  Key_RightShift,
  Key_RightAlt,
  Key_RightControl,
  Key_Function,
  Key_F1,
  Key_F2,
  Key_F3,
  Key_F4,
  Key_F5,
  Key_F6,
  Key_F7,
  Key_F8,
  Key_F9,
  Key_F10,
  Key_F11,
  Key_F12,
  Key_F13,
  Key_F14,
  Key_F15,
  Key_F16,
  Key_F17,
  Key_F18,
  Key_F19,
  Key_F20,
  Key_F21,
  Key_F22,
  Key_F23,
  Key_F24,
  Key_Help,
  Key_Home,
  Key_Insert,
  Key_PageUp,
  Key_ForwardDelete,
  Key_End,
  Key_PageDown,
  Key_LeftArrow,
  Key_RightArrow,
  Key_DownArrow,
  Key_UpArrow,

  Key_KeyboardDiscreteCount,
};
