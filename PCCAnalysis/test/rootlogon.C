void rootlogon() {
  TStyle *CMS = new TStyle("CMS", "CMS Style");

  // For the canvas
  CMS->SetCanvasBorderMode(0);
  CMS->SetCanvasColor(kWhite);
  CMS->SetCanvasDefH(600);
  CMS->SetCanvasDefW(600);
  CMS->SetCanvasDefX(0);
  CMS->SetCanvasDefY(0);

  // For the pad
  CMS->SetPadBorderMode(0);
  CMS->SetPadColor(kWhite);
  CMS->SetPadGridX(false);
  CMS->SetPadGridY(false);
  CMS->SetGridColor(0);
  CMS->SetGridStyle(3);
  CMS->SetGridWidth(1);

  // For the frame
  CMS->SetFrameBorderMode(0);
  CMS->SetFrameBorderSize(1);
  CMS->SetFrameFillColor(0);
  CMS->SetFrameFillStyle(0);
  CMS->SetFrameLineColor(1);
  CMS->SetFrameLineStyle(1);
  CMS->SetFrameLineWidth(1);

  // Margins
  CMS->SetPadTopMargin(1);    // Default: 0.08
  CMS->SetPadBottomMargin(1); // Default: 0.13
  CMS->SetPadLeftMargin(1);   // Default: 0.16
  CMS->SetPadRightMargin(1);  // Default: 0.02

  // For the Global title
  CMS->SetOptTitle(0);
  CMS->SetTitleFont(42);
  CMS->SetTitleColor(1);
  CMS->SetTitleTextColor(1);
  CMS->SetTitleFillColor(10);
  CMS->SetTitleFontSize(0.05);

  // For the axis titles
  CMS->SetTitleColor(1, "XYZ");
  CMS->SetTitleFont(42, "XYZ");
  CMS->SetTitleSize(0.03, "XYZ");
  CMS->SetTitleXOffset(0.9);
  CMS->SetTitleYOffset(1.25);

  // For the axis labels
  CMS->SetLabelColor(1, "XYZ");
  CMS->SetLabelFont(42, "XYZ");
  CMS->SetLabelOffset(0.007, "XYZ");
  CMS->SetLabelSize(0.03, "XYZ");

  // For the axis
  CMS->SetAxisColor(1, "XYZ");
  CMS->SetStripDecimals(kTRUE);
  CMS->SetTickLength(0.03, "XYZ");
  CMS->SetNdivisions(510, "XYZ");

  // For the legend
  CMS->SetLegendBorderSize(0);
  CMS->SetLegendFont(42);

  // Change for CMS: pad tick marks
  CMS->SetPadTickX(1);
  CMS->SetPadTickY(1);

  // Histos
  CMS->SetHistLineColor(1);
  CMS->SetHistLineStyle(0);
  CMS->SetHistLineWidth(1);
  CMS->SetEndErrorSize(2);
  CMS->SetErrorX(0.5);
  CMS->SetMarkerStyle(20);
  CMS->SetMarkerSize(0.5);

  // Fit/function
  CMS->SetOptFit(0);
  CMS->SetFitFormat("5.4g");
  CMS->SetFuncColor(2);
  CMS->SetFuncStyle(1);
  CMS->SetFuncWidth(1);

  // Date
  CMS->SetOptDate(0);

  // Statistics box
  CMS->SetOptFile(0);
  CMS->SetOptStat(0);
  CMS->SetStatColor(kWhite);
  CMS->SetStatFont(42);
  CMS->SetStatFontSize(0.025);
  CMS->SetStatStyle(0);
  CMS->SetStatBorderSize(1);
  CMS->SetStatX(0.9);
  CMS->SetStatY(0.9);

  gROOT->SetStyle("CMS");
  gROOT->ForceStyle();

  gSystem->Load("libHist");
  gSystem->Load("libTree");

  std::cout << "ROOT has been configured with CMS style settings." << std::endl;
}
