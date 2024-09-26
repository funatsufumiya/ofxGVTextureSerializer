#pragma once

#include "ofMain.h"
#include "ofxGVTexture.h"

// #define GVT_DXT1 1
// #define GVT_DXT3 3
// #define GVT_DXT5 5
// #define GVT_BC7 7

enum ImageFormat {
    FORMAT_UNKNOWN = 99,
    FORMAT_RGBA8 = 0,
    FORMAT_DXT1 = 1,
    FORMAT_DXT3 = 3,
    FORMAT_DXT5 = 5,
    FORMAT_BPTC_RGBA = 7,
};

struct LZ4Data {
    int width;
    int height;
    int format;
    int frame_size;
    ofBuffer lz4_compressed_bytes;

    LZ4Data(int width, int height, int format, int frame_size, const ofBuffer &lz4_compressed_bytes)
    {
        this->width = width;
        this->height = height;
        this->format = format;
        this->frame_size = frame_size;
        this->lz4_compressed_bytes = lz4_compressed_bytes;
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
    int getFormat() { return format; }
    int getFrameSize() { return frame_size; }
    ofBuffer getData() { return lz4_compressed_bytes; }
    size_t getCompressedSize() { return lz4_compressed_bytes.size(); }
};

inline std::string pixelFormatToString(ofPixelFormat format)
{
    switch (format) {
        case OF_PIXELS_GRAY: return "OF_PIXELS_GRAY";
        case OF_PIXELS_GRAY_ALPHA: return "OF_PIXELS_GRAY_ALPHA";
        case OF_PIXELS_RGB: return "OF_PIXELS_RGB";
        case OF_PIXELS_BGR: return "OF_PIXELS_BGR";
        case OF_PIXELS_RGBA: return "OF_PIXELS_RGBA";
        case OF_PIXELS_BGRA: return "OF_PIXELS_BGRA";
        case OF_PIXELS_RGB565: return "OF_PIXELS_RGB565";
        case OF_PIXELS_NV12: return "OF_PIXELS_NV12";
        case OF_PIXELS_NV21: return "OF_PIXELS_NV21";
        case OF_PIXELS_YV12: return "OF_PIXELS_YV12";
        case OF_PIXELS_I420: return "OF_PIXELS_I420";
        case OF_PIXELS_YUY2: return "OF_PIXELS_YUY2";
        case OF_PIXELS_UYVY: return "OF_PIXELS_UYVY";
        case OF_PIXELS_Y: return "OF_PIXELS_Y";
        case OF_PIXELS_U: return "OF_PIXELS_U";
        case OF_PIXELS_V: return "OF_PIXELS_V";
        case OF_PIXELS_UV: return "OF_PIXELS_UV";
        case OF_PIXELS_VU: return "OF_PIXELS_VU";
        case OF_PIXELS_NUM_FORMATS: return "OF_PIXELS_NUM_FORMATS";
        case OF_PIXELS_UNKNOWN: return "OF_PIXELS_UNKNOWN";
        case OF_PIXELS_NATIVE: return "OF_PIXELS_NATIVE";
        default: return "Unknown pixel format";
    }
}

inline std::string imageFormatToString(int format)
{
    switch (format) {
        case FORMAT_RGBA8: return "FORMAT_RGBA8";
        case FORMAT_DXT1: return "FORMAT_DXT1";
        case FORMAT_DXT3: return "FORMAT_DXT3";
        case FORMAT_DXT5: return "FORMAT_DXT5";
        case FORMAT_BPTC_RGBA: return "FORMAT_BPTC_RGBA";
        default: return "Unknown image format";
    }
}

class ofxGVTextureSerializer {
protected:
    ImageFormat compressedImageFormat = FORMAT_UNKNOWN;

    ofBuffer serializeImageWithoutLZ4(const ofPixels &pixels);
    ofBuffer compressLZ4(const ofBuffer &data);
    ofBuffer decompressLZ4(const ofBuffer &data, size_t frameSize);
    ofBuffer createGVTextureByteArray(LZ4Data lz4Data);
    LZ4Data getLZ4DataFromGVTextureByteArray(const ofBuffer &data);
    int imageFormatToGVTextureFormat(int image_format);
    int gvTextureFormatToImageFormat(int gv_texture_format);

public:
    
    ofxGVTextureSerializer();
    ~ofxGVTextureSerializer();

    ofBuffer serializeImage(const ofPixels &pixels);
    ofBuffer serializeTexture(const ofTexture &texture);
    ofTexture deserialize(const ofBuffer &data);

    int getCompressedImageFormat() { return compressedImageFormat; }
};
