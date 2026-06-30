#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <cstdint>
#include <string>
#include <vector>

struct ArchiveEntry {
    char filename[256];
    uint32_t offset;
    uint32_t size;
};

struct ArchiveHeader {
    uint32_t magic;
    uint32_t entry_count;
    uint32_t entries_offset;
};

class ArchiveParser {
public:
    bool parse(const uint8_t* data, size_t size);
    const std::vector<std::vector<uint8_t>>& getEntries() const { return extracted_entries_; }
    size_t getEntryCount() const { return entries_.size(); }

private:
    std::vector<ArchiveEntry> entries_;
    std::vector<std::vector<uint8_t>> extracted_entries_;
};

bool parseArchive(const uint8_t* data, size_t size);

#endif