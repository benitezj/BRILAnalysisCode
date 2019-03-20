
#include "plot_linearity.C"


void compareFills(){

  configFill(6854);
  TH2F * H_6854 = getLinearityHisto("H_6854","bcm",CUTBX);//TString("bx==")+BXSel);

  configFill(7358);
  TH2F * H_7358 = getLinearityHisto("H_7358","bcm",CUTBX);//TString("bx==")+BXSel);

  C.Print("compareFills.pdf[");

  C.Clear();
  H_6854->GetYaxis()->SetRangeUser(0.85,1.15);
  H_6854->Draw("histp");
  H_7358->SetMarkerColor(1);
  H_7358->Draw("histpsame");
  C.Print("compareFills.pdf");
    
  C.Print("compareFills.pdf]");

}
