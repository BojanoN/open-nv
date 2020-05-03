#include "imad.hpp"

namespace ESM {

ImageSpaceAdapter::ImageSpaceAdapter(ESMReader& reader) : Record(reader) {
	while(reader.hasMoreSubrecords()) {
		reader.readNextSubrecordHeader();
		switch(reader.subrecordType()) {
			case ESMType::EDID:
				reader.readStringSubrecord(editorId);
				break;
			case ESMType::DNAM:
				reader.readSubrecord(counts);
				break;
			case ESMType::BNAM:
				reader.readArraySubrecord(blurRadius);
				break;
			case ESMType::VNAM:
				reader.readArraySubrecord(doubleVisionStrength);
				break;
			case ESMType::TNAM:
				reader.readArraySubrecord(tintColor);
				break;
			case ESMType::NAM3:
    			reader.readArraySubrecord(fadeColor);
				break;
			case ESMType::RNAM:
				reader.readArraySubrecord(radialBlurStrength);
				break;
			case ESMType::SNAM:
				reader.readArraySubrecord(radialBlurRampUp);
				break;
			case ESMType::UNAM:
				reader.readArraySubrecord(radialBlurStart);
				break;
			case ESMType::NAM1:
				reader.readArraySubrecord(radialBlurRampDown);
				break;
			case ESMType::NAM2:
				reader.readArraySubrecord(radialBlurDownStart);
				break;
			case ESMType::WNAM:
				reader.readArraySubrecord(depthOfFieldStrength);
				break;
			case ESMType::XNAM:
				reader.readArraySubrecord(depthOfFieldDistance);
				break;
			case ESMType::YNAM:
				reader.readArraySubrecord(depthOfFieldRange);
				break;
			case ESMType::NAM4:
				reader.readArraySubrecord(motionBlurStrength);
				break;
			case ESMType::IAD_BASE + 0x00:
				reader.readArraySubrecord(eyeAdaptSpeedMult);
				break;
			case ESMType::AT_IAD:
				reader.readArraySubrecord(eyeAdaptSpeedAdd);
				break;
			case ESMType::IAD_BASE + 0x01:
				reader.readArraySubrecord(bloomBlurRadiusMult);
				break;
			case ESMType::AIAD:
				reader.readArraySubrecord(bloomBlurRadiusAdd);
				break;
			case ESMType::IAD_BASE + 0x02:
				reader.readArraySubrecord(bloomThresholdMult);
				break;
			case ESMType::BIAD:
				reader.readArraySubrecord(bloomThresholdAdd);
				break;
			case ESMType::IAD_BASE + 0x03:
				reader.readArraySubrecord(bloomScaleMult);
				break;
			case ESMType::CIAD:
				reader.readArraySubrecord(bloomScaleAdd);
				break;
			case ESMType::IAD_BASE + 0x04:
				reader.readArraySubrecord(targetLUMMinMult);
				break;
			case ESMType::DIAD:
				reader.readArraySubrecord(targetLUMMinAdd);
				break;
			case ESMType::IAD_BASE + 0x05:
				reader.readArraySubrecord(targetLUMMaxMult);
				break;
			case ESMType::EIAD:
				reader.readArraySubrecord(targetLUMMaxAdd);
				break;
			case ESMType::IAD_BASE + 0x06:
				reader.readArraySubrecord(sunlightScaleMult);
				break;
			case ESMType::FIAD:
				reader.readArraySubrecord(sunlightScaleAdd);
				break;
			case ESMType::IAD_BASE + 0x07:
				reader.readArraySubrecord(skyScaleMult);
				break;
			case ESMType::GIAD:
				reader.readArraySubrecord(skyScaleAdd);
				break;
			case ESMType::IAD_BASE + 0x08:case ESMType::HIAD:	
			case ESMType::IAD_BASE + 0x09:case ESMType::IIAD:	
			case ESMType::IAD_BASE + 0x0A:case ESMType::JIAD:	
			case ESMType::IAD_BASE + 0x0B:case ESMType::KIAD:	
			case ESMType::IAD_BASE + 0x0C:case ESMType::LIAD:	
			case ESMType::IAD_BASE + 0x0D:case ESMType::MIAD:	
			case ESMType::IAD_BASE + 0x0E:case ESMType::NIAD:	
			case ESMType::IAD_BASE + 0x0F:case ESMType::OIAD:	
			case ESMType::IAD_BASE + 0x10:case ESMType::PIAD:	
			case ESMType::IAD_BASE + 0x14:case ESMType::TIAD:
				reader.skipSubrecord();
				break;
			case ESMType::IAD_BASE + 0x11:
				reader.readArraySubrecord(cinematicSaturationMult);
				break;
			case ESMType::QIAD:
				reader.readArraySubrecord(cinematicSaturationAdd);
				break;
			case ESMType::IAD_BASE + 0x12:
				reader.readArraySubrecord(cinematicBrightnessMult);
				break;
			case ESMType::RIAD:
				reader.readArraySubrecord(cinematicBrightnessAdd);
				break;
			case ESMType::IAD_BASE + 0x13:
				reader.readArraySubrecord(cinematicContrastMult);
				break;
			case ESMType::SIAD:
				reader.readArraySubrecord(cinematicContrastAdd);
				break;
			case ESMType::RDSD:
				reader.readSubrecord(soundIntro);
				break;
			case ESMType::RDSI:
				reader.readSubrecord(soundOutro);
				break;
      		default:
        		std::stringstream s;
        		s << "Invalid subrecord type " << Util::typeValueToName(reader.subrecordType()) << '\n';
        		throw std::runtime_error(s.str());
		}
	}
}

};