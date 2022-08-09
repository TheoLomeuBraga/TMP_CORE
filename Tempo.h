#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "Config.h"







namespace Tempo {

	float tempo;
	float velocidadeTempo = 1;



	int FPS;

	float tempUltFrameEntidades, varTempEntidades;
	float tempUltFrameRender, varTempRender;
	float tempUltFrame,  varInputTemp;
	




	
	bool depurar_FPS = true;


	chrono::steady_clock::time_point inicio;
	chrono::steady_clock::time_point atual;






	void ContinuarTempo() {
		atual = chrono::steady_clock::now();
		chrono::duration<float> t = atual - inicio;
		tempo = t.count();
		FPS = 1 / varTempRender;



		



	}



	void IniciarTempo() {
		cout << "Relogio Interno Online" << endl;

		inicio = chrono::steady_clock::now();




		while (true)
		{
			if (configuracoes::desligar) { break; }
			ContinuarTempo();
		}


	}

}
