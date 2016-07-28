void plot_Acceptance_forICHEP2016(std::string elogForPlot=""){
  
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gROOT->LoadMacro("CMS_lumi_v2.C");
  
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
  double xmin=0.;  
  TCanvas* c1 = new TCanvas("name","name",10,10,W,H);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L/W );
  c1->SetRightMargin( R/W );
  c1->SetTopMargin( T/H );
  c1->SetBottomMargin( B/H );
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
  
  //TFile *file = new TFile("LostLepton2_MuonEfficienciesFromTTbar_Elog213.root","R"); 
  //sprintf(tempname,"TauHad/Stack/%sLostLepton2_MuonEfficienciesFromstacked.root",elogForPlot.c_str());
  sprintf(tempname,"TauHad/Stack/%sLostLepton2_MuonEfficienciesFromstacked_LowStatBinNotcorrected.root",elogForPlot.c_str());
  TFile *file   = new TFile(tempname,"R");

  sprintf(tempnameMod,"TauHad/Stack/%smodifiedLostLepton2_MuonEfficienciesFromstacked.root",elogForPlot.c_str());
  TFile *file2   = new TFile(tempnameMod,"RECREATE");

  int NumHistsToCorrect=2;
  for(int x=1;x<=NumHistsToCorrect;x++){
    if(x==1){
      std::cout<<" High delphi region "<<endl;
      sprintf(tempname,"hAcc");
      sprintf(tempNum,"hAccPass");
      sprintf(tempDen,"hAccAll");
    }
    if(x==2){
      std::cout<<" Low delphi region "<<endl;
      sprintf(tempname,"hAcc_lowDphi");
      sprintf(tempNum,"hAccPass_lowDphi");
      sprintf(tempDen,"hAccAll_lowDphi");
    }


    thist = (TH1D*)file->Get(tempname)->Clone();
    histNum = (TH1D*)file->Get(tempNum)->Clone();
    histDen = (TH1D*)file->Get(tempDen)->Clone();

    int nbins=histNum->GetNbinsX();
    std::cout<<"nbins "<<nbins<<endl;
    if(x==1){
      for(int j=1;j<=nbins;j++){
	//std::cout<<"..........................."<<endl;
	double binvalue=0;
	double ratio=thist->GetBinContent(j);
	double newN=0;
	double newD=0;
	//std::cout<<"Bin "<< j<< "thist_j"<<ratio<<endl;
	if(j==52 || j==55 || j==61 || j==64 || j==67){
	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(j+1);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(j+1);      
	  ratio=newN/newD;
	  binvalue=ratio;
      
	  double accOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(j+1) );

	  if(binvalue!=0)
	    std::cout<<" Bin "<< j<< " Numerator(j) "<< histNum->GetBinContent(j) <<" Numerator(j+1) "<<histNum->GetBinContent(j+1) <<" newNumerator "<<newN<<" Denominator(j) "<<histDen->GetBinContent(j)<<" Denominator(j+1) "<< histDen->GetBinContent(j+1)<<" newDenominator "<<newD<< " oldAcc(j) " << accOld << " newAcc(j) " << ratio << " oldErr(j) " << errOld << " err(j+1) " <<  thist->GetBinError(j+1) << " newErr(j) " << thist->GetBinError(j) << endl;

	}
      }
    }
    if(x==2){
      for(int j=1;j<=nbins;j++){
	//std::cout<<"..........................."<<endl;
	double binvalue=0;
	double ratio=thist->GetBinContent(j);
	double newN=0;
	double newD=0;
	//std::cout<<"Bin "<< j<< "thist_j"<<ratio<<endl;
	if(j==52 || j==55 || j==61 || j==64 || j==67 || j==70 || j==72){
	  int kbin=j+1;
	  if(j==72) kbin=j-1;
	  newN=histNum->GetBinContent(j)+histNum->GetBinContent(kbin);
	  newD=histDen->GetBinContent(j)+histDen->GetBinContent(kbin);      
	  ratio=newN/newD;
	  binvalue=ratio;
      
	  double accOld = thist->GetBinContent(j);
	  double errOld = thist->GetBinError(j);
	  thist->SetBinContent(j,ratio);
	  thist->SetBinError(j,  thist->GetBinError(kbin) );

	  if(binvalue!=0)
	    std::cout<<" Bin "<< j<< " Numerator("<<j<<") "<< histNum->GetBinContent(j) <<" Numerator("<<kbin<<") "<<histNum->GetBinContent(kbin) <<" newNumerator "<<newN<<" Denominator("<<j<<") "<<histDen->GetBinContent(j)<<" Denominator("<<kbin<<") "<< histDen->GetBinContent(kbin)<<" newDenominator "<<newD<< " oldAcc("<<j<<") " << accOld << " newAcc("<<j<<") " << ratio << " oldErr("<<j<<") " << errOld << " err("<<kbin<<") " <<  thist->GetBinError(kbin) << " newErr("<<j<<") " << thist->GetBinError(j) << endl;

	}
      }
    }

  

    thist->SetLineColor(1);
    thist->SetLineWidth(3);
    thist->SetStats(kFALSE);

    thist->SetTitle("");
    thist->GetXaxis()->SetRangeUser(1,73);

    thist->SetMaximum(ymax);
    thist->SetMinimum(ymin);
    thist->GetXaxis()->SetLabelFont(42);
    thist->GetXaxis()->SetLabelOffset(0.007);
    thist->GetXaxis()->SetLabelSize(0.04);
    thist->GetXaxis()->SetTitleSize(0.05);
    thist->GetXaxis()->SetTitleOffset(1.0);
    thist->GetXaxis()->SetTitleFont(42);
    thist->GetYaxis()->SetLabelFont(42);
    thist->GetYaxis()->SetLabelOffset(0.007);
    thist->GetYaxis()->SetLabelSize(0.04);
    thist->GetYaxis()->SetTitleSize(0.05);
    thist->GetYaxis()->SetTitleOffset(1.20);
    thist->GetYaxis()->SetTitleFont(42);

    thist->GetYaxis()->SetTitle("Acceptance");
    thist->GetXaxis()->SetTitle("Bin");
    thist->SetMarkerStyle(20);
    thist->Draw();

    TLatex * ttext1 = new TLatex(6.5 , ytext , " N_{jets} = 3");
    //TLatex * ttext1 = new TLatex(5.5 , ytext , " N_{jets} = 3");                                                                                                     
    ttext1->SetTextFont(42);
    ttext1->SetTextSize(0.04);
    ttext1->SetTextAlign(22);
    ttext1->Draw();
   //TLatex * ttext1 = new TLatex(16.5 , ytext , " N_{jets} = 3");                                                                                                     
    TLatex * ttext2 = new TLatex(18.5 , ytext , " N_{jets} = 4");
    ttext2->SetTextFont(42);
    ttext2->SetTextSize(0.04);
    ttext2->SetTextAlign(22);
    ttext2->Draw();

    //TLatex * ttext1 = new TLatex(27.5 , ytext , " N_{jets} = 3");                                                                                                     
    TLatex * ttext3 = new TLatex(30.5 , ytext , " N_{jets} = 5");
    ttext3->SetTextFont(42);
    ttext3->SetTextSize(0.04);
    ttext3->SetTextAlign(22);
    ttext3->Draw();

    //TLatex * ttext1 = new TLatex(38.5 , ytext , " N_{jets} = 3");                                                                                                     
    TLatex * ttext4 = new TLatex(42.5 , ytext , " N_{jets} = 6");
    ttext4->SetTextFont(42);
    ttext4->SetTextSize(0.04);
    ttext4->SetTextAlign(22);
    ttext4->Draw();
    //TLatex * ttext5 = new TLatex(46.5 , ytext , "7 #leq N_{jets} #leq 8");
    TLatex * ttext5 = new TLatex(54.5 , ytext , "7 #leq N_{jets} #leq 8");
    ttext5->SetTextFont(42);
    ttext5->SetTextSize(0.04);
    ttext5->SetTextAlign(22);
    ttext5->Draw();

    //TLatex * ttext6 = new TLatex(52.5 , ytext , "N_{jets} #geq 9");                                                                                             
    TLatex * ttext6 = new TLatex(66.5 , ytext , "N_{jets} #geq 9");
    ttext6->SetTextFont(42);
    ttext6->SetTextSize(0.04);
    ttext6->SetTextAlign(22);
    ttext6->Draw();


    TLine *tline_1 = new TLine(13,ymin,13,ymax);
    tline_1->SetLineStyle(2);
    tline_1->Draw();

    TLine *tline_2 = new TLine(25,ymin,25,ymax);
    tline_2->SetLineStyle(2);
    tline_2->Draw();
  
    TLine *tline_3 = new TLine(37,ymin,37,ymax);
    tline_3->SetLineStyle(2);
    tline_3->Draw();

    TLine *tline_4 = new TLine(49,ymin,49,ymax);
    tline_4->SetLineStyle(2);
    tline_4->Draw();
  
    TLine *tline_5 = new TLine(61,ymin,61,ymax);
    tline_5->SetLineStyle(2);
    tline_5->Draw();
  
    if(x==1){
      c1->Print("plot_Acceptance.png");
      c1->Print("plot_Acceptance.pdf");
    }

    if(x==2){
      c1->Print("plot_Acceptance_lowDphi.png");
      c1->Print("plot_Acceptance_lowDphi.pdf");
    }

    file2->cd();
    thist->Write();
  }
}
