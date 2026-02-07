#include "Textures.h"
#include "rlgl.h"
#include "Logger.h"

void TextureManager::LoadTextures()
{
    space = LoadTexture("src/picture/space.jpg");
    if (space.id == 0) {
        Logger::Log(Logger::Level::Error, "Failed to load space texture: src/picture/space1.jpg");
    }
    else {
        spaceModel = LoadModelFromMesh(GenMeshSphere(1.0f, 128, 128));
        if (spaceModel.meshCount == 0) {
            Logger::Log(Logger::Level::Error, "Failed to create space model from mesh");
        }
        else {
            spaceModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = space;
            spaceModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
            Logger::Log(Logger::Level::Info, "Space texture and model loaded successfully");
        }
    }
    Testing::TestDrawTexture(space);
}

void TextureManager::UnloadTextures()
{
    if (space.id != 0) {
        UnloadTexture(space);
        Logger::Log(Logger::Level::Info, "UnloadTexture 'space' successfully.");
    }
    else {
        Logger::Log(Logger::Level::Warning, "Texture 'space' was not loaded or already unloaded.");
    }

    if (spaceModel.meshCount > 0) {
        UnloadModel(spaceModel);
        Logger::Log(Logger::Level::Info, "UnloadModel 'spaceModel' successfully.");
    }
    else {
        Logger::Log(Logger::Level::Warning, "Model 'spaceModel' was not loaded or already unloaded.");
    }
}

void TextureManager::DrawTextures()
{
    if (spaceModel.meshCount > 0) {
        rlDisableBackfaceCulling();
        DrawModel(spaceModel, camera.position, 150.0f, WHITE);
        rlEnableBackfaceCulling();
    }

    for (int i = 0; i < NUM_TOWERS; i++) {
        DrawCube({ -4.0f + i * 4.0f, 0.5f, 0.0f }, 2.0f, 1.0f, 2.0f, DARKGRAY);
        for (Disk& disk : towers[i]) {
            UpdateDiskPosition(disk);
            DrawDiskWithOutline(disk);
        }
    }
}



