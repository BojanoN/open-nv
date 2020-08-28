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


InputStream* FileProvider::openFile(const std::string& filepath) {

	if(this->openedFiles.find(filepath) != this->openedFiles.end()) {
		return NULL;
	}

	InputStream* file = findAndOpenFile(filepath);

	if(file == NULL) {
		throw std::invalid_argument("File not found: " + filepath);
	}

	return file;
}

InputStream* FileProvider::findAndOpenFile(const std::string& filepath) {
	
	InputStream* file = openLooseFile(filepath);
	if(file != NULL) {
		
		lockFile(filepath, file);
		return file;
	}

	file = openArchiveFile(filepath);
	if(file != NULL) {
		return file;
	}

	registerFileOpened(filepath, file);

	return NULL;
}

void FileProvider::lockFile(const std::string& filepath, InputStream* file) {

	openedInputFiles.insert(fileStream);
	openedInputFilesPathMap.emplace(std::make_pair(filepath, fileStream));
	openedInputFilesStreamMap.emplace(std::make_pair(fileStream, filepath));
}


InputStream* FileProvider::openLooseFile(const std::string& filepath) {

	fs::path fullPath = this->rootPath / fs::path(filepath);

	if(!fs::exists(fullPath)) {
		return NULL;
	}
	if(!fs::is_regular_file(fullPath)) {
		log_error("Attempted to open %s, not a regular file!", filepath.c_str());
		return NULL;
	}
	return new FileInputStream(fullPath);
	
	return fileStream;
}


InputStream* FileProvider::openArchiveFile(const std::string& filepath) {
	
	BSA* containingArchive = getContainingArchive(filepath);

	if (containingArchive == NULL) {
		return NULL;
	}

	return new ByteArrayInputStream(archive->getFile(filepath));
}


BSA* FileProvider::getContainingArchive(const std::string& filepath) {
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
}


void FileProvider::closeFile(InputStream* file) {
	auto it = openedFiles.find(file);
	if (it != openedFiles.end()) {

		openedInputFilesPathMap.erase(openedInputFilesStreamMap.find(file)->second);
		openedInputFilesStreamMap.erase(file);
		openedFiles.erase(it);
	}
	file->close();
}