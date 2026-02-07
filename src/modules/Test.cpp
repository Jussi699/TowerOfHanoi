#include "Test.h"
#include "Logger.h"
#include <string>

bool isSuccessfulyDrawDisk = false;

bool Testing::TestMoveDisk(Disk& d) {
    int diskNumber = 0;

    if (d.radius > 1.85f) diskNumber = 1;
    else if (d.radius > 1.55f) diskNumber = 2;
    else if (d.radius > 1.25f) diskNumber = 3;
    else if (d.radius > 0.95f) diskNumber = 4;
    else if (d.radius > 0.65f) diskNumber = 5;
    else if (d.radius > 0.35f) diskNumber = 6;
    else diskNumber = 7;

    std::string colorStr = "Color: (R: " + std::to_string((int)d.color.r) + ", G: " + std::to_string((int)d.color.g) + ", B: " + std::to_string((int)d.color.b) + ")";

    if (d.isMoving) {
        std::string msg = "Disk " + std::to_string(diskNumber) + " is moving! Platform: " + std::to_string(d.platformIndex) + " " + colorStr;
        Logger::Log(Logger::Level::Info, msg);
        return true;
    }
    else {
        std::string msg = "Disk " + std::to_string(diskNumber) + " is not moving! " + colorStr;
        Logger::Log(Logger::Level::Debug, msg);
        return false;
    }
}

bool Testing::TestDrawTexture(Texture2D& texture)
{
    if (texture.id == 0) {
        Logger::Log(Logger::Level::Error, "TestDrawTexture = Completed with an error! texture.id == 0");
        return false;
    }
    else {
        Logger::Log(Logger::Level::Info, "TestDrawTexture = Successfully completed!");
        return true;
    }
}

bool Testing::TestDrawDisk(const std::vector<std::vector<Disk>>& towers) {
    bool allDisksValid = true;

    for (size_t i = 0; i < towers.size(); ++i) {
        for (size_t j = 0; j < towers[i].size(); ++j) {
            const Disk& disk = towers[i][j];

            if (disk.radius <= 0) {
                Logger::Log(Logger::Level::Warning, "Disk on tower " + std::to_string(i) + " at position " + std::to_string(j) + " has invalid radius!");
                allDisksValid = false;
            }

            if (disk.position.y < 0) {
                Logger::Log(Logger::Level::Warning, "Disk on tower " + std::to_string(i) + " at position " + std::to_string(j) + " has invalid position!");
                allDisksValid = false;
            }

            if (disk.color.a == 0) {
                Logger::Log(Logger::Level::Warning, "Disk on tower " + std::to_string(i) + " at position " + std::to_string(j) + " has invalid color (transparent)!");
                allDisksValid = false;
            }
        }
    }

    if (!isSuccessfulyDrawDisk) {
        if (allDisksValid) {
            Logger::Log(Logger::Level::Info, "TestDrawDisk = Successfully completed!");
        }
        isSuccessfulyDrawDisk = true;
    }

    return allDisksValid;
}

bool Testing::TestSaveResult(const std::ofstream& f)
{
    static bool alreadyLogged = false;
    if (alreadyLogged) return f.good();

    if (f.good())
    {
        Logger::Log(Logger::Level::Info, "TestSaveResult = Successfully completed!");
        alreadyLogged = true;
        return true;
    }
    else if (f.bad()) {
        Logger::Log(Logger::Level::Error, "TestSaveResult = Completed with an error!");
        alreadyLogged = true;
        return false;
    }
    else {
        Logger::Log(Logger::Level::Warning, "TestSaveResult = Unknown stream state (neither good nor bad)");
    }
    return false;
}


