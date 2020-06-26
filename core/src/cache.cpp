//
// Created by Feed on 6/25/2020.
//

#include "cache.h"

#include <iostream>

CacheBank::Cache* CacheBank::fetchCache(std::string name) {
    Cache* new_cache = nullptr;
    static std::vector<u8> buffer;
    for(auto& cache : m_caches) {
        if(cache.name == name) {
            if(!cache.orphaned) {
                return &cache;
            } else {
                new_cache = &cache;
            }
        }
    }
    std::filesystem::path path = Platform::getPrefPath();
    path += name;
    path += ".spt";
    if(std::filesystem::exists(path)) {
        if(new_cache == nullptr) {
            m_caches.push_back({name, path});
            new_cache = &m_caches.back();
        } else {
            new_cache->orphaned = false;
        }

        std::ifstream input;
        input.open(new_cache->path, std::ios::binary);
        input.seekg(0, input.end);
        u32 length = input.tellg();
        input.seekg(0, input.beg);
        buffer.resize(length);
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        input.close();

        new_cache->data = json::from_cbor(buffer);
        return new_cache;
    } /*else {
        std::ofstream cache_file = std::ofstream(path, std::ios::binary | std::ios::trunc);
        if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
            std::cout << "Failed to create cache " << name << std::endl;
        } else {
            m_caches.push_back({name, path});
            Cache& new_cache = m_caches.back();
            return &new_cache;
        }
    }*/
    return nullptr;
}

CacheBank::Cache* CacheBank::newCache(std::string name) {
    for(auto& cache : m_caches) {
        if(cache.name == name) {
            cache.data = json();
            cache.orphaned = false;
            std::ofstream cache_file = std::ofstream(cache.path, std::ios::binary | std::ios::trunc);
            if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
                std::cout << "Failed to create cache " << cache.name << std::endl;
            }
            return &cache;
        }
    }
    std::filesystem::path path = Platform::getPrefPath();
    path += name;
    path += ".spt";
    std::ofstream cache_file = std::ofstream(path, std::ios::binary | std::ios::trunc);
    if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
        std::cout << "Failed to create cache " << name << std::endl;
    } else {
        m_caches.push_back({name, path});
        Cache& new_cache = m_caches.back();
        return &new_cache;
    }
    return nullptr;
}

void CacheBank::destroyCache(Cache* cache) {
    if(cache != nullptr) {
        cache->orphaned = true;
        std::filesystem::remove(cache->path);
        m_caches.remove_if([&](const Cache& c) { return c.orphaned; });
    }
}

b32 CacheBank::saveCache(Cache* cache) {
    if(cache != nullptr) {
        static std::vector<u8> buffer;
        std::ofstream cache_file = std::ofstream(cache->path,
                                                 std::ios::binary | std::ios::trunc);
        if (!cache_file || !cache_file.is_open() || !cache_file.good()) {
            std::cout << "Failed to open cache file" << std::endl;
            return false;
        } else {
            buffer = json::to_cbor(cache->data);
            cache_file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            cache_file.flush();
            if (!cache_file.good()) {
                std::cout << "Failed to write to cache file" << std::endl;
                return false;
            }
        }
        return true;
    }
    return false;
}