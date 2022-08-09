#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "RecursosT.h"



class sfml_audio_api_classe : public API_audio_classe {
public:
    sfml_audio_api_classe() {}
    
    mapeamento_ponteiros<string, sf::SoundBuffer> mapeamento_audio_buffer;
    
    
    vector<string> audio_byffers;
    
    

    
    map<audio_info* , sf::Sound*> audios_infos_map;
    


    void alterar(audio_info* info) {
        if (mapeamento_audio_buffer.pegar(info->nome) != NULL) {
            audios_infos_map[info]->setBuffer(*mapeamento_audio_buffer.pegar(info->nome));
            audios_infos_map[info]->setPlayingOffset(sf::seconds(info->tempo));
            audios_infos_map[info]->setVolume(info->volume);
            audios_infos_map[info]->setLoop(info->loop);
            audios_infos_map[info]->setPitch(info->velocidade * (int)!info->pausa);

            audios_infos_map[info]->play();

            info->aplicar = false;
        }
    }

    



    void adicionar(audio_info* info) {
        sf::SoundBuffer* bufferSom = mapeamento_audio_buffer.pegar(info->nome);
        if (bufferSom == NULL) {
            bufferSom = new sf::SoundBuffer();
            if (bufferSom->loadFromFile(info->nome)) {
                mapeamento_audio_buffer.aplicar(info->nome, bufferSom);
                audio_byffers.push_back(info->nome);
                
            }
            else {
                cout << "erro ao carregar arquivo de som " << info->nome << endl;
                delete bufferSom;
    
            }
        }
    
        //https://www.sfml-dev.org/tutorials/2.5/audio-sounds.php
        if (bufferSom != NULL) {
            sf::Sound* sound = new sf::Sound();
            audios_infos_map.insert({ info , sound });
            alterar(info);
            
        }
    
        int id = map_vetor(audios_ids).size();
        if (audios_ids.find(info) == audios_ids.end()) {
            audios_ids.insert(pair<audio_info*, int>(info, id));
        }
        
    
    
    }
    void remover(audio_info* info) {
        audios_ids.erase(info);
        delete audios_infos_map[info];
        audios_infos_map.erase(info);
    }
    void atualizar(audio_info* info) {
        if (audios_ids.find(info) == audios_ids.end()) {
            adicionar(info);
        }
    }

    void LoopAudio() {
    }

    void IniciarLoopAudio() {
        while (true) { LoopAudio(); }
    }



};






