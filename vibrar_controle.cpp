#pragma once

#include <glm/glm.hpp>



#ifdef WINDOWS

#include <iostream>
#include <windows.h>
#include <Xinput.h>


#pragma comment(user, "Compiled on " __DATE__ " at " __TIME__)
#pragma comment(lib, "XInput.lib")

using namespace std;

class Controller {
private:
    XINPUT_STATE _controllerState;
    int _controllerNum;
public:
    Controller(int num);
    ~Controller();
    Controller() {
        Controller(1);
    }
    XINPUT_STATE getState();
    bool isConnected();
    void vibrate(int left = 0, int right = 0);
};

Controller::Controller(int num) {
    _controllerNum = num - 1;
}

Controller::~Controller() {

}

XINPUT_STATE Controller::getState() {
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}

bool Controller::isConnected() {
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    return Result == ERROR_SUCCESS;
}

void Controller::vibrate(int left, int right) {
    XINPUT_VIBRATION v;

    ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

    v.wLeftMotorSpeed = left;
    v.wRightMotorSpeed = right;

    XInputSetState(_controllerNum, &v);
}

bool iniciado_vibrar_controle_windows;
Controller controles[7];

void iniciar_vibrar_controle_windows() {
    for (int i = 0; i < 7; i++) {
        controles[i] = Controller(i);
        

    }


}

void vibrar_controle_widows(int controle, vec2 forca) {
    if (!iniciado_vibrar_controle_windows) {
        iniciar_vibrar_controle_windows();
    }
    
    controles[controle].vibrate(abs(forca.x) * 65535, abs(forca.y) *65535);
    
}









#endif 



void vibrar_controle(int controle, vec2 forca) {
#ifdef WINDOWS
    vibrar_controle_widows(controle, forca);
#endif 

    

}





