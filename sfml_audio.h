#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "RecursosT.h"
#include "Objetos.h"


mapeamento_assets<sf::SoundBuffer> buffers_som_sfml;

shared_ptr<sf::SoundBuffer> carregar_audio_buffer_sfml(string local) {
	if (buffers_som_sfml.pegar(local) == NULL) {
		buffers_som_sfml.aplicar(local, sf::SoundBuffer());
		buffers_som_sfml.pegar(local)->loadFromFile(local);
	}
	return buffers_som_sfml.pegar(local);
}

void carregar_audio_buffer_thread(string local, shared_ptr<sf::SoundBuffer>* ret) {
	*ret = carregar_audio_buffer_sfml(local);
}


//"recursos\\Som\\teste de audio.wav"
class sfml_audio : public componente {
public:

	

	

	

	audio_info info;
	sf::Sound som;
	shared_ptr<sf::SoundBuffer> buffer;

	void aplicar_info() {
		carregar_audio_buffer_sfml(info.nome);
		buffer = buffers_som_sfml.pegar(info.nome);
		som.setBuffer(*buffer);
		
		
		som.setPlayingOffset(sf::seconds(info.tempo));
		som.setVolume(info.volume);
		som.setLoop(info.loop);
		som.setPitch(info.velocidade);
		if (info.pausa) {
			som.pause();
		}
		else
		{
			som.play();
		}
		
	}


	audio_info pegar_info() {
		audio_info ret;

		ret.tempo = som.getPlayingOffset().asSeconds();
		ret.volume =som.getVolume();
		ret.loop =som.getLoop();
		ret.velocidade = som.getPitch();
		

		return ret;
	}


	int iniciado = 0;
	void iniciar() {
		aplicar_info();
		
	}

	

	sfml_audio() {
		
	}
	sfml_audio(audio_info info) {
		this->info = info;
	}

	


	

	
	

	void finalisar() {
		cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbb\n";
		som.pause();
		
	}

	
};













