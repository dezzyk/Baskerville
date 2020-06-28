//
// Created by Feed on 6/20/2020.
//

#pragma once

#include "common.h"
#include "cache.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

using json = nlohmann::json ;

class Project {
public:
    Project();
    Project(CacheBank& cache_bank);
    Project(Project&& other) noexcept;
    Project& operator=(Project&& data);
    ~Project();
    Project(const Project&) = delete;
    Project& operator=(const Project& data) = delete;
    b32 open();
    b32 save();
    b32 exportToTXT();
    std::string getLastLine();
    void pushLine(std::string line);
    void completeParagraph();
private:
    CacheBank* m_cache_bank = nullptr;
    CacheBank::Cache* m_app_cache = nullptr;
    CacheBank::Cache* m_project_cache = nullptr;
};