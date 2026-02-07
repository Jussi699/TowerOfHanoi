#include "Particles.h"
#include "MainGame.h"
#include "Disk.h"
#include "Logger.h"
#include <system_error>

BaseClass base;

enum class Anchor {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER,
    BOTTOM_CENTER,
    UNDER_CENTER,
    BOTTOM_LEFT,
};

Vector2 GetAnchoredTextPos(
    const char* text,
    Font font,
    float fontSize,
    float spacing,
    Anchor anchor
) {
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
 
    base.screenWidth = GetMonitorWidth(0);
    base.screenHeight = GetMonitorHeight(0);

    const float marginX = base.screenWidth * 0.015f;
    const float marginY = base.screenHeight * 0.02f;

    switch (anchor) {
    case Anchor::TOP_LEFT:
        return { 15, 20 };
    case Anchor::TOP_CENTER:
        return { base.screenWidth / 2 - size.x / 2, 20 };
    case Anchor::TOP_RIGHT:
        return { base.screenWidth - size.x - 60, 25 };
    case Anchor::CENTER:
        return { base.screenWidth / 2 - size.x / 2 + 52, base.screenHeight / 2 - size.y / 2 - 120};
    case Anchor::UNDER_CENTER:
        return { base.screenWidth / 2 - size.x / 2 + 360, base.screenHeight / 2 - size.y / 2 + 60 };
    case Anchor::BOTTOM_CENTER:
        return { base.screenWidth / 2 - size.x / 2, base.screenHeight - size.y - 20 };
    case Anchor::BOTTOM_LEFT:
        return { marginX, base.screenHeight - size.y - marginY - 40 };
    }
    return { 0, 0 };
}

MainGame::MainGame()
{
    resultsSaved = false;
    stars = 1;
    showHelp = false;
    showLastResult = false;
    isFullscreen = false;
    lastResultCached = false;
    lastResultCache.clear();
    lastResultCacheTTLSeconds = 5;
}

std::string MainGame::LoadLastVictoryResult() {
    const std::filesystem::path p("Result/victory_results.txt");
    try {
        if (lastResultCached) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastResultCacheTime).count() < lastResultCacheTTLSeconds) {
                if (std::filesystem::exists(p)) {
                    try {
                        auto currentWriteTime = std::filesystem::last_write_time(p);
                        if (currentWriteTime == lastResultFileTime) {
                            return lastResultCache;
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        Logger::Log(Logger::Level::Warning, std::string("Could not get last_write_time: ") + e.what());
                    }
                }
                else {
                    lastResultCached = false;
                    lastResultCache.clear();
                    Logger::Log(Logger::Level::Info, "Victory results file deleted Ч cache invalidated");
                }
            }
            else {
                lastResultCached = false;
                Logger::Log(Logger::Level::Debug, "Victory results cache TTL expired Ч invalidating");
            }
        }

        std::string allResults;
        if (std::filesystem::exists(p)) {
            std::ifstream file(p);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    allResults += line + "\n";
                }
                file.close();

                try {
                    lastResultFileTime = std::filesystem::last_write_time(p);
                }
                catch (const std::filesystem::filesystem_error& e) {
                    Logger::Log(Logger::Level::Warning, std::string("Failed to read last_write_time for cache: ") + e.what());
                }
                lastResultCache = allResults.empty() ? "No previous results found" : allResults;
                lastResultCached = true;
                lastResultCacheTime = std::chrono::steady_clock::now();

                Logger::Log(Logger::Level::Info, "Loaded victory_results.txt into cache");
            }
            else {
                Logger::Log(Logger::Level::Error, "Unable to open Result/victory_results.txt for reading");
                allResults = "No previous results found";
            }
        }
        else {
            Logger::Log(Logger::Level::Info, "Result/victory_results.txt does not exist");
            allResults = "No previous results found";
        }

        return allResults;
    }
    catch (const std::exception& ex) {
        Logger::Log(Logger::Level::Error, std::string("Exception reading victory results: ") + ex.what());
        return std::string("No previous results found");
    }
}

void MainGame::SaveVictoryResults() {
    if (resultsSaved) return;

    try {
        std::filesystem::create_directories("Result");
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::Log(Logger::Level::Error, std::string("Failed to create Result directory: ") + e.what());
        return;
    }
    catch (const std::exception& e) {
        Logger::Log(Logger::Level::Error, std::string("Failed to create Result directory (std::exception): ") + e.what());
        return;
    }
    catch (...) {
        Logger::Log(Logger::Level::Error, "Failed to create Result directory (unknown error).");
        return;
    }

    const std::filesystem::path p("Result/victory_results.txt");
    std::ofstream file(p, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        Logger::Log(Logger::Level::Error, "It was not possible to open the file for recording results.");
        return;
    }

    file << "Victory achieved!\n";
    file << "Total Moves: " << Disks::moveCount << "\n";
    file << "Total Time: " << TextFormat("%.1f", Disks::timer) << " s\n";
    file << "Stars: " << stars << "\n";
    file.flush();

    bool testOk = Testing::TestSaveResult(file);

    file.close();
    resultsSaved = true;

    try {
        if (std::filesystem::exists(p)) {
            lastResultFileTime = std::filesystem::last_write_time(p);
            lastResultCached = false;
            lastResultCache.clear();
            LoadLastVictoryResult();
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::Log(Logger::Level::Warning, std::string("Failed to update cache file time after save: ") + e.what());
    }

    if (!testOk) {
        Logger::Log(Logger::Level::Warning, "TestSaveResult reported a problem.");
    }
    else {
        Logger::Log(Logger::Level::Info, "Victory results saved successfully.");
    }
}

void MainGame::ToggleFullscreen()
{
    isFullscreen = !isFullscreen;
    if (isFullscreen) {
        SetWindowState(FLAG_FULLSCREEN_MODE);
    }
    else {
        ClearWindowState(FLAG_FULLSCREEN_MODE);
        SetWindowSize(screenWidth, screenHeight);
    }
}

void MainGame::HandleGlobalHotkeys()
{
    if (IsKeyPressed(KEY_ONE)) { Disks::NUM_TOWERS = 4; difficulty = 1; Disks::InitDisks(); }
    if (IsKeyPressed(KEY_TWO)) { Disks::NUM_TOWERS = 3; difficulty = 2; Disks::InitDisks(); }

    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }

    if (IsKeyPressed(KEY_V)) { showLastResult = !showLastResult; }

    if (IsKeyPressed(KEY_Q)) {
        difficulty = 0;
    }

    if (IsKeyPressed(KEY_R)) {
        Disks::InitDisks();
    }

    if (IsKeyPressed(KEY_W)) {
        Disks::UndoMove();
    }
}

void MainGame::Menu() {
    DrawTextEx( font, "Choose the difficulty\n1 - Easy\n2 - Hard\nR - Help",
        GetAnchoredTextPos("Choose the difficulty\n1 - Easy\n2 - Hard\nR - Help", font, 40, 2, Anchor::CENTER), 40, 2, BLACK );
    
    HandleGlobalHotkeys();

    if (IsKeyPressed(KEY_R)) { showHelp = !showHelp; }
    if (showHelp) {
        DrawTextEx(font, help, GetAnchoredTextPos(help, font, 25, 2, Anchor::BOTTOM_LEFT), 25, 2, BLACK);
    }

    if (showLastResult) { DrawTextEx(font, LoadLastVictoryResult().c_str(), GetAnchoredTextPos("Last Result", font, 40, 2, Anchor::TOP_RIGHT), 30, 2, RED); }

    if (IsKeyPressed(KEY_M)) {
        Musics::musicEnabled = !Musics::musicEnabled;
        if (Musics::musicEnabled) {
            PlayMusicStream(Musics::music);
        }
        else {
            StopMusicStream(Musics::music);
        }
    }

    if (Musics::musicEnabled) {
        DrawTextEx(font, "Music: ON", GetAnchoredTextPos("Music: ON", font, 25, 2, Anchor::TOP_LEFT), 30, 5, LIME);
    }
    else {
        DrawTextEx(font, "Music: OFF", GetAnchoredTextPos("Music: OFF", font, 25, 2, Anchor::TOP_LEFT), 30, 5, RED);
    }
}

void MainGame::cVictory() {
    if (Disks::timer < 240.0f) stars = 3;
    else if (Disks::timer < 360.0f) stars = 2;
    else stars = 1;

    if (stars < 1) stars = 1;
    if (stars > 3) stars = 3;

    SaveVictoryResults();
    timerActive = false;
    float deltaTime = GetFrameTime();
    Vector2 diskPosition = { GetScreenWidth() / 2, GetScreenHeight() / 2 };


    //------------------------------------------------------RECTANGLE WIN (START)------------------------------------------------------//
    partic.UpdateParticles(deltaTime);
    DrawCircleV(diskPosition, 50, GOLD);
    partic.CreateVictoryParticles(diskPosition, 50);
    partic.DrawParticles();

    Rectangle panel = {
        GetScreenWidth() / 2.0f - 210,
        GetScreenHeight() / 2.0f - 130,
        420,
        260
    };

    DrawRectangleRec(panel, Fade(BLACK, 0.5f));
    DrawRectangleLinesEx(panel, 5, GOLD);

    Rectangle inner = {
        panel.x + 10,
        panel.y + 10,
        panel.width - 20,
        panel.height - 20
    };

    DrawRectangleRec(inner, RAYWHITE);
    //------------------------------------------------------RECTANGLE WIN (END)------------------------------------------------------//


	//------------------------------------------------------TEXT WIN (START)------------------------------------------------------//
    float startY = base.screenHeight * 0.4f; 
    float lineSpacing = 28.0f;

    DrawTextEx(font, "WIN!",
        { (base.screenWidth - 20)/ 2.0f - MeasureText("WIN!", 40) / 2.0f, startY }, 60, 2, GOLD);

    startY += lineSpacing + 20; // увеличенный отступ после заголовка

    DrawTextEx(font, TextFormat("Total Moves: %d", Disks::moveCount),
        { (base.screenWidth - 70) / 2.0f - MeasureText(TextFormat("Total Moves: %d", Disks::moveCount), 20) / 2.0f, startY }, 40, 2, RED);

    startY += lineSpacing;

    DrawTextEx(font, TextFormat("Total Time: %.1f s", Disks::timer),
        { (base.screenWidth - 55) / 2.0f - MeasureText(TextFormat("Total Time: %.1f s", Disks::timer), 20) / 2.0f, startY }, 40, 2, RED);

    startY += lineSpacing;

    DrawTextEx(font, TextFormat("Stars: %d", stars),
        { (base.screenWidth - 20) / 2.0f - MeasureText(TextFormat("Stars: %d", stars), 20) / 2.0f, startY }, 40, 2, RED);

    startY += lineSpacing + 10;

    DrawTextEx(font, "Press R to return to menu",
        { (base.screenWidth - 50) / 2.0f - MeasureText("Press R to return to menu", 20) / 2.0f, startY }, 35, 2, RED);


    float starOffsetX = GetScreenWidth() / 2 - (stars - 1) * 30;
    for (int i = 0; i < stars; i++) {
        partic.DrawStar({ starOffsetX + i * 60, (float)GetScreenHeight() / 2 - 150 }, 20, GOLD);
    }

    if (IsKeyPressed(KEY_R)) {
        difficulty = 0;
        resultsSaved = false;
    }
    //------------------------------------------------------TEXT WIN (END)------------------------------------------------------//
}

void MainGame::HotKeyAndText() {
    const float margin = GetScreenWidth() * 0.015f;
    const float textSize = 25.0f;
    const float titleSize = 35.0f;
    const float spacing = 3.0f;


    //------------------------------------------------------TOP LEFT PANEL(START)------------------------------------------------------//
    Rectangle statsPanel = { margin - 25, margin - 8, 170, 75 };
    DrawRectangleRec(statsPanel, Fade(BLACK, 0.5f));
    DrawTextEx( font, TextFormat("Moves: %d", Disks::moveCount), { statsPanel.x + 15, statsPanel.y + 8 }, textSize, spacing, WHITE );
    DrawTextEx( font, TextFormat("Time: %.2f s", Disks::timer), { statsPanel.x + 15, statsPanel.y + 38 }, textSize, spacing, WHITE );

    //------------------------------------------------------TOP LEFT PANEL(END)------------------------------------------------------//



    //------------------------------------------------------BUTTON TOP RIGHT PANEL(START)------------------------------------------------------//
    Rectangle backButton = { GetScreenWidth() - margin - 155, margin - 10, 165, 40 };
    DrawRectangleRec(backButton, Fade(BLACK, 0.5f));
    DrawTextEx( font, "Back to Menu", { backButton.x + 10, backButton.y + 9 }, textSize, spacing, WHITE );

    Rectangle restartButton = { backButton.x - margin - 155, margin - 10, 155, 40 };
    DrawRectangleRec(restartButton, Fade(BLACK, 0.5f));
    DrawTextEx( font, "Restart", { restartButton.x + 35, restartButton.y + 9 }, textSize, spacing, WHITE );
    //------------------------------------------------------BUTTON TOP RIGHT PANEL(END)------------------------------------------------------//


    //------------------------------------------------------RULES PANEL (START)------------------------------------------------------//

    Rectangle rulesPanel = { restartButton.x, restartButton.y + restartButton.height + margin - 20, 350, 300 };
    DrawRectangleRec(rulesPanel, Fade(BLACK, 0.5f));
    DrawTextEx(font, "Rules:", { rulesPanel.x + 10, rulesPanel.y + 10 }, titleSize, spacing, WHITE );

    DrawTextEx(
        font,
        "R - restart\n"
        "W - undo last move\n"
        "Q - back to menu\n"
        "M - turn music\n"
        "A - rotate left\n"
        "D - rotate right\n"
        "Z - reset camera\n"
        "F11 - FullScreen\n"
        "Hold RMB - rotate camera",
        { rulesPanel.x + 10, rulesPanel.y + 45 },
        textSize,
        spacing,
        WHITE
    );
    //------------------------------------------------------RULES PANEL (END)------------------------------------------------------//

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, backButton)) {
            difficulty = 0;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, restartButton)) {
            Disks::InitDisks();
        }
    }

    HandleGlobalHotkeys();

    if (IsKeyPressed(KEY_M)) {
        Musics::musicEnabled = !Musics::musicEnabled;
        if (Musics::musicEnabled) {
            PlayMusicStream(Musics::music);
        }
        else {
            StopMusicStream(Musics::music);
        }
    }
}

void MainGame::UpdateCameraControls() {
    static Vector2 previousMousePosition = { 0.0f, 0.0f };
    float rotationSpeed = 1.5f;
    static float yaw = 0.0f;

    if (yaw > 360.0f) yaw -= 360.0f; 
    if (yaw < -360.0f) yaw += 360.0f;

    static float pitch = 0.0f;
    static float radiuss = 15.0f;

    camera.position = { 0.0f, 1.0f, radiuss };

    float scroll = GetMouseWheelMove();
    radiuss -= scroll * 2.0f;
	if (radiuss < 5.0f) radiuss = 5.0f; // Min. distance
	if (radiuss > 50.0f) radiuss = 50.0f; // Max. distance

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouseDelta = Vector2Subtract(GetMousePosition(), previousMousePosition);
        yaw += mouseDelta.x * 0.1f;
        pitch -= mouseDelta.y * 0.1f;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -30.0f) pitch = -30.0f;
    }

    if (IsKeyDown(KEY_A)) {
        yaw -= rotationSpeed;
    }
    if (IsKeyDown(KEY_D)) {
        yaw += rotationSpeed;
    }
    if (IsKeyPressed(KEY_Z)) {
        yaw = 0.0f;
        pitch = 0.0f;
        radiuss = 15.0f;
    }

    float radius = Vector3Length(Vector3Subtract(camera.position, camera.target));
    camera.position.x = camera.target.x + radius * cosf(DEG2RAD * pitch) * sinf(DEG2RAD * yaw);
    camera.position.y = camera.target.y + 10.0f + radius * sinf(DEG2RAD * pitch);
    camera.position.z = camera.target.z + radius * cosf(DEG2RAD * pitch) * cosf(DEG2RAD * yaw);
    previousMousePosition = GetMousePosition();
}