/*#include "src/file_loader/file_loader.h"
#include <string>

void setup() {
  Serial.begin(115200);

  FileLoader fileLoader;

  File file = fileLoader.loadFile("/pattern1.txt");
  if (file) {
    Serial.println("Successfully loaded file");
  
  Serial.println("**********************");
  Serial.println("Loading file char by char");
    while(file.available())
      Serial.write(file.read());

  fileLoader.closeFile(file);
  file = fileLoader.loadFile("/pattern1.txt");
  
  Serial.println("**********************");
  Serial.println("Loading file line by line");
  std::string line;
  while(fileLoader.getLine(file, line))
    Serial.println(line.c_str());
    
  fileLoader.closeFile(file);
  }
  else
    Serial.println("Error while loading file");
}

void loop() {}*/

#include "src/motors/motors.h"
#include "src/encoder/encoder.h"

#include "src/route_planning/RoutePlanner.h"
#include "src/localization/localization.h"
#include "src/steering/Control.h"

#include "src/file_loader/file_loader.h"

void setup() {
  Serial.begin(115200);

  FileLoader fileLoader;
  RoutePlanner routePlanner(&fileLoader);

  Serial.println("RoutePlanner created successfully");
  std::vector<Point*> path = routePlanner.getPath("/pattern1.txt");

  for(int i = 0; i < path.size(); i++) {
    Serial.print(path[i]->coordX);
    Serial.print(" ");
    Serial.println(path[i]->coordX);
  }
 
  Encoder encoder;
  Motors motors;
  motors.addEncoder(&encoder);
  
  Localization localization(&encoder);
  Steering steering(&motors, &localization);
  Control control(path, steering);

  //control.accomplishTrace();

}

void loop() {

}
  
