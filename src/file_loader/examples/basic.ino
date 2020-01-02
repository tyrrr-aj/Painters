#include "src/file_loader/file_loader.h"
#include <string>

void setup() {
  Serial.begin(115200);

  FileLoader fileLoader;

  File file = fileLoader.loadFile("/pattern1.txt");
  if (file) {
    Serial.println("Successfully loaded file");
	
	Serial.printn("**********************");
	Serial.println("Loading file char by char");
    while(file.available())
      Serial.write(file.read());
	
	fileLoader.closeFile(file);
	file = fileLoader.load("/pattern1.txt");
	
	Serial.println("**********************");
	Serial.println("Loading file line by line");
	std::string line;
	while(line = fileLoader.getLine(file, line))
		Serial.println(line);
		
	fileLoader.closeFile(file);
  }
  else
    Serial.println("Error while loading file");
}

void loop() {}