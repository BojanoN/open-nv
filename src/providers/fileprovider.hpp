#include "util/strinutils.hpp"
#include "bsa/bsa.hpp"
#include "logc/log.h"
#include "streams/file/ifstream.hpp"
#include "streams/bytearray/ifstream.hpp"
#include <filesystem>
#include <string>
#include <vector>

class FileProvider {

private:
	
	// The root data directory.
	std::filesystem::path 		  		  root;

	// Available .bsa archive files.
	std::vector<BSA>	     	  		  archives;

	// Cache of file paths to archives.
	std::unordered_map<std::string, BSA*> archivePathCache;

	// Set of all open input stream objects.
	std::unordered_set<InputStream*>    openedInputFiles;

	// Map of opened input files.
	std::unordered_map<std::string, InputStream*> openedInputFilesPathMap;

	// Loads the .bsa archives.
	void 			initArchives();

	// Opens a loose file. 
	FileInputStream&    openLooseFile(const std::string& filepath);
	
	// Opens a file from a .bsa archive.
	ByteArrayInputStream&    openArchiveFile(const std::string& filepath);

	// Closes the given file.
	void 			closeFile(InputStream& file);

public:

	FileProvider(const char* rootPath);

	std::ifstream openFile(std::string filePath);
};