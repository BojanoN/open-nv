#include "../conversion.hpp"
#include "types/errorpair.hpp"
#include <cstdint>
#include <cstdio>

using Types::ErrorPair;
using Types::parseUInt;
using Types::parseInt;
using Types::parseFloat;

static const char* nullString = NULL;
static const char* emptyString = "";

static const char* strZero = "0";
static const char* strMaxUInt = "18446744073709551615";
static const char* strMiddleUInt = "9223372036854775808";
static const char* strOverflowUInt = "18446744073709551616";

static const uint64_t uIntZero = 0ull;
static const uint64_t uIntMax = 18446744073709551615ull;
static const uint64_t uIntMiddle = 9223372036854775808ull;

static const char* strMaxInt = "9223372036854775807";
static const char* strMinInt = "-9223372036854775807";
static const char* strOverflowInt = "9223372036854775808";
static const char* strUnderflowInt = "-9223372036854775808";

static const uint64_t intZero = 0ll;
static const int64_t intMax = 9223372036854775807ll;
static const int64_t intMin = -9223372036854775807ll;

static const char* strFloatZero = "0.0";
static const char* strFloatPositive = "420.0";
static const char* strFloatPositiveDecimal = "420.69";
static const char* strFloatNegative = "-420.69";
static const char* strFloatNegativeDecimal = "-420.69";
static const char* strFloatSubZeroPositive = "0.69";
static const char* strFloatSubZeroNegative = "-0.69";

static const char* strFloatSevenDigits = "1234567.0";
static const char* strFloatEightDigits = "12345678.0";
static const char* strFloatSevenDigitsHigh = "1234567000.0";

static const char* strFloatSevenDigitsNegative = "-1234567.0";
static const char* strFloatEightDigitsNegative = "-12345678.0";
static const char* strFloatSevenDigitsHighNegative = "-1234567000.0";

static const char* strFloatSevenDigitsSubZero = "0.1234567";
static const char* strFloatEightDigitsSubZero = "0.12345678";
static const char* strFloatSevenDigitsSubZeroHigh = "0.0001234567";

static const char* strFloatSevenDigitsSubZeroNegative = "-0.1234567";
static const char* strFloatEightDigitsSubZeroNegative = "-0.12345678";
static const char* strFloatSevenDigitsSubZeroHighNegative = "-0.0001234567";

static const float floatZero = 0.0f;
static const float floatPositive = 420.0f;
static const float floatPositiveDecimal = 420.69f;
static const float floatNegative = -420.69f;
static const float floatNegativeDecimal = -420.69f;

static const float floatSubZeroPositive = 0.69f;
static const float floatSubZeroNegative = -0.69f;

static const float sevenDigits = 1234567.0f;
static const float eightDigits = 12345670.0f;
static const float sevenDigitsHigh = 1234567000.0f;

static const float sevenDigitsNegative = -1234567.0f;
static const float eightDigitsNegative = -12345670.0f;
static const float sevenDigitsHighNegative = -1234567000.0f;

static const float sevenDigitsSubZero = 0.1234567f;
static const float eightDigitsSubZero = 0.12345670f;
static const float sevenDigitsSubZeroHigh = 0.0001234567f;

static const float sevenDigitsSubZeroNegative = -0.1234567f;
static const float eightDigitsSubZeroNegative = -0.12345670f;
static const float sevenDigitsSubZeroHighNegative = -0.0001234567f;


static const char* _dots =
    ".........................................................................."
    "......";
#define STRLEN_PASSED (6)
#define STRLEN_FAILED_AT_LINE (14)

#define GAMAL_ABDUL_NASSERT(expression)                                       \
  {                                                                           \
    if ((expression)) {                                                       \
      std::printf("%s%spassed\n", __FUNCTION__,                               \
                  &_dots[std::strlen(__FUNCTION__) + STRLEN_PASSED]);         \
    } else {                                                                  \
      unsigned int length = 0;                                                \
      for (unsigned int i = __LINE__; i > 0; i /= 10) {                        \
        length += 1;                                                          \
      }                                                                       \
      std::printf(                                                            \
          "%s%s failed at line %d\n", __FUNCTION__,                           \
          &_dots[std::strlen(__FUNCTION__) + length + STRLEN_FAILED_AT_LINE], \
          __LINE__);                                                          \
    }                                                                         \
  };

static void test_uIntConvertZero();
static void test_uIntConvertMax();
static void test_uIntConvertMiddle();

static void test_uInt();

static void test_run();


static void test_uIntNullStringError() {
	ErrorPair<uint64_t> result = parseUInt(nullString);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntEmptyStringError() {
	ErrorPair<uint64_t> result = parseUInt(emptyString);
	GAMAL_ABDUL_NASSERT(result.fail());
}


static void test_uIntConvertZeroNoError() {
	ErrorPair<uint64_t> result = parseUInt(strZero);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_uIntConvertZeroValue() {
	ErrorPair<uint64_t> result = parseUInt(strZero);
	GAMAL_ABDUL_NASSERT(result.value == uIntZero);
}

static void test_uIntConvertMiddleNoError() {
	ErrorPair<uint64_t> result = parseUInt(strMiddleUInt);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_uIntConvertMiddleValue() {
	ErrorPair<uint64_t> result = parseUInt(strMiddleUInt);
	GAMAL_ABDUL_NASSERT(result.value == uIntMiddle);
}

static void test_uIntConvertMaxNoError() {
	ErrorPair<uint64_t> result = parseUInt(strMaxUInt);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_uIntConvertMaxValue() {
	ErrorPair<uint64_t> result = parseUInt(strMaxUInt);
	GAMAL_ABDUL_NASSERT(result.value == uIntMax);
}

static void test_uIntOverflow() {
	ErrorPair<uint64_t> result = parseUInt(strOverflowUInt);
	GAMAL_ABDUL_NASSERT(result.fail());
}


static void test_uInt() {
	test_uIntNullStringError();
	test_uIntEmptyStringError();
	test_uIntConvertZeroNoError();
	test_uIntConvertZeroValue();
	test_uIntConvertMiddleNoError();
	test_uIntConvertMiddleValue();
	test_uIntConvertMaxNoError();
	test_uIntConvertMaxValue();
	test_uIntOverflow();
}

static void test_intNullStringError() {
	ErrorPair<int64_t> result = parseInt(nullString);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intEmptyStringError() {
	ErrorPair<int64_t> result = parseInt(emptyString);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intConvertZeroNoError() {
	ErrorPair<int64_t> result = parseInt(strZero);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_intConvertZeroValue() {
	ErrorPair<int64_t> result = parseInt(strZero);
	GAMAL_ABDUL_NASSERT(result.value == intZero);
}

static void test_intConvertMaxNoError() {
	ErrorPair<int64_t> result = parseInt(strMaxInt);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_intConvertMaxValue() {
	ErrorPair<int64_t> result = parseInt(strMaxInt);
	GAMAL_ABDUL_NASSERT(result.value == intMax);
}

static void test_intConvertMinNoError() {
	ErrorPair<int64_t> result = parseInt(strMinInt);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_intConvertMinValue() {
	ErrorPair<int64_t> result = parseInt(strMinInt);
	GAMAL_ABDUL_NASSERT(result.value == intMin);
}

static void test_intOverflow() {
	ErrorPair<int64_t> result = parseInt(strOverflowInt);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intUnderflow() {
	ErrorPair<int64_t> result = parseInt(strUnderflowInt);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_int() {
	test_intNullStringError();
	test_intEmptyStringError();
	test_intConvertZeroNoError();
	test_intConvertZeroValue();
	test_intConvertMaxNoError();
	test_intConvertMaxValue();
	test_intConvertMinNoError();
	test_intConvertMinValue();
	test_intOverflow();
	test_intUnderflow();
}

static void test_floatNullStringError() {
	ErrorPair<float> result = parseFloat(nullString);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatEmptyStringError() {
	ErrorPair<float> result = parseFloat(emptyString);
	GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatConvertZeroNoError() {
	ErrorPair<float> result = parseFloat(strFloatZero);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertZeroValue() {
	ErrorPair<float> result = parseFloat(strFloatZero);
	GAMAL_ABDUL_NASSERT(result.value == floatZero);
}

static void test_floatConvertPositiveNoError() {
	ErrorPair<float> result = parseFloat(strFloatPositive);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertPositiveValue() {
	ErrorPair<float> result = parseFloat(strFloatPositive);
	GAMAL_ABDUL_NASSERT(result.value == floatPositive);
}

static void test_floatConvertPositiveDecimalNoError() {
	ErrorPair<float> result = parseFloat(strFloatPositiveDecimal);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertPositiveDecimalValue() {
	ErrorPair<float> result = parseFloat(strFloatPositiveDecimal);
	GAMAL_ABDUL_NASSERT(result.value == floatPositiveDecimal);
}

static void test_floatConvertNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatNegative);
	GAMAL_ABDUL_NASSERT(result.value == floatNegative);
}

static void test_floatConvertNegativeDecimalNoError() {
	ErrorPair<float> result = parseFloat(strFloatNegativeDecimal);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertNegativeDecimalValue() {
	ErrorPair<float> result = parseFloat(strFloatNegativeDecimal);
	GAMAL_ABDUL_NASSERT(result.value == floatNegativeDecimal);
}

static void test_floatConvertSubZeroPositiveNoError() {
	ErrorPair<float> result = parseFloat(strFloatSubZeroPositive);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSubZeroPositiveValue() {
	ErrorPair<float> result = parseFloat(strFloatSubZeroPositive);
	GAMAL_ABDUL_NASSERT(result.value == floatSubZeroPositive);
}

static void test_floatConvertSubZeroNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatSubZeroNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSubZeroNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatSubZeroNegative);
	GAMAL_ABDUL_NASSERT(result.value == floatSubZeroNegative);
}

static void test_floatConvertSevenDigitsNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigits);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigits);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigits);
}

static void test_floatConvertEightDigitsNoError() {
	ErrorPair<float> result = parseFloat(strFloatEightDigits);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertEightDigitsValue() {
	ErrorPair<float> result = parseFloat(strFloatEightDigits);
	GAMAL_ABDUL_NASSERT(result.value == eightDigits);
}

static void test_floatConvertSevenDigitsHighNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsHigh);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsHighValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsHigh);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsHigh);
}

static void test_floatConvertSevenDigitsNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsNegative);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsNegative);
}

static void test_floatConvertEightDigitsNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertEightDigitsNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsNegative);
	GAMAL_ABDUL_NASSERT(result.value == eightDigitsNegative);
}

static void test_floatConvertSevenDigitsHighNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsHighNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsHighNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsHighNegative);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsHighNegative);
}

static void test_floatConvertSevenDigitsSubZeroNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZero);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsSubZeroValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZero);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsSubZero);
}

static void test_floatConvertEightDigitsSubZeroNoError() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsSubZero);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertEightDigitsSubZeroValue() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsSubZero);
	GAMAL_ABDUL_NASSERT(result.value == eightDigitsSubZero);
}

static void test_floatConvertSevenDigitsSubZeroHighNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroHigh);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsSubZeroHighValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroHigh);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsSubZeroHigh);
}

static void test_floatConvertSevenDigitsSubZeroNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsSubZeroNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroNegative);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsSubZeroNegative);
}

static void test_floatConvertEightDigitsSubZeroNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsSubZeroNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertEightDigitsSubZeroNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatEightDigitsSubZeroNegative);
	GAMAL_ABDUL_NASSERT(result.value == eightDigitsSubZeroNegative);
}

static void test_floatConvertSevenDigitsSubZeroHighNegativeNoError() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroHighNegative);
	GAMAL_ABDUL_NASSERT(!result.fail());
}

static void test_floatConvertSevenDigitsSubZeroHighNegativeValue() {
	ErrorPair<float> result = parseFloat(strFloatSevenDigitsSubZeroHighNegative);
	GAMAL_ABDUL_NASSERT(result.value == sevenDigitsSubZeroHighNegative);
}

static void test_float() {
	test_floatNullStringError();
	test_floatEmptyStringError();
	test_floatConvertZeroNoError();	
	test_floatConvertZeroValue();
	test_floatConvertPositiveNoError();	
	test_floatConvertPositiveValue();
	test_floatConvertPositiveDecimalNoError();	
	test_floatConvertPositiveDecimalValue();
	test_floatConvertNegativeNoError();	
	test_floatConvertNegativeValue();
	test_floatConvertNegativeDecimalNoError();	
	test_floatConvertNegativeDecimalValue();
	test_floatConvertSubZeroPositiveNoError();
	test_floatConvertSubZeroPositiveValue();
	test_floatConvertSubZeroNegativeNoError();
	test_floatConvertSubZeroNegativeValue();
	test_floatConvertSevenDigitsNoError();
	test_floatConvertSevenDigitsValue();
	test_floatConvertEightDigitsNoError();
	test_floatConvertEightDigitsValue();
	test_floatConvertSevenDigitsHighNoError();
	test_floatConvertSevenDigitsHighValue();
	test_floatConvertSevenDigitsNegativeNoError();
	test_floatConvertSevenDigitsNegativeValue();
	test_floatConvertEightDigitsNegativeNoError();
	test_floatConvertEightDigitsNegativeValue();
	test_floatConvertSevenDigitsHighNegativeNoError();
	test_floatConvertSevenDigitsHighNegativeValue();
	test_floatConvertSevenDigitsSubZeroNoError();
	test_floatConvertSevenDigitsSubZeroValue();
	test_floatConvertEightDigitsSubZeroNoError();
	test_floatConvertEightDigitsSubZeroValue();
	test_floatConvertSevenDigitsSubZeroHighNoError();
	test_floatConvertSevenDigitsSubZeroHighValue();
	test_floatConvertSevenDigitsSubZeroNegativeNoError();
	test_floatConvertSevenDigitsSubZeroNegativeValue();
	test_floatConvertEightDigitsSubZeroNegativeNoError();
	test_floatConvertEightDigitsSubZeroNegativeValue();
	test_floatConvertSevenDigitsSubZeroHighNegativeNoError();
	test_floatConvertSevenDigitsSubZeroHighNegativeValue();
}

static void test_run() {
	test_uInt();
	test_int();
	test_float();
}

int main(int argc, char const *argv[])
{
	test_run();
	return 0;
}