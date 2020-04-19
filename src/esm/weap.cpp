#include "weap.hpp"

namespace ESM {

Weapon::Weapon(ESMReader& reader) : Record(reader) {

	reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::EDID);
    reader.readStringSubrecord(editorId);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::OBND);
    reader.readSubrecord(objectBounds);

    if(reader.peekNextType() == ESMType::FULL) {
	    reader.readNextSubrecordHeader();
        reader.readStringSubrecord(name);
    } 

    while(reader.peekNextType() != ESMType::ETYP) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::MODL:
    			ModelData::load(reader, modelData, 0);
    			break;
    		case ESMType::ICON:
    			reader.readStringSubrecord(largeIconFilename);
    			break;
    		case ESMType::MICO:
    			reader.readStringSubrecord(smallIconFilename);
    			break;
    		case ESMType::SCRI:
    			reader.readSubrecord(script);
    			break;
    		case ESMType::EITM:
    			reader.readSubrecord(objectEffect);
    			break;
    		case ESMType::EAMT:
    			reader.readSubrecord(enchantmentChargeAmount);
    			break;
    		case ESMType::NAM0:
    			reader.readSubrecord(ammo);
    			break;
    		case ESMType::DEST:
    			DestructionData::load(reader, destructionData);
    			break;
    		case ESMType::REPL:
    			reader.readSubrecord(repairList);
    			break;
    		default:
    			std::stringstream s;
    			s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    			reader.reportError(s.str()); 
    	}
    }

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::ETYP);
    reader.readSubrecord(equipmentType);

    int snamRead = 0;
    while(reader.peekNextType() != ESMType::DATA) {
    	reader.readNextSubrecordHeader();
    	switch(reader.subrecordType()) {
    		case ESMType::BIPL:
    			reader.readSubrecord(bipedModelList);
    			break;
    		case ESMType::YNAM:
    			reader.readSubrecord(soundPickUp);
    			break;
    		case ESMType::ZNAM:
    			reader.readSubrecord(soundDrop);
    			break;
    		case ESMType::MOD2:
			    ModelData::load(reader, shellCasingModelData, 1);
    			break;
    		case ESMType::MOD3:
			    ModelData::load(reader, scopeModelData, 2);
    			break;
    		case ESMType::EFSD:
    			reader.readSubrecord(scopeEffect);
    			break;
    		case ESMType::MOD4:
    			ModelData::load(reader, scopeEffectModelData, 3);
    			break;
    		case ESMType::MWD1:
    			reader.readStringSubrecord(modelWithMod1);
    			break;
    		case ESMType::MWD2:
    			reader.readStringSubrecord(modelWithMod2);
    			break;
    		case ESMType::MWD3:
    			reader.readStringSubrecord(modelWithMod12);
    			break;
    		case ESMType::MWD4:
    			reader.readStringSubrecord(modelWithMod3);    		
    			break;
    		case ESMType::MWD5:
    			reader.readStringSubrecord(modelWithMod13);    		
    			break;
    		case ESMType::MWD6:
    			reader.readStringSubrecord(modelWithMod23);    		
    			break;
    		case ESMType::MWD7:
    			reader.readStringSubrecord(modelWithMod123);    		
    			break;
    		case ESMType::VANM:
    			reader.readStringSubrecord(vatsAttackName);
    			break;
    		case ESMType::NNAM:
    			reader.readStringSubrecord(embeddedWeaponNode);    			
    			break;
    		case ESMType::INAM:
    			reader.readSubrecord(impactDataset);
    			break;
    		case ESMType::WNAM:
    			reader.readSubrecord(firstPersonModel);
    			break;
    		case ESMType::WNM1:
    			reader.readSubrecord(firstPersonModelWithMod1);
    			break;
    		case ESMType::WNM2:
    			reader.readSubrecord(firstPersonModelWithMod2);
    			break;
    		case ESMType::WNM3:
    			reader.readSubrecord(firstPersonModelWithMod12);
    			break;
    		case ESMType::WNM4:
    			reader.readSubrecord(firstPersonModelWithMod3);
    			break;
    		case ESMType::WNM5:
    			reader.readSubrecord(firstPersonModelWithMod13);
    			break;
    		case ESMType::WNM6:
    			reader.readSubrecord(firstPersonModelWithMod23);
    			break;
    		case ESMType::WNM7:
    			reader.readSubrecord(firstPersonModelWithMod123);
    			break;
    		case ESMType::WMI1:
    			reader.readSubrecord(weaponMod1);
    			break;
    		case ESMType::WMI2:
    			reader.readSubrecord(weaponMod2);
    			break;
    		case ESMType::WMI3:
    			reader.readSubrecord(weaponMod3);
    			break;
    		case ESMType::SNAM:
    			if(snamRead == 0) {
    				snamRead++;
    				reader.readSubrecord(soundGunShoot3D);
    			} else if(snamRead == 1 ) {
    				reader.readSubrecord(soundGunShootDist);    				
    			} else {
    				std::stringstream s;
    				s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    				reader.reportError(s.str()); 
    			}
    			break;
    		case ESMType::XNAM:
    			reader.readSubrecord(soundGunShoot2D);
    			break;
    		case ESMType::NAM7:
				reader.readSubrecord(soundGunShoot3DLooping);
    			break;
    		case ESMType::TNAM:
    			reader.readSubrecord(soundMeleeSwing);
    			break;
    		case ESMType::NAM6:
    			reader.readSubrecord(soundBlock);
    			break;
    		case ESMType::UNAM:
    			reader.readSubrecord(soundIdle);
    			break;
    		case ESMType::NAM9:
    			reader.readSubrecord(soundEquip);
    			break;
    		case ESMType::NAM8:
    			reader.readSubrecord(soundUnequip);
    			break;
    		case ESMType::WMS1:
    			reader.readSubrecord(soundMod1Shoot3D);
    			break;
    		case ESMType::WMS2:
    			reader.readSubrecord(soundMod1Shoot2D);
    			break;
    		default:
    			std::stringstream s;
    			s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
    			reader.reportError(s.str()); 
    	}
    }
    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DATA);
    reader.readSubrecord(data);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::DNAM);
    reader.readSubrecord(attributes);

    reader.readNextSubrecordHeader();
    reader.checkSubrecordHeader(ESMType::CRDT);
    reader.readSubrecord(criticalData);

    reader.readNextSubrecordHeader();
    if(reader.subrecordType() == ESMType::VATS) {
    	reader.readSubrecord(vats);
    	reader.readNextSubrecordHeader();
    }
    reader.checkSubrecordHeader(ESMType::VNAM);
    reader.readSubrecord(soundLevel);
    
}

void CriticalData::load(ESMReader& reader, CriticalData& criticalData) {
	if(reader.getCurrentSubrecord().dataSize == 12) {
		reader.readSubrecord(*reinterpret_cast<CriticalData::CriticalDataNullEffect*>(&criticalData));
	} else {
		reader.readSubrecord(criticalData);
	}
}

void VATSData::load(ESMReader& reader, VATSData& vats) {
	if(reader.getCurrentSubrecord().dataSize == 16) {
		reader.readSubrecord(*reinterpret_cast<VATSData::VATSDataNullEffect*>(&(vats.skill)));
	} else {
		reader.readSubrecord(vats);
	}
}

};