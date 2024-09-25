#include "ofxGVTextureSerializer.h"

#include "lz4.h"
#include "stb_dxt.h"

#include <memory>

#define GVTS_LOG_VERBOSE 0

// NOTE
// ## Binary File Format (GVTexture)

// ```txt
// 0: uint32_t width
// 4: uint32_t height
// 12: uint32_t format (DXT1 = 1, DXT3 = 3, DXT5 = 5, BC7 = 7)
// 16: uint32_t frame bytes
// 20: raw frame storage (lz4 compressed)
// ```

ofxGVTextureSerializer::ofxGVTextureSerializer()
{
	// ofLogNotice() << "GVTextureSerializer::constructor";
}

ofxGVTextureSerializer::~ofxGVTextureSerializer()
{
	// ofLogNotice() << "GVTextureSerializer::destructor";
}

ofBuffer ofxGVTextureSerializer::serializeImageWithoutLZ4(const ofPixels &pixels)
{
	// ofLogNotice() << "GVTextureSerializer::serializeImageWithoutLZ4";

	ofPixelFormat pixel_format = pixels.getPixelFormat();
	std::string format_name = pixelFormatToString(pixel_format);
	// ofLogNotice() << "Pixel format: " + format_name;

	if (pixel_format != OF_PIXELS_RGBA) {
		ofLogError() << "Unsupported pixel format: " + format_name;
		ofLogNotice() << "OF_PIXELS_RGBA is only supported";
		return ofBuffer();
	}

	
	ofBuffer bytes_dxt5;
	size_t rgba_size = pixels.getWidth() * pixels.getHeight() * 4;
	// NOTE: dxt5 size is 1/4 of rgba8 size, dxt1 size is 1/6 of rgba8 size
	bytes_dxt5.allocate( rgba_size / 4 );

	rygCompress(
		(unsigned char *)bytes_dxt5.getData(),
		(unsigned char *)pixels.getData(),
		pixels.getWidth(),
		pixels.getHeight(),
		1
	);

	this->compressedImageFormat = FORMAT_DXT5;

	return bytes_dxt5;

}

ofBuffer ofxGVTextureSerializer::compressLZ4(const ofBuffer &data)
{
	// ofLogNotice() << "GVTextureSerializer::compressLZ4";

	ofBuffer compressed_data;
	int compressed_bound = LZ4_compressBound(data.size());
	// ofLogNotice() << "data.size(): " + ofToString(data.size());
	// ofLogNotice() << "compressed_bound: " + ofToString(compressed_bound);
	compressed_data.allocate( compressed_bound );
	// return ofBuffer();
	int real_compressed_size = LZ4_compress_default(
		(const char *)data.getData(),
		(char *)compressed_data.getData(),
		data.size(),
		compressed_bound
	);
	
	if (real_compressed_size < 0) {
		ofLogError() << "Compression failed: " + ofToString(real_compressed_size);
		assert(false);
	}
	// ofLogNotice() << "real_compressed_size: " + ofToString(real_compressed_size);
	compressed_data.resize(real_compressed_size);

	// assert(compressed_data.size() <= compressed_bound);
	// assert(compressed_data.size() == real_compressed_size);

	return compressed_data;
}

ofBuffer ofxGVTextureSerializer::decompressLZ4(const ofBuffer &data, size_t frameSize)
{
	// ofLogNotice() << "GVTextureSerializer::decompressLZ4";

	// ofLogNotice() << "frameSize: " + ofToString(frameSize);
	// ofLogNotice() << "data.size(): " + ofToString(data.size());

	ofBuffer decompressed_data;
	// size_t decompressed_bound = frameSize;

	size_t decompressed_bound = frameSize;

	decompressed_data.allocate( decompressed_bound );
	// decompressed_data.reserve(decompressed_bound);

	int real_decompressed_size = LZ4_decompress_safe(
		(const char *)data.getData(),
		(char *)decompressed_data.getData(),
		data.size(),
		decompressed_bound
	);

	// int real_decompressed_size = LZ4_decompress_fast(
	// 	(const char *)data.getData(),
	// 	(char *)decompressed_data.getData(),
	// 	data.size()
	// );

	if (real_decompressed_size < 0) {
		ofLogError() << "Decompression failed: " + ofToString(real_decompressed_size);
		assert(false);
	}
	decompressed_data.resize(real_decompressed_size);

	// ofLogNotice() << "real_decompressed_size: " + ofToString(real_decompressed_size);

	return decompressed_data;
}

ofBuffer ofxGVTextureSerializer::createGVTextureByteArray(LZ4Data lz4Data)
{
	ofBuffer gv_texture_bytes;
	int compressed_bytes = lz4Data.getCompressedSize();
	gv_texture_bytes.allocate(20 + compressed_bytes);

	char *gv_texture_bytes_ptr = (char *)gv_texture_bytes.getData();
	char* lz4DataPtr = (char *)lz4Data.getData().getData();
	int width = lz4Data.getWidth();
	int height = lz4Data.getHeight();
	int format = lz4Data.getFormat();
	int frame_size = lz4Data.getFrameSize();
	std::memcpy(gv_texture_bytes_ptr, &width, 4);
	std::memcpy(gv_texture_bytes_ptr + 4, &height, 4);
	std::memcpy(gv_texture_bytes_ptr + 8, &format, 4);
	std::memcpy(gv_texture_bytes_ptr + 12, &frame_size, 4);
	std::memcpy(gv_texture_bytes_ptr + 16, &compressed_bytes, 4);
	std::memcpy(gv_texture_bytes_ptr + 20, lz4DataPtr, compressed_bytes);

#if GVTS_LOG_VERBOSE
	ofLogNotice() << "serialized info";
	ofLogNotice() << "lz4Data.width: " + ofToString(lz4Data.width);
	ofLogNotice() << "lz4Data.height: " + ofToString(lz4Data.height);
	ofLogNotice() << "lz4Data.format: " + imageFormatToString(lz4Data.format);
	ofLogNotice() << "lz4Data.frame_size: " + ofToString(lz4Data.frame_size);
	ofLogNotice() << "lz4Data.compressed_bytes.size(): " + ofToString(lz4Data.lz4_compressed_bytes.size());
#endif

	return gv_texture_bytes;
}

LZ4Data ofxGVTextureSerializer::getLZ4DataFromGVTextureByteArray(const ofBuffer &data)
{
	const char *gv_texture_bytes_ptr = (char *)data.getData();
	// lz4Data.width = gv_texture_bytes_ptr[0];
	// lz4Data.height = gv_texture_bytes_ptr[1];
	// lz4Data.format = gv_texture_bytes_ptr[2];
	// lz4Data.frame_size = gv_texture_bytes_ptr[3];
	int width = 0;
	int height = 0;
	int format = 0;
	int frame_size = 0;
	int compressed_bytes = 0;
	std::memcpy(&width, gv_texture_bytes_ptr, 4);
	std::memcpy(&height, gv_texture_bytes_ptr + 4, 4);
	std::memcpy(&format, gv_texture_bytes_ptr + 8, 4);
	std::memcpy(&frame_size, gv_texture_bytes_ptr + 12, 4);
	std::memcpy(&compressed_bytes, gv_texture_bytes_ptr + 16, 4);

	ofBuffer buf;
	buf.allocate(compressed_bytes);

	char* lz4DataPtr = buf.getData();
	std::memcpy(lz4DataPtr, gv_texture_bytes_ptr + 20, compressed_bytes);

	LZ4Data lz4Data = 
		LZ4Data (
			width,
			height,
			format,
			frame_size,
			buf
		);

	return lz4Data;
}

inline void printFirstAndLastBytes(const ofBuffer &buf)
{
	const unsigned char *ptr = (unsigned char *)buf.getData();
	ofLogNotice() << "First 16 bytes:";
	std::string s;
	for (int i = 0; i < 16; i++) {
		s += ofToString((int)ptr[i]);
		s += " ";
	}
	ofLogNotice() << s;
	ofLogNotice() << "Last 16 bytes:";
	s = ""; 
	for (int i = buf.size() - 16; i < buf.size(); i++) {
		s += ofToString((int)ptr[i]);
		s += " ";
	}
	ofLogNotice() << s;
}

ofBuffer ofxGVTextureSerializer::serializeImage(const ofPixels &pixels)
{
	// ofLogNotice() << "GVTextureSerializer::serializeImage";

	ofBuffer bytes_dxt5 = serializeImageWithoutLZ4(pixels);
	int frame_size = bytes_dxt5.size();

#if GVTS_LOG_VERBOSE
	ofLogNotice() << "serialized DX5 bytes:";
	printFirstAndLastBytes(bytes_dxt5);
#endif

	ofBuffer compressed_bytes = compressLZ4(bytes_dxt5);

#if GVTS_LOG_VERBOSE
	ofLogNotice() << "serialized compressed bytes:";
	printFirstAndLastBytes(compressed_bytes);
#endif

	LZ4Data lz4Data = 
		LZ4Data(
			pixels.getWidth(),
			pixels.getHeight(),
			imageFormatToGVTextureFormat(this->compressedImageFormat),
			frame_size,
			compressed_bytes
		);

	return createGVTextureByteArray(lz4Data);
}

ofBuffer ofxGVTextureSerializer::serializeTexture(const ofTexture &texture)
{
	// ofLogNotice() << "GVTextureSerializer::serializeTexture";

	ofPixels pixels;
	texture.readToPixels(pixels);
	return serializeImage(pixels);
}

ofTexture ofxGVTextureSerializer::deserialize(const ofBuffer &buf)
{
	// ofLogNotice() << "GVTextureSerializer::deserialize";

	LZ4Data lz4Data = getLZ4DataFromGVTextureByteArray(buf);

#if GVTS_LOG_VERBOSE
	ofLogNotice() << "deserialized info";
	ofLogNotice() << "lz4Data.width: " + ofToString(lz4Data.width);
	ofLogNotice() << "lz4Data.height: " + ofToString(lz4Data.height);
	ofLogNotice() << "lz4Data.format: " + imageFormatToString(lz4Data.format);
	ofLogNotice() << "lz4Data.frame_size: " + ofToString(lz4Data.frame_size);
	ofLogNotice() << "lz4Data.compressed_bytes.size(): " + ofToString(lz4Data.lz4_compressed_bytes.size());

	ofLogNotice() << "deserialized compressed bytes:";
	printFirstAndLastBytes(lz4Data.lz4_compressed_bytes);
#endif

	ofBuffer decompressed_bytes = decompressLZ4(lz4Data.lz4_compressed_bytes, lz4Data.frame_size);

#if GVTS_LOG_VERBOSE
	ofLogNotice() << "decompressed DXT5 bytes:";
	printFirstAndLastBytes(decompressed_bytes);
#endif

	GLuint _texture = 0;

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    int _glFmt = FORMAT_UNKNOWN;
    
	switch (lz4Data.getFormat()) {
		case FORMAT_DXT1:
			_glFmt = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;
		case FORMAT_DXT3:
			_glFmt = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FORMAT_DXT5:
			_glFmt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		case FORMAT_BPTC_RGBA:
			_glFmt = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
			break;
	}

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, _glFmt, lz4Data.getWidth(), lz4Data.getHeight(), 0, lz4Data.getFrameSize(), decompressed_bytes.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	ofTexture tex;
	ofTextureData &data = tex.texData;
	data.textureID = 0;
	tex.setUseExternalTextureID(_texture);
	// data.glInternalFormat = _glFmt;

	data.width = lz4Data.width;
	data.height = lz4Data.height;
	data.textureTarget = GL_TEXTURE_2D;
	data.tex_w = lz4Data.width;
	data.tex_h = lz4Data.height;
	data.tex_t = 1.0f;
	data.tex_u = 1.0f;

	return tex;
}

int ofxGVTextureSerializer::imageFormatToGVTextureFormat(int image_format)
{
	return image_format;
}
