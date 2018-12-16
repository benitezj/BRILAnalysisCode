#include <iostream>
#include <fstream>
#include <string>

//BPIX
std::map<long,int> LY;
std::map<long,int> LD;
std::map<long,int> MD;

//FPIX
std::map<long,int> SD;
std::map<long,int> DI;
std::map<long,int> BL;
std::map<long,int> PN;


void plotBadModules(string BadList="SiPixelQuality_byPCL_stuckTBM_v1.mod", string outpath="."){

  gROOT->ProcessLine(".x BRILAnalysisCode/PCCAnalysis/plots/rootlogon.C");




  //BPIX
  ifstream cfile_bpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_BPix.txt");
  if (!cfile_bpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_bpix;
  while (std::getline(cfile_bpix,cline_bpix)){
    std::stringstream iss(cline_bpix);
    long module,ly,ld,mod;
    iss>>module>>ly>>ld>>mod;
    LY[module]=ly;
    LD[module]=ld;
    MD[module]=mod;
  }
  TH2F B[4];
  TH2I BModId[4];
  int bins[4]={6,14,22,32};
  for(long l=0;l<4;l++){
    B[l]=TH2F(TString("L")+l,"",9,-4.5,4.5,2*bins[l]+1,-bins[l]-0.5,bins[l]+.5);
    B[l].SetStats(0);
    B[l].GetXaxis()->SetTitle("Signed Module");
    B[l].GetYaxis()->SetTitle("Signed Ladder");

    BModId[l]=TH2I(TString("ModIdL")+l,"",9,-4.5,4.5,2*bins[l]+1,-bins[l]-0.5,bins[l]+.5);
  }


  ////FPIX
  ifstream cfile_fpix("BRILAnalysisCode/PCCAnalysis/test/ModuleCoords_FPix.txt");
  if (!cfile_fpix.is_open()){
    std::cout << "Unable to open coordinates"<<endl;
    return;
  }
  std::string cline_fpix;
  while (std::getline(cfile_fpix,cline_fpix)){
    std::stringstream iss(cline_fpix);
    long module,sd,di,bl,pn;
    iss>>module>>sd>>di>>bl>>pn;
    SD[module]=sd;
    DI[module]=di;
    BL[module]=bl;
    PN[module]=pn;
  }
  TH2F F[2];
  TH2I FModId[2];
  for(long l=0;l<2;l++){
    F[l]=TH2F(TString("D")+l,"",7,-3.5,3.5,57,-28.5,28.5);
    F[l].SetStats(0);
    F[l].GetXaxis()->SetTitle("Signed Disk");
    F[l].GetYaxis()->SetTitle("Signed Blade");
    
    FModId[l]=TH2I(TString("ModIdD")+l,"",7,-3.5,3.5,57,-28.5,28.5);
  }


  ///////////////////////////////
  ///open and loop over the IOV's
  ifstream myfile(BadList.c_str());
  if (!myfile.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }


  std::string line;
  int run,ls;
  long iov;
  int module=0;
  int totiovs=0;

  std::map<int,int> modcount;

  std::getline(myfile,line);//skip first line with labels
  while (std::getline(myfile,line)){
    ///cout<<line<<"HELLO"<<endl;;
    totiovs++;

    std::stringstream iss(line);
    iss>>run>>ls>>iov;

    //std::string word;
    while (iss >> module) {
      //module=atoi(word.c_str());

      //BPIX
      if ( MD.find(module) != MD.end() ){
	int md=MD[module]>4?MD[module]-4:MD[module]-5;
	int ld=LD[module]>bins[LY[module]-1]?LD[module]-bins[LY[module]-1]:LD[module]-bins[LY[module]-1]-1;      
	B[LY[module]-1].Fill(md,ld);
	//BModId[LY[module]-1].Fill(md,ld,module);
	modcount[module]++;
      }
    
      //FPIX
      if( SD.find(module) != SD.end() ){
	int di=(SD[module]==2?1:-1)*DI[module];
	int bl=BL[module]>28?BL[module]-28:BL[module]-29;
	F[PN[module]-1].Fill(di,bl);
	//FModId[PN[module]-1].Fill(di,bl,module);
	modcount[module]++;
      }
      
    }
  }


  TCanvas C;
  ////2D map Plots
  for(long l=0;l<4;l++){
    B[l].Scale(1./totiovs);
    C.Clear();
    B[l].Draw("colz");
    C.Print(TString(outpath.c_str())+"/VetoMap_Layer"+(l+1)+".png");
  }

  for(long l=0;l<2;l++){
    F[l].Scale(1./totiovs);
    C.Clear();
    F[l].Draw("colz");
    C.Print(TString(outpath.c_str())+"/VetoMap_FPIX"+(l+1)+".png");
  }


  /////////////////////////////
  //1D prob distributions 

  TH1F HP("HP","",200,0,1);
  ofstream vetofile;
  vetofile.open("./ModuleVetoList.txt");
  if (!vetofile.is_open()){
    std::cout << "Unable to open "<<endl;
    return;
  }
  vetofile<<std::setprecision(9);
  map<int,int>::iterator it;
  for ( it = modcount.begin(); it!= modcount.end(); it++ ){
    float prob=(float)(it->second)/totiovs;
    HP.Fill(prob);
    //cout<<it->first<<" "<<prob<<endl;
    if(prob>0.005)
      vetofile<<it->first<<endl;
  }  
  vetofile.close();


  TH1F HProb("BPIX+FPIX","",200,0,1);
  TH1F HProbB("HProbB","",200,0,1);
  TH1F HProbF("HProbF","",200,0,1);
  for(long l=0;l<4;l++){
    for(int bx=1;bx<=B[l].GetNbinsX();bx++){
      for(int by=1;by<=B[l].GetNbinsY();by++){
	if(B[l].GetXaxis()->GetBinCenter(bx)==0 || B[l].GetYaxis()->GetBinCenter(by)==0) continue;

	HProb.Fill(B[l].GetBinContent(bx,by));
	HProbB.Fill(B[l].GetBinContent(bx,by));
      }
    }
  }  

  for(long l=0;l<2;l++){
    for(int bx=1;bx<=F[l].GetNbinsX();bx++){
      for(int by=1;by<=F[l].GetNbinsY();by++){
	if(F[l].GetXaxis()->GetBinCenter(bx)==0 || F[l].GetYaxis()->GetBinCenter(by)==0) continue;

	HProb.Fill(F[l].GetBinContent(bx,by));
	HProbF.Fill(F[l].GetBinContent(bx,by));
      }
    }
  }


  HProb.GetYaxis()->SetTitle("# of modules");
  HProb.GetXaxis()->SetTitle("probability");
  //HProb.SetStats(0);
  TLegend leg(0.5,0.7,0.7,0.9);
  leg.AddEntry(&HProb,"BPIX+FPIX","l");
  leg.AddEntry(&HProbB,"BPIX","l");
  leg.AddEntry(&HProbF,"FPIX","l");
  C.Clear();
  C.SetLogy(1);
  HProb.Draw("hist");
  HProbB.SetLineColor(2);
  HProbB.Draw("histsame");
  HProbF.SetLineColor(4);
  HProbF.Draw("histsame");
  //HP.Draw("histpsame");
  leg.Draw();
  C.Print(TString(outpath.c_str())+"/Prob.png");


  C.Clear();
  C.SetLogy(1);
  HProb.GetXaxis()->SetRangeUser(0,0.10);
  HProb.Draw("hist");
  HProbB.Draw("histsame");
  HProbF.Draw("histsame");
  leg.Draw();
  C.Print(TString(outpath.c_str())+"/Prob_zoom.png");


}
