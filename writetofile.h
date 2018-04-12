#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <iomanip>

class WriteToFile{
 public:
  void SaveData(std::vector<struct player>);

 private:
   void GetDateStream(std::stringstream& datum);
   void CopyFile(std::ifstream& inData, std::ofstream& outData);
};
