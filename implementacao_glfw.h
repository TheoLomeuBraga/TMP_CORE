#include "RecursosT.h"
#include "input.h"
#include "LoopPrincipal.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>


GLFWwindow* janela ;

bool interromper_loop_input = false;

namespace mouse {






	static void pos_cursor(GLFWwindow* window, double xpos, double ypos)
	{
		double px, py;
		int rx, ry;



		// glfwSetCursorPos(win, 0, 0);





		glfwGetCursorPos(window, &px, &py);



		glfwGetWindowSize(janela, &rx, &ry);

		Mouse.cursorPos[0] = (float)px / (float)rx;
		Mouse.cursorPos[1] = (float)py / (float)ry;


		//cout << cursorPosX << "   " << cursorPosY << endl;
	}

	static void botoes_mouse(GLFWwindow* window, int button, int action, int mods)
	{



		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				Mouse.botao[0] = true;
			}
			if (action == GLFW_RELEASE) {
				Mouse.botao[0] = false;
			}


		}


		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				Mouse.botao[1] = true;
			}
			if (action == GLFW_RELEASE) {
				Mouse.botao[1] = false;
			}


		}



		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (action == GLFW_PRESS) {
				Mouse.botao[2] = true;
			}
			if (action == GLFW_RELEASE) {
				Mouse.botao[2] = false;
			}


		}






	}

	static void scrol_rot(GLFWwindow* window, double xoffset, double yoffset) {
		Mouse.scrolRot += yoffset;
	}










}

namespace teclas {


	//glfwGetKey(window, GLFW_KEY_ESCAPE )

	//https://www.glfw.org/docs/3.3/group__keys.html

	void botoes_teclado(GLFWwindow* window, int key, int scancode, int action, int mods) {
		//.teclas["w"] =
		switch (key)
		{
			//
		case GLFW_KEY_BACKSPACE:
			if (action == GLFW_PRESS && Teclado.input_texto.size() != 0) { Teclado.input_texto.pop_back(); }
			break;
		case GLFW_KEY_W:
			if (action == GLFW_PRESS) { Teclado.teclas["w"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["w"] = false; }
			break;

		case GLFW_KEY_A:
			if (action == GLFW_PRESS) { Teclado.teclas["a"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["a"] = false; }
			break;

		case GLFW_KEY_S:
			if (action == GLFW_PRESS) { Teclado.teclas["s"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["s"] = false; }
			break;

		case GLFW_KEY_D:
			if (action == GLFW_PRESS) { Teclado.teclas["d"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["d"] = false; }
			break;

			//

		case GLFW_KEY_R:
			if (action == GLFW_PRESS) { Teclado.teclas["r"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["r"] = false; }
			break;

		case GLFW_KEY_C:
			if (action == GLFW_PRESS) { Teclado.teclas["c"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["c"] = false; }
			break;

		case GLFW_KEY_Q:
			if (action == GLFW_PRESS) { Teclado.teclas["q"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["q"] = false; }
			break;

		case GLFW_KEY_E:
			if (action == GLFW_PRESS) { Teclado.teclas["e"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["e"] = false; }
			break;

		case GLFW_KEY_N:
			if (action == GLFW_PRESS) { Teclado.teclas["n"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["n"] = false; }
			break;

		case GLFW_KEY_M:
			if (action == GLFW_PRESS) { Teclado.teclas["m"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["m"] = false; }
			break;

		case GLFW_KEY_F:
			if (action == GLFW_PRESS) { Teclado.teclas["f"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["f"] = false; }
			break;

		case GLFW_KEY_X:
			if (action == GLFW_PRESS) { Teclado.teclas["x"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["x"] = false; }
			break;

		case GLFW_KEY_T:
			if (action == GLFW_PRESS) { Teclado.teclas["t"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["t"] = false; }
			break;

		case GLFW_KEY_G:
			if (action == GLFW_PRESS) { Teclado.teclas["g"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["g"] = false; }
			break;

		case GLFW_KEY_Z:
			if (action == GLFW_PRESS) { Teclado.teclas["z"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["z"] = false; }
			break;

			//

		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS) { Teclado.teclas["space"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["space"] = false; }
			break;

		case GLFW_KEY_LEFT_SHIFT:
			if (action == GLFW_PRESS) { Teclado.teclas["shift"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["shift"] = false; }
			break;

		case GLFW_KEY_LEFT_CONTROL:
			if (action == GLFW_PRESS) { Teclado.teclas["ctrl"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["ctrl"] = false; }
			break;

		case GLFW_KEY_LEFT_ALT:
			if (action == GLFW_PRESS) { Teclado.teclas["alt"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["alt"] = false; }
			break;

		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) { Teclado.teclas["esc"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["esc"] = false; }
			break;

		case GLFW_KEY_TAB:
			if (action == GLFW_PRESS) { Teclado.teclas["tab"] = true; }
			if (action == GLFW_RELEASE) { Teclado.teclas["tab"] = false; }
			break;










		}












	}

	void input_texto(GLFWwindow* window, unsigned  int codepoint) {
		if (Teclado.pegar_input_texto) {
			Teclado.input_texto.push_back((char)codepoint);
		}
		else
		{
			Teclado.input_texto = "";
		}
	}

}

namespace controle {
	//https://www.glfw.org/docs/3.3/input_guide.html
	//https://www.glfw.org/docs/3.3/group__joysticks.html










	void joystick_loop() {
		GLFWgamepadstate state;

		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			//botoes
			if (state.buttons[GLFW_GAMEPAD_BUTTON_A]) { Joystick[1].botoes[0] = true; }
			else { Joystick[1].botoes[0] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_B]) { Joystick[1].botoes[1] = true; }
			else { Joystick[1].botoes[1] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_X]) { Joystick[1].botoes[2] = true; }
			else { Joystick[1].botoes[2] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_Y]) { Joystick[1].botoes[3] = true; }
			else { Joystick[1].botoes[3] = false; }

			if (state.buttons[GLFW_GAMEPAD_BUTTON_START]) { Joystick[1].botoes[8] = true; }
			else { Joystick[1].botoes[8] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK]) { Joystick[1].botoes[9] = true; }
			else { Joystick[1].botoes[9] = false; }

			if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) { Joystick[1].botoes[4] = true; }
			else { Joystick[1].botoes[4] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]) { Joystick[1].botoes[5] = true; }
			else { Joystick[1].botoes[5] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) { Joystick[1].botoes[6] = true; }
			else { Joystick[1].botoes[6] = false; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]) { Joystick[1].botoes[7] = true; }
			else { Joystick[1].botoes[7] = false; }


			//d-pad input.D_PAD
			Joystick[1].D_PAD = vec2(0, 0);

			if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) { Joystick[1].D_PAD.y = 1; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) { Joystick[1].D_PAD.y = -1; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]) { Joystick[1].D_PAD.x = 1; }
			if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]) { Joystick[1].D_PAD.x = -1; }

			//eixos
			Joystick[1].analogio[0] = vec2(0, 0);
			Joystick[1].analogio[0].x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
			Joystick[1].analogio[0].y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

			Joystick[1].analogio[1] = vec2(0, 0);
			Joystick[1].analogio[1].x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
			Joystick[1].analogio[1].y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

			Joystick[1].gatilhos[0] = (1 + state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]) / 2;
			Joystick[1].gatilhos[1] = (1 + state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]) / 2;

			vibrar_controle(1, Joystick[1].VIBRAR);











		}






	}




}

void loopInput() {


	while (interromper_loop_input){}


	controle::joystick_loop();

	Tempo::varInputTemp = Tempo::tempo - Tempo::tempUltFrame;
	Tempo::tempUltFrame = Tempo::tempo;


}




void inicioInput() {

	while (janela == NULL){}



	glfwSetCursorPosCallback(janela, mouse::pos_cursor);
	glfwSetScrollCallback(janela, mouse::scrol_rot);


	glfwSetMouseButtonCallback(janela, mouse::botoes_mouse);
	glfwSetKeyCallback(janela, teclas::botoes_teclado);
	glfwSetCharCallback(janela, teclas::input_texto);

	cout << "Inputs Online" << endl;



	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(janela, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}






	loopInput();









}




class glfw_input_manager : public input_manager {
public:
	glfw_input_manager() { }
	void loop() {
		inicioInput();
		escrever("glfw_input_manager online");
			while (configuracoes::desligar == false) {
				loopInput();
			}
	}
};


//janela

bool janela_inteira = false;

void checarEstencoesGL() {
	GLenum err = glewInit();
	if (err != GLEW_OK)
		exit(1); // or handle the error in a nicer way
	if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
		exit(1); // or handle the error in a nicer way
}

void mudar_logo_janela(shared_ptr<imagem> img) {
	if (img != NULL) {
		GLFWimage icones[2];
		icones[0].height = img->res.x;
		icones[0].width = img->res.y;
		icones[0].pixels = &img->data[0];
		icones[1].height = img->res.x;
		icones[1].width = img->res.y;
		icones[1].pixels = &img->data[0];
		glfwSetWindowIcon(janela, 2, icones);
	}
}


void mudar_posicao_cursor(float x, float y) {
	glfwSetCursorPos(janela, x, y);
}


void mudar_imagem_cursor(shared_ptr<imagem> img) {
	if (img == NULL) {
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		GLFWimage image;
		image.width = img->res.x;
		image.height = img->res.y;
		image.pixels = &img->data[0];


		glfwSetCursor(janela, glfwCreateCursor(&image, 0, 0));
	}
}





vector<int> a;




void AntesReindenizar() {
	for (function<void()> f : Iniciar_Render_Func) {
		f();
	}






}


bool iniciada_logica_scripts;

void Reindenizar() {

	

	for (function<void()> f : Antes_Render_Func) {
		f();
	}


	
	

	Objetos::cena_objetos_selecionados->atualisar();
	Objetos::cena_objetos_selecionados->atualisar_transforms();
	Objetos::cena_objetos_selecionados->atualisar_Logica_Scripst();

	
	
	
	
	reindenizar_cenario();
	


	for (function<void()> f : Depois_Render_Func) {
		f();
	}
}




// Janela


void MudarTamanhoJanela(GLFWwindow* janela, int x, int y)
{
	//mudar_render_res(x, y);
}


void MudarRes(int x, int y) {

	//mudar_render_res(x, y);
	glfwSetWindowSize(janela, x, y);
	//configuracoes::janelaConfig.X = x;
	//configuracoes::janelaConfig.Y = y;
	cout << "tamanho da janela mudado para: " << x << " " << y <<endl;
	int a, b;
	glfwGetWindowSize(janela, &a, &b);
	cout << "resolução da janela " << a << " " << b << endl;


}


bool janelaInteira = false;

void IniciarJanela() {

	
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		cout << "falha en iniciar o GLFW\n";
	}

	



	

	glfwWindowHint(GLFW_SAMPLES, configuracoes::janelaConfig.antiCerrilhado); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 



	if (configuracoes::janelaConfig.transparente == true) {
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	}
	//icone

	GLFWimage* icon[1];





	// Open a window and create its OpenGL context

	if (janelaInteira) {
		//janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, configuracoes::janelaConfig.nome, glfwGetPrimaryMonitor(), NULL);
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(pegar_local_aplicacao()).c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else {
		//janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, configuracoes::janelaConfig.nome, NULL, NULL);
		janela = glfwCreateWindow(configuracoes::janelaConfig.X, configuracoes::janelaConfig.Y, pegar_nome_arquivo(pegar_local_aplicacao()).c_str(), NULL, NULL);
		
	}



	if (janela == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();

	}
	glfwMakeContextCurrent(janela); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "falha en iniciar o GLFW\n");
		cout << "falha en iniciar o GLFW\n";

	}


	glfwSetWindowSizeCallback(janela, MudarTamanhoJanela);
	//icone   load_icon("ico.png");  

	mudar_logo_janela(ManuseioDados::carregar_Imagem("resources/ico.png"));

	AntesReindenizar();

	cout << "Graficos Online" << endl;


	glfwSetInputMode(janela, GLFW_STICKY_KEYS, GL_TRUE);
	
	


}

void loop_janela() {
	
	//tempo
	Tempo::varTempRender = Tempo::tempUltFrameRender - Tempo::tempo;
	Tempo::varTempRender = Tempo::tempo - Tempo::tempUltFrameRender;
	Tempo::tempUltFrameRender = Tempo::tempo;
	Tempo::FPS = 1 / Tempo::varTempRender;
	






	glfwPollEvents();




	Reindenizar();









	glfwPollEvents();


	// Swap buffers
	glfwSwapBuffers(janela);
	glfwPollEvents();
	
	

	

	
}

class gerenciador_janela_glfw : public gerenciador_janela {
public:
	gerenciador_janela_glfw(bool tela_inteira) { janela_inteira = tela_inteira; }
	bool esta_janela_inteira(){return !glfwGetWindowAttrib(janela, GLFW_MAXIMIZED);}
	void mudar_cursor(shared_ptr<imagem> cursor) { mudar_imagem_cursor(cursor); }
	void mudar_imagem_janela(shared_ptr<imagem> janela) { mudar_logo_janela(janela); }
	void mudar_pos_cursor(float pos_x, float pos_y) { mudar_posicao_cursor(pos_x, pos_y); }
	void mudar_res(float res_x, float res_y) { MudarRes(res_x, res_y); }
	bool e_janela_cheia() { return janela_inteira; }
	vec2 pegar_res() {
		int x, y;
		glfwGetWindowSize(janela, &x, &y);
		//cout << "resolução da janela " << x << " " << y << endl;
		return vec2(x, y);
	}
	
	void loop() {
		fechar = glfwWindowShouldClose(janela) == 1;
		loop_janela();
	}
	void iniciar() {
		interromper_loop_input = false;
		IniciarJanela();
	}
	void encerrar() {
		interromper_loop_input = true;
		this_thread::sleep_for(chrono::milliseconds(100));
		glfwTerminate();
	}
	ivec2 pos_janela;
	void setar_tela_inteira_como(bool tela_cheia) {
		vec2 res = pegar_res();
		
		if (!janelaInteira) {
			glfwGetWindowPos(janela, &pos_janela.x, &pos_janela.y);
		}
		

		janelaInteira = tela_cheia;
		
		
		if (janelaInteira) {
			glfwSetWindowMonitor(janela, glfwGetPrimaryMonitor(), 500,500, res.x, res.y, 0);
		}
		else {
			glfwSetWindowMonitor(janela, NULL, pos_janela.x, pos_janela.y, res.x, res.y, 0);
		}

		
	}
};



