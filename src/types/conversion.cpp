#include "conversion.hpp"
#include "error/error.hpp"

namespace Types {

ErrorPair<uint64_t> parseUInt(const char* stringValue) {

	if(stringValue == NULL) {
		return ErrorPair<uint64_t>(Err::NullPointerError, 0);
	}

	uint32_t numberOfDigits = std::strlen(stringValue);

	if(numberOfDigits > maximumUnsignedDigits || numberOfDigits == 0) {
		return ErrorPair<uint64_t>(Err::NumberFormatError, 0);
	}

	uint64_t digitMagnitude = numberOfDigits;
	const char* digit = stringValue;
	uint64_t parsedValue = 0ull;

	while(*digit != '\0') {
		
		if(*digit < '0' || *digit > '9') {
			return ErrorPair<uint64_t>(Err::NumberFormatError, 0);
		}

		uint64_t nextValue = parsedValue + decdigits[(*digit - '0') * rowSize + digitMagnitude - 1];

		if(nextValue < parsedValue) {
			return ErrorPair<uint64_t>(Err::OutOfRange, 0);
		}

		parsedValue = nextValue;
		digitMagnitude--;
		digit++;
	}

	return ErrorPair<uint64_t>(Err::Success, parsedValue);
}

ErrorPair<int64_t> parseInt(const char* stringValue) {
	if(stringValue == NULL) {
		return ErrorPair<int64_t>(Err::NullPointerError, 0);
	}

	bool negative = false;
	uint32_t numberOfDigits;
	const char* digit;

	if(stringValue[0] == '-') {
		
		negative = true;
		numberOfDigits = std::strlen(stringValue) - 1;
		digit = stringValue + 1;

	} else {
		numberOfDigits = std::strlen(stringValue);
		digit = stringValue;
	}

	if(numberOfDigits > maximumSignedDigits || numberOfDigits == 0) {
		return ErrorPair<int64_t>(Err::NumberFormatError, 0);
	}

	uint64_t digitMagnitude = numberOfDigits;
	int64_t parsedValue = 0ll;

	while(*digit != '\0') {
		
		if(*digit < '0' || *digit > '9') {
			return ErrorPair<int64_t>(Err::NumberFormatError, 0);
		}

		int64_t nextValue = parsedValue + decdigits[(*digit - '0') * rowSize + digitMagnitude - 1];

		if(nextValue < parsedValue) {
			return ErrorPair<int64_t>(Err::OutOfRange, 0);
		}

		parsedValue = nextValue;
		digitMagnitude--;
		digit++;
	}

	if(negative) {
		return ErrorPair<int64_t>(Err::Success, -parsedValue);
	} else {
		return ErrorPair<int64_t>(Err::Success, parsedValue);
	}
}


ErrorPair<float> parseFloat(const char* stringValue) {
	if(stringValue == NULL) {
		return ErrorPair<float>(Err::NullPointerError, 0);
	}

	bool negative = false;
	uint32_t numberOfDigits;
	const char* digit;

	if(stringValue[0] == '-') {
		
		negative = true;
		numberOfDigits = std::strlen(stringValue) - 1;
		digit = stringValue + 1;

	} else {
		numberOfDigits = std::strlen(stringValue);
		digit = stringValue;
	}

	if (numberOfDigits == 0) {
		return ErrorPair<float>(1, 0.0f);
	}

	if(numberOfDigits > floatSignificantDigits) {
		numberOfDigits = floatSignificantDigits;
	}

	float parsedValue = 0.0f;
	bool havePassedDecimalPoint = false;
	float currentDecimalDivisor = 1.0f;
	bool digitFound = false;

	while(*digit != '\0') {

		if((*digit < '0' || *digit > '9') && *digit != '.') {
			return ErrorPair<float>(Err::NumberFormatError, 0);
		}

		if(numberOfDigits == 0) {
			if(havePassedDecimalPoint || *digit == '.') {
				break;
			} else {
				parsedValue *= 10.0f;
			}
		} else {

			if(!havePassedDecimalPoint) {
				if(*digit == '.') {
					havePassedDecimalPoint = true;
					numberOfDigits++;


				} else {
					parsedValue *= 10;
					uint8_t digitValue = (*digit - '0');
					if(!digitFound && digitValue != 0) {
						digitFound = true;
					}
					parsedValue += (float)(digitValue);
					
					if(!digitFound) {
						numberOfDigits++;
					}
				}

				

			} else {
				if(*digit == '.') {
					return ErrorPair<float>(Err::NumberFormatError, 0.0f);
				} else {
					currentDecimalDivisor /= 10.0f;

					uint8_t digitValue = (*digit - '0');
					if(!digitFound && digitValue != 0) {
						digitFound = true;
					}

					parsedValue += (float)(digitValue) * currentDecimalDivisor;

					if(!digitFound) {
						numberOfDigits++;
					}
				}

			}
			numberOfDigits--;
		}

		if(parsedValue == HUGE_VAL) {
			if(negative) {
				return ErrorPair<float>(Err::FloatInfinity, -HUGE_VAL);
			} 
			return ErrorPair<float>(Err::FloatInfinity, HUGE_VAL);
		}
		
		digit++;
	}

	if(negative) {
		return ErrorPair<float>(Err::Success, -parsedValue);
	} else {
		return ErrorPair<float>(Err::Success, parsedValue);
	}
} 

};