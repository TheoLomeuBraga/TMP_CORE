#pragma once
#include <iostream>
using namespace std;
#include "RecursosT.h"
#include "ManusearArquivos.h"


namespace Gerenciador_Cena {
	class Cena
	{
	public:

		Cena() {}
		string nome;
		

		vector < malha*> malhas;
		vector <imagem*> imagens;

		vector<string> malhas_para_carregar;
		vector<string> imagens_para_carregar;
		
		



		void importar(const char* local) {



		}



	};






























}


