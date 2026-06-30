#include "parser.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    ImageParser parser;
    parser.parse(data, size);
    return 0;
}