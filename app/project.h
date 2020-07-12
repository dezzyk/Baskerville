//
// Created by Feed on 6/20/2020.
//

#pragma once

#include "common.h"
#include "cache.h"

#include "nlohmann/json.hpp"
#include "flatbuffers/flatbuffers.h"

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

using json = nlohmann::json ;

class Project {
public:
    Project();
    Project(Project&& other) noexcept;
    Project& operator=(Project&& data);
    ~Project();
    Project(const Project&) = delete;
    Project& operator=(const Project& data) = delete;
    b32 open();
    b32 save();
    b32 exportToTXT();
    //std::string getLastLine();
    void pushLine(std::string line);
    std::string* getLineFromId(u32 id);
    void destroyLineFromId(u32 id);
    //void completeParagraph();
private:
    std::filesystem::path m_cache_path = "";
    std::vector<std::string> m_lines;

   //struct Paragraph {
        //std::vector<std::string> lines;
    //};
    //std::vector<Paragraph> m_paragraphs;
    u16 m_version_year = 0;
    u16 m_version_major = 0;
    u16 m_version_minor = 0;
};