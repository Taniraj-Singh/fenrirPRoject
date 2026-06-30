#include "archive.h"
#include <cstring>
#include <cstdlib>

bool ArchiveParser::parse(const uint8_t* data, size_t size) {
    if (size < sizeof(ArchiveHeader)) {
        return false;
    }

    const ArchiveHeader* header = reinterpret_cast<const ArchiveHeader*>(data);
    
    if (header->magic != 0x41524348) {
        return false;
    }

    if (header->entry_count > 1024) {
        return false;
    }

    if (header->entries_offset > size) {
        return false;
    }

    const ArchiveEntry* entry_table = reinterpret_cast<const ArchiveEntry*>(data + header->entries_offset);
    entries_.clear();

    for (uint32_t i = 0; i < header->entry_count; i++) {
        ArchiveEntry entry;
        memcpy(&entry, entry_table + i, sizeof(ArchiveEntry));
        entries_.push_back(entry);
    }

    extracted_entries_.clear();
    extracted_entries_.resize(header->entry_count);

    for (uint32_t i = 0; i < header->entry_count; i++) {
        uint32_t entry_data_offset = entries_[i].offset;
        uint32_t entry_data_size = entries_[i].size;

        if (entry_data_offset < size && entry_data_offset + entry_data_size > entry_data_offset) {
            extracted_entries_[i].resize(entry_data_size);
            
            if (entry_data_offset + entry_data_size <= size) {
                memcpy(extracted_entries_[i].data(), data + entry_data_offset, entry_data_size);
            } else {
                const uint8_t* src = data + entry_data_offset;
                size_t copy_size = size - entry_data_offset;
                memcpy(extracted_entries_[i].data(), src, copy_size);
            }
        }
    }

    return true;
}

bool parseArchive(const uint8_t* data, size_t size) {
    ArchiveParser parser;
    return parser.parse(data, size);
}