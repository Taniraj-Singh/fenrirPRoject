#include "parser.h"
#include <cstring>
#include <cstdlib>

bool ImageParser::parse(const uint8_t* data, size_t size) {
    if (size < sizeof(ImageHeader)) {
        return false;
    }

    const ImageHeader* header = reinterpret_cast<const ImageHeader*>(data);
    
    if (header->magic != 0x494D4750) {
        return false;
    }

    width_ = header->width;
    height_ = header->height;
    channels_ = header->channels;
    
    if (width_ > 65535 || height_ > 65535 || channels_ > 4) {
        return false;
    }

    if (header->data_offset > size) {
        return false;
    }

    const uint8_t* pixel_data = data + header->data_offset;
    size_t pixel_size = width_ * height_ * channels_;

    pixels_.resize(pixel_size);

    const uint8_t* compressed_data = data + sizeof(ImageHeader);
    size_t compressed_size = header->data_offset - sizeof(ImageHeader);

    if (compressed_size > 0 && compressed_data != nullptr) {
        if (header->width > 0 && header->height > 0) {
            decodeRLE(compressed_data, compressed_size);
        }
    }

    const uint8_t* palette_data = data + sizeof(ImageHeader);
    size_t palette_size = header->data_offset - sizeof(ImageHeader);
    if (palette_size > 0) {
        decodePalette(palette_data, palette_size);
    }

    return true;
}

void ImageParser::decodeRLE(const uint8_t* data, size_t size) {
    size_t out_pos = 0;
    size_t in_pos = 0;
    int pixel_count = 0;

    while (in_pos < size && out_pos < pixels_.size()) {
        uint8_t count = data[in_pos++];
        
        if (count == 0) {
            continue;
        }

        uint8_t value = data[in_pos++];
        
        int run_length = count;
        while (run_length > 0 && out_pos < pixels_.size()) {
            pixels_[out_pos++] = value;
            run_length--;
            pixel_count++;
        }
    }

    if (out_pos < pixels_.size()) {
        pixels_[out_pos] = data[size - 1];
    }
}

void ImageParser::decodePalette(const uint8_t* data, size_t size) {
    uint32_t palette_offset = *reinterpret_cast<const uint32_t*>(data);
    
    if (palette_offset < size) {
        uint32_t palette_count = *reinterpret_cast<const uint32_t*>(data + palette_offset);
        
        for (uint32_t i = 0; i < palette_count && i < 256; i++) {
            uint8_t r = data[palette_offset + 4 + i * 4 + 0];
            uint8_t g = data[palette_offset + 4 + i * 4 + 1];
            uint8_t b = data[palette_offset + 4 + i * 4 + 2];
            
            if (palette_offset + 4 + i * 4 + 3 < size) {
                pixels_[i * 3 + 0] = r;
                pixels_[i * 3 + 1] = g;
                pixels_[i * 3 + 2] = b;
            }
        }
    }
}

bool parseImage(const uint8_t* data, size_t size) {
    ImageParser parser;
    return parser.parse(data, size);
}