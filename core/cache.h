//
// Created by Feed on 6/25/2020.
//

#pragma once

#include "common.h"
#include "event.h"
#include "platform.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <list>

using json = nlohmann::json ;

class CacheBank {
public:
    struct Cache {
        Cache(std::string& n, std::filesystem::path& p) : name(std::move(n)), path(std::move(p)) {}
        const std::string name;
        const std::filesystem::path path;
        json data;
        friend class CacheBank;
    private:
        b32 orphaned = false;
    };
    Cache* fetchCache(std::string name);
    Cache* newCache(std::string name);
    void destroyCache(Cache* cache);
    b32 saveCache(Cache* cache);
private:
    //std::vector<Cache> m_caches;
    std::list<Cache> m_caches;
};
