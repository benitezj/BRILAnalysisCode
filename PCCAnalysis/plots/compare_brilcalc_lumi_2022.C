#include <iostream>
#include <fstream>
#include <map>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TGraph.h>

void compare_brilcalc_lumi_2022() {

  std::ifstream filePCC("combined_lumi_pcc.txt");
  std::ifstream fileHFET("combined_lumi_hfet.txt");

  if (!filePCC.is_open()) {
    std::cout << "Error opening combined_lumi_pcc.txt!" << std::endl;
    return;
  }

  if (!fileHFET.is_open()) {
    std::cout << "Error opening combined_lumi_hfet.txt!" << std::endl;
    return;
  }

  std::map<std::pair<int, int>, float> luminosityMapPCC; 
  int runNumber1, lumiSection1;
  float luminosity1;

  while (filePCC >> runNumber1 >> lumiSection1 >> luminosity1) {
    luminosityMapPCC[{runNumber1, lumiSection1}] = luminosity1;
  }

  int runNumber2, lumiSection2;
  float luminosity2;
  //int matchedEntries = 0;

  TH2F *hist2D = new TH2F("hist2D", "Sequential Matched Lumi Section vs Ratio;Sequential Matched Lumi Section;Ratio", 2000, 0, 100000, 2000, 0.96, 1.04);
  TH1F *hist1D = new TH1F("hist1D", "Distribution of Luminosity Ratios;Ratio;Entries", 2000, 0.96, 1.04);

std::ofstream outputFile("ls.dat");	

  int matchedCounter = 0;

  while (fileHFET >> runNumber2 >> lumiSection2 >> luminosity2) {
    auto key = std::make_pair(runNumber2, lumiSection2);
    if (luminosityMapPCC.find(key) != luminosityMapPCC.end()) {
      float pccValue = luminosityMapPCC[key];
      float ratio = (pccValue == 0) ? 0 : pccValue / luminosity2;

      if ((matchedCounter >= 0 && matchedCounter <= 7000) || (matchedCounter >= 72000 && matchedCounter <= 76000)) {
	matchedCounter++;
	continue;
      }


      if (ratio != 0) {
	hist1D->Fill(ratio);
	hist2D->Fill(matchedCounter, ratio);
        outputFile << runNumber2 << "   " << lumiSection2 << "   " << pccValue << "   " << luminosity2 << "\n";
	matchedCounter++;
	//matchedEntries++;
      }
    }
  }

  TCanvas *canvas2D = new TCanvas("c2D", "Luminosity Ratio", 1400, 400);
  hist2D->Draw("COLZ");
  canvas2D->SaveAs("/eos/user/a/asehrawa/luminosity_ratio_histogram.png");

  TCanvas *canvas1D = new TCanvas("c1D", "Luminosity Ratio Distribution", 800, 600);
  hist1D->Draw("hist");
  canvas1D->SaveAs("/eos/user/a/asehrawa/luminosity_ratio_distribution.png");

 
  std::cout << "Total matched entries: " << matchedCounter << std::endl;

  filePCC.close();
  fileHFET.close();
  outputFile.close();
}
