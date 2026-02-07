#include "Particles.h"
#include "Music.h"
#include "Disk.h"
#include "MainGame.h"
#include "BaseClass.h"
#include "Textures.h"

void Initialize(MainGame& maingame, Disks& disks, BaseClass& baseclass,
    TextureManager& textureMG, Musics& music)
{
    InitWindow(baseclass.screenWidth, baseclass.screenHeight, "Hanoi tower");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    maingame.font = LoadFont("src/font/Snowline.otf");
    music.InitMusic();
   
    SetWindowIcon(textureMG.icon);

    disks.InitDisks();
    disks.InitsCamera();
    textureMG.LoadTextures();
}

void UpdateGame(MainGame& maingame, Disks& disks, TextureManager& textureMG,
    BaseClass& baseclass, Musics& music)
{
    music.UpdateMusic();
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (baseclass.difficulty == 0) {
        maingame.Menu();
    }
    else {
        if (disks.timerActive) 
            disks.timer += GetFrameTime();

        maingame.UpdateCameraControls();
        disks.Disc();
        textureMG.DrawTextures();

        EndMode3D();

        if (disks.CheckVictory()) {
            maingame.cVictory();
        }

        maingame.HotKeyAndText();
    }

    EndDrawing();
}

void Shutdown(TextureManager& textureMG, Musics& music, MainGame& maingame) {
    if (maingame.font.texture.id != 0) {
        UnloadFont(maingame.font);
    }

    music.CloseMusic();
    textureMG.UnloadTextures();
    CloseWindow();
}

int main() {
    MainGame maingame;
    Disks disks;
    Particles partic;
    BaseClass baseclass;
    TextureManager textureMG;
    Musics music;

    Initialize(maingame, disks, baseclass, textureMG, music);

    while (!WindowShouldClose()) {
        UpdateGame(maingame, disks, textureMG, baseclass, music);
    }

    Shutdown(textureMG, music, maingame);
    return 0;
}