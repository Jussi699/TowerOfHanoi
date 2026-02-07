#pragma once

#ifndef MAINGAME_H
#define MAINGAME_H

#include "Particles.h"
#include "BaseClass.h"
#include "Music.h"
#include "Test.h"
#include "raylib.h"
#include <filesystem>
#include <string>
#include "fstream"
#include <chrono>

class MainGame : public BaseClass, public Musics
{
public:
    MainGame();

    void Menu();
    void cVictory();
    void HotKeyAndText();
    void UpdateCameraControls();
    void HandleGlobalHotkeys();
    void ToggleFullscreen();

    bool resultsSaved;
    Font font;

private:
    void SaveVictoryResults(); 
    std::string LoadLastVictoryResult();

    Rectangle lastResultButton;
    Vector2 diskPosition;
    Particles partic;

    std::filesystem::file_time_type lastResultFileTime{};
    std::chrono::steady_clock::time_point lastResultCacheTime{};
    int lastResultCacheTTLSeconds = 5;

    int stars = 1;
    const char* help =
        "1. Move discs to build a tower on another platform.\n"
        "2. A smaller disk cannot be put on a larger.\n"
        "3. Click F11 to switch fullscreen mode.\n"
        "4. Press M to toggle music.\n"
        "5. Press V to view last result.\n"
        "6. Press ESC to close the game.\n"
        "7. During the game, you can switch modes using 1 and 2.";
    std::string lastResultCache;
    bool lastResultCached = false;

};
#endif // !MAINGAME_H