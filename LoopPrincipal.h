#pragma once

#include <iostream>
#include<algorithm>
#include <functional>
#include <thread>
using namespace std;

//imgui
/*
#include "imgui.cpp"
#include "imgui_draw.cpp"
#include "imgui_tables.cpp"
#include "imgui_widgets.cpp"
#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_glfw.cpp"
*/




#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
using namespace glm;

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "Config.h"
#include "input.h"
#include "Tempo.h"
#include "Objetos.h"
using namespace Objetos;





//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

vector<function<void()>> Iniciar_Render_Func;
vector<function<void()>> Antes_Render_Func;
vector<function<void()>> Depois_Render_Func;
vector<function<void()>> atualisar_fisica;

class gerenciador_janela {
public:
	bool fechar = false;
	gerenciador_janela() {}
	virtual bool esta_minimizado() { return false; }
	virtual void mudar_cursor(shared_ptr<imagem> cursor) {}
	virtual void mudar_imagem_janela(shared_ptr<imagem> janela) {}
	virtual void mudar_pos_cursor(float pos_x, float pos_y) {}
	virtual void mudar_res(float res_x, float res_y) {}
	virtual vec2 pegar_res() { return vec2(0,0); }
	virtual bool e_janela_cheia() { return false; }
	virtual void iniciar() {}
	virtual void loop() {}
	virtual void setar_tela_inteira_como(bool tela_cheia) {}
	virtual void encerrar() {}
};
gerenciador_janela* gerente_janela;
namespace loop_principal {








	






	//variaveis
	 //#define OPENGL 0

	



	




	shared_ptr<imagem> iconeImagem;

	

	
	

	

	void mudar_cursor(shared_ptr<imagem> cursor) {
		if (gerente_janela != NULL) {
			gerente_janela->mudar_cursor(cursor);
		}
	}
	void mudar_imagem_janela(shared_ptr<imagem> janela) {
		if (gerente_janela != NULL) {
			gerente_janela->mudar_imagem_janela(janela);
		}
	}
	void mudar_pos_cursor(float pos_x, float pos_y) {
		if (gerente_janela != NULL) {
			gerente_janela->mudar_pos_cursor(pos_x, pos_y);
		}
	}
	void mudar_res(float res_x, float res_y) {
		if (gerente_janela != NULL) {
			gerente_janela->mudar_res(res_x, res_y);
		}
	}
	void iniciar() {
		while (gerente_janela == NULL)
		{
			escrever("esperando gerente_janela");
		}
		escrever("iniciando loop janela");
		gerente_janela->iniciar();
	}
	void loop() {
		while (gerente_janela == NULL)
		{
			escrever("esperando gerente_janela");
		}
		gerente_janela->loop();
	}
	vec2 pegar_res() {
		vec2 ret = vec2(0,0);
		if (gerente_janela != NULL) {
			ret = gerente_janela->pegar_res();
		}
		return ret;
	}
	void encerrar() {
		if (gerente_janela != NULL) {
			gerente_janela->encerrar();
		}
	}

	void setar_tela_inteira_como(bool tela_cheia) {
		if (gerente_janela != NULL) {
			gerente_janela->setar_tela_inteira_como(tela_cheia);
		}
	}


	void loop_principal() {
		iniciar();
		do {
			loop();
		} while (gerente_janela->fechar == false);
		

		
		
	}







	
	







}

