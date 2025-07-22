

void AllocateInputManager(InputManager *input, MemoryArena *arena, int32 capacity, int32 deviceCapacity) {
  input->events = MakeDynamicArray<InputEvent>(arena, capacity);

  input->deviceCount = deviceCapacity;
  input->devices = (InputDevice *)malloc(sizeof(InputDevice) * deviceCapacity);

  memset(input->devices, 0, input->deviceCount * sizeof(InputDevice));

  input->charSize = 32;
  input->charCount = 0;
  input->inputChars = (char *)malloc(input->charSize);

  KeyTypeMap = (int32 *)malloc(sizeof(int32) * (KEY_KP_EQUAL + 1));
  KeyTypeMapLength = KEY_KP_EQUAL + 1;
  
  PopulateKeyTypeMap();
}

void AllocateInputDevice(InputDevice *device, InputDeviceType type, int32 discreteCount, int32 analogueCount, int32 index) {
  device->type = type;
  device->discreteCount = discreteCount;
  device->index = index;
    
  device->framesHeld = (int32 *)malloc(sizeof(int32) * discreteCount);
  device->released = (bool *)malloc(sizeof(bool) * discreteCount);
  device->pressed = (bool *)malloc(sizeof(bool) * discreteCount);
  device->timePressed = (float32 *)malloc(sizeof(float32) * discreteCount);

  device->analogueCount = analogueCount;
  device->prevAnalogue = (float32 *)malloc(sizeof(float32) * analogueCount);
  device->analogue = (float32 *)malloc(sizeof(float32) * analogueCount);

  memset(device->framesHeld, -1, sizeof(int32) * discreteCount);
  memset(device->pressed, 0, sizeof(bool) * discreteCount);
  memset(device->released, 0, sizeof(bool) * discreteCount);
  memset(device->timePressed, 0, sizeof(float32) * discreteCount);

  memset(device->analogue, 0, sizeof(float32) * analogueCount);
  memset(device->prevAnalogue, 0, sizeof(float32) * analogueCount);
}

void PushInputEvent(InputManager *input, InputEvent e) {
  PushBack(&input->events, e);
}

bool InputPressed(InputDevice *device, int32 inputID) {
  return device->pressed[inputID] && device->framesHeld[inputID] == 0;
}

bool InputReleased(InputDevice *device, int32 inputID) {
  return device->released[inputID];
}

bool InputHeld(InputDevice *device, int32 inputID) {
  return device->framesHeld[inputID] > 0;
}

bool InputHeldSeconds(InputDevice *device, int32 inputID, float32 time) {
  bool held = device->framesHeld[inputID] > 0;

  return held && (Time - device->timePressed[inputID] >= time);
}

float32 InputAnalogue(InputDevice *device, int32 inputID) {
  return device->analogue[inputID];
}


void RaylibPushKeyboardEvents(InputManager *input, InputDevice *device) {
  int32 key = GetKeyPressed();

  // @TODO: I think we need to just iterate over all the keys on Raylib...

  for (int key = 0; key < KEY_KP_EQUAL; key++) {
    InputEvent event = {};
    event.device = device;

    if (key >= KeyTypeMapLength) {
      continue;
    }
    
    event.index = KeyTypeMap[key];

    if (IsKeyPressed(key) ||
        IsKeyDown(key)) {
      event.discreteValue = true;
    }
    else if (IsKeyReleased(key)) {

    }
    else {
      continue;
    }

    //Print("raylib key %d mapped to %d / %d", key, event.index, device->discreteCount);

    PushInputEvent(input, event);
  }
}

void RaylibPushMouseEvents(InputManager *input, InputDevice *device) {
  int32 key = GetKeyPressed();

  // @TODO: I think we need to just iterate over all the keys on Raylib...

  for (int button = 0; button <= MOUSE_BUTTON_BACK; button++) {
    InputEvent event = {};
    event.device = device;

    if (button >= ArrayLength(uint32, MouseButtonMap)) {
      continue;
    }
    
    event.index = MouseButtonMap[button];

    if (IsMouseButtonPressed(button) ||
        IsMouseButtonDown(button)) {
      event.discreteValue = true;
    }
    else if (IsMouseButtonReleased(button)) {

    }
    else {
      continue;
    }

    //Print("raylib button %d mapped to %d / %d", button, event.index, device->discreteCount);

    PushInputEvent(input, event);
  }
}


void RaylibPushGamepadEvents(InputManager *input, InputDevice *device) {
  int32 gamepadIndex = device->index;
  
  for (int button = 0; button <= GAMEPAD_BUTTON_RIGHT_THUMB; button++) {
    InputEvent event = {};
    event.device = device;

    if (button >= ArrayLength(uint32, GamepadButtonMap)) {
      continue;
    }
    
    event.index = GamepadButtonMap[button];

    if (IsGamepadButtonPressed(gamepadIndex, button) ||
        IsGamepadButtonDown(gamepadIndex, button)) {
      event.discreteValue = true;
    }
    else if (IsGamepadButtonReleased(gamepadIndex, button)) {

    }
    else {
      continue;
    }

    PushInputEvent(input, event);
  }

  float32 deadzone = 0.1f;
  
  for (int axis = 0; axis <= GAMEPAD_AXIS_RIGHT_TRIGGER; axis++) {
    InputEvent event = {};
    event.device = device;

    if (axis >= ArrayLength(uint32, GamepadAnalogueMap)) {
      continue;
    }

    event.index = GamepadAnalogueMap[axis];

    event.value = GetGamepadAxisMovement(gamepadIndex, axis);
    
    if (event.index == Input_RightTrigger ||
        event.index == Input_LeftTrigger) {
      event.value = (event.value + 1.0f) / 2;
    }
    
    if (Abs(event.value) < deadzone) { continue; }

    PushInputEvent(input, event);
  }
}

void InputManagerUpdate(InputManager *input) {

  for (int i = 0; i < input->deviceCount; i++) {
    InputDevice *device = &input->devices[i];

    // clear the device
    for (int i = 0; i < device->discreteCount; i++) {
      device->released[i] = false;
            
      // @NOTE: until we get a release event we consider a key to be pressed
      if (device->framesHeld[i] >= 0) {
        device->framesHeld[i]++;
        device->pressed[i] = false;
        device->released[i] = false;
      }
    }

    // @TODO: we dont want to do this for the mouse.
    for (int i = 0; i < device->analogueCount; i++) {
      device->analogue[i] = 0;
    }

    for (int i = 0; i < input->events.count; i++) {
      InputEvent event = input->events[i];

      if (event.device != device) { continue; }
      
      int32 index = event.index;
        
      //Print("event %d frames %d\n", i, device->framesHeld[index]);

      InputDevice *device = event.device;

      //Print("device %x", device);

      if (!event.discreteValue) {
        if (device->framesHeld[index] > 0) {
          device->released[index] = true;
        }
            
        device->timePressed[index] = -1;
        device->framesHeld[index] = -1;
        device->pressed[index] = false;

        device->analogue[index] = event.value;
      }
      else {
        if (device->framesHeld[index] < 0) {
          //Print("pressed\n");

          device->timePressed[index] = Time;
          device->framesHeld[index] = 0;
          device->pressed[index] = true;
          device->released[index] = false;
        }
        else {
          // @NOTE: we shouldnt even get a pressed event when the key is being held
        }
      }
    }

    // switch (device->type) {
    //   default : {}; break;

    //   case InputDeviceType_Keyboard : {
        
    //   } break;
    // }
  }
}
