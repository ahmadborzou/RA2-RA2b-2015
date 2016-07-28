/*
  root.exe -b -q HadTauEstimation_output_format.C 
*/

void binMap(TH1* input, TH1* output);
void binMap_ICHEP2016(TH1* input, TH1* output);
void binMap_QCD(TH1* input, TH1* output);
void binMap_QCD_ICHEP2016(TH1* input, TH1* output);
void printMaxMin(TH1* sys);
void takeAverage(TH1* sys);
void takeDiffForSys(TH1* sys, TH1* input_nominal);
void accErrPropagation(TH1* sys, TH1* input_nominal);
void effMapStatErrPropagation(TH1* input, TH1* output);
void effMapConstErrPropagation(TH1* input, double fractionalError, TH1* output);
void muFromTauStatErrPropagation(TH1* input, TH1* one, TH1* output);
void isoTrkVetoErrPropagation(TH1* input, double sys, TH1* one, TH1* output_tot, TH1* output_stat, TH1* output_sys);
void combineHighLowHT_searchBin(TH1* default, TH1* lowHT);
void combineHighLowHT_QCDBin(TH1* default, TH1* lowHT);
void makeNjNbCorr_searchBin(TH1* hist);
void makeNjNbCorr_QCDBin_HiDphi(TH1* hist);
void makeNjNbCorr_QCDBin_LowDphi(TH1* hist);
void reformat(TH1* input, TH1* output);

//These correction factors are derived when low Dphi efficiencies were not corrected for low statistics.
 
//double NjNbCorr[16]={
//  1.08678, 1.1606, 1.21688, 1.38799,
//  0.99422, 1.03512, 1.09677, 1.15551,
//  0.960125, 0.99264, 1.04157, 1.22838,
//  0.863833, 0.803388, 1.01109, 1.12302};
//

//double NjNbCorr[16]={
//  1.06726,1.13884,1.19437,1.36361,
//  0.982114,1.02468,1.08705,1.14535,
//  0.954231,0.986859,1.03692,1.221,
//  0.858298,0.794967,1.00711,1.11734};
//
//double NjNbCorr[16]={1.10302,1.05589,1.11467,1.15494,1.02888,1.00077,1.04321,0.921019,1.00569,0.993849,1.01237,1.02326,0.906839,0.815019,0.990902,0.988837};
//double NjNbCorr[16]={1.09554,1.07391,1.12317,1.25163,1.0102,1.00845,1.05714,1.02093,0.996454,1.01738,0.990167,1.04041,0.833852,0.818807,0.96625,0.95135};
double NjNbCorr[16]=
		  {1.1207,1.10321,1.14125,1.2758,1.03568,1.03778,1.07653,1.03688,1.00263,1.04462,1.00767,1.06245,0.846192,0.885746,0.991038,0.991126};
//double NjNbCorr_QCDHDP[16]={
//  1.06067,1.15766,1.21137,1.38428,
//  0.988007,1.02257,1.05086,1.17749,
//  0.937987,0.97341,0.99937,1.16912,
//  0.842024,0.854788,0.97895,1.12172};
//
//double NjNbCorr_QCDHDP[16]={
//  1.04212,1.13864,1.19202,1.36303,
//  0.980526,1.0161,1.04462,1.17064,
//  0.93641,0.972226,0.998193,1.16753,
//  0.843044,0.856616,0.981188,1.12397};
//
//double NjNbCorr_QCDHDP[16]={1.07954,1.05912,1.11673,1.13352,1.02719,1.00215,1.00608,0.932549,0.986,0.983652,0.977785,0.979731,0.891571,0.879714,0.96842,0.992207};
//double NjNbCorr_QCDHDP[16]={1.0727,1.07053,1.12063,1.17505,1.0059,1.00026,1.02861,0.998668,0.977091,0.989521,0.966759,1.01327,0.891883,0.843374,0.962022,1.00473};                
double NjNbCorr_QCDHDP[16]={1.09651,1.09637,1.14059,1.20009,1.03098,1.0296,1.04985,1.01684,0.98427,1.01241,0.981604,1.02756,0.87334,0.876462,0.943229,0.999277};

//double NjNbCorr_QCDLDP[16]={
//  0.864283,1.04062,1.10166,1.31812,
//  0.843319,0.915936,0.940394,1.13485,
//  0.850232,0.866817,0.924276,1.04468,
//  0.788246,0.804593,0.883932,0.949569};           
//
//double NjNbCorr_QCDLDP[16]={
//  0.915218,1.10424,1.17326,1.4113,
//  0.886127,0.961631,0.987567,1.19345,
//  0.885528,0.903907,0.965426,1.09268,
//  0.816972,0.834622,0.917226,0.984751};           
//
//double NjNbCorr_QCDLDP[16]={0.969584,0.960883,1.02801,0.958625,0.938399,0.939326,0.944426,0.913042,0.937141,0.91543,0.945902,0.915756,0.863937,0.856496,0.910216,0.872439};
//double NjNbCorr_QCDLDP[16]={0.953996,0.968867,1.05964,1.03167,0.915907,0.947684,0.950087,0.988761,0.918411,0.903336,0.968701,0.958839,0.914316,0.877071,0.883752,0.891303};
double NjNbCorr_QCDLDP[16]={0.994667,1.00887,1.09966,1.03729,0.957701,0.982594,0.977459,0.983404,0.942112,0.924123,0.985946,1.01246,0.915061,0.870221,0.908097,0.929114};

void HadTauEstimation_output_format(//string elogForData="KHElog425_",       // Data
				    //string elogForData="ARElog40_4fb_",
				    //string elogForData="ARElog41_2.6fb_",
				    //string elogForData="ARElog46_7.6ifb_",
				    //string elogForData="ARElog49_7.6ifb_",
				    string elogForData="ARElog60_12.9ifb_",
				    //string elogForData="ARElog42_4fb_",
				    string elogForData2="KHElog424_",      // Data 
				    //string elogForMCExp="KHElog420_",      // MC expectation
				    //string elogForMCPre="KHElog424_",      // MC prediction
				    //string elogForMCExp="ARElog41_",
				    //string elogForMCPre="ARElog41_",
				    //
				    string elogForMCExp="ARElog57_",
				    string elogForMCPre="ARElog57_",
				    string elogForSys="ARElog58_",          // MC-based systematics evaluation for Btag mistag uncertainties and muon efficiency stat uncertainties
				    string elogForMuSys="ARElog58_",        // MC-based systematics evaluation for muon ID&Iso systematic efficiencies
				    //string elogForJECSysUp="Elog426_",     // JEC Up systematics
				    //string elogForJECSysDn="Elog426_",     // JEC Down systematics
				    //string elogForJECSysRef="Elog426_",    // JEC uncertainty reference


				    string elogForJECSysUp="ARElog40_",     // JEC Up systematics
				    string elogForJECSysDn="ARElog40_",     // JEC Down systematics
				     string elogForJECSysRef="ARElog40_",    // JEC uncertainty reference
				    
				    string elogForAccPDF="ARElog38_",      // Acceptance uncertainty due to PDF
				    string elogForAccScale="ARElog38_",    // Acceptance uncertainty due to scale
				    //
				    string elogForIsoTrkVeto="ARElog52_",   // Isotrack veto efficiency stat uncertainty
				    string elogForMuFromTau="ARElog52_",    // Muon from tau stat uncertainty
				    string elogForAccStat="ARElog52_",      // Acceptance stat uncertainty
				    string elogForMTStat="ARElog52_",       // MT cut efficiency
				    //
				    double trigEff=1.000,                  // Trigger efficiency for highHT selection (now corrected in the tauhad2_templace.cpp code)
				    double trigEff2=1.000,                 //                    for lowHT  selection
				    double trigEffErr=0.02,                // Trigger efficiency fractional uncertainties
				    double dilep=0.02,                     // Dilepton contamination subtraction uncertainty (fractional)
				    double MtSysFlat=0.01,                 // MT cut efficiency uncertainty
				    double IsoTrkVetoFlat=0.1,             // IsoTrkVeto efficiency uncertainty
                                    // because 2015C does not have our trigger, we scale 2015D to account for that
				    //double lumiTarget=2.584653,            // Luminosity of the search trigger sample 
				    //double lumiControl=2.585297,           // Luminosity of the SingleMuon PD used for the control sample
				    // lumi for V8
				    double lumiTarget=12.9,
				    double lumiControl=12.9,
				    int isys==0){

  char tempname[200];

  //
  // ----- Opening input files
  // -------------------------------

  //
  // Open data files
  //
  //  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_20160624v1_hadd.root",elogForData.c_str());
  //    sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v16b_.root",elogForData.c_str());
  //sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_.root",elogForData.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_V9bcd_.root",elogForData.c_str());
  TFile *DataEstFile = TFile::Open(tempname,"R");
  printf("Opened %s\n",tempname);
  /*
  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_20160623v4_hadd.root",elogForData2.c_str());
  TFile *DataEstFile_lowHT = TFile::Open(tempname,"R");
  printf("Opened %s\n",tempname);
  */

  //
  // Convenient utility histogram
  //
  TH1D* searchBin_one = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_one");  
  searchBin_one->Reset();
  searchBin_one->SetLineColor(1);
  for (int ibin=0; ibin<searchBin_one->GetNbinsX(); ibin++){
    searchBin_one->SetBinContent(ibin+1,1.);
    searchBin_one->SetBinError(ibin+1,0.);
  }

  TH1D* searchBin_box = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_box");
  searchBin_box->Reset();
  searchBin_box->SetLineColor(1);

  TH1D* QCDBin_one = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_one");
  QCDBin_one->Reset();
  QCDBin_one->SetLineColor(1);
  for (int ibin=0; ibin<QCDBin_one->GetNbinsX(); ibin++){
    QCDBin_one->SetBinContent(ibin+1,1.);
    //QCDBin_one->SetBinError(ibin+1,0.);
  }

  TH1D* QCDBin_box = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_box");
  QCDBin_box->Reset();
  QCDBin_box->SetLineColor(1);

  // reformat
  TH1D* searchBin_one_input = static_cast<TH1D*>(searchBin_one->Clone("searchBin_one_input"));
  reformat(searchBin_one_input,searchBin_one);
  TH1D* searchBin_box_input = static_cast<TH1D*>(searchBin_box->Clone("searchBin_box_input"));
  reformat(searchBin_box_input,searchBin_box);

  TH1D* QCDBin_one_input = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_one_input"));
  reformat(QCDBin_one_input,QCDBin_one);
  TH1D* QCDBin_box_input = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_box_input"));
  reformat(QCDBin_box_input,QCDBin_box);
  
  //
  // Open MC expectation and prediction code for closure systematics
  // 
  string sample="stacked";

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad/%sGenInfo_HadTauEstimation_%s.root",elogForMCExp.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad/Stack/%sGenInfo_HadTauEstimation_%s.root",elogForMCExp.c_str(),sample.c_str());
  TFile * MCGenFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForMCPre.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForMCPre.c_str(),sample.c_str());
  TFile * MCEstFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Open MC file including systematics.
  //
  //sprintf(tempname,"TauHad2/HadTauEstimation_%sContainigStatandSysErrors.root",elogForSys.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_withSystematics.root",elogForSys.c_str());
  TFile * MCSysFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //sprintf(tempname,"TauHad2/HadTauEstimation_MuonIDIsoSys_AllSamples_%sV2_.root",elogForMuSys.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_withSystematics.root",elogForSys.c_str());
  TFile * MuSysFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  //
  // JEC variation
  //
  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  //sprintf(tempname,"TauHad2/%stemplatePlus_HadTauEstimation_.root",elogForJECSysUp.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysUp.root",elogForJECSysUp.c_str());
  TFile * JECSysUpFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  //sprintf(tempname,"TauHad2/%stemplateMinus_HadTauEstimation_.root",elogForJECSysDn.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysDn.root",elogForJECSysDn.c_str());
  TFile * JECSysDnFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysRef.root",elogForJECSysRef.c_str());
  TFile * JECSysRefFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Isotrack veto efficiency
  //
  sprintf(tempname,"TauHad/Stack/%smodifiedIsoEfficiencies_stacked.root",elogForIsoTrkVeto.c_str());
  TFile * IsoTrkVetoFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  TH1D* IsoTrkVetoEff = (TH1D*)IsoTrkVetoFile->Get("IsoEff")->Clone("IsoTrkVetoEff");
  TH1D* IsoTrkVetoEff_LowDphi = (TH1D*)IsoTrkVetoFile->Get("IsoEff_lowDphi")->Clone("IsoTrkVetoEff_LowDphi");

  TH1D* searchBin_IsoTrkVetoEff = (TH1D*)searchBin_box->Clone("searchBin_IsoTrkVetoEff"); //searchBin_box--->searchbin histogram of data file
  searchBin_IsoTrkVetoEff->Reset();
  binMap_ICHEP2016(IsoTrkVetoEff,searchBin_IsoTrkVetoEff);

  TH1D* QCDBin_HiDphi_IsoTrkVetoEff = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_IsoTrkVetoEff");
  QCDBin_HiDphi_IsoTrkVetoEff->Reset();
  binMap_QCD_ICHEP2016(IsoTrkVetoEff,QCDBin_HiDphi_IsoTrkVetoEff);

  TH1D* QCDBin_LowDphi_IsoTrkVetoEff = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_IsoTrkVetoEff");
  QCDBin_LowDphi_IsoTrkVetoEff->Reset();
  binMap_QCD_ICHEP2016(IsoTrkVetoEff_LowDphi,QCDBin_LowDphi_IsoTrkVetoEff);

  TH1D* searchBin_IsoTrkVetoEffUncertaintyTot  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintyTot");
  TH1D* searchBin_IsoTrkVetoEffUncertaintyStat = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintyStat");
  TH1D* searchBin_IsoTrkVetoEffUncertaintySys  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintySys");

  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys");

  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys");

  //
  // --- Final propagation of isotrack veto efficiency uncertainty
  isoTrkVetoErrPropagation(searchBin_IsoTrkVetoEff, IsoTrkVetoFlat, searchBin_one,
			   searchBin_IsoTrkVetoEffUncertaintyTot, searchBin_IsoTrkVetoEffUncertaintyStat, searchBin_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_HiDphi_IsoTrkVetoEff, IsoTrkVetoFlat, QCDBin_one,
			   QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_LowDphi_IsoTrkVetoEff, IsoTrkVetoFlat, QCDBin_one,
			   QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys);


  //
  // Mu from tau
  // -----------
  sprintf(tempname,"TauHad2/Stack/%smodifiedProbability_Tau_mu_stacked.root",elogForMuFromTau.c_str());
  TFile * Prob_Tau_mu_file = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  sprintf(tempname,"hProb_Tau_mu");
  TH1D * hProb_Tau_mu =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();
  sprintf(tempname,"hProb_Tau_mu_lowDelphi");
  TH1D * hProb_Tau_mu_LowDphi =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();

  TH1D* searchBin_MuFromTau = (TH1D*)searchBin_box->Clone("searchBin_MuFromTau");
  searchBin_MuFromTau->Reset();
  binMap_ICHEP2016(hProb_Tau_mu,searchBin_MuFromTau);

  TH1D* QCDBin_HiDphi_MuFromTau = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_MuFromTau");
  QCDBin_HiDphi_MuFromTau->Reset();
  binMap_QCD_ICHEP2016(hProb_Tau_mu,QCDBin_HiDphi_MuFromTau);

  TH1D* QCDBin_LowDphi_MuFromTau = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_MuFromTau");
  QCDBin_LowDphi_MuFromTau->Reset();
  binMap_QCD_ICHEP2016(hProb_Tau_mu_LowDphi,QCDBin_LowDphi_MuFromTau);

  TH1D* searchBin_MuFromTauStat  = (TH1D*)searchBin_MuFromTau->Clone("searchBin_MuFromTauStat");
  TH1D* QCDBin_HiDphi_MuFromTauStat  = (TH1D*)QCDBin_HiDphi_MuFromTau->Clone("QCDBin_HiDphi_MuFromTauStat");
  TH1D* QCDBin_LowDphi_MuFromTauStat = (TH1D*)QCDBin_LowDphi_MuFromTau->Clone("QCDBin_LowDphi_MuFromTauStat");

  // 
  // --- Propagation of stat uncertainty on the subtraction of muons from taus
  muFromTauStatErrPropagation(searchBin_MuFromTau,searchBin_one,searchBin_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_HiDphi_MuFromTau, QCDBin_one,QCDBin_HiDphi_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_LowDphi_MuFromTau,QCDBin_one,QCDBin_LowDphi_MuFromTauStat);

  //
  // Acceptance
  // ----------
  sprintf(tempname,"TauHad/Stack/%smodifiedLostLepton2_MuonEfficienciesFromstacked.root",elogForAccStat.c_str());
  TFile * MuAcc_file = new TFile(tempname,"R");  
  printf("Opened %s\n",tempname);
  sprintf(tempname,"hAcc");
  TH1D * hAcc =(TH1D *) MuAcc_file->Get(tempname)->Clone();
  sprintf(tempname,"hAcc_lowDphi");
  TH1D * hAcc_LowDphi =(TH1D *) MuAcc_file->Get(tempname)->Clone();

  TH1D* searchBin_Acc = (TH1D*)searchBin_box->Clone("searchBin_Acc");
  searchBin_Acc->Reset();
  binMap_ICHEP2016(hAcc,searchBin_Acc);

  TH1D* QCDBin_HiDphi_Acc = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_Acc");
  QCDBin_HiDphi_Acc->Reset();
  binMap_QCD_ICHEP2016(hAcc,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_Acc = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_Acc");
  QCDBin_LowDphi_Acc->Reset();
  binMap_QCD_ICHEP2016(hAcc_LowDphi,QCDBin_LowDphi_Acc);

  //
  TH1D* searchBin_AccStat  = (TH1D*)searchBin_Acc->Clone("searchBin_AccStat");
  TH1D* QCDBin_HiDphi_AccStat  = (TH1D*)QCDBin_HiDphi_Acc->Clone("QCDBin_HiDphi_AccStat");
  TH1D* QCDBin_LowDphi_AccStat = (TH1D*)QCDBin_LowDphi_Acc->Clone("QCDBin_LowDphi_AccStat");

  //
  // --- Propagation of stat uncertainties for acceptance corrections
  effMapStatErrPropagation(searchBin_Acc,searchBin_AccStat);
  effMapStatErrPropagation(QCDBin_HiDphi_Acc, QCDBin_HiDphi_AccStat);
  effMapStatErrPropagation(QCDBin_LowDphi_Acc,QCDBin_LowDphi_AccStat);

  // Due to PDF
  //----------

  sprintf(tempname,"TauHad/%sAcceptanceSystematicsFromPDF_AllSamples.root",elogForAccPDF.c_str());
  TFile * AccSysFromPDFFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hAccSysPDFUp = (TH1D *) AccSysFromPDFFile->Get("hAccSysMax")->Clone();
  TH1D * hAccSysPDFDn = (TH1D *) AccSysFromPDFFile->Get("hAccSysMin")->Clone();
  TH1D * hAccSysPDFUp_LowDphi = (TH1D *) AccSysFromPDFFile->Get("hAccSysMax_lowDphi")->Clone();
  TH1D * hAccSysPDFDn_LowDphi = (TH1D *) AccSysFromPDFFile->Get("hAccSysMin_lowDphi")->Clone();

  TH1D* searchBin_AccSysPDFUp = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_AccSysPDFUp");
  searchBin_AccSysPDFUp->Reset();
  binMap_ICHEP2016(hAccSysPDFUp,searchBin_AccSysPDFUp);
  accErrPropagation(searchBin_AccSysPDFUp,searchBin_Acc);
  TH1D* searchBin_AccSysPDFDn = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_AccSysPDFDn");
  searchBin_AccSysPDFDn->Reset();
  binMap_ICHEP2016(hAccSysPDFDn,searchBin_AccSysPDFDn);
  accErrPropagation(searchBin_AccSysPDFDn,searchBin_Acc);

  TH1D* QCDBin_HiDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFUp");
  QCDBin_HiDphi_AccSysPDFUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFUp,QCDBin_HiDphi_AccSysPDFUp);
  accErrPropagation(QCDBin_HiDphi_AccSysPDFUp,QCDBin_HiDphi_Acc);
  TH1D* QCDBin_HiDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFDn");
  QCDBin_HiDphi_AccSysPDFDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFDn,QCDBin_HiDphi_AccSysPDFDn);
  accErrPropagation(QCDBin_HiDphi_AccSysPDFDn,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFUp");
  QCDBin_LowDphi_AccSysPDFUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFUp_LowDphi,QCDBin_LowDphi_AccSysPDFUp);
  accErrPropagation(QCDBin_LowDphi_AccSysPDFUp,QCDBin_LowDphi_Acc);
  TH1D* QCDBin_LowDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFDn");
  QCDBin_LowDphi_AccSysPDFDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFDn_LowDphi,QCDBin_LowDphi_AccSysPDFDn);
  accErrPropagation(QCDBin_LowDphi_AccSysPDFDn,QCDBin_LowDphi_Acc);

  // Due to scale
  //----------
  sprintf(tempname,"TauHad/%sAcceptanceSystematicsFromScale_AllSamples.root",elogForAccScale.c_str());
  TFile * AccSysFromScaleFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hAccSysScaleUp = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMax")->Clone();
  TH1D * hAccSysScaleDn = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMin")->Clone();
  TH1D * hAccSysScaleUp_LowDphi = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMax_lowDphi")->Clone();
  TH1D * hAccSysScaleDn_LowDphi = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMin_lowDphi")->Clone();

  TH1D* searchBin_AccSysScaleUp = (TH1D*)searchBin_box->Clone("searchBin_AccSysScaleUp");
  searchBin_AccSysScaleUp->Reset();
  binMap_ICHEP2016(hAccSysScaleUp,searchBin_AccSysScaleUp);
  accErrPropagation(searchBin_AccSysScaleUp,searchBin_Acc);
  TH1D* searchBin_AccSysScaleDn = (TH1D*)searchBin_box->Clone("searchBin_AccSysScaleDn");
  searchBin_AccSysScaleDn->Reset();
  binMap_ICHEP2016(hAccSysScaleDn,searchBin_AccSysScaleDn);
  accErrPropagation(searchBin_AccSysScaleDn,searchBin_Acc);

  TH1D* QCDBin_HiDphi_AccSysScaleUp = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_AccSysScaleUp");
  QCDBin_HiDphi_AccSysScaleUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleUp,QCDBin_HiDphi_AccSysScaleUp);
  accErrPropagation(QCDBin_HiDphi_AccSysScaleUp,QCDBin_HiDphi_Acc);
  TH1D* QCDBin_HiDphi_AccSysScaleDn = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_AccSysScaleDn");
  QCDBin_HiDphi_AccSysScaleDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleDn,QCDBin_HiDphi_AccSysScaleDn);
  accErrPropagation(QCDBin_HiDphi_AccSysScaleDn,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_AccSysScaleUp = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_AccSysScaleUp");
  QCDBin_LowDphi_AccSysScaleUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleUp_LowDphi,QCDBin_LowDphi_AccSysScaleUp);
  accErrPropagation(QCDBin_LowDphi_AccSysScaleUp,QCDBin_LowDphi_Acc);
  TH1D* QCDBin_LowDphi_AccSysScaleDn = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_AccSysScaleDn");
  QCDBin_LowDphi_AccSysScaleDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleDn_LowDphi,QCDBin_LowDphi_AccSysScaleDn);
  accErrPropagation(QCDBin_LowDphi_AccSysScaleDn,QCDBin_LowDphi_Acc);

  //
  // MT cut efficiency
  // -----------------
  sprintf(tempname,"TauHad2/%smodified_MtEff.root",elogForMTStat.c_str());
  TFile * MtFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hMT = (TH1D *) MtFile->Get("MtCutEff")->Clone();
  TH1D * hMT_LowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();

  string histname;  

  TH1D* searchBin_MtEff = (TH1D*)searchBin_box->Clone("searchBin_MtEff");
  searchBin_MtEff->Reset();
  binMap_ICHEP2016(hMT,searchBin_MtEff);

  TH1D* QCDBin_HiDphi_MtEff = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_MtEff");
  QCDBin_HiDphi_MtEff->Reset();
  binMap_QCD_ICHEP2016(hMT,QCDBin_HiDphi_MtEff);

  TH1D* QCDBin_LowDphi_MtEff = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_MtEff");
  QCDBin_LowDphi_MtEff->Reset();
  binMap_QCD_ICHEP2016(hMT_LowDphi,QCDBin_LowDphi_MtEff);

  //
  TH1D* searchBin_MtEffStat  = (TH1D*)searchBin_MtEff->Clone("searchBin_MtEffStat");
  TH1D* QCDBin_HiDphi_MtEffStat  = (TH1D*)QCDBin_HiDphi_MtEff->Clone("QCDBin_HiDphi_MtEffStat");
  TH1D* QCDBin_LowDphi_MtEffStat = (TH1D*)QCDBin_LowDphi_MtEff->Clone("QCDBin_LowDphi_MtEffStat");

  effMapStatErrPropagation(searchBin_MtEff,searchBin_MtEffStat);
  effMapStatErrPropagation(QCDBin_HiDphi_MtEff, QCDBin_HiDphi_MtEffStat);
  effMapStatErrPropagation(QCDBin_LowDphi_MtEff,QCDBin_LowDphi_MtEffStat);

  TH1D * searchBin_MTSysUp  = (TH1D*) searchBin_one->Clone("searchBin_MTSysUp");
  TH1D * searchBin_MTSysDn  = (TH1D*) searchBin_one->Clone("searchBin_MTSysDn");
  TH1D * searchBin_MTSysRef = (TH1D*) searchBin_one->Clone("searchBin_MTSysRef");

  TH1D * QCDBin_HiDphi_MTSysUp  = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysUp");   // Template for MTSysUp
  TH1D * QCDBin_HiDphi_MTSysDn  = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysDn");   // Template for MTSysDn
  TH1D * QCDBin_HiDphi_MTSysRef = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysRef");  

  TH1D * QCDBin_LowDphi_MTSysUp  = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysUp");  // Template for MTSysUp
  TH1D * QCDBin_LowDphi_MTSysDn  = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysDn");  // Template for MTSysDn
  TH1D * QCDBin_LowDphi_MTSysRef = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysRef");

  effMapConstErrPropagation(searchBin_MTSysRef,-MtSysFlat,searchBin_MTSysUp); // MT cut efficiency reduces -> prediction goes up (i.e. plus)  
  effMapConstErrPropagation(searchBin_MTSysRef,+MtSysFlat,searchBin_MTSysDn);
  effMapConstErrPropagation(QCDBin_HiDphi_MTSysRef,-MtSysFlat,QCDBin_HiDphi_MTSysUp);
  effMapConstErrPropagation(QCDBin_HiDphi_MTSysRef,+MtSysFlat,QCDBin_HiDphi_MTSysDn);
  effMapConstErrPropagation(QCDBin_LowDphi_MTSysRef,-MtSysFlat,QCDBin_LowDphi_MTSysUp);
  effMapConstErrPropagation(QCDBin_LowDphi_MTSysRef,+MtSysFlat,QCDBin_LowDphi_MTSysDn);

  //
  // Const uncertainty
  //
  // dileptonic subtraction
  TH1D *searchBin_DileptonUncertainty = (TH1D*) searchBin_one->Clone("searchBin_DileptonUncertainty");
  searchBin_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_HiDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_HiDphi_DileptonUncertainty");
  QCDBin_HiDphi_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_LowDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_LowDphi_DileptonUncertainty");
  QCDBin_LowDphi_DileptonUncertainty->Scale(dilep);

  //
  // Const uncertainty
  //
  // trigger efficiency
  TH1D *searchBin_TrigEffUncertainty = (TH1D*) searchBin_one->Clone("searchBin_TrigEffUncertainty");
  searchBin_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  TH1D *QCDBin_HiDphi_TrigEffUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_HiDphi_TrigEffUncertainty");
  QCDBin_HiDphi_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  TH1D *QCDBin_LowDphi_TrigEffUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_LowDphi_TrigEffUncertainty");
  QCDBin_LowDphi_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  //
  // ---- Nominal predictions and stat uncertainties -----
  // -----------------------------------------------------

  //
  // ----- Normal search bin predicitons -----
  //

  TH1D* searchBin_nominal = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_nominal");  
  TH1D* searchBin_nominal_input = static_cast<TH1D*>(searchBin_nominal->Clone("searchBin_nominal_input"));
  reformat(searchBin_nominal_input,searchBin_nominal);
  //searchBin_nominal->Print();
  searchBin_nominal->Scale(1/trigEff*lumiTarget/lumiControl); 
  //searchBin_nominal->Print();
  TH1D* searchBin_nominal_fullstatuncertainty = (TH1D*)searchBin_nominal->Clone("searchBin_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<searchBin_nominal->GetNbinsX(); ibin++){
    searchBin_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(searchBin_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  /* KH: no longer used
  TH1D* searchBin_nominal_lowHT = (TH1D*)DataEstFile_lowHT->Get("searchH_b")->Clone("searchBin_nominal_lowHT");
  //searchBin_nominal->Print();
  searchBin_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl); 
  //searchBin_nominal->Print();
  TH1D* searchBin_nominal_fullstatuncertainty_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<searchBin_nominal_lowHT->GetNbinsX(); ibin++){
    searchBin_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(searchBin_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  */

  /* KH: no longer used
  combineHighLowHT_searchBin(searchBin_nominal, searchBin_nominal_lowHT);
  makeNjNbCorr_searchBin(searchBin_nominal);

  combineHighLowHT_searchBin(searchBin_nominal_fullstatuncertainty, searchBin_nominal_fullstatuncertainty_lowHT);
  makeNjNbCorr_searchBin(searchBin_nominal_fullstatuncertainty);
  */

  //
  // ----- QCD bin predicitons -----
  //
  TH1D* QCDBin_HiDphi_nominal  = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_nominal");
  TH1D* QCDBin_HiDphi_nominal_input = static_cast<TH1D*>(QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_nominal_input"));
  reformat(QCDBin_HiDphi_nominal_input,QCDBin_HiDphi_nominal);
  QCDBin_HiDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_HiDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_HiDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_HiDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_HiDphi_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  TH1D* QCDBin_LowDphi_nominal = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_nominal");
  TH1D* QCDBin_LowDphi_nominal_input = static_cast<TH1D*>(QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_input"));
  reformat(QCDBin_LowDphi_nominal_input,QCDBin_LowDphi_nominal);
  QCDBin_LowDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_LowDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_LowDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_LowDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_LowDphi_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  //---

  /* KH: no longer used
  TH1D* QCDBin_HiDphi_nominal_lowHT = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_nominal_lowHT");
  QCDBin_HiDphi_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl);
  TH1D* QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT = (TH1D*)QCDBin_HiDphi_nominal_lowHT->Clone("QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<QCDBin_HiDphi_nominal_lowHT->GetNbinsX(); ibin++){
    QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(QCDBin_HiDphi_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  TH1D* QCDBin_LowDphi_nominal_lowHT = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_nominal_lowHT");
  QCDBin_LowDphi_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl);
  TH1D* QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<QCDBin_LowDphi_nominal_lowHT->GetNbinsX(); ibin++){
    QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(QCDBin_LowDphi_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  */

  /* KH: no longer used
  combineHighLowHT_QCDBin(QCDBin_HiDphi_nominal, QCDBin_HiDphi_nominal_lowHT);
  combineHighLowHT_QCDBin(QCDBin_LowDphi_nominal,QCDBin_LowDphi_nominal_lowHT);
  combineHighLowHT_QCDBin(QCDBin_HiDphi_nominal_fullstatuncertainty, QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT);
  combineHighLowHT_QCDBin(QCDBin_LowDphi_nominal_fullstatuncertainty, QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT);
  */

  // Some additional variables
  const int NSearchBinArray=161;
  const int NQCDBinArray=209;
  double searchBin_stat_uncertainty_fractional[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_up[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_dn[NSearchBinArray];
  double searchBin_uncertainty_correction_stats[NSearchBinArray];

  /* KH: no longer used
  double searchBin_stat_uncertainty_fractional_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_up_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_dn_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_lowHT[NSearchBinArray];
  */

  double QCDBin_HiDphi_stat_uncertainty_fractional[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats_up[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats_dn[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats[NQCDBinArray];

  double QCDBin_LowDphi_stat_uncertainty_fractional[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats_up[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats_dn[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats[NQCDBinArray];

  TH1D* searchBin_StatUncertainties = (TH1D*)searchBin_nominal->Clone("searchBin_StatUncertainties");
  searchBin_StatUncertainties->Reset();
  TH1D* searchBin_StatUncertaintiesFractional = (TH1D*)searchBin_nominal->Clone("searchBin_StatUncertaintiesFractional");
  searchBin_StatUncertaintiesFractional->Reset();

  /* KH: no longer used
  TH1D* searchBin_StatUncertainties_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_StatUncertainties_lowHT");
  searchBin_StatUncertainties_lowHT->Reset();
  TH1D* searchBin_StatUncertaintiesFractional_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_StatUncertaintiesFractional_lowHT");
  searchBin_StatUncertaintiesFractional_lowHT->Reset();
  */

  TH1D* QCDBin_HiDphi_StatUncertainties = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_StatUncertainties");
  QCDBin_HiDphi_StatUncertainties->Reset();
  TH1D* QCDBin_HiDphi_StatUncertaintiesFractional = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_StatUncertaintiesFractional");
  QCDBin_HiDphi_StatUncertaintiesFractional->Reset();

  TH1D* QCDBin_LowDphi_StatUncertainties = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_StatUncertainties");
  QCDBin_LowDphi_StatUncertainties->Reset();
  TH1D* QCDBin_LowDphi_StatUncertaintiesFractional = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_StatUncertaintiesFractional");
  QCDBin_LowDphi_StatUncertaintiesFractional->Reset();

  //
  // ----- Stat uncertainties
  //

  printf("\nstat uncertainty check\n\n");
  
  for (int ibin=1;ibin<=searchBin_nominal->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_stat_uncertainty_fractional[ibin]=0.;
    if (searchBin_nominal->GetBinContent(ibin)!=0.){
      /*
      printf("ibin=%3d: %8.4f\n",ibin,
	     searchBin_nominal->GetBinError(ibin)/searchBin_nominal->GetBinContent(ibin) 
	     );
      */
      searchBin_StatUncertainties->SetBinContent(ibin,searchBin_nominal->GetBinError(ibin));
      searchBin_stat_uncertainty_fractional[ibin] = searchBin_nominal_fullstatuncertainty->GetBinError(ibin)/searchBin_nominal->GetBinContent(ibin);
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,searchBin_stat_uncertainty_fractional[ibin]);
    } else {
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
  }

  /* KH: no longer used
  for (int ibin=1;ibin<=searchBin_nominal_lowHT->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_stat_uncertainty_fractional_lowHT[ibin]=0.;
    if (searchBin_nominal_lowHT->GetBinContent(ibin)!=0.){
      printf("ibin=%3d: %8.4f\n",ibin,
	     searchBin_nominal_lowHT->GetBinError(ibin)/searchBin_nominal_lowHT->GetBinContent(ibin) 
	     );
      searchBin_StatUncertainties_lowHT->SetBinContent(ibin,searchBin_nominal_lowHT->GetBinError(ibin));
      searchBin_stat_uncertainty_fractional_lowHT[ibin] = searchBin_nominal_fullstatuncertainty_lowHT->GetBinError(ibin)/searchBin_nominal_lowHT->GetBinContent(ibin);
      searchBin_StatUncertaintiesFractional_lowHT->SetBinContent(ibin,searchBin_stat_uncertainty_fractional[ibin]);
    } else {
      searchBin_StatUncertaintiesFractional_lowHT->SetBinContent(ibin,2.);
    }
  }
  */

  for (int ibin=1;ibin<=QCDBin_HiDphi_nominal->GetNbinsX();ibin++){
    //
    // Stat uncertainty
    QCDBin_HiDphi_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_HiDphi_nominal->GetBinContent(ibin)!=0.){
      QCDBin_HiDphi_StatUncertainties->SetBinContent(ibin,QCDBin_HiDphi_nominal->GetBinError(ibin));
      QCDBin_HiDphi_stat_uncertainty_fractional[ibin] = QCDBin_HiDphi_nominal->GetBinError(ibin)/QCDBin_HiDphi_nominal->GetBinContent(ibin);    
      QCDBin_HiDphi_StatUncertaintiesFractional->SetBinContent(ibin,QCDBin_HiDphi_stat_uncertainty_fractional[ibin]);
    } else {
      QCDBin_HiDphi_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
    //
    // Stat uncertainty
    QCDBin_LowDphi_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_LowDphi_nominal->GetBinContent(ibin)!=0.){
      QCDBin_LowDphi_StatUncertainties->SetBinContent(ibin,QCDBin_LowDphi_nominal->GetBinError(ibin));
      QCDBin_LowDphi_stat_uncertainty_fractional[ibin] = QCDBin_LowDphi_nominal->GetBinError(ibin)/QCDBin_LowDphi_nominal->GetBinContent(ibin);
      QCDBin_LowDphi_StatUncertaintiesFractional->SetBinContent(ibin,QCDBin_LowDphi_stat_uncertainty_fractional[ibin]);
    } else {
      QCDBin_LowDphi_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
  }

  /* KHtmp
  
  //
  // Jack's inclusive binning
  // ------------------------
  TH1D* hPredHTMHT0b_nominal_ABbins = (TH1D*)DataEstFile->Get("hPredHTMHT0b")->Clone("hPredHTMHT0b_nominal_ABbins");
  TH1D* hPredHTMHTwb_nominal_ABbins = (TH1D*)DataEstFile->Get("hPredHTMHTwb")->Clone("hPredHTMHTwb_nominal_ABbins");
  TH1D* hPredNJetBins_nominal = (TH1D*)DataEstFile->Get("hPredNJetBins")->Clone("hPredNJetBins_nominal");
  TH1D* hPredNbBins_nominal = (TH1D*)DataEstFile->Get("hPredNbBins")->Clone("hPredNbBins_nominal");
  
  TH1D* hPredHTMHT0b_nominal = new TH1D("hPredHTMHT0b_nominal", ";HTMHT Box;Events / Bin", 6, 0.5, 6.5);
  TH1D* hPredHTMHTwb_nominal = new TH1D("hPredHTMHTwb_nominal", ";HTMHT Box;Events / Bin", 6, 0.5, 6.5);
  for (int ibin=0;ibin<6;ibin++){
    hPredHTMHT0b_nominal->SetBinContent(ibin+1,hPredHTMHT0b_nominal_ABbins->GetBinContent(ibin+1));
    hPredHTMHT0b_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHT0b_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
    hPredHTMHTwb_nominal->SetBinContent(ibin+1,hPredHTMHTwb_nominal_ABbins->GetBinContent(ibin+1));
    hPredHTMHTwb_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHTwb_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  for (int ibin=0; ibin<hPredNJetBins_nominal->GetNbinsX(); ibin++){
    hPredNJetBins_nominal->SetBinError(ibin+1,pow(pow(hPredNJetBins_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  for (int ibin=0; ibin<hPredNbBins_nominal->GetNbinsX(); ibin++){
    hPredNbBins_nominal->SetBinError(ibin+1,pow(pow(hPredNbBins_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  KHtmp */

  //
  // ---- Closure systematics -----
  // -----------------------------------------------------

  //
  // From MC file
  //
  TH1D * GenHist, * EstHist, * thist;
  TH1D * histTemplate;
  
  // For closure systematics
  histname="searchH_b";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();

  // reformat
  TH1D* EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  TH1D* GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);

  makeNjNbCorr_searchBin(EstHist);
  
  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);
  
  TH1D * searchBin_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("searchBin_closureUncertainty_adhoc"));
  searchBin_closureUncertainty_adhoc->Reset();
  for (int ibin=1;ibin<=searchBin_closureUncertainty_adhoc->GetNbinsX();ibin++){
    int index=int((ibin-1)/10);
    //std::cout << index << " " << ibin << std::endl;
    searchBin_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr[index]-1.));
  }
  //searchBin_closureUncertainty_adhoc->Print("all");
  
  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  TH1D * searchBin_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("searchBin_closureUncertainty"));
  searchBin_closureUncertainty->Reset();

  TH1D * closureRatio= static_cast<TH1D*>(GenHist->Clone("closureRatio"));
  TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));

  TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
  TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
  closureRatio->Divide(GenHist_Clone,EstHist_Clone,1,1,"");  // Expectation/Prediction-1 - Non closure

  //EstHist_Clone->Print("all");

  //closureRatio->Print("all");
  
  double searchBin_closure_stat_uncertainty_fractional[NSearchBinArray];
  //-----
  int n10percent=0;
  int n30percent=0;
  double avenj1;
  double avenj1nb01;
  double avenj1nb23;
  double avenj2;
  double avenj3;
  //-----
  for (int ibin=1;ibin<=closureRatio->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_closure_stat_uncertainty_fractional[ibin]=0.;
    double Unc,Unc1,Unc2;
    if (closureRatio->GetBinContent(ibin)!=0.){
      searchBin_closure_stat_uncertainty_fractional[ibin] = closureRatio->GetBinError(ibin)/closureRatio->GetBinContent(ibin);
      Unc1=searchBin_closure_stat_uncertainty_fractional[ibin];
    }
    Unc2=fabs(closureRatio->GetBinContent(ibin)-1.);
    Unc = TMath::Max(fabs(closureRatio->GetBinContent(ibin)-1.),searchBin_closure_stat_uncertainty_fractional[ibin]);
    //
    //
    //double Unc3=searchBin_closureUncertainty_adhoc->GetBinContent(ibin);
    //Unc = pow(Unc3*Unc3+Unc*Unc,0.5); 
    //
    //
    if (Unc>1.) Unc=1.;
    searchBin_closureUncertainty->SetBinContent(ibin,Unc);    
    //----
    printf("%5d, %8.2f, %8.2f, %8.2f\n",ibin,Unc,Unc1,Unc2);
    if (Unc<0.1) n10percent++;
    if (Unc<0.3) n30percent++;
    if (ibin>=1 &&ibin<=24) avenj1+=Unc/24.;
    if (ibin>=1 &&ibin<=12) avenj1nb01+=Unc/12.;
    if (ibin>=13&&ibin<=24) avenj1nb23+=Unc/12.;
    if (ibin>=25&&ibin<=48) avenj2+=Unc/24.;
    if (ibin>=49&&ibin<=72) avenj3+=Unc/24.;
    //-----
  }
  printf("10percent %5d\n",n10percent);
  printf("30precent %5d\n",n30percent);
  printf("nj1 average=%8.2f\n",avenj1);
  printf("nj1nb01 average=%8.2f\n",avenj1nb01);
  printf("nj1nb23 average=%8.2f\n",avenj1nb23);
  printf("nj2 average=%8.2f\n",avenj2);
  printf("nj3 average=%8.2f\n",avenj3);
  //searchBin_closureUncertainty->Print("all");

  // For closure systematics
  histname="QCD_Up";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();

  // reformat
  TH1D* EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  TH1D* GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);

  makeNjNbCorr_QCDBin_HiDphi(EstHist);

  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);
  
  TH1D * QCDBin_HiDphi_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureUncertainty_adhoc"));
  QCDBin_HiDphi_closureUncertainty_adhoc->Reset();
  for (int ibin=1;ibin<=QCDBin_HiDphi_closureUncertainty_adhoc->GetNbinsX();ibin++){
    int index=int((ibin-1)/13);
    //std::cout << index << " " << ibin << std::endl;
    QCDBin_HiDphi_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr_QCDHDP[index]-1.));
  }
  //QCDBin_HiDphi_closureUncertainty_adhoc->Print("all");
  
  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  TH1D * QCDBin_HiDphi_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureUncertainty"));
  QCDBin_HiDphi_closureUncertainty->Reset();
  QCDBin_HiDphi_closureUncertainty->SetFillColor(0);
  QCDBin_HiDphi_closureUncertainty->SetLineColor(1);

  TH1D * QCDBin_HiDphi_closureRatio= static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureRatio"));
  TH1D * QCDBin_HiDphi_numerator   = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_numerator"));

  TH1D * QCDBin_HiDphi_GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_GenHist_Clone"));
  TH1D * QCDBin_HiDphi_EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("QCDBin_HiDphi_EstHist_Clone"));
  QCDBin_HiDphi_closureRatio->Divide(QCDBin_HiDphi_GenHist_Clone,QCDBin_HiDphi_EstHist_Clone,1,1,"");  
  // Expectation/Prediction-1 - Non closure

  double QCDBin_HiDphi_closure_stat_uncertainty_fractional[221];

  for (int ibin=1;ibin<=220;ibin++){
    // Stat uncertainty
    QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_HiDphi_closureRatio->GetBinContent(ibin)!=0.){
      QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin] = QCDBin_HiDphi_closureRatio->GetBinError(ibin)/QCDBin_HiDphi_closureRatio->GetBinContent(ibin);
    }
    /*
    std::cout << ibin << " "
  	      << QCDBin_HiDphi_closureRatio->GetBinContent(ibin)-1. << " "
  	      << QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin] << std::endl;
    */
    double Unc;
    Unc = TMath::Max(fabs(QCDBin_HiDphi_closureRatio->GetBinContent(ibin)-1.),QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin]);
    if (Unc>1.) Unc=1.;
    QCDBin_HiDphi_closureUncertainty->SetBinContent(ibin,Unc);    
  }
  //QCDBin_HiDphi_closureUncertainty->Print("all");

  // For closure systematics
  histname="QCD_Low";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();
  //std::cout<<" histname_QCD_Low "<<endl;

  // reformat
  TH1D* EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  TH1D* GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);

  makeNjNbCorr_QCDBin_LowDphi(EstHist);

  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);

  //std::cout<<" QCDBin_LowDphi_closureUncertainty_adhoc "<<endl; 
  TH1D * QCDBin_LowDphi_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureUncertainty_adhoc"));
  QCDBin_LowDphi_closureUncertainty_adhoc->Reset();
  for (int ibin=1;ibin<=QCDBin_LowDphi_closureUncertainty_adhoc->GetNbinsX();ibin++){
    int index=int((ibin-1)/13);
    //std::cout << index << " " << ibin << std::endl;
    QCDBin_LowDphi_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr_QCDLDP[index]-1.));
  }
  //QCDBin_LowDphi_closureUncertainty_adhoc->Print("all");

  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  //std::cout<<" QCDBin_LowDphi_closureUncertainty "<<endl;
  TH1D * QCDBin_LowDphi_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureUncertainty"));
  QCDBin_LowDphi_closureUncertainty->Reset();
  QCDBin_LowDphi_closureUncertainty->SetFillColor(0);
  QCDBin_LowDphi_closureUncertainty->SetLineColor(1);

  TH1D * QCDBin_LowDphi_closureRatio= static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureRatio"));
  TH1D * QCDBin_LowDphi_numerator   = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_numerator"));

  TH1D * QCDBin_LowDphi_GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_GenHist_Clone"));
  TH1D * QCDBin_LowDphi_EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("QCDBin_LowDphi_EstHist_Clone"));
  QCDBin_LowDphi_closureRatio->Divide(QCDBin_LowDphi_GenHist_Clone,QCDBin_LowDphi_EstHist_Clone,1,1,"");  
  // Expectation/Prediction-1 - Non closure
  //std::cout<<" QCDBin_LowDphi_closure_stat_uncertainty_fractional "<<endl;
  double QCDBin_LowDphi_closure_stat_uncertainty_fractional[221];
  for (int ibin=1;ibin<=220;ibin++){
    // Stat uncertainty
    //std::cout<<" Inside for loop "<<" ibin "<<ibin<< endl;
    QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_LowDphi_closureRatio->GetBinContent(ibin)!=0.){
      QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin] = QCDBin_LowDphi_closureRatio->GetBinError(ibin)/QCDBin_LowDphi_closureRatio->GetBinContent(ibin);
    }
    /*
    std::cout << ibin << " "
  	      << QCDBin_LowDphi_closureRatio->GetBinContent(ibin)-1. << " "
  	      << QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin] << std::endl;
    */
    double Unc;
    Unc = TMath::Max(fabs(QCDBin_LowDphi_closureRatio->GetBinContent(ibin)-1.),QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin]);
    if (Unc>1.) Unc=1.;
    //std::cout<<" setbin content for QCDBin_LowDphi "<<endl;
    QCDBin_LowDphi_closureUncertainty->SetBinContent(ibin,Unc);    
  }
  //QCDBin_LowDphi_closureUncertainty->Print("all");

  //
  // ---- BMistag & MuRecoIso systematics -----
  // -----------------------------------------------------
  // 
  // From MC sys file
  //
  histname="searchH_b_";
  string cutname="delphi";
  sprintf(tempname,"allEvents/%s/%s",cutname.c_str(),histname.c_str());
  TH1D * searchBin_default = (TH1D*)MCSysFile->Get(tempname)->Clone();
  TH1D * searchBin_default2 = (TH1D*)MuSysFile->Get(tempname)->Clone();
  //searchBin_default->Print("all");
  //searchBin_default2->Print("all");

  sprintf(tempname,"BMistagPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagDn");
  //std::cout<<" searchBin_BMistagDn "<<endl;
  sprintf(tempname,"RecoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuRecoSysUp = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuRecoSysDn = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuRecoSysDn");
  //std::cout<<" searchBin_MuRecoSysDn "<<endl;
  sprintf(tempname,"IsoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuIsoSysUp = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuIsoSysDn = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuIsoSysDn");

  //std::cout<<" searchBin_MuIsoSysDn "<<endl;


  sprintf(tempname,"MuRecoIsoPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoDn");

  TH1D* searchBin_default_input = static_cast<TH1D*>(searchBin_default->Clone("searchBin_default_input")); reformat(searchBin_default_input,searchBin_default);
  TH1D* searchBin_default2_input = static_cast<TH1D*>(searchBin_default2->Clone("searchBin_default2_input")); reformat(searchBin_default2_input,searchBin_default2);

  TH1D* searchBin_BMistagUp_input = static_cast<TH1D*>(searchBin_BMistagUp->Clone("searchBin_BMistagUp_input")); reformat(searchBin_BMistagUp_input,searchBin_BMistagUp);
  TH1D* searchBin_BMistagDn_input = static_cast<TH1D*>(searchBin_BMistagDn->Clone("searchBin_BMistagDn_input")); reformat(searchBin_BMistagDn_input,searchBin_BMistagDn);

  TH1D* searchBin_MuRecoSysUp_input = static_cast<TH1D*>(searchBin_MuRecoSysUp->Clone("searchBin_MuRecoSysUp_input")); reformat(searchBin_MuRecoSysUp_input,searchBin_MuRecoSysUp);
  TH1D* searchBin_MuRecoSysDn_input = static_cast<TH1D*>(searchBin_MuRecoSysDn->Clone("searchBin_MuRecoSysDn_input")); reformat(searchBin_MuRecoSysDn_input,searchBin_MuRecoSysDn);

  TH1D* searchBin_MuIsoSysUp_input = static_cast<TH1D*>(searchBin_MuIsoSysUp->Clone("searchBin_MuIsoSysUp_input")); reformat(searchBin_MuIsoSysUp_input,searchBin_MuIsoSysUp);
  TH1D* searchBin_MuIsoSysDn_input = static_cast<TH1D*>(searchBin_MuIsoSysDn->Clone("searchBin_MuIsoSysDn_input")); reformat(searchBin_MuIsoSysDn_input,searchBin_MuIsoSysDn);

  TH1D* searchBin_MuRecoIsoUp_input = static_cast<TH1D*>(searchBin_MuRecoIsoUp->Clone("searchBin_MuRecoIsoUp_input")); reformat(searchBin_MuRecoIsoUp_input,searchBin_MuRecoIsoUp);
  TH1D* searchBin_MuRecoIsoDn_input = static_cast<TH1D*>(searchBin_MuRecoIsoDn->Clone("searchBin_MuRecoIsoDn_input")); reformat(searchBin_MuRecoIsoDn_input,searchBin_MuRecoIsoDn);
  
  //
  string histname="QCD_";
  string cutname="delphi";
  sprintf(tempname,"allEvents/%s/%s",cutname.c_str(),histname.c_str());
  TH1D * QCDBin_HiDphi_default = (TH1D*)MCSysFile->Get(tempname)->Clone();

  sprintf(tempname,"BMistagPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_BMistagDn");
  
  sprintf(tempname,"RecoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_MuRecoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_MuRecoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoSysDn");
  //std::cout<< " QCDBin_HiDphi_MuRecoSysDn "<<endl;
  sprintf(tempname,"IsoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoDn");

  TH1D* QCDBin_HiDphi_default_input = static_cast<TH1D*>(QCDBin_HiDphi_default->Clone("QCDBin_HiDphi_default_input")); reformat(QCDBin_HiDphi_default_input,QCDBin_HiDphi_default);

  TH1D* QCDBin_HiDphi_BMistagUp_input = static_cast<TH1D*>(QCDBin_HiDphi_BMistagUp->Clone("QCDBin_HiDphi_BMistagUp_input")); reformat(QCDBin_HiDphi_BMistagUp_input,QCDBin_HiDphi_BMistagUp);
  TH1D* QCDBin_HiDphi_BMistagDn_input = static_cast<TH1D*>(QCDBin_HiDphi_BMistagDn->Clone("QCDBin_HiDphi_BMistagDn_input")); reformat(QCDBin_HiDphi_BMistagDn_input,QCDBin_HiDphi_BMistagDn);

  TH1D* QCDBin_HiDphi_MuRecoSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoSysUp->Clone("QCDBin_HiDphi_MuRecoSysUp_input")); reformat(QCDBin_HiDphi_MuRecoSysUp_input,QCDBin_HiDphi_MuRecoSysUp);
  TH1D* QCDBin_HiDphi_MuRecoSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoSysDn->Clone("QCDBin_HiDphi_MuRecoSysDn_input")); reformat(QCDBin_HiDphi_MuRecoSysDn_input,QCDBin_HiDphi_MuRecoSysDn);

  TH1D* QCDBin_HiDphi_MuIsoSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuIsoSysUp->Clone("QCDBin_HiDphi_MuIsoSysUp_input")); reformat(QCDBin_HiDphi_MuIsoSysUp_input,QCDBin_HiDphi_MuIsoSysUp);
  TH1D* QCDBin_HiDphi_MuIsoSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuIsoSysDn->Clone("QCDBin_HiDphi_MuIsoSysDn_input")); reformat(QCDBin_HiDphi_MuIsoSysDn_input,QCDBin_HiDphi_MuIsoSysDn);

  TH1D* QCDBin_HiDphi_MuRecoIsoUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoIsoUp->Clone("QCDBin_HiDphi_MuRecoIsoUp_input")); reformat(QCDBin_HiDphi_MuRecoIsoUp_input,QCDBin_HiDphi_MuRecoIsoUp);
  TH1D* QCDBin_HiDphi_MuRecoIsoDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoIsoDn->Clone("QCDBin_HiDphi_MuRecoIsoDn_input")); reformat(QCDBin_HiDphi_MuRecoIsoDn_input,QCDBin_HiDphi_MuRecoIsoDn);

  //
  
  string histname="QCD_";
  string cutname="low_Dphi";
  sprintf(tempname,"allEvents/%s/%s",cutname.c_str(),histname.c_str());
  TH1D * QCDBin_LowDphi_default = (TH1D*)MCSysFile->Get(tempname)->Clone();

  sprintf(tempname,"BMistagPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_LowDphi_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_LowDphi_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_BMistagDn");
  
  sprintf(tempname,"RecoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_LowDphi_MuRecoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_LowDphi_MuRecoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoSysDn");

  sprintf(tempname,"IsoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_LowDphi_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_LowDphi_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_LowDphi_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_LowDphi_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoIsoDn");

  TH1D* QCDBin_LowDphi_default_input = static_cast<TH1D*>(QCDBin_LowDphi_default->Clone("QCDBin_LowDphi_default_input")); reformat(QCDBin_LowDphi_default_input,QCDBin_LowDphi_default);

  TH1D* QCDBin_LowDphi_BMistagUp_input = static_cast<TH1D*>(QCDBin_LowDphi_BMistagUp->Clone("QCDBin_LowDphi_BMistagUp_input")); reformat(QCDBin_LowDphi_BMistagUp_input,QCDBin_LowDphi_BMistagUp);
  TH1D* QCDBin_LowDphi_BMistagDn_input = static_cast<TH1D*>(QCDBin_LowDphi_BMistagDn->Clone("QCDBin_LowDphi_BMistagDn_input")); reformat(QCDBin_LowDphi_BMistagDn_input,QCDBin_LowDphi_BMistagDn);

  TH1D* QCDBin_LowDphi_MuRecoSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoSysUp->Clone("QCDBin_LowDphi_MuRecoSysUp_input")); reformat(QCDBin_LowDphi_MuRecoSysUp_input,QCDBin_LowDphi_MuRecoSysUp);
  TH1D* QCDBin_LowDphi_MuRecoSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoSysDn->Clone("QCDBin_LowDphi_MuRecoSysDn_input")); reformat(QCDBin_LowDphi_MuRecoSysDn_input,QCDBin_LowDphi_MuRecoSysDn);

  TH1D* QCDBin_LowDphi_MuIsoSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuIsoSysUp->Clone("QCDBin_LowDphi_MuIsoSysUp_input")); reformat(QCDBin_LowDphi_MuIsoSysUp_input,QCDBin_LowDphi_MuIsoSysUp);
  TH1D* QCDBin_LowDphi_MuIsoSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuIsoSysDn->Clone("QCDBin_LowDphi_MuIsoSysDn_input")); reformat(QCDBin_LowDphi_MuIsoSysDn_input,QCDBin_LowDphi_MuIsoSysDn);

  TH1D* QCDBin_LowDphi_MuRecoIsoUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoIsoUp->Clone("QCDBin_LowDphi_MuRecoIsoUp_input")); reformat(QCDBin_LowDphi_MuRecoIsoUp_input,QCDBin_LowDphi_MuRecoIsoUp);
  TH1D* QCDBin_LowDphi_MuRecoIsoDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoIsoDn->Clone("QCDBin_LowDphi_MuRecoIsoDn_input")); reformat(QCDBin_LowDphi_MuRecoIsoDn_input,QCDBin_LowDphi_MuRecoIsoDn);
  

  //
  // search bin
  // ----------
  
  takeDiffForSys(searchBin_BMistagUp,searchBin_default);
  takeDiffForSys(searchBin_BMistagDn,searchBin_default);

  // //
  // // --- smoothing starts
  // for (int ibin=1; ibin<=searchBin_BMistagUp->GetNbinsX(); ibin++){
  //   if (ibin==24||ibin==48||ibin==66||ibin==72){
  //     double ave;
  //     ave = (searchBin_BMistagUp->GetBinContent(ibin-1)+
  // 	     searchBin_BMistagUp->GetBinContent(ibin-2)+
  // 	     searchBin_BMistagUp->GetBinContent(ibin-3)+
  // 	     searchBin_BMistagUp->GetBinContent(ibin-4)+
  // 	     searchBin_BMistagUp->GetBinContent(ibin-5))/5.;
  //     searchBin_BMistagUp->SetBinContent(ibin,ave);
  //     ave = (searchBin_BMistagDn->GetBinContent(ibin-1)+
  // 	     searchBin_BMistagDn->GetBinContent(ibin-2)+
  // 	     searchBin_BMistagDn->GetBinContent(ibin-3)+
  // 	     searchBin_BMistagDn->GetBinContent(ibin-4)+
  // 	     searchBin_BMistagDn->GetBinContent(ibin-5))/5.;
  //     searchBin_BMistagDn->SetBinContent(ibin,ave);
  //   }
  // }
  // // --- smoothing ends
   
  takeDiffForSys(searchBin_MuRecoSysUp,searchBin_default2);
  takeDiffForSys(searchBin_MuRecoSysDn,searchBin_default2);

  takeDiffForSys(searchBin_MuIsoSysUp,searchBin_default2);
  takeDiffForSys(searchBin_MuIsoSysDn,searchBin_default2);

  takeDiffForSys(searchBin_MuRecoIsoUp,searchBin_default);
  takeDiffForSys(searchBin_MuRecoIsoDn,searchBin_default);

  //
  // QCDBin HiDphi
  // -------------
  takeDiffForSys(QCDBin_HiDphi_BMistagUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_BMistagDn,QCDBin_HiDphi_default);

  // //
  // // --- smoothing starts
  // for (int ibin=1; ibin<=QCDBin_HiDphi_BMistagUp->GetNbinsX(); ibin++){
  //   if (ibin==43||ibin==44||
  // 	ibin==87||ibin==88||
  // 	ibin==131||ibin==132||
  // 	ibin==175||ibin==176||
  // 	ibin==186||ibin==187||
  // 	ibin==197||ibin==198||
  // 	ibin==208||ibin==209||
  // 	ibin==219||ibin==220
  // 	){
  //     double ave;
  //     ave = (QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-1)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-2)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-3)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-4)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-5)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-6)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-7)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-8)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-9)
  // 	     )/9.;
  //     QCDBin_HiDphi_BMistagUp->SetBinContent(ibin,ave);
  //     ave = (QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-1)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-2)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-3)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-4)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-5)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-6)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-7)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-8)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-9)
  // 	     )/9.;
  //     QCDBin_HiDphi_BMistagDn->SetBinContent(ibin,ave);
  //   }
  //   if (ibin==39||ibin==42||ibin==216
  // 	){
  //     ave = (QCDBin_HiDphi_BMistagUp->GetBinContent(ibin-1)+
  // 	     QCDBin_HiDphi_BMistagUp->GetBinContent(ibin+1)
  // 	     )/2.;
  //     QCDBin_HiDphi_BMistagUp->SetBinContent(ibin,ave);
  //     ave = (QCDBin_HiDphi_BMistagDn->GetBinContent(ibin-1)+
  // 	     QCDBin_HiDphi_BMistagDn->GetBinContent(ibin+1)
  // 	     )/2.;
  //     QCDBin_HiDphi_BMistagDn->SetBinContent(ibin,ave);      
  //   }
  // }
  // // --- smoothing ends

  takeDiffForSys(QCDBin_HiDphi_MuRecoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuIsoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuIsoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoDn,QCDBin_HiDphi_default);

  // //
  // // QCDBin LowDphi
  // // --------------
  takeDiffForSys(QCDBin_LowDphi_BMistagUp,QCDBin_LowDphi_default);
  takeDiffForSys(QCDBin_LowDphi_BMistagDn,QCDBin_LowDphi_default);

  // //
  // // --- smoothing starts
  // for (int ibin=1; ibin<=QCDBin_LowDphi_BMistagUp->GetNbinsX(); ibin++){
  //   if (ibin==43||ibin==44||
  // 	ibin==87||ibin==88||
  // 	ibin==131||ibin==132||
  // 	ibin==175||ibin==176||
  // 	ibin==186||ibin==187||
  // 	ibin==197||ibin==198||
  // 	ibin==208||ibin==209||
  // 	ibin==219||ibin==220
  // 	){
  //     double ave;
  //     ave = (QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-1)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-2)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-3)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-4)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-5)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-6)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-7)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-8)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-9)
  // 	     )/9.;
  //     QCDBin_LowDphi_BMistagUp->SetBinContent(ibin,ave);
  //     ave = (QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-1)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-2)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-3)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-4)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-5)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-6)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-7)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-8)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-9)
  // 	     )/9.;
  //     QCDBin_LowDphi_BMistagDn->SetBinContent(ibin,ave);
  //   }
  // }
  // for (int ibin=1; ibin<=QCDBin_LowDphi_BMistagUp->GetNbinsX(); ibin++){
  //   if (ibin==153||ibin==164||ibin==194||ibin==205||ibin==207||ibin==216||ibin==218
  // 	){
  //     ave = (QCDBin_LowDphi_BMistagUp->GetBinContent(ibin-1)+
  // 	     QCDBin_LowDphi_BMistagUp->GetBinContent(ibin+1)
  // 	     )/2.;
  //     QCDBin_LowDphi_BMistagUp->SetBinContent(ibin,ave);
  //     ave = (QCDBin_LowDphi_BMistagDn->GetBinContent(ibin-1)+
  // 	     QCDBin_LowDphi_BMistagDn->GetBinContent(ibin+1)
  // 	     )/2.;
  //     QCDBin_LowDphi_BMistagDn->SetBinContent(ibin,ave);      
  //   }
  // }
  // // --- smoothing ends

 takeDiffForSys(QCDBin_LowDphi_MuRecoSysUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuRecoSysDn,QCDBin_LowDphi_default);

 takeDiffForSys(QCDBin_LowDphi_MuIsoSysUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuIsoSysDn,QCDBin_LowDphi_default);

 takeDiffForSys(QCDBin_LowDphi_MuRecoIsoUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuRecoIsoDn,QCDBin_LowDphi_default);

  //
  // ---- JEC & MT cut efficiency systematics -----
  // -----------------------------------------------------
  //
  // JEC & MT cut efficiency
  //

  string histname="searchH_b";
  sprintf(tempname,"%s",histname.c_str());
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("searchBin_JECSysUp");  
  //TH1D * searchBin_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("searchBin_JECSysDn");  
  //TH1D * searchBin_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("searchBin_JECSysRef");  
  //TH1D * searchBin_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * searchBin_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("searchBin_JECSysUp");
  TH1D * searchBin_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("searchBin_JECSysDn");
  TH1D * searchBin_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("searchBin_JECSysRef");
  searchBin_JECSysUp->Scale(3000.);
  searchBin_JECSysDn->Scale(3000.);
  searchBin_JECSysRef->Scale(3000.);

  TH1D* searchBin_JECSysUp_input = static_cast<TH1D*>(searchBin_JECSysUp->Clone("searchBin_JECSysUp_input")); reformat(searchBin_JECSysUp_input,searchBin_JECSysUp);
  TH1D* searchBin_JECSysDn_input = static_cast<TH1D*>(searchBin_JECSysDn->Clone("searchBin_JECSysDn_input")); reformat(searchBin_JECSysDn_input,searchBin_JECSysDn);
  TH1D* searchBin_JECSysRef_input = static_cast<TH1D*>(searchBin_JECSysRef->Clone("searchBin_JECSysRef_input")); reformat(searchBin_JECSysRef_input,searchBin_JECSysRef);
  
  takeDiffForSys(searchBin_JECSysUp,searchBin_JECSysRef);
  takeDiffForSys(searchBin_JECSysDn,searchBin_JECSysRef);

  string histname="QCD_Up";
  sprintf(tempname,"%s",histname.c_str());
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysUp");  
  //TH1D * QCDBin_HiDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysDn");  
  //TH1D * QCDBin_HiDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysRef");  
  //TH1D * QCDBin_HiDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * QCDBin_HiDphi_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysUp");
  TH1D * QCDBin_HiDphi_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysDn");
  TH1D * QCDBin_HiDphi_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysRef");
  QCDBin_HiDphi_JECSysUp->Scale(3000.);
  QCDBin_HiDphi_JECSysDn->Scale(3000.);
  QCDBin_HiDphi_JECSysRef->Scale(3000.);

  TH1D* QCDBin_HiDphi_JECSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysUp->Clone("QCDBin_HiDphi_JECSysUp_input")); reformat(QCDBin_HiDphi_JECSysUp_input,QCDBin_HiDphi_JECSysUp);
  TH1D* QCDBin_HiDphi_JECSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysDn->Clone("QCDBin_HiDphi_JECSysDn_input")); reformat(QCDBin_HiDphi_JECSysDn_input,QCDBin_HiDphi_JECSysDn);
  TH1D* QCDBin_HiDphi_JECSysRef_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysRef->Clone("QCDBin_HiDphi_JECSysRef_input")); reformat(QCDBin_HiDphi_JECSysRef_input,searchBin_JECSysRef);
  
  takeDiffForSys(QCDBin_HiDphi_JECSysUp,QCDBin_HiDphi_JECSysRef);
  takeDiffForSys(QCDBin_HiDphi_JECSysDn,QCDBin_HiDphi_JECSysRef);

  takeAverage(QCDBin_HiDphi_JECSysUp);
  takeAverage(QCDBin_HiDphi_JECSysDn);

  string histname="QCD_Low";
  sprintf(tempname,"%s",histname.c_str());
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysUp");  
  //TH1D * QCDBin_LowDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysDn");  
  //TH1D * QCDBin_LowDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysRef");  
  //TH1D * QCDBin_LowDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * QCDBin_LowDphi_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysUp");
  TH1D * QCDBin_LowDphi_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysDn");
  TH1D * QCDBin_LowDphi_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysRef");
  QCDBin_LowDphi_JECSysUp->Scale(3000.);
  QCDBin_LowDphi_JECSysDn->Scale(3000.);
  QCDBin_LowDphi_JECSysRef->Scale(3000.);

  TH1D* QCDBin_LowDphi_JECSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysUp->Clone("QCDBin_LowDphi_JECSysUp_input")); reformat(QCDBin_LowDphi_JECSysUp_input,QCDBin_LowDphi_JECSysUp);
  TH1D* QCDBin_LowDphi_JECSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysDn->Clone("QCDBin_LowDphi_JECSysDn_input")); reformat(QCDBin_LowDphi_JECSysDn_input,QCDBin_LowDphi_JECSysDn);
  TH1D* QCDBin_LowDphi_JECSysRef_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysRef->Clone("QCDBin_LowDphi_JECSysRef_input")); reformat(QCDBin_LowDphi_JECSysRef_input,searchBin_JECSysRef);

  takeDiffForSys(QCDBin_LowDphi_JECSysUp,QCDBin_LowDphi_JECSysRef);
  takeDiffForSys(QCDBin_LowDphi_JECSysDn,QCDBin_LowDphi_JECSysRef);

  takeAverage(QCDBin_LowDphi_JECSysUp);
  takeAverage(QCDBin_LowDphi_JECSysDn);

  // //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // // 
  // // Displaying errors
  // //
  // TCanvas *tc1 = new TCanvas("searchBin_UncertaintySummary","searchBin_UncertaintySummary",1200,600);
  // double ymax=+1.5;
  // double ymin=-1.0;
  // double yoff= 0.1;
  // searchBin_box->SetMaximum(ymax);
  // searchBin_box->SetMinimum(ymin);
  // searchBin_box->SetStats(false);
  // searchBin_box->SetTitle("Uncertainties versus search bins");
  // searchBin_box->GetXaxis()->SetTitle("Search bin");
  // searchBin_box->GetYaxis()->SetTitle("Fractional uncertainties");
  // searchBin_box->SetLineWidth(2);
  // searchBin_box->SetLineColor(0);
  // searchBin_box->SetFillColor(0);
  // searchBin_box->Draw();

  // searchBin_StatUncertaintiesFractional->SetLineColor(1);
  // searchBin_StatUncertaintiesFractional->Draw("same");
  // searchBin_StatUncertaintiesFractional->SetFillColor(21);
  
  // searchBin_closureUncertainty->SetLineColor(2);
  // searchBin_closureUncertainty->SetLineWidth(2);
  // searchBin_closureUncertainty->SetFillColor(0);
  // searchBin_closureUncertainty->Draw("same");

  // searchBin_BMistagUp->SetLineColor(8);
  // searchBin_BMistagUp->SetLineWidth(2);
  // searchBin_BMistagUp->Draw("hist,same");

  // searchBin_BMistagDn->SetLineColor(8);
  // searchBin_BMistagDn->SetLineWidth(2);
  // searchBin_BMistagDn->SetLineStyle(2);
  // searchBin_BMistagDn->Draw("hist,same");

  // searchBin_MuRecoSysUp->SetLineColor(4);
  // searchBin_MuRecoSysUp->SetLineWidth(2);
  // searchBin_MuRecoSysUp->SetLineStyle(2);
  // searchBin_MuRecoSysDn->SetLineColor(4);
  // searchBin_MuRecoSysDn->SetLineWidth(2);
  // searchBin_MuRecoSysUp->Draw("hist,same");
  // searchBin_MuRecoSysDn->Draw("hist,same");

  // searchBin_MuIsoSysUp->SetLineColor(4);
  // searchBin_MuIsoSysUp->SetLineWidth(2);
  // searchBin_MuIsoSysUp->SetLineStyle(2);
  // searchBin_MuIsoSysDn->SetLineColor(4);
  // searchBin_MuIsoSysDn->SetLineWidth(2);
  // searchBin_MuIsoSysUp->Draw("hist,same");
  // searchBin_MuIsoSysDn->Draw("hist,same");

  // searchBin_JECSysUp->SetFillColor(0);
  // searchBin_JECSysDn->SetFillColor(0);
  // searchBin_JECSysUp->SetLineWidth(2);
  // searchBin_JECSysDn->SetLineWidth(2);
  // searchBin_JECSysUp->SetLineColor(6);
  // searchBin_JECSysDn->SetLineColor(6);
  // searchBin_JECSysUp->SetLineStyle(2);
  // searchBin_JECSysUp->Draw("hist,same");
  // searchBin_JECSysDn->Draw("hist,same");

  // searchBin_MTSysUp->SetFillColor(0);
  // searchBin_MTSysDn->SetFillColor(0);
  // searchBin_MTSysUp->SetLineWidth(2);
  // searchBin_MTSysDn->SetLineWidth(2);
  // searchBin_MTSysUp->SetLineColor(12);
  // searchBin_MTSysDn->SetLineColor(12);
  // searchBin_MTSysUp->SetLineStyle(2);
  // searchBin_MTSysUp->Draw("hist,same");
  // searchBin_MTSysDn->Draw("hist,same");

  // searchBin_IsoTrkVetoEffUncertaintyTot->SetFillColor(0);
  // searchBin_IsoTrkVetoEffUncertaintyTot->SetLineWidth(2);
  // searchBin_IsoTrkVetoEffUncertaintyTot->SetLineColor(16);
  // searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");
  // searchBin_IsoTrkVetoEffUncertaintySys->SetFillColor(0);
  // searchBin_IsoTrkVetoEffUncertaintySys->SetLineWidth(2);
  // searchBin_IsoTrkVetoEffUncertaintySys->SetLineColor(16);
  // searchBin_IsoTrkVetoEffUncertaintyStat->SetFillColor(0);
  // searchBin_IsoTrkVetoEffUncertaintyStat->SetLineWidth(2);
  // searchBin_IsoTrkVetoEffUncertaintyStat->SetLineColor(16);

  // searchBin_MtEffStat->SetLineWidth(2);
  // searchBin_MtEffStat->SetLineColor(12);
  // searchBin_MtEffStat->SetLineStyle(2);
  // searchBin_MtEffStat->Draw("hist,same");  

  // searchBin_AccStat->SetLineWidth(2);
  // searchBin_AccStat->SetLineColor(2);
  // searchBin_AccStat->SetLineStyle(4);
  // searchBin_AccStat->Draw("hist,same");

  // searchBin_MuFromTauStat->SetLineWidth(2);
  // searchBin_MuFromTauStat->SetLineColor(4);
  // searchBin_MuFromTauStat->SetLineStyle(4);
  // searchBin_MuFromTauStat->Draw("hist,same");

  // searchBin_DileptonUncertainty->SetLineWidth(2);
  // searchBin_DileptonUncertainty->SetLineColor(8);
  // searchBin_DileptonUncertainty->SetLineStyle(4);
  // searchBin_DileptonUncertainty->Draw("hist,same");

  // //
  // // Drawing lines
  // TLine *tlzero = new TLine(1.,0.,73.,0.);
  // tlzero->SetLineStyle(2);
  // tlzero->SetLineColor(4);
  // tlzero->Draw();

  // float ymax_bottom  = ymax * 0.9;
  // float ymax2_bottom = ymax * 0.8;
  // float ymax3_bottom = ymax * 0.8;
  // float ymax4_bottom = ymax * 0.8;
  // float ymin_bottom  = ymin * 0.8;

  // TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  // tline_bottom_1->SetLineStyle(2);
  // tline_bottom_1->Draw();
  
  // TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  // tline_bottom_2->SetLineStyle(2);
  // tline_bottom_2->Draw();

  // TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  // tline_bottom_1a->SetLineStyle(3);
  // tline_bottom_1a->Draw();
  // TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  // tline_bottom_1b->SetLineStyle(3);
  // tline_bottom_1b->Draw();
  // TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  // tline_bottom_1c->SetLineStyle(3);
  // tline_bottom_1c->Draw();

  // TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  // tline_bottom_2a->SetLineStyle(3);
  // tline_bottom_2a->Draw();
  // TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  // tline_bottom_2b->SetLineStyle(3);
  // tline_bottom_2b->Draw();
  // TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  // tline_bottom_2c->SetLineStyle(3);
  // tline_bottom_2c->Draw();
      
  // TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  // tline_bottom_3a->SetLineStyle(3);
  // tline_bottom_3a->Draw();
  // TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  // tline_bottom_3b->SetLineStyle(3);
  // tline_bottom_3b->Draw();
  // TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  // tline_bottom_3c->SetLineStyle(3);
  // tline_bottom_3c->Draw();

  // //
  // // Legend & texts
  // TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  // ttext1->SetTextFont(42);
  // ttext1->SetTextSize(0.04);
  // ttext1->SetTextAlign(22);
  // ttext1->Draw();

  // TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  // ttext2->SetTextFont(42);
  // ttext2->SetTextSize(0.04);
  // ttext2->SetTextAlign(22);
  // ttext2->Draw();

  // TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  // ttext3->SetTextFont(42);
  // ttext3->SetTextSize(0.04);
  // ttext3->SetTextAlign(22);
  // ttext3->Draw();

  // TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  // ttext1a->SetTextFont(42);
  // ttext1a->SetTextSize(0.04);
  // ttext1a->SetTextAlign(22);
  // ttext1a->Draw();
  
  // TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  // ttext1b->SetTextFont(42);
  // ttext1b->SetTextSize(0.04);
  // ttext1b->SetTextAlign(22);
  // ttext1b->Draw();
  
  // TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  // ttext1c->SetTextFont(42);
  // ttext1c->SetTextSize(0.04);
  // ttext1c->SetTextAlign(22);
  // ttext1c->Draw();
  
  // TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  // ttext1d->SetTextFont(42);
  // ttext1d->SetTextSize(0.04);
  // ttext1d->SetTextAlign(22);
  // ttext1d->Draw();

  // //
  // //
  // //
  // Float_t legendX1 = .15; //.50;
  // Float_t legendX2 = .30; //.70;
  // Float_t legendY1 = .13; //.65;
  // Float_t legendY2 = .28; //

  // TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  // catLeg1->SetTextSize(0.032);
  // catLeg1->SetTextFont(42);
  // catLeg1->SetFillColor(0);
  // catLeg1->SetLineColor(0);
  // catLeg1->SetBorderSize(0);

  // sprintf(tempname,"Data stat uncertainties");
  // catLeg1->AddEntry(searchBin_StatUncertainties,tempname);
  // sprintf(tempname,"Non-closure uncertainties");
  // catLeg1->AddEntry(searchBin_closureUncertainty,tempname);
  // sprintf(tempname,"B-tag mistag modeling uncertainties");
  // catLeg1->AddEntry(searchBin_BMistagUp,tempname);
  // sprintf(tempname,"Muon efficiency uncertainties");
  // catLeg1->AddEntry(searchBin_MuRecoSysDn,tempname);
  // catLeg1->Draw();

  // TLegend* catLeg1b = new TLegend(legendX1+0.25,legendY1,legendX2+0.25,legendY2);
  // catLeg1b->SetTextSize(0.032);
  // catLeg1b->SetTextFont(42);
  // catLeg1b->SetFillColor(0);
  // catLeg1b->SetLineColor(0);
  // catLeg1b->SetBorderSize(0);

  // sprintf(tempname,"Tau energy scale uncertainties");
  // catLeg1b->AddEntry(searchBin_JECSysDn,tempname);
  // sprintf(tempname,"MT scale uncertainties");
  // catLeg1b->AddEntry(searchBin_MTSysDn,tempname);
  // sprintf(tempname,"Isotrack veto uncertainties (sys)");
  // catLeg1b->AddEntry(searchBin_IsoTrkVetoEffUncertaintySys,tempname);
  // sprintf(tempname,"Dilepton background subtraction");
  // catLeg1b->AddEntry(searchBin_DileptonUncertainty,tempname);
  // catLeg1b->Draw();

  // TLegend* catLeg1c = new TLegend(legendX1+0.50,legendY1,legendX2+0.50,legendY2);
  // catLeg1c->SetTextSize(0.032);
  // catLeg1c->SetTextFont(42);
  // catLeg1c->SetFillColor(0);
  // catLeg1c->SetLineColor(0);
  // catLeg1c->SetBorderSize(0);

  // sprintf(tempname,"M_{T} cut efficiency (stat)");
  // catLeg1c->AddEntry(searchBin_MtEffStat,tempname);
  // sprintf(tempname,"Acceptance (stat)");
  // catLeg1c->AddEntry(searchBin_AccStat,tempname);
  // sprintf(tempname,"#mu from tau correction (stat)");
  // catLeg1c->AddEntry(searchBin_MuFromTauStat,tempname);
  // catLeg1c->Draw();

  // sprintf(tempname,"searchBin_UncertaintySummary.png");
  // tc1->SaveAs(tempname);
  // sprintf(tempname,"searchBin_UncertaintySummary.pdf");
  // tc1->SaveAs(tempname);
    
  // //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // // 
  // // Displaying errors
  // //
  // TCanvas *tc2 = new TCanvas("searchBin_UncertaintySummary2","searchBin_UncertaintySummary2",1200,600);
  // ymax=+1.5;
  // ymin=-1.0;
  // yoff= 0.1;
  // searchBin_box->SetMaximum(ymax);
  // searchBin_box->SetMinimum(ymin);
  // searchBin_box->Draw();

  // searchBin_closureUncertainty->Draw("same");

  // searchBin_BMistagUp->Draw("hist,same");

  // searchBin_BMistagDn->Draw("hist,same");

  // searchBin_MuRecoSysUp->Draw("hist,same");
  // searchBin_MuRecoSysDn->Draw("hist,same");

  // searchBin_JECSysUp->Draw("hist,same");
  // searchBin_JECSysDn->Draw("hist,same");

  // searchBin_MTSysUp->Draw("hist,same");
  // searchBin_MTSysDn->Draw("hist,same");

  // searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  // //
  // // Drawing lines
  // TLine *tlzero = new TLine(1.,0.,73.,0.);
  // tlzero->SetLineStyle(2);
  // tlzero->SetLineColor(4);
  // tlzero->Draw();

  // ymax_bottom  = ymax * 0.9;
  // ymax2_bottom = ymax * 0.8;
  // ymax3_bottom = ymax * 0.8;
  // ymax4_bottom = ymax * 0.8;
  // ymin_bottom  = ymin * 0.8;

  // TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  // tline_bottom_1->SetLineStyle(2);
  // tline_bottom_1->Draw();
  
  // TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  // tline_bottom_2->SetLineStyle(2);
  // tline_bottom_2->Draw();

  // TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  // tline_bottom_1a->SetLineStyle(3);
  // tline_bottom_1a->Draw();
  // TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  // tline_bottom_1b->SetLineStyle(3);
  // tline_bottom_1b->Draw();
  // TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  // tline_bottom_1c->SetLineStyle(3);
  // tline_bottom_1c->Draw();

  // TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  // tline_bottom_2a->SetLineStyle(3);
  // tline_bottom_2a->Draw();
  // TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  // tline_bottom_2b->SetLineStyle(3);
  // tline_bottom_2b->Draw();
  // TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  // tline_bottom_2c->SetLineStyle(3);
  // tline_bottom_2c->Draw();
      
  // TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  // tline_bottom_3a->SetLineStyle(3);
  // tline_bottom_3a->Draw();
  // TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  // tline_bottom_3b->SetLineStyle(3);
  // tline_bottom_3b->Draw();
  // TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  // tline_bottom_3c->SetLineStyle(3);
  // tline_bottom_3c->Draw();

  // //
  // // Legend & texts
  // TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  // ttext1->SetTextFont(42);
  // ttext1->SetTextSize(0.04);
  // ttext1->SetTextAlign(22);
  // ttext1->Draw();

  // TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  // ttext2->SetTextFont(42);
  // ttext2->SetTextSize(0.04);
  // ttext2->SetTextAlign(22);
  // ttext2->Draw();

  // TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  // ttext3->SetTextFont(42);
  // ttext3->SetTextSize(0.04);
  // ttext3->SetTextAlign(22);
  // ttext3->Draw();

  // TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  // ttext1a->SetTextFont(42);
  // ttext1a->SetTextSize(0.04);
  // ttext1a->SetTextAlign(22);
  // ttext1a->Draw();
  
  // TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  // ttext1b->SetTextFont(42);
  // ttext1b->SetTextSize(0.04);
  // ttext1b->SetTextAlign(22);
  // ttext1b->Draw();
  
  // TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  // ttext1c->SetTextFont(42);
  // ttext1c->SetTextSize(0.04);
  // ttext1c->SetTextAlign(22);
  // ttext1c->Draw();
  
  // TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  // ttext1d->SetTextFont(42);
  // ttext1d->SetTextSize(0.04);
  // ttext1d->SetTextAlign(22);
  // ttext1d->Draw();

  // //
  // //
  // //
  // Float_t legendX1 = .25; //.50;
  // Float_t legendX2 = .45; //.70;
  // Float_t legendY1 = .13; //.65;
  // Float_t legendY2 = .28; //

  // TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  // catLeg1->SetTextSize(0.032);
  // catLeg1->SetTextFont(42);
  // catLeg1->SetFillColor(0);
  // catLeg1->SetLineColor(0);
  // catLeg1->SetBorderSize(0);

  // //sprintf(tempname,"Data stat uncertainties");
  // //catLeg1->AddEntry(searchBin_StatUncertainties,tempname);
  // sprintf(tempname,"Non-closure uncertainties");
  // catLeg1->AddEntry(searchBin_closureUncertainty,tempname);
  // sprintf(tempname,"B-tag mistag modeling uncertainties");
  // catLeg1->AddEntry(searchBin_BMistagUp,tempname);
  // sprintf(tempname,"Muon efficiency uncertainties");
  // catLeg1->AddEntry(searchBin_MuRecoSysDn,tempname);
  // catLeg1->Draw();

  // catLeg1b->Draw();

  // sprintf(tempname,"searchBin_UncertaintySummary2.png");
  // tc2->SaveAs(tempname);
  // sprintf(tempname,"searchBin_UncertaintySummary2.pdf");
  // tc2->SaveAs(tempname);
    
  // //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // // 
  // // Displaying errors
  // //
  // TCanvas *tc2a = new TCanvas("searchBin_UncertaintySummary2a","searchBin_UncertaintySummary2a",1200,600);
  // ymax=+0.3;
  // ymin=-0.3;
  // yoff=0.05;
  // searchBin_box->SetMaximum(ymax);
  // searchBin_box->SetMinimum(ymin);
  // searchBin_box->Draw();

  // searchBin_closureUncertainty->Draw("same");

  // searchBin_BMistagUp->Draw("hist,same");
  // searchBin_BMistagDn->Draw("hist,same");

  // searchBin_MuRecoSysUp->Draw("hist,same");
  // searchBin_MuRecoSysDn->Draw("hist,same");

  // //searchBin_JECSysUp->Draw("hist,same");
  // //searchBin_JECSysDn->Draw("hist,same");

  // //searchBin_MTSysUp->Draw("hist,same");
  // //searchBin_MTSysDn->Draw("hist,same");

  // //searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  // //
  // // Drawing lines
  // TLine *tlzero = new TLine(1.,0.,73.,0.);
  // tlzero->SetLineStyle(2);
  // tlzero->SetLineColor(4);
  // tlzero->Draw();

  // ymax_bottom  = ymax * 0.9;
  // ymax2_bottom = ymax * 0.8;
  // ymax3_bottom = ymax * 0.8;
  // ymax4_bottom = ymax * 0.8;
  // ymin_bottom  = ymin * 0.8;

  // TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  // tline_bottom_1->SetLineStyle(2);
  // tline_bottom_1->Draw();
  
  // TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  // tline_bottom_2->SetLineStyle(2);
  // tline_bottom_2->Draw();

  // TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  // tline_bottom_1a->SetLineStyle(3);
  // tline_bottom_1a->Draw();
  // TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  // tline_bottom_1b->SetLineStyle(3);
  // tline_bottom_1b->Draw();
  // TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  // tline_bottom_1c->SetLineStyle(3);
  // tline_bottom_1c->Draw();

  // TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  // tline_bottom_2a->SetLineStyle(3);
  // tline_bottom_2a->Draw();
  // TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  // tline_bottom_2b->SetLineStyle(3);
  // tline_bottom_2b->Draw();
  // TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  // tline_bottom_2c->SetLineStyle(3);
  // tline_bottom_2c->Draw();
      
  // TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  // tline_bottom_3a->SetLineStyle(3);
  // tline_bottom_3a->Draw();
  // TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  // tline_bottom_3b->SetLineStyle(3);
  // tline_bottom_3b->Draw();
  // TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  // tline_bottom_3c->SetLineStyle(3);
  // tline_bottom_3c->Draw();

  // //
  // // Legend & texts
  // TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  // ttext1->SetTextFont(42);
  // ttext1->SetTextSize(0.04);
  // ttext1->SetTextAlign(22);
  // ttext1->Draw();

  // TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  // ttext2->SetTextFont(42);
  // ttext2->SetTextSize(0.04);
  // ttext2->SetTextAlign(22);
  // ttext2->Draw();

  // TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  // ttext3->SetTextFont(42);
  // ttext3->SetTextSize(0.04);
  // ttext3->SetTextAlign(22);
  // ttext3->Draw();

  // TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  // ttext1a->SetTextFont(42);
  // ttext1a->SetTextSize(0.04);
  // ttext1a->SetTextAlign(22);
  // ttext1a->Draw();
  
  // TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  // ttext1b->SetTextFont(42);
  // ttext1b->SetTextSize(0.04);
  // ttext1b->SetTextAlign(22);
  // ttext1b->Draw();
  
  // TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  // ttext1c->SetTextFont(42);
  // ttext1c->SetTextSize(0.04);
  // ttext1c->SetTextAlign(22);
  // ttext1c->Draw();
  
  // TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  // ttext1d->SetTextFont(42);
  // ttext1d->SetTextSize(0.04);
  // ttext1d->SetTextAlign(22);
  // ttext1d->Draw();

  // //
  // //
  // //
  // catLeg1->Draw();
  // //catLeg1b->Draw();

  // sprintf(tempname,"searchBin_UncertaintySummary2a.png");
  // tc2a->SaveAs(tempname);
  // sprintf(tempname,"searchBin_UncertaintySummary2a.pdf");
  // tc2a->SaveAs(tempname);
    
  // //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // // 
  // // Displaying errors
  // //
  // TCanvas *tc2b = new TCanvas("searchBin_UncertaintySummary2b","searchBin_UncertaintySummary2b",1200,600);
  // ymax=+0.3;
  // ymin=-0.3;
  // yoff=0.05;
  // searchBin_box->SetMaximum(ymax);
  // searchBin_box->SetMinimum(ymin);
  // searchBin_box->Draw();

  // //searchBin_closureUncertainty->Draw("same");

  // //searchBin_BMistagUp->Draw("hist,same");
  // //searchBin_BMistagDn->Draw("hist,same");

  // //searchBin_MuRecoSysUp->Draw("hist,same");
  // //searchBin_MuRecoSysDn->Draw("hist,same");

  // searchBin_JECSysUp->Draw("hist,same");
  // searchBin_JECSysDn->Draw("hist,same");

  // searchBin_MTSysUp->Draw("hist,same");
  // searchBin_MTSysDn->Draw("hist,same");

  // searchBin_IsoTrkVetoEffUncertaintySys->Draw("hist,same");

  // searchBin_DileptonUncertainty->Draw("hist,same");

  // //
  // // Drawing lines
  // TLine *tlzero = new TLine(1.,0.,73.,0.);
  // tlzero->SetLineStyle(2);
  // tlzero->SetLineColor(4);
  // tlzero->Draw();

  // ymax_bottom  = ymax * 0.9;
  // ymax2_bottom = ymax * 0.8;
  // ymax3_bottom = ymax * 0.8;
  // ymax4_bottom = ymax * 0.8;
  // ymin_bottom  = ymin * 0.8;

  // TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  // tline_bottom_1->SetLineStyle(2);
  // tline_bottom_1->Draw();
  
  // TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  // tline_bottom_2->SetLineStyle(2);
  // tline_bottom_2->Draw();

  // TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  // tline_bottom_1a->SetLineStyle(3);
  // tline_bottom_1a->Draw();
  // TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  // tline_bottom_1b->SetLineStyle(3);
  // tline_bottom_1b->Draw();
  // TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  // tline_bottom_1c->SetLineStyle(3);
  // tline_bottom_1c->Draw();

  // TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  // tline_bottom_2a->SetLineStyle(3);
  // tline_bottom_2a->Draw();
  // TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  // tline_bottom_2b->SetLineStyle(3);
  // tline_bottom_2b->Draw();
  // TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  // tline_bottom_2c->SetLineStyle(3);
  // tline_bottom_2c->Draw();
      
  // TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  // tline_bottom_3a->SetLineStyle(3);
  // tline_bottom_3a->Draw();
  // TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  // tline_bottom_3b->SetLineStyle(3);
  // tline_bottom_3b->Draw();
  // TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  // tline_bottom_3c->SetLineStyle(3);
  // tline_bottom_3c->Draw();

  // //
  // // Legend & texts
  // TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  // ttext1->SetTextFont(42);
  // ttext1->SetTextSize(0.04);
  // ttext1->SetTextAlign(22);
  // ttext1->Draw();

  // TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  // ttext2->SetTextFont(42);
  // ttext2->SetTextSize(0.04);
  // ttext2->SetTextAlign(22);
  // ttext2->Draw();

  // TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  // ttext3->SetTextFont(42);
  // ttext3->SetTextSize(0.04);
  // ttext3->SetTextAlign(22);
  // ttext3->Draw();

  // TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  // ttext1a->SetTextFont(42);
  // ttext1a->SetTextSize(0.04);
  // ttext1a->SetTextAlign(22);
  // ttext1a->Draw();
  
  // TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  // ttext1b->SetTextFont(42);
  // ttext1b->SetTextSize(0.04);
  // ttext1b->SetTextAlign(22);
  // ttext1b->Draw();
  
  // TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  // ttext1c->SetTextFont(42);
  // ttext1c->SetTextSize(0.04);
  // ttext1c->SetTextAlign(22);
  // ttext1c->Draw();
  
  // TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  // ttext1d->SetTextFont(42);
  // ttext1d->SetTextSize(0.04);
  // ttext1d->SetTextAlign(22);
  // ttext1d->Draw();

  // TLegend* catLeg1b = new TLegend(legendX1+0.25,legendY1,legendX2+0.25,legendY2);
  // catLeg1b->SetTextSize(0.032);
  // catLeg1b->SetTextFont(42);
  // catLeg1b->SetFillColor(0);
  // catLeg1b->SetLineColor(0);
  // catLeg1b->SetBorderSize(0);

  // sprintf(tempname,"Tau energy scale uncertainties");
  // catLeg1b->AddEntry(searchBin_JECSysDn,tempname);
  // sprintf(tempname,"MT scale uncertainties");
  // catLeg1b->AddEntry(searchBin_MTSysDn,tempname);
  // sprintf(tempname,"Isotrack veto uncertainties (sys)");
  // catLeg1b->AddEntry(searchBin_IsoTrkVetoEffUncertaintySys,tempname);
  // sprintf(tempname,"Dilepton background subtraction");
  // catLeg1b->AddEntry(searchBin_DileptonUncertainty,tempname);
  // catLeg1b->Draw();

  // TLegend* catLeg1c = new TLegend(legendX1+0.50,legendY1,legendX2+0.50,legendY2);
  // catLeg1c->SetTextSize(0.032);
  // catLeg1c->SetTextFont(42);
  // catLeg1c->SetFillColor(0);
  // catLeg1c->SetLineColor(0);
  // catLeg1c->SetBorderSize(0);

  // sprintf(tempname,"M_{T} cut efficiency (stat)");
  // catLeg1c->AddEntry(searchBin_MtEffStat,tempname);
  // sprintf(tempname,"Acceptance (stat)");
  // catLeg1c->AddEntry(searchBin_AccStat,tempname);
  // sprintf(tempname,"#mu from tau correction (stat)");
  // catLeg1c->AddEntry(searchBin_MuFromTauStat,tempname);
  // sprintf(tempname,"Isotrack veto uncertainties (stat)");
  // catLeg1c->AddEntry(searchBin_IsoTrkVetoEffUncertaintyStat,tempname);
  // //catLeg1c->Draw();

  // //
  // //
  // //
  // //catLeg1->Draw();
  // catLeg1b->Draw();

  // sprintf(tempname,"searchBin_UncertaintySummary2b.png");
  // tc2b->SaveAs(tempname);
  // sprintf(tempname,"searchBin_UncertaintySummary2b.pdf");
  // tc2b->SaveAs(tempname);
    
  // //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // // 
  // // Displaying errors
  // //
  // TCanvas *tc2c = new TCanvas("searchBin_UncertaintySummary2c","searchBin_UncertaintySummary2c",1200,600);
  // ymax=+0.3;
  // ymin=-0.3;
  // yoff=0.05;
  // searchBin_box->SetMaximum(ymax);
  // searchBin_box->SetMinimum(ymin);
  // searchBin_box->Draw();

  // //searchBin_closureUncertainty->Draw("same");

  // //searchBin_BMistagUp->Draw("hist,same");
  // //searchBin_BMistagDn->Draw("hist,same");

  // //searchBin_MuRecoSysUp->Draw("hist,same");
  // //searchBin_MuRecoSysDn->Draw("hist,same");

  // //searchBin_JECSysUp->Draw("hist,same");
  // //searchBin_JECSysDn->Draw("hist,same");

  // //searchBin_MTSysUp->Draw("hist,same");
  // //searchBin_MTSysDn->Draw("hist,same");

  // //searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  // searchBin_MtEffStat->Draw("hist,same");  
  // searchBin_AccStat->Draw("hist,same");
  // searchBin_MuFromTauStat->Draw("hist,same");
  // searchBin_IsoTrkVetoEffUncertaintyStat->Draw("hist,same");

  // //
  // // Drawing lines
  // TLine *tlzero = new TLine(1.,0.,73.,0.);
  // tlzero->SetLineStyle(2);
  // tlzero->SetLineColor(4);
  // tlzero->Draw();

  // ymax_bottom  = ymax * 0.9;
  // ymax2_bottom = ymax * 0.8;
  // ymax3_bottom = ymax * 0.8;
  // ymax4_bottom = ymax * 0.8;
  // ymin_bottom  = ymin * 0.8;

  // TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  // tline_bottom_1->SetLineStyle(2);
  // tline_bottom_1->Draw();
  
  // TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  // tline_bottom_2->SetLineStyle(2);
  // tline_bottom_2->Draw();

  // TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  // tline_bottom_1a->SetLineStyle(3);
  // tline_bottom_1a->Draw();
  // TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  // tline_bottom_1b->SetLineStyle(3);
  // tline_bottom_1b->Draw();
  // TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  // tline_bottom_1c->SetLineStyle(3);
  // tline_bottom_1c->Draw();

  // TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  // tline_bottom_2a->SetLineStyle(3);
  // tline_bottom_2a->Draw();
  // TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  // tline_bottom_2b->SetLineStyle(3);
  // tline_bottom_2b->Draw();
  // TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  // tline_bottom_2c->SetLineStyle(3);
  // tline_bottom_2c->Draw();
      
  // TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  // tline_bottom_3a->SetLineStyle(3);
  // tline_bottom_3a->Draw();
  // TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  // tline_bottom_3b->SetLineStyle(3);
  // tline_bottom_3b->Draw();
  // TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  // tline_bottom_3c->SetLineStyle(3);
  // tline_bottom_3c->Draw();

  // //
  // // Legend & texts
  // TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  // ttext1->SetTextFont(42);
  // ttext1->SetTextSize(0.04);
  // ttext1->SetTextAlign(22);
  // ttext1->Draw();

  // TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  // ttext2->SetTextFont(42);
  // ttext2->SetTextSize(0.04);
  // ttext2->SetTextAlign(22);
  // ttext2->Draw();

  // TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  // ttext3->SetTextFont(42);
  // ttext3->SetTextSize(0.04);
  // ttext3->SetTextAlign(22);
  // ttext3->Draw();

  // TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  // ttext1a->SetTextFont(42);
  // ttext1a->SetTextSize(0.04);
  // ttext1a->SetTextAlign(22);
  // ttext1a->Draw();
  
  // TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  // ttext1b->SetTextFont(42);
  // ttext1b->SetTextSize(0.04);
  // ttext1b->SetTextAlign(22);
  // ttext1b->Draw();
  
  // TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  // ttext1c->SetTextFont(42);
  // ttext1c->SetTextSize(0.04);
  // ttext1c->SetTextAlign(22);
  // ttext1c->Draw();
  
  // TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  // ttext1d->SetTextFont(42);
  // ttext1d->SetTextSize(0.04);
  // ttext1d->SetTextAlign(22);
  // ttext1d->Draw();

  // //
  // //
  // //
  // //catLeg1->Draw();
  // //catLeg1b->Draw();
  // catLeg1c->Draw();

  // sprintf(tempname,"searchBin_UncertaintySummary2c.png");
  // tc2c->SaveAs(tempname);
  // sprintf(tempname,"searchBin_UncertaintySummary2c.pdf");
  // tc2c->SaveAs(tempname);
    
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  //

  //
  // Store outputs
  //
  sprintf(tempname,"HadTauEstimation_data_formatted.root");
  TFile HadTauEstimation_OutputFile(tempname,"RECREATE");
  
  //
  searchBin_nominal->Write();
  searchBin_nominal_fullstatuncertainty->Write();
  searchBin_StatUncertainties->Write();

  // Note: The lines below are used when the systematics are still under evaluation, and just want to give dummy numbers
  // Closure & Bmistag
  //TH1D* searchBin_closureUncertainty = static_cast<TH1D*>(searchBin_box->Clone("searchBin_closureUncertainty"));
  //TH1D* searchBin_BMistagUp = static_cast<TH1D*>(searchBin_box->Clone("searchBin_BMistagUp"));
  //TH1D* searchBin_BMistagDn = static_cast<TH1D*>(searchBin_box->Clone("searchBin_BMistagDn"));
  // Mu
  //TH1D* searchBin_MuRecoSysUp = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuRecoSysUp"));
  //TH1D* searchBin_MuRecoSysDn = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuRecoSysDn"));
  //TH1D* searchBin_MuIsoSysUp = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuIsoSysUp"));
  //TH1D* searchBin_MuIsoSysDn = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuIsoSysDn"));
  //TH1D* searchBin_MuRecoIsoUp = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuRecoIsoUp"));
  //TH1D* searchBin_MuRecoIsoDn = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuRecoIsoDn"));
  // JEC
  //TH1D* searchBin_JECSysUp = static_cast<TH1D*>(searchBin_box->Clone("searchBin_JECSysUp"));
  //TH1D* searchBin_JECSysDn = static_cast<TH1D*>(searchBin_box->Clone("searchBin_JECSysDn"));
  // MT
  //TH1D* searchBin_MTSysUp = static_cast<TH1D*>(searchBin_one->Clone("searchBin_MTSysUp"));
  //TH1D* searchBin_MTSysDn = static_cast<TH1D*>(searchBin_one->Clone("searchBin_MTSysDn"));
  //TH1D* searchBin_MtEffStat = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MtEffStat"));
  // Tsotrack
  //TH1D* searchBin_IsoTrkVetoEffUncertaintyStat = static_cast<TH1D*>(searchBin_box->Clone("searchBin_IsoTrkVetoEffUncertaintyStat"));
  //TH1D* searchBin_IsoTrkVetoEffUncertaintySys = static_cast<TH1D*>(searchBin_box->Clone("searchBin_IsoTrkVetoEffUncertaintySys"));
  // Acc
  //TH1D* searchBin_AccStat = static_cast<TH1D*>(searchBin_box->Clone("searchBin_AccStat"));
  //TH1D* searchBin_AccSysPDFUp = static_cast<TH1D*>(searchBin_one->Clone("searchBin_AccSysPDFUp"));
  //TH1D* searchBin_AccSysPDFDn = static_cast<TH1D*>(searchBin_one->Clone("searchBin_AccSysPDFDn"));
  //TH1D* searchBin_AccSysScaleUp = static_cast<TH1D*>(searchBin_one->Clone("searchBin_AccSysScaleUp"));
  //TH1D* searchBin_AccSysScaleDn = static_cast<TH1D*>(searchBin_one->Clone("searchBin_AccSysScaleDn"));
  // MuFromTau
  //TH1D* searchBin_MuFromTauStat = static_cast<TH1D*>(searchBin_box->Clone("searchBin_MuFromTauStat"));
  // Dilepton
  //TH1D* searchBin_DileptonUncertainty = static_cast<TH1D*>(searchBin_box->Clone("searchBin_DileptonUncertainty"));  
  // Trigger
  //TH1D* searchBin_TrigEffUncertainty = static_cast<TH1D*>(searchBin_box->Clone("searchBin_TrigEffUncertainty"));
    
  searchBin_closureUncertainty->Add(searchBin_one);       searchBin_closureUncertainty->Write();
  searchBin_closureUncertainty_adhoc->Add(searchBin_one); searchBin_closureUncertainty_adhoc->Write();
  searchBin_BMistagUp->Add(searchBin_one);                searchBin_BMistagUp->Write();
  searchBin_BMistagDn->Add(searchBin_one);                searchBin_BMistagDn->Write();
  searchBin_MuRecoSysUp->Add(searchBin_one);              searchBin_MuRecoSysUp->Write();
  searchBin_MuRecoSysDn->Add(searchBin_one);              searchBin_MuRecoSysDn->Write();
  searchBin_MuIsoSysUp->Add(searchBin_one);               searchBin_MuIsoSysUp->Write();
  searchBin_MuIsoSysDn->Add(searchBin_one);               searchBin_MuIsoSysDn->Write();
  searchBin_MuRecoIsoUp->Add(searchBin_one);              searchBin_MuRecoIsoUp->Write();
  searchBin_MuRecoIsoDn->Add(searchBin_one);              searchBin_MuRecoIsoDn->Write();
  searchBin_JECSysUp->Add(searchBin_one);                 searchBin_JECSysUp->Write("searchBin_JECSysUp");
  searchBin_JECSysDn->Add(searchBin_one);                 searchBin_JECSysDn->Write("searchBin_JECSysDn");
  searchBin_MTSysUp->SetName("searchBin_MTSysUp");
  searchBin_MTSysDn->SetName("searchBin_MTSysDn");
  searchBin_MTSysUp->Write();
  searchBin_MTSysDn->Write();
  searchBin_MtEffStat->Add(searchBin_one);                searchBin_MtEffStat->Write();  
  searchBin_IsoTrkVetoEffUncertaintyStat->Add(searchBin_one); searchBin_IsoTrkVetoEffUncertaintyStat->Write();
  searchBin_IsoTrkVetoEffUncertaintySys->Add(searchBin_one);  searchBin_IsoTrkVetoEffUncertaintySys->Write();
  searchBin_AccStat->Add(searchBin_one);                  searchBin_AccStat->Write();
  searchBin_AccSysPDFUp->Write();
  searchBin_AccSysPDFDn->Write();
  searchBin_AccSysScaleUp->Write();
  searchBin_AccSysScaleDn->Write();
  searchBin_MuFromTauStat->Add(searchBin_one);            searchBin_MuFromTauStat->Write();
  searchBin_DileptonUncertainty->Add(searchBin_one);      searchBin_DileptonUncertainty->Write();
  searchBin_TrigEffUncertainty->Add(searchBin_one);       searchBin_TrigEffUncertainty->Write();

  // printMaxMin(searchBin_BMistagUp);
  // printMaxMin(searchBin_BMistagDn);

  // printMaxMin(searchBin_JECSysUp);
  // printMaxMin(searchBin_JECSysDn);

  // printMaxMin(searchBin_MuRecoSysUp);
  // printMaxMin(searchBin_MuRecoSysDn);
  // printMaxMin(searchBin_MuIsoSysUp);
  // printMaxMin(searchBin_MuIsoSysDn);
  // printMaxMin(searchBin_MuRecoIsoUp);
  // printMaxMin(searchBin_MuRecoIsoDn);

  // printMaxMin(searchBin_IsoTrkVetoEffUncertaintyStat);
  // printMaxMin(searchBin_IsoTrkVetoEffUncertaintySys);

  // printMaxMin(searchBin_MTSysUp);
  // printMaxMin(searchBin_MTSysDn);
  // printMaxMin(searchBin_MtEffStat);

  // printMaxMin(searchBin_AccStat);
  // printMaxMin(searchBin_AccSysPDFUp);
  // printMaxMin(searchBin_AccSysPDFDn);

  // printMaxMin(searchBin_MuFromTauStat);
  // printMaxMin(searchBin_DileptonUncertainty);
  // printMaxMin(searchBin_TrigEffUncertainty);
  // //TH1D* QCDBin_HiDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFUp");
  // //TH1D* QCDBin_HiDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFDn");
  // //TH1D* QCDBin_LowDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFUp");
  // //TH1D* QCDBin_LowDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFDn");

  // //
  // hPredHTMHT0b_nominal->Write();
  // hPredHTMHTwb_nominal->Write();
  // hPredNJetBins_nominal->Write();
  // hPredNbBins_nominal->Write();

  //
  QCDBin_HiDphi_nominal->Write();
  QCDBin_HiDphi_nominal_fullstatuncertainty->Write();
  QCDBin_HiDphi_StatUncertainties->Write();

  // Note: The lines below are used when the systematics are still under evaluation, and just want to give dummy numbers
  // Closure & Bmistag
  //TH1D* QCDBin_HiDphi_closureUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_closureUncertainty"));
  //TH1D* QCDBin_HiDphi_BMistagUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_BMistagUp"));
  //TH1D* QCDBin_HiDphi_BMistagDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_BMistagDn"));
  // Mu
  //TH1D* QCDBin_HiDphi_MuRecoSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuRecoSysUp"));
  //TH1D* QCDBin_HiDphi_MuRecoSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuRecoSysDn"));
  //TH1D* QCDBin_HiDphi_MuIsoSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuIsoSysUp"));
  //TH1D* QCDBin_HiDphi_MuIsoSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuIsoSysDn"));
  //TH1D* QCDBin_HiDphi_MuRecoIsoUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuRecoIsoUp"));
  //TH1D* QCDBin_HiDphi_MuRecoIsoDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuRecoIsoDn"));
  // JEC
  //TH1D* QCDBin_HiDphi_JECSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_JECSysUp"));
  //TH1D* QCDBin_HiDphi_JECSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_JECSysDn"));
  // MT
  //TH1D* QCDBin_HiDphi_MTSysUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_MTSysUp"));
  //TH1D* QCDBin_HiDphi_MTSysDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_MTSysDn"));
  //TH1D* QCDBin_HiDphi_MtEffStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MtEffStat"));
  // Tsotrack
  //TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat"));
  //TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys"));
  // Acc
  //TH1D* QCDBin_HiDphi_AccStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_AccStat"));
  //TH1D* QCDBin_HiDphi_AccSysPDFUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_AccSysPDFUp"));
  //TH1D* QCDBin_HiDphi_AccSysPDFDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_AccSysPDFDn"));
  //TH1D* QCDBin_HiDphi_AccSysScaleUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_AccSysScaleUp"));
  //TH1D* QCDBin_HiDphi_AccSysScaleDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_HiDphi_AccSysScaleDn"));
  // MuFromTau
  //TH1D* QCDBin_HiDphi_MuFromTauStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_MuFromTauStat"));
  // Dilepton
  //TH1D* QCDBin_HiDphi_DileptonUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_DileptonUncertainty"));  
  // Trigger
  //TH1D* QCDBin_HiDphi_TrigEffUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_HiDphi_TrigEffUncertainty"));

  QCDBin_HiDphi_closureUncertainty->Add(QCDBin_one);       QCDBin_HiDphi_closureUncertainty->Write();
  QCDBin_HiDphi_closureUncertainty_adhoc->Add(QCDBin_one); QCDBin_HiDphi_closureUncertainty_adhoc->Write();
  QCDBin_HiDphi_BMistagUp->Add(QCDBin_one);                QCDBin_HiDphi_BMistagUp->Write();
  QCDBin_HiDphi_BMistagDn->Add(QCDBin_one);                QCDBin_HiDphi_BMistagDn->Write();
  QCDBin_HiDphi_MuRecoSysUp->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoSysUp->Write();
  QCDBin_HiDphi_MuRecoSysDn->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoSysDn->Write();
  QCDBin_HiDphi_MuIsoSysUp->Add(QCDBin_one);               QCDBin_HiDphi_MuIsoSysUp->Write();
  QCDBin_HiDphi_MuIsoSysDn->Add(QCDBin_one);               QCDBin_HiDphi_MuIsoSysDn->Write();
  QCDBin_HiDphi_MuRecoIsoUp->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoIsoUp->Write();
  QCDBin_HiDphi_MuRecoIsoDn->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoIsoDn->Write();
  QCDBin_HiDphi_JECSysUp->Add(QCDBin_one);                 QCDBin_HiDphi_JECSysUp->Write("QCDBin_HiDphi_JECSysUp");
  QCDBin_HiDphi_JECSysDn->Add(QCDBin_one);                 QCDBin_HiDphi_JECSysDn->Write("QCDBin_HiDphi_JECSysDn");
  QCDBin_HiDphi_MTSysUp->Write("QCDBin_HiDphi_MTSysUp");
  QCDBin_HiDphi_MTSysDn->Write("QCDBin_HiDphi_MTSysDn");
  QCDBin_HiDphi_MtEffStat->Add(QCDBin_one);                QCDBin_HiDphi_MtEffStat->Write();  
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_HiDphi_AccStat->Add(QCDBin_one);                  QCDBin_HiDphi_AccStat->Write();
  QCDBin_HiDphi_AccSysPDFUp->Write();
  QCDBin_HiDphi_AccSysPDFDn->Write();
  QCDBin_HiDphi_AccSysScaleUp->Write();
  QCDBin_HiDphi_AccSysScaleDn->Write();
  QCDBin_HiDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_HiDphi_MuFromTauStat->Write();
  QCDBin_HiDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_HiDphi_DileptonUncertainty->Write();
  QCDBin_HiDphi_TrigEffUncertainty->Add(QCDBin_one);       QCDBin_HiDphi_TrigEffUncertainty->Write();

  //
  QCDBin_LowDphi_nominal->Write();
  QCDBin_LowDphi_nominal_fullstatuncertainty->Write();
  QCDBin_LowDphi_StatUncertainties->Write();

  // Note: The lines below are used when the systematics are still under evaluation, and just want to give dummy numbers
  // Closure & Bmistag
  //TH1D* QCDBin_LowDphi_closureUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_closureUncertainty"));
//  TH1D* QCDBin_LowDphi_BMistagUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_BMistagUp"));
//  TH1D* QCDBin_LowDphi_BMistagDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_BMistagDn"));
//  // Mu
//  TH1D* QCDBin_LowDphi_MuRecoSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuRecoSysUp"));
//  TH1D* QCDBin_LowDphi_MuRecoSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuRecoSysDn"));
//  TH1D* QCDBin_LowDphi_MuIsoSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuIsoSysUp"));
//  TH1D* QCDBin_LowDphi_MuIsoSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuIsoSysDn"));
//  TH1D* QCDBin_LowDphi_MuRecoIsoUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuRecoIsoUp"));
//  TH1D* QCDBin_LowDphi_MuRecoIsoDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuRecoIsoDn"));
//  // JEC
  //TH1D* QCDBin_LowDphi_JECSysUp = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_JECSysUp"));
  //TH1D* QCDBin_LowDphi_JECSysDn = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_JECSysDn"));
  // MT
  //TH1D* QCDBin_LowDphi_MTSysUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_MTSysUp"));
  //TH1D* QCDBin_LowDphi_MTSysDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_MTSysDn"));
  //TH1D* QCDBin_LowDphi_MtEffStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MtEffStat"));
  // Tsotrack
  //TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat"));
  //TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys"));
  // Acc
  //TH1D* QCDBin_LowDphi_AccStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_AccStat"));
  //TH1D* QCDBin_LowDphi_AccSysPDFUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_AccSysPDFUp"));
  //TH1D* QCDBin_LowDphi_AccSysPDFDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_AccSysPDFDn"));
  //TH1D* QCDBin_LowDphi_AccSysScaleUp = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_AccSysScaleUp"));
  //TH1D* QCDBin_LowDphi_AccSysScaleDn = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_LowDphi_AccSysScaleDn"));
  // MuFromTau
  //TH1D* QCDBin_LowDphi_MuFromTauStat = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_MuFromTauStat"));
  // Dilepton
  //TH1D* QCDBin_LowDphi_DileptonUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_DileptonUncertainty"));  
  // Trigger
  //TH1D* QCDBin_LowDphi_TrigEffUncertainty = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_LowDphi_TrigEffUncertainty"));

  QCDBin_LowDphi_closureUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_closureUncertainty->Write();
  QCDBin_LowDphi_closureUncertainty_adhoc->Add(QCDBin_one);QCDBin_LowDphi_closureUncertainty_adhoc->Write();
  QCDBin_LowDphi_BMistagUp->Add(QCDBin_one);               QCDBin_LowDphi_BMistagUp->Write();
  QCDBin_LowDphi_BMistagDn->Add(QCDBin_one);               QCDBin_LowDphi_BMistagDn->Write();
  QCDBin_LowDphi_MuRecoSysUp->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoSysUp->Write();
  QCDBin_LowDphi_MuRecoSysDn->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoSysDn->Write();
  QCDBin_LowDphi_MuIsoSysUp->Add(QCDBin_one);              QCDBin_LowDphi_MuIsoSysUp->Write();
  QCDBin_LowDphi_MuIsoSysDn->Add(QCDBin_one);              QCDBin_LowDphi_MuIsoSysDn->Write();
  QCDBin_LowDphi_MuRecoIsoUp->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoIsoUp->Write();
  QCDBin_LowDphi_MuRecoIsoDn->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoIsoDn->Write();
  QCDBin_LowDphi_JECSysUp->Add(QCDBin_one);                QCDBin_LowDphi_JECSysUp->Write("QCDBin_LowDphi_JECSysUp");
  QCDBin_LowDphi_JECSysDn->Add(QCDBin_one);                QCDBin_LowDphi_JECSysDn->Write("QCDBin_LowDphi_JECSysDn");
  QCDBin_LowDphi_MTSysUp->Write("QCDBin_LowDphi_MTSysUp");
  QCDBin_LowDphi_MTSysDn->Write("QCDBin_LowDphi_MTSysDn");
  QCDBin_LowDphi_MtEffStat->Add(QCDBin_one);                QCDBin_LowDphi_MtEffStat->Write();  
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_LowDphi_AccStat->Add(QCDBin_one);                  QCDBin_LowDphi_AccStat->Write();
  QCDBin_LowDphi_AccSysPDFUp->Write();
  QCDBin_LowDphi_AccSysPDFDn->Write();
  QCDBin_LowDphi_AccSysScaleUp->Write();
  QCDBin_LowDphi_AccSysScaleDn->Write();
  QCDBin_LowDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_LowDphi_MuFromTauStat->Write();
  QCDBin_LowDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_DileptonUncertainty->Write();
  QCDBin_LowDphi_TrigEffUncertainty->Add(QCDBin_one);       QCDBin_LowDphi_TrigEffUncertainty->Write();

  HadTauEstimation_OutputFile.Close();

}

// ----------
void printMaxMin(TH1* input){

  double max=0.;
  double min=100.;
  int    ient=0;
  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    double content = fabs(input->GetBinContent(ibin+1)-1.);
    if (content>max) max=content; 
    if (content<min) min=content; 
  }
  //std::cout << input->GetTitle() << std::endl;
  std::cout << input->GetName() << std::endl;
  printf("max: %8.3f, min: %8.3f\n",max,min);
  //sum = sum/double(ient);
  //std::cout << sum << std::endl;
  //for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
  //  sys->SetBinContent(ibin+1,sum);
  //}

};

// ----------
void takeAverage(TH1* sys){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  double sum=0.;
  int    ient=0;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    sum  += input_sys->GetBinContent(ibin+1);
    ient ++;
  }
  sum = sum/double(ient);
  //std::cout << sum << std::endl;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    sys->SetBinContent(ibin+1,sum);
  }

};

// ----------
void takeDiffForSys(TH1* sys, TH1* input_nominal){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  Float_t diff;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    if (input_nominal->GetBinContent(ibin+1)){
      diff = float((input_sys->GetBinContent(ibin+1)-input_nominal->GetBinContent(ibin+1))
		   /input_nominal->GetBinContent(ibin+1));
      sys->SetBinContent(ibin+1,diff);
    }
  }
  
}

// ----------
void accErrPropagation(TH1* sys, TH1* input_nominal){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  Float_t diff;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    if (input_nominal->GetBinContent(ibin+1)){
      /* */
      diff = (input_nominal->GetBinContent(ibin+1)) /
	(input_nominal->GetBinContent(ibin+1) + input_sys->GetBinContent(ibin+1));
      /* */
      //diff=input_sys->GetBinContent(ibin+1);
      sys->SetBinContent(ibin+1,diff);
    }
  }
  
}

// ----------
void effMapStatErrPropagation(TH1* input, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinError(ibin+1)/input->GetBinContent(ibin+1));
  }
  //output->Divide(input);  

}
// ----------
void effMapConstErrPropagation(TH1* input, double fractionalError, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinContent(ibin+1)/(input->GetBinContent(ibin+1)*(1.+fractionalError)));
  }
  //output->Divide(input);  

}

// ----------
void muFromTauStatErrPropagation(TH1* input, TH1* one, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinError(ibin+1)/(1.-input->GetBinContent(ibin+1)));
  }
  //input->Add(one,-1.);    // (f-1.) ~ -0.7
  //input->Scale(-1.);      // (1-f) ~ 0.7
  //output->Divide(input);  // sigma(f)/(1-f)

}

// ----------
void isoTrkVetoErrPropagation(TH1* input, double syserr, TH1* one,
			      TH1* output_tot, TH1* output_stat, TH1* output_sys){

  //
  // Apply a constant systematic error on the fraction of rejected events
  // Also, the statistical uncertainty is propagated
  //

  output_tot->Reset();
  output_stat->Reset();
  output_sys->Reset();

  for (int ibin=0;ibin<output_sys->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1)){      
      output_sys->SetBinContent(ibin+1,(1.-input->GetBinContent(ibin+1))*syserr/input->GetBinContent(ibin+1));
      output_stat->SetBinContent(ibin+1,input->GetBinError(ibin+1)/input->GetBinContent(ibin+1));
      double unc = pow(pow(output_sys->GetBinContent(ibin+1),2)
		       + pow(output_stat->GetBinError(ibin+1),2),0.5);      
      output_tot->SetBinContent(ibin+1,unc);      
    }
  }

  /*
  output_sys->Add(one,-1.);  // (eff-1.) ~ -0.4
  output_sys->Scale(-1.);    // (1-eff) ~ 0.4
  output_sys->Scale(syserr); // (1-eff)*0.1 ~ 0.04, i.e. 10% of the (1-eff)
  for (int ibin=0;ibin<output_tot->GetNbinsX();ibin++){
    double unc = pow(pow(output_sys->GetBinContent(ibin+1),2)
		     + pow(input->GetBinError(ibin+1),2),0.5);
    output_tot->SetBinContent(ibin+1,unc);
    output_stat->SetBinContent(ibin+1,input->GetBinError(ibin+1));
  }
  output_sys->Divide(input);   // (1-eff)*0.1/eff
  output_stat->Divide(input);  // sigma(eff)/eff
  output_tot->Divide(input);   // 
  */

}

// ----------
void binMap(TH1* input, TH1* output){
  //
  // Map from 11*3+6*2=45 binning to 72 binning
  //
  // This involves two steps
  // 1: 45 binning to 6*3=18 bins
  // 2: 18 bins to 18*4=72 bins

  double noB[18]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noBe[18]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noB_ent[18]
    ={6,6,6,6,3,6,
      1,1,1,1,1,1,
      1,1,1,1,1,1};
  int map_input[45] // which 72/4=18 bins this 45 efficiency binning corresponds to?
    ={1,2,3,1,2,3,4,4,5,6,6,
      1,2,3,1,2,3,4,4,5,6,6,
      1,2,3,1,2,3,4,4,5,6,6,
       7, 8, 9,10,11,12,
      13,14,15,16,17,18};

  int map_output[72]
    ={1,2,3,4,5,6,
      1,2,3,4,5,6,
      1,2,3,4,5,6,
      1,2,3,4,5,6,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      13,14,15,16,17,18,
      13,14,15,16,17,18,
      13,14,15,16,17,18,
      13,14,15,16,17,18};

  // 1: 45 binning to 6*3=18 bins
  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
  }
  
  // 2: 18 bins to 18*4=72 bins
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_ICHEP2016(TH1* input, TH1* output){
  //
  // Map from 12*6=72 binning to 4*4*10=160 binning
  //
  // This involves two steps
  // 1: 72 binning to 12*4= 48  bins
  // 2: 48 bins    to 40*4=160 bins
  std::cout<<" binMap_ICHEP2016 "<< endl;

  double noB[48]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noBe[48]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noB_ent[48]
    ={2,2,2,2,2,2,2,2,2,2,2,2,
      2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1};
  int map_input[72] // which 48 bins this 72 efficiency binning corresponds to?
    ={ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,  
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,
      13,14,15,16,17,18,19,20,21,22,23,24,
      13,14,15,16,17,18,19,20,21,22,23,24,
      25,26,27,28,29,30,31,32,33,34,35,36,
      37,38,39,40,41,42,43,44,45,46,47,48};

  int map_output[160]
    ={ 4, 5, 6, 7, 8, 9,10,11,12,12,
       4, 5, 6, 7, 8, 9,10,11,12,12,
       4, 5, 6, 7, 8, 9,10,11,12,12,
       4, 5, 6, 7, 8, 9,10,11,12,12,
      16,17,18,19,20,21,22,23,24,24,
      16,17,18,19,20,21,22,23,24,24,
      16,17,18,19,20,21,22,23,24,24,
      16,17,18,19,20,21,22,23,24,24,
      28,29,30,31,32,33,34,35,36,36,
      28,29,30,31,32,33,34,35,36,36,
      28,29,30,31,32,33,34,35,36,36,
      28,29,30,31,32,33,34,35,36,36,
      40,41,42,43,44,45,46,47,48,48,
      40,41,42,43,44,45,46,47,48,48,
      40,41,42,43,44,45,46,47,48,48,
      40,41,42,43,44,45,46,47,48,48};
      
  // 1: 72 binning to 9*4=48 bins
  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){ 
    std::cout<<" ibin "<<ibin<<" map_input[ibin-1] "<< map_input[ibin-1]<<" input_GetBinContent"<<input->GetBinContent(ibin)<< " noB_ent[map_input[ibin-1]-1] "<< noB_ent[map_input[ibin-1]-1]<<endl; 
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
  }

  for(int j=0;j<48;j++){
    std::cout<< " j "<< j<<" noB[j] "<<noB[j]<<endl;
  }
  
  // 2: 48 bins to 40*4=160 bins
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    std::cout<<" ibin "<<ibin<<" map_output[ibin-1] "<< map_output[ibin-1]<< " noB[map_output[ibin-1]-1] "<< noB[map_output[ibin-1]-1]<<endl;
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_QCD(TH1* input, TH1* output){
  //
  // Map from 11*3+6*2=45 binning to 44*5 binning
  //

  double noB[45]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noBe[45]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};

  int map_output[220]
  ={1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    12,13,14,15,16,17,18,19,20,21,22, // 12-22,
    12,13,14,15,16,17,18,19,20,21,22, // 
    12,13,14,15,16,17,18,19,20,21,22, // 
    12,13,14,15,16,17,18,19,20,21,22, // 
    23,24,25,26,27,28,29,30,31,32,33, // 23-33,
    23,24,25,26,27,28,29,30,31,32,33, // 
    23,24,25,26,27,28,29,30,31,32,33, // 
    23,24,25,26,27,28,29,30,31,32,33, // 
    34,35,36,34,35,36,37,37,38,39,39, // 34-39
    34,35,36,34,35,36,37,37,38,39,39, //
    34,35,36,34,35,36,37,37,38,39,39, //
    34,35,36,34,35,36,37,37,38,39,39, //
    40,41,42,40,41,42,43,43,44,45,45, // 40-45
    40,41,42,40,41,42,43,43,44,45,45, //
    40,41,42,40,41,42,43,43,44,45,45, //
    40,41,42,40,41,42,43,43,44,45,45  //
  };

  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    noB[ibin-1]  = input->GetBinContent(ibin);
    noBe[ibin-1] = input->GetBinError(ibin);
  }

  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_QCD_ICHEP2016(TH1* input, TH1* output){
  //
  // Map from 12*6=72 binning to 4*4*13=208 binning
  //
  // This involves two steps
  // 1: 72 binning to 12*4= 48  bins
  // 2: 48 bins    to 13*4*4=208 bins
  std::cout<<" binMap_QCD_ICHEP2016 "<< endl;
  double noB[48]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noBe[48]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noB_ent[48]
    ={2,2,2,2,2,2,2,2,2,2,2,2,
      2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1};
  int map_input[72] // which 48 bins this 72 efficiency binning corresponds to?
    ={ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,  
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,
      13,14,15,16,17,18,19,20,21,22,23,24,
      13,14,15,16,17,18,19,20,21,22,23,24,
      25,26,27,28,29,30,31,32,33,34,35,36,
      37,38,39,40,41,42,43,44,45,46,47,48};

  int map_output[208]
    ={ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
      13,14,15,16,17,18,19,20,21,22,23,24,24,
      13,14,15,16,17,18,19,20,21,22,23,24,24,
      13,14,15,16,17,18,19,20,21,22,23,24,24,
      13,14,15,16,17,18,19,20,21,22,23,24,24,
      25,26,27,28,29,30,31,32,33,34,35,36,36,
      25,26,27,28,29,30,31,32,33,34,35,36,36,
      25,26,27,28,29,30,31,32,33,34,35,36,36,
      25,26,27,28,29,30,31,32,33,34,35,36,36,
      37,38,39,40,41,42,43,44,45,46,47,48,48,
      37,38,39,40,41,42,43,44,45,46,47,48,48,
      37,38,39,40,41,42,43,44,45,46,47,48,48,
      37,38,39,40,41,42,43,44,45,46,47,48,48};
      
  // 1: 72 binning to 9*4=48 bins
  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    std::cout<<" ibin "<<ibin<<" map_input[ibin-1] "<< map_input[ibin-1]<<" input_GetBinContent"<<input->GetBinContent(ibin)<< " noB_ent[map_input[ibin-1]-1] "<< noB_ent[map_input[ibin-1]-1]<<endl; 
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
  }
  for(int j=0;j<48;j++){
    std::cout<< " j "<< j<<" noB[j] "<<noB[j]<<endl;
  }
  
  // 2: 48 bins to 13*4*4=208 bins
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    std::cout<<" ibin "<<ibin<<" map_output[ibin-1] "<< map_output[ibin-1]<< " noB[map_output[ibin-1]-1] "<< noB[map_output[ibin-1]-1]<<endl;
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

void combineHighLowHT_searchBin(TH1* default, TH1* lowHT){

  for (int ibin=1; ibin<=default->GetNbinsX(); ibin++){    
    //std::cout << ibin%10 << std::endl;
    if (ibin%10==1 || ibin%10==4 ){
    default->SetBinContent(ibin,lowHT->GetBinContent(ibin));
    default->SetBinError(ibin,lowHT->GetBinError(ibin));
    }
  }

};

void combineHighLowHT_QCDBin(TH1* default, TH1* lowHT){

  for (int ibin=1; ibin<=default->GetNbinsX(); ibin++){    
    //std::cout << ibin%13 << std::endl;
    if (ibin%13==1 || ibin%13==4 || ibin%13==7 ){
    default->SetBinContent(ibin,lowHT->GetBinContent(ibin));
    default->SetBinError(ibin,lowHT->GetBinError(ibin));
    }
  }

};

void makeNjNbCorr_searchBin(TH1* hist){

  /*
  double NjNbCorr[16]={
  1.08678, 1.1606, 1.21688, 1.38799,
  0.99422, 1.03512, 1.09677, 1.15551,
  0.960125, 0.99264, 1.04157, 1.22838,
  0.863833, 0.803388, 1.01109, 1.12302};
  */

  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    int index=int((ibin-1)/10);
    //std::cout << index << std::endl;
    double corr = NjNbCorr[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void makeNjNbCorr_QCDBin_HiDphi(TH1* hist){

  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    int index=int((ibin-1)/13);
    //std::cout << index << std::endl;
    double corr = NjNbCorr_QCDHDP[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void makeNjNbCorr_QCDBin_LowDphi(TH1* hist){

  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    int index=int((ibin-1)/13);
    //std::cout << index << std::endl;
    double corr = NjNbCorr_QCDLDP[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void reformat(TH1* input, TH1* output){

  char tempname[200];  

  //std::cout << input->GetBinLowEdge(1) << std::endl;
  if (input->GetBinLowEdge(1)==1) {
    output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  } else {
    output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1),input->GetBinLowEdge(input->GetNbinsX()+1));
  }

  for (int ibin=1;ibin<=input->GetNbinsX();ibin++){    
    output->SetBinContent(ibin,input->GetBinContent(ibin));    
    output->SetBinError(ibin,input->GetBinError(ibin));    

  }

  // Search Binning
  if(output->GetNbinsX()==160)
    for(int nji = 0; nji<4; ++nji){
      for(int nbi = 0; nbi<4; ++nbi){
	for(int hti = 0; hti<10; ++hti){
	  int mhti =0;
	  if(hti >=0 && hti <=2) mhti = 0;
	  else if(hti >=3 && hti <=5) mhti = 1;
	  else if(hti >=6 && hti <=7) mhti = 2;
	  else mhti = 3;
	  char binlabel[100];
	  int bi = (nji) * 40 + nbi * 10 + hti + 1;
	  sprintf(binlabel, "NJets%d-BTags%d-MHT%d-HT%d", nji, nbi, mhti, hti);
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	}
      }
    }

  // QCD Binning
  if(output->GetNbinsX()==208)
    for(int nji = 0; nji<4; ++nji){
      for(int nbi = 0; nbi<4; ++nbi){
	for(int hti = 0; hti<13; ++hti){
	  int mhti =0;
	  if(hti >=0 && hti <=2) mhti = -1;
	  else if(hti >=3 && hti <=5) mhti = 0;
	  else if(hti >=6 && hti <=8) mhti = 1;
	  else if(hti >=9 && hti <=10) mhti = 2;
	  else mhti = 3;
	  char binlabel[100];
	  int bi = (nji) * 52 + nbi * 13 + hti + 1;
	  if(mhti < 0)  sprintf(binlabel, "NJets%d-BTags%d-MHTC-HT%d", nji, nbi, hti);
	  else sprintf(binlabel, "NJets%d-BTags%d-MHT%d-HT%d", nji, nbi, mhti, hti-3);
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	}
      }
    }
  
  output -> GetXaxis() -> LabelsOption("v");
  
};
