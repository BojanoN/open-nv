#pragma once
#include "providers/fileprovider.hpp"
#include <memory>


namespace File {

class Reader {

private:
	static std::unique_ptr<FileProvider> fileProvider;

protected:
	static FileProvider& getFileProvider();

public:
	static void setRootFilePath(const std::string& path);

};

}; // namespace