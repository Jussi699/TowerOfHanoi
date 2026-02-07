#pragma once

#ifndef DISK_H
#define DISK_H

#include "Particles.h"
#include "BaseClass.h"
#include "DiskStruct.h"
#include <vector>

class Disks : public BaseClass
{
public:
    Disks();
    void Disc();
    void UpdateDiskPosition(Disk& disk);
    void DrawDiskWithOutline(Disk& disk);
    bool CanMoveDisk(int from, int to);
    void MoveDisk(int from, int to);
    bool CheckVictory();
    static void InitDisks();
    static void UndoMove();

protected:
    static int selectedTower;
    static bool victoryScreen;
    static bool victoryTested;
    static Disk* selectedDisk;
    static std::vector<Move> moveHistory;
    static std::vector<std::vector<Disk>> towers;
    static Color rainbowColors[7];
    static Vector3 lightDirection;
    static Color lightColor;
};

#endif // !DISK_H
