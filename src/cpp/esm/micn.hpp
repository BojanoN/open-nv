#include "esm/record.hpp"

namespace ESM {

struct MenuIcon : public Record {
  std::string editorId;
  std::string largeIconFilename;
  std::string smallIconFilename;

  MenuIcon(formid id, std::string editorId, std::string largeIconFilename,
           std::string smallIconFilename)
      : Record(id), editorId{editorId},
        largeIconFilename{largeIconFilename},
        smallIconFilename{smallIconFilename} {}

  static std::string getType() {
  	return "Menu icon";
  }
};

};