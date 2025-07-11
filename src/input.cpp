

void AllocateInputManager(InputManager *input, MemoryArena *arena, int32 capacity, int32 deviceCapacity) {
  input->events = MakeDynamicArray<InputEvent>(arena, capacity);

  input->deviceCount = deviceCapacity;
  input->devices = (InputDevice *)malloc(sizeof(InputDevice) * deviceCapacity);

  memset(input->devices, 0, input->deviceCount * sizeof(InputDevice));

  input->charSize = 32;
  input->charCount = 0;
  input->inputChars = (char *)malloc(input->charSize);
}

void AllocateInputDevice(InputDevice *device, InputDeviceType type, int32 discreteCount, int32 analogueCount) {
  device->type = type;
  device->discreteCount = discreteCount;
    
  device->framesHeld = (int32 *)malloc(sizeof(int32) * discreteCount);
  device->released = (bool *)malloc(sizeof(bool) * discreteCount);
  device->pressed = (bool *)malloc(sizeof(bool) * discreteCount);
  device->timePressed = (real32 *)malloc(sizeof(real32) * discreteCount);

  device->analogueCount = analogueCount;
  device->prevAnalogue = (real32 *)malloc(sizeof(real32) * analogueCount);
  device->analogue = (real32 *)malloc(sizeof(real32) * analogueCount);

  memset(device->framesHeld, -1, sizeof(int32) * discreteCount);
  memset(device->pressed, 0, sizeof(bool) * discreteCount);
  memset(device->released, 0, sizeof(bool) * discreteCount);
  memset(device->timePressed, 0, sizeof(real32) * discreteCount);

  memset(device->analogue, 0, sizeof(real32) * analogueCount);
  memset(device->prevAnalogue, 0, sizeof(real32) * analogueCount);
}

// @TODO: problem with doing the Raylib GetKeyPressed thing is we dont
// have a GetKeyReleased so maybe what we want to do is just make a
// mapping from our key to raylib's, and just use that to wrap the function?

void InputManagerUpdate() {
  //int key = GetKeyPressed(void);                      
}
