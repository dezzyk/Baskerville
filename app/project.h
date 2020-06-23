//
// Created by Feed on 6/20/2020.
//

#pragma once

#include "common.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

using json = nlohmann::json ;

namespace Project {
    void startup();
    void shutdown();
    void open();
    void save();
    std::string getCurProjectPath();
    std::string getLastLine();
    void pushLine(std::string line);
    void completeParagraph();
}