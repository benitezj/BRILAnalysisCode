#include"globals.h"
#include <iostream>
#include <vector>
#include <dirent.h>

void afterglow_uncertainty() {
  
  TString inpath="/eos/user/a/asehrawa/afterglow_allrun";
  TString outpath="/eos/user/a/asehrawa/BRIL-new/";
  
  TCanvas*C = new TCanvas("residual vs time");
  C->cd();
  
  std::vector<int> run_number;
  
  run_number = { 315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785,315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219,316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472,316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878,316879, 316928, 316944, 316985, 316993, 316994, 316995, 317080, 317087, 317088, 317089, 317170, 317182, 317212, 317213, 317239, 317279,317291, 317292, 317295, 317296, 317297, 317319, 317320,317338, 317339, 317340, 317382, 317383, 317391, 317392, 317434, 317435, 317438, 317475, 317478, 317479, 317480, 317481, 317482, 317484, 317488, 317509, 317510, 317511, 317512, 317527, 317580, 317591, 317626, 317640, 317641, 317648,317649, 317650, 317661, 317663, 317683, 317696, 318070, 318622, 318653, 318661, 318662, 318663, 318667, 318669, 318670, 318675, 318712,318714, 318733, 318734, 318785,318816, 318817, 318819, 318820, 318828, 318834, 318836, 318837, 318872, 318874, 318876, 318877, 318939, 318944, 318945, 318953, 318980, 318981, 318982, 318983, 318984, 318992, 319006, 319011, 319015, 319016, 319018, 319019, 319077, 319097, 319098, 319099, 319100, 319101, 319103, 319104, 319124, 319125, 319159, 319160, 319173, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319273, 319274, 319300, 319310, 319311, 319337, 319347, 319348, 319349, 319449, 319450, 319452, 319456, 319459, 319460, 319462, 319463, 319464, 319466, 319467, 319468, 319469, 319470, 319471, 319472, 319486, 319488, 319503, 319506, 319524, 319525, 319526, 319528, 319579, 319606, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319852, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319928, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320003, 320006, 320007, 320008, 320009, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320058, 320059, 320060, 320061, 320062, 320063, 320064, 320065, 320500, 320569, 320570, 320571, 320612, 320617, 320654, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321078, 321119, 321121, 321122, 321123, 321124, 321126, 321134, 321138, 321140, 321149, 321162, 321164, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321261, 321262, 321283, 321294, 321295, 321296, 321305, 321310, 321311, 321312, 321313, 321383, 321386, 321388, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321528, 321536, 321586, 321607, 321636, 321640, 321650, 321652, 321653, 321665,321710, 321712, 321730, 321731, 321732, 321735, 321755, 321758, 321759, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321794, 321796, 321813, 321815, 321817, 321818, 321819, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321949, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322049, 322057, 322068, 322079, 322088, 322106, 322113, 322118, 322165, 322167, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322483, 322484, 322485, 322487, 322492, 322510, 322599, 322602, 322603, 322605, 322616, 322617, 322625, 322633, 322958, 322959, 322961, 322963, 322964, 323363, 323364, 323365, 323367, 323368, 323370, 323371, 323373, 323374, 323375, 323376, 323377, 323378, 323388, 323391, 323393, 323394, 323395, 323396, 323397, 323398, 323399, 323414, 323415, 323416, 323417, 323418, 323419, 323420, 323421, 323422, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323698, 323699, 323700, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324078, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420, 324564, 324571, 324612, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325111, 325113, 325114, 325117, 325159, 325168, 325169, 325170, 325172, 325175};
  
  //run_number = {315252, 315255, 315257, 315258, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315365, 315366, 315420, 315488, 315489, 315490, 315506, 315509, 315510, 315512, 315533, 315543, 315555, 315556, 315557, 315587, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315787, 315788, 315789, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316095, 316109, 316110, 316111, 316112, 316113, 316114, 316151, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219,316239, 316240, 316241, 316260, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316456, 316457, 316469, 316470, 316472, 316477, 316478, 316505, 316569, 316590, 316613, 316615, 316664, 316665, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316797, 316798, 316799, 316808, 316820, 316839, 316851, 316876,316877, 316878,316879, 316928, 316944, 316985, 316993, 316994, 316995};
  
  TFile* f=new TFile("/eos/user/a/asehrawa/afterglow_allrun_test/afterglow_allrun/afterglow.root");
  
  TH1* Residual_type1[1000];
  TGraph* residual_type1vstime;  
  residual_type1vstime=new TGraph();

  TH1* Residual_type2[1000];
  TGraph* residual_type2vstime;
  residual_type2vstime=new TGraph();
  
  TGraph *type1_residual;
  type1_residual=new TGraph();
  
  TH1* Raw_histogram[3000];
  
  for (unsigned int j=0;j<run_number.size();j++){
    Residual_type1[j] = (TH1F*)f->Get(TString("Type1Res_")+run_number.at(j));
    //if(Residual_type1[j]->GetMean() >  -0.01) { 
    residual_type1vstime->SetPoint(residual_type1vstime->GetN(), run_number.at(j), Residual_type1[j]->GetMean());
    //std::cout<<residual_type1vstime->GetN()<<"  "<<run_number.at(j)<<"  "<<Residual_type1[j]->GetMean()<<std::endl;
    //}
    Residual_type2[j] = (TH1F*)f->Get(TString("Type2Res_")+run_number.at(j));
    residual_type2vstime->SetPoint(residual_type2vstime->GetN(), run_number.at(j), Residual_type2[j]->GetMean());
    //std::cout<<residual_type2vstime->GetN()<<"  "<<run_number.at(j)<<"  "<<Residual_type2[j]->GetMean()<<std::endl;
    
  }
  
  TF1 *fitfn = new TF1("fitfn","[0]*x+[1]",315252,325175);
  //TF1 *fitfn = new TF1("fitfn","[0]*x+[1]",315252,316995);
  residual_type1vstime->SetMarkerStyle(8);
  residual_type1vstime->SetMarkerColor(2);
  residual_type1vstime->Fit("fitfn");
  
  TF1 *fitfn1 = new TF1("fitfn1","[0]*x+[1]",315252,325175);
  //TF1 *fitfn1 = new TF1("fitfn1","[0]*x+[1]",315252,316995);
  residual_type2vstime->SetMarkerStyle(8);
  residual_type2vstime->SetMarkerColor(2);
  residual_type2vstime->Fit("fitfn1");
  
  //auto nPoints = residual_type1vstime->GetN();
  //for(int i=0; i < nPoints; ++i) {
  //type1_residual->SetPoint(((residual_type1vstime->GetPointY(i))-fitfn->Eval(residual_type1vstime->GetPointX(i)))/fitfn->Eval(residual_type1vstime->GetPointX(i)));
  //}
  
  residual_type1vstime->GetXaxis()->SetTitle("run number");
  residual_type1vstime->GetYaxis()->SetTitle("type 1 residual");
  //residual_type1vstime->GetYaxis()->SetTitle("residual");
  //residual_type1vstime->GetYaxis()->SetRangeUser(-0.0001, 0.0001);
  //residual_type1vstime->SetMarkerStyle(8);
  residual_type1vstime->SetLineColor(4);
  residual_type1vstime->SetMarkerColor(4);
  residual_type1vstime->SetMarkerSize(0.5);
  fitfn->SetLineColor(2);
  fitfn->SetMarkerColor(2);
  fitfn->SetMarkerSize(0.5);
  fitfn1->SetLineColor(2);
  fitfn1->SetMarkerColor(2);
  fitfn1->SetMarkerSize(0.5);
  
  residual_type2vstime->GetXaxis()->SetTitle("run number");
  residual_type2vstime->GetYaxis()->SetTitle("type 2 residual");
  //residual_type2vstime->GetYaxis()->SetRangeUser(0, 0.02);
  residual_type2vstime->SetMarkerStyle(8);
  residual_type2vstime->SetLineColor(4);
  residual_type2vstime->SetMarkerColor(4);
  residual_type2vstime->SetMarkerSize(0.5);
  
  residual_type1vstime->Draw("AP");
  fitfn->Draw("same");
  //residual_type2vstime->Draw("PSAME");
  //residual_type2vstime->Draw("AP");
  //fitfn1->Draw("same");
  
  auto legend = new TLegend(0.75, 0.75, 0.89, 0.89);
  legend->AddEntry(residual_type1vstime, "type 1 residual", "l");
  legend->AddEntry(residual_type2vstime, "type 2 residual", "l");
  legend->SetFillColor(0);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  //legend->Draw("same");
  C->Print(outpath+"PCC_afterglow_type1_type2_residualvstime_Run2018A"+".png");
  
  
  system(TString("mkdir -p ")+outpath);
  
  TCanvas C1("C1","",1200,500);
  
  std::vector<int> run;
  
  for (unsigned int k=0;k<run_number.size();k++){
    
    run={run_number.at(k)};
    
    //iterate over the TFile keys
    TFile FCORR((inpath+"/"+run+".root").Data(),"read");
    TIter next(FCORR.GetListOfKeys());
    TObject* key;
    while ((key = next())) {
      TString kname(key->GetName());
      if(!kname.Contains("RawLumiAvg")) continue;
      TObjArray * a = kname.Tokenize("_");
      long r=atoi(((TObjString*)(*a)[1])->GetName());
      long l=atoi(((TObjString*)(*a)[2])->GetName());
      long ls1=atoi(((TObjString*)(*a)[3])->GetName());
      long ls2=atoi(((TObjString*)(*a)[4])->GetName());
      
      if(r!=run) continue;
      if(l>0) continue;//only look at the first ls block
      
      TH1F* HRAW = (TH1F*) FCORR.Get(TString("RawLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!HRAW)      continue;
      float scale=1./HRAW->GetMaximum();
      //HRAW->Scale(scale);
      
      TH1F* HCORR = (TH1F*) FCORR.Get(TString("CorrectedLumiAvg_")+r+"_"+l+"_"+ls1+"_"+ls2);
      if(!HCORR)      continue;
      //HCORR->Scale(scale);
      
      C1.Clear();
      HRAW->SetTitle(TString("Run: ")+run+" , LS: "+ls1+" - "+ls2);
      HRAW->GetYaxis()->SetTitle("counts");
      HRAW->GetXaxis()->SetTitle("bcid");
      HRAW->GetYaxis()->SetRangeUser(-0.1,1.1*HRAW->GetMaximum());
      HRAW->SetMarkerColor(1);
      HRAW->Draw("histp");
      HCORR->SetMarkerColor(4);
      //HCORR->Draw("histpsame");
      C1.Print(outpath+"/compareRandomRawCorr_"+run+"_"+l+".png");
      
      l++;
    }
  }
  
  gROOT->ProcessLine(".q");
}

