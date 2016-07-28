#include <TH1D.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TList.h>
#include <TStyle.h>
#include <vector>
#include <cstdio>

using namespace std;

/*

  Usage:

root.exe -b -q 'Plot_closure.C("delphi","NJet","stacked","Elog410_","Elog410_",true)'
root.exe -b -q 'Plot_closure.C("delphi","NBtag","stacked","Elog410_","Elog410_",true)'
root.exe -b -q 'Plot_closure.C("delphi","HT","stacked","Elog410_","Elog410_",true)'
root.exe -b -q 'Plot_closure.C("delphi","MHT","stacked","Elog410_","Elog410_",true)'

.x Plot_closure.C("delphi","NJet","stacked","Elog377_")
.x Plot_closure.C("delphi","NBtag","stacked","Elog377_")
.x Plot_closure.C("delphi","HT","stacked","Elog377_")
.x Plot_closure.C("delphi","MHT","stacked","Elog377_")
.x Plot_closure.C("delphi","MET","stacked","Elog377_")

root.exe -b -q 'Plot_closure.C("delphi","NJet","stacked","Elog401_","Elog381_",true)'
root.exe -b -q 'Plot_closure.C("delphi","NBtag","stacked","Elog401_","Elog381_",true)'
root.exe -b -q 'Plot_closure.C("delphi","HT","stacked","Elog401_","Elog381_",true)'
root.exe -b -q 'Plot_closure.C("delphi","MHT","stacked","Elog401_","Elog381_",true)'

root.exe -b -q 'Plot_closure.C("delphi","NJet","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("delphi","NBtag","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("delphi","HT","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("delphi","MHT","stacked","Elog377_",true)'

root.exe -b -q 'Plot_closure.C("mht_200","NJet","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("isoPion","NJet","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("delphi","NJet","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("delphi_NoIso","NJet","stacked","",true)'
root.exe -b -q 'Plot_closure.C("met_500","NJet","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("J46_HT5001200_MHT500750","NJet","stacked","Elog377_",true)'

.x Plot_closure.C("delphi","DelPhiN","stacked","Elog365_")
.x Plot_closure.C("delphi","DelPhi1","stacked","Elog365_")
.x Plot_closure.C("delphi","DelPhi2","stacked","Elog365_")
.x Plot_closure.C("delphi","DelPhi3","stacked","Elog365_")

root.exe -b -q 'Plot_closure.C("mht_200","DelPhi1","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_200","DelPhi2","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_200","DelPhi3","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_200","DelPhi4","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_200","TauJet_MHT_delPhi","stacked","Elog377_",true,true)'

root.exe -b -q 'Plot_closure.C("mht_500","DelPhi1","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_500","DelPhi2","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_500","DelPhi3","stacked","Elog377_",true,true)'
root.exe -b -q 'Plot_closure.C("mht_500","DelPhi4","stacked","Elog377_",true,true)'

root.exe -b -q 'Plot_closure.C("J46_HT5001200_MHT500750","DelPhi1","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("J46_HT5001200_MHT500750","DelPhi2","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("J46_HT5001200_MHT500750","DelPhi3","stacked","Elog377_",true)'
root.exe -b -q 'Plot_closure.C("J46_HT5001200_MHT500750","DelPhi4","stacked","Elog377_",true)'

 */

void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];  
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

Plot_closure(string cutname="delphi", string histname="NBtag",string sample="stacked",
	     string elogForPre="Elog410_PlusRare_",string elogForExp="Elog410_PlusRare_",
	     bool zoom=true, bool debug=true){

  //
  // Luminosity information for scaling
  //double lumi     = 2.109271; // normaliza to this lumi (fb-1)
  //  double lumi     = 2.26198; // normaliza to this lumi (fb-1)
  double lumi     = 7.6;
  double lumi_ref = 7.6; // normaliza to 3 (fb-1)
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  //
  // Set basic style
  //
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gROOT->LoadMacro("CMS_lumi.C");

  writeExtraText = true;
  extraText   = "        Supplementary (Simulation)";  // default extra text is "Preliminary"
  lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  cmsTextSize  = 0.60;
  lumiTextSize = 0.52;

  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  int iPos=0;

  TString line = "";
  char tempname[200];
  sprintf(tempname,"%8.1f",lumi);
  line+=tempname;
  line+=" fb^{-1} (13 TeV)";
  lumi_sqrtS = line;

  int W = 600;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;

  //
  // Various vertical line coordinates
  float ymax_top = 100000.;
  float ymin_top = 0.04.;
  float ymax_bottom = 2.65;
  float ymin_bottom = 0.0;
  float ytext_top = 2000.;
  float x_legend = 10.;
  float y_legend = 4000.;
  float xtext_top;
  
  if (zoom){
    ymax_bottom = 1.7;
    ymin_bottom = 0.45;
  }

  //

  //double xsec_ttbar   = 806.1; // (pb) https://twiki.cern.ch/twiki/bin/viewauth/CMS/RA2b13TeV
  //int    nevent_ttbar = 25348009;
  //double lumi_ttbar   = 31.445; // (fb-1)

  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // More specific style set, opening input files etc
  //

  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots 
  char xtitlename[200];
  char ytitlename[200];
  
  vector<TFile *> filevec;
  
  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad/%sGenInfo_HadTauEstimation_%s.root",elogForExp.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad/Stack/%sGenInfo_HadTauEstimation_%s.root",elogForExp.c_str(),sample.c_str());
  std::cout<<"cp0 tempname " <<tempname<<std::endl;  
  filevec.push_back(TFile::Open(tempname,"R"));

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForPre.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForPre.c_str(),sample.c_str());
  // sprintf(tempname,"TauHad2/Storage/HadTauEstimation_TTbar_Feb_17_2015.root");
  std::cout<<"cp00 tempname " <<tempname<<std::endl;
  filevec.push_back(TFile::Open(tempname,"R"));

  //
  // Define legend
  //
  Float_t legendX1 = .40; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .62; //.65;
  Float_t legendY2 = .82;

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);

  catLeg1->SetTextSize(0.044);
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
  canvas->SetRightMargin( 0.2 );
  canvas->SetTopMargin( T/H );
  canvas->SetBottomMargin( B/H );
  //canvas->SetTickx(0);
  //canvas->SetTicky(0);


  canvas->Divide(1, 2);

  //
  // Define pads
  //
  TPad* canvas_up = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_1");
  TPad* canvas_dw = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_2");
 
  // define the size
  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.1;  // please tune the font size parameter for bottom figure
  double dw_height    = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.040; // KH, added to put the bottom one closer to the top panel
 
  // set pad size
  canvas_up->SetPad(0., 1 - up_height +0.01, 0.97, 1.);
  canvas_dw->SetPad(0., dw_height_offset, 0.97, dw_height+dw_height_offset);
  canvas_up->SetFrameFillColor(0);
  canvas_up->SetFillColor(0);
  canvas_up->SetTopMargin(0.10);
  canvas_up->SetRightMargin(0.03);
  canvas_dw->SetFillColor(0);
  canvas_dw->SetFrameFillColor(0);
  canvas_dw->SetBottomMargin(0.30);
  canvas_dw->SetRightMargin(0.03);
  
  // set top margin 0 for bottom figure
  canvas_dw->SetTopMargin(0);
  
  // draw top figure
  canvas_up->cd();

  THStack * tempstack;
  TH1D * GenHist, * EstHist,* thist;
  TH1D * histTemplate;

  double HT_x_max=2500.;
  //double HT_x_min=400.;
  double HT_x_min=300.;
  double MHT_x_max=1000.;
  double NJet_x_max=11.;
  double NBtag_x_max=3.5;
  double search_x_max=19.;
  double search_x_min=0.;
  double DelPhiN_x_max=20.;
  double Delphi1_x_max=3.2;
  //Double_t NJ_bins[11]={0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,20.}; 
  Double_t NJ_bins[12]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,11.5,19.5}; 
  Double_t HT_bins[13]={0.,300.,500.,700.,900.,1100.,1300.,1500.,1700.,1900.,2100.,2300.,2500.};
 
  TH1D * GenHist_Clone;
  
  for(int i=0; i<filevec.size(); i++){

    if(sample.find("stacked")!=string::npos){

      if(i==0){
        if(histname=="search"){
          sprintf(tempname,"searchH");
	  std::cout<<"cp1 tempname " <<tempname<<std::endl;
          GenHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }
        else{

          sprintf(tempname,"allEvents/%s/%s_%s_allEvents",cutname.c_str(),histname.c_str(),cutname.c_str());
	  std::cout<<"cp2 tempname " <<tempname<<std::endl;
          tempstack = (THStack*)filevec.at(i)->Get(tempname)->Clone();
          GenHist=(TH1D*) tempstack->GetStack()->Last();
        }
      }
      if(i==1){
        if(histname=="search"){
          sprintf(tempname,"searchH");
	  std::cout<<"cp3 tempname " <<tempname<<std::endl;    
	  GenHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }        
        else{
          sprintf(tempname,"allEvents/%s/%s_%s_allEvents",cutname.c_str(),histname.c_str(),cutname.c_str());
	  std::cout<<"cp4 tempname " <<tempname<<std::endl;         
	  tempstack = (THStack*)filevec.at(i)->Get(tempname)->Clone();
          EstHist=(TH1D*) tempstack->GetStack()->Last();
        }
      }
      tempstack = (THStack*)filevec.at(i)->Get(tempname)->Clone();
      thist=(TH1D*) tempstack->GetStack()->Last();
      thist->SetLineColor(2*i+2);
      thist->SetFillColor(0);
      std::cout << "tempstack print starts" << std::endl;
      tempstack->Print();
      std::cout << "tempstack ends" << std::endl;

    }
    else{
      
      if(i==0){
        if(histname=="search"){
          sprintf(tempname,"searchH");
          GenHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }
        else{
          sprintf(tempname,"allEvents/%s/%s_%s_allEvents",cutname.c_str(),histname.c_str(),cutname.c_str());
          GenHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }
      }
      if(i==1){
  //      if(histname=="NBtag")sprintf(tempname,"allEvents/%s/nB_new_%s_allEvents",cutname.c_str(),cutname.c_str());
  //      else sprintf(tempname,"allEvents/%s/%s_%s_allEvents",cutname.c_str(),histname.c_str(),cutname.c_str());
        if(histname=="search"){
          sprintf(tempname,"searchH");
          EstHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }
        else{
          sprintf(tempname,"allEvents/%s/%s_%s_allEvents",cutname.c_str(),histname.c_str(),cutname.c_str());
          EstHist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
        }
      }
      //    thist=(TH1D*) filevec.at(i)->Get(tempname)->Clone();
      thist=static_cast<TH1D*>(filevec.at(i)->Get(tempname)->Clone());
      thist->SetLineColor(2*i+2);
      
    }

    //KH
    if(histname=="NBtag" || histname=="NJet"){
      TH1D * GenHist_input;
      TH1D * EstHist_input;
      if (i==0){ 
	GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
	shift_bin(GenHist_input,GenHist);
	thist=GenHist;
	if(histname=="NJet")GenHist=(TH1D*)GenHist->Rebin(11,"GenHist",NJ_bins);
	if(histname=="NJet")thist=(TH1D*) thist->Rebin(11,"thist",NJ_bins);
      }
      if (i==1){
	EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
	shift_bin(EstHist_input,EstHist);
	thist=EstHist;
	if(histname=="NJet")EstHist=(TH1D*)EstHist->Rebin(11,"EstHist",NJ_bins);
	if(histname=="NJet")thist=(TH1D*) thist->Rebin(11,"thist",NJ_bins);
      }
    }
    if(histname=="HT"){
      TH1D * GenHist_input;
      TH1D * EstHist_input;
      if (i==0){ 
	//GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
	//shift_bin(GenHist_input,GenHist);
	thist=GenHist;
	if(histname=="HT")GenHist=(TH1D*)GenHist->Rebin(12,"GenHist",HT_bins);
	if(histname=="HT")thist=(TH1D*) thist->Rebin(12,"thist",HT_bins);
      }
      if (i==1){
	//EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
	//shift_bin(EstHist_input,EstHist);
	thist=EstHist;
	if(histname=="HT")EstHist=(TH1D*)EstHist->Rebin(12,"EstHist",HT_bins);
	if(histname=="HT")thist=(TH1D*) thist->Rebin(12,"thist",HT_bins);
      }
    }
    //KH

    thist->SetTitle("");
    //    thist->Scale(lumi/lumi_ttbar);
    thist->Scale(lumi/lumi_ref);

    //
    // Setting style
    //---------------
    double xlatex, ylatex;

    if(i==0){
      thist->SetMaximum(1.4);
      //thist->GetXaxis()->SetLabelFont(42);
      //thist->GetXaxis()->SetLabelOffset(0.007);
      //thist->GetXaxis()->SetLabelSize(0.04);
      thist->GetXaxis()->SetTitleSize(0.05);
      thist->GetXaxis()->SetTitleOffset(1.9);
      thist->GetXaxis()->SetLabelOffset(1.2);
      //thist->GetXaxis()->SetTitleOffset(1.2);
      thist->GetXaxis()->SetTitleFont(42);
      //thist->GetYaxis()->SetLabelFont(42);
      //thist->GetYaxis()->SetLabelOffset(0.007);
      //thist->GetYaxis()->SetLabelSize(0.04);
      thist->GetYaxis()->SetTitleSize(0.06);
      thist->GetYaxis()->SetTitleOffset(1.2);
      thist->GetYaxis()->SetTitleFont(42);
      thist->GetYaxis()->SetLabelSize(0.04);
    }
    if(histname=="search"){
      sprintf(xtitlename,"Search bin");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(2000);
      thist->SetMinimum(0.);
      thist->GetXaxis()->SetRangeUser(search_x_min,search_x_max);
    }
    
    if(histname=="HT"){
      xtext_top = 1800.;
      //y_legend  = 2000.;
      //xlatex=1896.257;ylatex=11.59619;
      xlatex=0.65;ylatex=0.5;
      ymax_top = 300000.;
      ymin_top = 0.2.;
      sprintf(xtitlename,"H_{T} [GeV]");
      sprintf(ytitlename,"Events / 200 GeV");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(HT_x_min,HT_x_max);
      gPad->SetLogy();
      //thist->GetXaxis()->SetLimits(0.,HT_x_max);
    }
    if(histname=="MHT"){
      xtext_top = 650.;
      //y_legend = 500.;
      ymax_top = 1000000.;
      //xlatex=726.1259;ylatex=12.97329;
      xlatex=0.65;ylatex=0.5;
      ymin_top = 0.05;
      sprintf(xtitlename,"{H}_{T}^{miss} [GeV]");
      sprintf(ytitlename,"Events / 50 GeV");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(100.,MHT_x_max);
      gPad->SetLogy();
      //thist->GetXaxis()->SetLimits(0.,MHT_x_max);
    }     
    if(histname=="NBtag"){
      xtext_top = 2.8;
      //y_legend = 3000.;
      //ymax_top = 1400.;
      ymax_top = 2000.;
      if(cutname=="Njet_9") ymax_top = 100.;
      ymin_top = 0.0;
      ytext_top = 0.65*ymax_top;
      xlatex=0.65;ylatex=0.5;
      sprintf(xtitlename,"N_{b-jet}");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(0.);
      thist->GetXaxis()->SetRangeUser(0.,NBtag_x_max);
      thist->GetXaxis()->SetNdivisions(505);
    }    
    if(histname=="NJet"){
      xtext_top = 11.;
      //y_legend = 2000.;
      ymax_top = 300000.;
      ymin_top = 2.;
      //ytext_top = ymax_top*0.005;
      //xlatex=14.88397;ylatex=0.962934;
      xlatex=0.65;ylatex=0.5;
      sprintf(xtitlename,"N_{jet}");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(3.,NJet_x_max);
      gPad->SetLogy();
    }
    if(histname=="MET"){
      sprintf(xtitlename,"MET (GeV)");
      sprintf(ytitlename,"Events / 100 GeV");
      thist->SetMaximum(50000);
      thist->GetXaxis()->SetRangeUser(0.,MHT_x_max);
      gPad->SetLogy();
      //thist->GetXaxis()->SetLimits(0.,MHT_x_max);
    }
    if(histname=="DelPhiN"){
      xtext_top = 14.;
      //y_legend = 1300.;
      ymax_top = 600.;
      ymin_top = 0.0;
      ytext_top = ymax_top*0.65;
      sprintf(xtitlename,"DelPhiN");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,DelPhiN_x_max);
    }   
    if(histname=="DelPhi1"){
      xtext_top = 1.2;
      //y_legend = 1300.;
      //ymax_top = 10000.;
      ymax_top = 10000.;
      //ymin_top = 1.;
      ymin_top = 0.1;  
      ytext_top = ymax_top*0.05;
      sprintf(xtitlename,"DelPhi1");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    }
    if(histname=="DelPhi2"){
      xtext_top = 2.2;
      //y_legend = 1300.;
      //ymax_top = 1000.;
      //ymin_top = 10.;
      ymax_top = 10000.;                                                                                                                                                
      //ymin_top = 1.;   
      ymin_top = 0.1;
      ytext_top = ymax_top*0.2;
      sprintf(xtitlename,"DelPhi2");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    }
    if(histname=="DelPhi3"){
      xtext_top = 2.2;
      //y_legend = 1300.;
      ymax_top = 1000.;
      ymin_top = 10.;
      ytext_top = ymax_top*0.2;
      sprintf(xtitlename,"DelPhi3");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    } 
    if(histname=="DelPhi4"){
      xtext_top = 2.2;
      //y_legend = 1300.;
      ymax_top = 1000.;
      ymin_top = 10.;
      ytext_top = ymax_top*0.2;
      sprintf(xtitlename,"DelPhi4");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    }
    if(histname=="TauJet_MHT_delPhi"){
      xtext_top = 2.2;
      //y_legend = 1300.;
      ymax_top = 1000.;
      ymin_top = 1.;
      ytext_top = ymax_top*0.2;
      sprintf(xtitlename,"DelPhi-TauJet");
      sprintf(ytitlename,"Events");
      thist->Rebin(5);
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    }

    if(histname=="TauJet_MHT_delPhi"){
      xtext_top = 2.2;
      //y_legend = 1300.;
      ymax_top = 100.;
      ymin_top = 0.01;
      ytext_top = ymax_top*0.2;
      sprintf(xtitlename,"#{Phi}(tau,mht)");
      sprintf(ytitlename,"Events");
      thist->SetMaximum(ymax_top);
      thist->SetMinimum(ymin_top);
      thist->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
      gPad->SetLogy();
    }

    //
    // Drawing plots
    //
    thist->GetXaxis()->SetTitle(xtitlename);
    thist->GetYaxis()->SetTitle(ytitlename);
    if(i==0){
      //thist->Draw();
      thist->SetMarkerStyle(20);
      thist->SetLineColor(1);
      thist->DrawCopy("e");      
      GenHist_Clone   = static_cast<TH1D*>(thist->Clone("GenHist_Clone"));
      if (debug) {
	std::cout << "thist print starts" << std::endl;
	thist->Print("all");
	std::cout << "thist print ends" << std::endl;
      }
    }
    else{
      thist->SetFillStyle(3144);
      thist->SetFillColor(kRed-10);
      thist->SetMarkerStyle(20);
      thist->SetMarkerSize(0.0001);
      thist->DrawCopy("e2same ");
      EstHist_Clone   = static_cast<TH1D*>(thist->Clone("EstHist_Clone"));

      // Troy's trick
      //thist->DrawCopy("esame ");
      TH1D *EstHist_Normalize_Clone = (TH1D*)thist->Clone("EstHist_Normalize_Clone");
      for(int ibin=1; ibin<72; ibin++) {
	EstHist_Normalize_Clone->SetBinError(ibin,0.00001);
      }
      EstHist_Normalize_Clone->SetFillColor(kWhite);
      EstHist_Normalize_Clone->Draw("esame");
      // Troy trick ends

      if (debug) {
	std::cout << "thist print starts" << std::endl;
	thist->Print("all");
	std::cout << "thist print ends" << std::endl;
      }
    }

    //
    // Set up canvas
    //
    if(i==0){
      //sprintf(tempname,"#tau_{hadronic} BG expectation (MC truth)");
      sprintf(tempname,"Hadronic #tau-lepton background");
      catLeg1->SetHeader(tempname);
      sprintf(tempname,"Direct from simulation");
      catLeg1->AddEntry(GenHist_Clone,tempname,"p");
    }
    else if(i==1){
      //sprintf(tempname,"Prediction from MC");
      std::cout << "tlegend" << std::endl;
      sprintf(tempname,"Treat simulation like data");
      catLeg1->AddEntry(EstHist_Clone,tempname);
    }
  }

  GenHist_Clone->DrawCopy("esame");
  catLeg1->Draw();

  sprintf(tempname,"Normalized to %8.1f fb^{-1}",lumi);
  //TText * ttext = new TLatex(xtext_top, ytext_top, "Normalized to 3 fb^{-1}");
  TText * ttext = new TLatex(xtext_top, ytext_top,tempname);
  ttext->SetTextFont(42);
  ttext->SetTextSize(0.045);
  ttext->SetTextAlign(22);
//  ttext->Draw();

  
//  TLatex * tex = new TLatex(xlatex,ylatex,"arXiv:1602.06581");
//  tex->SetTextColor(4);
//  tex->SetTextFont(61);
//  tex->SetTextSize(0.045);
//  tex->SetLineColor(4);
//  tex->SetLineWidth(2);
//  //tex->Draw();
//  tex->DrawLatexNDC(xlatex,ylatex,"arXiv:1602.06581");
  gPad->RedrawAxis();

  //
  // Bottom ratio plot
  //
  // ----------

  //if(choice==1){
    
      GenHist->SetMarkerStyle(20);
      GenHist->SetLineColor(1);

      EstHist->SetFillStyle(3144);
      EstHist->SetFillColor(kRed-10);
      EstHist->SetMarkerStyle(20);
      EstHist->SetMarkerSize(0.0001);

      TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));
      TH1D * denominator = static_cast<TH1D*>(EstHist->Clone("denominator"));

      TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
      TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
      TH1D * EstHist_NoError = static_cast<TH1D*>(EstHist->Clone("EstHist_NoError"));
      for (int ibin=0; ibin<EstHist_NoError->GetNbinsX()+2; ibin++){ // scan including underflow and overflow bins
	EstHist_NoError->SetBinError(ibin,0.);
      }


      if(histname=="TauJet_MHT_delPhi"){
	numerator->Rebin(5);
	denominator->Rebin(5);
	GenHist_Clone->Rebin(5);
	EstHist_Clone->Rebin(5);
	EstHist_NoError->Rebin(5);
      }

      numerator->Divide(GenHist_Clone,EstHist_NoError,1,1,"");
      denominator->Divide(EstHist_Clone,EstHist_NoError,1,1,"");

      // draw bottom figure
      canvas_dw->cd();
      // font size
      numerator->GetXaxis()->SetLabelSize(font_size_dw);
      numerator->GetXaxis()->SetTitleSize(font_size_dw);
      numerator->GetYaxis()->SetLabelSize(font_size_dw);
      numerator->GetYaxis()->SetTitleSize(font_size_dw);
      
      //
      // Specific to each bottom plot
      //
      if(histname=="HT"){
        sprintf(xtitlename,"H_{T} [GeV]");
        numerator->GetXaxis()->SetRangeUser(HT_x_min,HT_x_max);
        TLine *tline = new TLine(HT_x_min,1.,HT_x_max,1.);
      }
      if(histname=="MHT"){
        sprintf(xtitlename,"H_{T}^{miss} [GeV]");
        numerator->GetXaxis()->SetRangeUser(100.,MHT_x_max);
        TLine *tline = new TLine(100.,1.,MHT_x_max,1.);
      }   
      if(histname=="NBtag"){
        sprintf(xtitlename,"N_{b-jet}");
        numerator->GetXaxis()->SetRangeUser(0.,NBtag_x_max);
        TLine *tline = new TLine(0.,1.,NBtag_x_max,1.);
	//ymax_bottom=1.5;
	//ymin_bottom=0.7;
      }    
      if(histname=="NJet"){
        sprintf(xtitlename,"N_{jet}");
        numerator->GetXaxis()->SetRangeUser(3.,NJet_x_max);
        TLine *tline = new TLine(2.5,1.,NJet_x_max,1.);
	//ymax_bottom=1.5;
	//ymin_bottom=0.5;
      }
      if(histname=="MET"){
        sprintf(xtitlename,"MET (GeV)");
        numerator->GetXaxis()->SetRangeUser(0.,MHT_x_max);
        TLine *tline = new TLine(0.,1.,MHT_x_max,1.);
      }
      if(histname=="DelPhiN"){
        sprintf(xtitlename,"DelPhiN");
        numerator->GetXaxis()->SetRangeUser(0.,DelPhiN_x_max);
        TLine *tline = new TLine(0.,1.,DelPhiN_x_max,1.);
      }
      if(histname=="DelPhi1"){
        sprintf(xtitlename,"DelPhi1");
        numerator->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
        TLine *tline = new TLine(0.,1.,Delphi1_x_max,1.);
      } 
      if(histname=="DelPhi2"){
        sprintf(xtitlename,"DelPhi2");
        numerator->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
        TLine *tline = new TLine(0.,1.,Delphi1_x_max,1.);
      }
      if(histname=="DelPhi3"){
        sprintf(xtitlename,"DelPhi3");
        numerator->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
        TLine *tline = new TLine(0.,1.,Delphi1_x_max,1.);
      }
      if(histname=="DelPhi4"){
        sprintf(xtitlename,"DelPhi4");
        numerator->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
        TLine *tline = new TLine(0.,1.,Delphi1_x_max,1.);
      }
      if(histname=="TauJet_MHT_delPhi"){
        sprintf(xtitlename,"#Phi(tau,MHT)");
        numerator->GetXaxis()->SetRangeUser(0.,Delphi1_x_max);
        TLine *tline = new TLine(0.,1.,Delphi1_x_max,1.);
      }

      //
      // Common to all bottom plots
      //
      //      sprintf(ytitlename,"#frac{Estimate - #tau_{had} BG}{#tau_{had} BG} ");
      sprintf(ytitlename,"#frac{Direct}{Prediction} ");
      numerator->SetMaximum(ymax_bottom);
      numerator->SetMinimum(ymin_bottom);

      // Setting style
      //numerator->SetMaximum(1.4);
      //numerator->GetXaxis()->SetLabelFont(42);
      //numerator->GetXaxis()->SetLabelOffset(0.007);
      //numerator->GetXaxis()->SetLabelSize(0.04);
      numerator->GetXaxis()->SetTitleSize(0.16);
      numerator->GetXaxis()->SetTitleOffset(0.80);
      numerator->GetXaxis()->SetTitleFont(42);
      numerator->GetXaxis()->SetLabelSize(0.12);
      numerator->GetXaxis()->SetLabelOffset(0.007);

      numerator->GetYaxis()->SetTitleSize(0.16);
      numerator->GetYaxis()->SetTitleOffset(0.4);
      numerator->GetYaxis()->SetTitleFont(42);
      numerator->GetYaxis()->SetLabelSize(0.12);
      numerator->GetYaxis()->SetLabelOffset(0.007);

      numerator->GetXaxis()->SetTitle(xtitlename);
      numerator->GetYaxis()->SetTitle(ytitlename);

      numerator->GetXaxis()->SetTickSize(0.08);
      numerator->SetTitle("");
      if (zoom) numerator->GetYaxis()->SetNdivisions(510);
      numerator->Draw();
      if (debug) numerator->Print("all");

      denominator->DrawCopy("e2same");

      // Troy's trick
      //denominator->DrawCopy("same");
      TH1D *denominator_Clone = (TH1D*)denominator->Clone();
      denominator_Clone->SetFillColor(kWhite);
      for(int ibin=1; ibin<72; ibin++) {
	denominator_Clone->SetBinError(ibin,0.00001);
      }
      denominator_Clone->Draw("same");
      // Troy's trick ends

      numerator->DrawCopy("same");

      tline->SetLineStyle(2);
      tline->Draw();

      gPad->RedrawAxis();

  //}
  {
    CMS_lumi( canvas_up, iPeriod, iPos );   // writing the lumi information and the CMS "logo"
  }
  canvas->Update();
  canvas->RedrawAxis();

  sprintf(tempname,"Closure_%s_%s_%s_%s%sPlot.png",histname.c_str(),cutname.c_str(),sample.c_str(),elogForPre.c_str(),elogForExp.c_str());
  canvas->Print(tempname);
  sprintf(tempname,"Closure_%s_%s_%s_%s%sPlot.pdf",histname.c_str(),cutname.c_str(),sample.c_str(),elogForPre.c_str(),elogForExp.c_str());
  canvas->Print(tempname);
  canvas->Print("test.C");
}

