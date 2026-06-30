#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include <vector>

struct ImageHeader {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t data_offset;
};

class ImageParser {
public:
    bool parse(const uint8_t* data, size_t size);
    const std::vector<uint8_t>& getPixels() const { return pixels_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    int getChannels() const { return channels_; }

private:
    void decodeRLE(const uint8_t* data, size_t size);
    void decodePalette(const uint8_t* data, size_t size);

    std::vector<uint8_t> pixels_;
    int width_ = 0;
    int height_ = 0;
    int channels_ = 3;
    bool is_valid_ = false;
};

bool parseImage(const uint8_t* data, size_t size);

#endif