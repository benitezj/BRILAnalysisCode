void rootlogon(){
 
  //TStyle * Style = new TStyle("Style","The Perfect Style for Plots ;-)");

  // Canvas
  gStyle->SetCanvasColor     (0);
  gStyle->SetCanvasBorderSize(10);
  gStyle->SetCanvasBorderMode(0);
 
  // gStyle->SetCanvasDefH      (700);
  // gStyle->SetCanvasDefW      (700);
  // gStyle->SetCanvasDefX      (100);
  // gStyle->SetCanvasDefY      (100);

  // color palette for 2D temperature plots
  gStyle->SetPalette(1,0);
  
  // Pads
  gStyle->SetPadColor       (0);
  gStyle->SetPadBorderSize  (10);
  gStyle->SetPadBorderMode  (0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadTopMargin   (0.08);
  gStyle->SetPadLeftMargin  (0.15);
  gStyle->SetPadRightMargin (10);
  gStyle->SetPadGridX       (0);
  gStyle->SetPadGridY       (0);
  gStyle->SetPadTickX       (1);
  gStyle->SetPadTickY       (1);

  // Frames
  gStyle->SetLineWidth(3);
  gStyle->SetFrameFillStyle (0);
  gStyle->SetFrameFillColor (0);
  gStyle->SetFrameLineColor (1);
  gStyle->SetFrameLineStyle (0);
  gStyle->SetFrameLineWidth (2);
  gStyle->SetFrameBorderSize(10);
  gStyle->SetFrameBorderMode(0);
  
  ////// Histograms
  gStyle->SetHistFillColor(0);
  gStyle->SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  gStyle->SetNdivisions(10);

  // Functions
  gStyle->SetFuncColor(1);
  gStyle->SetFuncStyle(0);
  gStyle->SetFuncWidth(2);

  // Various
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerColor(kBlack);
  gStyle->SetMarkerSize (0.3);


  gStyle->SetTitleBorderSize(0);
  //gStyle->SetTitleFillColor (0);
  //gStyle->SetTitleX         (0.2);

  //gStyle->SetTitleSize  (0.055,"X");
  //gStyle->SetTitleOffset(1.400,"X");
  //gStyle->SetLabelOffset(0.005,"X");
  //gStyle->SetLabelSize  (0.050,"X");
  //gStyle->SetLabelFont  (42   ,"X");

  gStyle->SetStripDecimals(kFALSE);
  gStyle->SetLineStyleString(11,"20 10");

  //gStyle->SetTitleSize  (0.055,"Y");
  //gStyle->SetTitleOffset(1.400,"Y");
  //gStyle->SetLabelOffset(0.010,"Y");
  //gStyle->SetLabelSize  (0.050,"Y");
  //gStyle->SetLabelFont  (42   ,"Y");

  gStyle->SetTextSize   (0.055);
  gStyle->SetTextFont   (42);
  gStyle->SetStatFont   (42);
  //gStyle->SetTitleFont  (42);
  //gStyle->SetTitleFont  (42,"X");
  //gStyle->SetTitleFont  (42,"Y");

  gStyle->SetOptStat(1111111);
  gROOT->ForceStyle();
  
  //gROOT->SetStyle("Style");


  //std::cout<<"done style"<<std::endl;
}
