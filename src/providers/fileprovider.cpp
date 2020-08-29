#include "fileprovider.hpp"
#include <algorithm>
#include <cctype>
#include <thread>
#include <cassert>

namespace fs = std::filesystem;

bool FileProvider::instantiated = false;

FileProvider::FileProvider(const char* rootPath) {
	assert(!this->instantiated);

	fs::path root(rootPath);

	if(!fs::exists(root)) {
		throw std::invalid_argument(std::string("Root data path does not exist! ") + std::string(rootPath));
	}

	if(!fs::is_directory(root)) {
		throw std::invalid_argument(std::string("Root data path is not a directory! ") + std::string(rootPath));
	}

	this->root = root;
	log_info("Setting root data folder: %s", rootPath);

	this->initArchives();

	this->instantiated = true;
}

FileProvider::~FileProvider() {
	for(auto archive : archives) {
		delete archive;
	}
	this->instantiated = false;
}


void FileProvider::initArchives() {

	for(auto& entry : fs::directory_iterator(this->root)) { // fs::directory_entry
		std::string name = entry.path().filename().string();
		if(StringUtils::endsWith(name, ".bsa")) {

			try {
				log_info("Loading %s", entry.path().string().c_str());
				std::string path = entry.path().string();
				this->archives.push_back(new BSA::BSA(new FileInputStream(path), path));
			} catch(std::runtime_error& e) {
				log_error(e.what());
			}
		}
	}
}


InputStream* FileProvider::openFile(const std::string& filepath) {

	std::unique_lock<std::mutex> lock(this->mutex);
	
	while(openedInputFiles.find(filepath) != openedInputFiles.end()) {
		log_debug("Thread %u waiting!", std::this_thread::get_id());
		fileAlreadyOpen.wait(lock);
		log_debug("Thread %u woke up!", std::this_thread::get_id());
	}
	log_debug("Placing %s into opened files map.", filepath.c_str());
	openedInputFiles.insert(filepath);

	lock.unlock();

	log_debug("Thread %u passed block! ", std::this_thread::get_id());

	InputStream* file = findAndOpenFile(&filepath);

	if(file == NULL) {

		lock.lock();

		openedInputFiles.erase(filepath);

		lock.unlock();

		log_error("File not found: %s", filepath.c_str());
		return NULL;
		//throw std::invalid_argument("File not found: " + filepath);
	}

	return file;
}

InputStream* FileProvider::findAndOpenFile(const std::string* filepath) {
	
	InputStream* file = openLooseFile(filepath);
	if(file != NULL) {
	
		log_debug("Inserting %p, %s into map.", file, filepath->c_str());
		openedInputFilesMap.insert(std::make_pair(file, *filepath));
		
		return file;
	}

	file = openArchiveFile(filepath);
	if(file == NULL) {
		return NULL;
	}

	log_debug("Inserting %p, %p into map.", file, filepath);
	openedInputFilesMap.insert(std::make_pair(file, *filepath));
	return file;
}

InputStream* FileProvider::openLooseFile(const std::string* filepath) {

	#ifdef __linux__ 
	fs::path fullPath = this->root / StringUtils::replaceChar(*filepath, '\\', '/');
	#else
	fs::path fullPath = this->root / *filepath
	#endif
	
	log_info("Searching for loose file: %s", fullPath.string().c_str());

	if(!fs::exists(fullPath)) {
		return NULL;
	}
	if(!fs::is_regular_file(fullPath)) {
		log_error("Attempted to open %s, not a regular file!", filepath->c_str());
		return NULL;
	}

	log_info("Found loose file: %s", fullPath.string().c_str());
	return new FileInputStream(fullPath);
}


InputStream* FileProvider::openArchiveFile(const std::string* filepath) {
	
	std::string lowerCased = *filepath;
	std::transform(filepath->begin(), filepath->end(), lowerCased.begin(), [](unsigned char c) { return std::tolower(c); });

	log_info("Searching for file in archive: %s", lowerCased.c_str());

	BSA::BSA* containingArchive = getContainingArchive(&lowerCased);

	if (containingArchive == NULL) {
		return NULL;
	}

	return containingArchive->getFile(lowerCased);
}


BSA::BSA* FileProvider::getContainingArchive(const std::string* filepath) {
	
	BSA::BSA* containingArchive = NULL;

	log_debug("Searching for archived file: %s", filepath->c_str());
	log_debug("Checking path cache.");
	auto archiveIt = this->archivePathCache.find(*filepath);
	if(archiveIt != this->archivePathCache.end()) {
		log_debug("Hit! %s", archiveIt->second->getName().c_str());		
		containingArchive = archiveIt->second;
	
	} else {

		for(auto archive : archives) {
			
			log_debug("Searching %s", archive->getName().c_str());
			if(archive->fileExists(*filepath)) {
					
				log_debug("Found!");
				this->archivePathCache.insert(std::make_pair(*filepath, /*static_cast<BSA::BSA*>(*/archive/*)*/));
				containingArchive = archive;

				break;
			}
		}	
	}

	return containingArchive;
}


void FileProvider::closeFile(InputStream* file) {
	std::unique_lock<std::mutex> lock(this->mutex);

	log_debug("Attempting to remove %p from map.", file);

	auto it = openedInputFilesMap.find(file);
	if (it != openedInputFilesMap.end()) {

		auto path = openedInputFilesMap.find(file);

		int s = openedInputFiles.size();
		log_debug("Removing %s", path->second.c_str());

		openedInputFiles.erase(path->second);
		openedInputFilesMap.erase(it);

		if(openedInputFiles.size() != s - 1) {
			throw std::runtime_error("AAA");
		}
	}
	file->close();

	lock.unlock();
	fileAlreadyOpen.notify_one();
}