

void plotYearlySBIL(){

  //brilcalc lumi  --begin 378238 --end 386951 -u hz/ub  --byls -b "STABLE BEAMS" --output-style=csv --normtag=/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -i /eos/user/c/cmsdqm/www/CAF/certification/Collisions24/Cert_Collisions2024_378981_386951_Golden.json >> brilcalc_2024_lumi.csv
  //brilcalc beam --begin 378238 --end 386951 --output-style=csv >> brilcalc_2024_beam.csv

  
  TString YEAR="2024";
  TString inputlumi="brilcalc_2024_lumi.csv"; //brilcalc data for lumi per LS. eg.:
  //TString inputlumi="brilcalc_2024_lumi_noGolden.csv";//
  TString inputbeam="brilcalc_2024_beam.csv"; //brilcalc beam data for number of bx per LS, eg.:

  //TString YEAR="2023";
  //TString inputlumi="brilcalc_2023_lumi.csv";
  //TString inputlumi="brilcalc_2023_lumi_noGolden.csv";//
  //TString inputbeam="brilcalc_2023_beam.csv";

  
  //load the number of bx per run
  ifstream beamfile (inputbeam.Data());
  if (!beamfile.is_open()){
    cout << "Unable to open file: "<<inputbeam.Data()<<endl;
  }
  std::string beamline;
  string tmp;
  int run=0;
  int Ncoll=0;
  std::map<int,int> NBX;
  while (std::getline(beamfile, beamline)){
      std::stringstream iss(beamline);
      std::string token;

      ///read the fill
      std::getline(iss,token, ',');
      std::stringstream fiss(token);
      fiss>>tmp;//fill

      ///read the run
      std::getline(iss,token, ',');
      std::stringstream riss(token);
      riss>>run;

      //skip to the end
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::stringstream Niss(token);
      Niss>>Ncoll;
      //cout<<run<<" "<<Ncoll<<endl;
      NBX[run]=Ncoll;
      
  }

  
  //return;
  
  TH1F HL("HL","",200,0,22000);
  TH1F HSBIL("HSBIL",YEAR+" SBIL",200,0,12);
  
  //Loop over the lumi per LS
  ifstream myfile (inputlumi.Data());
  if (!myfile.is_open()){
    cout << "Unable to open file: "<<inputlumi.Data()<<endl;
  }
  std::string line;
  //int run=0;
  int ls=0;
  int energy=0;
  float lsL=0; //lumi for given LS
  //int Ncoll=0;
  while (std::getline(myfile, line)){
      std::stringstream iss(line);
      std::string token;

      ///read the run number
      std::getline(iss,token, ',');
      std::stringstream runiss(token);
      runiss>>tmp;//RUN:FILL
      std::stringstream rss(tmp);
      std::getline(rss,token, ':');
      std::stringstream Rss(token);
      Rss>>run;

      ///read the LS
      std::getline(iss,token, ',');
      std::stringstream lsiss(token);
      lsiss>>tmp;

      //
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::getline(iss,token, ',');
      std::stringstream Eiss(token);
      Eiss>>energy;

      if(energy!=6800) continue;
      
      ///read the  Lumi per ls
      std::getline(iss,token, ',');
      std::stringstream totLiss(token);
      totLiss>>lsL;

      //cout<<run<<" "<<energy<<" "<<lsL<<endl;

      if(NBX[run]<=0) continue;
      
      HL.Fill(lsL);
      HSBIL.Fill(lsL/NBX[run],lsL*23.3/1E9);
      //HSBIL.Fill(lsL/NBX[run]);
  }

  TCanvas C;
  HL.SetStats(1);
  C.Clear();
  HL.Draw("hist");
  C.Print(YEAR+"_avg_sbil_lumi.png");


  gStyle->SetOptStat(1001110);
  HSBIL.GetYaxis()->SetTitle("Integrated Lumi [1/fb]");
  HSBIL.GetXaxis()->SetTitle("SBIL [Hz/ub]");
  C.Clear();
  HSBIL.Draw("hist");
  C.Print(YEAR+"_avg_sbil.png");
  
  
}
