#include "plots.C"


void plot_linearity(long fill=7358){

  gROOT->ProcessLine(".x ../rootlogon.C");
 
  configFill(fill);
  

  ////// show each bcid vs time
  std::vector<TString> detl=DETLIST;
  detl.push_back(detsel);
  plot_lumi_vstime(detl,std::vector<long>{BXSel});
  plot_lumi_vsls(detl,std::vector<long>{BXSel});
  
  plot_det_ratio_vstime(std::vector<long>{BXSel},0.9,1.20);
  plot_det_ratio_vsls(std::vector<long>{BXSel},0.9,1.20);

  //plot_det_correlation(CUTBX);

  //plot_det_linearity(std::vector<long>{BXSel});
  //plot_det_linearity(BXLIST);
  //plot_det_linearity(BXLeading);
  //plot_det_linearity(BXSpecial);
  //plot_det_linearity(BXSpecialTrain);

  //plot_linearity_compare(BXSpecial,BXTrain,"Leading bunches","Train bunches");

  //plot_det_linearity_perbx(std::vector<long>{BXSel});  
  //plot_det_linearity_perbx(BXLeading);  
  //plot_det_linearity_perbx(BXLIST);
  //plot_det_linearity_perbx(BXSpecial);
  //plot_det_linearity_perbx(BXSpecialTrain);

  
  //plot_det_linearity_pertrain();

 
  /////  PLT channels
  //plot_plt_linearity(CUTBX);
  //plot_plt_chan_perbx();

  
  gROOT->ProcessLine(".q");
  
}
