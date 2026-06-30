#include "archive.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    ArchiveParser parser;
    parser.parse(data, size);
    return 0;
}