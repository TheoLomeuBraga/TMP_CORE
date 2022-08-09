#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;


#include "Config.h"
#include "Tempo.h"
#include "vibrar_controle.cpp"
#include "RecursosT.h"

class input_manager {
public:
	input_manager() {}
	virtual void loop() {}
};

input_mouse Mouse;
teclado Teclado;
bool conectado;
joystick Joystick[8];




input_manager* manuseio_inputs;

void loop_input() {
	
	while (manuseio_inputs == NULL) { escrever("esperando input_manager"); }
	escrever("iniciando input_manager");
	manuseio_inputs->loop();
}

#define MOUSE Mouse
#define TECLADO Teclado
#define JOYSTICK Joystick





