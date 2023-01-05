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

#include "projetil.h"



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

json material_json(Material mat){
	vector<string> textures;
	for (shared_ptr<imagem> i : mat.texturas) {
		if (i != NULL) {
			textures.push_back(i->local);
		}
		else
		{
			textures.push_back("");
		}
	}

	vector<float> inputs;
	for (float i : mat.inputs) {
		inputs.push_back(i);
	}

	json JSON = {
		{"shader",mat.shad},
		{"color",{{"r",mat.cor.x},{"g",mat.cor.y},{"b",mat.cor.z},{"a",mat.cor.w}}},
		{"position_scale",{{"x",mat.uv_pos_sca.x},{"y",mat.uv_pos_sca.y},{"z",mat.uv_pos_sca.z},{"w",mat.uv_pos_sca.w}}},
		{"metallic",mat.metalico},
		{"softness",mat.suave},
		{"textures",textures},
		{"inputs",inputs},
		
	};
	return JSON;
}

Material json_material(json JSON) {
	Material ret = Material();
	json color = JSON["color"].get<json>(), position_scale = JSON["position_scale"].get<json>();
	vector<string> textures = JSON["textures"].get<vector<string>>();
	vector<float> inputs = JSON["inputs"].get<vector<float>>();
	
	ret.shad = JSON["shader"].get<string>();
	ret.cor = vec4(color["r"].get<float>(), color["g"].get<float>(), color["b"].get<float>(), color["a"].get<float>());
	ret.uv_pos_sca = vec4(position_scale["x"].get<float>(), position_scale["y"].get<float>(), position_scale["z"].get<float>(), position_scale["w"].get<float>());
	ret.metalico = JSON["metallic"].get<float>();
	ret.suave = JSON["softness"].get<float>();

	for (int i = 0; i < textures.size(); i++) {
		ret.texturas[i] = ManuseioDados::carregar_Imagem(textures[i]);
	}
	for (int i = 0; i < inputs.size(); i++) {
		ret.inputs[i] = inputs[i];
	}

	return ret;
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
	int set_window(lua_State* L) {
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
		if (argumentos == 0) {
			Objetos::cena_objetos_selecionados->adicionar_objeto(obj);
		}
		else if (argumentos == 1) {
			Objetos::cena_objetos_selecionados->adicionar_objeto(string_ponteiro<objeto_jogo>(lua_tostring(L, 1)), obj);
		}


		lua_pushstring(L, ponteiro_string<objeto_jogo>(obj.get()).c_str());
		return 1;
	}

	int get_object_with_name(lua_State* L) {
		string output = "";
		shared_ptr<objeto_jogo> obj = (*Objetos::cena_objetos_selecionados)[lua_tostring(L, 1)];
		output = ponteiro_string(obj.get());
		lua_pushstring(L, output.c_str());
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

	

	int get_object_family_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL) {
			vector<json> criancas;

			for (shared_ptr<objeto_jogo> p : obj->filhos) {
				criancas.push_back(ponteiro_string(p.get()));
			}

			json JSON = { 
				{"father",ponteiro_string(obj->pai)},
				{"children",criancas},
			};

			output = JSON.dump();
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

	int get_tile_set_size(lua_State* L) {
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
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->info[lua_tonumber(L, 2)][lua_tonumber(L, 3)];
		}
		lua_pushnumber(L, output);
		return 1;
	}

	//adicionar lua cena 3D
	int get_scene_3D_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		if (argumentos == 1) {
			output = converter_cena_3D_para_json(ManuseioDados::carregar_modelo_3D(lua_tostring(L, 1)));
		}
		
		lua_pushstring(L, output.c_str());
		return 1;
	}

	//input

	

	void push_input_joystick(lua_State* L, string nome, float precao) {
		lua_pushstring(L, nome.c_str());
		lua_pushnumber(L, precao);
		lua_settable(L, -3);
	}

	void push_input_mouse_keyboard(lua_State* L, string nome, bool precao) {
		lua_pushstring(L, nome.c_str());
		lua_pushboolean(L, precao);
		lua_settable(L, -3);
	}
	
	int set_keyboard_text_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 1) {
			TECLADO.pegar_input_texto = lua_toboolean(L, 1);
		}
		return 0;
	}
	int set_cursor_position(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 2) {
			gerente_janela->mudar_pos_cursor(lua_tonumber(L,1), lua_tonumber(L, 2));
		}
		return 0;
	}
	int get_keyboard_input(lua_State* L) {
		lua_newtable(L);
		{
			for (pair<string, bool> p : Teclado.teclas) {
				push_input_mouse_keyboard(L, p.first, p.second);
			}
		}
		return 1;
	}

	int get_mouse_input(lua_State* L) {
		lua_newtable(L);
		{
			push_input_mouse_keyboard(L, "L", MOUSE.botao[0]);
			push_input_mouse_keyboard(L, "R", MOUSE.botao[1]);
			push_input_mouse_keyboard(L, "scroll", MOUSE.botao[2]);
			push_input_joystick(L, "scroll_rotation", MOUSE.scrolRot);

			lua_pushstring(L, "cursos_position");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", MOUSE.cursorPos[0]);
				push_input_joystick(L, "y", MOUSE.cursorPos[1]);
				lua_settable(L, -3);
			}
			
		}
		return 1;
	}

	int get_joystick_input(lua_State* L) {

		lua_newtable(L);
		{
			push_input_joystick(L, "A", JOYSTICK[1].botoes[0]);
			push_input_joystick(L, "B", JOYSTICK[1].botoes[1]);
			push_input_joystick(L, "X", JOYSTICK[1].botoes[2]);
			push_input_joystick(L, "Y", JOYSTICK[1].botoes[3]);

			push_input_joystick(L, "RB", JOYSTICK[1].botoes[4]);
			push_input_joystick(L, "RT", JOYSTICK[1].botoes[5]);
			push_input_joystick(L, "LB", JOYSTICK[1].botoes[6]);
			push_input_joystick(L, "RT", JOYSTICK[1].botoes[7]);

			push_input_joystick(L, "START", JOYSTICK[1].botoes[8]);
			push_input_joystick(L, "BACK", JOYSTICK[1].botoes[9]);


			lua_pushstring(L, "D_PAD");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].D_PAD.x);
				push_input_joystick(L, "y", JOYSTICK[1].D_PAD.y);
				

				lua_settable(L, -3);
			}

			lua_pushstring(L,"analogL");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].analogio[0].x);
				push_input_joystick(L, "y", JOYSTICK[1].analogio[0].y);

				lua_settable(L, -3);
			}

			lua_pushstring(L, "analogR");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].analogio[1].x);
				push_input_joystick(L, "y", JOYSTICK[1].analogio[1].y);

				lua_settable(L, -3);
			}
			
			
		}
		

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

	int get_transform_json(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (argumentos == 1 && tf != NULL) {
			json JSON;

			JSON["is_ui"] = tf->UI;

			JSON["position"] = { {"x",tf->pos.x} ,{"y",tf->pos.y} ,{"z",tf->pos.z}};
			vec3 rot = tf->pegar_angulo_graus();
			JSON["rotation"] = { {"x",rot.x} ,{"y",rot.y} ,{"z",rot.z} };
			JSON["scale"] = { {"x",tf->esca.x} ,{"y",tf->esca.y} ,{"z",tf->esca.z} };

			output = JSON.dump();
		}

		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_transform_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (argumentos == 2 && obj != NULL && tf != NULL) {
			json JSON = json::parse(lua_tostring(L, 2));
			tf->UI = JSON["is_ui"].get<bool>();

			json pos = JSON["position"].get<json>(), rot = JSON["rotation"].get<json>(), sca = JSON["scale"].get<json>();
			tf->pos = vec3(pos["x"].get<float>(), pos["y"].get<float>(), pos["z"].get<float>());
			tf->mudar_angulo_graus(vec3(rot["x"].get<float>(), rot["y"].get<float>(), rot["z"].get<float>()));
			tf->esca = vec3(sca["x"].get<float>(), sca["y"].get<float>(), sca["z"].get<float>());

		}
		return 0;
	}

	int move_transform(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->pos += v3;
		}
		return 0;
	}

	int rotate_transform(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (tf != NULL) {
			
			vec3 v3a = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)),v3b = tf->pegar_angulo_graus(),v3c = vec3(v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z);
			tf->mudar_angulo_graus(v3c);
		}
		return 0;
	}

	int change_transfotm_position(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->pos = v3;
		}
		return 0;
	}

	int change_transfotm_rotation(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->mudar_angulo_graus(v3);
		}
		return 0;
	}

	int change_transfotm_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<Objetos::transform> tf = obj->pegar_componente<Objetos::transform>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->esca = v3;
		}
		return 0;
	}

	
	//sprite render

	


	int get_sprite_render_json(lua_State* L) {
		objeto_jogo* obj = NULL;
		int argumentos = lua_gettop(L);
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = {};
		shared_ptr<render_sprite> b2d = obj->pegar_componente<render_sprite>();
		if (b2d != NULL) {
			JSON = {
				{"layer",b2d->camada},
				{"selected_tile",b2d->tile_selecionado},
				{"tile_set_local",b2d->tiles->local},
				{"material",material_json(b2d->mat)},
			};
		}
		lua_pushstring(L, JSON.dump().c_str());
		return 1;
	}

	int set_sprite_render_json(lua_State* L) {
		objeto_jogo* obj = NULL;
		int argumentos = lua_gettop(L);
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = json::parse(lua_tostring(L, 2));
		shared_ptr<render_sprite> b2d = obj->pegar_componente<render_sprite>();
		if (b2d != NULL) {
			b2d->camada = JSON["layer"].get<int>();
			b2d->tile_selecionado = JSON["selected_tile"].get<int>();
			b2d->tiles = ManuseioDados::carregar_tile_set(JSON["tile_set_local"].get<string>());
			b2d->mat = json_material(JSON["material"].get<json>());
		}
		return 0;
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



	int get_render_tilemap_json(lua_State* L) {
		objeto_jogo* obj = NULL;
		int argumentos = lua_gettop(L);
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = {};
		shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
		if (rtm != NULL) {
			JSON = {
				{"layer",rtm->camada},
				{"material",material_json(rtm->mat)},
				{"render_tilemap_only_layer",rtm->apenas_camada},
				{"tile_set_local",rtm->tiles->local},
				{"tile_map_local",rtm->map_info->local},
			};
		}
		lua_pushstring(L, JSON.dump().c_str());
		return 1;
	}
	int set_render_tilemap_json(lua_State* L) {
		objeto_jogo* obj = NULL;
		int argumentos = lua_gettop(L);
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = json::parse(lua_tostring(L, 2));
		shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
		if (rtm != NULL) {
			rtm->camada = JSON["layer"].get<int>();
			rtm->mat = json_material(JSON["material"].get<json>());
			rtm->apenas_camada = JSON["render_tilemap_only_layer"].get<int>();
			rtm->tiles = ManuseioDados::carregar_tile_set(JSON["tile_set_local"].get<string>());
			rtm->map_info = ManuseioDados::carregar_info_tile_map(JSON["tile_map_local"].get<string>());
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

	int set_text(lua_State* L) {
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

	int get_text(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		string output = "";
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj->pegar_componente<render_texto>() != NULL) {
			output = obj->pegar_componente<render_texto>()->texto;
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int get_text_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = {};
		shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
		if (rt != NULL && argumentos == 1) {
			JSON = {
				{"layer",rt->camada},
				{"font",rt->font->local},
				{"text",rt->texto},
				{"line_size",rt->tamanho_max_linha},
				{"space_betwen_chars",rt->espaco_entre_letras},
				{"max_space_betwen_chars",rt->espaco_entre_letras_max},
				{"min_space_betwen_chars",rt->espaco_entre_letras_min},
				{"material",material_json(rt->mat)},
			};
		}
		lua_pushstring(L, JSON.dump().c_str());
		return 1;
	}

	int set_text_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = {};
		shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
		if (rt != NULL && argumentos == 2) {
			JSON = json::parse(lua_tostring(L, 2));
			rt->camada = JSON["layer"].get<int>();
			rt->font = ManuseioDados::carregar_fonte(JSON["font"].get<string>());
			rt->texto = JSON["text"].get<string>();
			rt->espaco_entre_letras = JSON["space_betwen_chars"].get<float>();
			rt->espaco_entre_letras_max = JSON["max_space_betwen_chars"].get<float>();
			rt->espaco_entre_letras_min = JSON["min_space_betwen_chars"].get<float>();
			rt->tamanho_max_linha = JSON["line_size"].get<float>();
			rt->mat = json_material(JSON["material"].get<json>());
		}
		return 0;
	}
	

	//fisica

	int add_force(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->adicionar_forca(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
			}
		}
		return 0;
	}

	

	int get_physic_2D_json(lua_State* L) {
		objeto_jogo* obj = NULL;
		int argumentos = lua_gettop(L);
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON = {};
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (b2d != NULL) {
			json scale = { {"x",b2d->escala.x},{"y",b2d->escala.y}};
			vector<string> objetos_colidindo;
			vector<json> camada_colisao;
			for (shared_ptr<objeto_jogo> oc : b2d->objs_colidindo) {
				objetos_colidindo.push_back(ponteiro_string<objeto_jogo>(oc.get()));
			}

			camada_colisao.push_back({"layer",b2d->camada.camada} );
			camada_colisao.push_back({ "layers_can_colide",b2d->camada.camada_colide });
			


			JSON = { 
				{"scale",scale},
				{"boady_dynamic",b2d->dinamica},
				{"colision_shape",b2d->forma},
				{"rotate",b2d->rotacionar},
				{"triger",b2d->gatilho},
				{"friction",b2d->atrito},
				{"objects_coliding",objetos_colidindo},
				{"colision_layer",camada_colisao},
			};
		}
		lua_pushstring(L, JSON.dump().c_str());
		return 1;
	}
	int set_physic_2D_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (argumentos == 2 && b2d != NULL ) {
			json JSON = json::parse(lua_tostring(L, 1));

			json scale = JSON["scale"].get<json>();
			b2d->escala = vec2(scale["x"].get<float>(), scale["y"].get<float>());
			b2d->dinamica = JSON["boady_dynamic"].get<int>();
			b2d->forma = JSON["colision_shape"].get<int>();
			b2d->rotacionar = JSON["rotate"].get<bool>();
			b2d->gatilho = JSON["triger"].get<bool>();
			b2d->atrito = JSON["friction"].get<float>();

			json camada_colisao = JSON["colision_layer"].get<json>();
			b2d->camada.camada = camada_colisao["layer"].get<int>();
			b2d->camada.camada_colide = camada_colisao["layers_can_colide"].get<vector<int>>();

		}
		return 0;
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
				obj->pegar_componente<camera>()->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5));
			}
			else if (argumentos == 6) {
				obj->pegar_componente<camera>()->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6));
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

		return ret;
	}

	//audio

	int set_audio(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}

		if (obj->pegar_componente<sfml_audio>() != NULL && argumentos == 7) {

			audio_info* info = &obj->pegar_componente<sfml_audio>()->info;
			info->nome = lua_tostring(L, 2);
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
		lua_pushstring(L, output.nome.c_str());
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
	

	int get_post_processing_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		if (api_grafica != NULL) {
			vector<string> textures;
			for (int i = 0; i < NO_TEXTURAS; i++) {
				if (api_grafica->pos_processamento_info.texturas[i] != NULL) {
					textures.push_back(api_grafica->pos_processamento_info.texturas[i]->local);
				}else{
					textures.push_back("");
				}
			}


			vector<float> inputs;
			for (int i = 0; i < NO_INPUTS; i++) {
				inputs.push_back(api_grafica->pos_processamento_info.inputs[i]);
			}
			json color = { { "r",api_grafica->pos_processamento_info.cor.x }, { "g",api_grafica->pos_processamento_info.cor.y }, { "b",api_grafica->pos_processamento_info.cor.z }, { "a",api_grafica->pos_processamento_info.cor.w } };
			json position_scale = { {"x",api_grafica->pos_processamento_info.uv_pos_sca.x},{"y",api_grafica->pos_processamento_info.uv_pos_sca.y},{"z",api_grafica->pos_processamento_info.uv_pos_sca.z},{"w",api_grafica->pos_processamento_info.uv_pos_sca.w} };

			json JSON = {
				{"shader",api_grafica->pos_processamento_info.shad},
				{"color",color},
				{"position_scale",position_scale},
				{"textures",textures},
				{"inputs",inputs},
			};

			output = JSON.dump();
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}
	int set_post_processing_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 1 && api_grafica != NULL) {
			json JSON = json::parse(lua_tostring(L, 1));
			api_grafica->pos_processamento_info.shad = JSON["shader"].get<string>();
			json cor = JSON["color"].get<json>();
			api_grafica->pos_processamento_info.cor = vec4(cor["r"].get<float>(), cor["g"].get<float>(), cor["b"].get<float>(), cor["a"].get<float>());
			json uv_pos_sca = JSON["position_scale"].get<json>();
			api_grafica->pos_processamento_info.uv_pos_sca = vec4(uv_pos_sca["x"].get<float>(), uv_pos_sca["y"].get<float>(), uv_pos_sca["z"].get<float>(), uv_pos_sca["w"].get<float>());
			vector<string> textures = JSON["textures"].get<vector<string>>();
			for (int i = 0; i < NO_TEXTURAS; i++) {
					api_grafica->pos_processamento_info.texturas[i] = ManuseioDados::carregar_Imagem(textures[i]);
			}
			vector<float> inputs = JSON["inputs"].get<vector<float>>();
			for (int i = 0; i < NO_INPUTS; i++) {
				api_grafica->pos_processamento_info.inputs[i] = inputs[i];
			}
		}
		return 0;
	}

	//render layers

	

	int get_render_layer_instruction_json(lua_State* L) {
		string output = "";
		vector<json> camadas;

		for (instrucoes_render ir : api_grafica->info_render) {
			json camada = {
				{"camera_selected",ir.camera},
				{"start_render",ir.iniciar_render},
				{"clean_color",ir.limpar_buffer_cores},
				{"clean_deph",ir.limpar_buffer_profundidade},
				{"enable",ir.desenhar_objetos},
				{"end_render",ir.terminar_render},
				{"use_deeph",ir.usar_profundidade},
			};
			camadas.push_back(camada);
		}

		json JSON = {"layers",camadas };
		output = JSON.dump();
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_render_layer_instruction_json(lua_State* L) {
		json JSON = json::parse(lua_tostring(L,1));
		vector<json> camadas = JSON["layers"].get<vector<json>>();

		api_grafica->info_render.clear();

		for (json j : camadas) {
			instrucoes_render ir;

			ir.camera = j["camera_selected"].get<int>();
			ir.iniciar_render = j["start_render"].get<bool>();
			ir.limpar_buffer_cores = j["clean_color"].get<bool>();
			ir.limpar_buffer_profundidade = j["clean_deph"].get<bool>();
			ir.desenhar_objetos = j["enable"].get<bool>();
			ir.terminar_render = j["end_render"].get<bool>();
			ir.usar_profundidade = j["use_deeph"].get<bool>();

			api_grafica->info_render.push_back(ir);
		}

		return 0;
	}

	int get_mesh_json(lua_State* L) {
		string output = "";
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		json JSON;
		shared_ptr<render_malha> rm = obj->pegar_componente<render_malha>();
		if (rm != NULL) {
			vector<json> materials;
			for (Material m : rm->mats) {
				materials.push_back(material_json(m));
			}

			vector<json> meshes;
			for (shared_ptr<malha> m : rm->minhas_malhas) {
				meshes.push_back({ {"file",m->arquivo_origem},{"name",m->nome}});
			}

			JSON = {
				{"layer",rm->camada},
				{"use_oclusion",rm->usar_oclusao},
				{"normal_direction",rm->lado_render},
				{"meshes",meshes},
				{"materials",materials},
			};
		}


		output = JSON.dump();
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_mesh_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<render_malha> rm = obj->pegar_componente<render_malha>();
		if (rm != NULL) {
			json JSON = json::parse(lua_tostring(L,2));
			vector<json> meshe_infos = JSON["meshes"].get<json>();
			vector<json> material_infos = JSON["materials"].get<json>();

			rm->camada = JSON["layer"].get<int>();
			rm->usar_oclusao = JSON["use_oclusion"].get<bool>();
			rm->lado_render = JSON["normal_direction"].get<int>();
			
			for (json j : meshe_infos) {
				rm->minhas_malhas.clear();
				rm->minhas_malhas.push_back(ManuseioDados::carregar_malha(j["file"].get<string>(), j["name"].get<string>()));
			}
			for (json j : material_infos) {
				rm->mats.clear();
				rm->mats.push_back(json_material(j));
			}
		}
		return 0;
	}


	int get_projectile_json(lua_State* L) {
		return 0;
	}

	int set_projectile_json(lua_State* L) {
		return 0;
	}
	

	map<string, int(*)(lua_State*)> funcoes_ponte_map = {

		//input
		pair<string, int(*)(lua_State*)>("get_keyboard_input", funcoes_ponte::get_keyboard_input),
		pair<string, int(*)(lua_State*)>("get_mouse_input", funcoes_ponte::get_mouse_input),
		pair<string, int(*)(lua_State*)>("get_joystick_input", funcoes_ponte::get_joystick_input),
		pair<string, int(*)(lua_State*)>("set_keyboard_text_input", funcoes_ponte::set_keyboard_text_input),
		pair<string, int(*)(lua_State*)>("set_cursor_position", funcoes_ponte::set_cursor_position),

		
		

		//tempo
		pair<string, int(*)(lua_State*)>("get_time", funcoes_ponte::get_time),
		pair<string, int(*)(lua_State*)>("set_time_scale", funcoes_ponte::set_time_scale),

		//memoria
		pair<string, int(*)(lua_State*)>("clear_memory", funcoes_ponte::clear_memory),
		pair<string, int(*)(lua_State*)>("load_asset", funcoes_ponte::load_asset),
		pair<string, int(*)(lua_State*)>("asset_is_load", funcoes_ponte::asset_is_load),

		//assets
		pair<string, int(*)(lua_State*)>("get_tile_set_size", funcoes_ponte::get_tile_set_size),
		pair<string, int(*)(lua_State*)>("get_tile_set_tile", funcoes_ponte::get_tile_set_tile),
		pair<string, int(*)(lua_State*)>("get_tilemap_size", funcoes_ponte::get_tilemap_size),
		pair<string, int(*)(lua_State*)>("get_tilemap_layer_size", funcoes_ponte::get_tilemap_layer_size),
		pair<string, int(*)(lua_State*)>("get_tilemap_data", funcoes_ponte::get_tilemap_data),

		pair<string, int(*)(lua_State*)>("get_scene_3D_json", funcoes_ponte::get_scene_3D_json),
		

		//objeto
		pair<string, int(*)(lua_State*)>("create_object", funcoes_ponte::create_object),
		pair<string, int(*)(lua_State*)>("get_object_with_name", funcoes_ponte::get_object_with_name),
		pair<string, int(*)(lua_State*)>("remove_object", funcoes_ponte::remove_object),
		pair<string, int(*)(lua_State*)>("add_component", add_component),
		pair<string, int(*)(lua_State*)>("remove_component", remove_component),
		pair<string, int(*)(lua_State*)>("reset_components", funcoes_ponte::reset_components),
		pair<string, int(*)(lua_State*)>("have_component", have_component),

		

		pair<string, int(*)(lua_State*)>("get_object_family_json", funcoes_ponte::get_object_family_json),
		

		//movimento
		pair<string, int(*)(lua_State*)>("to_move", funcoes_ponte::to_move),
		pair<string, int(*)(lua_State*)>("in_ceiling", funcoes_ponte::in_ceiling),
		pair<string, int(*)(lua_State*)>("in_floor", funcoes_ponte::in_floor),
		pair<string, int(*)(lua_State*)>("get_gravity", funcoes_ponte::get_gravity),
		pair<string, int(*)(lua_State*)>("set_gravity", funcoes_ponte::set_gravity),

		//transform
		pair<string, int(*)(lua_State*)>("get_transform_json", funcoes_ponte::get_transform_json),
		pair<string, int(*)(lua_State*)>("set_transform_json", funcoes_ponte::set_transform_json),

		pair<string, int(*)(lua_State*)>("move_transform", funcoes_ponte::move_transform),
		pair<string, int(*)(lua_State*)>("rotate_transform", funcoes_ponte::rotate_transform),

		pair<string, int(*)(lua_State*)>("change_transfotm_position", funcoes_ponte::change_transfotm_position),
		pair<string, int(*)(lua_State*)>("change_transfotm_rotation", funcoes_ponte::change_transfotm_rotation),
		pair<string, int(*)(lua_State*)>("change_transfotm_scale", funcoes_ponte::change_transfotm_scale),
		
	
	
		
		

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
		pair<string, int(*)(lua_State*)>("get_post_processing_json", funcoes_ponte::get_post_processing_json),
		pair<string, int(*)(lua_State*)>("set_post_processing_json", funcoes_ponte::set_post_processing_json),

		//camadas render
		pair<string, int(*)(lua_State*)>("get_render_layer_instruction_json", funcoes_ponte::get_render_layer_instruction_json),
		pair<string, int(*)(lua_State*)>("set_render_layer_instruction_json", funcoes_ponte::set_render_layer_instruction_json),
		

		//janela
		pair<string, int(*)(lua_State*)>("set_window", funcoes_ponte::set_window),
		pair<string, int(*)(lua_State*)>("get_window", funcoes_ponte::get_window),

		//sprite
		pair<string, int(*)(lua_State*)>("get_sprite_render_json", funcoes_ponte::get_sprite_render_json),
		pair<string, int(*)(lua_State*)>("set_sprite_render_json", funcoes_ponte::set_sprite_render_json),
		

		//render tilemap
		pair<string, int(*)(lua_State*)>("get_render_tilemap_json", funcoes_ponte::get_render_tilemap_json),
		pair<string, int(*)(lua_State*)>("set_render_tilemap_json", funcoes_ponte::set_render_tilemap_json),
		


		//text
		pair<string, int(*)(lua_State*)>("set_font", funcoes_ponte::set_font),
		pair<string, int(*)(lua_State*)>("get_font", funcoes_ponte::get_font),
		pair<string, int(*)(lua_State*)>("set_text", funcoes_ponte::set_text),
		pair<string, int(*)(lua_State*)>("get_text", funcoes_ponte::get_text),

		pair<string, int(*)(lua_State*)>("get_text_json", funcoes_ponte::get_text_json),
		pair<string, int(*)(lua_State*)>("set_text_json", funcoes_ponte::set_text_json),
		

		//physic
		pair<string, int(*)(lua_State*)>("add_force", funcoes_ponte::add_force),
		pair<string, int(*)(lua_State*)>("get_physic_2D_json", funcoes_ponte::get_physic_2D_json),
		pair<string, int(*)(lua_State*)>("set_physic_2D_json", funcoes_ponte::set_physic_2D_json),
		

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

		//mesh

		pair<string, int(*)(lua_State*)>("get_mesh_json", funcoes_ponte::get_mesh_json),
		pair<string, int(*)(lua_State*)>("set_mesh_json", funcoes_ponte::set_mesh_json),

		//projetil
			pair<string, int(*)(lua_State*)>("get_projectile_json", funcoes_ponte::get_projectile_json),
			pair<string, int(*)(lua_State*)>("get_projectile_json", funcoes_ponte::get_projectile_json),
			
			
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
				//lua_register(L, "get_input", funcoes_ponte::get_input);
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
			if (scripts_lua_iniciados[p.first]) {
				lua_State* L = p.second;
				lua_getglobal(L, "UPDATE");
				lua_call(L, 0, 0);
			}
			else {
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

	void chamar_funcao(string script, string func) {
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
		else if (string(lua_tostring(L, 2)).compare("projectile") == 0) {
			obj->adicionar_componente<projetil>();
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
		else if (string(lua_tostring(L, 2)).compare("projectile") == 0) {
			obj->remover_componente<projetil>();
		}

	}


	return 0;
}

int get_component_size(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	int ret = 0;
	if (argumentos == 1) {
		ret = obj->pegar_lista_componentes().size();
	}

	lua_pushnumber(L, ret);
	return 1;
}



int have_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	bool ret = false;
	if (argumentos == 2) {
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
		else if (string(lua_tostring(L, 2)).compare("projectile") == 0) {
			obj->tem_componente<projetil>();
		}
	}

	lua_pushboolean(L, ret);
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
		cl->adicionar_script(lua_tostring(L, 2));
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
		else if (lua_tonumber(L, 4) == 1) {
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

int call_script_function(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 3 && cl != NULL) {
		cl->chamar_funcao(lua_tostring(L, 2), lua_tostring(L, 3));
	}
	return 0;
}
