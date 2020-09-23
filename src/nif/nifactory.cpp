#include "nifactory.hpp"
#include <string>
#include <unordered_map>
#include "nifreader.hpp"
#include "main/main.hpp"
#include <memory>

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
		m["NiTriShape"] = &NiTriShape::create;
		m["NiTriShapeData"] = &NiTriShapeData::create;		
		m["NiTriStrips"] = &NiTriStrips::create;
		m["NiTriStripsData"] = &NiTriStripsData::create;
		m["BSShaderPPLightingProperty"] = &BSShaderPPLightingProperty::create;
		m["BSShaderTextureSet"] = &BSShaderTextureSet::create;
		m["BSShaderLightingProperty"] = &BSShaderLightingProperty::create;
		m["BSShaderProperty"] = &BSShaderProperty::create;
		m["NiShadeProperty"] = &NiShadeProperty::create;
		m["NiMaterialProperty"] = &NiMaterialProperty::create;
		m["NiAlphaProperty"] = &NiAlphaProperty::create;
		m["NiTimeController"] = &NiTimeController::create;
		m["BSFadeNode"] = &BSFadeNode::create;
		m["BSDismemberSkinInstance"] = &BSDismemberSkinInstance::create;
		m["NiSkinData"] = &NiSkinData::create;
		m["BSBound"] = &BSBound::create;
		m["NiSkinPartition"] = &NiSkinPartition::create;
		m["NiIntegerExtraData"] = &NiIntegerExtraData::create;
		m["NiFloatExtraData"] = &NiIntegerExtraData::create;
		m["NiFloatsExtraData"] = &NiIntegerExtraData::create;
		m["BSXFlags"] = &BSXFlags::create;

		log_info("Registered %d nif types", m.size());
        return m;
    }

const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();