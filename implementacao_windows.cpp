#pragma once
#include "RecursosT.h"

class foncoes_Windows : public funcoes_OS_classe {
private:
public:
	virtual string nome_OS(string s) { return ""; }
	virtual vector<string> pegar_arquivos_em_diretorio(string s) { return {}; }
	virtual vector<string> pegar_diretorios_em_diretorio(string s) { return {}; }
	virtual void criar_diretorio(string s) {}
	virtual void vibrar_controle(vec2 v) {}
};