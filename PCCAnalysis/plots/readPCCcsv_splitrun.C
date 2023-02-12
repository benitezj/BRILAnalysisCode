#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <dirent.h>

void readPCCcsv_splitrun() {
  DIR *dir;
  struct dirent *ent;
  std::string directory = "/eos/user/a/asehrawa/PCC_newformat/EXPRESS_datasets/zerobias_27Oct2022_Run2018B";
  std::vector<std::string> csvFiles;
  
  // Open the directory and loop over its contents
  if ((dir = opendir (directory.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      // Check if the file is a CSV file
      if (ent->d_name[strlen(ent->d_name) - 4] == '.' &&
          ent->d_name[strlen(ent->d_name) - 3] == 'c' &&
          ent->d_name[strlen(ent->d_name) - 2] == 's' &&
          ent->d_name[strlen(ent->d_name) - 1] == 'v') {
        csvFiles.push_back(ent->d_name);
      }
    }
    closedir (dir);
  } else {
    // Handle error opening the directory
    std::cerr << "Error opening directory: " << directory << std::endl;
    return 1;
  }
  
  // Create a map to store the output file streams
  std::map<int, std::ofstream> outputFiles;
  
  // Loop over all the CSV files
  for (const auto &file : csvFiles) {
    std::string fullPath = directory + "/" + file;
    std::ifstream inputFile(fullPath);
    
    if (inputFile.is_open()) {
      std::string line;
      // Read each line of the file
      while (getline(inputFile, line)) {
	std::stringstream lineStream(line);
	std::string cell;
	std::vector<std::string> cells;
        
        // Split the line into cells
        while (getline(lineStream, cell, ',')) {
          cells.push_back(cell);
        }
        
        if (cells.size() >= 3) {
          int runNumber = std::stoi(cells[0]);
          int lumiSection = std::stoi(cells[1]);
          double pcc = std::stod(cells[2]);

	  std::cout<<runNumber<<" "<<lumiSection<<" "<<pcc<<std::endl;
          
          // Open the output file for this run number if it hasn't been opened yet
          if (outputFiles.count(runNumber) == 0) {
	    std::string outputFileName = directory + "/" +std::to_string(runNumber) + ".csv";
            outputFiles[runNumber].open(outputFileName);
          }
          
          // Write the information to the output file
          outputFiles[runNumber] << runNumber <<"," <<lumiSection << "," << pcc << std::endl;
        }
      }
      inputFile.close();
    } else {
      // Handle error opening the file                                                                           
      std::cerr << "Error opening file: " << fullPath << std::endl;
    }
  }
}

