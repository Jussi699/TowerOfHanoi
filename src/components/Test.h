#pragma once

#ifndef TEST_H
#define TEST_H

#include <raylib.h>
#include <iostream>
#include "DiskStruct.h"
#include <fstream>

class Testing
{
public:
    static bool TestMoveDisk(Disk& d);
    static bool TestDrawTexture(Texture2D& t);
    static bool TestDrawDisk(const std::vector<std::vector<Disk>>& towers);
    static bool TestSaveResult(const std::ofstream& f);
};

#endif // TEST_H
