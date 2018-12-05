void checkXrootdFiles(std::string txtfile){
  
  
  ifstream newfile(txtfile.c_str());
  if (!newfile.is_open()){
    std::cout << "Unable to open runs.dat in "<<txtfile.c_str()<<std::endl;
    return;
  }

  std::string line;
  while (std::getline(newfile,line)){
    
    //TFile *f=TFile::Open("root://xrootd-cms.infn.it//store/data/Commissioning2018/AlCaLumiPixels0/ALCARECO/AlCaPCCRandom-02May2018-v1/70000/6CE65B93-D657-E811-BBB5-FA163E8006EF.root"); 
    TFile *f=TFile::Open(line.c_str(),"TIMEOUT=30");
    if(f && !f->IsZombie()){

      cout<<f->GetName()<<endl;
      TTree*t=(TTree*)f->Get("Events");
      if(t){ cout<<"Entries="<<t->GetEntries()<<endl; }

    }

  
  }

}
