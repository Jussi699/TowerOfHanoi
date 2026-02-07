#include "Music.h"
#include "Logger.h"

bool Musics::musicEnabled = false;
Music Musics::music;

void Musics::InitMusic() {
    InitAudioDevice();  
    music = LoadMusicStream("src/music/Game.mp3");
    if (music.stream.buffer == NULL) {
        musicEnabled = false;
        Logger::Log(Logger::Level::Error, "Music error! Check the path to the file.");
    }
    else {
        musicEnabled = false;
    }
    SetMusicVolume(music, 0.1f / 2);
}

void Musics::UpdateMusic() {
    if (IsAudioDeviceReady()) {
        UpdateMusicStream(music);
    }
    else {
        Logger::Log(Logger::Level::Warning, "The audio device is inactive. Music cannot be updated.");
    }
}

void Musics::CloseMusic() {
    if (music.stream.buffer != NULL) {
        UnloadMusicStream(music);
    }
    CloseAudioDevice();
}