#include <cstdint>
#include "../configuration.hpp"
#include "types/errorpair.hpp"
#include "error/error.hpp"

using Err::Error;

static File::Configuration configuration;

static uint64_t uintValue = 69;
static uint64_t reUIntValue = 420;
static const char* uIntConfigurationName = "uPenisSize";
static const char* invalidUIntConfigurationName = "PenisSize";

static int64_t intValue = -69;
static int64_t reIntValue = -420;
static const char* intConfigurationName = "iNippleProtrusionApex";
static const char* invalidIntConfigurationName = "NippleProtrusionApex";

static float floatValue = 4.20f;
static float reFloatValue = -6.9f;
static const char* floatConfigurationName = "fErectionAngleModifier";
static const char* invalidFloatConfigurationName = "ErectionAngleModifier";

static bool boolValue = true;
static bool reBoolValue = false;
static const char* boolConfigurationName = "bHomosexualityStatus";
static const char* invalidBoolConfigurationName = "HomosexualityStatus";

static const char* stringValue = "Autonomna pokrajina Kosovo i Metohija";
static const char* sameSizeStringValue =
    "aUTONOMNA POKRAJINA kOSOVO I mETOHIJA";
static const char* longerStringValue =
    "Socijalisticka autonomna pokrajina Kosovo i Metohija";
static const char* reStringValue = "Republika Kosovo";
static const char* stringConfigurationName = "sBait";
static const char* invalidStringConfigurationName = "Bait";

static const char* emptyName = "";

static const char* _dots =
    ".........................................................................."
    "......";
#define STRLEN_PASSED (6)
#define STRLEN_FAILED_AT_LINE (14)

#define GAMAL_ABDUL_NASSERT(expression)                                      \
  {                                                                          \
    if ((expression)) {                                                      \
      std::printf("%s%spassed\n", __FUNCTION__,                                \
                  &_dots[std::strlen(__FUNCTION__) + STRLEN_PASSED]);         \
    } else {                                                                 \
      unsigned int length = 0;                                               \
      for (unsigned int i = __LINE__; i > 0; i /= 10) {                       \
        length += 1;                                                         \
      }                                                                      \
      std::printf(                                                           \
          "%s%s failed at line %d\n", __FUNCTION__,                            \
          &_dots[std::strlen(__FUNCTION__) + length + STRLEN_FAILED_AT_LINE], \
          __LINE__);                                                         \
    }                                                                        \
  };

static void test_uIntCannotGetValueEmptyName();
static void test_uIntCannotGetValueForNameWithInvalidPrefix();
static void test_uIntNoValueBeforeSet();
static void test_uIntCannotSetValueEmptyName();
static void test_uIntCannotSetValueForNameWithInvalidPrefix();
static void test_uIntCanSetValueForNameWithValidPrefix();
static void test_uIntCanGetPreviouslySetValueForNameWithValidPrefix();
static void test_uIntWillReturnCorrectValue();
static void test_uIntCanReplaceValue();
static void test_uIntWillReturnCorrectValueAfterReplacement();

static void test_uIntRunTests();

static void test_intCannotGetValueEmptyName();
static void test_intCannotGetValueForNameWithInvalidPrefix();
static void test_intNoValueBeforeSet();
static void test_intCannotSetValueEmptyName();
static void test_intCannotSetValueForNameWithInvalidPrefix();
static void test_intCanSetValueForNameWithValidPrefix();
static void test_intCanGetPreviouslySetValueForNameWithValidPrefix();
static void test_intWillReturnCorrectValue();
static void test_intCanReplaceValue();
static void test_intWillReturnCorrectValueAfterReplacement();

static void test_intRunTests();

static void test_floatCannotGetValueEmptyName();
static void test_floatCannotGetValueForNameWithInvalidPrefix();
static void test_floatNoValueBeforeSet();
static void test_floatCannotSetValueEmptyName();
static void test_floatCannotSetValueForNameWithInvalidPrefix();
static void test_floatCanSetValueForNameWithValidPrefix();
static void test_floatCanGetPreviouslySetValueForNameWithValidPrefix();
static void test_floatWillReturnCorrectValue();
static void test_floatCanReplaceValue();
static void test_floatWillReturnCorrectValueAfterReplacement();

static void test_floatRunTests();

static void test_boolCannotGetValueEmptyName();
static void test_boolCannotGetValueForNameWithInvalidPrefix();
static void test_boolNoValueBeforeSet();
static void test_boolCannotSetValueEmptyName();
static void test_boolCannotSetValueForNameWithInvalidPrefix();
static void test_boolCanSetValueForNameWithValidPrefix();
static void test_boolCanGetPreviouslySetValueForNameWithValidPrefix();
static void test_boolWillReturnCorrectValue();
static void test_boolCanReplaceValue();
static void test_boolWillReturnCorrectValueAfterReplacement();

static void test_boolRunTests();

static void test_stringCannotGetValueEmptyName();
static void test_stringCannotGetValueForNameWithInvalidPrefix();
static void test_stringNoValueBeforeSet();
static void test_stringCannotSetValueEmptyName();
static void test_stringCannotSetValueForNameWithInvalidPrefix();
static void test_stringCanSetValueForNameWithValidPrefix();
static void test_stringCanGetPreviouslySetValueForNameWithValidPrefix();
static void test_stringWillReturnCorrectValue();
static void test_stringCanReplaceValue();
static void test_stringWillReturnCorrectValueAfterReplacement();

static void test_stringRunTests();

static void test_uIntNoValueBeforeSet() {
  Types::ErrorPair result = configuration.nGetUInt(uIntConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntCannotSetValueEmptyName() {
  Error result = configuration.nSetUInt(emptyName, uintValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntCannotGetValueEmptyName() {
  Types::ErrorPair result = configuration.nGetUInt(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntCannotSetValueForNameWithInvalidPrefix() {
  Error result = configuration.nSetUInt(invalidUIntConfigurationName, uintValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntCannotGetValueForNameWithInvalidPrefix() {
  Types::ErrorPair result =
      configuration.nGetUInt(invalidUIntConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_uIntCanSetValueForNameWithValidPrefix() {
  Error result = configuration.nSetUInt(uIntConfigurationName, uintValue);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_uIntCanGetPreviouslySetValueForNameWithValidPrefix() {
  Error setResult = configuration.nSetUInt(uIntConfigurationName, uintValue);
  Types::ErrorPair getResult = configuration.nGetUInt(uIntConfigurationName);
  GAMAL_ABDUL_NASSERT(!getResult.fail());
}

static void test_uIntWillReturnCorrectValue() {
  Error setResult = configuration.nSetUInt(uIntConfigurationName, uintValue);
  Types::ErrorPair getResult = configuration.nGetUInt(uIntConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == uintValue);
}

static void test_uIntCanReplaceValue() {
  Error firstSetResult =
      configuration.nSetUInt(uIntConfigurationName, uintValue);
  Error secondSetResult =
      configuration.nSetUInt(uIntConfigurationName, reUIntValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_uIntWillReturnCorrectValueAfterReplacement() {
  Error firstSetResult =
      configuration.nSetUInt(uIntConfigurationName, uintValue);
  Error secondSetResult =
      configuration.nSetUInt(uIntConfigurationName, reUIntValue);
  Types::ErrorPair getResult = configuration.nGetUInt(uIntConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == reUIntValue);
}

static void test_uIntRunTests() {
  test_uIntNoValueBeforeSet();
  test_uIntCannotSetValueEmptyName();
  test_uIntCannotSetValueForNameWithInvalidPrefix();
  test_uIntCannotGetValueEmptyName();
  test_uIntCannotGetValueForNameWithInvalidPrefix();
  test_uIntCanSetValueForNameWithValidPrefix();
  test_uIntCanGetPreviouslySetValueForNameWithValidPrefix();
  test_uIntWillReturnCorrectValue();
  test_uIntCanReplaceValue();
  test_uIntWillReturnCorrectValueAfterReplacement();
}

static void test_intNoValueBeforeSet() {
  Types::ErrorPair result = configuration.nGetInt(intConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intCannotSetValueEmptyName() {
  Error result = configuration.nSetInt(emptyName, intValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intCannotGetValueEmptyName() {
  Types::ErrorPair result = configuration.nGetInt(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intCannotSetValueForNameWithInvalidPrefix() {
  Error result = configuration.nSetInt(invalidUIntConfigurationName, intValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intCannotGetValueForNameWithInvalidPrefix() {
  Types::ErrorPair result = configuration.nGetInt(invalidUIntConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_intCanSetValueForNameWithValidPrefix() {
  Error result = configuration.nSetInt(intConfigurationName, intValue);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_intCanGetPreviouslySetValueForNameWithValidPrefix() {
  Error setResult = configuration.nSetInt(intConfigurationName, intValue);
  Types::ErrorPair getResult = configuration.nGetInt(intConfigurationName);
  GAMAL_ABDUL_NASSERT(!getResult.fail());
}

static void test_intWillReturnCorrectValue() {
  Error setResult = configuration.nSetInt(intConfigurationName, intValue);
  Types::ErrorPair getResult = configuration.nGetInt(intConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == intValue);
}

static void test_intCanReplaceValue() {
  Error firstSetResult = configuration.nSetInt(intConfigurationName, intValue);
  Error secondSetResult =
      configuration.nSetInt(intConfigurationName, reIntValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_intWillReturnCorrectValueAfterReplacement() {
  Error firstSetResult = configuration.nSetInt(intConfigurationName, intValue);
  Error secondSetResult =
      configuration.nSetInt(intConfigurationName, reIntValue);
  Types::ErrorPair getResult = configuration.nGetInt(intConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == reIntValue);
}

static void test_intRunTests() {
  test_intNoValueBeforeSet();
  test_intCannotSetValueEmptyName();
  test_intCannotSetValueForNameWithInvalidPrefix();
  test_intCannotGetValueEmptyName();
  test_intCannotGetValueForNameWithInvalidPrefix();
  test_intCanSetValueForNameWithValidPrefix();
  test_intCanGetPreviouslySetValueForNameWithValidPrefix();
  test_intWillReturnCorrectValue();
  test_intCanReplaceValue();
  test_intWillReturnCorrectValueAfterReplacement();
}

static void test_floatNoValueBeforeSet() {
  Types::ErrorPair result = configuration.nGetFloat(floatConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatCannotSetValueEmptyName() {
  Error result = configuration.nSetFloat(emptyName, floatValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatCannotGetValueEmptyName() {
  Types::ErrorPair result = configuration.nGetFloat(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatCannotSetValueForNameWithInvalidPrefix() {
  Error result =
      configuration.nSetFloat(invalidFloatConfigurationName, floatValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatCannotGetValueForNameWithInvalidPrefix() {
  Types::ErrorPair result =
      configuration.nGetFloat(invalidFloatConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_floatCanSetValueForNameWithValidPrefix() {
  Error result = configuration.nSetFloat(floatConfigurationName, floatValue);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_floatCanGetPreviouslySetValueForNameWithValidPrefix() {
  Error setResult = configuration.nSetFloat(floatConfigurationName, floatValue);
  Types::ErrorPair getResult = configuration.nGetFloat(floatConfigurationName);
  GAMAL_ABDUL_NASSERT(!getResult.fail());
}

static void test_floatWillReturnCorrectValue() {
  Error setResult = configuration.nSetFloat(floatConfigurationName, floatValue);
  Types::ErrorPair getResult = configuration.nGetFloat(floatConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == floatValue);
}

static void test_floatCanReplaceValue() {
  Error firstSetResult =
      configuration.nSetFloat(floatConfigurationName, floatValue);
  Error secondSetResult =
      configuration.nSetFloat(floatConfigurationName, reFloatValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_floatWillReturnCorrectValueAfterReplacement() {
  Error firstSetResult =
      configuration.nSetFloat(floatConfigurationName, floatValue);
  Error secondSetResult =
      configuration.nSetFloat(floatConfigurationName, reFloatValue);
  Types::ErrorPair getResult = configuration.nGetFloat(floatConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == reFloatValue);
}

static void test_floatRunTests() {
  test_floatNoValueBeforeSet();
  test_floatCannotSetValueEmptyName();
  test_floatCannotSetValueForNameWithInvalidPrefix();
  test_floatCannotGetValueEmptyName();
  test_floatCannotGetValueForNameWithInvalidPrefix();
  test_floatCanSetValueForNameWithValidPrefix();
  test_floatCanGetPreviouslySetValueForNameWithValidPrefix();
  test_floatWillReturnCorrectValue();
  test_floatCanReplaceValue();
  test_floatWillReturnCorrectValueAfterReplacement();
}

static void test_boolNoValueBeforeSet() {
  Types::ErrorPair result = configuration.nGetBool(boolConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_boolCannotSetValueEmptyName() {
  Error result = configuration.nSetBool(emptyName, boolValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_boolCannotGetValueEmptyName() {
  Types::ErrorPair result = configuration.nGetBool(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_boolCannotSetValueForNameWithInvalidPrefix() {
  Error result = configuration.nSetBool(invalidBoolConfigurationName, boolValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_boolCannotGetValueForNameWithInvalidPrefix() {
  Types::ErrorPair result =
      configuration.nGetBool(invalidBoolConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_boolCanSetValueForNameWithValidPrefix() {
  Error result = configuration.nSetBool(boolConfigurationName, boolValue);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_boolCanGetPreviouslySetValueForNameWithValidPrefix() {
  Error setResult = configuration.nSetBool(boolConfigurationName, boolValue);
  Types::ErrorPair getResult = configuration.nGetBool(boolConfigurationName);
  GAMAL_ABDUL_NASSERT(!getResult.fail());
}

static void test_boolWillReturnCorrectValue() {
  Error setResult = configuration.nSetBool(boolConfigurationName, boolValue);
  Types::ErrorPair getResult = configuration.nGetBool(boolConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == boolValue);
}

static void test_boolCanReplaceValue() {
  Error firstSetResult =
      configuration.nSetBool(boolConfigurationName, boolValue);
  Error secondSetResult =
      configuration.nSetBool(boolConfigurationName, reBoolValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_boolWillReturnCorrectValueAfterReplacement() {
  Error firstSetResult =
      configuration.nSetBool(boolConfigurationName, boolValue);
  Error secondSetResult =
      configuration.nSetBool(boolConfigurationName, reBoolValue);
  Types::ErrorPair getResult = configuration.nGetBool(boolConfigurationName);
  GAMAL_ABDUL_NASSERT(getResult.value == reBoolValue);
}

static void test_boolRunTests() {
  test_boolNoValueBeforeSet();
  test_boolCannotSetValueEmptyName();
  test_boolCannotSetValueForNameWithInvalidPrefix();
  test_boolCannotGetValueEmptyName();
  test_boolCannotGetValueForNameWithInvalidPrefix();
  test_boolCanSetValueForNameWithValidPrefix();
  test_boolCanGetPreviouslySetValueForNameWithValidPrefix();
  test_boolWillReturnCorrectValue();
  test_boolCanReplaceValue();
  test_boolWillReturnCorrectValueAfterReplacement();
}

static void test_stringNoValueBeforeSet() {
  Types::ErrorPair result = configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_stringCannotSetValueEmptyName() {
  Error result = configuration.nSetString(emptyName, stringValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_stringCannotGetValueEmptyName() {
  Types::ErrorPair result = configuration.nGetString(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_stringCannotSetValueForNameWithInvalidPrefix() {
  Error result =
      configuration.nSetString(invalidStringConfigurationName, stringValue);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_stringCannotGetValueForNameWithInvalidPrefix() {
  Types::ErrorPair result =
      configuration.nGetString(invalidStringConfigurationName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_stringCanSetValueForNameWithValidPrefix() {
  Error result = configuration.nSetString(stringConfigurationName, stringValue);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_stringCanGetPreviouslySetValueForNameWithValidPrefix() {
  Error setResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Types::ErrorPair getResult =
      configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(!getResult.fail());
}

static void test_stringWillReturnCorrectValue() {
  Error setResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Types::ErrorPair getResult =
      configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(!std::strcmp(getResult.value, stringValue));
}

static void test_stringCanReplaceValueSameSizeString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, sameSizeStringValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_stringCanReplaceValueLongerString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, longerStringValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_stringCanReplaceValueShorterString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, reStringValue);
  GAMAL_ABDUL_NASSERT(secondSetResult.success());
}

static void test_stringWillReturnCorrectValueAfterReplacementSameSizeString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, sameSizeStringValue);
  Types::ErrorPair getResult =
      configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(!std::strcmp(getResult.value, sameSizeStringValue));
}

static void test_stringWillReturnCorrectValueAfterReplacementLongerString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, longerStringValue);
  Types::ErrorPair getResult =
      configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(!std::strcmp(getResult.value, longerStringValue));
}

static void test_stringWillReturnCorrectValueAfterReplacementShorterString() {
  Error firstSetResult =
      configuration.nSetString(stringConfigurationName, stringValue);
  Error secondSetResult =
      configuration.nSetString(stringConfigurationName, reStringValue);
  Types::ErrorPair getResult =
      configuration.nGetString(stringConfigurationName);
  GAMAL_ABDUL_NASSERT(!std::strcmp(getResult.value, reStringValue));
}

static void test_stringRunTests() {
  test_stringCannotGetValueEmptyName();
  test_stringCannotGetValueForNameWithInvalidPrefix();
  test_stringNoValueBeforeSet();
  test_stringCannotSetValueEmptyName();
  test_stringCannotSetValueForNameWithInvalidPrefix();
  test_stringCanSetValueForNameWithValidPrefix();
  test_stringCanGetPreviouslySetValueForNameWithValidPrefix();
  test_stringWillReturnCorrectValue();
  test_stringCanReplaceValueSameSizeString();
  test_stringCanReplaceValueLongerString();
  test_stringCanReplaceValueShorterString();
  test_stringWillReturnCorrectValueAfterReplacementSameSizeString();
  test_stringWillReturnCorrectValueAfterReplacementLongerString();
  test_stringWillReturnCorrectValueAfterReplacementShorterString();
}

static void runTests() {
  test_uIntRunTests();
  test_intRunTests();
  test_floatRunTests();
  test_boolRunTests();
  test_stringRunTests();
}

int main(int argc, char const* argv[]) {
  runTests();

  return 0;
}