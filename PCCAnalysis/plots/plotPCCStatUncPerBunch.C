#include "globals.h"

#define PURANGE 60
#define PLOTYRANGE 0.5
#define NTSTEP 10
#define TriggerPerBXStep 200  //Hz
#define PCCPerInteraction 100  //2018 mod veto, 13TeV
#define TStep 50            //integration time step


void plotPCCStatUncPerBunch(){


  //float TriggerPerBXStep=TotalTrigger/(float)NCollidingBX;
  

  TCanvas C;
  C.SetLeftMargin(0.15);
  C.Clear();
  
  TH1F HFrame("HFrame","",1,0,PURANGE); //pileup range
  HFrame.GetYaxis()->SetRangeUser(0,PLOTYRANGE);
  HFrame.GetYaxis()->SetTitle("Stat unc. per bunch [%]");
  HFrame.GetXaxis()->SetTitle("Pileup");
  HFrame.SetStats(0);
  HFrame.Draw("hist");

  
  TGraph*G[6];
  TLegend leg(0.6,0.5,0.9,0.9);
  
  for(int I=1;I<=NTSTEP;I++){ 
    //calculate how many events are recorded based on the trigger
    float NEv=(TriggerPerBXStep*I)*TStep;

    G[I]=new TGraph();
    G[I]->SetLineColor(I);
    G[I]->SetLineWidth(2);
    
    for(int p=1;p<=PURANGE;p+=1){//curves with points every 0.5 pileup
      //calculate total number of clusters and stat error
      float NPCC=PCCPerInteraction*p*NEv;
      float NPCCerr=sqrt(NPCC);

      //cout<<I<<" "<<p<<" "<<NPCCerr/NPCC<<endl;

      G[I]->SetPoint(G[I]->GetN(),p,100*NPCCerr/NPCC);
      
    }

    G[I]->Draw("lpsame");
    leg.AddEntry(G[I],TString("")+(TriggerPerBXStep*I)+" Hz","pl");
  }
  leg.Draw();
  
  TLatex text;
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.2,0.80,TString("Scan step time = ")+TStep+" s");
  text.DrawLatexNDC(0.2,0.75,TString("PCC per pp = ")+PCCPerInteraction);
  //text.DrawLatexNDC(0.2,0.8,TString("Trigger Rate = ")+(TotalTrigger)+" Hz");
  //text.DrawLatexNDC(0.2,0.75,TString("N bunches = ")+NCollidingBX);

  
  C.Print("plotPCCStatUncPerBunch.png");
  
  
}
