#include <cstdint>
#include "../configuration.hpp"
#include "../configuration_manager.hpp"
#include "types/errorpair.hpp"
#include "error/error.hpp"

using Err::Error;
using Types::ErrorPair;

static File::Configuration configuration;
static File::ConfigurationManager* configurationManager;

static const char* testConfigFile = "/home/crop/openNV/src/file/test/test_configuration_manager_data.txt";
static const char* testCategoryName = "Category_1";
static const char* testCategoryNameNotLoaded = "Category_3";

static const char* testUIntValueName = "uValue";
static const char* testIntValueName = "iValue";
static const char* testFloatValueName = "fValue";
static const char* testStringValueName = "sValue";

static const uint64_t testUIntValueValue = 2;
static const int64_t testIntValueValue = -4;
static const float testFloatValueValue = 4.5f;
static const char* testStringValueValue = "Stevo";

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


static void setUp() {
  configurationManager = new File::ConfigurationManager();
}

static void tearDown() {
  delete configurationManager;
}

static void test_invalidPathError() {
  Error result = configurationManager->loadFile(emptyName);
  GAMAL_ABDUL_NASSERT(result.fail());
}

static void test_validFileNoError() {
  Error result = configurationManager->loadFile(testConfigFile);
  GAMAL_ABDUL_NASSERT(result.success());
}

static void test_configurationCategoryNotLoaded() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryNameNotLoaded);
  GAMAL_ABDUL_NASSERT(getResult.fail());  
}

static void test_configurationCategorySuccessfullyLoaded() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  GAMAL_ABDUL_NASSERT(getResult.success());
}

static void test_configurationCategoryHasCorrectValueUIntNoError() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<uint64_t> valueGetResult = category->nGetUInt(testUIntValueName);
  GAMAL_ABDUL_NASSERT(valueGetResult.success());
}

static void test_configurationCategoryHasCorrectValueUIntValue() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<uint64_t> valueGetResult = category->nGetUInt(testUIntValueName);
  GAMAL_ABDUL_NASSERT(testUIntValueValue == valueGetResult.value);
}

static void test_configurationCategoryHasCorrectValueIntNoError() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<int64_t> valueGetResult = category->nGetInt(testIntValueName);
  GAMAL_ABDUL_NASSERT(valueGetResult.success());
}

static void test_configurationCategoryHasCorrectValueIntValue() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<int64_t> valueGetResult = category->nGetInt(testIntValueName);
  GAMAL_ABDUL_NASSERT(testIntValueValue == valueGetResult.value);
}


static void test_configurationCategoryHasCorrectValueFloatNoError() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<float> valueGetResult = category->nGetFloat(testFloatValueName);
  GAMAL_ABDUL_NASSERT(valueGetResult.success());
}

static void test_configurationCategoryHasCorrectValueFloatValue() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<float> valueGetResult = category->nGetFloat(testFloatValueName);
  GAMAL_ABDUL_NASSERT(testFloatValueValue == valueGetResult.value);
}

static void test_configurationCategoryHasCorrectValueStringNoError() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<const char*> valueGetResult = category->nGetString(testStringValueName);
  GAMAL_ABDUL_NASSERT(valueGetResult.success());
}

static void test_configurationCategoryHasCorrectValueStringValue() {
  Error result = configurationManager->loadFile(testConfigFile);
  ErrorPair<File::Configuration*> getResult = configurationManager->nGetConfiguration(testCategoryName);
  File::Configuration* category = getResult.value;
  ErrorPair<const char*> valueGetResult = category->nGetString(testStringValueName);
  GAMAL_ABDUL_NASSERT(!std::strcmp(testStringValueValue, valueGetResult.value));
}

typedef void (*testFunction)(void);

static void run_tests() {
  testFunction funcs[] = {
    test_invalidPathError,
    test_validFileNoError,
    test_configurationCategoryNotLoaded,
    test_configurationCategorySuccessfullyLoaded,
    test_configurationCategoryHasCorrectValueUIntNoError,
    test_configurationCategoryHasCorrectValueUIntValue,
    test_configurationCategoryHasCorrectValueIntNoError,
    test_configurationCategoryHasCorrectValueIntValue,
    test_configurationCategoryHasCorrectValueFloatNoError,
    test_configurationCategoryHasCorrectValueFloatValue,
    test_configurationCategoryHasCorrectValueStringNoError,
    test_configurationCategoryHasCorrectValueStringValue
  };

  for(unsigned int i = 0; i < sizeof(funcs) / sizeof(funcs[0]); i++) {
    setUp();
    funcs[i]();
    tearDown();
  }
}

int main(int argc, char const *argv[])
{
  run_tests();
  return 0;
}