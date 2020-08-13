#include "nifactory.hpp"
#include <string>
#include <unordered_map>
#include "nifreader.hpp"
#include "main/main.hpp"

std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::createCreatorMap() {
        std::unordered_map<std::string, NiObject* (*)(NifReader&)> m;
		m["NiObject"] = &NiObject::create;
		m["NiAVObject"] = &NiAVObject::create;
		m["NiDynamicEffect"] = &NiDynamicEffect::create;
		m["NiProperty"] = &NiProperty::create;
		m["NiObjectNET"] = &NiObjectNET::create;
		m["NiCollisionObject"] = &NiCollisionObject::create;
		m["NiNode"] = &NiNode::create;
		m["NiStringExtraData"] = &NiStringExtraData::create;
		m["NiTriStrips"] = &NiTriStrips::create;
		m["NiTriStripsData"] = &NiTriStripsData::create;
		m["BSShaderPPLightingProperty"] = &BSShaderPPLightingProperty::create;
		m["BSShaderTextureSet"] = &BSShaderTextureSet::create;
		m["BSShaderLightingProperty"] = &BSShaderLightingProperty::create;
		m["BSShaderProperty"] = &BSShaderProperty::create;
		m["NiShadeProperty"] = &NiShadeProperty::create;
		m["NiMaterialProperty"] = &NiMaterialProperty::create;
		m["NiAlphaProperty"] = &NiAlphaProperty::create;
        return m;
    }

const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();