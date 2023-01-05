#pragma once
#include <math.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <map>
#include <set>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <thread>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace glm;

using namespace std;


//se esse progeto for para frende pfv adicionar suporte a linux

//#define EDITOR_MODE true
#define EDITOR_MODE false

vec3 gravidade = vec3(0, -9.8f, 0);
//vec3 gravidade = vec3(0, 0, 0);

class asset {
public:
	asset() {}
	static void deletar() {}
};

		


		
		class input_mouse {
		public:
			input_mouse() {}
			bool botao[3];
			float cursorPos[2];
			float scrolRot;
			float cursorPos_editor[2];
			float cursorMov[2];


		};

		class teclado {
		public:
			bool pegar_input_texto = false;
			string input_texto = "";
			map<string, bool> teclas = {
				pair<string, bool>("w",false),
				pair<string, bool>("a",false),
				pair<string, bool>("s",false),
				pair<string, bool>("d",false),

				pair<string, bool>("r",false),
				pair<string, bool>("c",false),
				pair<string, bool>("q",false),
				pair<string, bool>("e",false),
				pair<string, bool>("n",false),
				pair<string, bool>("m",false),
				pair<string, bool>("f",false),
				pair<string, bool>("x",false),
				pair<string, bool>("t",false),
				pair<string, bool>("g",false),
				pair<string, bool>("z",false),

				pair<string, bool>("space",false),
				pair<string, bool>("shift",false),
				pair<string, bool>("ctrl",false),
				pair<string, bool>("alt",false),
				pair<string, bool>("tab",false),
				pair<string, bool>("esc",false),

			};


		};

		class joystick {
		public:
			bool botoes[10];
			vec2 analogio[2];
			float gatilhos[2];
			ivec2 D_PAD;


			vec2 VIBRAR;


		};


		class progresso_carregamento {
		public:
			bool carregado;
			uint8_t progresso;
		};


		enum filtro_textura {
			PIXEL = 0,
			LINEAR = 1




		};


		class imagem;
		void remover_textura(imagem* img);

		class imagem : public asset {
		public:
			int ID = -1;

			
			string local = "";
			ivec2 res;
			int ContagemPixels,canais,tamanho;
			int mip_map;

			//unsigned char* data;
			vector<unsigned char> data;

			imagem(){}
			imagem( int x, int y,int c, unsigned char* dat) {
				res.x = x;
				res.y = y;
				ContagemPixels = x * y;
				canais = c;
				tamanho = x * y * c;

				data.resize(tamanho);
				for (int i = 0; i < tamanho; i++) {
					data[i] = dat[i];
				}

				delete[] dat;
				//data = dat;
			}

			friend void remover_da_api_grafica() ;

			void deletar(){
				remover_textura(this);
				//delete[] data;
				vector<unsigned char> nada;
				data.swap(nada);
			}
			~imagem() {
				deletar();
			}

			vec4 pegarPixel(int x, int y) {
				int valor = canais * (res.x * y) + x;
				return vec4(data[valor], data[valor+1], data[valor+2], data[valor+3]);
			}
			
			void mudarPixel(int x, int y,vec4 cor) {
				int valor = canais * (res.x * y) + x;
				data[valor] = cor.x;
				data[valor + 1] = cor.y;
				data[valor + 2] = cor.z;
				data[valor +3] = cor.w;
			}

		};










		int pegar_inicio_pixel(ivec2 res, ivec2 local, int canais) {
			return (int)(canais * (res.x * local.y) + local.x);
		}

		struct tile_struct {
			uint8 sprite_id = 0;
			string nome = "";
			uint8 tipo_colisao = 0;
			bool visivel = true;
			bool interativel = false;
			string script = "";
			string comentario = "";
		}; typedef struct tile_struct tile;

		class tile_set : public asset {
		public:
			tile_set(){}
			string local, local_imagem;
			ivec2 res_tile,quant_tiles;
			shared_ptr<imagem> tiles_img;
			vector<tile> tiles;
			

			tile pegar_tile(int n) { 
				return tiles[n];
			}

			void deletar() {
			}
			~tile_set() {
				deletar();
			}
		};

		

		class tile_map_info : public asset {
		public:
			string local;
			tile_map_info(){}
			ivec2 res;
			vector<vector<int>> info;

			~tile_map_info() {
				cout << "tilemap info: " << this << " apagado" << endl;
			}

		};

		




		

		



		class JanelaConfig {
		public:
			const char* nome = "";
			int X = 256, Y = 256;
			bool janelaInteira = false,transparente = false;
			int antiCerrilhado = 1;

			JanelaConfig(const char* n, int x, int y, bool fs,bool transp) {
				nome = n;
				X = x;
				Y = y;
				janelaInteira = fs;
				transparente = transp;
			}
			JanelaConfig(const char* n, int x, int y, bool fs, int aa, bool transp) {
				nome = n;
				X = x;
				Y = y;
				janelaInteira = fs;
				antiCerrilhado = aa;
				transparente = transp;
			}




		};


		












		
		

		
		
		


		
		

		





		

		struct instrucoes_render_struct {
			int camera = 0;
			bool iniciar_render = true, limpar_buffer_cores = true, limpar_buffer_profundidade = true, desenhar_objetos = true, terminar_render = true,usar_profundidade = true;




		};
		typedef struct instrucoes_render_struct instrucoes_render;

		




		struct colis_info_struct {
			shared_ptr<void> obj_ptr = NULL;
			shared_ptr<void> cos_obj_ptr = NULL;
			bool sensor;
			bool colidindo = true;
			float distancia;
			vec3 pos;
			float velocidade;
			vec3 nor;



		};
		typedef struct colis_info_struct colis_info;





		struct info_camada_render_struct {
			int camada = 0;
			vector<int> camada_colide = {0,};

		};
		typedef info_camada_render_struct info_camada;

		enum tipo_dinamica_corpo {
			estatico = 0,
			dinamico = 1,
			cinematico = 2,
		};
		enum formato_colisao {
			caixa = 0,
			esfera = 1,
			convexo = 2,
			tile_suave = 3,
		};
		



#define NUM_CARACTERES 128

		struct Caractere_info_struct {
			char letra;
			ivec2 res;
			vec4 pos_sca;
			float avancamento;
			vector<unsigned char> data;
			

			



		};
		typedef struct Caractere_info_struct Caractere_info;


		class fonte;
		void remover_fonte(fonte* f);
		class fonte : public asset {
		public:
			string local;
			fonte(){}
			int ID = -1;
			int qualidade;
			bool pixel_perfeito;
			float avancamento;
			Caractere_info Characters[NUM_CARACTERES];
			void remover_da_api_grafica();
			
			void deletar() {
				remover_fonte(this);
			}
			~fonte(){
				deletar();
			}
		};
		
		string Local_Fonte = "resources/Fonts/arial.json";
		shared_ptr<fonte> fonte_principal;




		class TMP_L2D {
		public:
			string nome;
			int X, Y;
			string tema, musica;
			vector<string> level, fundo;

			TMP_L2D() {};
			TMP_L2D(int x,int y) {
				X = x; Y = y;
				level.resize(X * Y);fundo.resize(X * Y);
			};

		};
		


		class config_audio {
		public:
			
			config_audio();

			float volume = 1, volumeMusica = 1, volumeEfeitosSonoros = 1, volumeVozes = 1;

			

			config_audio(float volumeGeral) {
				volume = volumeGeral;
				volumeMusica = volumeGeral;
				volumeEfeitosSonoros = volumeGeral;
				volumeVozes = volumeGeral;

			}


			config_audio(float volumeGeral, float musica, float som, float vozes) {

				volume = volumeGeral;
				volumeMusica = musica;
				volumeEfeitosSonoros = som;
				volumeVozes = vozes;

			}
		};


		enum tag_audio {
			volumeEfeitosSonoros = 0,
			volumeMusica = 1,
			volumeVozes = 2,

		};

		config_audio config_som = config_audio(1.0f);
		
		class audio_info {
		public:
			
			string nome;
			int ID = -1;
			tag_audio tag;
			
			bool pausa = false, loop = false;
			float tempo = 0.001f,velocidade = 1,volume = 1;

			audio_info(){}
			audio_info(string n) { nome = n; }


		};

		
		
		


		

		

		
		

		class shader : public asset {
		public:
			shader(){}
			string local;
			shader(string local) { this->local = local; }
		};

		//struct PP_struct {
		//	shader* shad;
		//
		//	vec4 cor;
		//	float gama;
		//
		//};
		//typedef struct PP_struct PosProcessamento;

		template<typename X>
		bool tem_item(vector<X> vec,X item){
			bool ret = false;
			for(X x : vec){
				if(x == item){
					ret = true;
					break;
				}
			}
			return ret;
		}

		template<typename X>
		vector<X> remover_elementos_duplicados(vector<X> vec)
		{
			vector<X> ret;
			for(X x : vec){
				if(!tem_item<X>(ret,x)){
					ret.push_back(x);
				}
			}

			return ret;
		}
		
		enum tipo_luz {
			ponto = 0,
			holofote = 1,
			direcional = 2,
		};

#define NO_TEXTURAS 6
#define NO_INPUTS 16
		class Material{
		public:
			
			string shad = "recursos/Shaders/padr�o";

			
			
			
			shared_ptr<imagem> texturas[NO_TEXTURAS];
			float inputs[NO_INPUTS];
			float interpolacao,gama = 1,metalico = 0,suave = 0;
			vec4 cor = vec4(1, 1, 1, 1);
			vec4 uv_pos_sca = vec4(0, 0, 1, 1);
			Material() {
				for (int i = 0; i < NO_TEXTURAS; i++) {
					texturas[i] = NULL;
				}
			}
		};


				//3D
				struct vertice_struct {
					float posicao[3];
					float uv[2];
					float normal[3];
					float cor[3];

					bool operator==(const vertice_struct &v) const{
						return posicao[0] == v.posicao[0] && posicao[1] == v.posicao[1] && posicao[2] == v.posicao[2] && uv[0] == v.uv[0] && uv[1] == v.uv[1] && normal[0] == v.normal[0] && normal[1] == v.normal[1] && normal[2] == v.normal[2] && cor[0] == v.cor[0] && cor[1] == v.cor[1] && cor[2] == v.cor[2];
					}
					bool operator<(const vertice_struct &v) const{
						return posicao[0] + posicao[1] + posicao[2] + uv[0] + uv[1] + normal[0] + normal[1] + normal[2] + cor[0] + cor[1] + cor[2] < v.posicao[0] + v.posicao[1] + v.posicao[2] + v.uv[0] + v.uv[1] + v.normal[0] + v.normal[1] + v.normal[2] + v.cor[0] + v.cor[1] + v.cor[2];
					}
				};
				typedef struct vertice_struct vertice;

				void escrever_vertice(vertice v){
					cout << "{" << endl;
					cout << "posicao: " << v.posicao[0] << ","<< v.posicao[1] << ","<< v.posicao[2] << endl;
					cout << "uv: " << v.uv[0] << ","<< v.uv[1] << endl;
					cout << "normal: " << v.normal[0] << ","<< v.normal[1] << ","<< v.normal[2] << endl;
					cout << "cor: " << v.cor[0] << ","<< v.cor[1] << ","<< v.cor[2] << endl;
					cout << "}" << endl;
				}

				enum lado_render_malha{
					front = 0,
					back = 1,
					both = 2,
				};

				class malha;
				void remover_malha(malha* ma);
				class malha : public asset{
				public:
					malha() {}
					string arquivo_origem,nome;
					vector<unsigned int> indice;
					vector<vertice> vertices;
					map<string,vector<vertice>> shape_keys;
					vec3 tamanho_maximo = vec3(0,0,0);
					malha(vector<unsigned int> indice, vector<vertice> vertices) {
						this->indice = indice;
						this->vertices = vertices;
					}

					//testar pode nao funcionar
					void comprimir(){
						//comprimir shape keys
						for(pair<string,vector<vertice>> p : shape_keys){
							shape_keys.insert(pair<string,vector<vertice>>(p.first,remover_elementos_duplicados<vertice>(p.second)));
						}

						//comprimir malha e gerar index
						vector<vertice> novos_vertices = remover_elementos_duplicados<vertice>(vertices);
						vector<unsigned int> novo_indice;

						novo_indice.resize(indice.size());

						for(int a = 0; a < vertices.size();a++){
							for(int b = 0; b < novos_vertices.size();b++){
								if(vertices[a] == novos_vertices[b]){
									novo_indice[a] = b;
								}
							}
						}
						indice = novo_indice;
						vertices = novos_vertices;

					}

					void pegar_tamanho_maximo(){
						for(vertice v : vertices){
							tamanho_maximo.x = std::max(tamanho_maximo.x,std::abs(v.posicao[0]));
							tamanho_maximo.y = std::max(tamanho_maximo.y,std::abs(v.posicao[1]));
							tamanho_maximo.z = std::max(tamanho_maximo.z,std::abs(v.posicao[2]));
						}
						cout << "adasdafsdsdfsdgsdgfdfggh: " << tamanho_maximo.x << " " << tamanho_maximo.y << " " << tamanho_maximo.z << endl;
					}

					~malha(){
						remover_malha(this);
					}
				};

				struct objeto_3D_struct{
					string nome;
					vector<string> minhas_malhas;
					vector<string> meus_materiais;
					map<string,string> variaveis;
					vec3 posicao;
					quat quaternion;
					vec3 escala;
					vector<struct objeto_3D_struct> filhos;
				};
				typedef struct objeto_3D_struct objeto_3D;

				struct key_frame_struct{
					float duracao;
					objeto_3D frame;
				};
				typedef struct key_frame_struct key_frame;

				float transicionar(float temp,float A,float B){
					return A + ((B - A)* temp);
				}

				void transicionar_objeto_3D_individual(float progreso,objeto_3D* A,objeto_3D B){
					A->posicao = vec3(transicionar(progreso,A->posicao.x,B.posicao.x),transicionar(progreso,A->posicao.y,B.quaternion.y),transicionar(progreso,A->posicao.z,B.posicao.z));
					A->quaternion = quat(transicionar(progreso,A->quaternion.x,B.quaternion.x),transicionar(progreso,A->quaternion.y,B.quaternion.y),transicionar(progreso,A->quaternion.z,B.quaternion.z),transicionar(progreso,A->quaternion.w,B.quaternion.w));
					A->posicao = vec3(transicionar(progreso,A->escala.x,B.escala.x),transicionar(progreso,A->escala.y,B.escala.y),transicionar(progreso,A->escala.z,B.escala.z));
				}

				void transicionar_objeto_3D(float progreso,objeto_3D* A,objeto_3D B){
					transicionar_objeto_3D_individual(progreso,A,B);
					for(int i = 0; i < A->filhos.size();i++){
						transicionar_objeto_3D(progreso,&A->filhos[i],B.filhos[i]);
					}
				}

				


				class cena_3D : public asset{
				public:
					cena_3D(){}
					string caminho;
					map<string,shared_ptr<malha>> malhas;
					map<string,Material> materiais;
					map<string,shared_ptr<imagem>> texturas;
					objeto_3D objetos;
					map<string,vector<key_frame>> animacoes;

				};


				

				template<typename X, typename Y>
				vector<X> pegar_primeiros_map(map<X, Y> m) {
					vector<X> ret;
					for (pair<X, Y> p : m) {
						ret.push_back(p.first);
					}
					return ret;
				}
				template<typename X, typename Y>
				void pegar_primeiros_map_thread(map<X, Y> m, vector<X>* ret) {
					*ret = pegar_primeiros_map<X, Y>(m);
				}

				void pegar_nomes_malhas_thread(map<string, shared_ptr<malha>> m, vector<string> *ret) {
					pegar_primeiros_map_thread<string, shared_ptr<malha>>(m,ret );
				}

				void pegar_nomes_materiais_thread(map<string, Material> m, vector<string>* ret) {
					pegar_primeiros_map_thread<string, Material > (m, ret);
				}

				void pegar_nomes_texturas_thread(map<string, shared_ptr<imagem>> m, vector<string>* ret) {
					pegar_primeiros_map_thread<string, shared_ptr<imagem>>(m, ret);
				}

				

				json converter_objeto_3D_para_json(objeto_3D o3D) {
					vector<json> filhos = {};

					for(objeto_3D o3D : o3D.filhos){
						filhos.push_back(converter_objeto_3D_para_json(o3D));
					}

					json ret = {
						{"name",o3D.nome},
						{"meshes",o3D.minhas_malhas},
						{"materials",o3D.meus_materiais},
						{"variables",o3D.variaveis},
						{"position",{{"x",o3D.posicao.x},{"y",o3D.posicao.y},{"z",o3D.posicao.z}}},
						{"quaternion",{{"x",o3D.quaternion.x},{"y",o3D.quaternion.y},{"z",o3D.quaternion.z},{"w",o3D.quaternion.w}}},
						{"escale",{{"x",o3D.escala.x},{"y",o3D.escala.y},{"z",o3D.escala.z}}},
						{"childrens", filhos},
					};
					return ret;
				}

				void converter_objeto_3D_para_json_thread(objeto_3D o3D, json *ret) {
					*ret = converter_objeto_3D_para_json(o3D);
				}

				//adicionar lua cena 3D
				string converter_cena_3D_para_json(shared_ptr<cena_3D> c3D) {
					string ret = "";

					vector<string> malhas;
					thread ta(pegar_nomes_malhas_thread, c3D->malhas, &malhas);

					vector<string> materiais;
					thread tb(pegar_nomes_materiais_thread, c3D->materiais, &materiais);

					vector<string> texturas;
					thread tc(pegar_nomes_texturas_thread, c3D->texturas, &texturas);

					json objetos = {};
					thread td(converter_objeto_3D_para_json_thread, c3D->objetos, &objetos);

					json animacoes = {};
					for (pair<string, vector<key_frame>> p : c3D->animacoes) {
						vector<json> animacao = {};
						
						for (key_frame kf : p.second) {
							json key_frame = {};

							key_frame["duration"] = kf.duracao;
							key_frame["frame"] = converter_objeto_3D_para_json(kf.frame);

							animacao.push_back(key_frame);
						}
						
						
						animacoes[p.first] = animacao;
					}


					ta.join();
					tb.join();
					tc.join();
					td.join();

					

					json JSON = {
						{"name",c3D->caminho},
						{"meshs",malhas},
						{"materials",materiais},
						{"textures",texturas},
						{"objects",objetos},
						{"animation",animacoes},
					};


					ret = JSON.dump();
					return ret;
				}


#define SAIDAS_SHADER 6
#define quantidade_fontes_luz 99
		class API_grafica_classe {
		public:
			API_grafica_classe() {}

			string modelo_gpu;
			bool pixel_perfeito = true;
			float res_dinamica = 1;
			ivec2 res_interna = ivec2(256, 256);
			vector<instrucoes_render> info_render;
			Material pos_processamento_info;
			uint8_t relevancia_camera = 0;



			virtual void iniciar_lib() {}
			virtual void desenhar_tela(Material mat) {}
			virtual void mudar_render_res(int X, int Y) {}
			virtual shared_ptr<imagem> captura_de_tela() { return NULL; }
			shared_ptr<imagem> rodar_compute_shader(Material mat, int resx, int resy, int chanels) { return NULL; }
			virtual int pegar_id_obj(int X,int Y) { return 0; }
			virtual void limpar_bufers_render(bool cor, bool profundidade) {}
			virtual void remover_textura(imagem* img) {}
			virtual void remover_malha(malha* ma){}
			virtual void remover_fonte(fonte* f) {}
			virtual void remover_shader(shader* nome) {}
			virtual void reindenizar_cenario() {}


		};
		API_grafica_classe* api_grafica;
		void remover_textura(imagem* img) {
			if (api_grafica != NULL) {
				api_grafica->remover_textura(img);
				cout << "textura: " << img << " foi removida\n";
			}
		}
		void remover_malha(malha* ma) {
			if (api_grafica != NULL) {
				api_grafica->remover_malha(ma);
				cout << "malha: " << ma->nome << " foi removida\n";
			}
		}
		void remover_fonte(fonte* f) {
			if (api_grafica != NULL) {
				api_grafica->remover_fonte(f);
				cout << "fonte: " << f << " foi removida\n";
			}
		}


		void mudar_render_res(int X, int Y) {
			if (api_grafica != NULL) {
				api_grafica->mudar_render_res(X, Y);
			}
		}
		void limpar_bufers_render(bool cor, bool profundidade) {
			if (api_grafica != NULL) {
				api_grafica->limpar_bufers_render(cor, profundidade);
			}
		}
		void reindenizar_cenario() {
			if (api_grafica != NULL) {
				api_grafica->reindenizar_cenario();
			}
		}
		shared_ptr<imagem> captura_de_tela() {
			return api_grafica->captura_de_tela();
		}
		shared_ptr<imagem> rodar_compute_shader(Material mat,int resx,int resy,int chanels) {
			return api_grafica->rodar_compute_shader(mat, resx, resy, chanels);
		}


		class funcoes_OS_classe {
		private:
		public:
			virtual string nome_OS(string s) { return ""; }
			virtual vector<string> pegar_arquivos_em_diretorio(string s) { return {}; }
			virtual vector<string> pegar_diretorios_em_diretorio(string s) { return {}; }
			virtual void criar_diretorio(string s){}
			virtual void vibrar_controle(vec2 v){}
		};
		funcoes_OS_classe* funcoes_OS;
		











		template< typename Y>
		class mapeamento_assets {
		private:
			map<string, shared_ptr<Y>> mapa;
		public: 
			mapeamento_assets(){}
			string local;
			void aplicar(string local, Y valor) {
				if (mapa.find(local) == mapa.end()) {
					mapa.insert(pair<string, shared_ptr<Y>>(local, make_shared<Y>(valor)));
				}
				else {
					mapa[local] = make_shared<Y>(valor);
				}
			}

			

			void retirar(string local) {
				map<string, shared_ptr<Y>> mapa2;
				for (pair<string, shared_ptr<Y>> p : mapa) {
					if (p.first.compare(local) != 0) {
						mapa2.insert(p);
					}
				}
				mapa.swap(mapa2);
			}

			void limpar() {
				map<string, shared_ptr<Y>> m;
				mapa.swap(m);
			}
			void limpar_lixo() {
				map<string, shared_ptr<Y>> mapa2;
				for (pair<string, shared_ptr<Y>> p : mapa) {
					if (p.second.use_count() > 2) {
						//cout << p.second.use_count() << endl;
						mapa2.insert(p);
					}
					else {
						cout << p.second.get() << " foi deletado\n";
					}
				}
				mapa.swap(mapa2);
			}

			shared_ptr<Y> pegar(string local) {
				if (mapa.find(local) != mapa.end()) {
					return mapa[local];
				}
				else {
					shared_ptr<Y> ret = NULL;
					return ret;
				}
			}




		};

		class requerimentos_cena{
		public:
			requerimentos_cena(){}
			vector<string> fontes,imagens;
		};

		vector<string> argumentos;

		void aplicar_argumentos(int argc, char** argv) {
			argumentos.resize(argc);
			for (int i = 0; i < argc; i++) {
				argumentos[i] = argv[i];
			}
		}

		map<char, int> char_int = {
			pair<char, int>('0',0),
			pair<char, int>('1',1),
			pair<char, int>('2',2),
			pair<char, int>('3',3),
			pair<char, int>('4',4),
			pair<char, int>('5',5),
			pair<char, int>('6',6),
			pair<char, int>('7',7),
			pair<char, int>('8',8),
			pair<char, int>('9',9)
		};

		map<char, bool> YN = {
			pair<char, bool>('y',true),
			pair<char, bool>('n',false),
			pair<char, bool>('Y',true),
			pair<char, bool>('N',false),
		};

		template<typename X>
		void escrever(X texto) {
			cout << texto << endl;
		}

		


			vec3 quat_graus(quat q) {
				return degrees(eulerAngles(q));
			}

			quat graus_quat(vec3 r) {
				return quat(radians(r));
			}

			bool e_visivel(mat4 P, mat4 V, mat4 T,vector<vec3> pos){
				bool ret = false;

				mat4 mat = P * V * T;

				for (vec3 p : pos) {

					vec4 pos_final = mat * vec4(p.x,p.y,p.z,1);




				}

				return ret;
			}


			bool e_visivel(mat4 P, mat4 V, mat4 T, vec3 pos) {
				return e_visivel(P, V, T, {pos});
			}

			bool e_visivel(mat4 P, mat4 V, mat4 T) {
				return e_visivel(P, V, T, vec3(0,0,0));
			}



			


		
		template<typename X>
		X* vetor_ponteiro(vector<X> a){
			X* ret;
			ret = new X[a.size()];
			for (int i = 0; i < a.size(); i++) {
				ret[i] = a[i];
			}
			return ret;
			
		}

		template<typename X>
		vector<X> ponteiro_vetor(X* a,int tamanho) {
			vector<X> ret;
			ret.resize(tamanho);



			return ret;
		}


		
		


		template<typename X>
		string pegar_tipo_variavel(X x) {
			return typeid(x).name();
		}
		template<typename X>
		string pegar_tipo_variavel() {
			return typeid(X).name();
		}
		


		



		

		//template<typename X>
		//void escrever(X texto) { cout << texto << endl; }



		template<typename X>
		string ponteiro_string(X* p) {
			stringstream ss;
			ss << p;
			return ss.str();
		}

		template<typename X>
		X* string_ponteiro(string s) {
			stringstream ss(s);
			void* ret = NULL;
			ss >> ret;
			return (X*)ret;
		}


		


		
		template<typename X, typename Y>
		vector<pair<X,Y>> map_vetor(map<X,Y> m) {
			vector<pair<X, Y>> ret;
			ret.resize(0);
			for (pair<X, Y> p : m) {
				ret.push_back(p);
			}
			return ret;
		}

		

		 string pegar_local_aplicacao() {
			return std::filesystem::current_path().string();
		 }

		 void setar_diretorio_aplicacao(string local) {
			 std::filesystem::current_path(local);
		 }

		 string pegar_estencao_arquivo(string arquivo) {
			 string ret = arquivo;
			 for (int i = 0; i < arquivo.length(); i++) {
				 if (arquivo.at(i) != '.') {
					 ret.erase(ret.begin());
				 }
				 else {
					 break;
				 }
			 }
			 return ret;
		 }
		 string pegar_nome_arquivo(string arquivo) {
			 return arquivo.substr(arquivo.find_last_of("/\\") + 1);
		 }
		 string pegar_pasta_arquivo(string arquivo) {
			 string ret = arquivo;
			 for (int i = arquivo.length() - 1; i > 0; i--) {
				 if (arquivo.at(i) != '/') {
					 ret.pop_back();
				 }
				 else {
					 break;
				 }
			 }
			 return ret;
		 }

		 vector<string> pegar_retorno_comando(string comando) {
			 vector<string> ret = {};
			 char buffer[128];
			 FILE* pipe = _popen(comando.c_str(), "r");
			 if (!pipe) throw std::runtime_error("popen() failed!");
			 try {
				 while (fgets(buffer, sizeof buffer, pipe) != NULL)
					 ret.push_back(string("") += buffer);
			 }
			 catch (...) {
				 _pclose(pipe);
				 throw;
			 }


			 return ret;
		 }

		 class lista_ponteiros {
			 string tipo;
			 vector<void*> lista;
			 vector<int> items_vazios;

			 template<typename X>
			 void Reset() {
				 tipo = pegar_tipo_variavel<X>();
				 lista.clear();
			 }

			 template<typename X>
			 lista_ponteiros() {
				 Reset<X>();
			 }

			 void adicionar(void* item) {
				 if (items_vazios.size() == 0) {
					 lista.push_back(item);
				 }
				 else
				 {
					 lista[items_vazios[0]] = item;
					 items_vazios.erase(items_vazios.begin());
				 }


			 }

			 void remover(int i) {
				 lista[i] = NULL;
				 items_vazios.push_back(i);
			 }


		 };




		 

		 





	







