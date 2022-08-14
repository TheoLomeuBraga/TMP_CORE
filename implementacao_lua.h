#pragma once

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "LoopPrincipal.h"
#include "input.h"
#include "Tempo.h"
using namespace Tempo;
#include "Objetos.h"
using namespace Objetos;
#include "box_2d.h"
#include "sfml_audio.h"
#include "lua/lua.hpp"





mapeamento_assets<string> mapeamento_scripts_lua;

shared_ptr<string> carregar_script_lua(string local) {
	if (mapeamento_scripts_lua.pegar(local) == NULL) {
		mapeamento_scripts_lua.aplicar(local, ManuseioDados::Carregar_string(local));
	}
	return mapeamento_scripts_lua.pegar(local);
	return NULL;
}

void carregar_script_lua_thread(string local, shared_ptr<string>* ret) {
	*ret = carregar_script_lua(local);
}





//criar tabela
			//https://stackoverflow.com/questions/37854422/how-to-create-table-in-table-in-lua-5-1-using-c-api
			// https://stackoverflow.com/questions/20147027/creating-a-simple-table-with-lua-tables-c-api/20148091

int add_component(lua_State* L);
int remove_component(lua_State* L);
int get_component_size(lua_State* L);
int have_component(lua_State* L);


//scripts
int get_script_size(lua_State* L);

int get_script_name(lua_State* L);

int have_script(lua_State* L);

int add_script_lua(lua_State* L);

int remove_script(lua_State* L);

int get_script_var(lua_State* L);

int set_script_var(lua_State* L);

int call_script_function(lua_State* L);




namespace funcoes_ponte {
	int hello_world(lua_State* L) {
		cout << "hello world\n";
		int ret = 0;
		lua_pushinteger(L, ret);
		return ret;
	}
	
	//screen
	int set_window(lua_State* L){
		int argumentos = lua_gettop(L);
		if (argumentos == 3) {
			loop_principal::mudar_res((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
			loop_principal::setar_tela_inteira_como((bool)lua_toboolean(L, 3));
			//cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << lua_tonumber(L, 1) << " " << lua_tonumber(L, 2) <<endl;
		}
		return 0;
	}

	int get_window(lua_State* L) {
		if (gerente_janela != NULL) {
			vec2 res = gerente_janela->pegar_res();
			bool janela_cheia = gerente_janela->e_janela_cheia();
			lua_pushnumber(L, res.x);
			lua_pushnumber(L, res.y);
			lua_pushboolean(L, janela_cheia);
			return 3;
		}
		else
		{
			return 0;
		}
	}

	//time
	int get_time(lua_State* L) {
		lua_pushnumber(L, Tempo::tempo);
		lua_pushnumber(L, Tempo::varTempRender);
		lua_pushnumber(L, Tempo::velocidadeTempo);
		return 3;
	}

	int set_time_scale(lua_State* L) {
		Tempo::velocidadeTempo = lua_tointeger(L, 1);
		return 0;
	}

	//object
	int create_object(lua_State* L) {
		int argumentos = lua_gettop(L);
		shared_ptr<objeto_jogo> obj = novo_objeto_jogo();
		if(argumentos == 0 ){ 
			Objetos::cena_objetos_selecionados->adicionar_objeto(obj);
		}
		else if(argumentos == 1){ 
			Objetos::cena_objetos_selecionados->adicionar_objeto(string_ponteiro<objeto_jogo>(lua_tostring(L, 1)),obj);
		}

		
		lua_pushstring(L,ponteiro_string<objeto_jogo>(obj.get()).c_str());
		return 1;
	}

	int remove_object(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos == 1) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			obj->excluir();
		}
		
		return 0;
	}

	int reset_components(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			obj->limpar_componentes();
		}
		return 0;
	}

	int get_father(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			output = ponteiro_string(obj->pai);
		}

		lua_pushstring(L, output.c_str());
		return 1;
	}

	int get_childrens_size(lua_State* L) {
		int output = 0;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			output = obj->filhos.size();
			//escrever(obj->filhos.size());
		}

		lua_pushnumber(L, output);
		return 1;
	}
	
	int get_children(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL) {
			output = ponteiro_string(obj->filhos[(int)lua_tonumber(L,2)].get());
		}

		lua_pushstring(L, output.c_str());
		return 1;
	}



	//memory
	int clear_memory(lua_State* L) {
		buffers_som_sfml.limpar_lixo();
		ManuseioDados::mapeamento_fontes.limpar_lixo();
		ManuseioDados::mapeamento_imagems.limpar_lixo();
		ManuseioDados::mapeamento_tilesets.limpar_lixo();
		ManuseioDados::mapeamento_tile_map_infos.limpar_lixo();
		ManuseioDados::cenas_3D.limpar_lixo();
		mapeamento_scripts_lua.limpar_lixo();
		return 0;
	}

	int load_asset(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 2 && string(lua_tostring(L, 1)).compare("font") == 0) {
			ManuseioDados::carregar_fonte(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("image") == 0) {
			ManuseioDados::carregar_Imagem(lua_tostring(L, 2));
		}
		else if (argumentos == 3 && string(lua_tostring(L, 1)).compare("tile_set") == 0) {
			ManuseioDados::carregar_tile_set(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("tile_map") == 0) {
			ManuseioDados::carregar_info_tile_map(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("audio") == 0) {
			carregar_audio_buffer_sfml(lua_tostring(L, 2));
		}

		return 0;
	}

	int asset_is_load(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		if (argumentos == 2) {
			if (string(lua_tostring(L, 1)).compare("font") == 0) {
				if (ManuseioDados::mapeamento_fontes.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("image") == 0) {
				if (ManuseioDados::mapeamento_imagems.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("tile_set") == 0) {
				if (ManuseioDados::mapeamento_tilesets.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("tile_map") == 0) {
				if (ManuseioDados::mapeamento_tile_map_infos.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("audio") == 0) {
				if (buffers_som_sfml.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
		}
		lua_pushboolean(L, output);
		return 1;
	}

	//read assets

	int tile_set_size(lua_State* L) {
		int output = 0;
		int argumentos = lua_gettop(L);
		if (argumentos == 1) {
			output = ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles.size();
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int get_tile_set_tile(lua_State* L) {
		tile output;
		int argumentos = lua_gettop(L);

		
		
		if (argumentos == 3 && (int)lua_tonumber(L, 3) > 0 && lua_tonumber(L, 3) < ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles.size()) {
			output = ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles[(int)lua_tonumber(L, 3)];
		}


		lua_pushnumber(L, output.sprite_id);
		lua_pushstring(L, output.nome.c_str());
		lua_pushnumber(L, output.tipo_colisao);
		lua_pushboolean(L, output.visivel);
		lua_pushboolean(L, output.interativel);
		lua_pushstring(L, output.script.c_str());
		lua_pushstring(L, output.comentario.c_str());
		return 7;
	}

	int get_tilemap_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		vec2 output;
		if (argumentos == 1) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->res;
		}
		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		return 2;
	}

	int get_tilemap_layer_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		if (argumentos == 1) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->info.size();
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int get_tilemap_data(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		if (argumentos == 3) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->info[lua_tonumber(L,2)][lua_tonumber(L, 3)];
		}
		lua_pushnumber(L, output);
		return 1;
	}

	//input

	int get_input(lua_State* L) {
		//http://gamedevgeek.com/tutorials/calling-c-functions-from-lua/
		
		
		int argumentos = lua_gettop(L);

		string input1 = lua_tostring(L,1);
		string input2 = lua_tostring(L, 2);
		float output = 0;

		if (argumentos == 2) {
			if (input1.compare("joystick") == 0) {

				if (input2.compare("A") == 0 && JOYSTICK[1].botoes[0]) {
					output = 1;
				}
				else if (input2.compare("B") == 0 && JOYSTICK[1].botoes[1]) {
					output = 1;
				}
				else if (input2.compare("X") == 0 && JOYSTICK[1].botoes[2]) {
					output = 1;
				}
				else if (input2.compare("Y") == 0 && JOYSTICK[1].botoes[3]) {
					output = 1;
				}

				else if (input2.compare("start") == 0 && JOYSTICK[1].botoes[8]) {
					output = 1;
				}
				else if (input2.compare("back") == 0 && JOYSTICK[1].botoes[9]) {
					output = 1;
				}

				else if (input2.compare("RB") == 0 && JOYSTICK[1].botoes[4]) {
					output = 1;
				}
				else if (input2.compare("LB") == 0 && JOYSTICK[1].botoes[6]) {
					output = 1;
				}
				else if (input2.compare("LT") == 0 && JOYSTICK[1].gatilhos[0]) {
					output = JOYSTICK[1].gatilhos[0];
				}
				else if (input2.compare("RT") == 0 && JOYSTICK[1].gatilhos[1]) {
					output = JOYSTICK[1].gatilhos[1];
				}

				else if (input2.compare("D_pad_left") == 0 && JOYSTICK[1].D_PAD.x < 0) {
					output = 1;
				}
				else if (input2.compare("D_pad_right") == 0 && JOYSTICK[1].D_PAD.x > 0) {
					output = 1;
				}
				else if (input2.compare("D_pad_up") == 0 && JOYSTICK[1].D_PAD.y > 0) {
					output = 1;
				}
				else if (input2.compare("D_pad_down") == 0 && JOYSTICK[1].D_PAD.y < 0) {
					output = 1;
				}

				else if (input2.compare("analog_left_X") == 0) {
					output = JOYSTICK[1].analogio[1].x;
				}
				else if (input2.compare("analog_left_Y") == 0) {
					output = JOYSTICK[1].analogio[1].y;
				}
				else if (input2.compare("analog_right_X") == 0) {
					output = JOYSTICK[1].analogio[0].x;
				}
				else if (input2.compare("analog_right_Y") == 0) {
					output = JOYSTICK[1].analogio[0].y;
				}


			}

			else if (input1.compare("mouse") == 0) {

				if (input2.compare("left") == 0 && MOUSE.botao[0]) {
					output = 1;
				}
				else if (input2.compare("right") == 0 && MOUSE.botao[1]) {
					output = 1;
				}
				else if (input2.compare("scroll_button") == 0 && MOUSE.botao[2]) {
					output = 1;
				}
				else if (input2.compare("scroll") == 0) {
					output = MOUSE.scrolRot;
				}
				else if (input2.compare("position_x") == 0) {
					output = MOUSE.cursorPos[0];
				}
				else if (input2.compare("position_y") == 0) {
					output = MOUSE.cursorPos[1];
				}



			}
		}
		
		
		
		lua_pushnumber(L, output);

		return 1;
	}

	//char control

	int to_move(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL) {
			if (argumentos == 3) {
				shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
				if (b2d != NULL) {
					b2d->mover(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
				}
			}
		}
		return 0;
	}

	int in_ceiling(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL) {
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL) {
				output = fc->teto;
			}
		}
		
		lua_pushboolean(L, output);
		return 1;
	}

	int in_floor(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL) {
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL) {
				output = fc->chao;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}
	
	//gravity

	int get_gravity(lua_State* L) {
		lua_pushnumber(L, gravidade.x);
		lua_pushnumber(L, gravidade.y);
		lua_pushnumber(L, gravidade.z);
		return 3;
	}

	int set_gravity(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 3) {
			gravidade = vec3(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
		}
		return 0;
	}
	
	//transform

	int get_is_ui(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			if (obj->pegar_componente<Objetos::transform>() != NULL) {
				output = obj->pegar_componente<Objetos::transform>()->UI;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}

	int set_is_ui(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				tf->UI = lua_toboolean(L, 2);
			}
		}
		return 0;
	}

	int get_position(lua_State* L) {
		vec3 output = vec3(0,0,0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = tf->pos;
			}
		}

		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int get_global_position(lua_State* L) {
		vec3 output = vec3(0, 0, 0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = tf->pegar_pos_global();
			}
		}

		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int set_position(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4 && obj != NULL) {

			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();

			if (tf != NULL) {
				tf->pos = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));

				shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
				if (b2d != NULL) {
					b2d->mudar_pos(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
				}
			}
		}
		return 0;
	}

	int get_local_vector_direction(lua_State* L) {
		vec3 output = vec3(0, 0, 0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = tf->pegar_direcao_local(vec3(lua_tonumber(L,2), lua_tonumber(L,3), lua_tonumber(L,4)));
			}
		}


		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int get_scale(lua_State* L) {
		vec3 output = vec3(0, 0, 0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = tf->esca;
			}
		}

		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int set_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4 && obj != NULL) {

			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();

			if (tf != NULL) {
				tf->esca = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			}
		}
		return 0;
	}

	int get_rotation(lua_State* L) {
		vec3 output = vec3(0, 0, 0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = quat_graus(tf->quater);
			}
		}

		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int get_global_rotation(lua_State* L) {
		vec3 output = vec3(0, 0, 0);
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
			if (tf != NULL) {
				output = tf->pegar_graus_global();
			}
		}

		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		return 3;
	}

	int set_rotation(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4 && obj != NULL) {

			shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();

			if (tf != NULL) {
				tf->quater = graus_quat(vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));

				shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
				if (b2d != NULL) {
					b2d->mudar_rot(quat_graus(tf->quater).y);
				}
			}
		}
		return 0;
	}

	

	

	//sprite render

	int set_tileset(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_sprite>() != NULL) {
			ManuseioDados::carregar_tile_set_thread(lua_tostring(L, 2), &obj->pegar_componente<render_sprite>()->tiles);
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			ManuseioDados::carregar_tile_set_thread(lua_tostring(L, 2), &obj->pegar_componente<render_tilemap>()->tiles);
		}
		return 0;
	}
	int get_tileset(lua_State* L) {
		string output[2];
		output[0] = "";
		output[1] = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1) {
			if (argumentos == 3 && obj != NULL && obj->pegar_componente<render_sprite>() != NULL) {
				output[0] = obj->pegar_componente<render_sprite>()->tiles->local;
			}
			if (argumentos == 3 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
				output[0] = obj->pegar_componente<render_tilemap>()->tiles->local;
			}
		}
		lua_pushstring(L, output[0].c_str());
		return 1;
	}

	int set_tile(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_sprite>() != NULL) {
			obj->pegar_componente<render_sprite>()->tile_selecionado = lua_tointeger(L, 2);
		}
		return 0;
	}

	int get_tile(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output = 0;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL && obj->pegar_componente<render_sprite>() != NULL) {
			output = obj->pegar_componente<render_sprite>()->tile_selecionado;
		}
		lua_pushnumber(L, output);
		return 1;
	}

	//render tilemap

	
	int get_tilemap(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL && obj->pegar_componente<render_tilemap>()->map_info != NULL) {
			output = obj->pegar_componente<render_tilemap>()->map_info->local;
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_tilemap(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			ManuseioDados::carregar_info_tile_map_thread(lua_tostring(L, 2), &obj->pegar_componente<render_tilemap>()->map_info);
			//obj->pegar_componente<render_tilemap>()->map_info = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 2));
		}
		return 0;
	}

	int get_render_only_tilemap_layer(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = -1;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			output = obj->pegar_componente<render_tilemap>()->apenas_camada;
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int set_render_only_tilemap_layer(lua_State* L) {
		int argumentos = lua_gettop(L);
		
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			obj->pegar_componente<render_tilemap>()->apenas_camada = lua_tonumber(L, 2);
		}
		return 0;
	}

	//texto

	int set_font(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_texto>() != NULL) {
			//ManuseioDados::carregar_fonte_thread(lua_tostring(L, 2), &obj->pegar_componente<render_texto>()->font);
			obj->pegar_componente<render_texto>()->font = ManuseioDados::carregar_fonte(lua_tostring(L, 2));
		}
		return 0;
	}

	int get_font(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL && obj->pegar_componente<render_texto>() != NULL) {
			output = obj->pegar_componente<render_texto>()->font->local;
		}
		lua_pushstring(L, output.c_str());

		return 0;
	}

	int set_text(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_texto>() != NULL) {
			obj->pegar_componente<render_texto>()->texto = lua_tostring(L, 2);
		}
		return 0;
	}

	int get_text(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		string output = "";
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj->pegar_componente<render_texto>() != NULL) {
			output = obj->pegar_componente<render_texto>()->texto;
		}
		lua_pushstring(L,output.c_str());
		return 1;
	}

	//fisica

	int get_physic_2D(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output[7];
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 7 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				output[0] = b2d->escala.x;
				output[1] = b2d->escala.y;
				output[2] = b2d->dinamica;
				output[3] = b2d->forma;
				output[4] = b2d->rotacionar;
				output[5] = b2d->gatilho;
				output[6] = b2d->atrito;
			}
		}



		lua_pushnumber(L, output[0]);
		lua_pushnumber(L, output[1]);
		lua_pushnumber(L, output[2]);
		lua_pushnumber(L, output[3]);
		lua_pushboolean(L, (bool)output[4]);
		lua_pushboolean(L, (bool)output[5]);
		lua_pushnumber(L, output[6]);

		return 5;
	}

	int set_physic_2D(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		
		if (argumentos == 8 && obj != NULL) {
			shared_ptr < box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->escala = vec2(lua_tonumber(L, 2), lua_tonumber(L, 3));
				b2d->dinamica = lua_tonumber(L, 4);
				b2d->forma = lua_tonumber(L, 5);
				b2d->rotacionar = lua_toboolean(L, 6);
				b2d->gatilho = lua_toboolean(L, 7);
				b2d->atrito = lua_tonumber(L, 8);
				b2d->aplicar();
			}
		}
		return 0;
	}

	int get_physic_layer(lua_State* L) { 
		int argumentos = lua_gettop(L);
		float output;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				output = b2d->camada.camada;
			}
		}



		lua_pushnumber(L, output);
		
		return 1;
	}

	int set_physic_layer(lua_State* L) { 
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}

		if (argumentos == 2 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->camada.camada = lua_tonumber(L, 2);
			}
		}
		return 0; 
	}

	int get_layers_can_colide_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr < box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				output = b2d->camada.camada_colide.size();
			}
		}



		lua_pushnumber(L, output);

		return 1;
	}

	int set_layers_can_colide_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}

		if (argumentos == 2 && obj != NULL) {
			shared_ptr < box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->camada.camada_colide.resize(lua_tonumber(L, 2));
			}
		}
		return 0;
	}

	int get_layers_can_colide(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				output = b2d->camada.camada_colide[lua_tonumber(L,2)];
			}
		}



		lua_pushnumber(L, output);

		return 1;
	}

	int set_layers_can_colide(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}

		if (argumentos == 3 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->camada.camada_colide[lua_tonumber(L, 2)] = lua_tonumber(L, 3);
			}
		}
		return 0;
	}

	int add_force(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->adicionar_forca(vec2(lua_tonumber(L,2), lua_tonumber(L, 3)));
			}
		}
		return 0;
	}

	int get_colisions_number(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				output = b2d->objs_colidindo.size();
			}
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int get_colision_object(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL && lua_tonumber(L, 2) >= 0 && lua_tonumber(L, 2) < b2d->objs_colidindo.size()) {
				output = ponteiro_string(b2d->objs_colidindo[(int)lua_tonumber(L, 2)].get());
			}
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	//camera

	int set_camera(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}

		if (obj->pegar_componente<camera>() != NULL) {
			if (argumentos == 5) {
				obj->pegar_componente<camera>()->configurar_camera(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0, 0.0f), vec3(0, 1, 0),lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
			}
			else if (argumentos == 6) {
				obj->pegar_componente<camera>()->configurar_camera(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0, 0.0f), vec3(0, 1, 0), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
			}
		}


		
		return 0;
	}

	int get_camera(lua_State* L) {
		int argumentos = lua_gettop(L);
		int ret = 0;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL) {
			
			shared_ptr<camera> cam = obj->pegar_componente<camera>();
			if (cam != NULL) {
				if (cam->ortografica) {
					lua_pushstring(L, "ortho");
					lua_pushnumber(L, cam->tamanho.x);
					lua_pushnumber(L, cam->tamanho.y);
					ret = 5;
				}
				else
				{
					lua_pushstring(L, "perspective");
					lua_pushnumber(L, cam->zoom);
					lua_pushnumber(L, cam->res.x);
					lua_pushnumber(L, cam->res.y);
					ret = 6;
				}

				lua_pushnumber(L, cam->ncp);
				lua_pushnumber(L, cam->fcp);
			}
		}

		return ret ;
	}

	//audio

	int set_audio(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		
		if (obj->pegar_componente<sfml_audio>() != NULL && argumentos == 7) {

			audio_info* info = &obj->pegar_componente<sfml_audio>()->info;
			info->nome = lua_tostring(L,2);
			info->pausa = lua_toboolean(L, 3);
			info->loop = lua_toboolean(L, 4);
			info->tempo = lua_tonumber(L, 5);
			info->velocidade = lua_tonumber(L, 6);
			info->volume = lua_tonumber(L, 7);


			obj->pegar_componente<sfml_audio>()->aplicar_info();
		}


		return 0;
	}

	int get_audio(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		audio_info output;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj->pegar_componente<sfml_audio>() != NULL && argumentos == 1) {
			output = obj->pegar_componente<sfml_audio>()->info;
			
		}
		lua_pushfstring(L, output.nome.c_str());
		lua_pushboolean(L, output.pausa);
		lua_pushboolean(L, output.loop);
		lua_pushnumber(L, output.tempo);
		lua_pushnumber(L, output.velocidade);
		lua_pushnumber(L, output.volume);
		return 6;
	}

	//geral render

	

	int set_render_layer(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2) {
			if (obj->pegar_componente<render_sprite>() != NULL) {
				obj->pegar_componente<render_sprite>()->camada = lua_tointeger(L, 2);
			}
			if (obj->pegar_componente<render_texto>() != NULL) {
				obj->pegar_componente<render_texto>()->camada = lua_tointeger(L, 2);
			}
			if (obj->pegar_componente<render_tilemap>() != NULL) {
				obj->pegar_componente<render_tilemap>()->camada = lua_tointeger(L, 2);
			}
			if (obj->pegar_componente<render_malha>() != NULL) {
				obj->pegar_componente<render_malha>()->camada = lua_tointeger(L, 2);
			}

		}
		return 0;
	}

	int get_render_layer(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1) {
			if (obj->pegar_componente<render_sprite>() != NULL) {
				output = obj->pegar_componente<render_sprite>()->camada;
			}
			if (obj->pegar_componente<render_texto>() != NULL) {
				output = obj->pegar_componente<render_texto>()->camada;
			}
			if (obj->pegar_componente<render_tilemap>() != NULL) {
				output = obj->pegar_componente<render_tilemap>()->camada;
			}
			if (obj->pegar_componente<render_malha>() != NULL) {
							output = obj->pegar_componente<render_malha>()->camada;
						}
		}
		lua_pushnumber(L, output);
		return 1;
	}

	//material
	int set_shader(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3) {

				if (obj->pegar_componente<render_sprite>() != NULL) {
					obj->pegar_componente<render_sprite>()->mat.shad = lua_tostring(L, 3);
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					obj->pegar_componente<render_texto>()->mat.shad = lua_tostring(L, 3);
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					obj->pegar_componente<render_tilemap>()->mat.shad = lua_tostring(L, 3);
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					obj->pegar_componente<render_shader>()->mat.shad = lua_tostring(L, 3);
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].shad = lua_tostring(L, 3);
								}

		}
		return 0;
	}

	int get_shader(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2) {

				if (obj->pegar_componente<render_sprite>() != NULL) {
					output = obj->pegar_componente<render_sprite>()->mat.shad;
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					output = obj->pegar_componente<render_texto>()->mat.shad;
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					output = obj->pegar_componente<render_tilemap>()->mat.shad;
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					output = obj->pegar_componente<render_shader>()->mat.shad;
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									output = obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].shad;
								}

		}


		lua_pushstring(L, output.c_str());

		return 1;
	}

	int get_color(lua_State* L) {
		vec4 output = vec4();
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2) {

				if (obj->pegar_componente<render_sprite>() != NULL) {
					output = obj->pegar_componente<render_sprite>()->mat.cor;
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					output = obj->pegar_componente<render_texto>()->mat.cor;
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					output = obj->pegar_componente<render_tilemap>()->mat.cor;
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					output = obj->pegar_componente<render_shader>()->mat.cor;
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									output = obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].cor;
								}

		}
		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		lua_pushnumber(L, output.w);
		return 4;
	}

	int set_color(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 6) {

				if (obj->pegar_componente<render_sprite>() != NULL) {
					obj->pegar_componente<render_sprite>()->mat.cor = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					obj->pegar_componente<render_texto>()->mat.cor = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					obj->pegar_componente<render_tilemap>()->mat.cor = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					obj->pegar_componente<render_shader>()->mat.cor = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].cor = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
								}

		}
		return 0;
	}

	int get_material_position_scale(lua_State* L) {
		vec4 output = vec4();
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2) {
				if (obj->pegar_componente<render_sprite>() != NULL) {
					output = obj->pegar_componente<render_sprite>()->mat.uv_pos_sca;
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					output = obj->pegar_componente<render_texto>()->mat.uv_pos_sca;
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					output = obj->pegar_componente<render_tilemap>()->mat.uv_pos_sca;
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					output = obj->pegar_componente<render_shader>()->mat.uv_pos_sca;
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									output = obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].uv_pos_sca;
								}

		}
		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		lua_pushnumber(L, output.z);
		lua_pushnumber(L, output.w);
		return 4;
	}

	int set_material_position_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 6) {
				if (obj->pegar_componente<render_sprite>() != NULL) {
					obj->pegar_componente<render_sprite>()->mat.uv_pos_sca = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					obj->pegar_componente<render_texto>()->mat.uv_pos_sca = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					obj->pegar_componente<render_tilemap>()->mat.uv_pos_sca = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					obj->pegar_componente<render_shader>()->mat.uv_pos_sca = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].uv_pos_sca = vec4(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
								}

		}
		return 0;
	}

	int get_max_textures_material(lua_State* L) {
		lua_pushnumber(L, NO_TEXTURAS);
		return 1;
	}

	int set_texture(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4) {
				if (obj->pegar_componente<render_sprite>() != NULL) {
					ManuseioDados::carregar_Imagem_thread(lua_tostring(L, 4), &obj->pegar_componente<render_sprite>()->mat.texturas[(int)lua_tonumber(L, 3)]);
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					ManuseioDados::carregar_Imagem_thread(lua_tostring(L, 4), &obj->pegar_componente<render_texto>()->mat.texturas[(int)lua_tonumber(L, 3)]);
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					ManuseioDados::carregar_Imagem_thread(lua_tostring(L, 4), &obj->pegar_componente<render_tilemap>()->mat.texturas[(int)lua_tonumber(L, 3)]);
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					ManuseioDados::carregar_Imagem_thread(lua_tostring(L, 4), &obj->pegar_componente<render_shader>()->mat.texturas[(int)lua_tonumber(L, 3)]);
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									ManuseioDados::carregar_Imagem_thread(lua_tostring(L, 4), &obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].texturas[(int)lua_tonumber(L, 3)]);
								}

		}

		return 0;
	}

	int get_texture(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3) {
				if (obj->pegar_componente<render_sprite>() != NULL && obj->pegar_componente<render_sprite>()->mat.texturas[(int)lua_tonumber(L, 3)] != NULL) {
					output = obj->pegar_componente<render_sprite>()->mat.texturas[(int)lua_tonumber(L, 3)]->local;
				}
				if (obj->pegar_componente<render_texto>() != NULL && obj->pegar_componente<render_texto>()->mat.texturas[(int)lua_tonumber(L, 3)] != NULL) {
					output = obj->pegar_componente<render_texto>()->mat.texturas[(int)lua_tonumber(L, 3)]->local;
				}
				if (obj->pegar_componente<render_tilemap>() != NULL && obj->pegar_componente<render_tilemap>()->mat.texturas[(int)lua_tonumber(L, 3)] != NULL) {
					output = obj->pegar_componente<render_tilemap>()->mat.texturas[(int)lua_tonumber(L, 3)]->local;
				}
				if (obj->pegar_componente<render_shader>() != NULL && obj->pegar_componente<render_shader>()->mat.texturas[(int)lua_tonumber(L, 3)] != NULL) {
					output = obj->pegar_componente<render_shader>()->mat.texturas[(int)lua_tonumber(L, 3)]->local;
				}
				if (obj->pegar_componente<render_malha>() != NULL && obj->pegar_componente<render_shader>()->mat.texturas[(int)lua_tonumber(L, 3)] != NULL) {
									output = obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].texturas[(int)lua_tonumber(L, 3)]->local;
								}

		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int get_max_inputs_material(lua_State* L) {
		lua_pushnumber(L, NO_INPUTS);
		return 1;
	}

	int set_material_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 4) {
				if (obj->pegar_componente<render_sprite>() != NULL) {
					obj->pegar_componente<render_sprite>()->mat.inputs[(int)lua_tonumber(L, 3)] = lua_tonumber(L, 4);
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					obj->pegar_componente<render_texto>()->mat.inputs[(int)lua_tonumber(L, 3)] = lua_tonumber(L, 4);
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					obj->pegar_componente<render_tilemap>()->mat.inputs[(int)lua_tonumber(L, 3)] = lua_tonumber(L, 4);
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					obj->pegar_componente<render_shader>()->mat.inputs[(int)lua_tonumber(L, 3)] = lua_tonumber(L, 4);
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].inputs[(int)lua_tonumber(L, 3)] = lua_tonumber(L, 4);
								}

		}

		return 0;
	}

	int get_material_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output = 0;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3) {
				if (obj->pegar_componente<render_sprite>() != NULL) {
					output = obj->pegar_componente<render_sprite>()->mat.inputs[(int)lua_tonumber(L, 3)];
				}
				if (obj->pegar_componente<render_texto>() != NULL) {
					output = obj->pegar_componente<render_texto>()->mat.inputs[(int)lua_tonumber(L, 3)];
				}
				if (obj->pegar_componente<render_tilemap>() != NULL) {
					output = obj->pegar_componente<render_tilemap>()->mat.inputs[(int)lua_tonumber(L, 3)];
				}
				if (obj->pegar_componente<render_shader>() != NULL) {
					output = obj->pegar_componente<render_shader>()->mat.inputs[(int)lua_tonumber(L, 3)];
				}
				if (obj->pegar_componente<render_malha>() != NULL) {
									output = obj->pegar_componente<render_malha>()->mats[(int)lua_tonumber(L, 2)].inputs[(int)lua_tonumber(L, 3)];
								}

		}
		lua_pushnumber(L, output);
		return 1;
	}



//post_processing
	//api_grafica->pos_processamento_info
	int set_post_processing_shader(lua_State* L) {
		int argumentos = lua_gettop(L);
		if(api_grafica != NULL && argumentos == 1){
			api_grafica->pos_processamento_info.shad = lua_tostring(L,1);
		}
		return 0;
	}

	int get_post_processing_shader(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		if(api_grafica != NULL){
			output = api_grafica->pos_processamento_info.shad;
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int get_post_processing_color(lua_State* L) {
		int argumentos = lua_gettop(L);
		vec4 output;
		if(api_grafica != NULL){
			output = api_grafica->pos_processamento_info.cor;
		}
		lua_pushnumber(L,output.x);
		lua_pushnumber(L,output.y);
		lua_pushnumber(L,output.z);
		lua_pushnumber(L,output.w);
		return 4;
	}

	int set_post_processing_color(lua_State* L) {
		int argumentos = lua_gettop(L);
		if(api_grafica != NULL && argumentos == 4){
			api_grafica->pos_processamento_info.cor = vec4(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
		}
		return 0;
	}

	int get_post_processing_position_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		vec4 output;
		if(api_grafica != NULL){
			output = api_grafica->pos_processamento_info.uv_pos_sca;
		}
		lua_pushnumber(L,output.x);
		lua_pushnumber(L,output.y);
		lua_pushnumber(L,output.z);
		lua_pushnumber(L,output.w);
		return 4;
	}

	int set_post_processing_position_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		if(api_grafica != NULL && argumentos == 4){
			api_grafica->pos_processamento_info.uv_pos_sca = vec4(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
		}
		return 0;
	}

	int set_post_processing_texture(lua_State* L) {
		int argumentos = lua_gettop(L);
		if(api_grafica != NULL && argumentos == 2){
			ManuseioDados::carregar_Imagem_thread(lua_tostring(L,2),&api_grafica->pos_processamento_info.texturas[(int)lua_tonumber(L,1)]);
		}
		return 0;
	}

	int get_post_processing_texture(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		if(api_grafica != NULL && argumentos == 1 && api_grafica->pos_processamento_info.texturas[(int)lua_tonumber(L, 1)] != NULL){
			output = api_grafica->pos_processamento_info.texturas[(int)lua_tonumber(L, 1)]->local;
		}
		lua_pushstring(L,output.c_str());
		return 1;
	}

	int set_post_processing_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		if(api_grafica != NULL && argumentos == 2){
			api_grafica->pos_processamento_info.inputs[(int)lua_tonumber(L,1)] = lua_tonumber(L,2);
		}
		return 0;
	}

	int get_post_processing_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		float output = 0;
		if(api_grafica != NULL && argumentos == 1){
			output = api_grafica->pos_processamento_info.inputs[(int)lua_tonumber(L, 1)];
		}
		lua_pushnumber(L,output);
		return 1;
	}

	int get_render_layer_instruction_size(lua_State* L){
		int output = 0;

		if(api_grafica != NULL){
			output = api_grafica->info_render.size();
		}
		lua_pushnumber(L,output);
		return 1;
	}

	int set_render_layer_instruction_size(lua_State* L){
		int argumentos = lua_gettop(L);
		if(argumentos == 1 && api_grafica != NULL){
			api_grafica->info_render.resize((int)lua_tonumber(L,1));
		}
		return 0;
	}

	int get_render_layer_instruction(lua_State* L){
		int argumentos = lua_gettop(L);
		instrucoes_render_struct ir;
		if(argumentos == 1){
			ir = api_grafica->info_render[(int)lua_tonumber(L,1)];
		}
		lua_pushnumber(L,ir.camera);
		lua_pushboolean(L,ir.iniciar_render);
		lua_pushboolean(L,ir.limpar_buffer_cores);
		lua_pushboolean(L,ir.limpar_buffer_profundidade);
		lua_pushboolean(L,ir.desenhar_objetos);
		lua_pushboolean(L,ir.terminar_render);
		lua_pushboolean(L,ir.usar_profundidade);
		return 7;
	}

	int set_render_layer_instruction(lua_State* L){
		int argumentos = lua_gettop(L);
		if(argumentos == 8){
			instrucoes_render_struct ir;
			ir.camera = (int)lua_tonumber(L,2);
			ir.iniciar_render = (bool)lua_toboolean(L,3);
			ir.limpar_buffer_cores = (bool)lua_toboolean(L,4);
			ir.limpar_buffer_profundidade = (bool)lua_toboolean(L,5);
			ir.desenhar_objetos = (bool)lua_toboolean(L,6);
			ir.terminar_render = (bool)lua_toboolean(L,7);
			ir.usar_profundidade = (bool)lua_toboolean(L,8);

			api_grafica->info_render[(int)lua_tonumber(L,1)] = ir;

		}
		return 0;
	}

	int get_mesh_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if(argumentos == 1 && obj != NULL && obj->pegar_componente<render_malha>() != NULL){
			output = obj->pegar_componente<render_malha>()->minhas_malhas.size();
		}
		lua_pushnumber(L,output);
		return 1;
	}
	int set_mesh_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if(argumentos == 2 && obj != NULL && obj->pegar_componente<render_malha>() != NULL){
			obj->pegar_componente<render_malha>()->minhas_malhas.resize((int)lua_tonumber(L, 2));
			obj->pegar_componente<render_malha>()->mats.resize((int)lua_tonumber(L, 2));
		}
		return 0;
	}

	int get_mesh(lua_State* L) {
		int argumentos = lua_gettop(L);
		return 2;
	}

	int set_mesh(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
			objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if(argumentos == 4 && obj != NULL && obj->pegar_componente<render_malha>() != NULL){
			ManuseioDados::carregar_malha_thread(lua_tostring(L, 3),lua_tostring(L, 4),&obj->pegar_componente<render_malha>()->minhas_malhas[(int)lua_tonumber(L,2)]);
		}
		return 0;
	}




	map<string, int(*)(lua_State*)> funcoes_ponte_map = {
		//tempo
		pair<string, int(*)(lua_State*)>("get_time", funcoes_ponte::get_time),
		pair<string, int(*)(lua_State*)>("set_time_scale", funcoes_ponte::set_time_scale),

		//memoria
		pair<string, int(*)(lua_State*)>("clear_memory", funcoes_ponte::clear_memory),
		pair<string, int(*)(lua_State*)>("load_asset", funcoes_ponte::load_asset),
		pair<string, int(*)(lua_State*)>("asset_is_load", funcoes_ponte::asset_is_load),

		//assets
		pair<string, int(*)(lua_State*)>("tile_set_size", funcoes_ponte::tile_set_size),
		pair<string, int(*)(lua_State*)>("get_tile_set_tile", funcoes_ponte::get_tile_set_tile),
		pair<string, int(*)(lua_State*)>("get_tilemap_size", funcoes_ponte::get_tilemap_size),
		pair<string, int(*)(lua_State*)>("get_tilemap_layer_size", funcoes_ponte::get_tilemap_layer_size),
		pair<string, int(*)(lua_State*)>("get_tilemap_data", funcoes_ponte::get_tilemap_data),

		//objeto
		pair<string, int(*)(lua_State*)>( "create_object", funcoes_ponte::create_object),
		pair<string, int(*)(lua_State*)>( "remove_object", funcoes_ponte::remove_object),
		pair<string, int(*)(lua_State*)>( "add_component", add_component),
		pair<string, int(*)(lua_State*)>( "remove_component", remove_component),
		pair<string, int(*)(lua_State*)>( "reset_components", funcoes_ponte::reset_components),
		//pair<string, int(*)(lua_State*)>( "get_component_size", get_component_size),
		pair<string, int(*)(lua_State*)>( "have_component", have_component),

		pair<string, int(*)(lua_State*)>( "get_father", funcoes_ponte::get_father),
		pair<string, int(*)(lua_State*)>( "get_childrens_size", funcoes_ponte::get_childrens_size),
		pair<string, int(*)(lua_State*)>( "get_children", funcoes_ponte::get_children),

		//movimento
		pair<string, int(*)(lua_State*)>( "to_move", funcoes_ponte::to_move),
		pair<string, int(*)(lua_State*)>( "in_ceiling", funcoes_ponte::in_ceiling),
		pair<string, int(*)(lua_State*)>( "in_floor", funcoes_ponte::in_floor),
		pair<string, int(*)(lua_State*)>( "get_gravity", funcoes_ponte::get_gravity),
		pair<string, int(*)(lua_State*)>( "set_gravity", funcoes_ponte::set_gravity),

		//transform
		pair<string, int(*)(lua_State*)>("get_is_ui", funcoes_ponte::get_is_ui),
		pair<string, int(*)(lua_State*)>("set_is_ui", funcoes_ponte::set_is_ui),
		pair<string, int(*)(lua_State*)>("get_position", funcoes_ponte::get_position),
		pair<string, int(*)(lua_State*)>("get_global_position", funcoes_ponte::get_global_position),
		pair<string, int(*)(lua_State*)>("set_position", funcoes_ponte::set_position),
		pair<string, int(*)(lua_State*)>("get_local_vector_direction", funcoes_ponte::get_local_vector_direction),
		pair<string, int(*)(lua_State*)>("get_scale", funcoes_ponte::get_scale),
		pair<string, int(*)(lua_State*)>("set_scale", funcoes_ponte::set_scale),
		pair<string, int(*)(lua_State*)>("get_rotation", funcoes_ponte::get_rotation),
		pair<string, int(*)(lua_State*)>("get_global_rotation", funcoes_ponte::get_global_rotation),
		pair<string, int(*)(lua_State*)>("set_rotation", funcoes_ponte::set_rotation),

		//camada render
		pair<string, int(*)(lua_State*)>("set_render_layer", funcoes_ponte::set_render_layer),
		pair<string, int(*)(lua_State*)>("get_render_layer", funcoes_ponte::get_render_layer),

		//material
		pair<string, int(*)(lua_State*)>("set_shader", funcoes_ponte::set_shader),
		pair<string, int(*)(lua_State*)>("get_shader", funcoes_ponte::get_shader),
		pair<string, int(*)(lua_State*)>("get_color", funcoes_ponte::get_color),
		pair<string, int(*)(lua_State*)>("set_color", funcoes_ponte::set_color),
		pair<string, int(*)(lua_State*)>("get_material_position_scale", funcoes_ponte::get_material_position_scale),
		pair<string, int(*)(lua_State*)>("set_material_position_scale", funcoes_ponte::set_material_position_scale),
		pair<string, int(*)(lua_State*)>("get_max_textures_material", funcoes_ponte::get_max_textures_material),
		pair<string, int(*)(lua_State*)>("get_max_inputs_material", funcoes_ponte::get_max_inputs_material),
		pair<string, int(*)(lua_State*)>("set_texture", funcoes_ponte::set_texture),
		pair<string, int(*)(lua_State*)>("get_texture", funcoes_ponte::get_texture),
		pair<string, int(*)(lua_State*)>("set_material_input", funcoes_ponte::set_material_input),
		pair<string, int(*)(lua_State*)>("get_material_input", funcoes_ponte::get_material_input),

		//pos-procesing
		pair<string, int(*)(lua_State*)>("set_post_processing_shader", funcoes_ponte::set_post_processing_shader),
		pair<string, int(*)(lua_State*)>("get_post_processing_shader", funcoes_ponte::get_post_processing_shader),
		pair<string, int(*)(lua_State*)>("get_post_processing_color", funcoes_ponte::get_post_processing_color),
		pair<string, int(*)(lua_State*)>("set_post_processing_color", funcoes_ponte::set_post_processing_color),
		pair<string, int(*)(lua_State*)>("get_post_processing_position_scale", funcoes_ponte::get_post_processing_position_scale),
		pair<string, int(*)(lua_State*)>("set_post_processing_position_scale", funcoes_ponte::set_post_processing_position_scale),
		pair<string, int(*)(lua_State*)>("set_post_processing_texture", funcoes_ponte::set_post_processing_texture),
		pair<string, int(*)(lua_State*)>("get_post_processing_texture", funcoes_ponte::get_post_processing_texture),
		pair<string, int(*)(lua_State*)>("set_post_processing_input", funcoes_ponte::set_post_processing_input),
		pair<string, int(*)(lua_State*)>("get_post_processing_input", funcoes_ponte::get_post_processing_input),

		//camadas render
		pair<string, int(*)(lua_State*)>("get_render_layer_instruction_size", funcoes_ponte::get_render_layer_instruction_size),
		pair<string, int(*)(lua_State*)>("set_render_layer_instruction_size", funcoes_ponte::set_render_layer_instruction_size),
		pair<string, int(*)(lua_State*)>("get_render_layer_instruction", funcoes_ponte::get_render_layer_instruction),
		pair<string, int(*)(lua_State*)>("set_render_layer_instruction", funcoes_ponte::set_render_layer_instruction),


		//janela
		pair<string, int(*)(lua_State*)>("set_window", funcoes_ponte::set_window),
		pair<string, int(*)(lua_State*)>("get_window", funcoes_ponte::get_window),

		//sprite
		pair<string, int(*)(lua_State*)>("set_tileset", funcoes_ponte::set_tileset),
		pair<string, int(*)(lua_State*)>("get_tileset", funcoes_ponte::get_tileset),
		pair<string, int(*)(lua_State*)>("set_tile", funcoes_ponte::set_tile),
		pair<string, int(*)(lua_State*)>("get_tile", funcoes_ponte::get_tile),

		//render tilemap
		pair<string, int(*)(lua_State*)>( "get_tilemap", funcoes_ponte::get_tilemap),
		pair<string, int(*)(lua_State*)>( "set_tilemap", funcoes_ponte::set_tilemap),

		pair<string, int(*)(lua_State*)>("get_render_only_tilemap_layer", funcoes_ponte::get_render_only_tilemap_layer),
		pair<string, int(*)(lua_State*)>("set_render_only_tilemap_layer", funcoes_ponte::set_render_only_tilemap_layer),


		//text
		pair<string, int(*)(lua_State*)>("set_font", funcoes_ponte::set_font),
		pair<string, int(*)(lua_State*)>("get_font", funcoes_ponte::get_font),
		pair<string, int(*)(lua_State*)>("set_text", funcoes_ponte::set_text),
		pair<string, int(*)(lua_State*)>("get_text", funcoes_ponte::get_text),

		//physic
		pair<string, int(*)(lua_State*)>("get_physic_2D", funcoes_ponte::get_physic_2D),
		pair<string, int(*)(lua_State*)>("set_physic_2D", funcoes_ponte::set_physic_2D),
		pair<string, int(*)(lua_State*)>("get_physic_layer", funcoes_ponte::get_physic_layer),
		pair<string, int(*)(lua_State*)>("set_physic_layer", funcoes_ponte::set_physic_layer),
		pair<string, int(*)(lua_State*)>("get_layers_can_colide_size", funcoes_ponte::get_layers_can_colide_size),
		pair<string, int(*)(lua_State*)>("set_layers_can_colide_size", funcoes_ponte::set_layers_can_colide_size),
		pair<string, int(*)(lua_State*)>("get_layers_can_colide", funcoes_ponte::get_layers_can_colide),
		pair<string, int(*)(lua_State*)>("set_layers_can_colide", funcoes_ponte::set_layers_can_colide),
		pair<string, int(*)(lua_State*)>("add_force", funcoes_ponte::add_force),
		pair<string, int(*)(lua_State*)>("get_colisions_number", funcoes_ponte::get_colisions_number),
		pair<string, int(*)(lua_State*)>("get_colision_object", funcoes_ponte::get_colision_object),

		//camera
		pair<string, int(*)(lua_State*)>("get_camera", funcoes_ponte::get_camera),
		pair<string, int(*)(lua_State*)>("set_camera", funcoes_ponte::set_camera),

		//audio
		pair<string, int(*)(lua_State*)>("get_audio", funcoes_ponte::get_audio),
		pair<string, int(*)(lua_State*)>("set_audio", funcoes_ponte::set_audio),

		//script
		pair<string, int(*)(lua_State*)>("get_script_size", get_script_size),
		pair<string, int(*)(lua_State*)>("get_script_name", get_script_name),
		pair<string, int(*)(lua_State*)>("have_script", have_script),
		pair<string, int(*)(lua_State*)>("add_script_lua", add_script_lua),
		pair<string, int(*)(lua_State*)>("remove_script", remove_script),
		pair<string, int(*)(lua_State*)>("get_script_var", get_script_var),
		pair<string, int(*)(lua_State*)>("set_script_var", set_script_var),
		pair<string, int(*)(lua_State*)>("call_script_function", call_script_function),

		pair<string, int(*)(lua_State*)>("get_mesh_size", get_mesh_size),
		pair<string, int(*)(lua_State*)>("set_mesh_size", set_mesh_size),
		pair<string, int(*)(lua_State*)>("get_mesh", get_mesh),
		pair<string, int(*)(lua_State*)>("set_mesh", set_mesh),

	};
	
};







namespace funcoes_lua {

	void adicionar_parte_funcoes_ponte_estado_lua(lua_State* L, vector<pair<string, int(*)(lua_State*)>> funcoes, int inicio, int tamanho) {
		for (int i = inicio; i < tamanho; i++) {
			lua_register(L, funcoes[i].first.c_str(), funcoes[i].second);
		}
	}

	void adicionar_funcoes_ponte_estado_lua(lua_State* L) {
		vector<pair<string, int(*)(lua_State*)>> funcoes_ponte_pair = map_vetor<string, int(*)(lua_State*)>(funcoes_ponte::funcoes_ponte_map);

		

		//solu��o 2
		//thread af(adicionar_parte_funcoes_ponte_estado_lua, L, funcoes_ponte_pair, 0, (int)funcoes_ponte_pair.size() / 2);
		//adicionar_parte_funcoes_ponte_estado_lua(L, funcoes_ponte_pair, (int)funcoes_ponte_pair.size() / 2, (int)funcoes_ponte_pair.size());
		//af.join();

		//solu��o 1
		for (pair<string, int(*)(lua_State*) > p : funcoes_ponte::funcoes_ponte_map) {
			lua_register(L, p.first.c_str(), p.second);
		}
	}

	

	void iniciar_estado_lua(lua_State* L) {
		

		//argumentos
		{
			lua_newtable(L);
			for (int i = 0; i < argumentos.size(); i++) {
				lua_pushinteger(L, i);
				lua_pushstring(L, argumentos[i].c_str());
				lua_settable(L, -3);
			}
			lua_setglobal(L, "args");

			lua_pushinteger(L, argumentos.size());
			lua_setglobal(L, "argsn");
		}
		
		//get_input
		{
			//controle & mouse
			{
				lua_register(L, "get_input", funcoes_ponte::get_input);
			}
			//teclado
			{
				lua_getglobal(L, "get_keys_input");
				if (lua_toboolean(L, -1)) {
					lua_getglobal(L, "keys");
					//input texto
					lua_pushstring(L, "text_input");
					lua_pushstring(L, TECLADO.input_texto.c_str());
					lua_settable(L, -3);
					//input teclas
					for (pair<string, bool> p : TECLADO.teclas) {
						lua_pushstring(L, p.first.c_str());
						lua_pushboolean(L, p.second);
						lua_settable(L, -3);
					}
				}
			}
		}

		//af.join();
		
		funcoes_lua::adicionar_funcoes_ponte_estado_lua(L);

	}


	map<string, string> scripts_lua;
	void limpar_scripts_lua() {
		scripts_lua.clear();
	}

	lua_State* carregar_luaState(string s) {
		//criar
		lua_State* ret = luaL_newstate();
		luaL_openlibs(ret);

		
		//configurar diretorio
		string local = pegar_local_aplicacao() + "/resources/Scripts/?.lua";
		lua_getglobal(ret, "package");
		lua_pushstring(ret, local.c_str());
		lua_setfield(ret, -2, "path");
		lua_pop(ret, 1);

		
		//int i = luaL_dofile(ret,s.c_str());
		int i = luaL_dostring(ret, carregar_script_lua(s)->c_str());
		funcoes_lua::iniciar_estado_lua(ret);
		

		//buscar erros
		if (i != LUA_OK) {
			escrever("LUA NOT OK");
			escrever(s);
			string erro = lua_tostring(ret, -1);
			escrever(erro);
		}
		

		
		

		return ret;
	}



	
};



class componente_lua : public componente {
	bool iniciado = false;
	map<string, lua_State*> estados_lua;
	map<string, shared_ptr<string>> scripts_lua_string;
	map<string, bool> scripts_lua_iniciados;
public:
	
	vector<string> pegar_lista_scripts() {
		vector<string> ret = {};
		for (pair<string, lua_State*> p : estados_lua) {
			ret.push_back(p.first);
		}
		return ret;
	}

	map<string, lua_State*> pegar_estados_lua() {
		return estados_lua;
	}

	void deletar_estado_lua(lua_State* L) {
		lua_close(L);
	}

	void remover_script(string s) {
		if (estados_lua.find(s) != estados_lua.end()) {
			deletar_estado_lua(estados_lua[s]);
			estados_lua.erase(s);
			scripts_lua_string.erase(s);
			scripts_lua_iniciados.erase(s);
		}
	}
	void remover_scripts(vector<string> s) {
		for (string S : s) {
			remover_script(S);
		}
	}
	void limpar_scripts() {
		vector<string> scripts;
		for (pair<string, lua_State*> p : estados_lua) {
			scripts.push_back(p.first);
		}
		remover_scripts(scripts);
	}

	void adicionar_script(string s) {
		if (estados_lua.find(s) != estados_lua.end()) {
			lua_getglobal(estados_lua[s], "END");
			lua_call(estados_lua[s], 0, 0);
			remover_script(s);
		}
		
		
		lua_State* L = funcoes_lua::carregar_luaState(s);
		estados_lua.insert(pair<string, lua_State*>(s, L));
		scripts_lua_string.insert(pair<string, shared_ptr<string>>(s, carregar_script_lua(s)));
		scripts_lua_iniciados.insert(pair<string, bool>(s, false));
		
		lua_pushstring(L, ponteiro_string(esse_objeto.get()).c_str());
		lua_setglobal(L, "this_object_ptr");
		
		//if (iniciado) {
		//	lua_getglobal(estados_lua[s], "START");
		//	lua_call(estados_lua[s], 0, 0);
		//}


	}
	void adicionar_scripts(vector<string> s) {
		for (string S : s) {
			adicionar_script(S);
		}
	}


	



	
	

	void iniciar() { 
		iniciado = true;
		for (pair<string, lua_State*> p : estados_lua) {
			//esse objetoget_tile_set_tile
			lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
			lua_setglobal(p.second, "this_object_ptr");

			lua_getglobal(p.second, "START");
			lua_call(p.second, 0, 0);
			scripts_lua_iniciados[p.first] = true;
		}
	}
	void atualisar() {
		
		for (pair<string, lua_State*> p : estados_lua) {
			if(scripts_lua_iniciados[p.first]){
				lua_State* L = p.second;
				lua_getglobal(L, "UPDATE");
				lua_call(L,0,0);
			}else{
				lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
				lua_setglobal(p.second, "this_object_ptr");
				lua_getglobal(p.second, "START");
				lua_call(p.second, 0, 0);
				scripts_lua_iniciados[p.first] = true;
			}
		}

	}
	void colidir(colis_info col) {
		for (pair<string, lua_State*> p : estados_lua) {
			lua_State* L = p.second;

			if (col.obj_ptr != NULL) {
				lua_getglobal(L, "COLIDE");

				//tabela
				lua_newtable(L);
				//id obj
				lua_pushstring(L, "object_ID");
				lua_pushstring(L, ponteiro_string(col.obj_ptr.get()).c_str());

				lua_settable(L, -3);
				//fim tabela



				lua_call(L, 1, 0);
			}
		}
	}
	void finalisar() {
		for (pair<string, lua_State*> p : estados_lua) {



			lua_State* L = p.second;


			lua_getglobal(L, "END");
			lua_call(L, 0, 0);



		}
	}
	~componente_lua() { 
		limpar_scripts();
	}
	//mudar
	void mudar_string(string script, string var, string valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushstring(estados_lua[script], valor.c_str());
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_int(string script, string var, int valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushinteger(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_numero(string script, string var, float valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushnumber(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_boleana(string script, string var, bool valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushboolean(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}

	//pegar
	string pegar_string(string script, string var) {
		string ret = "";
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tostring(estados_lua[script], -1);
		}
		return ret;
	}
	int pegar_int(string script, string var) {
		int ret = 0; 
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tointeger(estados_lua[script], -1);
		}
		return ret;
	}
	float pegar_numero(string script, string var) {
		float ret = 0;
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tonumber(estados_lua[script], -1);
		}
		return ret;
	}
	bool pegar_boleana(string script, string var) {
		bool ret = false;
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_toboolean(estados_lua[script], -1);
		}
		return ret;
	}
	
	void chamar_funcao(string script, string func){
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], func.c_str());
			lua_call(estados_lua[script], 0, 0);
		}
	}

	componente_lua() {
		
	}
	componente_lua(string s) { 
		adicionar_script(s);
	}
	componente_lua(vector<string> s) { 
		adicionar_scripts(s);
	}


};



int add_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2) {

		if (string(lua_tostring(L, 2)).compare("transform") == 0) {
			obj->adicionar_componente<Objetos::transform>(Objetos::transform(false, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
		}
		else if (string(lua_tostring(L, 2)).compare("camera") == 0) {
			obj->adicionar_componente<camera>(camera());
		}
		else if (string(lua_tostring(L, 2)).compare("render_text") == 0) {
			obj->adicionar_componente<render_texto>(render_texto());
		}
		else if (string(lua_tostring(L, 2)).compare("render_sprite") == 0) {
			obj->adicionar_componente<render_sprite>(render_sprite());
			obj->pegar_componente<render_sprite>()->mat.shad = "recursos/Shaders/sprite";
		}
		else if (string(lua_tostring(L, 2)).compare("render_shader") == 0) {
			obj->adicionar_componente<render_shader>(render_shader());
			obj->pegar_componente<render_shader>()->mat.shad = "recursos/Shaders/sprite";
		}
		else if (string(lua_tostring(L, 2)).compare("physics_2D") == 0) {
			obj->adicionar_componente<box_2D>(box_2D());
		}
		else if (string(lua_tostring(L, 2)).compare("character_physics_2D") == 0) {
			obj->adicionar_componente<fisica_char_B2D>(fisica_char_B2D());
		}
		else if (string(lua_tostring(L, 2)).compare("audio_source") == 0) {
			obj->adicionar_componente<sfml_audio>(sfml_audio());
		}
		else if (string(lua_tostring(L, 2)).compare("lua_scripts") == 0) {
			obj->adicionar_componente<componente_lua>(componente_lua());
		}
		else if (string(lua_tostring(L, 2)).compare("render_tile_map") == 0) {
			obj->adicionar_componente<render_tilemap>(render_tilemap());
		}
		else if (string(lua_tostring(L, 2)).compare("render_mesh") == 0) {
			obj->adicionar_componente<render_malha>();
		}
		
	}


	return 0;
}

int remove_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2) {

		if (string(lua_tostring(L, 2)).compare("transform") == 0) {
			obj->remover_componente<Objetos::transform>();
		}
		else if (string(lua_tostring(L, 2)).compare("camera") == 0) {
			obj->remover_componente<Objetos::camera>();
		}
		else if (string(lua_tostring(L, 2)).compare("render_text") == 0) {
			obj->remover_componente<Objetos::render_texto>();
		}
		else if (string(lua_tostring(L, 2)).compare("render_sprite") == 0) {
			obj->remover_componente<Objetos::render_sprite>();
		}
		else if (string(lua_tostring(L, 2)).compare("render_shader") == 0) {
			obj->remover_componente<render_shader>();
		}
		else if (string(lua_tostring(L, 2)).compare("physics_2D") == 0) {
			obj->remover_componente<box_2D>();
		}
		else if (string(lua_tostring(L, 2)).compare("character_physics_2D") == 0) {
			obj->remover_componente<fisica_char_B2D>();
		}
		else if (string(lua_tostring(L, 2)).compare("audio_source") == 0) {
			obj->remover_componente<sfml_audio>();
		}
		else if (string(lua_tostring(L, 2)).compare("lua_scripts") == 0) {
			obj->remover_componente<componente_lua>();
		}
		else if (string(lua_tostring(L, 2)).compare("render_tile_map") == 0) {
			obj->remover_componente<render_tilemap>();
		}
		else if (string(lua_tostring(L, 2)).compare("render_mesh") == 0) {
			obj->remover_componente<render_malha>();
		}

	}


	return 0;
}

int get_component_size(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		int ret = 0;
		if(argumentos == 1){
			ret = obj->pegar_lista_componentes().size();
		}

		lua_pushnumber(L,ret);
		return 1;
}



int have_component(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		bool ret = false;
		if(argumentos == 2){
			if (string(lua_tostring(L, 2)).compare("transform") == 0) {
				ret = obj->tem_componente<Objetos::transform>();
			}
			else if (string(lua_tostring(L, 2)).compare("camera") == 0) {
				ret = obj->tem_componente<Objetos::camera>();
			}
			else if (string(lua_tostring(L, 2)).compare("render_text") == 0) {
				ret = obj->tem_componente<Objetos::render_texto>();
			}
			else if (string(lua_tostring(L, 2)).compare("render_sprite") == 0) {
				ret = obj->tem_componente<Objetos::render_sprite>();
			}
			else if (string(lua_tostring(L, 2)).compare("render_shader") == 0) {
				ret = obj->tem_componente<Objetos::render_shader>();
			}
			else if (string(lua_tostring(L, 2)).compare("physics_2D") == 0) {
				ret = obj->tem_componente<box_2D>();
			}
			else if (string(lua_tostring(L, 2)).compare("character_physics_2D") == 0) {
				ret = obj->tem_componente<fisica_char_B2D>();
			}
			else if (string(lua_tostring(L, 2)).compare("audio_source") == 0) {
				ret = obj->tem_componente<sfml_audio>();
			}
			else if (string(lua_tostring(L, 2)).compare("lua_scripts") == 0) {
				ret = obj->tem_componente<componente_lua>();
			}
			else if (string(lua_tostring(L, 2)).compare("render_tile_map") == 0) {
				ret = obj->tem_componente<render_tilemap>();
			}
			else if (string(lua_tostring(L, 2)).compare("render_mesh") == 0) {
				obj->tem_componente<render_malha>();
			}
		}

		lua_pushboolean(L,ret);
		return 1;
}

int add_script_lua(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		cl->adicionar_script( lua_tostring(L, 2));
	}
	return 0;
}

int remove_script(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		cl->remover_script(lua_tostring(L, 2));
	}
	return 0;
}

//scripts

//pegar_lista_scripts

int get_script_size(lua_State* L) {
	int output = 0;
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 1 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL) {
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts().size();
	}
	lua_pushnumber(L, output);
	return 1;
}

int get_script_name(lua_State* L) {
	string output = "";
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL) {
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts()[0];
	}
	lua_pushstring(L, output.c_str());
	return 1;
}

int have_script(lua_State* L) {
	bool output = false;
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		map<string, lua_State*> m = cl->pegar_estados_lua();
		if (m.find(lua_tostring(L, 2)) != m.end()) {
			output = true;
		}
	}
	lua_pushboolean(L, output);
	return 1;
}

int get_script_var(lua_State* L) {

	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 4 && cl != NULL) {
		
		if (lua_tonumber(L, 4) == 0) {
			lua_pushstring(L, cl->pegar_string(lua_tostring(L, 2), lua_tostring(L, 3)).c_str());
			return 1;
		}
		else if(lua_tonumber(L, 4) == 1) {
			lua_pushnumber(L, cl->pegar_numero(lua_tostring(L, 2), lua_tostring(L, 3)));
			return 1;
		}
		else if (lua_tonumber(L, 4) == 2) {
			lua_pushboolean(L, cl->pegar_boleana(lua_tostring(L, 2), lua_tostring(L, 3)));
			return 1;
		}

	}
	return 0;
}

int set_script_var(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 5 && cl != NULL) {

		if (lua_tonumber(L, 4) == 0) {
			cl->mudar_string(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 5));
		}
		else if (lua_tonumber(L, 4) == 1) {
			cl->mudar_numero(lua_tostring(L, 2), lua_tostring(L, 3), lua_tonumber(L, 5));
			
		}
		else if (lua_tonumber(L, 4) == 2) {
			cl->mudar_boleana(lua_tostring(L, 2), lua_tostring(L, 3), lua_toboolean(L, 5));
			
		}

	}
	return 0;
}

int call_script_function(lua_State* L){
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 3 && cl != NULL) {
		cl->chamar_funcao(lua_tostring(L, 2),lua_tostring(L, 3));
	}
	return 0;
}
