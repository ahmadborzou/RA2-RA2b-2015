void plot_MuonsFromTaus_forICHEP2016(std::string elogForPlot="Elog401_"){

  //
  // icomp=0: only show own results
  //       1: show also Koushik's results
  //
  
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
  
  double ymax=1.0;
  double ymin=0.0;
  double ytext=0.82;
  
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
  
  //sprintf(tempname,"TauHad2/Stack/Probability_Tau_mu_stacked.root");
  //sprintf(tempname,"TauHad2/Stack/Elog401_Probability_Tau_mu_stacked.root");
  //    sprintf(tempname,"TauHad2/Stack/Elog433_Probability_Tau_mu_stacked.root"); 
  sprintf(tempname,"TauHad2/Stack/%sProbability_Tau_mu_stacked_LowStatBinsNotCorrected.root",elogForPlot.c_str());
  std::cout<<" tempname received "<<endl;
  TFile *file   = new TFile(tempname,"R");
  sprintf(tempnameMod,"TauHad2/Stack/%smodifiedProbability_Tau_mu_stacked.root",elogForPlot.c_str());
  TFile *file2   = new TFile(tempnameMod,"RECREATE");

  int NumHistsToCorrect=2;
  for(int x=1;x<=NumHistsToCorrect;x++){
    if(x==1){
      std::cout<<" High delphi regio "<<endl;
      sprintf(tempname,"hProb_Tau_mu");
      sprintf(tempNum,"hNonW_mu");
      sprintf(tempDen,"hAll_mu");
    }
    if(x==2){
      std::cout<<" Low delphi regio "<<endl;
      sprintf(tempname,"hProb_Tau_mu_lowDelphi");
      sprintf(tempNum,"hNonW_mu_lowDelphi");
      sprintf(tempDen,"hAll_mu_lowDelphi");
    }

    thist = (TH1D*)file->Get(tempname)->Clone();
    histNum = (TH1D*)file->Get(tempNum)->Clone();
    histDen = (TH1D*)file->Get(tempDen)->Clone();

    int nbins=histNum->GetNbinsX();
    std::cout<<"nbins "<<nbins<<endl;

    for(int j=1;j<=nbins;j++){
      //std::cout<<"..........................."<<endl;                                                                                       
      double binvalue=0;
      double ratio=thist->GetBinContent(j);
      double newN=0;
      double newD=0;
      if(x==1){
	if(j==48 || j==52 || j==55 || j==60 || j==61 || j==64 || j==67 ||j==70 ||j==72){
	  int kbin=j+1;
	  if(j==48 || j==60 || j==72) kbin=j-1;
	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(kbin);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(kbin);
	  ratio=newN/newD;
	  binvalue=ratio;
	  double ProbMuTauOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(kbin) );

	  //if(binvalue!=0)
	  std::cout<<" Bin "<< j<< " Numerator("<<j<<") "<< histNum->GetBinContent(j) <<" Numerator("<<kbin<<") "<<histNum->GetBinContent(kbin) <<" newNumerator "<<newN<<" Denominator("<<j<<") "<<histDen->GetBinContent(j)<<" Denominator("<<kbin<<") "<< histDen->GetBinContent(kbin)<<" newDenominator "<<newD<< " oldProbMuTau("<<j<<") " << ProbMuTauOld << " newProbMuTau("<<j<<") " << ratio << " oldErr("<<j<<") " << errOld << " err("<<kbin<<") " <<  thist->GetBinError(kbin) << " newErr("<<j<<") " << thist->GetBinError(j) << endl;

	}
      }
      if(x==2){
	if(j==43 || j==52 || j==55 || j==61 || j==64 || j==67 ||j==70){
	  int kbin=j+1;
	  //if(j==60) kbin=j-1;
	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(kbin);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(kbin);
	  ratio=newN/newD;
	  binvalue=ratio;
	  double ProbMuTauOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(kbin) );
      
	  //if(binvalue!=0)
	  std::cout<<" Bin "<< j<< " Numerator("<<j<<") "<< histNum->GetBinContent(j) <<" Numerator("<<kbin<<") "<<histNum->GetBinContent(kbin) <<" newNumerator "<<newN<<" Denominator("<<j<<") "<<histDen->GetBinContent(j)<<" Denominator("<<kbin<<") "<< histDen->GetBinContent(kbin)<<" newDenominator "<<newD<< " oldProbMuTau("<<j<<") " << ProbMuTauOld << " newProbMuTau("<<j<<") " << ratio << " oldErr("<<j<<") " << errOld << " err("<<kbin<<") " <<  thist->GetBinError(kbin) << " newErr("<<j<<") " << thist->GetBinError(j) << endl;
      
	}
      }
    }
    std::cout<<" Before calling shift_bin "<<endl;
    thist_input = static_cast<TH1D*>(thist->Clone("thist_input"));
    shift_bin(thist_input,thist);
    thist_fixed = static_cast<TH1D*>(thist->Clone("thist_fixed"));
    std::cout<<" shift_bin called "<<endl;
    /*
      TH1F *thist_fixed = new TH1F("thist_fixed","thist_fixed",18,1.,19.);
      for (int ibin=0;ibin<18;ibin++){
      thist_fixed->SetBinContent(ibin+1,thist->GetBinContent(ibin+1));
      thist_fixed->SetBinError(ibin+1,thist->GetBinError(ibin+1));
      }
    */
  
    thist_fixed->SetLineColor(1);
    thist_fixed->SetLineWidth(3);
    thist_fixed->SetStats(kFALSE);

    thist_fixed->SetTitle("");
  
    thist_fixed->SetMaximum(ymax);
    thist_fixed->SetMinimum(0.0);
    thist_fixed->GetXaxis()->SetLabelFont(42);
    thist_fixed->GetXaxis()->SetLabelOffset(0.007);
    thist_fixed->GetXaxis()->SetLabelSize(0.045);
    thist_fixed->GetXaxis()->SetTitleSize(0.06);
    thist_fixed->GetXaxis()->SetTitleOffset(0.9);
    thist_fixed->GetXaxis()->SetTitleFont(42);
    thist_fixed->GetYaxis()->SetLabelFont(42);
    thist_fixed->GetYaxis()->SetLabelOffset(0.007);
    thist_fixed->GetYaxis()->SetLabelSize(0.045);
    thist_fixed->GetYaxis()->SetTitleSize(0.06);
    thist_fixed->GetYaxis()->SetTitleOffset(0.60);
    thist_fixed->GetYaxis()->SetTitleFont(42);

    //KH adhoc
    //  thist_fixed->SetBinContent(18,0.2);
  
    thist_fixed->GetYaxis()->SetTitle("Fraction of muons from #tau decays");
    thist_fixed->GetXaxis()->SetTitle("Bin number");
    thist_fixed->SetMarkerStyle(20);
    thist_fixed->Draw();

    //TLatex * ttext1 = new TLatex(6.0 , ytext , "N_{jets}=4");
    TLatex * ttext1 = new TLatex(6.25 , ytext , "N_{jets}=3");
    ttext1->SetTextFont(42);
    ttext1->SetTextSize(0.05);
    ttext1->SetTextAlign(22);
    ttext1->Draw();

    TLatex * ttext2 = new TLatex(18.25 , ytext , "N_{jets}=4");
    ttext2->SetTextFont(42);
    ttext2->SetTextSize(0.05);
    ttext2->SetTextAlign(22);
    ttext2->Draw();

    TLatex * ttext3 = new TLatex(30.25 , ytext , "N_{jets}=5");

    ttext3->SetTextFont(42);
    ttext3->SetTextSize(0.05);
    ttext3->SetTextAlign(22);
    ttext3->Draw();

    TLatex * ttext4 = new TLatex(42.25, ytext , "N_{jets}=6");
    ttext4->SetTextFont(42);
    ttext4->SetTextSize(0.05);
    ttext4->SetTextAlign(22);
    ttext4->Draw();

    TLatex * ttext5 = new TLatex(54.25 , ytext , "7 #leq N_{jets} #leq 8");
    ttext5->SetTextFont(42);
    ttext5->SetTextSize(0.05);
    ttext5->SetTextAlign(22);
    ttext5->Draw();

    TLatex * ttext6 = new TLatex(66.25 , ytext , "N_{jets} #geq 9");
    ttext6->SetTextFont(42);
    ttext6->SetTextSize(0.05);
    ttext6->SetTextAlign(22);
    ttext6->Draw();


    TLine *tline_1 = new TLine(12.5,ymin,12.5,ymax);
    tline_1->SetLineStyle(2);
    tline_1->Draw();

    TLine *tline_2 = new TLine(24.5,ymin,24.5,ymax);
    tline_2->SetLineStyle(2);
    tline_2->Draw();
  
    TLine *tline_3 = new TLine(36.5,ymin,36.5,ymax);
    tline_3->SetLineStyle(2);
    tline_3->Draw();
  
    TLine *tline_4 = new TLine(48.5,ymin,48.5,ymax);
    tline_4->SetLineStyle(2);
    tline_4->Draw();

    TLine *tline_5 = new TLine(60.5,ymin,60.5,ymax);
    tline_5->SetLineStyle(2);
    tline_5->Draw();
 
    CMS_lumi( c1, iPeriod, iPos );   // writing the lumi information and the CMS "logo"
  
    double xlatex=0.75;
    double ylatex=0.55;
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
    */
    if(x==1){
      c1->Print("plot_MuonsFromTaus.png");
      c1->Print("plot_MuonsFromTaus.pdf");
    }
    if(x==2){
      c1->Print("plot_MuonsFromTaus_lowDelphi.png");
      c1->Print("plot_MuonsFromTaus_lowDelphi.pdf");
    }

    file2->cd();
    thist->Write();
    //  TH1D *thist_lowDphi = (TH1D*)file->Get("hProb_Tau_mu_lowDelphi")->Clone();
    // thist_lowDphi->Write();
  }
}

void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];
  sprintf(tempname, "%s", output->GetName());  
  cout << "shiftbin hname " << tempname << std::endl;
  output->SetName(tempname);
  sprintf(temptitle, "%s", output->GetTitle());  
  cout << "shiftbin htitle " << temptitle << std::endl;
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
