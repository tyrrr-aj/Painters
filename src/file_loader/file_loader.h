#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "SPIFFS.h"
#include <string>

class FileLoader {
	public:
		FileLoader();
		File loadFile(std::string path);
		void closeFile(File file);
		bool getLine(File file, std::string* line);
};

#endif