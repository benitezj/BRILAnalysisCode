#include "plots.C"


void plot_linearity(long fill=7358){

  gROOT->ProcessLine(".x ../rootlogon.C");
 
  configFill(fill);
   

  C=new TCanvas();

  //C->Print(OUTPATH+"/plot_linearity.pdf[");

  ////// show each bcid vs time
  //plot_lumi_vstime_perbx(std::vector<long>{BXSel});
  //plot_det_ratio_vstime(std::vector<long>{BXSel});
  plot_det_ratio_vstime(BXSpecial);
  
  ////// Detector correlations
  //plot_det_correlation(CUTBX);

  //
  //plot_det_linearity(BXLIST);
  //plot_det_linearity(BXLeading);
  //plot_det_linearity(BXSpecial);
  //plot_det_linearity(BXSpecialTrain);


  //plot_linearity_compare(BXSpecial,BXTrain,"Leading bunches","Train bunches");
  
  //get_bx_corrections("pcc");

  //plot_det_linearity_perbx(BXLeading);  
  //plot_det_linearity_perbx(BXLIST);
  //plot_det_linearity_perbx(BXSpecial);
  //plot_det_linearity_perbx(BXSpecialTrain);
  
  //plot_det_linearity_pertrain();

 
  /////  PLT channels
  //plot_plt_linearity(CUTBX);
  //plot_plt_chan_perbx();

  
  //C->Print(OUTPATH+"/plot_linearity.pdf]");
  gROOT->ProcessLine(".q");
  
}
