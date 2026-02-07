#include "Disk.h"
#include "Particles.h"
#include "MainGame.h"
#include "Test.h"

bool Disks::victoryScreen = false;
int Disks::selectedTower = -1;
Disk* Disks::selectedDisk = nullptr;
std::vector<Move> Disks::moveHistory;
std::vector<std::vector<Disk>> Disks::towers(3);
Color Disks::rainbowColors[7] = { RED, ORANGE, YELLOW, GREEN, BLUE, DARKBLUE, PURPLE };
Vector3 Disks::lightDirection = { -1.0f, -1.0f, -1.0f };
Color Disks::lightColor = WHITE;

Disks::Disks() { Disks::InitDisks(); }

void Disks::InitDisks() {
    towers.clear();
    towers.resize(NUM_TOWERS);
    moveHistory.clear();

    for (int i = 0; i < NUM_DISKS; i++) {
        Disk disk = {
            {-4.0f, 1.0f + i * 0.5f, 0.0f},
            2.0f - i * 0.3f,
            rainbowColors[i % 7],
            {-4.0f, 1.0f + i * 0.5f, 0.0f},
            false, false,
            false,
            0
        };
        towers[0].push_back(disk);
    }
    moveCount = 0;
    timer = 0.0f;
    timerActive = true;

    Testing::TestDrawDisk(towers);
}

void Disks::Disc() {
    BeginMode3D(camera);
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Ray ray = GetScreenToWorldRay(mousePos, camera);

        for (int i = 0; i < NUM_TOWERS; i++) {
            BoundingBox towerBox = { { -4.0f + i * 4.0f - 1.0f, 0.0f, -1.0f },
                                     { -4.0f + i * 4.0f + 1.0f, 5.0f, 1.0f } };

            if (GetRayCollisionBox(ray, towerBox).hit) {
                if (selectedDisk) {
                    selectedDisk->isSelected = false;
                }
                if (!towers[i].empty() && selectedDisk == nullptr) {
                    selectedTower = i;
                    selectedDisk = &towers[i].back();
                    selectedDisk->isSelected = true;
                }
                else if (selectedDisk) {
                    MoveDisk(selectedTower, i);
                    selectedDisk->isSelected = false;
                    selectedDisk = nullptr;
                }
                break;
            }
        }
    }
}


void Disks::UpdateDiskPosition(Disk& disk) {
    if (!disk.isMoving) return;

    Vector3 direction = Vector3Subtract(disk.targetPosition, disk.position);
    float distance = Vector3Length(direction);
    if (distance < 0.02f) {
        disk.position = disk.targetPosition;
        disk.isMoving = false;
        disk.testMoveDisk = false;
    }
    else {
        disk.position = Vector3Add(disk.position, Vector3Scale(direction, 0.2f));
        if (!disk.testMoveDisk) {
            Testing::TestMoveDisk(disk);
            disk.testMoveDisk = true;
        }
    }
}

void Disks::DrawDiskWithOutline(Disk& disk) {
    Vector3 normal = { 0.0f, 1.0f, 0.0f };
    Vector3 lightDirNormalized = Vector3Normalize(lightDirection);

    float dotProduct = Vector3DotProduct(normal, lightDirNormalized);
    float intensity = Clamp(dotProduct, 0.0f, 1.0f);

    Color shadedColor = {
     (unsigned char)Clamp(disk.color.r * (0.85f + 0.85f * intensity), 0, 255),
     (unsigned char)Clamp(disk.color.g * (0.85f + 0.85f * intensity), 0, 255),
     (unsigned char)Clamp(disk.color.b * (0.85f + 0.85f * intensity), 0, 255),
     disk.color.a
    };

    DrawCylinder(disk.position, disk.radius, disk.radius, 0.5f, 35, shadedColor);

    if (disk.isSelected) {
        DrawCylinderWires(disk.position, disk.radius, disk.radius, 0.5f, 35, BLACK);
    }
}

void Disks::MoveDisk(int from, int to) {
    if (CanMoveDisk(from, to)) {
        Disk disk = towers[from].back();
        towers[from].pop_back();
        disk.targetPosition = { -4.0f + to * 4.0f, 1.0f + towers[to].size() * 0.5f, 0.0f };
        disk.isMoving = true;
        disk.platformIndex = to;
        towers[to].push_back(disk);
        moveCount++;
        moveHistory.push_back({ from, to });
    }
}

bool Disks::CanMoveDisk(int from, int to) {
    if (from < 0 || from >= NUM_TOWERS || to < 0 || to >= NUM_TOWERS) return false; 
    if (towers[from].empty()) return false;
    if (towers[to].empty()) return true;
    return towers[from].back().radius < towers[to].back().radius;
}

void Disks::UndoMove() {
    if (!moveHistory.empty()) {
        Move lastMove = moveHistory.back();
        moveHistory.pop_back();

        if (!towers[lastMove.to].empty()) {
            Disk disk = towers[lastMove.to].back();
            towers[lastMove.to].pop_back();
            disk.targetPosition = { -4.0f + lastMove.from * 4.0f, 1.0f + towers[lastMove.from].size() * 0.5f, 0.0f };
            disk.isMoving = true;
            disk.platformIndex = lastMove.from;
            towers[lastMove.from].push_back(disk);
            moveCount--;
        }
    }
}

bool Disks::CheckVictory() {
    for (int i = 1; i < NUM_TOWERS; i++) {
        if (towers[i].size() == NUM_DISKS) {
            for (size_t j = 1; j < towers[i].size(); j++) {
                if (towers[i][j].radius > towers[i][j - 1].radius) return false;
            }
            victoryScreen = true;
            timerActive = false;

            return true;
        }
    }

    if (NUM_TOWERS == 4 && towers[3].size() == NUM_DISKS) {
        for (int j = 1; j < towers[3].size(); j++) {
            if (towers[3][j].radius > towers[3][j - 1].radius) return false;
        }
        victoryScreen = true;
        timerActive = false;


        return true;
    }
    return false;
}
