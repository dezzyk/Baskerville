//
// Created by Feed on 6/20/2020.
//

#include "project.h"
#include "platform.h"

#include "nfd/nfd.h"
#include "flatbuffers/generated/project_serialize.h"

#include <iostream>

Project::Project() {

    m_cache_path = Platform::getPrefPath();
    m_cache_path += "project.dat";
    if(std::filesystem::exists(m_cache_path)) {
        b32 res = false;
        Platform::cautionOptionBox("Baskerville", "A previous session did no close correctly. Would you like to restore the cached project data?", res);
        if(res) {
            /*std::vector<u8> buffer;
            std::ifstream input;
            input.open(m_cache_path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();

            auto project_data = ProjectSemantics::GetProject(buffer.data());
            m_version_year = project_data->version_year();
            m_version_major = project_data->version_major();
            m_version_minor = project_data->version_minor();
            auto paragraphs = project_data->paragraphs();
            for(int i = 0; i < paragraphs->size(); ++i) {
                auto lines = paragraphs->Get(i)->lines();
                Paragraph new_paragraph;
                new_paragraph.lines.resize(lines->size());
                for(int i = 0; i < lines->size(); ++i) {
                    new_paragraph.lines[i] = lines->Get(i)->str();
                }
            }*/
        }
    } else {
        m_paragraphs.push_back({});
    }

}

Project::Project(Project&& other) noexcept {

}

Project& Project::operator=(Project&& other) {
    return *this;
}

Project::~Project() {

}

b32 Project::open() {

    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "spt", NULL, &outPath );

    if (result == NFD_OKAY ) {
        std::filesystem::path path = outPath;
        free(outPath);
        if(std::filesystem::exists(path)) { // Redundant but doesnt hurt
            /*m_paragraphs = {};

            std::vector<u8> buffer;
            std::ifstream input;
            input.open(path, std::ios::binary);
            input.seekg(0, input.end);
            u32 length = input.tellg();
            input.seekg(0, input.beg);
            buffer.resize(length);
            input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            input.close();

            auto project_data = ProjectSemantics::GetProject(buffer.data());
            m_version_year = project_data->version_year();
            m_version_major = project_data->version_major();
            m_version_minor = project_data->version_minor();
            auto paragraphs = project_data->paragraphs();
            for(int i = 0; i < paragraphs->size(); ++i) {
                auto lines = paragraphs->Get(i)->lines();
                Paragraph new_paragraph;
                new_paragraph.lines.resize(lines->size());
                for(int i = 0; i < lines->size(); ++i) {
                    new_paragraph.lines[i] = lines->Get(i)->str();
                }
                m_paragraphs.push_back(std::move(new_paragraph));
            }
            return true;*/

        }

    }
    else if ( result != NFD_CANCEL ) {
        printf("Error: %s\n", NFD_GetError() );
    }
    return false;
}

b32 Project::save() {

    nfdchar_t* savePath = NULL;
    nfdresult_t result = NFD_SaveDialog("spt", NULL, &savePath);
    if (result == NFD_OKAY) {
        /*std::filesystem::path path = savePath;
        if(path.filename().extension() != ".spt") {
            path += ".spt";
        }
        free(savePath);

        flatbuffers::FlatBufferBuilder builder;
        std::vector<flatbuffers::Offset<ProjectSemantics::Paragraph>> paragraphs_vector;
        for(int i = 0; i < m_paragraphs.size(); ++i) {
            std::vector<flatbuffers::Offset<flatbuffers::String>> lines;
            for(int j = 0; j < m_paragraphs[i].lines.size(); ++j) {
                lines.push_back(builder.CreateString(m_paragraphs[i].lines[j]));
            }
            auto l = builder.CreateVector(lines);
            auto p = ProjectSemantics::CreateParagraph(builder, l);
            paragraphs_vector.push_back(p);
        }
        auto name = builder.CreateString("Test"); // TEMP)
        auto b_paragraphs = builder.CreateVector(paragraphs_vector);
        auto finalized = ProjectSemantics::CreateProject(builder, 0, 0, 0, name, b_paragraphs);
        builder.Finish(finalized);

        u8 *buf = builder.GetBufferPointer();
        int size = builder.GetSize();

        std::ofstream save_file(path, std::ios::out | std::ios::binary);
        save_file.write(reinterpret_cast<char*>(buf), size);
        save_file.flush();

        return true;*/
    } else if (result == NFD_CANCEL) {
        std::cout <<"User pressed cancel." << std::endl;
    } else {
        printf("Error: %s\n", NFD_GetError());
    }
    return false;
}

//Fuck docx
b32 Project::exportToTXT() {
    /*const std::vector<json> &paragraphs = m_project_cache->data["data"]["paragraphs"];
    if (!paragraphs.empty()) {
        nfdchar_t* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("txt", NULL, &savePath);
        if (result == NFD_OKAY) {
            std::filesystem::path tar_path = savePath;
            if (tar_path.filename().extension() != ".txt") {
                tar_path += ".txt";
            }
            free(savePath);
            if (std::filesystem::exists(tar_path)) {
                std::filesystem::remove(tar_path);
            }
            std::ofstream export_file(tar_path);
            if (!export_file || !export_file.is_open() || !export_file.good()) {
                std::cout << "Failed to open cache file" << std::endl;
                return false;
            } else {
                for (const auto &p : paragraphs) {
                    std::string total = "";
                    const std::vector<std::string> &lines = p["lines"];
                    if (!lines.empty()) {
                        for (const auto &line : lines) {
                            total += line;
                        }
                    }
                    if(total != "") {
                        export_file << "\t" << total << std::endl;
                    }
                }
            }
        }
    }*/
    return true;
}

std::string Project::getLastLine() {
    if(!m_paragraphs.back().lines.empty()) {
        return m_paragraphs.back().lines.back();
    }
    return "";
}

void Project::pushLine(std::string line) {
    /*for(int i = 0; i < line.size(); ++i) {
        if(line[i] == ' ') {
            if(i != 0) {
                if(line[i-1] != ' ') {
                    word_count++;
                }
            }
        }
    }*/
    m_paragraphs.back().lines.push_back(line);
}

void Project::completeParagraph() {
    m_paragraphs.push_back({});
}
