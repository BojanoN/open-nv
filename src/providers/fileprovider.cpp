#include "fileprovider.hpp"

namespace fs = std::filesystem;

FileProvider::FileProvider(const char* rootPath) {
	fs::path root(rootPath);

	if(!fs::exists(root)) {
		throw std::invalid_argument(std::string("Root data path does not exist! ") + std::string(rootPath));
	}

	if(!fs::is_directory(root)) {
		throw std::invalid_argument(std::string("Root data path is not a directory! ") + std::string(rootPath));
	}

	this->root = root;
	this->initArchives();
}


void FileProvider::initArchives() {
	fs::directory_iterator bsaIterator(this->root);

	for(auto& entry = bsaIterator.begin(); entry != bsaIterator.end(); entry++) { // fs::directory_entry
		std::string name = entry.path().filename().string();
		if(StringUtils::endsWith(name, ".bsa")) {

			try {
				this->archives.emplace_back(entry.path().string());
			} catch(std::runtime_error& e) {
				log_error(e.what());
			}
		}
	}
}


InputStream& FileProvider::openFile(std::string filepath) {

	if(this->openedFiles.find(filepath) != this->openedFiles.end()) {
		std::logic_error("File already opened!");
	}

	std::string errors;


	try {
		return openLooseFile(filepath);
	} catch (std::invalid_argument& e) {
		// Not a loose file
		errors += e.what();
	}

	try {
		return openArchiveFile(filepath);
	} catch (std::invalid_argument e) {
		// Not an archive file
		errors += e.what();
	}
	
	throw std::invalid_argument(errors);
}


InputStream& FileProvider::openLooseFile(std::string filepath) {

	fs::path fullPath = this->rootPath / fs::path(filepath);

	if(!fs::exists(fullPath)) {
		throw std::invalid_argument(std::string("Loose file path does not exist! ") + std::string(fullPath));
	}
	if(!fs::is_regular_file(fullPath)) {
		throw std::invalid_argument(std::string("Not a regular file! ") + std::string(fullPath));
	}

	FileInputStream* fileStream = new FileInputStream(fullPath);
	
	openedInputFiles.insert(fileStream);
	openedInputFilesPathMap.emplace(std::make_pair(filepath, fileStream));
	
	return *fileStream;
}


InputStream& FileProvider::openArchiveFile(std::string filepath) {
	
	BSA* containingArchive = NULL;

	auto archiveIt = this->archivePathCache.find(filepath);
	if(archiveIt != this->archivePathCache.end()) {
		
		containingArchive = archiveIt->second;
	
	} else {

		for(auto archive = archives.begin(); archive != archives.end(); archive++) {
		
			if(archive->fileExists(filepath)) {
				this->archivePathCache[filepath] = archive;
				containingArchive = archive;

				break;
			}
		}	
	}

	if (containingArchive == NULL) {
		throw std::invalid_argument("File not found in any archive!");
	}

	ByteArrayInputStream* fileStream = new ByteArrayInputStream(archive->getFile(filepath));

	openedFiles.emplace(std::make_pair(std::string(filepath), ByteArrayInputStream(archive->getFile(filepath))));
	return openedFiles[filepath];
}


void FileProvider::closeFile(InputStream& file) {
	auto it = openedFiles.find(file.getPath());
	if (it != openedFiles.end()) {
		openedFiles.erase(it);
	}
}