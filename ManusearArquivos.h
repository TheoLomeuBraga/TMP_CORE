#pragma once
#include <iostream>
using namespace std;
#include  <fstream>
#include <sstream>
#include <thread>




#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Converter_JSON.h"
#include "OBJ_importer.h"
#include "Objetos.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb-master/stb_image_write.h>



bool CARREGANDO;


namespace ManuseioDados {



	string diretorio;

	




	
	





	bool Existe(const char* nome) {
		ifstream file(nome);
		if (!file) {
			return false;
		}
		else { return true; }

	}

	bool Existe(string nome) {
		return Existe(nome.c_str());

	}

	void Salvar(const char* nome, const char* conteudo) {

		ofstream file(nome);

		file << conteudo;

		file.close();


	}


	


	//template<typename X>
	string Carregar_string(string nome) {
		string s, s2;
		ifstream file(nome);
		const char* cc;

		if (file.is_open()) {
			while (getline(file, s))
			{
				s2 += s + "\n";
			}
		}
		else
		{
			s2 = "erro";
		}

		return s2;
	}
	




	template<typename X>
	const char* Carregar(X nome) {
		string s2 = Carregar_string(nome);

		return s2.c_str();

		 
	}


	mapeamento_assets<fonte> mapeamento_fontes;

	shared_ptr<fonte> carregar_fonte(string lugar) {
		ifstream file(lugar);

		if (file) {
			
			if (mapeamento_fontes.pegar(lugar).get() == NULL) {
				
				string S, l;

				file >> S;

				fonte F = desconverter_JSON::desconverter_fonte(S);
				
				F.local = lugar;

				mapeamento_fontes.aplicar(lugar, F);



				return mapeamento_fontes.pegar(lugar);


			}
			else
			{
				return mapeamento_fontes.pegar(lugar);
			}

		}
		else {
			shared_ptr<fonte> ret;
			return ret;
		}



	}


	void carregar_fonte_thread(string local, shared_ptr<fonte>* ret) {
		*ret = carregar_fonte(local);
	}





	mapeamento_assets<imagem> mapeamento_imagems;

	shared_ptr<imagem> carregar_Imagem(string local) {
		bool jaCarregada = false;

		int X = 1, Y = 1, canais = 4;

		unsigned char* data;



		if (Existe(local)) {
			if (mapeamento_imagems.pegar(local).get() == NULL) {
				data = stbi_load(local.c_str(), &X, &Y, &canais, canais);

				imagem image = imagem(X, Y, canais, data);
				image.local = local;
				//delete[] data;

				mapeamento_imagems.aplicar(local, image);

				escrever(local);
				escrever(mapeamento_imagems.pegar(local));


				return mapeamento_imagems.pegar(local);
			}
			else
			{
				return mapeamento_imagems.pegar(local);
			}
		}
		else
		{
			shared_ptr<imagem> ret;
			return ret;
		}
	}

	

	

	void carregar_Imagem_thread(string local, shared_ptr<imagem>* ret) {
		*ret = carregar_Imagem(local);
	}

	void salvar_imagem(unsigned char* data,ivec2 res,int canais, string local) {
		stbi_write_png((local + ".png").c_str(), res.x, res.y, canais, (const void*)data, res.x * canais * sizeof(char));

	}

	void salvar_imagem(shared_ptr<imagem> img, string local) {
		salvar_imagem(&img->data[0],ivec2(img->res.x, img->res.y), img->canais,local);
	}

	void imagem_teste() {

		uint8_t* data = new unsigned char[4 *2 * 2];
		int i = 0;
		data[i++] = (uint8_t)255 ;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255 ;

		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;

		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)0;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;

		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		data[i++] = (uint8_t)255;
		stbi_write_png("teste.png", 2,2,4, data, 2 * 4);
	}



	mapeamento_assets< tile_set> mapeamento_tilesets;
	//remover pasta_imagems
	shared_ptr<tile_set> carregar_tile_set(string local) {
		string pasta_imagems = pegar_pasta_arquivo(local);
		if (Existe(local.c_str())) {
			if (mapeamento_tilesets.pegar(local).get() == NULL) {
				cout << "criando tile_set" << endl;
				//pegar info tile_set
				tile_set ret = tile_set();
				ret.local = local;
				ifstream arquivo(local);
				json j;
				arquivo >> j;



				

				if (j.find("image") != j.end()) {
					ret.local_imagem = pasta_imagems + '/' + j["image"].get<string>();
					ret.tiles_img = carregar_Imagem(ret.local_imagem);
				}
				else {
					cout << "tile_set " << local << "n�o posui imagem" << endl;
				}

				if (j.find("tilewidth") != j.end() && j.find("tileheight") != j.end()) {
					ret.res_tile = ivec2(j["tilewidth"].get<int>(), j["tileheight"].get<int>());
					if (ret.tiles_img != NULL) {
						ret.quant_tiles = ivec2(ret.tiles_img->res.x / ret.res_tile.x, ret.tiles_img->res.y / ret.res_tile.y);
					}
				}
				
				//pegar info tiles
				if (j.find("tiles") != j.end()) {
					vector<json> tiles_infos = j["tiles"].get<vector<json>>();
					ret.tiles.resize(tiles_infos.size());
					



					for (int a = 0; a < tiles_infos.size(); a++) {
						
						tile t;
						//t.sprite_id = a;
						t.sprite_id = tiles_infos[a]["id"];
						
						
						//pegar propriedade tile
						vector<json> propriedades_json = tiles_infos[a]["properties"].get<vector<json>>();
						for (int b = 0; b < propriedades_json.size(); b++) {
							string name = propriedades_json[b]["name"].get<string>();

							if (name.compare("name") == 0) {
								t.nome = propriedades_json[b]["value"].get<string>();
							}
							else if (name.compare("colision_type") == 0) {
								t.tipo_colisao = propriedades_json[b]["value"].get<int>();
							}
							else if (name.compare("visible") == 0) {
								t.visivel = propriedades_json[b]["value"].get<int>();
							}
							else if (name.compare("interactive") == 0) {
								t.interativel = propriedades_json[b]["value"].get<int>();
							}
							
						}
						
						ret.tiles[a] = t;
					}
				}
				else { cout << "nao tem tiles\n"; }


				mapeamento_tilesets.aplicar(local, ret);

				
				
				
				return mapeamento_tilesets.pegar(local);
			}
			else {
				
				return mapeamento_tilesets.pegar(local);
			}
		}
		else {
			cout << "nao foi possivel carregar " << local << endl;
			shared_ptr<tile_set> ret;
			return ret;
		}
	}

	void carregar_tile_set_thread(string local, shared_ptr<tile_set>* ret) {
		*ret = carregar_tile_set( local);
	}





	mapeamento_assets<tile_map_info> mapeamento_tile_map_infos;

	shared_ptr<tile_map_info> carregar_info_tile_map(string local) {
		if (Existe(local.c_str())) {

			if (mapeamento_tile_map_infos.pegar(local).get() == NULL) {

				tile_map_info ret = tile_map_info();
				ret.local = local;
				ifstream arquivo(local);
				json j;
				arquivo >> j;

				
				ret.res = vec2(j["width"].get<int>(), j["height"].get<int>());

				//carregar camadas
				vector<json> camadas = j["layers"].get<vector<json>>();
				ret.info.resize(camadas.size());
				

				
				for (int i = 0; i < camadas.size(); i++) {
					if (camadas[i]["type"].get<string>().compare("tilelayer") == 0) {
						ret.info[i] = camadas[i]["data"].get<vector<int>>();
					}
					else {
						ret.info[i].resize(ret.res.x * ret.res.y);
						for (int a = 0; a < ret.res.x * ret.res.y; a++) {
							ret.info[i][a] = 0;
						}
					}
					
				}
				

				
				mapeamento_tile_map_infos.aplicar(local, ret);
				//cout << "quantidade camadas: " << ret->info.size() << endl;
				return mapeamento_tile_map_infos.pegar(local);
			}
			else {
				shared_ptr<tile_map_info>  ret = mapeamento_tile_map_infos.pegar(local);
				return ret;
			}
		}
		else {
			shared_ptr<tile_map_info>  ret;
			return ret;
		}
	}

	void carregar_info_tile_map_thread(string local, shared_ptr<tile_map_info>* ret) {
		*ret = carregar_info_tile_map(local);
	}



	//https://github.com/Biendeo/Doom-WAD-Reader
	//https://github.com/id-Software/DOOM/tree/master/linuxdoom-1.10


	













	mapeamento_assets<cena_3D> cenas_3D;

	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	vec3 decode_obj_f(string s){
		vec3 ret;
		string valores[3];
		int i =0;
		for(char c : s){
			if(c != '/'){
				valores[i].push_back(c);
			}else{
				i++;
			}
		}
		ret = vec3(stof(valores[0]),stof(valores[1]),stof(valores[2]));
		return ret;
	}

struct Obj_mesh_index_struct
{
	
	vector<unsigned int> v_index, vt_index, vn_index;
}; typedef struct Obj_mesh_index_struct Obj_mesh_index;

shared_ptr<cena_3D> importar_obj(string local) {
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
	string pasta = pegar_pasta_arquivo(local);
	cena_3D ret;
	ret.caminho = local;
	escrever(local);

	
	Material mat;

	ifstream arquivo_obj(local);
	string linha;



	

	string instrucao_selecionada;
	map<string, Obj_mesh_index> instrucoes;
	vector<vec3> v;
	vector<vec2> vt;
	vector<vec3> vn;

	bool s;
	string usemtl;
	vector<unsigned int> f;

	
	while (getline(arquivo_obj, linha)) {

		stringstream ss;
		ss << linha;

		vector<string> valor_linha;
		valor_linha.resize(4);
		ss >> valor_linha[0] >> valor_linha[1] >> valor_linha[2] >> valor_linha[3];
		if (!valor_linha[0].compare("o")) {
			Obj_mesh_index om;
			instrucoes.insert(pair<string, Obj_mesh_index>(valor_linha[1], om));
			instrucao_selecionada = valor_linha[1];
			
		}
		else if (!valor_linha[0].compare("v")) {
			vec3 v3;
			v3.x = stof(valor_linha[1]);
			v3.y = stof(valor_linha[2]);
			v3.z = stof(valor_linha[3]);
			v.push_back(v3);
		}
		else if (!valor_linha[0].compare("vt")) {
			vec2 v2;
			v2.x = stof(valor_linha[1]);
			v2.y = stof(valor_linha[2]);
			vt.push_back(v2);
		}
		else if (!valor_linha[0].compare("vn")) {
			vec3 v3;
			v3.x = stof(valor_linha[1]);
			v3.y = stof(valor_linha[2]);
			v3.z = stof(valor_linha[3]);
			vn.push_back(v3);
		}
		else if (!valor_linha[0].compare("usemtl")) {
			usemtl = valor_linha[1];
		}
		else if (!valor_linha[0].compare("s")) {
			map<string, bool> m = { pair<string,bool>("on",true),pair<string,bool>("off",false) };
			s = m[valor_linha[1]];
		}
		else if (!valor_linha[0].compare("f")) {

			vec3 a = decode_obj_f(valor_linha[1]);
			vec3 b = decode_obj_f(valor_linha[2]);
			vec3 c = decode_obj_f(valor_linha[3]);

			instrucoes[instrucao_selecionada].v_index.push_back(a.x - 1);
			instrucoes[instrucao_selecionada].v_index.push_back(b.x - 1);
			instrucoes[instrucao_selecionada].v_index.push_back(c.x - 1);

			instrucoes[instrucao_selecionada].vt_index.push_back(a.y - 1);
			instrucoes[instrucao_selecionada].vt_index.push_back(b.y - 1);
			instrucoes[instrucao_selecionada].vt_index.push_back(c.y - 1);

			instrucoes[instrucao_selecionada].vn_index.push_back(a.z - 1);
			instrucoes[instrucao_selecionada].vn_index.push_back(b.z - 1);
			instrucoes[instrucao_selecionada].vn_index.push_back(c.z - 1);



		}
	}

	
	for (pair<string, Obj_mesh_index> p : instrucoes) {
		
		pair<string, shared_ptr<malha>> m = pair<string, shared_ptr<malha>>(p.first, make_shared<malha>());
		//transformar em mesh
		
		m.second->vertices.resize(p.second.v_index.size());
		m.second->indice.resize(p.second.v_index.size());
		
		
		m.second->nome = p.first;
		

		for (int i = 0; i < p.second.v_index.size(); i++) {
			
			m.second->indice[i] = i;

			
			m.second->vertices[i].posicao[0] = v[p.second.v_index[i]].x;
			m.second->vertices[i].posicao[1] = v[p.second.v_index[i]].y;
			m.second->vertices[i].posicao[2] = v[p.second.v_index[i]].z;
			
			

			m.second->vertices[i].uv[0] = vt[p.second.vt_index[i]].x;
			m.second->vertices[i].uv[1] = vt[p.second.vt_index[i]].y;

			

			m.second->vertices[i].normal[0] = vn[p.second.vn_index[i]].x;
			m.second->vertices[i].normal[1] = vn[p.second.vn_index[i]].y;
			m.second->vertices[i].normal[2] = vn[p.second.vn_index[i]].z;

			



		}
		

		//adicionar malha
		m.second->arquivo_origem = local;
		m.second->comprimir();
		m.second->pegar_tamanho_maximo();
		ret.malhas.insert(m);
		ret.objetos.minhas_malhas.push_back(m.first);
	}

	
	//adicionar material
	ret.materiais.insert(pair<string, Material>("material", mat));
	ret.objetos.meus_materiais.push_back("material");

	cenas_3D.aplicar(local, ret);
	return cenas_3D.pegar(local);
}

	void importar_obj_thread(string local,string local_mtl, shared_ptr<cena_3D>* ret){
		*ret = importar_obj(local);
	}

	shared_ptr<cena_3D> importar_map(string local){
		cena_3D ret;
		ret.caminho = local;

		//https://github.com/stefanha/map-files

		cenas_3D.aplicar(local,ret);
		return cenas_3D.pegar(local);
	}
	void importar_map_thread(string local, shared_ptr<cena_3D>* ret){
		*ret = importar_map(local);
	}
	
	shared_ptr<cena_3D> importar_gltf(string local){
		cena_3D ret;
		ret.caminho = local;

		cenas_3D.aplicar(local,ret);
		return cenas_3D.pegar(local);
	}
	void importar_gltf_thread(string local, shared_ptr<cena_3D>* ret){
			*ret = importar_gltf(local);
	}

	//arquivo_origem
	map<string, shared_ptr<cena_3D>(*)(string)> funcoes_abrir_modelos_3D = {
			pair<string,shared_ptr<cena_3D>(*)(string)>(".obj",importar_obj),
			pair<string,shared_ptr<cena_3D>(*)(string)>(".map",importar_map),
			pair<string,shared_ptr<cena_3D>(*)(string)>(".gltf",importar_gltf),
	};
	shared_ptr<cena_3D> carregar_modelo_3D(string local){
		
		return funcoes_abrir_modelos_3D[pegar_estencao_arquivo(local)](local);
	}
	void carregar_modelo_3D(string local, shared_ptr<cena_3D>* ret){
		*ret = carregar_modelo_3D(local);
	}

	shared_ptr<malha> carregar_malha(string local,string nome){
		return carregar_modelo_3D(local)->malhas[nome];
	}
	void carregar_malha_thread(string local,string nome,shared_ptr<malha>* ret){
		*ret =  carregar_malha(local,nome);
	}

}




