//
// Created by Feed on 6/20/2020.
//

#include "project.h"
#include "platform.h"
#include "data_path.h"
#include "duckx/duckx.hpp"

#include "nfd/nfd.h"
#include "SDL2/SDL.h"

#include <iostream>

Project::Project() {}

Project::Project(CacheBank& cache_bank) {
    m_cache_bank = &cache_bank;
    m_app_cache = m_cache_bank->fetchCache("app");
    if(m_app_cache == nullptr) {
        m_app_cache = m_cache_bank->newCache("app");
        if(m_app_cache) {
            m_app_cache->data["project_path"] = "";
            m_app_cache->data["last_word_count"] = 0;
            m_cache_bank->saveCache(m_app_cache);
        }
    }
    m_project_cache = m_cache_bank->fetchCache("project");
    if(m_project_cache != nullptr) {
        std::vector<u8> buffer;
        b32 res = false;
        Platform::cautionOptionBox("Baskerville", "A previous session was not closed cleanly. Would you like to save the cached data?", res);
        if(res) {
            std::ifstream input;
            input.open(m_project_cache->path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();
            m_project_cache->data = json::from_cbor(buffer);
            m_app_cache->data["project_path"] = m_project_cache->data["project_path"];
            m_app_cache->data["last_word_count"] = m_project_cache->data["data"]["word_count"];
            m_cache_bank->saveCache(m_app_cache);
            save();
        } else {
            m_cache_bank->destroyCache(m_project_cache);
            m_project_cache = m_cache_bank->newCache("project");
            m_project_cache->data["project_path"] = m_app_cache->data["project_path"];

            std::string path = m_project_cache->data["project_path"];
            if(!path.empty()) {
                std::ifstream input;
                input.open(path, std::ios::binary);
                input.seekg(0, input.end);
                u32 length = input.tellg();
                input.seekg(0, input.beg);
                buffer.resize(length);
                input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
                input.close();
                m_project_cache->data["data"] = json::from_cbor(buffer);
                m_app_cache->data["last_word_count"] = m_project_cache->data["data"]["word_count"];
                m_cache_bank->saveCache(m_project_cache);
            }

            //m_project_cache->data["data"]["word_count"] = 0;
            //m_project_cache->data["data"]["paragraphs"] = std::vector<json>();
            //m_cache_bank->saveCache(m_project_cache);

            //m_app_cache->data["last_project_path"] = "";
            //m_cache_bank->saveCache(m_app_cache);
        }
    } else {
        std::string path = m_app_cache->data["project_path"];
        if(!path.empty()) {
            std::vector<u8> buffer;
            std::ifstream input;
            input.open(path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();

            m_project_cache = m_cache_bank->newCache("project");
            m_project_cache->data["project_path"] = path;
            m_project_cache->data["data"] = json::from_cbor(buffer);
            m_cache_bank->saveCache(m_project_cache);
        } else {
            m_project_cache = m_cache_bank->newCache("project");
            m_project_cache->data["project_path"] = "";
            m_project_cache->data["data"]["word_count"] = 0;
            m_project_cache->data["data"]["paragraphs"] = std::vector<json>();
            m_cache_bank->saveCache(m_project_cache);
        }
    }
}

Project::Project(Project&& other) noexcept :
            m_cache_bank(other.m_cache_bank),
            m_project_cache(other.m_project_cache),
            m_app_cache(other.m_app_cache)
{
    other.m_cache_bank = nullptr;
    other.m_app_cache = nullptr;
    other.m_project_cache = nullptr;
}

Project& Project::operator=(Project&& other) {
    if(this != &other) {
        m_cache_bank = other.m_cache_bank;
        m_project_cache = other.m_project_cache;
        m_app_cache = other.m_app_cache;
        other.m_cache_bank = nullptr;
        other.m_app_cache = nullptr;
        other.m_project_cache = nullptr;
    }
    return *this;
}

Project::~Project() {
    if(m_cache_bank && m_project_cache) {
        if(m_app_cache->data["last_word_count"] < m_project_cache->data["data"]["word_count"]) {
            b32 res = false;
            Platform::cautionOptionBox("Baskerville", "You have unsaved changes. Would you like to save them?", res);
            if(res) {
                save();
            }
        }
        m_cache_bank->destroyCache(m_project_cache);
    }
}

void Project::open() {

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "spt", NULL, &outPath );

    if (result == NFD_OKAY ) {
        std::filesystem::path path = outPath;
        free(outPath);
        if(std::filesystem::exists(path)) { // Redundant but doesnt hurt
            static std::vector<u8> buffer;
            std::ifstream input;
            input.open(path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();

            m_cache_bank->destroyCache(m_project_cache);
            m_project_cache = m_cache_bank->newCache("project");
            m_project_cache->data["project_path"] = path.string();
            m_project_cache->data["data"] = json::from_cbor(buffer);
            m_cache_bank->saveCache(m_project_cache);
            m_app_cache->data["project_path"] = path.string();
            m_app_cache->data["last_word_count"] = m_project_cache->data["data"]["word_count"];
            m_cache_bank->saveCache(m_app_cache);
        }

    }
    else if ( result == NFD_CANCEL ) {
        std::cout <<"User pressed cancel." << std::endl;
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }

}

void Project::save() {

    // Yes this is ugly, but there is a bug where empty strings in the json
    // will still return a size of 1 no matter what if the return is checked directly
    // ( m_app_cache->data["project_path"].empty() )
    // Need to submit an issue to the json++ repo
    std::string app_project_path = m_app_cache->data["project_path"];
    std::string project_project_path = m_project_cache->data["project_path"];
    static std::vector<u8> buffer;
    if(!app_project_path.empty() && app_project_path == project_project_path) {
        std::ofstream save_file(project_project_path, std::ios::out | std::ios::binary);
        buffer = json::to_cbor(m_project_cache->data["data"]);
        save_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
        save_file.flush();
        m_app_cache->data["last_word_count"] = m_project_cache->data["data"]["word_count"];
        m_cache_bank->saveCache(m_app_cache);
    } else {
        nfdchar_t* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("spt", NULL, &savePath);
        if (result == NFD_OKAY) {
            std::filesystem::path path = savePath;
            if(path.filename().extension() != ".spt") {
                path += ".spt";
            }
            free(savePath);
            m_project_cache->data["project_path"] = path.string();
            m_cache_bank->saveCache(m_project_cache);
            m_app_cache->data["project_path"] = path.string();
            m_app_cache->data["last_word_count"] = m_project_cache->data["data"]["word_count"];
            m_cache_bank->saveCache(m_app_cache);
            std::ofstream save_file(path, std::ios::out | std::ios::binary);
            buffer = json::to_cbor(m_project_cache->data["data"]);
            save_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            save_file.flush();
        } else if (result == NFD_CANCEL) {
            std::cout <<"User pressed cancel." << std::endl;
        } else {
            printf("Error: %s\n", NFD_GetError());
        }
    }

}

void Project::exportToDocx() {
    nfdchar_t* savePath = NULL;
    nfdresult_t result = NFD_SaveDialog("docx", NULL, &savePath);
    if (result == NFD_OKAY) {
        std::filesystem::path path = savePath;
        if (path.filename().extension() != ".docx") {
            path += ".docx";
        }
        free(savePath);
        std::string template_path = global_data_path;
        template_path += "template.docx";
        std::filesystem::copy(template_path, path);
        std::filesystem::rename(path, template_path);
        duckx::Document new_export(path.string());
        new_export.paragraphs().insert_paragraph_after(" Test 1 ");
        new_export.paragraphs().add_run(" Test 2 ");
        new_export.paragraphs().insert_paragraph_after(" Test 3 ");
        new_export.save();
    }
}

std::string Project::getLastLine() {
    const std::vector<json>& paragraphs = m_project_cache->data["data"]["paragraphs"];
    if(!paragraphs.empty()) {
        const std::vector<std::string>& lines = paragraphs[paragraphs.size() - 1]["lines"];
        if(!lines.empty()) {
            return lines[lines.size() - 1];
        }
    }
    return "";
}

void Project::pushLine(std::string line) {
    int word_count = m_project_cache->data["data"]["word_count"];
    for(auto& c : line) {
        if(c == ' ') {
            word_count++;
        }
    }
    m_project_cache->data["data"]["word_count"] = word_count;
    static std::vector<u8> buffer;
    const std::vector<json>& paragraphs = m_project_cache->data["data"]["paragraphs"];
    if(!paragraphs.empty()) {
        m_project_cache->data["data"]["paragraphs"][paragraphs.size() - 1]["lines"].push_back(line);
    } else {
        m_project_cache->data["data"]["paragraphs"][0]["lines"].push_back(line);
    }
    m_cache_bank->saveCache(m_project_cache);
}

void Project::completeParagraph() {
    m_project_cache->data["data"]["paragraphs"].push_back(json());
    m_cache_bank->saveCache(m_project_cache);
}
