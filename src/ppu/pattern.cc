//
// Created by huhua on 2021/6/7.
//

#include "pattern.h"
#include <string>

olc::Pixel &Pattern::getTable(int which) {
    if (which != 0 && which != 2) {
        throw std::runtime_error("Pattern only have 2 table but you give " + std::string(which));
    }

    // Now I need initial the table
    return <#initializer#>;
}
