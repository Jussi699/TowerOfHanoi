#include "BaseClass.h"

Camera3D BaseClass::camera;
int BaseClass::difficulty = 0;
int BaseClass::NUM_DISKS = 7;
int BaseClass::NUM_TOWERS = 3;
bool BaseClass::timerActive = false;
int BaseClass::moveCount = 0;
float BaseClass::timer = 0.0f;


void BaseClass::InitsCamera() {
    camera.position = { 0.0f, 8.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };      
    camera.fovy = 50.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

