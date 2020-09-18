#include "util/stringutils.hpp"
#include "bsa/bsa.hpp"
#include "logc/log.h"
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


class FileProvider {

private:
	
	//std::mutex mutex;
	//std::condition_variable fileAlreadyOpen;

	// The root data directory.
	std::filesystem::path 		  		  root;

	// Available .bsa archive files.
	std::vector<BSA::BSA*>	     	  		  archives;

	// Cache of file paths to archives.
	std::unordered_map<std::string, BSA::BSA*> archivePathCache;

	// Set of all open files.
	//std::unordered_set<std::string>    openedInputFiles;

	// Map of opened input files.
	//std::unordered_map<std::string, InputStream*> openedInputFilesPathMap;

	// Map of opened input streams.
	//std::unordered_map<InputStream*, std::string> openedInputFilesMap;

	// Loads the .bsa archives.
	void 			initArchives();

	// Opens a loose file. 
	std::vector<uint8_t>   readLooseFile(const std::string& filepath);
	
	// Opens a file from a .bsa archive.
	std::vector<uint8_t>    readArchiveFile(const std::string& filepath);

	BSA::BSA*		getContainingArchive(const std::string& filepath);
	
	static bool instantiated;

public:

	FileProvider(const std::string& rootPath);
	~FileProvider();

	std::vector<uint8_t> readFile(const std::string& filePath);
};