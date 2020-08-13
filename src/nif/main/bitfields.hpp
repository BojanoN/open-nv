
    #pragma once
    #include "enums.hpp"
	#include "typedefs.hpp"
    struct TimeControllerFlags {
	NiEnums::AnimType animType : 1; // 0x0001
	NiEnums::CycleType cycleType : 2; // 0x0006
	nif_bool_t active : 1; // 0x0008
	nif_bool_t playBackwards : 1; // 0x0010
	nif_bool_t managerControlled : 1; // 0x0020
	nif_bool_t computeScaledTime : 1; // 0x0040
	nif_bool_t forcedUpdate : 1; // 0x0080
	TimeControllerFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((animType & (b >> 0)) << 0) & 0x0001;
		result += ((cycleType & (b >> 1)) << 1) & 0x0006;
		result += ((active & (b >> 3)) << 3) & 0x0008;
		result += ((playBackwards & (b >> 4)) << 4) & 0x0010;
		result += ((managerControlled & (b >> 5)) << 5) & 0x0020;
		result += ((computeScaledTime & (b >> 6)) << 6) & 0x0040;
		result += ((forcedUpdate & (b >> 7)) << 7) & 0x0080;
		return result;
	}
	TimeControllerFlags& operator=(const uint16_t& value) {
	animType = static_cast<NiEnums::AnimType>(value & 0x0001);
	cycleType = static_cast<NiEnums::CycleType>(value & 0x0006);
	active = value & 0x0008;
	playBackwards = value & 0x0010;
	managerControlled = value & 0x0020;
	computeScaledTime = value & 0x0040;
	forcedUpdate = value & 0x0080;
		return *this;
	}
};
struct AlphaFlags {
	nif_bool_t alphaBlend : 1; // 0x0001
	NiEnums::AlphaFunction sourceBlendMode : 4; // 0x001E
	NiEnums::AlphaFunction destinationBlendMode : 4; // 0x01E0
	nif_bool_t alphaTest : 1; // 0x0200
	NiEnums::TestFunction testFunc : 3; // 0x1C00
	nif_bool_t noSorter : 1; // 0x2000
	nif_bool_t cloneUnique : 1; // 0x4000
	nif_bool_t editorAlphaThreshold : 1; // 0x8000
	AlphaFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((alphaBlend & (b >> 0)) << 0) & 0x0001;
		result += ((sourceBlendMode & (b >> 1)) << 1) & 0x001E;
		result += ((destinationBlendMode & (b >> 5)) << 5) & 0x01E0;
		result += ((alphaTest & (b >> 9)) << 9) & 0x0200;
		result += ((testFunc & (b >> 10)) << 10) & 0x1C00;
		result += ((noSorter & (b >> 13)) << 13) & 0x2000;
		result += ((cloneUnique & (b >> 14)) << 14) & 0x4000;
		result += ((editorAlphaThreshold & (b >> 15)) << 15) & 0x8000;
		return result;
	}
	AlphaFlags& operator=(const uint16_t& value) {
	alphaBlend = value & 0x0001;
	sourceBlendMode = static_cast<NiEnums::AlphaFunction>(value & 0x001E);
	destinationBlendMode = static_cast<NiEnums::AlphaFunction>(value & 0x01E0);
	alphaTest = value & 0x0200;
	testFunc = static_cast<NiEnums::TestFunction>(value & 0x1C00);
	noSorter = value & 0x2000;
	cloneUnique = value & 0x4000;
	editorAlphaThreshold = value & 0x8000;
		return *this;
	}
};
struct FogFlags {
	nif_bool_t enable : 1; // 0x0001
	NiEnums::FogFunction fogFunction : 3; // 0x0006
	FogFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((enable & (b >> 0)) << 0) & 0x0001;
		result += ((fogFunction & (b >> 1)) << 1) & 0x0006;
		return result;
	}
	FogFlags& operator=(const uint16_t& value) {
	enable = value & 0x0001;
	fogFunction = static_cast<NiEnums::FogFunction>(value & 0x0006);
		return *this;
	}
};
struct StencilFlags {
	nif_bool_t enable : 1; // 0x0001
	NiEnums::StencilAction failAction : 3; // 0x000E
	NiEnums::StencilAction zfailAction : 3; // 0x0070
	NiEnums::StencilAction passAction : 3; // 0x0380
	NiEnums::StencilDrawMode drawMode : 2; // 0x0C00
	NiEnums::StencilTestFunc testFunc : 3; // 0xF000
	StencilFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((enable & (b >> 0)) << 0) & 0x0001;
		result += ((failAction & (b >> 1)) << 1) & 0x000E;
		result += ((zfailAction & (b >> 4)) << 4) & 0x0070;
		result += ((passAction & (b >> 7)) << 7) & 0x0380;
		result += ((drawMode & (b >> 10)) << 10) & 0x0C00;
		result += ((testFunc & (b >> 12)) << 12) & 0xF000;
		return result;
	}
	StencilFlags& operator=(const uint16_t& value) {
	enable = value & 0x0001;
	failAction = static_cast<NiEnums::StencilAction>(value & 0x000E);
	zfailAction = static_cast<NiEnums::StencilAction>(value & 0x0070);
	passAction = static_cast<NiEnums::StencilAction>(value & 0x0380);
	drawMode = static_cast<NiEnums::StencilDrawMode>(value & 0x0C00);
	testFunc = static_cast<NiEnums::StencilTestFunc>(value & 0xF000);
		return *this;
	}
};
struct TexturingFlags {
	nif_bool_t multitexture : 1; // 0x0001
	NiEnums::ApplyMode applyMode : 3; // 0x000E
	uint16_t decalCount : 8; // 0x0FF0
	TexturingFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((multitexture & (b >> 0)) << 0) & 0x0001;
		result += ((applyMode & (b >> 1)) << 1) & 0x000E;
		result += ((decalCount & (b >> 4)) << 4) & 0x0FF0;
		return result;
	}
	TexturingFlags& operator=(const uint16_t& value) {
	multitexture = value & 0x0001;
	applyMode = static_cast<NiEnums::ApplyMode>(value & 0x000E);
	decalCount = value & 0x0FF0;
		return *this;
	}
};
struct TexturingMapFlags {
	uint16_t textureIndex : 8; // 0x00FF
	NiEnums::TexFilterMode filterMode : 4; // 0x0F00
	NiEnums::TexClampMode clampMode : 4; // 0x3000
	TexturingMapFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((textureIndex & (b >> 0)) << 0) & 0x00FF;
		result += ((filterMode & (b >> 8)) << 8) & 0x0F00;
		result += ((clampMode & (b >> 12)) << 12) & 0x3000;
		return result;
	}
	TexturingMapFlags& operator=(const uint16_t& value) {
	textureIndex = value & 0x00FF;
	filterMode = static_cast<NiEnums::TexFilterMode>(value & 0x0F00);
	clampMode = static_cast<NiEnums::TexClampMode>(value & 0x3000);
		return *this;
	}
};
struct VertexColorFlags {
	uint16_t colorMode : 3; // 0x0007
	NiEnums::LightingMode lightingMode : 1; // 0x0008
	NiEnums::SourceVertexMode sourceVertexMode : 2; // 0x0030
	VertexColorFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((colorMode & (b >> 0)) << 0) & 0x0007;
		result += ((lightingMode & (b >> 3)) << 3) & 0x0008;
		result += ((sourceVertexMode & (b >> 4)) << 4) & 0x0030;
		return result;
	}
	VertexColorFlags& operator=(const uint16_t& value) {
	colorMode = value & 0x0007;
	lightingMode = static_cast<NiEnums::LightingMode>(value & 0x0008);
	sourceVertexMode = static_cast<NiEnums::SourceVertexMode>(value & 0x0030);
		return *this;
	}
};
struct ZBufferFlags {
	nif_bool_t zbufferTest : 1; // 0x0001
	nif_bool_t zbufferWrite : 1; // 0x0002
	NiEnums::TestFunction testFunc : 3; // 0x003C
	ZBufferFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((zbufferTest & (b >> 0)) << 0) & 0x0001;
		result += ((zbufferWrite & (b >> 1)) << 1) & 0x0002;
		result += ((testFunc & (b >> 2)) << 2) & 0x003C;
		return result;
	}
	ZBufferFlags& operator=(const uint16_t& value) {
	zbufferTest = value & 0x0001;
	zbufferWrite = value & 0x0002;
	testFunc = static_cast<NiEnums::TestFunction>(value & 0x003C);
		return *this;
	}
};
struct NiAGDDataStreamFlags {
	nif_bool_t keep : 1; // 0x0001
	NiEnums::AGDConsistencyType consistencyType : 3; // 0x0006
	NiAGDDataStreamFlags() { }
	uint8_t operator&(const uint8_t& b) const {
		uint8_t result = 0;
		result += ((keep & (b >> 0)) << 0) & 0x0001;
		result += ((consistencyType & (b >> 1)) << 1) & 0x0006;
		return result;
	}
	NiAGDDataStreamFlags& operator=(const uint8_t& value) {
	keep = value & 0x0001;
	consistencyType = static_cast<NiEnums::AGDConsistencyType>(value & 0x0006);
		return *this;
	}
};
struct NiGeometryDataFlags {
	nif_bool_t numUVSets : 6; // 0x003F
	uint16_t havokMaterial : 6; // 0x0FC0
	NiEnums::NiNBTMethod nbtMethod : 2; // 0xF000
	NiGeometryDataFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((numUVSets & (b >> 0)) << 0) & 0x003F;
		result += ((havokMaterial & (b >> 6)) << 6) & 0x0FC0;
		result += ((nbtMethod & (b >> 12)) << 12) & 0xF000;
		return result;
	}
	NiGeometryDataFlags& operator=(const uint16_t& value) {
	numUVSets = value & 0x003F;
	havokMaterial = value & 0x0FC0;
	nbtMethod = static_cast<NiEnums::NiNBTMethod>(value & 0xF000);
		return *this;
	}
};
struct BSGeometryDataFlags {
	nif_bool_t hasUV : 6; // 0x0001
	uint16_t havokMaterial : 6; // 0x0FC0
	nif_bool_t hasTangents : 1; // 0x1000
	BSGeometryDataFlags() { }
	uint16_t operator&(const uint16_t& b) const {
		uint16_t result = 0;
		result += ((hasUV & (b >> 0)) << 0) & 0x0001;
		result += ((havokMaterial & (b >> 6)) << 6) & 0x0FC0;
		result += ((hasTangents & (b >> 12)) << 12) & 0x1000;
		return result;
	}
	BSGeometryDataFlags& operator=(const uint16_t& value) {
	hasUV = value & 0x0001;
	havokMaterial = value & 0x0FC0;
	hasTangents = value & 0x1000;
		return *this;
	}
};
struct CollisionFilterFlags {
	NiEnums::BipedPart bipedPart : 5; // 0x001F
	nif_bool_t moppScaled : 1; // 0x0020
	nif_bool_t noCollision : 1; // 0x0040
	nif_bool_t linkedGroup : 1; // 0x0080
	CollisionFilterFlags() { }
	uint8_t operator&(const uint8_t& b) const {
		uint8_t result = 0;
		result += ((bipedPart & (b >> 0)) << 0) & 0x001F;
		result += ((moppScaled & (b >> 5)) << 5) & 0x0020;
		result += ((noCollision & (b >> 6)) << 6) & 0x0040;
		result += ((linkedGroup & (b >> 7)) << 7) & 0x0080;
		return result;
	}
	CollisionFilterFlags& operator=(const uint8_t& value) {
	bipedPart = static_cast<NiEnums::BipedPart>(value & 0x001F);
	moppScaled = value & 0x0020;
	noCollision = value & 0x0040;
	linkedGroup = value & 0x0080;
		return *this;
	}
};
