#include "plots.C"


void plot_linearity(long fill=7358){
 
  configFill(fill);
   
  
  C.Print("plot_linearity.pdf[");

  ////////   check the active bunches ////////////
  printActiveBunches();
  
  ////// show each bcid vs time
  //plot_lumi_vstime_perbx(BXSpecial);
  plot_lumi_vstime_perbx(BXSel);
  //plot_lumi_vstime_perbx(BXLeading);
  //plot_lumi_vstime_perbx(BXLIST);

  ////// Detectors vs time
  //plot_det_vstime(); //selected bcid's
  //plot_det_ratio_vstime(CUTBX);//all bcid
  
  ////// Detector correlations
  //plot_det_correlation(CUTBX);

  plot_det_linearity(BXLIST);
  //plot_det_linearity(BXLeading);
  //plot_det_linearity(BXSpecial);

  //plot_det_linearity_perbx(BXSpecial);
  //plot_det_linearity_perbx(BXLeading);
  plot_det_linearity_perbx(BXLIST);

  //plot_det_linearity_pertrain();

 
  /////  PLT channels
  //plot_plt_linearity(CUTBX);
  //plot_plt_chan_perbx();

  
  C.Print("plot_linearity.pdf]");
  gROOT->ProcessLine(".q");
  
}
