#include <iostream>
#include <fstream>
#include <string>

#define MAXPCCRUN 5e10
#define PCCZEROLUMI 1e3
#define PCCRUNLUMIRatioMin 18
#define PCCRUNLUMIRatioMax 22
long RUNOFFSET=315000;


#define NLS 3000 //max number of lumi sections for histograms
float InstLumiScale=(1e30/23.31)/1e34; //brilcalc lumi/lumisection [1/microbarn], 1 barn = 10^-24 cm^2,  y value will be in unites of 1e34

////CMS plot labels
TString InstLumiAxisTitle("Inst. Luminosity [ E34#scale[1.0]{cm}^{-2} s^{-1} ]");

void drawLumiTitle(long FILL){
  TLatex text;
  text.SetTextColor(4);
  text.SetTextSize(0.040);
  text.DrawLatexNDC(0.25,0.94,TString("CMS Offline Luminosity, LHC Fill ")+(long)FILL+", #sqrt{s} = 13 TeV");
}

void drawCMSPrelim(long year,float xpos=0.2){
  TLatex text;
  text.SetTextColor(1);
  text.SetTextSize(0.050);
  text.DrawLatexNDC(xpos,0.85,TString("#font[62]{CMS} #font[52]{Preliminary ")+year+"}");
}
