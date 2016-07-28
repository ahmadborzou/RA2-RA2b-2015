#include "Selection.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "THStack.h"
#include "TFile.h"
#include "TH1.h"
#include "TDirectory.h"

using namespace std;

class mainClass{

  // Some variables
  double tempvalue;
  char tempname[200];
  char tempname2[200];
  vector<double> T_scalevec, WJet_scalevec, TTbar_scalevec;
  vector<TFile *> T_inputfilevec,WJet_inputfilevec, TTbar_inputfilevec;
  map<int, string> cutname, histname, Hname;
  map<int, string> Ttype, WJettype, TTbartype;
  TFile *file, *file2, *file30, *file3;
  TH1D *temphist, *temphist2, * temphist30, *temphistI, *temphistII, *temphistIII, *temphistI_lowDphi, *temphistII_lowDphi, *temphistIII_lowDphi;
  TH1D *temphistInj34, *temphistIInj34, *temphistIIInj34, *temphistInj34_lowDphi, *temphistIInj34_lowDphi, *temphistIIInj34_lowDphi;
  TH1D *temphistInj56, *temphistIInj56, *temphistIIInj56, *temphistInj56_lowDphi, *temphistIInj56_lowDphi, *temphistIIInj56_lowDphi;
  TH1D *temphistInj78, *temphistIInj78, *temphistIIInj78, *temphistInj78_lowDphi, *temphistIInj78_lowDphi, *temphistIIInj78_lowDphi;
  TH1D *temphistInj9,  *temphistIInj9,  *temphistIIInj9,  *temphistInj9_lowDphi,  *temphistIInj9_lowDphi,  *temphistIIInj9_lowDphi;
  THStack * tempstack;
  TDirectory *cdtoitt, *cdtoit;

public:
mainClass(int luminosity=10000){ // luminosity is in /pb unit

  bool doScale = false;
  double scalefactor=3000.; // normalized to 3/fb
  //double scalefactor=225;
  Selection * sel = new Selection();
  cutname = sel->cutName();

  WJettype[0]="allEvents";
  TTbartype[0]="allEvents";
  Ttype[0]="allEvents";

// .....................................................................................................................................................//
// Single Top Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> T_xs_vec;
  //T_xs_vec.push_back(2.);     // T_s
  //T_xs_vec.push_back(103.4);  // T_t 
  //T_xs_vec.push_back(35.);    // T_tW
  //T_xs_vec.push_back(1.);     // Tbar_s
  //T_xs_vec.push_back(61.6);   // Tbar_t 
  //T_xs_vec.push_back(35.);    // Tbar_tW
  T_xs_vec.push_back(44.07); // t_top
  T_xs_vec.push_back(26.23);  // t_antitop
  T_xs_vec.push_back(35.8);  // tW_top
  T_xs_vec.push_back(35.8); // tW_antitop
  T_xs_vec.push_back(3.34);  // s
  //T_xs_vec.push_back(3.34);  // s

  const int tnHT = (int) T_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=tnHT ; i++){
    /*
    if(i==1)sprintf(tempname,"../../Results/results_T_s_.root");
    else if(i==2)sprintf(tempname,"../../Results/results_T_t_.root");
    else if(i==3)sprintf(tempname,"../../Results/results_T_u_.root");
    else if(i==4)sprintf(tempname,"../../Results/results_Tbar_s_.root");
    else if(i==5)sprintf(tempname,"../../Results/results_Tbar_t_.root");
    else if(i==6)sprintf(tempname,"../../Results/results_Tbar_u_.root");
    else{cout << " Error!! There are only 6 T ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*T_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    */
    if     (i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_t_top_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_tW_top_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../GenInfo_HadTauEstimation_s_channel_.root");
    //else if(i==6)sprintf(tempname,"../GenInfo_HadTauEstimation_Tbar_s_.root");
    else{cout << " Error!! There are only 6 single top sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"cutflow_preselection");
    std::cout << luminosity << " " << T_xs_vec[i-1] << (* (TH1D* ) file->Get(tempname)).GetBinContent(1) << std::endl;
    tempvalue = (luminosity*T_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetBinContent(1));
    T_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "T normalization scale factor determination done \n " << std::endl;

  //..........................................//
  // main histograms like HT, MHT, ...
  //..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=tnHT ; i++){
    if     (i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_t_top_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_tW_top_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../GenInfo_HadTauEstimation_s_channel_.root");
    //else if(i==6)sprintf(tempname,"../GenInfo_HadTauEstimation_Tbar_s_.root");
    else{cout << " Error!! There are only 6 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 


  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_T_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";
  histname[8]="HT2";
  histname[9]="MHT2";
  histname[10]="DelPhi4";
  histname[11]="TauJet_MHT_delPhi";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
  Hname[4]="cutflow_preselection";
  Hname[5]="searchH_lowDphi"; 
  Hname[6]="hPredHTMHT0b";
  Hname[7]="hPredHTMHTwb";
  Hname[8]="hPredNJetBins";
  Hname[9]="hPredNbBins";
 
  // Loop over Hname
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<tnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }

  // Loop over event types, cutnames, etc
  for(map<int , string >::iterator itt=Ttype.begin(); itt!=Ttype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<tnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJtype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
          if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
          else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);

        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("T main histograms stacked \n ");


  //..........................................//
  // IsoTrk  
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  T_inputfilevec.clear();

  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../IsoEfficiencies_t_top_.root");
    else if(i==2)sprintf(tempname,"../IsoEfficiencies_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../IsoEfficiencies_tW_top_.root");
    else if(i==4)sprintf(tempname,"../IsoEfficiencies_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../IsoEfficiencies_s_channel_.root");
    else{cout << " Error!! There are only 4 single top ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"IsoEfficiencies_T_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="IsoEff";
  histname[1]="Iso_pass";
  histname[2]="Iso_all";
  histname[3]="Iso_pass_lowDphi";
  histname[4]="Iso_all_lowDphi";

  histname[5]="Iso_pass_nb_njet34";
  histname[6]="Iso_all_nb_njet34";
  histname[7]="Iso_pass_nb_njet34_lowDphi";
  histname[8]="Iso_all_nb_njet34_lowDphi";

  histname[9]="Iso_pass_nb_njet56";
  histname[10]="Iso_all_nb_njet56";
  histname[11]="Iso_pass_nb_njet56_lowDphi";
  histname[12]="Iso_all_nb_njet56_lowDphi";

  histname[13]="Iso_pass_nb_njet78";
  histname[14]="Iso_all_nb_njet78";
  histname[15]="Iso_pass_nb_njet78_lowDphi";
  histname[16]="Iso_all_nb_njet78_lowDphi";

  histname[17]="Iso_pass_nb_njet9";
  histname[18]="Iso_all_nb_njet9";
  histname[19]="Iso_pass_nb_njet9_lowDphi";
  histname[20]="Iso_all_nb_njet9_lowDphi";

  std::cout<< " Before looping"<< std::endl;
  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins
      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);
      std::cout<< " looping over ht bins"<< std::endl;
    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    if(j==3)temphistI_lowDphi=(TH1D*)temphist->Clone();
    if(j==4)temphistII_lowDphi=(TH1D*)temphist->Clone();

    if(j==5)temphistInj34=(TH1D*)temphist->Clone();
    if(j==6)temphistIInj34=(TH1D*)temphist->Clone();
    if(j==7)temphistInj34_lowDphi=(TH1D*)temphist->Clone();
    if(j==8)temphistIInj34_lowDphi=(TH1D*)temphist->Clone();

    if(j==9)temphistInj56=(TH1D*)temphist->Clone();
    if(j==10)temphistIInj56=(TH1D*)temphist->Clone();
    if(j==11)temphistInj56_lowDphi=(TH1D*)temphist->Clone();
    if(j==12)temphistIInj56_lowDphi=(TH1D*)temphist->Clone();

    if(j==13)temphistInj78=(TH1D*)temphist->Clone();
    if(j==14)temphistIInj78=(TH1D*)temphist->Clone();
    if(j==15)temphistInj78_lowDphi=(TH1D*)temphist->Clone();
    if(j==16)temphistIInj78_lowDphi=(TH1D*)temphist->Clone();

    if(j==17)temphistInj9=(TH1D*)temphist->Clone();
    if(j==18)temphistIInj9=(TH1D*)temphist->Clone();
    if(j==19)temphistInj9_lowDphi=(TH1D*)temphist->Clone();
    if(j==20)temphistIInj9_lowDphi=(TH1D*)temphist->Clone();

    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  std::cout<< " new THStack declared"<< std::endl;

  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff");
  temphistIII->SetTitle("IsoEff");
  temphistIII->Write();

  temphistIII_lowDphi = static_cast<TH1D*>(temphistI_lowDphi->Clone("IsoEff_lowDphi"));
  temphistIII_lowDphi->Divide(temphistI_lowDphi,temphistII_lowDphi,1,1,"B");
  temphistIII_lowDphi->SetName("IsoEff_lowDphi");
  temphistIII_lowDphi->SetTitle("IsoEff_lowDphi");
  temphistIII_lowDphi->Write();

  temphistIIInj34 = static_cast<TH1D*>(temphistInj34->Clone("IsoEff_NbNjet34"));
  temphistIIInj34->Divide(temphistInj34,temphistIInj34,temphistIInj34->GetSumOfWeights(),temphistInj34->GetSumOfWeights(),"B");
  temphistIIInj34->SetName("IsoEff_NbNjet34");
  temphistIIInj34->SetTitle("IsoEff_NbNjet34");
  temphistIIInj34->Write();

  temphistIIInj34_lowDphi = static_cast<TH1D*>(temphistInj34_lowDphi->Clone("IsoEff_NbNjet34_lowDphi"));
  temphistIIInj34_lowDphi->Divide(temphistInj34_lowDphi,temphistIInj34_lowDphi,temphistIInj34_lowDphi->GetSumOfWeights(),temphistInj34_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj34_lowDphi->SetName("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->SetTitle("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->Write();

  temphistIIInj56 = static_cast<TH1D*>(temphistInj56->Clone("IsoEff_NbNjet56"));
  temphistIIInj56->Divide(temphistInj56,temphistIInj56,temphistIInj56->GetSumOfWeights(),temphistInj56->GetSumOfWeights(),"B");
  temphistIIInj56->SetName("IsoEff_NbNjet56");
  temphistIIInj56->SetTitle("IsoEff_NbNjet56");
  temphistIIInj56->Write();

  temphistIIInj56_lowDphi = static_cast<TH1D*>(temphistInj56_lowDphi->Clone("IsoEff_NbNjet56_lowDphi"));
  temphistIIInj56_lowDphi->Divide(temphistInj56_lowDphi,temphistIInj56_lowDphi,temphistIInj56_lowDphi->GetSumOfWeights(),temphistInj56_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj56_lowDphi->SetName("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->SetTitle("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->Write();

  temphistIIInj78 = static_cast<TH1D*>(temphistInj78->Clone("IsoEff_NbNjet78"));
  temphistIIInj78->Divide(temphistInj78,temphistIInj78,temphistIInj78->GetSumOfWeights(),temphistInj78->GetSumOfWeights(),"B");
  temphistIIInj78->SetName("IsoEff_NbNjet78");
  temphistIIInj78->SetTitle("IsoEff_NbNjet78");
  temphistIIInj78->Write();

  temphistIIInj78_lowDphi = static_cast<TH1D*>(temphistInj78_lowDphi->Clone("IsoEff_NbNjet78_lowDphi"));
  temphistIIInj78_lowDphi->Divide(temphistInj78_lowDphi,temphistIInj78_lowDphi,temphistIInj78_lowDphi->GetSumOfWeights(),temphistInj78_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj78_lowDphi->SetName("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->SetTitle("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->Write();

  temphistIIInj9 = static_cast<TH1D*>(temphistInj9->Clone("IsoEff_NbNjet9"));
  temphistIIInj9->Divide(temphistInj9,temphistIInj9,temphistIInj9->GetSumOfWeights(),temphistInj9->GetSumOfWeights(),"B");
  temphistIIInj9->SetName("IsoEff_NbNjet9");
  temphistIIInj9->SetTitle("IsoEff_NbNjet9");
  temphistIIInj9->Write();

  temphistIIInj9_lowDphi = static_cast<TH1D*>(temphistInj9_lowDphi->Clone("IsoEff_NbNjet9_lowDphi"));
  temphistIIInj9_lowDphi->Divide(temphistInj9_lowDphi,temphistIInj9_lowDphi,temphistIInj9_lowDphi->GetSumOfWeights(),temphistInj9_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj9_lowDphi->SetName("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->SetTitle("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->Write();

  histname.clear();
  //histname[0]="IsoEff_temp";
  //histname[1]="Iso_pass_temp";
  //histname[2]="Iso_all_temp";

  for(int j=0; j<histname.size(); j++){
    std::cout<<"j "<< j<< " Loop over HT hists"<<std::endl;
    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());
    std::cout << "tempname " << tempname << std::endl;

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins
      std::cout<<"i "<<i<<" Loop over HT bins"<<std::endl;
      int size;
      size=0;
      std::cout<< "T_inputfilevec_size "<<T_inputfilevec.size()<<std::endl;
      
      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      std::cout<<"luminosity "<<luminosity <<endl;
      if (luminosity>0&&doScale){
	std::cout<<" doScale true "<<std::endl;
	temphist->Scale(T_scalevec[i]);
      }
      else if (luminosity>0&&!doScale){
	std::cout<<" doScale false "<<std::endl;
	temphist->Scale(scalefactor);
      }
      temphist->SetFillColor(i+2);
      std::cout<<" Adds hist to stack"<<std::endl;
      tempstack->Add(temphist);
      std::cout<<"i over "<<std::endl;
    }//end of loop over HTbins 1..7
    std::cout<<"End of loop over HT bins"<<std::endl;
    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff_temp"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff_temp");
  temphistIII->SetTitle("IsoEff_temp");
  temphistIII->Write();
  std::cout<<" Before file closing"<<std::endl;
  file->Close();
  printf("T IsoTrks calculated. \n ");

  //..........................................//
  // Acceptance  
  //..........................................//
  
  // Load the files to a vector
  // These are tau template files

  T_inputfilevec.clear();

  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromt_top_.root");
    else if(i==2)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromt_antitop_.root");
    else if(i==3)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromtW_top_.root");
    else if(i==4)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromtW_antitop_.root");
    else if(i==4)sprintf(tempname,"../LostLepton2_MuonEfficienciesFroms_channel_.root");
    else{cout << " Error!! There are only 4 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromT_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hAcc";
  histname[1]="hAccPass";
  histname[2]="hAccAll";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc");
  temphistIII->SetTitle("hAcc");
  temphistIII->Write();

  histname.clear();
  histname[0]="hAcc_0b_";
  histname[1]="hAcc_0b_Pass";
  histname[2]="hAcc_0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_0b_");
  temphistIII->SetTitle("hAcc_0b_");
  temphistIII->Write();

  histname.clear();
  histname[0]="hAcc_non0b_";
  histname[1]="hAcc_non0b_Pass";
  histname[2]="hAcc_non0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_non0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_non0b_");
  temphistIII->SetTitle("hAcc_non0b_");
  temphistIII->Write();


  histname.clear();
  histname[0]="hAcc_lowDphi";
  histname[1]="hAccPass_lowDphi";
  histname[2]="hAccAll_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    std::cout<<" Under acceptance_end of loop over Htbins"<<std::endl;
    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_lowDphi");
  temphistIII->SetTitle("hAcc_lowDphi");
  temphistIII->Write();


  file->Close();
  printf("T acceptance calculated. \n ");

// .....................................................................................................................................................//
// WJet Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> WJet_xs_vec;
  double k_w = 1.23;
  //WJet_xs_vec.push_back(k_w*1817.0); // HT 100-200
  //WJet_xs_vec.push_back(k_w*471.6);  // HT 200-400
  //WJet_xs_vec.push_back(k_w*55.61);  // HT 400-600
  //WJet_xs_vec.push_back(k_w*18.81);  // HT 600-Inf
  // Based on https://twiki.cern.ch/twiki/bin/view/CMS/RA2b13TeVCommissioning#Technical_details
  WJet_xs_vec.push_back(1635.); // HT 100-200
  WJet_xs_vec.push_back(437.);  // HT 200-400
  WJet_xs_vec.push_back(59.5);  // HT 400-600
  WJet_xs_vec.push_back(22.8);  // HT 600-800
  WJet_xs_vec.push_back(1);  // HT 800_1200
  WJet_xs_vec.push_back(1);  // HT 1200_2500
  WJet_xs_vec.push_back(1);  // HT 2500_Inf

  const int wjnHT = (int) WJet_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=wjnHT ; i++){
    /*
    if(i==1)sprintf(tempname,"../../Results/results_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../../Results/results_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../../Results/results_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../../Results/results_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*WJet_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    */
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"cutflow_preselection");
    tempvalue = (luminosity*WJet_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetBinContent(1));
    WJet_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "WJet normalization scale factor determination done \n " << std::endl;

  //..........................................//
  // main histograms like HT, MHT, ...
  //..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 


  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";
  histname[8]="HT2";
  histname[9]="MHT2";
  histname[10]="DelPhi4";
  histname[11]="TauJet_MHT_delPhi";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
  Hname[4]="cutflow_preselection";
  Hname[5]="searchH_lowDphi"; 
  Hname[6]="hPredHTMHT0b";
  Hname[7]="hPredHTMHTwb";
  Hname[8]="hPredNJetBins";
  Hname[9]="hPredNbBins";

 
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<wjnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }




  for(map<int , string >::iterator itt=WJettype.begin(); itt!=WJettype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<wjnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJtype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
          if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
          else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);

        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("WJet main histograms stacked \n ");

  //..........................................//
  // Tau Response Template
  //..........................................//

  // Load the files to a vector 
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTau_TauResponseTemplates_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";
  histname[8]="tau_GenJetPhi";
  histname[9]="hTauResp_0_Up";
  histname[10]="hTauResp_1_Up";
  histname[11]="hTauResp_2_Up";
  histname[12]="hTauResp_3_Up";
  histname[13]="hTauResp_0_Down";
  histname[14]="hTauResp_1_Down";
  histname[15]="hTauResp_2_Down";
  histname[16]="hTauResp_3_Down";

//  histname[4]="genHadTauPtHist";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);
  
    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }

  file->Close();
  printf("WJet tau response ready. \n ");

  //..........................................//
  // Mistag rate 
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../TauBtaggedRate_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../TauBtaggedRate_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../TauBtaggedRate_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../TauBtaggedRate_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../TauBtaggedRate_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../TauBtaggedRate_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../TauBtaggedRate_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TauBtaggedRate_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TauBtaggedRate";
  histname[1]="B_rate_tagged";
  histname[2]="B_rate_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TauBtaggedRate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TauBtaggedRate");
  temphistIII->SetTitle("TauBtaggedRate");
  temphistIII->Write();


  file->Close();
  printf("WJet mistag rate calculated. \n ");

  //..........................................//
  // Acceptance  
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromWJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hAcc";
  histname[1]="hAccPass";
  histname[2]="hAccAll";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc");
  temphistIII->SetTitle("hAcc");
  temphistIII->Write();

  histname.clear();
  histname[0]="hAcc_0b_";
  histname[1]="hAcc_0b_Pass";
  histname[2]="hAcc_0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_0b_");
  temphistIII->SetTitle("hAcc_0b_");
  temphistIII->Write();


  histname.clear();
  histname[0]="hAcc_non0b_";
  histname[1]="hAcc_non0b_Pass";
  histname[2]="hAcc_non0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_non0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_non0b_");
  temphistIII->SetTitle("hAcc_non0b_");
  temphistIII->Write();


  histname.clear();
  histname[0]="hAcc_lowDphi";
  histname[1]="hAccPass_lowDphi";
  histname[2]="hAccAll_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_lowDphi");
  temphistIII->SetTitle("hAcc_lowDphi");
  temphistIII->Write();


  file->Close();
  printf("WJet acceptance calculated. \n ");

  //..........................................//
  // IsoTrk  
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../IsoEfficiencies_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../IsoEfficiencies_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../IsoEfficiencies_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../IsoEfficiencies_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../IsoEfficiencies_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../IsoEfficiencies_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../IsoEfficiencies_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"IsoEfficiencies_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="IsoEff";
  histname[1]="Iso_pass";
  histname[2]="Iso_all";
  histname[3]="Iso_pass_lowDphi";
  histname[4]="Iso_all_lowDphi";

  histname[5]="Iso_pass_nb_njet34";
  histname[6]="Iso_all_nb_njet34";
  histname[7]="Iso_pass_nb_njet34_lowDphi";
  histname[8]="Iso_all_nb_njet34_lowDphi";

  histname[9]="Iso_pass_nb_njet56";
  histname[10]="Iso_all_nb_njet56";
  histname[11]="Iso_pass_nb_njet56_lowDphi";
  histname[12]="Iso_all_nb_njet56_lowDphi";

  histname[13]="Iso_pass_nb_njet78";
  histname[14]="Iso_all_nb_njet78";
  histname[15]="Iso_pass_nb_njet78_lowDphi";
  histname[16]="Iso_all_nb_njet78_lowDphi";

  histname[17]="Iso_pass_nb_njet9";
  histname[18]="Iso_all_nb_njet9";
  histname[19]="Iso_pass_nb_njet9_lowDphi";
  histname[20]="Iso_all_nb_njet9_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins
      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    if(j==3)temphistI_lowDphi=(TH1D*)temphist->Clone();
    if(j==4)temphistII_lowDphi=(TH1D*)temphist->Clone();

    if(j==5)temphistInj34=(TH1D*)temphist->Clone();
    if(j==6)temphistIInj34=(TH1D*)temphist->Clone();
    if(j==7)temphistInj34_lowDphi=(TH1D*)temphist->Clone();
    if(j==8)temphistIInj34_lowDphi=(TH1D*)temphist->Clone();

    if(j==9)temphistInj56=(TH1D*)temphist->Clone();
    if(j==10)temphistIInj56=(TH1D*)temphist->Clone();
    if(j==11)temphistInj56_lowDphi=(TH1D*)temphist->Clone();
    if(j==12)temphistIInj56_lowDphi=(TH1D*)temphist->Clone();

    if(j==13)temphistInj78=(TH1D*)temphist->Clone();
    if(j==14)temphistIInj78=(TH1D*)temphist->Clone();
    if(j==15)temphistInj78_lowDphi=(TH1D*)temphist->Clone();
    if(j==16)temphistIInj78_lowDphi=(TH1D*)temphist->Clone();

    if(j==17)temphistInj9=(TH1D*)temphist->Clone();
    if(j==18)temphistIInj9=(TH1D*)temphist->Clone();
    if(j==19)temphistInj9_lowDphi=(TH1D*)temphist->Clone();
    if(j==20)temphistIInj9_lowDphi=(TH1D*)temphist->Clone();

    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff");
  temphistIII->SetTitle("IsoEff");
  temphistIII->Write();

  temphistIII_lowDphi = static_cast<TH1D*>(temphistI_lowDphi->Clone("IsoEff_lowDphi"));
  temphistIII_lowDphi->Divide(temphistI_lowDphi,temphistII_lowDphi,1,1,"B");
  temphistIII_lowDphi->SetName("IsoEff_lowDphi");
  temphistIII_lowDphi->SetTitle("IsoEff_lowDphi");
  temphistIII_lowDphi->Write();


  temphistIIInj34 = static_cast<TH1D*>(temphistInj34->Clone("IsoEff_NbNjet34"));
  temphistIIInj34->Divide(temphistInj34,temphistIInj34,temphistIInj34->GetSumOfWeights(),temphistInj34->GetSumOfWeights(),"B");
  temphistIIInj34->SetName("IsoEff_NbNjet34");
  temphistIIInj34->SetTitle("IsoEff_NbNjet34");
  temphistIIInj34->Write();

  temphistIIInj34_lowDphi = static_cast<TH1D*>(temphistInj34_lowDphi->Clone("IsoEff_NbNjet34_lowDphi"));
  temphistIIInj34_lowDphi->Divide(temphistInj34_lowDphi,temphistIInj34_lowDphi,temphistIInj34_lowDphi->GetSumOfWeights(),temphistInj34_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj34_lowDphi->SetName("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->SetTitle("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->Write();

  temphistIIInj56 = static_cast<TH1D*>(temphistInj56->Clone("IsoEff_NbNjet56"));
  temphistIIInj56->Divide(temphistInj56,temphistIInj56,temphistIInj56->GetSumOfWeights(),temphistInj56->GetSumOfWeights(),"B");
  temphistIIInj56->SetName("IsoEff_NbNjet56");
  temphistIIInj56->SetTitle("IsoEff_NbNjet56");
  temphistIIInj56->Write();

  temphistIIInj56_lowDphi = static_cast<TH1D*>(temphistInj56_lowDphi->Clone("IsoEff_NbNjet56_lowDphi"));
  temphistIIInj56_lowDphi->Divide(temphistInj56_lowDphi,temphistIInj56_lowDphi,temphistIInj56_lowDphi->GetSumOfWeights(),temphistInj56_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj56_lowDphi->SetName("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->SetTitle("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->Write();

  temphistIIInj78 = static_cast<TH1D*>(temphistInj78->Clone("IsoEff_NbNjet78"));
  temphistIIInj78->Divide(temphistInj78,temphistIInj78,temphistIInj78->GetSumOfWeights(),temphistInj78->GetSumOfWeights(),"B");
  temphistIIInj78->SetName("IsoEff_NbNjet78");
  temphistIIInj78->SetTitle("IsoEff_NbNjet78");
  temphistIIInj78->Write();

  temphistIIInj78_lowDphi = static_cast<TH1D*>(temphistInj78_lowDphi->Clone("IsoEff_NbNjet78_lowDphi"));
  temphistIIInj78_lowDphi->Divide(temphistInj78_lowDphi,temphistIInj78_lowDphi,temphistIInj78_lowDphi->GetSumOfWeights(),temphistInj78_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj78_lowDphi->SetName("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->SetTitle("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->Write();

  temphistIIInj9 = static_cast<TH1D*>(temphistInj9->Clone("IsoEff_NbNjet9"));
  temphistIIInj9->Divide(temphistInj9,temphistIInj9,temphistIInj9->GetSumOfWeights(),temphistInj9->GetSumOfWeights(),"B");
  temphistIIInj9->SetName("IsoEff_NbNjet9");
  temphistIIInj9->SetTitle("IsoEff_NbNjet9");
  temphistIIInj9->Write();

  temphistIIInj9_lowDphi = static_cast<TH1D*>(temphistInj9_lowDphi->Clone("IsoEff_NbNjet9_lowDphi"));
  temphistIIInj9_lowDphi->Divide(temphistInj9_lowDphi,temphistIInj9_lowDphi,temphistIInj9_lowDphi->GetSumOfWeights(),temphistInj9_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj9_lowDphi->SetName("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->SetTitle("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->Write();

  histname.clear();
  //histname[0]="IsoEff_temp";
  //histname[1]="Iso_pass_temp";
  //histname[2]="Iso_all_temp";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff_temp"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff_temp");
  temphistIII->SetTitle("IsoEff_temp");
  temphistIII->Write();

  file->Close();
  printf("WJet IsoTrks calculated. \n ");


  //..........................................//
  // Trigger Efficiency 
  //..........................................//

/*
  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../TriggerEff_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../TriggerEff_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../TriggerEff_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../TriggerEff_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../TriggerEff_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../TriggerEff_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../TriggerEff_WJet_2500_Inf_.root");

    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TriggerEff_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TrigEff";
  histname[1]="trig_pass";
  histname[2]="trig_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TrigEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TrigEff");
  temphistIII->SetTitle("TrigEff");
  temphistIII->Write();


  file->Close();
  printf("WJet trigger efficiency calculated. \n ");

*/

// .....................................................................................................................................................//
// TTbar Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> TTbar_xs_vec;

  //TTbar_xs_vec.push_back(806.1); // 
  // Based on https://twiki.cern.ch/twiki/bin/view/CMS/RA2b13TeVCommissioning#Technical_details
  TTbar_xs_vec.push_back(815.96); // 

  const int ttbarnHT = (int) TTbar_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=ttbarnHT ; i++){
    /*
    if(i==1)sprintf(tempname,"../../Results/results_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*TTbar_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    */
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_TTbar_.root");
    else{cout << " Error!! There are only 1 inclusive TTbar sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"cutflow_preselection");
    tempvalue = (luminosity*TTbar_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetBinContent(1));
    TTbar_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "TTbar normalization scale factor determination done \n " << std::endl;
  



//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";
  histname[8]="HT2";
  histname[9]="MHT2";
  histname[10]="DelPhi4";
  histname[11]="TauJet_MHT_delPhi";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
  Hname[4]="cutflow_preselection";
  Hname[5]="searchH_lowDphi";
  Hname[6]="hPredHTMHT0b";
  Hname[7]="hPredHTMHTwb";
  Hname[8]="hPredNJetBins";
  Hname[9]="hPredNbBins";
 
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<ttbarnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }



  for(map<int , string >::iterator itt=TTbartype.begin(); itt!=TTbartype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<ttbarnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", TTbartype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
          if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
          else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);
        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("TTbar main histograms stacked \n ");

  //..........................................//
  // Tau Response Template
  //..........................................//

  // Load the files to a vector 
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTau_TauResponseTemplates_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTau_TauResponseTemplates_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";
  histname[8]="tau_GenJetPhi";
  histname[9]="hTauResp_0_Up";
  histname[10]="hTauResp_1_Up";
  histname[11]="hTauResp_2_Up";
  histname[12]="hTauResp_3_Up";
  histname[13]="hTauResp_0_Down";
  histname[14]="hTauResp_1_Down";
  histname[15]="hTauResp_2_Down";
  histname[16]="hTauResp_3_Down";

//  histname[4]="genHadTauPtHist";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);
  
    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }

  file->Close();
  printf("TTbar tau response ready. \n ");

//..........................................//
// Muon From Tau 
//..........................................//
/*
  // Load the files to a vector 
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hTau_mu";
  histname[2]="hW_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }

  file->Close();
  printf("TTbar muon from tau probability calculated. \n ");

*/
//..........................................//
// Mistag rate 
//..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../TauBtaggedRate_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TauBtaggedRate_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TauBtaggedRate";
  histname[1]="B_rate_tagged";
  histname[2]="B_rate_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TauBtaggedRate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TauBtaggedRate");
  temphistIII->SetTitle("TauBtaggedRate");
  temphistIII->Write();

  file->Close();
  printf("TTbar mistag rate calculated. \n ");

  //..........................................//
  // Acceptance  
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromTTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromTTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hAcc";
  histname[1]="hAccPass";
  histname[2]="hAccAll";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc");
  temphistIII->SetTitle("hAcc");
  temphistIII->Write();


  histname.clear();
  histname[0]="hAcc_0b_";
  histname[1]="hAcc_0b_Pass";
  histname[2]="hAcc_0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_0b_");
  temphistIII->SetTitle("hAcc_0b_");
  temphistIII->Write();


  histname.clear();
  histname[0]="hAcc_non0b_";
  histname[1]="hAcc_non0b_Pass";
  histname[2]="hAcc_non0b_All";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_non0b_"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_non0b_");
  temphistIII->SetTitle("hAcc_non0b_");
  temphistIII->Write();



  histname.clear();
  histname[0]="hAcc_lowDphi";
  histname[1]="hAccPass_lowDphi";
  histname[2]="hAccAll_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc_lowDphi");
  temphistIII->SetTitle("hAcc_lowDphi");
  temphistIII->Write();

  file->Close();
  printf("TTbar acceptance calculated. \n ");

  //..........................................//
  // IsoTrk  
  //..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../IsoEfficiencies_TTbar_.root");
    else{cout << " Error!! There are only 4 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"IsoEfficiencies_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="IsoEff";
  histname[1]="Iso_pass";
  histname[2]="Iso_all";
  histname[3]="Iso_pass_lowDphi";
  histname[4]="Iso_all_lowDphi";

  histname[5]="Iso_pass_nb_njet34";
  histname[6]="Iso_all_nb_njet34";
  histname[7]="Iso_pass_nb_njet34_lowDphi";
  histname[8]="Iso_all_nb_njet34_lowDphi";

  histname[9]="Iso_pass_nb_njet56";
  histname[10]="Iso_all_nb_njet56";
  histname[11]="Iso_pass_nb_njet56_lowDphi";
  histname[12]="Iso_all_nb_njet56_lowDphi";

  histname[13]="Iso_pass_nb_njet78";
  histname[14]="Iso_all_nb_njet78";
  histname[15]="Iso_pass_nb_njet78_lowDphi";
  histname[16]="Iso_all_nb_njet78_lowDphi";

  histname[17]="Iso_pass_nb_njet9";
  histname[18]="Iso_all_nb_njet9";
  histname[19]="Iso_pass_nb_njet9_lowDphi";
  histname[20]="Iso_all_nb_njet9_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    if(j==3)temphistI_lowDphi=(TH1D*)temphist->Clone();
    if(j==4)temphistII_lowDphi=(TH1D*)temphist->Clone();


    if(j==5)temphistInj34=(TH1D*)temphist->Clone();
    if(j==6)temphistIInj34=(TH1D*)temphist->Clone();
    if(j==7)temphistInj34_lowDphi=(TH1D*)temphist->Clone();
    if(j==8)temphistIInj34_lowDphi=(TH1D*)temphist->Clone();

    if(j==9)temphistInj56=(TH1D*)temphist->Clone();
    if(j==10)temphistIInj56=(TH1D*)temphist->Clone();
    if(j==11)temphistInj56_lowDphi=(TH1D*)temphist->Clone();
    if(j==12)temphistIInj56_lowDphi=(TH1D*)temphist->Clone();

    if(j==13)temphistInj78=(TH1D*)temphist->Clone();
    if(j==14)temphistIInj78=(TH1D*)temphist->Clone();
    if(j==15)temphistInj78_lowDphi=(TH1D*)temphist->Clone();
    if(j==16)temphistIInj78_lowDphi=(TH1D*)temphist->Clone();

    if(j==17)temphistInj9=(TH1D*)temphist->Clone();
    if(j==18)temphistIInj9=(TH1D*)temphist->Clone();
    if(j==19)temphistInj9_lowDphi=(TH1D*)temphist->Clone();
    if(j==20)temphistIInj9_lowDphi=(TH1D*)temphist->Clone();

    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff");
  temphistIII->SetTitle("IsoEff");
  temphistIII->Write();

  temphistIII_lowDphi = static_cast<TH1D*>(temphistI_lowDphi->Clone("IsoEff_lowDphi"));
  temphistIII_lowDphi->Divide(temphistI_lowDphi,temphistII_lowDphi,1,1,"B");
  temphistIII_lowDphi->SetName("IsoEff_lowDphi");
  temphistIII_lowDphi->SetTitle("IsoEff_lowDphi");
  temphistIII_lowDphi->Write();

  temphistIIInj34 = static_cast<TH1D*>(temphistInj34->Clone("IsoEff_NbNjet34"));
  temphistIIInj34->Divide(temphistInj34,temphistIInj34,temphistIInj34->GetSumOfWeights(),temphistInj34->GetSumOfWeights(),"B");
  temphistIIInj34->SetName("IsoEff_NbNjet34");
  temphistIIInj34->SetTitle("IsoEff_NbNjet34");
  temphistIIInj34->Write();

  temphistIIInj34_lowDphi = static_cast<TH1D*>(temphistInj34_lowDphi->Clone("IsoEff_NbNjet34_lowDphi"));
  temphistIIInj34_lowDphi->Divide(temphistInj34_lowDphi,temphistIInj34_lowDphi,temphistIInj34_lowDphi->GetSumOfWeights(),temphistInj34_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj34_lowDphi->SetName("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->SetTitle("IsoEff_NbNjet34_lowDphi");
  temphistIIInj34_lowDphi->Write();

  temphistIIInj56 = static_cast<TH1D*>(temphistInj56->Clone("IsoEff_NbNjet56"));
  temphistIIInj56->Divide(temphistInj56,temphistIInj56,temphistIInj56->GetSumOfWeights(),temphistInj56->GetSumOfWeights(),"B");
  temphistIIInj56->SetName("IsoEff_NbNjet56");
  temphistIIInj56->SetTitle("IsoEff_NbNjet56");
  temphistIIInj56->Write();

  temphistIIInj56_lowDphi = static_cast<TH1D*>(temphistInj56_lowDphi->Clone("IsoEff_NbNjet56_lowDphi"));
  temphistIIInj56_lowDphi->Divide(temphistInj56_lowDphi,temphistIInj56_lowDphi,temphistIInj56_lowDphi->GetSumOfWeights(),temphistInj56_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj56_lowDphi->SetName("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->SetTitle("IsoEff_NbNjet56_lowDphi");
  temphistIIInj56_lowDphi->Write();

  temphistIIInj78 = static_cast<TH1D*>(temphistInj78->Clone("IsoEff_NbNjet78"));
  temphistIIInj78->Divide(temphistInj78,temphistIInj78,temphistIInj78->GetSumOfWeights(),temphistInj78->GetSumOfWeights(),"B");
  temphistIIInj78->SetName("IsoEff_NbNjet78");
  temphistIIInj78->SetTitle("IsoEff_NbNjet78");
  temphistIIInj78->Write();

  temphistIIInj78_lowDphi = static_cast<TH1D*>(temphistInj78_lowDphi->Clone("IsoEff_NbNjet78_lowDphi"));
  temphistIIInj78_lowDphi->Divide(temphistInj78_lowDphi,temphistIInj78_lowDphi,temphistIInj78_lowDphi->GetSumOfWeights(),temphistInj78_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj78_lowDphi->SetName("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->SetTitle("IsoEff_NbNjet78_lowDphi");
  temphistIIInj78_lowDphi->Write();

  temphistIIInj9 = static_cast<TH1D*>(temphistInj9->Clone("IsoEff_NbNjet9"));
  temphistIIInj9->Divide(temphistInj9,temphistIInj9,temphistIInj9->GetSumOfWeights(),temphistInj9->GetSumOfWeights(),"B");
  temphistIIInj9->SetName("IsoEff_NbNjet9");
  temphistIIInj9->SetTitle("IsoEff_NbNjet9");
  temphistIIInj9->Write();

  temphistIIInj9_lowDphi = static_cast<TH1D*>(temphistInj9_lowDphi->Clone("IsoEff_NbNjet9_lowDphi"));
  temphistIIInj9_lowDphi->Divide(temphistInj9_lowDphi,temphistIInj9_lowDphi,temphistIInj9_lowDphi->GetSumOfWeights(),temphistInj9_lowDphi->GetSumOfWeights(),"B");
  temphistIIInj9_lowDphi->SetName("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->SetTitle("IsoEff_NbNjet9_lowDphi");
  temphistIIInj9_lowDphi->Write();

  histname.clear();
  //histname[0]="IsoEff_temp";
  //histname[1]="Iso_pass_temp";
  //histname[2]="Iso_all_temp";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff_temp"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff_temp");
  temphistIII->SetTitle("IsoEff_temp");
  temphistIII->Write();


  file->Close();
  printf("TTbar IsoTrks calculated. \n ");

//..........................................//
// Trigger Efficiency 
//..........................................//
/*
  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../TriggerEff_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TriggerEff_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TrigEff";
  histname[1]="trig_pass";
  histname[2]="trig_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(scalefactor);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TrigEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TrigEff");
  temphistIII->SetTitle("TrigEff");
  temphistIII->Write();


  file->Close();
  printf("TTbar trigger efficiency calculated. \n ");


*/

// ..................................................................................................................................................... //
// Stack main histograms from TTbar and WJet and Single Top
// ..................................................................................................................................................... //

  // There are three contributors 1-TTbar and 2-WJet 3-T
  int NSamples=3;

  // A vector that contains all the samples
  vector<TFile*> sample_inputfilevec;

  THStack * tempstack2 = new THStack("stack","Binned Sample Stack");

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=NSamples ; i++){
    if(i==1)sprintf(tempname,"GenInfo_HadTauEstimation_TTbar_stacked.root");
    else if(i==2)sprintf(tempname,"GenInfo_HadTauEstimation_WJet_stacked.root");
    else if(i==3)sprintf(tempname,"GenInfo_HadTauEstimation_T_stacked.root");
    else{cout << " Error!! There are only 3 contributors! " << endl;}
    sample_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  delete tempstack;
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";
  histname[8]="DelPhi4";
  histname[9]="TauJet_MHT_delPhi";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
  Hname[4]="cutflow_preselection";
  Hname[5]="searchH_lowDphi";
  Hname[6]="hPredHTMHT0b";
  Hname[7]="hPredHTMHTwb";
  Hname[8]="hPredNJetBins";
  Hname[9]="hPredNbBins";

  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<NSamples ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      tempstack2 = (THStack *) sample_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist = (TH1D*)tempstack2->GetStack()->Last();

      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);



    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }



  for(map<int , string >::iterator itt=WJettype.begin(); itt!=WJettype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();


      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<NSamples ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJettype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          tempstack2 = (THStack *) sample_inputfilevec.at(i)->Get(tempname)->Clone();
          temphist = (TH1D*)tempstack2->GetStack()->Last(); 
          
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);
        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("All samples main histograms stacked \n ");





// ..................................................................................................................................................... //
// Combine all tau response templates
// ..................................................................................................................................................... //

  // Add all the tau templates from different sources.
  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";
  histname[8]="tau_GenJetPhi";
  histname[9]="hTauResp_0_Up";
  histname[10]="hTauResp_1_Up";
  histname[11]="hTauResp_2_Up";
  histname[12]="hTauResp_3_Up";
  histname[13]="hTauResp_0_Down";
  histname[14]="hTauResp_1_Down";
  histname[15]="hTauResp_2_Down";
  histname[16]="hTauResp_3_Down";


  // Open the files to read
  sprintf(tempname,"HadTau_TauResponseTemplates_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"HadTau_TauResponseTemplates_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"HadTau_TauResponseTemplates_stacked.root");
  file3 = new TFile(tempname,"RECREATE");

  for(int j=0; j<histname.size(); j++){ 
  
    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    

    // Normalize the response distributions to get the probability density
    if( temphist->Integral("width") > 0. && histname[j]!="tau_GenJetPhi" ){
      temphist->Scale(1./temphist->Integral("width"));
    }

    temphist->Write();

    
  }
  file3->Close();
  file2->Close();
  file->Close();






// ..................................................................................................................................................... //
// Calcualte the rate of muons coming from tau
// ..................................................................................................................................................... //
/*
  // Open the files to read
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"Probability_Tau_mu_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="hTau_mu";
  histname[1]="hW_mu";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"hTau_mu");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hW_mu");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hProb_Tau_mu");
  temphist->SetTitle("hProb_Tau_mu");
  temphist->Write();

  file->Close();
  file2->Close();

*/

// ........................................... //
// Calcualte TauBtaggedRate
// ........................................... //

  // Open the files to read
  sprintf(tempname,"TauBtaggedRate_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"TauBtaggedRate_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"TauBtaggedRate_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="B_rate_tagged";
  histname[1]="B_rate_all";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"TauBtaggedRate_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"B_rate_tagged");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"B_rate_all");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("TauBtaggedRate");
  temphist->SetTitle("TauBtaggedRate");
  temphist->Write();

  file->Close();
  file2->Close();




// ........................................... //
// Calcualte Acceptance
// ........................................... //

  // Open the files to read
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromTTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromWJet_stacked.root");
  file2 = new TFile(tempname,"R");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromT_stacked.root");
  file30 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromstacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="hAccPass";
  histname[1]="hAccAll";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }


  histname.clear();
  histname[0]="hAcc_0b_Pass";
  histname[1]="hAcc_0b_All";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }


  histname.clear();
  histname[0]="hAcc_non0b_Pass";
  histname[1]="hAcc_non0b_All";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }



  histname.clear();
  histname[0]="hAccPass_lowDphi";
  histname[1]="hAccAll_lowDphi";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }

  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromstacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"hAccPass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAccAll");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hAcc");
  temphist->SetTitle("hAcc");
  temphist->Write();

  sprintf(tempname,"hAcc_0b_Pass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAcc_0b_All");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hAcc_0b_");
  temphist->SetTitle("hAcc_0b_");
  temphist->Write();

  sprintf(tempname,"hAcc_non0b_Pass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAcc_non0b_All");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hAcc_non0b_");
  temphist->SetTitle("hAcc_non0b_");
  temphist->Write();


  sprintf(tempname,"hAccPass_lowDphi");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAccAll_lowDphi");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hAcc_lowDphi");
  temphist->SetTitle("hAcc_lowDphi");
  temphist->Write();


  file->Close();
  file2->Close();

// ........................................... //
// Calcualte IsoTrk
// ........................................... //

  // Open the files to read
  sprintf(tempname,"IsoEfficiencies_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"IsoEfficiencies_WJet_stacked.root");
  file2 = new TFile(tempname,"R");
  sprintf(tempname,"IsoEfficiencies_T_stacked.root");
  file30 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"IsoEfficiencies_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="Iso_pass";
  histname[1]="Iso_all";
  histname[2]="Iso_pass_nb_njet34";
  histname[3]="Iso_all_nb_njet34";
  histname[4]="Iso_pass_nb_njet56";
  histname[5]="Iso_all_nb_njet56";
  histname[6]="Iso_pass_nb_njet78";
  histname[7]="Iso_all_nb_njet78";
  histname[8]="Iso_pass_nb_njet9";
  histname[9]="Iso_all_nb_njet9";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

    temphist->Write();

  }

  histname.clear();
  //histname[0]="Iso_pass_temp";
  //histname[1]="Iso_all_temp";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);
    
    temphist->Write();

  }

  histname.clear();
  histname[0]="Iso_pass_lowDphi";
  histname[1]="Iso_all_lowDphi";
  histname[2]="Iso_pass_nb_njet34_lowDphi";
  histname[3]="Iso_all_nb_njet34_lowDphi";
  histname[4]="Iso_pass_nb_njet56_lowDphi";
  histname[5]="Iso_all_nb_njet56_lowDphi";
  histname[6]="Iso_pass_nb_njet78_lowDphi";
  histname[7]="Iso_all_nb_njet78_lowDphi";
  histname[8]="Iso_pass_nb_njet9_lowDphi";
  histname[9]="Iso_all_nb_njet9_lowDphi";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }

  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"IsoEfficiencies_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"Iso_pass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"Iso_all");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("IsoEff");
  temphist->SetTitle("IsoEff");
  temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet34"); sprintf(tempname2,"Iso_all_nb_njet34");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet34");  temphist->SetTitle("IsoEff_NbNjet34"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet56"); sprintf(tempname2,"Iso_all_nb_njet56");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet56");  temphist->SetTitle("IsoEff_NbNjet56"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet78"); sprintf(tempname2,"Iso_all_nb_njet78");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet78");  temphist->SetTitle("IsoEff_NbNjet78"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet9"); sprintf(tempname2,"Iso_all_nb_njet9");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet9");  temphist->SetTitle("IsoEff_NbNjet9"); temphist->Write();

  /*
  sprintf(tempname,"Iso_pass_temp");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"Iso_all_temp");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("IsoEff_temp");
  temphist->SetTitle("IsoEff_temp");
  temphist->Write();
  */

  sprintf(tempname,"Iso_pass_lowDphi");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"Iso_all_lowDphi");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("IsoEff_lowDphi");
  temphist->SetTitle("IsoEff_lowDphi");
  temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet34_lowDphi"); sprintf(tempname2,"Iso_all_nb_njet34_lowDphi");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet34_lowDphi");  temphist->SetTitle("IsoEff_NbNjet34_lowDphi"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet56_lowDphi"); sprintf(tempname2,"Iso_all_nb_njet56_lowDphi");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet56_lowDphi");  temphist->SetTitle("IsoEff_NbNjet56_lowDphi"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet78_lowDphi"); sprintf(tempname2,"Iso_all_nb_njet78_lowDphi");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet78_lowDphi");  temphist->SetTitle("IsoEff_NbNjet78_lowDphi"); temphist->Write();

  sprintf(tempname,"Iso_pass_nb_njet9_lowDphi"); sprintf(tempname2,"Iso_all_nb_njet9_lowDphi");  
  temphist = (TH1D *) file->Get(tempname)->Clone();  temphist2 = (TH1D *) file2->Get(tempname2)->Clone();
  temphist->Divide(temphist,temphist2,temphist2->GetSumOfWeights(),temphist->GetSumOfWeights(),"B");
  temphist->SetName("IsoEff_NbNjet9_lowDphi");  temphist->SetTitle("IsoEff_NbNjet9_lowDphi"); temphist->Write();


  file->Close();
  file2->Close();

} // End of the constructor 

}; // End of the class 

int main(int argc, char *argv[]){

/////////////////////////////////////
if (argc != 2)
{
std::cout << "Please enter something like: ./Stack \"5000\" " << std::endl;
return EXIT_FAILURE;
}
const string lum = argv[1];
/////////////////////////////////////

int luminosity = atoi(lum.c_str());

mainClass mainObj(luminosity);

}
