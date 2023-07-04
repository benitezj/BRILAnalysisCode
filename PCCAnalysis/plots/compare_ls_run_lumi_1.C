#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <dirent.h>
#include <sys/stat.h>

struct RunData {
  std::unordered_set<int> lsSet;
  float pccSum;
};

void readCSVFile(const std::string& filepath, std::unordered_map<int, RunData>& runData) {
  std::ifstream file(filepath);
  std::string line;
  int run = 0;
  int ls = 0;
  float totalPCC = 0;

  while (std::getline(file, line)) {
    std::stringstream iss(line);
    std::string token;

    std::getline(iss, token, ',');
    std::stringstream runiss(token);
    runiss >> run;

    std::getline(iss, token, ',');
    std::stringstream lsiss(token);
    lsiss >> ls;

    std::getline(iss, token, ',');
    std::stringstream pcciss(token);
    pcciss >> totalPCC;

    // Add the LS to the set for the current run number
    runData[run].lsSet.insert(ls);

    // Sum the totalPCC for the current run number
    runData[run].pccSum += totalPCC;
  }

  file.close();
}

void processDirectory(const std::string& path, std::unordered_map<int, RunData>& runData) {
  DIR* dir;
  struct dirent* ent;
  struct stat file_stat;

  if ((dir = opendir(path.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std::string filename = ent->d_name;

      // Skip if not a regular file
      if (stat((path + "/" + filename).c_str(), &file_stat) != 0 || !S_ISREG(file_stat.st_mode)) {
        continue;
      }

      // Check if file has .csv extension
      if (filename.size() > 4 && filename.compare(filename.size() - 4, 4, ".csv") == 0) {
        readCSVFile(path + "/" + filename, runData);
      }
    }
    closedir(dir);
  } else {
    std::cerr << "Could not open directory: " << path << std::endl;
  }
}

void compare_ls_run_lumi_1() {
  std::unordered_map<int, RunData> runData1; // stores LS set and totalPCC sum for each run number in dir1
  std::unordered_map<int, RunData> runData2; // stores LS set and totalPCC sum for each run number in dir2

  std::string path1 = "/eos/user/a/asehrawa/PCC_newafterglowparameters_23May2023/ZeroBias/PCCcsvperrun_27May2023_D/Run2018D/"; // The path to the first directory
  std::string path2 = "/eos/user/a/asehrawa/PCC_newdatasets_13March2023/ZeroBias/Run2018_ZB_test/Run2018D/"; // The path to the second directory

  processDirectory(path1, runData1);
  processDirectory(path2, runData2);

  //std::cout << "Run\tLS Difference\tTotalPCC Difference\n";
  std::cout << "Run\tTotalPCC Difference\tLS Difference\n";
  std::cout << "----------------------------------------\n";

  // Iterate over runData1
  for (const auto& kv : runData1) {
    int run = kv.first;
    const RunData& data1 = kv.second;

    // Check if the run number exists in runData2
    if (runData2.find(run) != runData2.end()) {
      const RunData& data2 = runData2.at(run);

      // Calculate the difference in LS count
      int lsDiff = data2.lsSet.size() - data1.lsSet.size();

      // Calculate the difference in totalPCC
      float pccDiff = data2.pccSum - data1.pccSum;

      //std::cout << run << "\t" << lsDiff << "\t\t" << pccDiff << "\n";

      std::cout << run << "\t    " << pccDiff << "\t\t       " << lsDiff << "\n";
    } else {
      // Run number only exists in dir1
      //std::cout << run << "\t" << data1.lsSet.size() << "\t\t" << data1.pccSum << "\n";

      std::cout << run << "\t    " << data1.pccSum << "\t\t       " << data1.lsSet.size() << "\n";
    }
  }

  // Iterate over runData2 to find any additional run numbers only present in dir2
  for (const auto& kv : runData2) {
    int run = kv.first;
    const RunData& data2 = kv.second;

    if (runData1.find(run) == runData1.end()) {
      // Run number only exists in dir2
      //std::cout << run << "\t" << data2.lsSet.size() << "\t\t" << data2.pccSum << "\n";

      std::cout << run << "\t    " <<  data2.pccSum << "\t\t       " << data2.lsSet.size() << "\n";
    }
  }
}

int main() {
  compare_ls_run_lumi_1();
  return 0;
}
