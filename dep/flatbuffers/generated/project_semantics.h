// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PROJECT_PROJECTSEMANTICS_H_
#define FLATBUFFERS_GENERATED_PROJECT_PROJECTSEMANTICS_H_

#include "flatbuffers/flatbuffers.h"

namespace ProjectSemantics {

struct Paragraph;
struct ParagraphBuilder;

struct Project;
struct ProjectBuilder;

struct Paragraph FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ParagraphBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LINES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *lines() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_LINES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LINES) &&
           verifier.VerifyVector(lines()) &&
           verifier.VerifyVectorOfStrings(lines()) &&
           verifier.EndTable();
  }
};

struct ParagraphBuilder {
  typedef Paragraph Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_lines(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> lines) {
    fbb_.AddOffset(Paragraph::VT_LINES, lines);
  }
  explicit ParagraphBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ParagraphBuilder &operator=(const ParagraphBuilder &);
  flatbuffers::Offset<Paragraph> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Paragraph>(end);
    return o;
  }
};

inline flatbuffers::Offset<Paragraph> CreateParagraph(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> lines = 0) {
  ParagraphBuilder builder_(_fbb);
  builder_.add_lines(lines);
  return builder_.Finish();
}

inline flatbuffers::Offset<Paragraph> CreateParagraphDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *lines = nullptr) {
  auto lines__ = lines ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*lines) : 0;
  return ProjectSemantics::CreateParagraph(
      _fbb,
      lines__);
}

struct Project FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ProjectBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VERSION_YEAR = 4,
    VT_VERSION_MAJOR = 6,
    VT_VERSION_MINOR = 8,
    VT_NAME = 10,
    VT_PARAGRAPHS = 12
  };
  int16_t version_year() const {
    return GetField<int16_t>(VT_VERSION_YEAR, 0);
  }
  int16_t version_major() const {
    return GetField<int16_t>(VT_VERSION_MAJOR, 0);
  }
  int16_t version_minor() const {
    return GetField<int16_t>(VT_VERSION_MINOR, 0);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::Vector<flatbuffers::Offset<ProjectSemantics::Paragraph>> *paragraphs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ProjectSemantics::Paragraph>> *>(VT_PARAGRAPHS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int16_t>(verifier, VT_VERSION_YEAR) &&
           VerifyField<int16_t>(verifier, VT_VERSION_MAJOR) &&
           VerifyField<int16_t>(verifier, VT_VERSION_MINOR) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_PARAGRAPHS) &&
           verifier.VerifyVector(paragraphs()) &&
           verifier.VerifyVectorOfTables(paragraphs()) &&
           verifier.EndTable();
  }
};

struct ProjectBuilder {
  typedef Project Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_version_year(int16_t version_year) {
    fbb_.AddElement<int16_t>(Project::VT_VERSION_YEAR, version_year, 0);
  }
  void add_version_major(int16_t version_major) {
    fbb_.AddElement<int16_t>(Project::VT_VERSION_MAJOR, version_major, 0);
  }
  void add_version_minor(int16_t version_minor) {
    fbb_.AddElement<int16_t>(Project::VT_VERSION_MINOR, version_minor, 0);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Project::VT_NAME, name);
  }
  void add_paragraphs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ProjectSemantics::Paragraph>>> paragraphs) {
    fbb_.AddOffset(Project::VT_PARAGRAPHS, paragraphs);
  }
  explicit ProjectBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ProjectBuilder &operator=(const ProjectBuilder &);
  flatbuffers::Offset<Project> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Project>(end);
    return o;
  }
};

inline flatbuffers::Offset<Project> CreateProject(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t version_year = 0,
    int16_t version_major = 0,
    int16_t version_minor = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ProjectSemantics::Paragraph>>> paragraphs = 0) {
  ProjectBuilder builder_(_fbb);
  builder_.add_paragraphs(paragraphs);
  builder_.add_name(name);
  builder_.add_version_minor(version_minor);
  builder_.add_version_major(version_major);
  builder_.add_version_year(version_year);
  return builder_.Finish();
}

inline flatbuffers::Offset<Project> CreateProjectDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t version_year = 0,
    int16_t version_major = 0,
    int16_t version_minor = 0,
    const char *name = nullptr,
    const std::vector<flatbuffers::Offset<ProjectSemantics::Paragraph>> *paragraphs = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto paragraphs__ = paragraphs ? _fbb.CreateVector<flatbuffers::Offset<ProjectSemantics::Paragraph>>(*paragraphs) : 0;
  return ProjectSemantics::CreateProject(
      _fbb,
      version_year,
      version_major,
      version_minor,
      name__,
      paragraphs__);
}

inline const ProjectSemantics::Project *GetProject(const void *buf) {
  return flatbuffers::GetRoot<ProjectSemantics::Project>(buf);
}

inline const ProjectSemantics::Project *GetSizePrefixedProject(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<ProjectSemantics::Project>(buf);
}

inline bool VerifyProjectBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<ProjectSemantics::Project>(nullptr);
}

inline bool VerifySizePrefixedProjectBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<ProjectSemantics::Project>(nullptr);
}

inline void FinishProjectBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<ProjectSemantics::Project> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedProjectBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<ProjectSemantics::Project> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace ProjectSemantics

#endif  // FLATBUFFERS_GENERATED_PROJECT_PROJECTSEMANTICS_H_