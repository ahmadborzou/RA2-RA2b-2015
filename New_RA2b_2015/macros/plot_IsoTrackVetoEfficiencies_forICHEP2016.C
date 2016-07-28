
//void plot_IsoTrackVetoEfficiencies_forAN(std::string elogForPlot="Elog401_"){
void plot_IsoTrackVetoEfficiencies_forICHEP2016(std::string elogForPlot="Elog401_"){
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gROOT->LoadMacro("CMS_lumi.C");
  
  writeExtraText = true;
  extraText   = "       Supplementary (Simulation)";  // default extra text is "Preliminary"
  lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  //cmsTextSize  = 0.60;
  //lumiTextSize = 0.52;

  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  int iPos=0;

  char tempname[200];
  char tempname2[200];
  char tempNum[200];
  char tempDen[200];
  char tempnameMod[200];

  int W = 1200;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;
  
  double ymax=1.1;
  double ymin=0.2;
  double ytext=0.3;
  
  TCanvas* c1 = new TCanvas("name","name",10,10,W,H);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L/W );
  c1->SetRightMargin( R/W );
  c1->SetTopMargin( T/H );
  c1->SetBottomMargin( B/H );
  c1->SetTopMargin( 0.1 );
  c1->SetLeftMargin( 0.08 );
  //c1->SetTickx(0);
  //c1->SetTicky(0);

  gStyle->SetOptStat(000000);
  gStyle->SetErrorX(0.5); 
  
  Float_t legendX1 = .60; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .60; //.65;
  Float_t legendY2 = .90;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //
  
  sprintf(tempname,"TauHad/Stack/%sIsoEfficiencies_stacked_LowStatBinNotcorrected.root",elogForPlot.c_str());

  std::cout << "Adjusting " << tempname << std::endl;
  TFile *file   = new TFile(tempname,"R");
  sprintf(tempnameMod,"TauHad/Stack/%smodifiedIsoEfficiencies_stacked.root",elogForPlot.c_str());
  std::cout << "Adjusted " << tempnameMod << std::endl;
  TFile *file2   = new TFile(tempnameMod,"RECREATE");

  int NumHistsToCorrect=2;
  for(int x=1;x<=NumHistsToCorrect;x++){
    if(x==1){
      std::cout<<" High delphi region "<<endl;
      sprintf(tempname,"IsoEff");
      sprintf(tempNum,"Iso_pass");
      sprintf(tempDen,"Iso_all");
   
    }
    if(x==2){
      std::cout<<" Low delphi region "<<endl;
      sprintf(tempname,"IsoEff_lowDphi");
      sprintf(tempNum,"Iso_pass_lowDphi");
      sprintf(tempDen,"Iso_all_lowDphi");
    }






    TH1D* thist = (TH1D*)file->Get(tempname)->Clone();
    TH1D* histNum = (TH1D*)file->Get(tempNum)->Clone();
    TH1D* histDen = (TH1D*)file->Get(tempDen)->Clone();

    int nbins=histNum->GetNbinsX();
    std::cout<<"nbins "<<nbins<<endl;
  
    if(x==1){
      for(int j=1;j<=nbins;j++){
	double binvalue=0;
	double ratio=thist->GetBinContent(j);
	double newN=0;
	double newD=0;    
	//std::cout <<" j "<<j << " Num " << histNum->GetBinContent(j) << " Den " << histDen->GetBinContent(j) << " " << thist->GetName() << " bincontent "<< ratio <<endl;

	if(j==55 || j==60 ||j==61 || j==64 || j==67 || j==70 ||j==72){  

	  int kbin=j+1;
	  // bin 61 is low stat and 72 is last, so we use the previous bin
	  if(j==60 ||j==72) kbin=j-1;

	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(kbin);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(kbin);
	  ratio=newN/newD;
	  binvalue=ratio;
	  double IsoEffOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(kbin) );

	  std::cout<<" Bin "<< j<< " Numerator("<<j<<") "<< histNum->GetBinContent(j) <<" Numerator("<<kbin<<") "<<histNum->GetBinContent(kbin) <<" newNumerator "<<newN<<" Denominator("<<j<<") "<<histDen->GetBinContent(j)<<" Denominator("<<kbin<<") "<< histDen->GetBinContent(kbin)<<" newDenominator "<<newD<< " oldIsoEff("<<j<<") " << IsoEffOld << " newIsoEff("<<j<<") " << ratio << " oldErr("<<j<<") " << errOld << " err("<<kbin<<") " <<  thist->GetBinError(kbin) << " newErr("<<j<<") " << thist->GetBinError(j) << endl;
	} 
      }
    }

    if(x==2){
      for(int j=1;j<=nbins;j++){
	double binvalue=0;
	double ratio=thist->GetBinContent(j);
	double newN=0;
	double newD=0;    
	//std::cout <<" j "<<j << " Num " << histNum->GetBinContent(j) << " Den " << histDen->GetBinContent(j) << " " << thist->GetName() << " bincontent "<< ratio <<endl;

	if(j==43 || j==52 ||j==55 || j==61 || j==64 || j==67 || j==68 || j==70 ||j==72){  

	  int kbin=j+1;
	  // bin 61 is low stat and 72 is last, so we use the previous bin
	  if(j==67 ||j==72) kbin=j-1;

	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(kbin);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(kbin);
	  ratio=newN/newD;
	  binvalue=ratio;
	  double IsoEffOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(kbin) );

	  std::cout<<" Bin "<< j<< " Numerator("<<j<<") "<< histNum->GetBinContent(j) <<" Numerator("<<kbin<<") "<<histNum->GetBinContent(kbin) <<" newNumerator "<<newN<<" Denominator("<<j<<") "<<histDen->GetBinContent(j)<<" Denominator("<<kbin<<") "<< histDen->GetBinContent(kbin)<<" newDenominator "<<newD<< " oldIsoEff("<<j<<") " << IsoEffOld << " newIsoEff("<<j<<") " << ratio << " oldErr("<<j<<") " << errOld << " err("<<kbin<<") " <<  thist->GetBinError(kbin) << " newErr("<<j<<") " << thist->GetBinError(j) << endl;
	} 
      }
    }


  std::cout << "thist name " << thist->GetName() << std::endl;

  TH1D* thist_input = static_cast<TH1D*>(thist->Clone("thist_input"));
  shift_bin(thist_input,thist);
  TH1D* thist_fixed = static_cast<TH1D*>(thist->Clone("thist_fixed"));

  thist->SetLineColor(1);
  thist->SetLineWidth(3);
  thist->SetStats(kFALSE);

  thist->SetTitle("");
  thist->GetXaxis()->SetRangeUser(0.5,72.5);  
  thist->SetMaximum(ymax);
  thist->SetMinimum(ymin);
  thist->GetXaxis()->SetLabelFont(42);
  thist->GetXaxis()->SetLabelOffset(0.007);
  thist->GetXaxis()->SetLabelSize(0.045);
  thist->GetXaxis()->SetTitleSize(0.06);
  thist->GetXaxis()->SetTitleOffset(0.9);
  thist->GetXaxis()->SetTitleFont(42);
  thist->GetYaxis()->SetLabelFont(42);
  thist->GetYaxis()->SetLabelOffset(0.007);
  thist->GetYaxis()->SetLabelSize(0.045);
  thist->GetYaxis()->SetTitleSize(0.06);
  thist->GetYaxis()->SetTitleOffset(0.60);
  thist->GetYaxis()->SetTitleFont(42);

  thist->GetYaxis()->SetTitle("Isotrack veto efficiency");
  thist->GetXaxis()->SetTitle("Bin number");
  thist->SetMarkerStyle(20);
  thist->Draw();

  TLatex * ttext1 = new TLatex(6.25 , ytext , " N_{jets} = 3");
  //TLatex * ttext1 = new TLatex(5.5 , ytext , " N_{jets} = 3");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  //TLatex * ttext1 = new TLatex(16.5 , ytext , " N_{jets} = 3"); 
  TLatex * ttext2 = new TLatex(18.25 , ytext , " N_{jets} = 4");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  //TLatex * ttext1 = new TLatex(27.5 , ytext , " N_{jets} = 3"); 
  TLatex * ttext3 = new TLatex(30.25 , ytext , " N_{jets} = 5");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  //TLatex * ttext1 = new TLatex(38.5 , ytext , " N_{jets} = 3");
  TLatex * ttext4 = new TLatex(42.25 , ytext , " N_{jets} = 6");
  ttext4->SetTextFont(42);
  ttext4->SetTextSize(0.04);
  ttext4->SetTextAlign(22);
  ttext4->Draw();

  //TLatex * ttext5 = new TLatex(46.5 , ytext , "7 #leq N_{jets} #leq 8");
  TLatex * ttext5 = new TLatex(54.25 , ytext , "7 #leq N_{jets} #leq 8");
  ttext5->SetTextFont(42);
  ttext5->SetTextSize(0.04);
  ttext5->SetTextAlign(22);
  ttext5->Draw();

  //TLatex * ttext6 = new TLatex(52.5 , ytext , "N_{jets} #geq 9");
  TLatex * ttext6 = new TLatex(66.25 , ytext , "N_{jets} #geq 9");
  ttext6->SetTextFont(42);
  ttext6->SetTextSize(0.04);
  ttext6->SetTextAlign(22);
  ttext6->Draw();

  TLine *tline_1 = new TLine(12.5,ymin,12.5,ymax);
  //TLine *tline_1 = new TLine(11.,ymin,11.,ymax);
  tline_1->SetLineStyle(2);
  tline_1->Draw();

  TLine *tline_2 = new TLine(24.5,ymin,24.5,ymax);
  //TLine *tline_2 = new TLine(22.,ymin,22.,ymax);
  tline_2->SetLineStyle(2);
  tline_2->Draw();

  TLine *tline_3 = new TLine(36.5,ymin,36.5,ymax);
  //TLine *tline_3 = new TLine(33.,ymin,33.,ymax);
  tline_3->SetLineStyle(2);
  tline_3->Draw();

  TLine *tline_4 = new TLine(48.5,ymin,48.5,ymax);
  //TLine *tline_4 = new TLine(44.,ymin,44.,ymax);
  tline_4->SetLineStyle(2);
  tline_4->Draw();

  TLine *tline_5 = new TLine(60.5,ymin,60.5,ymax);
  //TLine *tline_5 = new TLine(50.,ymin,50.,ymax);
  tline_5->SetLineStyle(2);
  tline_5->Draw();


  CMS_lumi( c1, iPeriod, iPos );   // writing the lumi information and the CMS "logo"

  double xlatex=0.75;
  double ylatex=0.70;


    if(x==1){
      c1->Print("plot_IsoTrackVetoEfficiencies.png");
      c1->Print("plot_IsoTrackVetoEfficiencies.pdf");
    }

    if(x==2){
      c1->Print("plot_IsoTrackVetoEfficiencies_lowDphi.png");
      c1->Print("plot_IsoTrackVetoEfficiencies_lowDphi.pdf");
    }

    file2->cd();
    thist->Write();
  }
  sprintf(tempname,"IsoEff_NbNjet34"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet56"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet78"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet9");  thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();

  sprintf(tempname,"IsoEff_NbNjet34_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet56_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet78_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet9_lowDphi");  thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();

  file2->Close();


}






  /*
  TLatex *   tex = new TLatex(xlatex,ylatex,"arXiv:1602.06581");
  tex->SetTextColor(4);
  tex->SetTextFont(61);
  tex->SetTextSize(0.055);
  tex->SetLineColor(4);
  tex->SetLineWidth(2);
  //tex->Draw();
  tex->DrawLatexNDC(xlatex,ylatex-0.2,"arXiv:1602.06581");
  */
  /*
  TPaveText pt(xlatex,ylatex,xlatex+0.19,ylatex+0.1,"NDC");
  pt.AddText("arXiv:1602.06581");
  pt.SetFillColor(0);
  pt.SetLineColor(0);
  pt.SetLineWidth(0);
  pt.SetBorderSize(0);
  pt.SetTextColor(4);
  pt.SetTextFont(61);
  pt.SetTextSize(0.055);
  pt.Draw();
  
  c1->Print("plot_IsoTrackVetoEfficiencies.png");
  file2->cd();
  thist->Write();
  TH1D *thist_lowDphi = (TH1D*)file->Get("IsoEff_lowDphi")->Clone();
  thist_lowDphi->Write();

  //KH: keep the efficiency inputs for record
  histNum->Write();
  histDen->Write();
  
  //KH: Nb-dependence
  sprintf(tempname,"IsoEff_NbNjet34"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet56"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet78"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet9");  thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();

  sprintf(tempname,"IsoEff_NbNjet34_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet56_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet78_lowDphi"); thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();
  sprintf(tempname,"IsoEff_NbNjet9_lowDphi");  thist = (TH1D*)file->Get(tempname)->Clone(); thist->Write();

  file2->Close();

}
  */
void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];  
  sprintf(tempname, "%s", output->GetName());
  output->SetName(tempname);
  output->SetTitle(temptitle);
  output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  //input->Print("all");
  //output = new TH1D(tempname,temptitle,input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5); 
  // 0: underflow
  // 1: first bin [Use the lowedge of this bin]
  // input->GetNbinsX(): highest bin 
  // input->GetNbinsX()+1: overflow bin [use the lowedge of this bin]
  //

  for (int ibin=1;ibin<=input->GetNbinsX();ibin++){
    output->SetBinContent(ibin,input->GetBinContent(ibin));    
    output->SetBinError(ibin,input->GetBinError(ibin));    
    //std::cout << input->GetBinContent(ibin) << std::endl;
  }

}
