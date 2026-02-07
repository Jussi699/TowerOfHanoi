#pragma once

#ifndef BASECLASS_H
#define BASECLASS_H

#include "raylib.h"
#include "raymath.h"
#include <vector>

class BaseClass {
public:
	void InitsCamera();

	 int screenWidth = GetScreenWidth();
	 int screenHeight = GetScreenHeight();
	
	 static int difficulty;
	 static int moveCount;
	 static float timer;
	 static bool timerActive;
	 float deltaTime;

	bool showLastResult;
	bool showHelp;
	bool isFullscreen;

	static Camera3D camera;

protected:
	static int NUM_DISKS;
	static int NUM_TOWERS;
};

#endif // !BASECLASS_H
