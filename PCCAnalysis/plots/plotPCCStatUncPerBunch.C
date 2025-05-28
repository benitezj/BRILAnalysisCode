#include "globals.h"

#define PURANGE 65    //x-range 
#define PLOTYRANGE 0.5  // y-range in percent per bunch
#define PLOTYRANGEORBIT 0.15  // y-range in percent per orbit
#define NSTEP 5               //number of steps in trigger rate
#define TotalTriggerStep 50 //Total trigger rate for all bunches 
#define TotalBunches 1     //number of colliding bunches
#define PCCPerInteraction 20  //2017 veto:100, 2023 veto:20 
#define TStep 10              //integration time step


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


  
  TCanvas C("C","",1200,500);
  //C.SetLeftMargin(0.15);
  C.Divide(2,1);
  
  //////////////////////////////
  ///Stat precision per bunch
  C.cd(1);
  TH1F HFrame("HFrame","",1,0,PURANGE); //pileup range
  HFrame.GetYaxis()->SetRangeUser(0,PLOTYRANGE);
  HFrame.GetYaxis()->SetTitle("Stat precision per bunch [%]");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.SetStats(0);
  HFrame.DrawClone("hist");
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
  params+=TString(", PCC per pu = ")+PCCPerInteraction;
  params+=TString(", Integration  = ")+TStep+"s ";
  text.DrawLatexNDC(0.1,0.92,params);
  TLine line;
  line.SetLineStyle(2);
  line.DrawLine(0,1,PURANGE,1);
  //C.Print("plotPCCStatUncPerBunch.png");


  /////////////////////////////
  ///Stat precision per orbit
  C.cd(2);
  HFrame.GetYaxis()->SetRangeUser(0,PLOTYRANGEORBIT);
  HFrame.GetYaxis()->SetTitle("Stat precision per orbit [%]");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.DrawClone("hist");
  TLegend legorbit(0.6,0.9-0.05*NSTEP,0.9,0.9);
  
  for(int I=1;I<=NSTEP;I++){ 
    Gorbit[I]->SetLineColor(I);
    Gorbit[I]->SetLineWidth(2);
    Gorbit[I]->Draw("lpsame");
    legorbit.AddEntry(Gorbit[I],TString("Trigger: ")+(I*TotalTriggerStep)+" Hz","pl");
  }
  legorbit.Draw();
  text.DrawLatexNDC(0.1,0.92,params);
  line.DrawLine(0,0.05,PURANGE,0.05);
  //C.Print("plotPCCStatUncPerBunch_orbit.png");
  C.Print("plotPCCStatPrecision.png");
  
   
}
