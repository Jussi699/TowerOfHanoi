#pragma once

#ifndef TEXTURES_H
#define TEXTURES_H

#include "Disk.h"
#include "Test.h"
#include "raylib.h"
#include <iostream>

class TextureManager : public Disks {
public:
	void LoadTextures();
	void UnloadTextures();
	void DrawTextures();

	Image icon;
	Texture2D space;              
	TextureCubemap spaceCubemap;
	Model spaceModel;
};

#endif
