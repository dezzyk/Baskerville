//
// Created by Feed on 6/20/2020.
//

#include "project.h"

#include <iostream>

namespace {

    json data;
    //std::filesystem::path open_path;
    //std::ofstream cache_file;
    b32 loaded = false;
    std::vector<u8> buffer;

}

void Project::startup() {
    if(!std::filesystem::exists("cache")) {
        std::filesystem::create_directory("cache/");
    }
    if(std::filesystem::exists("cache/project.dat")) {
        std::ifstream input;
        //std::vector<u8> buffer;
        input.open("cache/project.dat", std::ios::binary);
        input.seekg(0, input.end);
        u32 length = input.tellg();
        input.seekg(0, input.beg);
        buffer.resize(length);
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        input.close();
        data = json::from_cbor(buffer);
        //cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    } else {
        data["path"] = "";
        data["data"]["paragraphs"] = std::vector<json>();
        //cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    }
    std::ofstream cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    if(!cache_file || !cache_file.is_open() || !cache_file.good()) {
        std::cout << "Failed to open cache file" << std::endl;
    } else {
        buffer = json::to_cbor(data);
        cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
        cache_file.flush();
        if(!cache_file.good()) {
            std::cout << "Failed to write to cache file" << std::endl;
        }
    }
}

void Project::shutdown() {

}

void Project::open(const std::filesystem::path& path) {
    if(std::filesystem::exists(path)) {
        std::ifstream input;
        //std::vector<u8> buffer;
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
        std::ofstream cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
        cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
        cache_file.flush();
    }
}

void Project::save(const std::filesystem::path &path) {
    data["path"] = path.string();
    std::ofstream file(path, std::ios::out | std::ios::binary);
    buffer = json::to_cbor(data["data"]);
    // Remember to write to both so that cache is updated with the new path.
    file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    file.flush();
    buffer = json::to_cbor(data);
    std::ofstream cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    cache_file.flush();
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
    std::ofstream cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    cache_file.flush();
}

void Project::completeParagraph() {
    data["data"]["paragraphs"].push_back(json());
    buffer = json::to_cbor(data);
    std::ofstream cache_file = std::ofstream("cache/project.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    cache_file.flush();
}
