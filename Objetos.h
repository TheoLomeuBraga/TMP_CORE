#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"








namespace Objetos {

	class objeto_jogo;

	class componente {
	public:
		shared_ptr<objeto_jogo> esse_objeto = NULL;
		//bool iniciado = false;
		componente() {}
		virtual void iniciar() {}
		virtual void atualisar() {}
		virtual void colidir(colis_info col) {}
		virtual void finalisar() {}




	};
	

	mat4 MatrizMundi = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	class transform : public componente {
	public:






		transform() {
			matrizTransform = MatrizMundi;
		}

		
		bool UI = false;
		transform* paiTF;
		


		

		int local_hierarquia;

		bool usar_pai_matriz;
		mat4 pai_matriz;


		glm::vec3 pos, esca;

		glm::mat4 matrizTransform;
		quat quater;




		


		glm::mat4 pegar_matriz() {
			glm::mat4 ret;

			if (paiTF == NULL) {
				ret = MatrizMundi;
			}
			else
			{
				ret = paiTF->matrizTransform;
			}



			ret = translate(ret, pos);

			ret *= toMat4(quater);
			ret = scale(ret, esca);

			matrizTransform = ret;




			return ret;
		}

		glm::vec3 pegar_pos_global() {
			vec3 nada;
			vec4 nada2;
			vec3 pos;
			quat nada3;
			glm::decompose(matrizTransform, nada, nada3, pos, nada, nada2);
			return pos;
		}
		glm::quat pegar_qua_global() {
			vec3 nada;
			vec4 nada2;
			vec3 nada3;
			quat qua;
			glm::decompose(matrizTransform, nada, qua, nada3, nada, nada2);
			return qua;
		}
		glm::vec3 pegar_graus_global() {
			return quat_graus(pegar_qua_global());
		}

		void matriz_transform(glm::mat4 mat) {
			vec3 nada;
			vec4 nada2;

			vec3 escala;
			vec3 pos;
			quat qua;
			glm::decompose(paiTF->matrizTransform, escala, qua, pos, nada, nada2);



		}

		vec3 pegar_direcao_local(vec3 dir) {
			mat4 m = glm::translate(matrizTransform,dir);
			vec3 nada;
			vec4 nada2;
			vec3 escala;
			vec3 pos;
			quat qua;
			glm::decompose(m, escala, qua, pos, nada, nada2);

			return pos - pegar_pos_global() ;
		}

		void reset(bool ui, transform* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			UI = ui;
			paiTF = pai;
			pos = p;
			quater = graus_quat(r);
			esca = e;
			pegar_matriz();
		}

		void atualizar_tf() {
			//OBS: o unico proposito dessa fun��o e eu n�o me confundir 
			pegar_matriz();
		}


		transform(glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(false, NULL, p, r, e);
		}




		transform(bool ui, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(ui, NULL, p, r, e);
		}





		transform(transform* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(false, pai, p, r, e);
		}

		transform(bool ui, transform* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(ui, pai, p, r, e);
		}

		vec3 pegar_angulo_graus() {
			return quat_graus(quater);

		}
		void mudar_angulo_graus(vec3 ang) {
			quater = graus_quat(ang);
		}

	};

	//adicionar suporte a proje��o oblica

	class camera : public componente {
	public:

		transform* paiTF;



		glm::vec3 pos, alvo, cima;

		bool ortografica = false;
		float zoom = 90;
		vec2 tamanho = vec2(20,20),res = vec2(1,1);
		float ncp = 0.01;
		float fcp = 100;
		glm::mat4 matrizVisao, matrizProjecao;

		int8_t prioridade = 0;

		imagem* alvo_render;

		camera() {}

		

		glm::mat4 gerar_matriz_perspectiva(float zoom, int resX, int resY, float ncp, float fcp) {

			mat4 ret = glm::perspective(glm::radians(zoom), (float)(resX / resY), ncp, fcp);
			matrizProjecao = ret;
			res.x = resX;
			res.y = resY;
			this->fcp = fcp;
			this->ncp = ncp;
			return ret;

		}

		



		glm::mat4 gerar_matriz_ortografica(float tamanhoX, float tamanhoY, float ncp, float fcp) {



			mat4 ret = glm::ortho(-tamanhoX / 2, tamanhoX / 2, -tamanhoY / 2, tamanhoY / 2, ncp, fcp);
			matrizProjecao = ret;
			this->fcp = fcp;
			this->ncp = ncp;
			return ret;
		}


		void configurar_camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float ZooM, int resX, int resY, float ncp, float fcp) {
			pos = p;
			alvo = a;
			cima = c;

			matrizVisao = glm::lookAt(p, a, c);

			ortografica = false;
			zoom = ZooM;
			this->fcp = fcp;
			this->ncp = ncp;
			matrizProjecao = gerar_matriz_perspectiva(ZooM, resX, resY, ncp, fcp);
		}


		void configurar_camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float tamanhoX, float tamanhoY, float ncp, float fcp) {
			pos = p;
			alvo = a;
			cima = c;

			matrizVisao = glm::lookAt(p, a, c);

			ortografica = true;
			tamanho = vec2(-tamanhoX, tamanhoY);
			this->fcp = fcp;
			this->ncp = ncp;
			matrizProjecao = gerar_matriz_ortografica(-tamanhoX, tamanhoY, ncp, fcp);
		}





		camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float zoom, int resX, int resY, float ncp, float fcp) {
			configurar_camera(p, a, c, zoom, resX, resY, ncp, fcp);

		}



		camera(glm::vec3 p, glm::vec3 a, glm::vec3 c, float tamanhoX, float tamanhoY, float ncp, float fcp) {
			configurar_camera(p, a, c, tamanhoX, tamanhoY, ncp, fcp);

		}





		void atualizar_tf() {

			if (paiTF != NULL) {
				//matrizVisao
				vec3 nada;
				vec4 nada2;

				vec3 pos;
				quat qua;
				glm::decompose(paiTF->matrizTransform, nada, qua, pos, nada, nada2);

				matrizVisao = glm::lookAt(
					this->pos + pos, // World Space
					pos + alvo, // and looks at the origin
					this->cima  // Head is up 
				);

				matrizVisao *= toMat4(qua);

				//cout << pos.x << " " << pos.y << " " << pos.z << " " << endl;
			}
		}




	};



	class render_shader : public componente {
	public:
		Material mat;
		bool ligado = true;
		uint8_t camada = 0;
		
		uint8_t tamanho = 6;
		render_shader() {}
	};

	class render_texto : public componente {
	public:
		Material mat;
		shared_ptr<fonte> font;
		bool ligado = true;
		uint8_t camada = 0;
		float tamanho_max_linha = 34;
		float tamanho_espaco = 1;
		float espaco_entre_letras = 0.0f;
		float espaco_entre_letras_max = 3.0f, espaco_entre_letras_min = 0.0f;
		float espaco_entre_linhas = 3;
		string texto = "";





		render_texto() {}



	};


	class render_tilemap : public componente {
	public:
		render_tilemap(){}
		Material mat;
		bool ligado = true;
		uint8_t camada = 0;
		int apenas_camada = 1;
		shared_ptr<tile_set> tiles;
		shared_ptr<tile_map_info> map_info;
		


	};

	class render_sprite : public componente {
	public:
		render_sprite() {}
		Material mat;
		bool ligado = true;
		uint8_t camada = 0;
		uint16_t tile_selecionado;
		shared_ptr<tile_set> tiles;



	};
	class render_malha : public componente {
		public:
		render_malha() {}
		bool ligado = true;
		char lado_render = lado_render_malha::both;
		bool usar_oclusao = true;
		float porcentagem_pode_ocupar_tela = 1;
		uint8_t camada = 0;
		map<string,float> shape_keys_state;
		vector<shared_ptr<malha>> minhas_malhas;
		vector<Material> mats;




	};

	enum tipo_luz {
		ponto = 0,
		holofote = 1,
		direcional = 2,//tipo um sol
		global = 3
	};

	class fonte_luz : public componente {
	public:

		char tipo_luz;

		vec3 direcao = vec3(0,0,-1);
		vec3 cor;
		float angulo;
		float alcance;

		bool ligado = false;
		fonte_luz() {}

	};






	

	




	




	



	//refaser
	
	


	
	


	
	class objeto_jogo {
	private:
		shared_ptr<objeto_jogo> esse_objeto;
		map<string, shared_ptr<componente>> componentes;
	public:
		static int instancias;
		void setar_esse_objeto(shared_ptr< objeto_jogo> objeto) {
			esse_objeto = objeto;

			instancias += 1;
		}
		
		bool lixo = false;
		int ID;
		string nome;
		bool em_cena = false;

		objeto_jogo* pai;
		vector<shared_ptr<objeto_jogo>> filhos;

		void limpar_lixo() {
			map<string, shared_ptr<componente>> componentesB;
			for (pair < string, shared_ptr<componente>> p : componentes) {
				componentesB.insert(p);
			}
			componentes.swap(componentesB);

			vector<shared_ptr<objeto_jogo>> filhosB;
			for (int i = 0; i < filhos.size(); i++) {
				filhosB.resize(filhos.size());
				filhosB[i] = filhos[i];
			}
			filhos.swap(filhosB);

		}

		void desconectar_componentes() {
			esse_objeto = NULL;

			for (pair<string, shared_ptr<componente>> p : componentes) {
				p.second->esse_objeto = NULL;
			}
		}

		objeto_jogo() {
		}
		
		objeto_jogo(bool en_cena) { 
			this->em_cena = em_cena;
		}

		objeto_jogo(string t) { 
			nome = t;
		}

		void excluir() {
			lixo = true;
			for (shared_ptr<objeto_jogo> obj : filhos) {
				obj->excluir();
			}
		}


		template<typename X>
		void adicionar_componente(string nome,X comp) {
			limpar_lixo();
			comp.esse_objeto = esse_objeto;
			if(componentes.find(nome) == componentes.end()){
				componentes.insert(pair<string, shared_ptr<componente>>(nome, std::make_shared<X>(comp)));
			
				if (em_cena) {
					componentes[nome]->iniciar();
				}
			}
		}
		template<typename X>
		void adicionar_componente(X comp) {
			adicionar_componente(pegar_tipo_variavel<X>(), comp);
		}
		template<typename X>
		void adicionar_componente() {
			adicionar_componente<X>(X());
		}

		
		template<typename X>
		shared_ptr<X> pegar_componente() {
			string nome_tipo = pegar_tipo_variavel<X>();
			if (componentes.find(nome_tipo) != componentes.end()) {
				//return componentes[nome_tipo];
				return dynamic_pointer_cast<X>(componentes[nome_tipo]);
			}
			else {
				return NULL;
			}
		}



		template<typename X>
		bool tem_componente(){
			shared_ptr<componente> c = pegar_componente<X>();
			if(c = NULL){
				return false;
			}else{
				return true;
			}

		}

		void remover_componente(string s) {
			limpar_lixo();
			if (componentes.find(s) != componentes.end()) {
				if (em_cena) {
					componentes[s]->finalisar();
					
				}
				componentes[s]->esse_objeto = NULL;
				map<string, shared_ptr<componente>> componentes2;

				for (pair<string, shared_ptr<componente>> p : componentes) {
					if (p.first.compare(s)) {
						componentes2.insert(p);
					}
				}

				componentes.swap(componentes2);
				//componentes.erase(s);
			}
		}

		template<typename X>
		void remover_componente() {
			remover_componente(typeid(X).name());

		}
		
		vector<string> pegar_lista_componentes() {
			vector<string> ret;

			for (pair<string, shared_ptr<componente>> c : componentes) {
				ret.push_back(c.first);
			}

			return ret;
		}
		
		void limpar_componentes() {
			componentes.clear();
			for (pair<string, shared_ptr<componente>> p : componentes) {
				remover_componente(p.first);
			}
			map<string, shared_ptr<componente>> vazio;
		    componentes.swap(vazio);

		}



		void limpar_parentesco() {
			pai = NULL;
			vector<shared_ptr<objeto_jogo>> vazios;
			filhos.swap(vazios);
		}


		~objeto_jogo() {
			instancias -= 1;
			limpar_componentes();
			limpar_parentesco();

			limpar_lixo();
		}




		void marcar_em_cena_como(bool b) {
			em_cena = b;
			for (shared_ptr<objeto_jogo> obj : filhos) {
				obj->marcar_em_cena_como(b);
			}
		}



		void iniciar() {
			em_cena = true;
				
			

			for (pair<string, shared_ptr<componente>> p : componentes) {
				p.second->esse_objeto = esse_objeto;
				p.second->iniciar();
			}

			
			
		}

		void atualisar() {
			
			if ( em_cena) {
				limpar_lixo();

				for (pair<string, shared_ptr<componente>> p : componentes) {
					p.second->atualisar();
				}
			}
			
		}



		

		void colidir(colis_info col) {
			if (em_cena) {
				

				for (pair<string, shared_ptr<componente>> p : componentes) {
					p.second->colidir(col);
				}
			}
		}



		void finalisar() {
			em_cena = false;
			

			for (pair<string, shared_ptr<componente>> p : componentes) {
				p.second->finalisar();
			}

			//for (shared_ptr<objeto_jogo> obj : filhos) {
			//	obj->finalisar();
			//}

		}


		void adicionar_cena() {
			iniciar();

			for (shared_ptr<objeto_jogo> obj : filhos) {
				obj->adicionar_cena();
			}
		}


		

		

	};
	int objeto_jogo::instancias = 0;


	shared_ptr<objeto_jogo> novo_objeto_jogo() {
		shared_ptr<objeto_jogo> ret = make_shared<objeto_jogo>();
		ret->setar_esse_objeto(ret);
		return ret;
	}
	shared_ptr<objeto_jogo> novo_objeto_jogo(bool em_cena) {
		shared_ptr<objeto_jogo> ret = make_shared<objeto_jogo>(objeto_jogo(em_cena));
		ret->setar_esse_objeto(ret);
		return ret;
	}
	


	vector<shared_ptr<objeto_jogo>> lixeira;
	void esvasiar_lixeira() {
		vector<shared_ptr<objeto_jogo>> vazio;
		lixeira.swap(vazio);
	}

	shared_ptr<objeto_jogo> objeto_cena = novo_objeto_jogo(true);
	

	class cena_objetos {
	public:
		string nome;
		
		vector<shared_ptr<objeto_jogo>> lista_objetos;
		
	
		
		vector<int> cameras_id;
		vector<shared_ptr<objeto_jogo>> cameras;
		

		vector<int> fontes_luzes_id;
		
		

		
		vector<vector<int>> objetos_camadas_render_id;
		vector<vector<shared_ptr<objeto_jogo>>> objetos_camadas_render;
		

		shared_ptr<objeto_jogo> operator [] (int i) { return lista_objetos[i]; }

		shared_ptr<objeto_jogo> operator [] (string s) { return pegar_objeto_nome(s); }

		void remover_objeto(shared_ptr<objeto_jogo> obj) {
			obj->finalisar();
			obj->marcar_em_cena_como(false);
			obj->desconectar_componentes();

			

			if (obj->pai != NULL) {
				for (int i = 0; i < obj->pai->filhos.size(); i++) {
					if (obj->pai->filhos[i] == obj) {

						obj->pai->filhos.erase(obj->pai->filhos.begin() + i);
						obj->pai = NULL;
						break;
					}
				}
			}
			lixeira.push_back(obj);
			for (shared_ptr<objeto_jogo> f : obj->filhos) {
				remover_objeto(f);
			}
		}

		void remover_objetos(vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) { remover_objeto(obj); }
		}

		void adicionar_objeto_lista(shared_ptr<objeto_jogo> obj) {
			if (obj != NULL) {
				if (obj->lixo) {
					remover_objeto(obj);

				}
				else {


					obj->em_cena = true;



					obj->ID = lista_objetos.size();
					lista_objetos.push_back(obj);

					//camera
					void* componente[6];
					componente[0] = obj->pegar_componente<camera>().get();
					if (componente[0] != NULL) {
						cameras_id.push_back(obj->ID);
						cameras.push_back(obj);
					}









					//objetos_camadas_render_id
					shared_ptr<render_shader> rs = obj->pegar_componente<render_shader>();
					if ((rs != NULL && rs->ligado)) {
						if (objetos_camadas_render_id.size() < (rs->camada + 1)) { objetos_camadas_render_id.resize(rs->camada + 1); }
						objetos_camadas_render_id[rs->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (rs->camada + 1)) { objetos_camadas_render.resize(rs->camada + 1); }
						objetos_camadas_render[rs->camada].push_back(obj);
					}
					shared_ptr < render_texto> RT = obj->pegar_componente<render_texto>();
					if ((RT != NULL && RT->ligado)) {
						if (objetos_camadas_render_id.size() < (RT->camada + 1)) { objetos_camadas_render_id.resize(RT->camada + 1); }
						objetos_camadas_render_id[RT->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RT->camada + 1)) { objetos_camadas_render.resize(RT->camada + 1); }
						objetos_camadas_render[RT->camada].push_back(obj);
					}
					shared_ptr < render_tilemap> RTM = obj->pegar_componente<render_tilemap>();
					if ((RTM != NULL && RTM->ligado)) {
						if (objetos_camadas_render_id.size() < (RTM->camada + 1)) { objetos_camadas_render_id.resize(RTM->camada + 1); }
						objetos_camadas_render_id[RTM->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RTM->camada + 1)) { objetos_camadas_render.resize(RTM->camada + 1); }
						objetos_camadas_render[RTM->camada].push_back(obj);
					}
					shared_ptr < render_sprite> RS = obj->pegar_componente<render_sprite>();
					if ((RS != NULL && RS->ligado)) {
						if (objetos_camadas_render_id.size() < (RS->camada + 1)) { objetos_camadas_render_id.resize(RS->camada + 1); }
						objetos_camadas_render_id[RS->camada].push_back(obj->ID);

						if (objetos_camadas_render.size() < (RS->camada + 1)) { objetos_camadas_render.resize(RS->camada + 1); }
						objetos_camadas_render[RS->camada].push_back(obj);
					}

					shared_ptr < render_malha> RM = obj->pegar_componente<render_malha>();
					if ((RM != NULL && RM->ligado)) {
						if (objetos_camadas_render_id.size() < (RM->camada + 1)) { objetos_camadas_render_id.resize(RM->camada + 1); }
							objetos_camadas_render_id[RM->camada].push_back(obj->ID);

							if (objetos_camadas_render.size() < (RM->camada + 1)) { objetos_camadas_render.resize(RM->camada + 1); }
							objetos_camadas_render[RM->camada].push_back(obj);
					}
					shared_ptr <fonte_luz> FL = obj->pegar_componente<fonte_luz>();
					if ((FL != NULL && FL->ligado)) {
						fontes_luzes_id.push_back(obj->ID);
					}


					for (shared_ptr<objeto_jogo> ob : obj->filhos) {
						adicionar_objeto_lista(ob);
					}


				}
			}
		}

		shared_ptr<objeto_jogo> pegar_objeto_nome(string n) {
			shared_ptr<objeto_jogo> ret = NULL;
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				if (!obj->nome.compare(n)) {
					return obj;
				}
			}
			return ret;
		}

		
		void atualisar() {

			

				esvasiar_lixeira();



				lista_objetos.clear();
				cameras_id.clear();
				objetos_camadas_render_id.clear();
				fontes_luzes_id.clear();
				cameras.clear();
				objetos_camadas_render.clear();

				adicionar_objeto_lista(objeto_cena);




				






		}

		void atualisar_transform_objeto(shared_ptr<objeto_jogo> obj) {
			shared_ptr<transform> TF = obj->pegar_componente<transform>();
			if (TF != NULL) {
				TF->atualizar_tf();
			}

			shared_ptr < camera> cam = obj->pegar_componente<camera>();
			if (cam != NULL) {
				cam->paiTF = TF.get();
				cam->atualizar_tf();
			}




			for (shared_ptr<objeto_jogo> ob : obj->filhos) {
				shared_ptr<transform> TF_filho = ob->pegar_componente<transform>();
				if (TF_filho != NULL) {
					TF_filho->paiTF = TF.get();
				}

				atualisar_transform_objeto(ob);
			}
		}

		void atualisar_transforms() {
			atualisar_transform_objeto(objeto_cena);

		}


		

		//adicionar objeto
		void adicionar_objeto(shared_ptr<objeto_jogo> pai, shared_ptr<objeto_jogo> obj){

			obj->marcar_em_cena_como(true);
			pai->filhos.push_back(obj);
			obj->pai = pai.get();
			obj->adicionar_cena();


			
		}

		void adicionar_objetos(shared_ptr<objeto_jogo> pai,vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) {
				adicionar_objeto(pai, obj);
			}
		}

		void adicionar_objeto(objeto_jogo* pai, shared_ptr<objeto_jogo> obj) {

			obj->marcar_em_cena_como(true);
			pai->filhos.push_back(obj);
			obj->pai = pai;
			obj->adicionar_cena();



		}

		void adicionar_objetos(objeto_jogo* pai, vector<shared_ptr<objeto_jogo>> objs) {
			for (shared_ptr<objeto_jogo> obj : objs) {
				adicionar_objeto(pai, obj);
			}
		}
		
		void adicionar_objeto(shared_ptr<objeto_jogo> obj) { adicionar_objeto(objeto_cena, obj); }

		void adicionar_objetos(vector<shared_ptr<objeto_jogo>> objs) { adicionar_objetos(objeto_cena, objs); }
		
		

		

		

		//objetos[a]->scriptsCpp
		void iniciar_Logica_Scripst() {
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				obj->iniciar();
			}
		}

		void atualisar_Logica_Scripst() {
			for (shared_ptr<objeto_jogo> obj : lista_objetos) {
				if (obj != NULL) {
					obj->atualisar();
				}
			}
		}

		





		cena_objetos(string n) {
			nome = n;
			adicionar_objeto(novo_objeto_jogo());
		}



	};



	






	cena_objetos* cena_objetos_selecionados;
	//cena_objetos cena_objetos_selecionados;





	


};

