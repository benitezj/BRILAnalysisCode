#include "globals.h"

#define PCCPerInteraction 100  //2018 mod veto, 13TeV
#define NCollidingBX 13       //BSRT fill 2022
#define TotalTrigger 70000   //
#define TStep 30  //integration time step

void plotPCCStatUncPerBunch(){


  float TriggerPerBX=TotalTrigger/(float)NCollidingBX;

  TCanvas C;
  C.SetLeftMargin(0.15);
  C.Clear();
  
  TH1F HFrame("HFrame","",1,0,100); //pileup range
  HFrame.GetYaxis()->SetRangeUser(0,0.15);
  HFrame.GetYaxis()->SetTitle("PCC stat uncertainty per bunch (%)");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.SetStats(0);
  HFrame.Draw("hist");

  
  TGraph*G[6];
  TLegend leg(0.6,0.5,0.9,0.9);
  
  for(int I=1;I<=6;I++){ 
    //calculate how many events are recorded based on the trigger
    float NEv=TriggerPerBX*I*5;//predictions for 5, 10, 15, ..., 30s integrations

    G[I]=new TGraph();
    G[I]->SetLineColor(I);
    G[I]->SetLineWidth(2);
    
    for(int p=1;p<=100;p+=1){//curves with points every 0.5 pileup
      //calculate total number of clusters and stat error
      float NPCC=PCCPerInteraction*p*NEv;
      float NPCCerr=sqrt(NPCC);

      //cout<<I<<" "<<p<<" "<<NPCCerr/NPCC<<endl;

      G[I]->SetPoint(G[I]->GetN(),p,100*NPCCerr/NPCC);
      
    }

    G[I]->Draw("lpsame");
    leg.AddEntry(G[I],TString("T=")+(I*TStep)+" seconds","pl");
  }
  leg.Draw();
  
  TLatex text;
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.2,0.8,TString("Trigger Rate = ")+(TotalTrigger/1000)+" kHz");
  text.DrawLatexNDC(0.2,0.75,TString("N bunches = ")+NCollidingBX);
  //text.DrawLatexNDC(0.2,0.70,TString("PCC per pp interaction = ")+PCCPerInteraction);
  
  C.Print("plotPCCStatUncPerBunch.png");
  
  
}
