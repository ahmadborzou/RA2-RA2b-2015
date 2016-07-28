#include <vector>
#include <cstdio>

using namespace std;

/*

root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","searchH_b")'

.L Plot_searchBin_full_Data.C
Plot_searchBin_full_Data("stacked","searchH_b")
Plot_searchBin_full_Data("stacked","hPredHTMHT0b")
Plot_searchBin_full_Data("stacked","hPredHTMHTwb")
Plot_searchBin_full_Data("stacked","hPredNJetBins")
Plot_searchBin_full_Data("stacked","hPredNbBins")

root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","searchH_b",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","QCD_Up",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","QCD_Low",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","hPredHTMHT0b",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","hPredHTMHTwb",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","hPredNJetBins",1.28)'
root.exe -b -q 'Plot_searchBin_full_Data.C("stacked","hPredNbBins",1.28)'

*/

void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];  
  output->SetName(tempname);
  output->SetTitle(temptitle);
  //  output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  std::cout<<" input_GetBinLowEdge(0) "<<input->GetBinLowEdge(0)<<" input_GetBinCener(0) "<<input->GetBinCenter(0)<<std::endl;
  std::cout<<" input_GetNbinsX "<<input->GetNbinsX()<<" input_GetBinLowEdge(1)-0.5 "<<input->GetBinLowEdge(1)-0.5<<" input_GetBinLowEdge(input->GetNbinsX()+1)-0.5 "<<input->GetBinLowEdge(input->GetNbinsX()+1)-0.5<<endl;
  std::cout<<" output_GetBinLowEdge(1) "<<output->GetBinLowEdge(1)<<std::endl;  

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

Plot_searchBin_full_Data(string sample="stacked",string histname="searchH_b",
			 //double lumiTarget=2.109271, double lumiControl=2.093663,
			 double lumiTarget=2.584653, double lumiControl=2.585297,
			 bool normalize=false,double trigEff=1.0){ // Now trigger efficiency is corrected in the code

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents.
  //
  // Set basic style
  //
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gStyle->SetPalette(1) ; // for better color output
  //gROOT->LoadMacro("CMS_lumi_v2.C");

  //
  // Canvas size

  int W = 1200;
  int H = 740;
  int H_ref = 740;
  int W_ref = 800;

  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;

  //
  // Various vertical line coordinates
  float ymax_top = 40000.;
  float ymin_top = 0.11;
  //float ymin_top = 0.002;

  float ymax2_top = 1000.;
  float ymax3_top = 200.;
  float ymax4_top = 30.;

  float ymax_bottom = 2.65;
  float ymin_bottom = 0.;

  float ymax2_bottom = 2.15;
  float ymax3_bottom = 2.15;
  float ymax4_bottom = 2.15;

  //
  // Luminosity information for scaling
  //double lumi = 10.; // normaliza to 10 (fb-1)

  double xsec_ttbar   = 806.1; // (pb) https://twiki.cern.ch/twiki/bin/viewauth/CMS/RA2b13TeV
  int    nevent_ttbar = 25348009;
  double lumi_ttbar   = 31.445; // (fb-1)
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // More specific style set, opening input files etc

  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots

  char tempname[200];
  char tempname2[200];
  char xtitlename[200];
  char ytitlename[200];
  
  //data file
  //TFile * EstFile = new TFile("TauHad2/HadTauEstimation_data_SingleMuon_v15cd_.root","R");
  //TFile * EstFile = new TFile("TauHad2/Elog404_HadTauEstimation_data_SingleMuon_v15d_TriggerOn.root","R");
  //  TFile * EstFile = new TFile("TauHad2/HadTauEstimation_data_SingleMuon_v15d_Elog408V3_V5_.root","R");
  //    TFile * EstFile = new TFile("TauHad2/HadTauEstimation_data_SingleMuon_v16b_.root","R");
  //    TFile * EstFile = new TFile("TauHad2/ARElog40_2.6_HadTauEstimation_data_SingleMuon_v17a_.root","R");
  //    TFile * EstFile = new TFile("TauHad2/ARElog41_2.6fb_HadTauEstimation_data_SingleMuon_v17a_.root","R");
  //TFile * EstFile = new TFile("TauHad2/ARElog42_4fb_HadTauEstimation_data_SingleMuon_v16b_.root","R");
  //  TFile * EstFile = new TFile("TauHad2/ARElog49_7.6ifb_HadTauEstimation_data_SingleMuon_V9bc_.root","R");
  //  TFile * EstFile = new TFile("TauHad2/ARElog56_12.9ifb_HadTauEstimation_data_SingleMuon_V9bcd_.root","R");
  TFile * EstFile = new TFile("TauHad2/ARElog60_12.9ifb_HadTauEstimation_data_SingleMuon_V9bcd_.root","R");
  sprintf(tempname,"TauHad/Stack/ARElog57_GenInfo_HadTauEstimation_%s.root",sample.c_str());
  //cout << "warning:\n Warning \n \n  using elog195 for pre and  exp \n \n ";
  TFile * GenFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
 
 

  //
  // Define legend
  //
  //  Float_t legendX1 = .66; //.50;
  Float_t legendX1 = .55;
  //  Float_t legendX2 = .85; //.70;
  Float_t legendX2 = .90;
  //  Float_t legendY1 = .80; //.65;
  Float_t legendY1 = .50;
  //  Float_t legendY2 = .92;
  //  Float_t legendY2 = .67;
  Float_t legendY2 = .65;

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);

  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //
  // Define canvas
  //
  TCanvas *canvas = new TCanvas("canvas","canvas",10,10,W,H);

  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetLeftMargin( L/W );
  canvas->SetRightMargin( R/W );
  canvas->SetRightMargin( 0.1 );
  canvas->SetTopMargin( T/H );
  canvas->SetBottomMargin( B/H );
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  canvas->Divide(1, 2);
  
  //
  // Define pads
  //
  TPad* canvas_up = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_1");
  TPad* canvas_dw = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_2");

  //
  // define the size
  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.1;  // please tune the font size parameter for bottom figure
  double dw_height    = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.040; // KH, added to put the bottom one closer to the top panel

  //
  // set pad size
  canvas_up->SetPad(0., 1 - up_height, 0.97, 1.);
  canvas_dw->SetPad(0., dw_height_offset, 0.97, dw_height+dw_height_offset);
  canvas_up->SetFrameFillColor(0);
  canvas_up->SetFillColor(0);
  canvas_dw->SetFillColor(0);
  canvas_dw->SetFrameFillColor(0);
  canvas_dw->SetBottomMargin(0.25);
  
  //
  // set top margin 0 for bottom figure
  canvas_dw->SetTopMargin(0);
  
  //
  // draw top figure
  canvas_up->cd();

  TH1D * GenHist, * EstHist, * EstHistD, * EstHist_Rectangle, * thist;
  TH1D * histTemplate;

  double HT_x_max=2500.;
  double HT_x_min=400.;
  double MHT_x_max=1000.;
  double NJet_x_max=15.;
  double NBtag_x_max=4.;
  //double search_x_max=73.;
  double search_x_max=161.-0.5;
  //double search_x_min=1.;
  double search_x_min=1.-0.5;
  if(histname.find("QCD")!=string::npos)search_x_max=224.;
  if(histname.find("hPredHTMHT")!=string::npos )search_x_max=7.;
  if(histname.find("hPredNJetBins")!=string::npos ){search_x_max=11.5;search_x_min=3.5;}
  if(histname.find("hPredNbBins")!=string::npos ){ search_x_max=3.5;search_x_min=-0.5;}
  
  if(histname=="searchH_b")sprintf(tempname2,"allEvents/delphi/searchH_b_");
  else if(histname=="QCD_Up")sprintf(tempname2,"allEvents/delphi/QCD_");
  else if(histname=="QCD_Low")sprintf(tempname2,"allEvents/low_Dphi/QCD_");
  else sprintf(tempname2,"%s",histname.c_str());
  EstHist=(TH1D*) EstFile->Get(tempname2)->Clone();
  EstHistD=(TH1D*) EstFile->Get(tempname2)->Clone();
  EstHist->SetName("h");
  EstHistD->SetName("h");
  EstHistD->SetName("h");

  TH1D * EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  TH1D * EstHistD_input = static_cast<TH1D*>(EstHistD->Clone("EstHistD_input"));
  shift_bin(EstHist_input,EstHist);
  shift_bin(EstHistD_input,EstHistD);
  
  // Adding extra poisson 0 error term
  for (int ibin=0; ibin<EstHist->GetNbinsX(); ibin++){
    EstHist->SetBinError( ibin+1,pow(pow(EstHist->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
    EstHistD->SetBinError(ibin+1,pow(pow(EstHistD->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
    //cout << "bin content: " << EstHist->GetBinContent(ibin+1) << endl;
    //cout << "bin error: " << EstHist->GetBinError(ibin+1) << endl;
  }

  EstHist_Rectangle=(TH1D*) EstHist->Clone("EstHist_Rectangle");  // This is for rectangle drawing, but the central value is not correct,
                                               // so use it with great care
  for (int ibin=0; ibin<EstHist_Rectangle->GetNbinsX(); ibin++){
    if (EstHist->GetBinContent(ibin+1)<EstHist->GetBinError(ibin+1)){
      EstHist_Rectangle->SetBinContent(ibin+1,(EstHist->GetBinContent(ibin+1)+EstHist->GetBinError(ibin+1))/2.);
      EstHist_Rectangle->SetBinError(ibin+1,(EstHist->GetBinContent(ibin+1)+EstHist->GetBinError(ibin+1))/2.);
    }
  }
  //EstHist->Print("all");
  //EstHistD->Print("all");
  //EstHist_Rectangle->Print("all");

  sprintf(tempname,"%s",histname.c_str());
  if(sample.find("stacked")!=string::npos){
    tempstack=(THStack*)GenFile->Get(tempname)->Clone(); 
    GenHist=(TH1D*) tempstack->GetStack()->Last();
    tempstack=(THStack*)GenFile->Get(tempname)->Clone();
    GenHistD=(TH1D*) tempstack->GetStack()->Last();
    
  }
  else{ 
    GenHist=(TH1D*) GenFile->Get(tempname)->Clone();
    GenHistD=(TH1D*) GenFile->Get(tempname)->Clone();
  }

  TH1D * GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  TH1D * GenHistD_input = static_cast<TH1D*>(GenHistD->Clone("GenHistD_input"));
  shift_bin(GenHist_input,GenHist);
  shift_bin(GenHistD_input,GenHistD);
  
  // Scale MC to data luminosity or area normaliztion (if normalize is non-zero)
  double trigEff=1.0; 
  double scale = EstHist->GetSumOfWeights()/trigEff/GenHist->GetSumOfWeights(); 
  if (normalize){
    GenHist->Scale(scale);
    GenHistD->Scale(scale);
  } else {
    GenHist->Scale(lumiTarget/3.);
    GenHistD->Scale(lumiTarget/3.);
  }
   
  // Correcting for trigger efficiency
  if (trigEff!=1.) EstHist->Scale(1/trigEff*lumiTarget/lumiControl);  
  EstHist->Print("all");

  GenHist->SetLineColor(2);
  EstHist->SetLineColor(4);
  //GenHist->GetXaxis()->SetLabelFont(42);
  //GenHist->GetXaxis()->SetLabelOffset(0.007);
  //GenHist->GetXaxis()->SetLabelSize(0.04);
  GenHist->GetXaxis()->SetTitleSize(0.05);
  GenHist->GetXaxis()->SetTitleOffset(0.9);
  GenHist->GetXaxis()->SetTitleFont(42);
  //GenHist->GetYaxis()->SetLabelFont(42);
  //GenHist->GetYaxis()->SetLabelOffset(0.007);
  //GenHist->GetYaxis()->SetLabelSize(0.04);
  GenHist->GetYaxis()->SetTitleSize(0.05);
  GenHist->GetYaxis()->SetTitleOffset(1.25);
  GenHist->GetYaxis()->SetTitleFont(42);

  //EstHist->GetXaxis()->SetLabelFont(42);
  //EstHist->GetXaxis()->SetLabelOffset(0.007);
  //EstHist->GetXaxis()->SetLabelSize(0.04);
  EstHist->GetXaxis()->SetTitleSize(0.05);
  EstHist->GetXaxis()->SetTitleOffset(0.9);
  EstHist->GetXaxis()->SetTitleFont(42);
  //EstHist->GetYaxis()->SetLabelFont(42);
  //EstHist->GetYaxis()->SetLabelOffset(0.007);
  //EstHist->GetYaxis()->SetLabelSize(0.04);
  EstHist->GetYaxis()->SetTitleSize(0.05);
  EstHist->GetYaxis()->SetTitleOffset(1.25);
  EstHist->GetYaxis()->SetTitleFont(42);

  sprintf(xtitlename,"Search bins");
  if(histname.find("hPredHTMHT")!=string::npos ) sprintf(xtitlename,"HTMHT bins");
  if(histname.find("hPredNJetBins")!=string::npos ) sprintf(xtitlename,"N_{jet}");
  if(histname.find("hPredNbBins")!=string::npos ) sprintf(xtitlename,"N_{b}");

  sprintf(ytitlename,"Events");
  gPad->SetLogy();
  GenHist->SetMaximum(ymax_top);
  GenHist->SetMinimum(ymin_top);
  GenHist->GetXaxis()->SetRangeUser(search_x_min,search_x_max);
  
  //gPad->SetGridx(1);
  GenHist->SetTitle("");
  GenHist->SetMarkerStyle(20);
  GenHist->SetLineColor(1);
  GenHist->GetXaxis()->SetTitle(xtitlename);
  GenHist->GetYaxis()->SetTitle(ytitlename);
  TH1D * GenHist_Normalize = static_cast<TH1D*>(GenHist->Clone("GenHist_Normalize"));
  //KH  GenHist_Normalize->Scale(lumi/lumi_ttbar);
  GenHist_Normalize->SetMaximum(ymax_top);
  GenHist_Normalize->SetMinimum(ymin_top);
  //GenHist->Draw("e");
  //KH GenHist->Draw("same");
  GenHist_Normalize->DrawCopy("e");

  //EstHist->SetFillStyle(3004);
  EstHist->SetFillStyle(3144);
  EstHist->SetFillColor(kRed-10);
  EstHist->SetMarkerStyle(20);
  EstHist->SetMarkerSize(0.0001);
  //EstHist->Draw("e2same");
  //EstHist->Draw("esame");
  TH1D * EstHist_Normalize = static_cast<TH1D*>(EstHist->Clone("EstHist_Normalize"));
  //EstHist_Normalize->Scale(lumi/lumi_ttbar);
  //EstHist_Normalize->DrawCopy("e2 same");
  EstHist_Rectangle->SetFillStyle(3144);
  EstHist_Rectangle->SetFillColor(kRed-10);
  EstHist_Rectangle->DrawCopy("e2 same");
  //EstHist_Normalize->DrawCopy("e0 same");
  EstHist_Normalize->DrawCopy("esame");

  /*
  TH1D *EstHist_Normalize_Clone = (TH1D*)EstHist_Normalize->Clone(); 
  for(int i=1; i<EstHist_Normalize_Clone->GetNbinsX(); i++) {
    EstHist_Normalize_Clone->SetBinError(i,0);
  } 
  EstHist_Normalize_Clone->SetFillColor(kWhite);
  EstHist_Normalize_Clone->Draw("esame"); 
  */

  GenHist->Print("all");
  EstHist->Print("all");
  
  //
  // Re-draw to have "expectation" on top of "prediction"
  GenHist_Normalize->DrawCopy("esame");
  //
  // Legend & texts
  sprintf(tempname,"#tau_{hadronic} BG expectation (MC truth)");
  catLeg1->AddEntry(GenHist,tempname,"p");
  sprintf(tempname,"prediction from data");
  catLeg1->AddEntry(EstHist,tempname);
  catLeg1->Draw();

  gPad->RedrawAxis();

  //
  
  char lumilabel[200];
  sprintf(lumilabel,"%8.1f fb^{-1}",lumiTarget);

  if(histname.find("searchH_b")!=string::npos ){
	    
    //-----------------------------------------------------------
    // Putting lines and labels explaining search region definitions
    //-----------------------------------------------------------

    // Njet separation lines
    TLine *tl_njet = new TLine();
    tl_njet->SetLineStyle(2);

    tl_njet->DrawLine(41.-0.5,ymin_top,41.-0.5,ymax_top);
    tl_njet->DrawLine(81.-0.5,ymin_top,81.-0.5,ymax_top);
    tl_njet->DrawLine(121.-0.5,ymin_top,121.-0.5,ymax_top);
 

    // Njet labels
    TLatex * ttext_njet = new TLatex();
    ttext_njet->SetTextFont(42);
    ttext_njet->SetTextSize(0.04);
    ttext_njet->SetTextAlign(22);

    ttext_njet->DrawLatex(21.-0.5 , ymax_top/4. , "3 #leq N_{#scale[0.2]{ }jet} #leq 4");
    ttext_njet->DrawLatex(61.-0.5 , ymax_top/4. , "5 #leq N_{#scale[0.2]{ }jet} #leq 6");
    ttext_njet->DrawLatex(101.-0.5 , ymax_top/4. , "7 #leq N_{#scale[0.2]{ }jet} #leq 8");
    ttext_njet->DrawLatex(141.-0.5 , ymax_top/4. , "N_{#scale[0.2]{ }jet} #geq 9");


    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);

    tl_nb->DrawLine(11.-0.5,ymin_top,11.-0.5,ymax2_top);
    tl_nb->DrawLine(21.-0.5,ymin_top,21.-0.5,ymax2_top);
    tl_nb->DrawLine(31.-0.5,ymin_top,31.-0.5,ymax2_top);
    tl_nb->DrawLine(51.-0.5,ymin_top,51.-0.5,ymax2_top);
    tl_nb->DrawLine(61.-0.5,ymin_top,61.-0.5,ymax2_top);
    tl_nb->DrawLine(71.-0.5,ymin_top,71.-0.5,ymax2_top);
    tl_nb->DrawLine(91.-0.5,ymin_top,91.-0.5,ymax2_top);
    tl_nb->DrawLine(101.-0.5,ymin_top,101.-0.5,ymax2_top);
    tl_nb->DrawLine(111.-0.5,ymin_top,111.-0.5,ymax2_top);
    tl_nb->DrawLine(131.-0.5,ymin_top,131.-0.5,ymax2_top);
    tl_nb->DrawLine(141.-0.5,ymin_top,141.-0.5,ymax2_top);
    tl_nb->DrawLine(151.-0.5,ymin_top,151.-0.5,ymax2_top);
 
    
    // Nb labels
    TLatex * ttext_nb = new TLatex();
    ttext_nb->SetTextFont(42);
    ttext_nb->SetTextSize(0.04);
    ttext_nb->SetTextAlign(22);

    ttext_nb->DrawLatex( 6.-0.5 , ymax_top/12. , "N_{#scale[0.2]{ }b-jet}");

    ttext_nb->DrawLatex( 6.-0.5 , ymax_top/40. , "0");
    ttext_nb->DrawLatex(16.-0.5 , ymax_top/40. , "1");
    ttext_nb->DrawLatex(26.-0.5 , ymax_top/40. , "2");
    ttext_nb->DrawLatex(36.-0.5 , ymax_top/40. , "#geq 3");

    ttext_nb->DrawLatex(46.-0.5 , ymax_top/40. , "0");
    ttext_nb->DrawLatex(56.-0.5 , ymax_top/40. , "1");
    ttext_nb->DrawLatex(66.-0.5 , ymax_top/40. , "2");
    ttext_nb->DrawLatex(76.-0.5 , ymax_top/40. , "#geq 3");

    ttext_nb->DrawLatex(86.-0.5 , ymax_top/40. , "0");
    ttext_nb->DrawLatex(96.-0.5 , ymax_top/40. , "1");
    ttext_nb->DrawLatex(106.-0.5 , ymax_top/40. , "2");
    ttext_nb->DrawLatex(116.-0.5 , ymax_top/40. , "#geq 3");

    ttext_nb->DrawLatex(126.-0.5 , ymax_top/40. , "0");
    ttext_nb->DrawLatex(136.-0.5 , ymax_top/40. , "1");
    ttext_nb->DrawLatex(146.-0.5 , ymax_top/40. , "2");
    ttext_nb->DrawLatex(156.-0.5 , ymax_top/40. , "#geq 3");

    //sprintf(tempname,"CMS Preliminary,    %s,    #sqrt{s} = 13 TeV",  lumilabel);
    sprintf(tempname,"CMS Preliminary                                                                                    %s (13 TeV)",  lumilabel);
    TLatex * ttext = new TLatex(search_x_min, ymax_top*1.15,tempname);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(11);
    ttext->Draw();
    /*   
    TText * ttext = new TLatex(105. , ymax_top/80. , lumilabel);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(22);
    ttext->Draw();
*/
    //
  } else if(histname.find("QCD")!=string::npos ){
    
    //-----------------------------------------------------------
    // Putting lines and labels explaining search region definitions
    //-----------------------------------------------------------

    // Njet separation lines
    TLine *tl_njet = new TLine();
    tl_njet->SetLineStyle(2);
    tl_njet->DrawLine( 45.,ymin_top, 45.,ymax_top); 
    tl_njet->DrawLine( 89.,ymin_top, 89.,ymax_top); 
    tl_njet->DrawLine(133.,ymin_top,133.,ymax_top); 
    tl_njet->DrawLine(177.,ymin_top,177.,ymax_top/100.); 

    // Njet labels
    TLatex * ttext_njet = new TLatex();
    ttext_njet->SetTextFont(42);
    ttext_njet->SetTextSize(0.04);
    ttext_njet->SetTextAlign(22);
    ttext_njet->DrawLatex(23. , ymax_top/4. , "N_{jets} = 4");
    ttext_njet->DrawLatex(67. , ymax_top/4. , "N_{jets} = 5");
    ttext_njet->DrawLatex(111., ymax_top/4. , "N_{jets} = 6");
    ttext_njet->DrawLatex(155., ymax_top/400. , "7 #leq N_{jets} #leq 8");
    ttext_njet->DrawLatex(199., ymax_top/400. , "N_{jets} #geq 9");

    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);
    tl_nb->DrawLine(12.,ymin_top,12.,ymax2_top); 
    tl_nb->DrawLine(23.,ymin_top,23.,ymax2_top); 
    tl_nb->DrawLine(34.,ymin_top,34.,ymax2_top); 

    tl_nb->DrawLine(56.,ymin_top,56.,ymax2_top); 
    tl_nb->DrawLine(67.,ymin_top,67.,ymax2_top); 
    tl_nb->DrawLine(78.,ymin_top,78.,ymax2_top); 

    tl_nb->DrawLine(100.,ymin_top,100.,ymax2_top); 
    tl_nb->DrawLine(111.,ymin_top,111.,ymax2_top); 
    tl_nb->DrawLine(122.,ymin_top,122.,ymax2_top); 

    tl_nb->DrawLine(144.,ymin_top,144.,ymax3_top); 
    tl_nb->DrawLine(155.,ymin_top,155.,ymax3_top); 
    tl_nb->DrawLine(166.,ymin_top,166.,ymax3_top); 

    tl_nb->DrawLine(188.,ymin_top,188.,ymax4_top); 
    tl_nb->DrawLine(199.,ymin_top,199.,ymax4_top); 
    tl_nb->DrawLine(210.,ymin_top,210.,ymax4_top); 

    // Nb labels
    TLatex * ttext_nb = new TLatex();
    ttext_nb->SetTextFont(42);
    ttext_nb->SetTextSize(0.04);
    ttext_nb->SetTextAlign(22);
    ttext_nb->SetTextAngle(90);

    ttext_nb->DrawLatex( 6. , ymax_top/50. , "N_{b} = 0");
    ttext_nb->DrawLatex(17. , ymax_top/50. , "N_{b} = 1");
    ttext_nb->DrawLatex(28. , ymax_top/50. , "N_{b} = 2");
    ttext_nb->DrawLatex(39. , ymax_top/50. , "N_{b} #geq 3");
    
    TText * ttext = new TLatex(160. , ymax_top/50. , lumilabel);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(22);
    ttext->Draw();

  } else if(histname.find("hPredHTMHT")!=string::npos ){
	  cout << "hello\n";
	TText * ttext = new TLatex(5.3 , 620.6 , lumilabel);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(22);
    ttext->Draw();  
	  
  }	
  else if(histname.find("hPredNJetBins")!=string::npos ){
	  cout << "hello\n";
	TText * ttext = new TLatex(9.3 , 620.6 , lumilabel);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(22);
    ttext->Draw();  
	  
  }	
  else if(histname.find("hPredNbBins")!=string::npos ){
	  cout << "hello\n";

	TText * ttext = new TLatex(2.4 , 620.6 , lumilabel);
    ttext->SetTextFont(42);
    ttext->SetTextSize(0.045);
    ttext->SetTextAlign(22);
    ttext->Draw();  
	  
  }	
  

  //
  // Bottom ratio plot
  //
  // ----------


  //
  // Preparing ratio histograms
  TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));
  TH1D * denominator = static_cast<TH1D*>(EstHist->Clone("denominator"));
  
  TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
  TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
  TH1D * EstHist_NoError = static_cast<TH1D*>(EstHist->Clone("EstHist_NoError"));
  for (int ibin=0; ibin<EstHist_NoError->GetNbinsX()+2; ibin++){ // scan including underflow and overflow bins
    EstHist_NoError->SetBinError(ibin,0.);
  }
  
  //EstHistD->Add(GenHistD,-1);
  numerator->Divide(GenHist_Clone,EstHist_NoError,1,1,"");
  denominator->Divide(EstHist_Clone,EstHist_NoError,1,1,"");
  
  for (int ibin=0;ibin<GenHist_Clone->GetNbinsX();ibin++){
    if (GenHist_Clone->GetBinContent(ibin+1)<ymin_top && EstHist_Clone->GetBinContent(ibin+1)<ymin_top){
      numerator->SetBinContent(ibin+1,-1.);
      denominator->SetBinContent(ibin+1,-1.);
    } 
    else if (GenHist_Clone->GetBinContent(ibin+1)>ymin_top && EstHist_Clone->GetBinContent(ibin+1)==0.){
      denominator->SetBinContent(ibin+1,1.);
      denominator->SetBinError(ibin+1,100.);
    }
  }
  /*
    for (int ibin=0;ibin<EstHist_Clone->GetNbinsX();ibin++){
    if (EstHist_Clone->GetBinContent(ibin+1)<ymin_top) denominator->SetBinContent(ibin+1,-1.);
    }
  */

  // draw bottom figure
  canvas_dw->cd();
  // font size
  numerator->GetXaxis()->SetLabelSize(font_size_dw);
  numerator->GetXaxis()->SetTitleSize(font_size_dw);
  numerator->GetYaxis()->SetLabelSize(font_size_dw);
  numerator->GetYaxis()->SetTitleSize(font_size_dw);

  //
  // Horizontal Lines
  TLine *tline = new TLine(search_x_min,1.,search_x_max,1.);

  //
  // Common to all bottom plots
  //
  //sprintf(ytitlename,"#frac{Estimate - #tau_{had} BG}{#tau_{had} BG} ");
  sprintf(ytitlename,"#frac{Expectation}{Prediction} ");
  numerator->SetMaximum(ymax_bottom);
  numerator->SetMinimum(ymin_bottom);
  
  //
  // Specific to each bottom plot
  //
  // Setting style
  //numerator->SetMaximum(1.4);
  //numerator->GetXaxis()->SetLabelFont(42);
  //numerator->GetXaxis()->SetLabelOffset(0.007);
  //numerator->GetXaxis()->SetLabelSize(0.04);
  numerator->GetXaxis()->SetTitleSize(0.12);
  numerator->GetXaxis()->SetTitleOffset(0.9);
  numerator->GetXaxis()->SetTitleFont(42);
  //numerator->GetYaxis()->SetLabelFont(42);
  //numerator->GetYaxis()->SetLabelOffset(0.007);
  //numerator->GetYaxis()->SetLabelSize(0.04);
  numerator->GetYaxis()->SetTitleSize(0.13);
  numerator->GetYaxis()->SetTitleOffset(0.5);
  numerator->GetYaxis()->SetTitleFont(42);

  numerator->GetXaxis()->SetTitle(xtitlename);
  numerator->GetYaxis()->SetTitle(ytitlename);

  //gPad->SetGridx(1);

  //
  // Plotting
  numerator->SetTitle("");
  numerator->DrawCopy();

  denominator->DrawCopy("e2same");
  denominator->DrawCopy("same");

  numerator->DrawCopy("same");

  //numerator->Print("all");
  //denominator->Print("all");
    
  //
  // Drawing lines
  tline->SetLineStyle(2);
  tline->Draw();

  //
  if(histname.find("searchH_b")!=string::npos ){
	
    // Njet separation lines
    TLine *tl_njet = new TLine();
    tl_njet->SetLineStyle(2);
    
    tl_njet->DrawLine(41.-0.5,ymin_top,41.-0.5,ymax_top);
    tl_njet->DrawLine(81.-0.5,ymin_top,81.-0.5,ymax_top);
    tl_njet->DrawLine(121.-0.5,ymin_top,121.-0.5,ymax_top);
    tl_njet->DrawLine(161.-0.5,ymin_top,161.-0.5,ymax_top);
 

    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);
    tl_nb->DrawLine(11.-0.5,ymin_top,11.-0.5,ymax2_top);
    tl_nb->DrawLine(21.-0.5,ymin_top,21.-0.5,ymax2_top);
    tl_nb->DrawLine(31.-0.5,ymin_top,31.-0.5,ymax2_top);
    tl_nb->DrawLine(51.-0.5,ymin_top,51.-0.5,ymax2_top);
    tl_nb->DrawLine(61.-0.5,ymin_top,61.-0.5,ymax2_top);
    tl_nb->DrawLine(71.-0.5,ymin_top,71.-0.5,ymax2_top);
    tl_nb->DrawLine(91.-0.5,ymin_top,91.-0.5,ymax2_top);
    tl_nb->DrawLine(101.-0.5,ymin_top,101.-0.5,ymax2_top);
    tl_nb->DrawLine(111.-0.5,ymin_top,111.-0.5,ymax2_top);
    tl_nb->DrawLine(131.-0.5,ymin_top,131.-0.5,ymax2_top);
    tl_nb->DrawLine(141.-0.5,ymin_top,141.-0.5,ymax2_top);
    tl_nb->DrawLine(151.-0.5,ymin_top,151.-0.5,ymax2_top); 

  } else if(histname.find("QCD")!=string::npos ) {
	
    // Njet separation lines
    TLine *tl_njet = new TLine();
    tl_njet->SetLineStyle(2);
    tl_njet->DrawLine( 45.,ymin_bottom, 45.,ymax_bottom); 
    tl_njet->DrawLine( 89.,ymin_bottom, 89.,ymax_bottom); 
    tl_njet->DrawLine(133.,ymin_bottom,133.,ymax_bottom); 
    tl_njet->DrawLine(177.,ymin_bottom,177.,ymax_bottom); 

    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);
    tl_nb->DrawLine(12.,ymin_bottom,12.,ymax2_bottom); 
    tl_nb->DrawLine(23.,ymin_bottom,23.,ymax2_bottom); 
    tl_nb->DrawLine(34.,ymin_bottom,34.,ymax2_bottom); 
    
    tl_nb->DrawLine(56.,ymin_bottom,56.,ymax2_bottom); 
    tl_nb->DrawLine(67.,ymin_bottom,67.,ymax2_bottom); 
    tl_nb->DrawLine(78.,ymin_bottom,78.,ymax2_bottom); 
      
    tl_nb->DrawLine(100.,ymin_bottom,100.,ymax2_bottom); 
    tl_nb->DrawLine(111.,ymin_bottom,111.,ymax2_bottom); 
    tl_nb->DrawLine(122.,ymin_bottom,122.,ymax2_bottom); 

    tl_nb->DrawLine(144.,ymin_bottom,144.,ymax2_bottom); 
    tl_nb->DrawLine(155.,ymin_bottom,155.,ymax2_bottom); 
    tl_nb->DrawLine(166.,ymin_bottom,166.,ymax2_bottom); 
      
    tl_nb->DrawLine(188.,ymin_bottom,188.,ymax2_bottom); 
    tl_nb->DrawLine(199.,ymin_bottom,199.,ymax2_bottom); 
    tl_nb->DrawLine(210.,ymin_bottom,210.,ymax2_bottom); 
    
  }

  gPad->RedrawAxis();

  sprintf(tempname,"dataPrediction_vs_MCexp_%s_%s_Full_Plot.png",histname.c_str(),sample.c_str());
  canvas->Print(tempname);
  sprintf(tempname,"dataPrediction_vs_MCexp_%s_%s_Full_Plot.pdf",histname.c_str(),sample.c_str(),);
  canvas->Print(tempname);


}
