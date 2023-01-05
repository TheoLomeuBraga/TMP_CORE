#pragma once

#include <iostream>
using namespace std;
#include  <fstream>
#include <sstream>
#include <filesystem>

#include "RecursosT.h"
#include  <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;





namespace converter_JSON {

	json converter_caractere(Caractere_info c) {

		vector<unsigned char> chars;
		chars.resize(c.res.x * c.res.y);

		for (int i = 0; i < c.res.x * c.res.y; i++) {
			chars[i] = c.data[i];
		}
		vector<float> fv = { c.pos_sca.x,c.pos_sca.y,c.pos_sca.z,c.pos_sca.w };
		vector<int> iv = { c.res.x,c.res.y };
		json JSON = {
			{"letra",c.letra},
			{"res",iv},
			{"pos_sca",fv},
			{"avancamento",c.avancamento},
			{"data",chars},


		};


		return JSON;
	}

	string converter_fonte(fonte* f) {
		json JSON;

		JSON["pixel_perfeito"] = (float)f->pixel_perfeito;


		vector<json> caractere_js;
		for (int i = 0; i < NUM_CARACTERES; i++) {
			caractere_js.push_back(converter_caractere(f->Characters[i]));
		}


		JSON["Characters"] = caractere_js;

		JSON["qualidade"] = f->qualidade;

		return JSON.dump();
	}

	string converter_TMP_L2D(TMP_L2D l) {
		json JSON;

		JSON["nome"] = l.nome;
		JSON["X"] = l.X; JSON["Y"] = l.Y;

		JSON["tema"] = l.tema;
		JSON["musica"] = l.musica;

		JSON["level"] = l.level;
		JSON["fundo"] = l.fundo;

		


		return JSON.dump();
	}





};






namespace desconverter_JSON {

	Caractere_info desconverter_caractere(json JSON) {
		Caractere_info ret;

		ret.letra = JSON["char"].get<char>();
		

		vector<float> vi = JSON["resolution"].get<vector<float>>();
		ret.res.x = vi[0];
		ret.res.y = vi[1];
		
		vector<float> vf = JSON["position_scale"].get<vector<float>>();
		ret.pos_sca.x = vf[0];
		ret.pos_sca.y = vf[1];
		ret.pos_sca.z = vf[2];
		ret.pos_sca.w = vf[3];
		
		ret.avancamento = JSON["adivancement"].get<int>();
		
		ret.data = JSON["data"].get<vector<unsigned char>>();


		return ret;
	}


	fonte desconverter_fonte(string J) {
		fonte ret;

		json JSON = json::parse(J);

		ret.qualidade = JSON["quality"].get<int>();
		ret.pixel_perfeito = (bool)JSON["pixel_perfect"].get<int>();
		if (JSON.find("adivancement") != JSON.end()) {
			ret.avancamento = JSON["adivancement"].get<float>();
		}
		vector<json> carac = JSON["Chars"].get<vector<json>>();
		for (int i = 0; i < NUM_CARACTERES; i++) {
			ret.Characters[i] = desconverter_caractere(carac[i]);
		}
		return ret;
	}









};