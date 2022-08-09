#pragma once

using namespace std;
#include <map>
#include <vector>
#include <string>

class resializador_binario {
	map<string, int> mapeamento;
	string local;
public:
	resializador_binario(string local) { this->local = local; }
	resializador_binario(string local,vector<string> mapeamento) {
		this->local = local;

		for (int i = 0; i < mapeamento.size(); i++) {
			this->mapeamento.insert(pair<string, int>(mapeamento[i],i));
		}
	}












};


void testar_resializador_binario() {


}

