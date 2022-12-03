#pragma once
#include <iostream>
#include "Config.h"
#include "RecursosT.h"
#include "Objetos.h"
#include "box_2d.h"

class projetil : public componente {
private:
	shared_ptr<Objetos::transform> tf = NULL;
	shared_ptr<box_2D> b2D = NULL;
public:
	projetil() {}
	vec3 velocidade = vec3(0,0,0);
	float dano = 0;

	
	vector<vec3> alvos_atracao = {};
	float distancia_atracao = 0,forca_atracao = 0;
	shared_ptr<Objetos::transform> alvo = NULL;



	void atualisar() {
		if (tf == NULL) { tf = esse_objeto->pegar_componente<Objetos::transform>(); }
		if (b2D == NULL) { b2D = esse_objeto->pegar_componente<box_2D>(); }

		if (tf != NULL) {
			if (b2D != NULL) {
				if (alvos_atracao.size() > 0) {
					
				}
				b2D->adicionar_forca(vec2(velocidade.x, velocidade.y));
			}
		}
	}
};