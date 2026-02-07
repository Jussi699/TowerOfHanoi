#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include "raylib.h"

class Musics {
public:
	static Music music;
	static bool musicEnabled;

	void InitMusic();
	void UpdateMusic();
	void CloseMusic();

};

#endif // !MUSIC_H
