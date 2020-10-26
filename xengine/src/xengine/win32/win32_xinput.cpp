#pragma once

#ifndef XINPUT_CPP
#define XINPUT_CPP

#define KEY_DOWN(key_code) (GetAsyncKeyState(key_code) < 0)
#define KEY_UP(key_code) (!KEY_DOWN(key_code))

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef XINPUT_GET_STATE(xinput_get_state);
XINPUT_GET_STATE(XInputGetStateFa)
{
    return 0;
}
static xinput_get_state *LXInputGetState = XInputGetStateFa;
#define XInputGetState LXInputGetState

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) 
typedef XINPUT_SET_STATE(xinput_set_state);
XINPUT_SET_STATE(XInputSetStateFa)
{
    return 0;
}
global xinput_set_state *LXInputSetState = XInputSetStateFa;
#define XInputSetState LXInputSetState

enum Button
{
    KEYBOARD_A,
    KEYBOARD_W,
    KEYBOARD_S,
    KEYBOARD_D,
    KEYBOARD_V,
    KEYBOARD_B,

    GAMEPAD_A,
    GAMEPAD_B,
    GAMEPAD_X,
    GAMEPAD_Y
};

enum State
{
    NONE,
    UP,
    PRESS,
    HOLD
};

struct Vec2i
{
    Vec2i() : x(0), y(0) {}
    Vec2i(uint32 x_v, uint32 y_v) : x(x_v), y(y_v) {}

    uint32 x, y;
};

struct MouseState
{
    Vec2i position = Vec2i(0, 0);
    Vec2i dtPosition = Vec2i(0, 0);
    real32 wheel = 0;
    bool32 isLeftButtonPressed = false;
    bool32 isRightButtonPressed = false;
};

struct KeyBoardStates
{
    bool32 buttons[256] = {};
};

std::vector<uint8_t*> loop_mess;

global MouseState currentMouseState = {};
global MouseState prevMouseState = {};
global KeyBoardStates keyboard = {};

global State inputMouseState = {};
global State inputKeyboardState = {};

internal void initInput()
{
    HMODULE xinput_lib = LoadLibrary("xinput1_4.dll");
    if (xinput_lib)
    {
        XInputGetState = (xinput_get_state*)GetProcAddress(xinput_lib, "XInputGetState");
        XInputSetState = (xinput_set_state*)GetProcAddress(xinput_lib, "XInputSetState");
    }

    loop_mess.reserve(64);

    RAWINPUTDEVICE rid[2] = {};

    // Mouse
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x02;
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = 0;

    // Keyboard
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x06;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = 0;

    int size = sizeof(rid) / sizeof(rid[0]);

    if (RegisterRawInputDevices(rid, size, sizeof(rid[0])) == FALSE)
    {
        //error("Failed to init RegisterRawInputDevices!");
    }
}

internal bool32 IsPressed(Button button)
{
    uint8 keycode = (uint8)button;

    switch (button)
    {
    case KEYBOARD_A:
        keycode = 0x41;
        break;
    case KEYBOARD_S:
        keycode = 0x53;
        break;
    case KEYBOARD_W:
        keycode = 0x57;
        break;
    case KEYBOARD_D:
        keycode = 0x44;
        break;
    case KEYBOARD_V:
        keycode = 'V';
        break;
    case KEYBOARD_B:
        keycode = 'B';
        break;
    default:
        break;
    }

    return KEY_DOWN(keycode);
}

internal 
void poll_events()
{
    for (DWORD controller_index = 0;
        controller_index < XUSER_MAX_COUNT;
        ++controller_index)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if (XInputGetState(controller_index, &state) == ERROR_SUCCESS)
        {
            XINPUT_GAMEPAD *gamepad = &state.Gamepad;
            bool32 up = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool32 down = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool32 a = (gamepad->wButtons & XINPUT_GAMEPAD_A);
            bool32 b = (gamepad->wButtons & XINPUT_GAMEPAD_B);
            bool32 x = (gamepad->wButtons & XINPUT_GAMEPAD_X);
            bool32 y = (gamepad->wButtons & XINPUT_GAMEPAD_Y);

            if (a)
            {
                printf("A button pressed\n");
            }

            if (b)
            {
                printf("B button pressed\n");
            }

            if (x)
            {
                printf("X button pressed\n");
            }

            if (y)
            {
                printf("Y button pressed\n");
            }

        }
        else
        {
            // unavailable
        }
    }
}

internal 
void update()
{
    if (prevMouseState.isLeftButtonPressed == false && currentMouseState.isLeftButtonPressed == false)
    {
        inputMouseState = State::UP;
    }

    if (prevMouseState.isLeftButtonPressed && currentMouseState.isLeftButtonPressed)
    {
        inputMouseState = State::HOLD;
    }

    if (prevMouseState.isLeftButtonPressed == false && currentMouseState.isLeftButtonPressed)
    {
        inputKeyboardState = State::PRESS;
    }

    prevMouseState.isLeftButtonPressed = currentMouseState.isLeftButtonPressed;
    prevMouseState.isRightButtonPressed = currentMouseState.isRightButtonPressed;

    switch (inputMouseState)
    {
    case State::PRESS:
    {

    } break;
    case State::UP:
    {
        //currentMouseState.is_left_button_pressed = false;
    } break;

    default:
        break;
    }

}

#endif
