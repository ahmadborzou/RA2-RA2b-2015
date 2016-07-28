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
  vector<double> T_scalevec, WJet_scalevec, TTbar_scalevec;
  vector<TFile *> T_inputfilevec,WJet_inputfilevec, TTbar_inputfilevec;
  map<int, string> cutname, histname,Hname;
  map<int, string> Ttype, WJettype, TTbartype;
  TFile *file, *file2, *file3, *file30;
  TH1D *temphist, *temphist2, *temphist30, *temphistI, *temphistII, *temphistIII;
  THStack * tempstack;
  TDirectory *cdtoitt, *cdtoit;

public:
mainClass(int luminosity=5000){ // luminosity is in /pb unit

  bool doScale = false;
  Selection * sel = new Selection();
  cutname = sel->cutName();

  //cutname[0]="mht_200";cutname[1]="isoPion";cutname[2]="delphi"; cutname[3]="low_Dphi";
  //cutname[4]="mht_500";cutname[5]="J46_HT5001200_MHT500750";cutname[6]="delphi_NoIso";
  //cutname[7]="Njet_9";

  //cutname[0]="delphi";

  WJettype[0]="allEvents";
  TTbartype[0]="allEvents";
  Ttype[0]="allEvents";

// .....................................................................................................................................................//
// Single Top Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> T_xs_vec;
  T_xs_vec.push_back(44.07); // t_top
  T_xs_vec.push_back(26.23);  // t_antitop 
  T_xs_vec.push_back(35.8);  // tW_top 
  T_xs_vec.push_back(35.8); // tW_antitop
  T_xs_vec.push_back(3.34);  // s 
//  T_xs_vec.push_back(3.34);  // s


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
    if(i==1)sprintf(tempname,"../HadTauEstimation_t_top_.root");
    else if(i==2)sprintf(tempname,"../HadTauEstimation_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../HadTauEstimation_tW_top_.root");
    else if(i==4)sprintf(tempname,"../HadTauEstimation_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../HadTauEstimation_s_channel_.root");
//    else if(i==6)sprintf(tempname,"../HadTauEstimation_Tbar_u_.root");
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
    if(i==1)sprintf(tempname,"../HadTauEstimation_t_top_.root");
    else if(i==2)sprintf(tempname,"../HadTauEstimation_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../HadTauEstimation_tW_top_.root");
    else if(i==4)sprintf(tempname,"../HadTauEstimation_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../HadTauEstimation_s_channel_.root");
//    else if(i==6)sprintf(tempname,"../HadTauEstimation_Tbar_u_.root");
    else{cout << " Error!! There are only 6 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 


  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTauEstimation_T_stacked.root");
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
 
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<tnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }



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
          else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
// Probability mu from nonW sources
//..........................................//

  // Load the files to a vector
  // These are tau template files

  T_inputfilevec.clear();

  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_t_top_.root");
    else if(i==2)sprintf(tempname,"../Probability_Tau_mu_t_antitop_.root");
    else if(i==3)sprintf(tempname,"../Probability_Tau_mu_tW_top_.root");
    else if(i==4)sprintf(tempname,"../Probability_Tau_mu_tW_antitop_.root");
    else if(i==5)sprintf(tempname,"../Probability_Tau_mu_s_channel_.root");    
    else{cout << " Error!! There are only 4 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_T_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hNonW_mu";
  histname[2]="hAll_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu");
  temphistIII->SetTitle("hProb_Tau_mu");
  temphistIII->Write();


  histname.clear();
  histname[0]="hProb_Tau_mu_lowDelphi";
  histname[1]="hNonW_mu_lowDelphi";
  histname[2]="hAll_mu_lowDelphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu_lowDelphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu_lowDelphi");
  temphistIII->SetTitle("hProb_Tau_mu_lowDelphi");
  temphistIII->Write();


  file->Close();
  printf("T Mu from nonW calculated. \n ");


//..........................................//
// dilepton rate
//..........................................//

  // Load the files to a vector
  // These are tau template files

  T_inputfilevec.clear();

  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../DileptonRate_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../DileptonRate_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../DileptonRate_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../DileptonRate_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../DileptonRate_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../DileptonRate_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../DileptonRate_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"DileptonRate_T_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="dilepton_rate";
  histname[1]="dilepton_pass";
  histname[2]="dilepton_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate");
  temphistIII->SetTitle("dilepton_rate");
  temphistIII->Write();

  histname.clear();
  histname[0]="dilepton_rate_lowDphi";
  histname[1]="dilepton_pass_lowDphi";
  histname[2]="dilepton_all_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<tnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(T_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate_lowDphi");
  temphistIII->SetTitle("dilepton_rate_lowDphi");
  temphistIII->Write();



  file->Close();
  printf("T dilepton rate calculated. \n ");





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
  WJet_xs_vec.push_back(1);  // HT 800-1200
  WJet_xs_vec.push_back(1);  // HT 1200_2500
  WJet_xs_vec.push_back(1);  // HT 2500-Inf

  const int wjnHT = (int) WJet_xs_vec.size();   // Total number of HT bin samples

cout << " flag \n " ;
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
    if(i==1)sprintf(tempname,"../HadTauEstimation_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../HadTauEstimation_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../HadTauEstimation_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../HadTauEstimation_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../HadTauEstimation_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../HadTauEstimation_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../HadTauEstimation_WJet_2500_Inf_.root");

    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"cutflow_preselection");
    tempvalue = (luminosity*WJet_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetBinContent(1));

    if (luminosity>0&&doScale)
      printf("Scale: %g, N: %g, Lum: %d, XS: %g \n ",tempvalue,((* (TH1D* ) file->Get(tempname)).GetEntries()),luminosity,WJet_xs_vec[i-1]);

    WJet_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "WJet normalization scale factor determination done \n " << std::endl;




//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTauEstimation_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../HadTauEstimation_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../HadTauEstimation_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../HadTauEstimation_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../HadTauEstimation_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../HadTauEstimation_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../HadTauEstimation_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTauEstimation_WJet_stacked.root");
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

  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<wjnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
          //cout << "HT#: " <<i << ", WJettype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
          if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
          else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
// Probability mu from nonW sources
//..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../Probability_Tau_mu_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../Probability_Tau_mu_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../Probability_Tau_mu_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../Probability_Tau_mu_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../Probability_Tau_mu_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../Probability_Tau_mu_WJet_2500_Inf_.root");

    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hNonW_mu";
  histname[2]="hAll_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu");
  temphistIII->SetTitle("hProb_Tau_mu");
  temphistIII->Write();


  histname.clear();
  histname[0]="hProb_Tau_mu_lowDelphi";
  histname[1]="hNonW_mu_lowDelphi";
  histname[2]="hAll_mu_lowDelphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu_lowDelphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu_lowDelphi");
  temphistIII->SetTitle("hProb_Tau_mu_lowDelphi");
  temphistIII->Write();


  file->Close();
  printf("WJet Mu from nonW calculated. \n ");






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
    else if(i==4)sprintf(tempname,"../TauBtaggedRate_WJet_600_inf_.root");
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
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
// Trigger Efficiency 
//..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../TriggerEff_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../TriggerEff_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../TriggerEff_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../TriggerEff_WJet_600_inf_.root");
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
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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


//..........................................//
// dilepton rate
//..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../DileptonRate_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../DileptonRate_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../DileptonRate_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../DileptonRate_WJet_600_800_.root");
    else if(i==5)sprintf(tempname,"../DileptonRate_WJet_800_1200_.root");
    else if(i==6)sprintf(tempname,"../DileptonRate_WJet_1200_2500_.root");
    else if(i==7)sprintf(tempname,"../DileptonRate_WJet_2500_Inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"DileptonRate_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="dilepton_rate";
  histname[1]="dilepton_pass";
  histname[2]="dilepton_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate");
  temphistIII->SetTitle("dilepton_rate");
  temphistIII->Write();

  histname.clear();
  histname[0]="dilepton_rate_lowDphi";
  histname[1]="dilepton_pass_lowDphi";
  histname[2]="dilepton_all_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(WJet_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate_lowDphi");
  temphistIII->SetTitle("dilepton_rate_lowDphi");
  temphistIII->Write();



  file->Close();
  printf("WJet dilepton rate calculated. \n ");



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
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*TTbar_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
*/
    if(i==1)sprintf(tempname,"../HadTauEstimation_TTbar_.root");
    else{cout << " Error!! There are only 1 inclusive TTbar sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"cutflow_preselection");
    tempvalue = (luminosity*TTbar_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetBinContent(1));

    if (luminosity>0)
      printf("Scale: %g, N: %g, Lum: %d, XS: %g \n ",tempvalue,((* (TH1D* ) file->Get(tempname)).GetEntries()),luminosity,TTbar_xs_vec[i-1]);

    TTbar_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "TTbar normalization scale factor determination done \n " << std::endl;
  



//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTauEstimation_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTauEstimation_TTbar_stacked.root");
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

  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<ttbarnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
          else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
// Probability mu from nonW sources
//..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hNonW_mu";
  histname[2]="hAll_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu");
  temphistIII->SetTitle("hProb_Tau_mu");
  temphistIII->Write();


  histname.clear();
  histname[0]="hProb_Tau_mu_lowDelphi";
  histname[1]="hNonW_mu_lowDelphi";
  histname[2]="hAll_mu_lowDelphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hProb_Tau_mu_lowDelphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hProb_Tau_mu_lowDelphi");
  temphistIII->SetTitle("hProb_Tau_mu_lowDelphi");
  temphistIII->Write();

  file->Close();
  printf("TTbar Mu from nonW calculated. \n ");


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
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
// Trigger Efficiency 
//..........................................//

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
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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


//..........................................//
// dilepton rate
//..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../DileptonRate_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"DileptonRate_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="dilepton_rate";
  histname[1]="dilepton_pass";
  histname[2]="dilepton_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate");
  temphistIII->SetTitle("dilepton_rate");
  temphistIII->Write();


  histname.clear();
  histname[0]="dilepton_rate_lowDphi";
  histname[1]="dilepton_pass_lowDphi";
  histname[2]="dilepton_all_lowDphi";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      if (luminosity>0&&doScale) temphist->Scale(TTbar_scalevec[i]);
      else if (luminosity>0&&!doScale) temphist->Scale(3000);
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
  temphistIII = static_cast<TH1D*>(temphistI->Clone("dilepton_rate_lowDphi"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("dilepton_rate_lowDphi");
  temphistIII->SetTitle("dilepton_rate_lowDphi");
  temphistIII->Write();




  file->Close();
  printf("TTbar dilepton rate calculated. \n ");







// ..................................................................................................................................................... //
// Stack main histograms from TTbar and WJet and Single Top
// ..................................................................................................................................................... //

  // There are two contributors 1-TTbar and 2-WJet 3-T
  int NSamples=3;

  // A vector that contains all the samples
  vector<TFile*> sample_inputfilevec;

  THStack * tempstack2 = new THStack("stack","Binned Sample Stack");

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=NSamples ; i++){
    if(i==1)sprintf(tempname,"HadTauEstimation_TTbar_stacked.root");
    else if(i==2)sprintf(tempname,"HadTauEstimation_WJet_stacked.root");
    else if(i==3)sprintf(tempname,"HadTauEstimation_T_stacked.root");
    else{cout << " Error!! There are only 3 contributors! " << endl;}
    sample_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  delete tempstack;
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTauEstimation_stacked.root");
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



// ........................................... //
//  Probability mu from nonW sources
// ........................................... //

  // Open the files to read
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"Probability_Tau_mu_WJet_stacked.root");
  file2 = new TFile(tempname,"R");
  sprintf(tempname,"Probability_Tau_mu_T_stacked.root");
  file30 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="hNonW_mu";
  histname[1]="hAll_mu";


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
  histname[0]="hNonW_mu_lowDelphi";
  histname[1]="hAll_mu_lowDelphi";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();
    temphist30 = (TH1D *) file30->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);
    temphist->Add(temphist,temphist30,1,1);

  temphist->Write();

  }

  file30->Close();
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");


  sprintf(tempname,"hNonW_mu");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAll_mu");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hProb_Tau_mu");
  temphist->SetTitle("hProb_Tau_mu");
  temphist->Write();


  sprintf(tempname,"hNonW_mu_lowDelphi");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAll_mu_lowDelphi");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hProb_Tau_mu_lowDelphi");
  temphist->SetTitle("hProb_Tau_mu_lowDelphi");
  temphist->Write();


  file->Close();
  file2->Close();




// ........................................... //
//  dilepton rate
// ........................................... //

  // Open the files to read
  sprintf(tempname,"DileptonRate_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"DileptonRate_WJet_stacked.root");
  file2 = new TFile(tempname,"R");
  sprintf(tempname,"DileptonRate_T_stacked.root");
  file30 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"DileptonRate_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="dilepton_pass";
  histname[1]="dilepton_all";


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
  histname[0]="dilepton_pass_lowDphi";
  histname[1]="dilepton_all_lowDphi";

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
  sprintf(tempname,"DileptonRate_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");

  sprintf(tempname,"dilepton_pass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"dilepton_all");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("dilepton_rate");
  temphist->SetTitle("dilepton_rate");
  temphist->Write();


  sprintf(tempname,"dilepton_pass_lowDphi");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"dilepton_all_lowDphi");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();
  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("dilepton_rate_lowDphi");
  temphist->SetTitle("dilepton_rate_lowDphi");
  temphist->Write();


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
