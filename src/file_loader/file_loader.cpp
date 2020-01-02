#include "file_loader.h"

FileLoader::FileLoader() {
	SPIFFS.begin(true);
}

File FileLoader::loadFile(std::string path) {
  return SPIFFS.open(path.c_str());
}

void FileLoader::closeFile(File file) {
	file.close();
}

bool FileLoader::getLine(File file, std::string* line) {
	if (file.available()) {
		char c;
		line->clear();
		do {
			c = file.read();
			line->push_back(c);
		}
		while (c != '\n' && file.available());
		return true;
	}
	return false;
}