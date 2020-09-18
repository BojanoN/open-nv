#pragma once
#include <vector>
#include <cstdint>


const uint32_t ddsMagic = 0x20534444; // 'DDS '

enum ddsHeaderFlags : uint32_t {
	DDSD_CAPS = 0x1, // always
	DDSD_HEIGHT = 0x2, // always
	DDSD_WIDTH = 0x4, // always
	DDSD_PITCH = 0x8, // if pitch provided, uncompressed texture
	DDSD_PIXELFORMAT = 0x1000, // always
	DDSD_MIPMAPCOUNT = 0x20000, // if mipmapped
	DDSD_LINEARSIZE = 0x80000, // if pitch provided, compressed texture
	DDSD_DEPTH = 0x800000 // if depth
};

enum ddsCaps : uint32_t {
	DDSCAPS_COMPLEX = 0x8, // if multiple surfaces
	DDSCAPS_MIPMAP = 0x400000, // if mipmapped
	DDSCAPS_TEXTURE = 0x1000 // always
};

enum ddsCaps2 : uint32_t {
	DDSCAPS2_CUBEMAP = 0x200, // if cubemap
	DDSCAPS2_CUBEMAP_POSITIVEX = 0x400, // if in cubemap
	DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800, // if in cubemap
	DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000, // if in cubemap
	DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000, // if in cubemap
	DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000, // if in cubemap
	DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000, // if in cubemap
	DDSCAPS2_VOLUME = 0x200000 // if volume
};

enum ddsPixelFormatFlags : uint32_t {
	DDPF_ALPHAPIXELS = 0x1, // has alpha
	DDPF_ALPHA = 0x2, // only alpha
	DDPF_FOURCC = 0x4, // compressed RGB
	DDPF_RGB = 0x40, // uncompressed RGB
	DDPF_YUV = 0x200, // uncompressed YUV
	DDPF_LUMINANCE = 0x20000 // only luminance
};

enum ddsFormat : uint32_t {
	DDS_FOURCC_DXT1 = 0x31545844,
	DDS_FOURCC_DXT2 = 0x32545844,
	DDS_FOURCC_DXT3 = 0x33545844,
	DDS_FOURCC_DXT4 = 0x34545844,
	DDS_FOURCC_DXT5 = 0x35545844,
	DDS_FOURCC_DX10 = 0x30315844
};

struct ddsPixelFormat {
	uint32_t size;
	ddsPixelFormatFlags flags;
	uint32_t fourcc;
	uint32_t rgbBitCount;
	uint32_t rBitMask;
	uint32_t gBitMask;
	uint32_t bBitMask;
	uint32_t aBitMask;
};

struct ddsHeader {
	uint32_t size; //124
	ddsHeaderFlags flags;
	uint32_t height;
	uint32_t width;
	uint32_t pitchOrLinearSize;
	uint32_t depth;
	uint32_t mipMapCount;
	uint32_t reserved_1[11];
	ddsPixelFormat pixelFormat;
	ddsCaps caps;
	ddsCaps2 caps2;
	uint32_t caps3;
	uint32_t caps4;
	uint32_t reserved_2;
};


class Texture2D {

private:

	ddsHeader header;
	std::vector<uint8_t> textureData;

public:

	Texture2D(ddsHeader header, std::vector<uint8_t> data) : header{header}, textureData{data} {}
};