#include "Selection.h"
#include <map>
#include <cstdio>
#include <string>

/*
root.exe -b -q 'EventYield_TauHad.C("TTbar_")'
root.exe -b -q 'EventYield_TauHad.C("WJet_600_inf_")'

root.exe -b -q 'EventYield_TauHad.C("stacked")'
root.exe -b -q 'EventYield_TauHad.C("TTbar_stacked")'
root.exe -b -q 'EventYield_TauHad.C("WJet_stacked")'
root.exe -b -q 'EventYield_TauHad.C("T_stacked")'
*/

EventYield_TauHad(string sample="TTbar_", bool mc=false){
  char tempname[200];
  TH1D * tempHist;
  THStack * tempstack;
  TH1D * expHist;
  TH1D * preHist;

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad/GenInfo_HadTauEstimation_%s.root",sample.c_str());
  else sprintf(tempname,"TauHad/Stack/ARElog47_GenInfo_HadTauEstimation_%s.root",sample.c_str()); 
  TFile * exp_f = new TFile(tempname,"R");
  std::cout << tempname << std::endl;
  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/HadTauEstimation_%s.root",sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/ARElog47_HadTauEstimation_%s.root",sample.c_str());
  TFile * pre_f = new TFile(tempname,"R");
  std::cout << tempname << std::endl;

  //
  // Up to preselection
  sprintf(tempname,"cutflow_preselection");

  if(sample.find("stack")!=string::npos){
    if (mc) {
    tempstack=(THStack*) exp_f->Get(tempname)->Clone();
    expHist= (TH1D*) tempstack->GetStack()->Last();
    }
    //
    tempstack=(THStack*) pre_f->Get(tempname)->Clone();
    preHist= (TH1D*) tempstack->GetStack()->Last();
  }
  else{
    if (mc)
    expHist=(TH1D*) exp_f->Get(tempname)->Clone("cutflow_preselection_exp");
    //
    preHist=(TH1D*) pre_f->Get(tempname)->Clone("cutflow_preselection_pre");
  }

  if (mc) {
  printf("\nExpectation\n");
  for (int ibin=1; ibin<=expHist->GetNbinsX(); ibin++){
    if (expHist->GetBinContent(ibin)){
      printf("%-20s: %12.1f\n",expHist->GetXaxis()->GetBinLabel(ibin),expHist->GetBinContent(ibin));
    }
  }
  }
  printf("\nPrediction\n");
  for (int ibin=1; ibin<=preHist->GetNbinsX(); ibin++){
    if (preHist->GetBinContent(ibin)){
      printf("%-20s: %12.1f\n",preHist->GetXaxis()->GetBinLabel(ibin),preHist->GetBinContent(ibin));
    }
  }

  //
  // Beyond Preselection
  //
  map<int,string> cutname;
/*
    cutname[0]="PreSel";cutname[1]="nolep";cutname[2]="Njet_4";cutname[3]="ht_500";
    cutname[4]="mht_200";cutname[5]="isoMu";cutname[6]="isoElec";cutname[7]="isoPion";
    cutname[8]="delphi";
*/
  //cutname[0]="mht_200";cutname[1]="isoPion";cutname[2]="delphi";cutname[3]="delphi_Only2Jet";cutname[4]="isotrk_Only2Jet";cutname[5]="mht_Only2Jet";
  cutname[0]="mht_200";cutname[1]="isoPion";cutname[2]="delphi";
    //cutname[3]="low_Dphi";cutname[4]="mht_500";cutname[5]="J46_HT5001200_MHT500750";


  double exp=0,pre=0;

  //cout<< " PreSel(corresponding with \"if(MuFromTauVec[0]==0){\" in elog 183 )
  //for prediction and with \"if( genTauPt > LeptonAcceptance::muonPtMin() && std::abs(genTauEta) < LeptonAcceptance::muonEtaMax() ){\" for expectation )\n";

  printf("\n");
  for(int i=0; i<cutname.size();i++){
    sprintf(tempname,"allEvents/%s/MHT_%s_allEvents",cutname[i].c_str(),cutname[i].c_str());

    if(sample.find("stack")!=string::npos){
      if (mc) {
      tempstack=(THStack*) exp_f->Get(tempname)->Clone();
      tempHist= (TH1D*) tempstack->GetStack()->Last();
      //exp=tempHist->GetEntries();
      exp=tempHist->GetSumOfWeights();
      }
      tempstack=(THStack*) pre_f->Get(tempname)->Clone();
      tempHist= (TH1D*) tempstack->GetStack()->Last();
      //pre=tempHist->GetEntries();
      pre=tempHist->GetSumOfWeights();
    }
    else{
      if (mc) {
      tempHist=(TH1D*) exp_f->Get(tempname)->Clone();
      //exp=tempHist->GetEntries();
      exp=tempHist->GetSumOfWeights();
      }
      tempHist=(TH1D*) pre_f->Get(tempname)->Clone();
      //pre=tempHist->GetEntries();
      pre=tempHist->GetSumOfWeights();
     
    }

    //printf("cutname: %s ==>> prediction: %g *0.64 = %g expectation: %g Pre*0.64/Exp: %g \n \n ",cutname[i].c_str(),pre,pre*0.64,exp,(pre*0.64/exp));
  
    if (mc)
    printf("cutname: %-12s ==>> prediction: %12.3f expectation: %10.3f Pre/Exp: %6.3f\n",cutname[i].c_str(),pre,exp,(pre/exp));
    else 
    printf("cutname: %-12s ==>> prediction: %12.3f\n\n",cutname[i].c_str(),pre);


  }

}
