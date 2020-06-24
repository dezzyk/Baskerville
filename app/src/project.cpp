//
// Created by Feed on 6/20/2020.
//

#include "project.h"
#include "platform.h"

#include "nfd/nfd.h"
#include "SDL2/SDL.h"

#include <iostream>

namespace {

    json data;
    std::filesystem::path proj_path;
    b32 loaded = false;
    std::vector<u8> buffer;

}

void Project::startup() {
    proj_path = SDL_GetPrefPath("Saltpowered", "Baskerville");
    if(!proj_path.empty()) {
        proj_path += "project.dat";
        if (std::filesystem::exists(proj_path)) {
            b32 res = false;
            Platform::cautionOptionBox("Baskerville", "A previous session was not closed cleanly. Would you like to save the cached data?", res);
            if(res) {
                std::ifstream input;
                input.open(proj_path, std::ios::binary);
                input.seekg(0, input.end);
                u32 length = input.tellg();
                input.seekg(0, input.beg);
                buffer.resize(length);
                input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
                input.close();
                data = json::from_cbor(buffer);
                save();
            } else {
                data["path"] = std::string();
                data["data"]["paragraphs"] = std::vector<json>();
                std::ofstream cache_file = std::ofstream(proj_path,
                                                         std::ios::out | std::ios::binary | std::ios::trunc);
                if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
                    std::cout << "Failed to open cache file" << std::endl;
                } else {
                    buffer = json::to_cbor(data);
                    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
                    cache_file.flush();
                    if (!cache_file.good()) {
                        std::cout << "Failed to write to cache file" << std::endl;
                    }
                }
            }
        } else {
            data["path"] = std::string();
            data["data"]["paragraphs"] = std::vector<json>();
            std::ofstream cache_file = std::ofstream(proj_path,
                                                     std::ios::out | std::ios::binary | std::ios::trunc);
            if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
                std::cout << "Failed to open cache file" << std::endl;
            } else {
                buffer = json::to_cbor(data);
                cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
                cache_file.flush();
                if (!cache_file.good()) {
                    std::cout << "Failed to write to cache file" << std::endl;
                }
            }
        }
    }
}

void Project::shutdown() {
    if(!proj_path.empty()) {
        std::filesystem::remove(proj_path);
    }
}

void Project::open() {

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "spt", NULL, &outPath );

    if ( result == NFD_OKAY ) {
        std::filesystem::path path = outPath;
        free(outPath);
        if(std::filesystem::exists(path)) { // Redundant but doesnt hurt
            std::ifstream input;
            input.open(path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();
            json project_data = json::from_cbor(buffer);

            data["path"] = path.string();
            data["data"] = project_data;
            buffer = json::to_cbor(data);
            std::ofstream cache_file = std::ofstream(proj_path, std::ios::out | std::ios::binary | std::ios::trunc);
            cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            cache_file.flush();
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

    std::string path = data["path"];
    if(!path.empty()) {
        std::ofstream save_file(path, std::ios::out | std::ios::binary);
        buffer = json::to_cbor(data["data"]);
        save_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
        save_file.flush();
    } else {
        nfdchar_t* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("spt", NULL, &savePath);
        if (result == NFD_OKAY) {
            std::filesystem::path path = savePath;
            path += ".spt";
            free(savePath);
            data["path"] = path.string();
            std::ofstream save_file(path, std::ios::out | std::ios::binary);
            buffer = json::to_cbor(data["data"]);
            // Remember to write to both so that cache is updated with the new path.
            save_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            save_file.flush();
            buffer = json::to_cbor(data);
            std::ofstream cache_file = std::ofstream(proj_path,
                                                     std::ios::out | std::ios::binary | std::ios::trunc);
            cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            cache_file.flush();
        } else if (result == NFD_CANCEL) {
            std::cout <<"User pressed cancel." << std::endl;
        } else {
            printf("Error: %s\n", NFD_GetError());
        }
    }
}

std::string Project::getCurProjectPath() {
    return data["path"];
}

std::string Project::getLastLine() {
    const std::vector<json>& paragraphs = data["data"]["paragraphs"];
    if(!paragraphs.empty()) {
        const std::vector<std::string>& lines = paragraphs[paragraphs.size() - 1]["lines"];
        if(!lines.empty()) {
            return lines[lines.size() - 1];
        }
    }
    return "";
}

void Project::pushLine(std::string line) {
    const std::vector<json>& paragraphs = data["data"]["paragraphs"];
    if(!paragraphs.empty()) {
        data["data"]["paragraphs"][paragraphs.size() - 1]["lines"].push_back(line);
    } else {
        data["data"]["paragraphs"][0]["lines"].push_back(line);
    }
    buffer = json::to_cbor(data);
    std::ofstream cache_file = std::ofstream(proj_path, std::ios::out | std::ios::binary | std::ios::trunc);
    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    cache_file.flush();
}

void Project::completeParagraph() {
    data["data"]["paragraphs"].push_back(json());
    buffer = json::to_cbor(data);
    std::ofstream cache_file = std::ofstream(proj_path, std::ios::out | std::ios::binary | std::ios::trunc);
    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    cache_file.flush();
}
