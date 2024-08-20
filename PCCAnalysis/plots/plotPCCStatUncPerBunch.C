#include "globals.h"

#define PURANGE 65    //x-range 
#define PLOTYRANGE 3  // y-range in percent
#define NSTEP 3               //number of steps in trigger rate
#define TotalTriggerStep  500 //Total trigger rate for all bunches 
#define TotalBunches 2300     //number of colliding bunches
#define PCCPerInteraction 20  //2017 veto:100, 2023 veto:20 
#define TStep 23              //integration time step


void plotPCCStatUncPerBunch(){

  TGraph* G[6];
  TGraph* Gorbit[6];
   
  for(int I=1;I<=NSTEP;I++){ 
    //calculate how many events are recorded based on the trigger    
    float NEv=((TotalTriggerStep/(float)TotalBunches))*TStep*I;

    G[I]=new TGraph();
    Gorbit[I]=new TGraph();
    
    for(int p=1;p<=PURANGE;p+=1){
      
      float NPCC = PCCPerInteraction*p*NEv;
      G[I]->SetPoint(G[I]->GetN(),p,100/sqrt(NPCC));

      float NPCCorbit = PCCPerInteraction*p*NEv*TotalBunches;
      Gorbit[I]->SetPoint(Gorbit[I]->GetN(),p,100/sqrt(NPCCorbit));

    }
  }


  
  TCanvas C;
  C.SetLeftMargin(0.15);

  ///Stat precision per bunch
  C.Clear(); 
  TH1F HFrame("HFrame","",1,0,PURANGE); //pileup range
  HFrame.GetYaxis()->SetRangeUser(0,PLOTYRANGE);
  HFrame.GetYaxis()->SetTitle("Stat unc. per bunch [%]");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.SetStats(0);
  HFrame.Draw("hist");
  TLegend leg(0.6,0.9-0.05*NSTEP,0.9,0.9);
  for(int I=1;I<=NSTEP;I++){ 
    G[I]->SetLineColor(I);
    G[I]->SetLineWidth(2);
    G[I]->Draw("lpsame");
    leg.AddEntry(G[I],TString("Trigger: ")+(I*TotalTriggerStep)+" Hz","pl");
  }
  leg.Draw();
  
  TLatex text;
  text.SetTextSize(0.03);
  TString params=TString("# of bunches = ")+TotalBunches;
  params+=TString(",   PCC per p.u. = ")+PCCPerInteraction;
  params+=TString(",   Integration time = ")+TStep+"s ";
  text.DrawLatexNDC(0.15,0.92,params);
  C.Print("plotPCCStatUncPerBunch.png");





  ///Stat precision per orbit
  C.Clear();
  HFrame.GetYaxis()->SetRangeUser(0,PLOTYRANGE/10);
  HFrame.GetYaxis()->SetTitle("Stat unc. per orbit [%]");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.Draw("hist");
  TLegend legorbit(0.6,0.9-0.05*NSTEP,0.9,0.9);
  
  for(int I=1;I<=NSTEP;I++){ 
    Gorbit[I]->SetLineColor(I);
    Gorbit[I]->SetLineWidth(2);
    Gorbit[I]->Draw("lpsame");
    legorbit.AddEntry(Gorbit[I],TString("Trigger: ")+(I*TotalTriggerStep)+" Hz","pl");
  }
  legorbit.Draw();
  text.DrawLatexNDC(0.15,0.92,params);
  C.Print("plotPCCStatUncPerBunch_orbit.png");
  
  
  
}
