#pragma once

#ifndef DISKSTRUCT_H
#define DISKSTRUCT_H

#include "raylib.h"
#include <vector>

struct Disk {
    Vector3 position;
    float radius = 0.0f;
    Color color = { 255,255,255,255 };
    Vector3 targetPosition;
    bool isMoving = false;
    bool isSelected = false;
    bool testMoveDisk = false;
    int platformIndex = 0;
};

struct Move {
    int from;
    int to;
};

#endif // !DISKSTRUCT_H