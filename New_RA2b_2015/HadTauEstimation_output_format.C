/*
  root.exe -b -q HadTauEstimation_output_format.C 
*/

void binMap(TH1* input, TH1* output);
void binMap_QCD(TH1* input, TH1* output);
void takeDiffForSys(TH1* sys, TH1* input_nominal);
void effMapStatErrPropagation(TH1* input, TH1* output);
void muFromTauStatErrPropagation(TH1* input, TH1* one, TH1* output);
void isoTrkVetoErrPropagation(TH1* input, double sys, TH1* one, TH1* output_tot, TH1* output_stat, TH1* output_sys);

void HadTauEstimation_output_format(string elogForData="Elog410_",     // Data
				    string elogForMCExp="Elog404_",    // MC expectation
				    string elogForMCPre="Elog408_",    // MC prediction
				    string elogForSys="Elog408_",      // MC-based systematics evaluation for Btag and muon efficiencies
				    string elogForMTSysUp="Elog402_",  // MT systematics
				    string elogForMTSysDn="Elog403_",
				    string elogForMTSysRef="Elog397_",
				    string elogForJECSysUp="Elog388_",   // JEC 
				    string elogForJECSysDn="Elog389_",
				    string elogForJECSysRef="Elog387_",
				    string elogForIsoTrkVeto="Elog401_", // Isotrack veto
				    string elogForMuFromTau="Elog401_",  // Muon from tau
				    string elogForAccStat="Elog401_",    // Acceptance 
				    string elogForMTStat="Elog401_",     // MT cut efficiency
				    string elogForAccPDFScale="",
				    double trigEff=0.948,
				    //double lumiTarget=1.280231,
				    //double lumiControl=1.263886,
				    double lumiTarget=2.109271,
				    double lumiControl=2.093663,
				    int isys==0){

  char tempname[200];

  //
  // ----- Opening input files
  // -------------------------------

  //
  // Open data files
  //
  //sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v15cd_.root",elogForData.c_str());
  //sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v15d_TriggerOn.root",elogForData.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v15d_.root",elogForData.c_str());
  TFile *DataEstFile = TFile::Open(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Convenient utility histogram
  //
  TH1D* searchBin_one = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_one");
  searchBin_one->Reset();
  searchBin_one->SetLineColor(1);
  for (int ibin=0; ibin<searchBin_one->GetNbinsX(); ibin++){
    searchBin_one->SetBinContent(ibin+1,1.);
    searchBin_one->SetBinError(ibin+1,0.);
  }

  TH1D* searchBin_box = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_box");
  searchBin_box->Reset();
  searchBin_box->SetLineColor(0);

  TH1D* QCDBin_one = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_one");
  QCDBin_one->Reset();
  QCDBin_one->SetLineColor(1);
  for (int ibin=0; ibin<QCDBin_one->GetNbinsX(); ibin++){
    QCDBin_one->SetBinContent(ibin+1,1.);
    //QCDBin_one->SetBinError(ibin+1,0.);
  }

  TH1D* QCDBin_box = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_box");
  QCDBin_box->Reset();
  QCDBin_box->SetLineColor(0);

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
  ///home/borzou/CMSSW_7_4_0_pre1/src/RA2-RA2b-2015/New_RA2b_2015/TauHad2/HadTauEstimation_Elog408_ContainigStatandSysErrors.root
  sprintf(tempname,"TauHad2/HadTauEstimation_%sContainigStatandSysErrors.root",elogForSys.c_str());
  //sprintf(tempname,"TauHad2/%sHadTauEstimation_haddedToContainSysErrorFolders_.root",elogForSys.c_str());
  TFile * MCSysFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // MTcut eff
  //
  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForMTSysUp.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForMTSysUp.c_str(),sample.c_str());
  TFile * MTSysUpFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForMTSysDn.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForMTSysDn.c_str(),sample.c_str());
  TFile * MTSysDnFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForMTSysRef.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForMTSysRef.c_str(),sample.c_str());
  TFile * MTSysRefFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  //
  // JEC variation
  //
  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  TFile * JECSysUpFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  TFile * JECSysDnFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  TFile * JECSysRefFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Isotrack veto efficiency
  //
  sprintf(tempname,"TauHad/Stack/%sIsoEfficiencies_stacked.root",elogForIsoTrkVeto.c_str());
  TFile * IsoTrkVetoFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  TH1D* IsoTrkVetoEff = (TH1D*)IsoTrkVetoFile->Get("IsoEff")->Clone("IsoTrkVetoEff");
  TH1D* IsoTrkVetoEff_LowDphi = (TH1D*)IsoTrkVetoFile->Get("IsoEff_lowDphi")->Clone("IsoTrkVetoEff_LowDphi");

  TH1D* searchBin_IsoTrkVetoEff = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_IsoTrkVetoEff");
  searchBin_IsoTrkVetoEff->Reset();
  binMap(IsoTrkVetoEff,searchBin_IsoTrkVetoEff);

  TH1D* QCDBin_HiDphi_IsoTrkVetoEff = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_IsoTrkVetoEff");
  QCDBin_HiDphi_IsoTrkVetoEff->Reset();
  binMap_QCD(IsoTrkVetoEff,QCDBin_HiDphi_IsoTrkVetoEff);

  TH1D* QCDBin_LowDphi_IsoTrkVetoEff = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_IsoTrkVetoEff");
  QCDBin_LowDphi_IsoTrkVetoEff->Reset();
  binMap_QCD(IsoTrkVetoEff_LowDphi,QCDBin_LowDphi_IsoTrkVetoEff);

  TH1D* searchBin_IsoTrkVetoEffUncertaintyTot  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("seaerchBin_IsoTrkVetoEffUncertaintyTot");
  TH1D* searchBin_IsoTrkVetoEffUncertaintyStat = (TH1D*)searchBin_IsoTrkVetoEff->Clone("seaerchBin_IsoTrkVetoEffUncertaintyStat");
  TH1D* searchBin_IsoTrkVetoEffUncertaintySys  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("seaerchBin_IsoTrkVetoEffUncertaintySys");

  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys");

  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys");

  isoTrkVetoErrPropagation(searchBin_IsoTrkVetoEff, 0.1, searchBin_one,
			   searchBin_IsoTrkVetoEffUncertaintyTot, searchBin_IsoTrkVetoEffUncertaintyStat, searchBin_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_HiDphi_IsoTrkVetoEff, 0.1, QCDBin_one,
			   QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_LowDphi_IsoTrkVetoEff, 0.1, QCDBin_one,
			   QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys);

  //
  // Mu from tau
  // -----------
  sprintf(tempname,"TauHad2/Stack/%sProbability_Tau_mu_stacked.root",elogForMuFromTau.c_str());
  TFile * Prob_Tau_mu_file = new TFile(tempname,"R");
  sprintf(tempname,"hProb_Tau_mu");
  TH1D * hProb_Tau_mu =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();
  sprintf(tempname,"hProb_Tau_mu_lowDelphi");
  TH1D * hProb_Tau_mu_LowDphi =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();
  
  TH1D* searchBin_MuFromTau = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_MuFromTau");
  searchBin_MuFromTau->Reset();
  binMap(hProb_Tau_mu,searchBin_MuFromTau);

  TH1D* QCDBin_HiDphi_MuFromTau = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_MuFromTau");
  QCDBin_HiDphi_MuFromTau->Reset();
  binMap_QCD(hProb_Tau_mu,QCDBin_HiDphi_MuFromTau);

  TH1D* QCDBin_LowDphi_MuFromTau = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_MuFromTau");
  QCDBin_LowDphi_MuFromTau->Reset();
  binMap_QCD(hProb_Tau_mu_LowDphi,QCDBin_LowDphi_MuFromTau);

  TH1D* searchBin_MuFromTauStat  = (TH1D*)searchBin_MuFromTau->Clone("seaerchBin_MuFromTauStat");
  TH1D* QCDBin_HiDphi_MuFromTauStat  = (TH1D*)QCDBin_HiDphi_MuFromTau->Clone("QCDBin_HiDphi_MuFromTauStat");
  TH1D* QCDBin_LowDphi_MuFromTauStat = (TH1D*)QCDBin_LowDphi_MuFromTau->Clone("QCDBin_LowDphi_MuFromTauStat");

  muFromTauStatErrPropagation(searchBin_MuFromTau,searchBin_one,searchBin_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_HiDphi_MuFromTau, QCDBin_one,QCDBin_HiDphi_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_LowDphi_MuFromTau,QCDBin_one,QCDBin_LowDphi_MuFromTauStat);

  //
  // Acceptance
  // ----------
  sprintf(tempname,"TauHad/Stack/%sLostLepton2_MuonEfficienciesFromstacked.root",elogForAccStat.c_str());
  TFile * MuAcc_file = new TFile(tempname,"R");  
  sprintf(tempname,"hAcc");
  TH1D * hAcc =(TH1D *) MuAcc_file->Get(tempname)->Clone();
  sprintf(tempname,"hAcc_lowDphi");
  TH1D * hAcc_LowDphi =(TH1D *) MuAcc_file->Get(tempname)->Clone();

  TH1D* searchBin_Acc = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_Acc");
  searchBin_Acc->Reset();
  binMap(hAcc,searchBin_Acc);

  TH1D* QCDBin_HiDphi_Acc = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_Acc");
  QCDBin_HiDphi_Acc->Reset();
  binMap_QCD(hAcc,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_Acc = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_LowDphi_Acc");
  QCDBin_LowDphi_Acc->Reset();
  binMap_QCD(hAcc_LowDphi,QCDBin_LowDphi_Acc);

  //
  TH1D* searchBin_AccStat  = (TH1D*)searchBin_Acc->Clone("seaerchBin_AccStat");
  TH1D* QCDBin_HiDphi_AccStat  = (TH1D*)QCDBin_HiDphi_Acc->Clone("QCDBin_HiDphi_AccStat");
  TH1D* QCDBin_LowDphi_AccStat = (TH1D*)QCDBin_LowDphi_Acc->Clone("QCDBin_LowDphi_AccStat");

  effMapStatErrPropagation(searchBin_Acc,searchBin_AccStat);
  effMapStatErrPropagation(QCDBin_HiDphi_Acc, QCDBin_HiDphi_AccStat);
  effMapStatErrPropagation(QCDBin_LowDphi_Acc,QCDBin_LowDphi_AccStat);

  //
  // MT cut efficiency
  // -----------------
  sprintf(tempname,"TauHad2/%sMtEff.root",elogForMTStat.c_str());
  TFile * MtFile = new TFile(tempname,"R");
  TH1D * hMT = (TH1D *) MtFile->Get("MtCutEff")->Clone();
  TH1D * hMT_LowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();

  TH1D* searchBin_MtEff = (TH1D*)DataEstFile->Get("searchH_b")->Clone("seaerchBin_MtEff");
  searchBin_MtEff->Reset();
  binMap(hMT,searchBin_MtEff);

  TH1D* QCDBin_HiDphi_MtEff = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_MtEff");
  QCDBin_HiDphi_MtEff->Reset();
  binMap_QCD(hMT,QCDBin_HiDphi_MtEff);

  TH1D* QCDBin_LowDphi_MtEff = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_MtEff");
  QCDBin_LowDphi_MtEff->Reset();
  binMap_QCD(hMT_LowDphi,QCDBin_LowDphi_MtEff);

  //
  TH1D* searchBin_MtEffStat  = (TH1D*)searchBin_MtEff->Clone("seaerchBin_MtEffStat");
  TH1D* QCDBin_HiDphi_MtEffStat  = (TH1D*)QCDBin_HiDphi_MtEff->Clone("QCDBin_HiDphi_MtEffStat");
  TH1D* QCDBin_LowDphi_MtEffStat = (TH1D*)QCDBin_LowDphi_MtEff->Clone("QCDBin_LowDphi_MtEffStat");

  effMapStatErrPropagation(searchBin_MtEff,searchBin_MtEffStat);
  searchBin_MtEffStat->Print("all");
  effMapStatErrPropagation(QCDBin_HiDphi_MtEff, QCDBin_HiDphi_MtEffStat);
  effMapStatErrPropagation(QCDBin_LowDphi_MtEff,QCDBin_LowDphi_MtEffStat);

  //
  // Const uncertainty
  //
  double dilep = 0.02; 
  TH1D *searchBin_DileptonUncertainty = (TH1D*) searchBin_one->Clone("searchBin_DileptonUncertainty");
  searchBin_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_HiDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_HiDphi_DileptonUncertainty");
  QCDBin_HiDphi_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_LowDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_LowDphi_DileptonUncertainty");
  QCDBin_LowDphi_DileptonUncertainty->Scale(dilep);

  //
  // ---- Nominal predictions and stat uncertainties -----
  // -----------------------------------------------------

  //
  // ----- Normal 72 bin predicitons -----
  //

  TH1D* searchBin_nominal = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_nominal");
  TH2D* hWeightForSearchBin = (TH2D*)DataEstFile->Get("hWeight4ForSearchBin")->Clone("hWeightForSearchBin");
  //searchBin_nominal->Print();
  searchBin_nominal->Scale(1/trigEff*lumiTarget/lumiControl); 
  //searchBin_nominal->Print();
  TH1D* searchBin_nominal_fullstatuncertainty = (TH1D*)searchBin_nominal->Clone("searchBin_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<searchBin_nominal->GetNbinsX(); ibin++){
    searchBin_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(searchBin_nominal->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }

  //
  // ----- QCD bin predicitons -----
  //
  TH1D* QCDBin_HiDphi_nominal  = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_nominal");
  QCDBin_HiDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_HiDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_HiDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_HiDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_HiDphi_nominal->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }

  TH1D* QCDBin_LowDphi_nominal = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_nominal");
  QCDBin_LowDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_LowDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_LowDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_LowDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_LowDphi_nominal->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }

  // Some additional variables
  double searchBin_stat_uncertainty_fractional[73];
  double searchBin_uncertainty_correction_stats_up[73];
  double searchBin_uncertainty_correction_stats_dn[73];
  double searchBin_uncertainty_correction_stats[73];

  double QCDBin_HiDphi_stat_uncertainty_fractional[221];
  double QCDBin_HiDphi_uncertainty_correction_stats_up[221];
  double QCDBin_HiDphi_uncertainty_correction_stats_dn[221];
  double QCDBin_HiDphi_uncertainty_correction_stats[221];

  double QCDBin_LowDphi_stat_uncertainty_fractional[221];
  double QCDBin_LowDphi_uncertainty_correction_stats_up[221];
  double QCDBin_LowDphi_uncertainty_correction_stats_dn[221];
  double QCDBin_LowDphi_uncertainty_correction_stats[221];

  TH1D* searchBin_StatUncertainties = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_StatUncertainties");
  searchBin_StatUncertainties->Reset();
  TH1D* searchBin_StatUncertaintiesFractional = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_StatUncertaintiesFractional");
  searchBin_StatUncertaintiesFractional->Reset();

  TH1D* QCDBin_HiDphi_StatUncertainties = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_StatUncertainties");
  QCDBin_HiDphi_StatUncertainties->Reset();
  TH1D* QCDBin_HiDphi_StatUncertaintiesFractional = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_StatUncertaintiesFractional");
  QCDBin_HiDphi_StatUncertaintiesFractional->Reset();

  TH1D* QCDBin_LowDphi_StatUncertainties = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_LowDphi_StatUncertainties");
  QCDBin_LowDphi_StatUncertainties->Reset();
  TH1D* QCDBin_LowDphi_StatUncertaintiesFractional = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_LowDphi_StatUncertaintiesFractional");
  QCDBin_LowDphi_StatUncertaintiesFractional->Reset();

  //
  // ----- Stat uncertainties
  //

  for (int ibin=1;ibin<=72;ibin++){
    // Stat uncertainty
    searchBin_stat_uncertainty_fractional[ibin]=0.;
    if (searchBin_nominal->GetBinContent(ibin)!=0.){
      searchBin_StatUncertainties->SetBinContent(ibin,searchBin_nominal->GetBinError(ibin));
      searchBin_stat_uncertainty_fractional[ibin] = searchBin_nominal_fullstatuncertainty->GetBinError(ibin)/searchBin_nominal->GetBinContent(ibin);
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,searchBin_stat_uncertainty_fractional[ibin]);
    } else {
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
  }

  for (int ibin=1;ibin<=220;ibin++){
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
    hPredHTMHT0b_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHT0b_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
    hPredHTMHTwb_nominal->SetBinContent(ibin+1,hPredHTMHTwb_nominal_ABbins->GetBinContent(ibin+1));
    hPredHTMHTwb_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHTwb_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }
  for (int ibin=0; ibin<hPredNJetBins_nominal->GetNbinsX(); ibin++){
    hPredNJetBins_nominal->SetBinError(ibin+1,pow(pow(hPredNJetBins_nominal->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }
  for (int ibin=0; ibin<hPredNbBins_nominal->GetNbinsX(); ibin++){
    hPredNbBins_nominal->SetBinError(ibin+1,pow(pow(hPredNbBins_nominal->GetBinError(ibin+1),2)+pow(0.275,2),0.5));
  }

  //
  // ---- Closure systematics -----
  // -----------------------------------------------------

  //
  // From MC file
  //
  TH1D * GenHist, * EstHist, * thist;
  TH1D * histTemplate;
  THStack *tempstack;
  
  // For closure systematics
  string histname="searchH_b";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();

  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  TH1D * searchBin_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("searchBin_closureUncertainty"));
  searchBin_closureUncertainty->SetFillColor(0);
  searchBin_closureUncertainty->SetLineColor(1);
  searchBin_closureUncertainty->Reset();

  TH1D * closureRatio= static_cast<TH1D*>(GenHist->Clone("closureRatio"));
  TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));

  TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
  TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
  closureRatio->Divide(GenHist_Clone,EstHist_Clone,1,1,"");  // Expectation/Prediction-1 - Non closure

  double searchBin_closure_stat_uncertainty_fractional[73];
  //-----
  int n10percent=0;
  int n30percent=0;
  double avenj1;
  double avenj1nb01;
  double avenj1nb23;
  double avenj2;
  double avenj3;
  //-----
  for (int ibin=1;ibin<=72;ibin++){
    // Stat uncertainty
    searchBin_closure_stat_uncertainty_fractional[ibin]=0.;
    if (numerator->GetBinContent(ibin)!=0.){
      searchBin_closure_stat_uncertainty_fractional[ibin] = closureRatio->GetBinError(ibin)/closureRatio->GetBinContent(ibin);
    }
    double Unc;
    Unc = TMath::Max(fabs(closureRatio->GetBinContent(ibin)-1.),searchBin_closure_stat_uncertainty_fractional[ibin]);
    if (Unc>1.) Unc=1.;
    searchBin_closureUncertainty->SetBinContent(ibin,Unc);    
    //----
    printf("%5d, %8.2f\n",ibin,Unc);
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
  searchBin_closureUncertainty->Print("all");

  // For closure systematics
  histname="QCD_Up";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();

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
  QCDBin_HiDphi_closureUncertainty->Print("all");


  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
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

  double QCDBin_LowDphi_closure_stat_uncertainty_fractional[221];
  for (int ibin=1;ibin<=220;ibin++){
    // Stat uncertainty
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
    QCDBin_LowDphi_closureUncertainty->SetBinContent(ibin,Unc);    
  }
  QCDBin_LowDphi_closureUncertainty->Print("all");

  //
  // ---- BMistag & MuRecoIso systematics -----
  // -----------------------------------------------------
  // 
  // From MC sys file
  //
  string histname="searchH_b_";
  string cutname="delphi";
  sprintf(tempname,"allEvents/%s/%s",cutname.c_str(),histname.c_str());
  TH1D * searchBin_default = (TH1D*)MCSysFile->Get(tempname)->Clone();

  sprintf(tempname,"BMistagPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagDn");
  
  sprintf(tempname,"RecoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuRecoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuRecoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoSysDn");

  sprintf(tempname,"IsoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* searchBin_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* searchBin_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoDn");

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

  sprintf(tempname,"IsoSysPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s",cutname.c_str(),histname.c_str());  
  TH1D* QCDBin_HiDphi_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s",cutname.c_str(),histname.c_str());
  TH1D* QCDBin_HiDphi_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoDn");

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

  //
  // search bin
  // ----------
  takeDiffForSys(searchBin_BMistagUp,searchBin_default);
  takeDiffForSys(searchBin_BMistagDn,searchBin_default);

  takeDiffForSys(searchBin_MuRecoSysUp,searchBin_default);
  takeDiffForSys(searchBin_MuRecoSysDn,searchBin_default);

  takeDiffForSys(searchBin_MuIsoSysUp,searchBin_default);
  takeDiffForSys(searchBin_MuIsoSysDn,searchBin_default);

  takeDiffForSys(searchBin_MuRecoIsoUp,searchBin_default);
  takeDiffForSys(searchBin_MuRecoIsoDn,searchBin_default);

  //
  // QCDBin HiDphi
  // -------------
  takeDiffForSys(QCDBin_HiDphi_BMistagUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_BMistagDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuRecoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuIsoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuIsoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoDn,QCDBin_HiDphi_default);

  //
  // QCDBin LowDphi
  // --------------
  takeDiffForSys(QCDBin_LowDphi_BMistagUp,QCDBin_LowDphi_default);
  takeDiffForSys(QCDBin_LowDphi_BMistagDn,QCDBin_LowDphi_default);

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
  tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("searchBin_JECSysUp");  
  TH1D * searchBin_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("searchBin_JECSysDn");  
  TH1D * searchBin_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("searchBin_JECSysRef");  
  TH1D * searchBin_JECSysRef = (TH1D*) tempstack->GetStack()->Last();

  tempstack=(THStack*)MTSysUpFile->Get(tempname)->Clone("searchBin_MTSysUp");  
  TH1D * searchBin_MTSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysDnFile->Get(tempname)->Clone("searchBin_MTSysDn");  
  TH1D * searchBin_MTSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysRefFile->Get(tempname)->Clone("searchBin_MTSysRef");  
  TH1D * searchBin_MTSysRef = (TH1D*) tempstack->GetStack()->Last();
  
  takeDiffForSys(searchBin_JECSysUp,searchBin_JECSysRef);
  takeDiffForSys(searchBin_JECSysDn,searchBin_JECSysRef);
  takeDiffForSys(searchBin_MTSysUp,searchBin_MTSysRef);
  takeDiffForSys(searchBin_MTSysDn,searchBin_MTSysRef);


  string histname="QCD_Up";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysUp");  
  TH1D * QCDBin_HiDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysDn");  
  TH1D * QCDBin_HiDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysRef");  
  TH1D * QCDBin_HiDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();

  tempstack=(THStack*)MTSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_MTSysUp");  
  TH1D * QCDBin_HiDphi_MTSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_MTSysDn");  
  TH1D * QCDBin_HiDphi_MTSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_MTSysRef");  
  TH1D * QCDBin_HiDphi_MTSysRef = (TH1D*) tempstack->GetStack()->Last();
  
  takeDiffForSys(QCDBin_HiDphi_JECSysUp,QCDBin_HiDphi_JECSysRef);
  takeDiffForSys(QCDBin_HiDphi_JECSysDn,QCDBin_HiDphi_JECSysRef);
  takeDiffForSys(QCDBin_HiDphi_MTSysUp,QCDBin_HiDphi_MTSysRef);
  takeDiffForSys(QCDBin_HiDphi_MTSysDn,QCDBin_HiDphi_MTSysRef);


  string histname="QCD_Low";
  sprintf(tempname,"%s",histname.c_str());
  tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysUp");  
  TH1D * QCDBin_LowDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysDn");  
  TH1D * QCDBin_LowDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysRef");  
  TH1D * QCDBin_LowDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();

  tempstack=(THStack*)MTSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_MTSysUp");  
  TH1D * QCDBin_LowDphi_MTSysUp  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_MTSysDn");  
  TH1D * QCDBin_LowDphi_MTSysDn  = (TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MTSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_MTSysRef");  
  TH1D * QCDBin_LowDphi_MTSysRef = (TH1D*) tempstack->GetStack()->Last();
  
  takeDiffForSys(QCDBin_LowDphi_JECSysUp,QCDBin_LowDphi_JECSysRef);
  takeDiffForSys(QCDBin_LowDphi_JECSysDn,QCDBin_LowDphi_JECSysRef);
  takeDiffForSys(QCDBin_LowDphi_MTSysUp,QCDBin_LowDphi_MTSysRef);
  takeDiffForSys(QCDBin_LowDphi_MTSysDn,QCDBin_LowDphi_MTSysRef);


  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // 
  // Displaying errors
  //
  TCanvas *tc1 = new TCanvas("searchBin_UncertaintySummary","searchBin_UncertaintySummary",1200,600);
  double ymax=+1.5;
  double ymin=-1.0;
  double yoff= 0.1;
  searchBin_box->SetMaximum(ymax);
  searchBin_box->SetMinimum(ymin);
  searchBin_box->SetStats(false);
  searchBin_box->SetTitle("Uncertainties versus search bins");
  searchBin_box->GetXaxis()->SetTitle("Search bin");
  searchBin_box->GetYaxis()->SetTitle("Fractional uncertainties");
  searchBin_box->SetLineWidth(2);
  searchBin_box->SetLineColor(0);
  searchBin_box->SetFillColor(0);
  searchBin_box->Draw();

  searchBin_StatUncertaintiesFractional->SetLineColor(1);
  searchBin_StatUncertaintiesFractional->Draw("same");
  searchBin_StatUncertaintiesFractional->SetFillColor(21);
  
  searchBin_closureUncertainty->SetLineColor(2);
  searchBin_closureUncertainty->SetLineWidth(2);
  searchBin_closureUncertainty->SetFillColor(0);
  searchBin_closureUncertainty->Draw("same");

  searchBin_BMistagUp->SetLineColor(8);
  searchBin_BMistagUp->SetLineWidth(2);
  searchBin_BMistagUp->Draw("hist,same");

  searchBin_BMistagDn->SetLineColor(8);
  searchBin_BMistagDn->SetLineWidth(2);
  searchBin_BMistagDn->SetLineStyle(2);
  searchBin_BMistagDn->Draw("hist,same");

  searchBin_MuRecoSysUp->SetLineColor(4);
  searchBin_MuRecoSysUp->SetLineWidth(2);
  searchBin_MuRecoSysUp->SetLineStyle(2);
  searchBin_MuRecoSysDn->SetLineColor(4);
  searchBin_MuRecoSysDn->SetLineWidth(2);
  searchBin_MuRecoSysUp->Draw("hist,same");
  searchBin_MuRecoSysDn->Draw("hist,same");

  searchBin_MuIsoSysUp->SetLineColor(4);
  searchBin_MuIsoSysUp->SetLineWidth(2);
  searchBin_MuIsoSysUp->SetLineStyle(2);
  searchBin_MuIsoSysDn->SetLineColor(4);
  searchBin_MuIsoSysDn->SetLineWidth(2);
  searchBin_MuIsoSysUp->Draw("hist,same");
  searchBin_MuIsoSysDn->Draw("hist,same");

  searchBin_JECSysUp->SetFillColor(0);
  searchBin_JECSysDn->SetFillColor(0);
  searchBin_JECSysUp->SetLineWidth(2);
  searchBin_JECSysDn->SetLineWidth(2);
  searchBin_JECSysUp->SetLineColor(6);
  searchBin_JECSysDn->SetLineColor(6);
  searchBin_JECSysUp->SetLineStyle(2);
  searchBin_JECSysUp->Draw("hist,same");
  searchBin_JECSysDn->Draw("hist,same");

  searchBin_MTSysUp->SetFillColor(0);
  searchBin_MTSysDn->SetFillColor(0);
  searchBin_MTSysUp->SetLineWidth(2);
  searchBin_MTSysDn->SetLineWidth(2);
  searchBin_MTSysUp->SetLineColor(12);
  searchBin_MTSysDn->SetLineColor(12);
  searchBin_MTSysUp->SetLineStyle(2);
  searchBin_MTSysUp->Draw("hist,same");
  searchBin_MTSysDn->Draw("hist,same");

  searchBin_IsoTrkVetoEffUncertaintyTot->SetFillColor(0);
  searchBin_IsoTrkVetoEffUncertaintyTot->SetLineWidth(2);
  searchBin_IsoTrkVetoEffUncertaintyTot->SetLineColor(16);
  searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");
  searchBin_IsoTrkVetoEffUncertaintySys->SetFillColor(0);
  searchBin_IsoTrkVetoEffUncertaintySys->SetLineWidth(2);
  searchBin_IsoTrkVetoEffUncertaintySys->SetLineColor(16);
  searchBin_IsoTrkVetoEffUncertaintyStat->SetFillColor(0);
  searchBin_IsoTrkVetoEffUncertaintyStat->SetLineWidth(2);
  searchBin_IsoTrkVetoEffUncertaintyStat->SetLineColor(16);

  searchBin_MtEffStat->SetLineWidth(2);
  searchBin_MtEffStat->SetLineColor(12);
  searchBin_MtEffStat->SetLineStyle(2);
  searchBin_MtEffStat->Draw("hist,same");  

  searchBin_AccStat->SetLineWidth(2);
  searchBin_AccStat->SetLineColor(2);
  searchBin_AccStat->SetLineStyle(4);
  searchBin_AccStat->Draw("hist,same");

  searchBin_MuFromTauStat->SetLineWidth(2);
  searchBin_MuFromTauStat->SetLineColor(4);
  searchBin_MuFromTauStat->SetLineStyle(4);
  searchBin_MuFromTauStat->Draw("hist,same");

  searchBin_DileptonUncertainty->SetLineWidth(2);
  searchBin_DileptonUncertainty->SetLineColor(8);
  searchBin_DileptonUncertainty->SetLineStyle(4);
  searchBin_DileptonUncertainty->Draw("hist,same");

  //
  // Drawing lines
  TLine *tlzero = new TLine(1.,0.,73.,0.);
  tlzero->SetLineStyle(2);
  tlzero->SetLineColor(4);
  tlzero->Draw();

  float ymax_bottom  = ymax * 0.9;
  float ymax2_bottom = ymax * 0.8;
  float ymax3_bottom = ymax * 0.8;
  float ymax4_bottom = ymax * 0.8;
  float ymin_bottom  = ymin * 0.8;

  TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  tline_bottom_1->SetLineStyle(2);
  tline_bottom_1->Draw();
  
  TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  tline_bottom_2->SetLineStyle(2);
  tline_bottom_2->Draw();

  TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  tline_bottom_1a->SetLineStyle(3);
  tline_bottom_1a->Draw();
  TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  tline_bottom_1b->SetLineStyle(3);
  tline_bottom_1b->Draw();
  TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  tline_bottom_1c->SetLineStyle(3);
  tline_bottom_1c->Draw();

  TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  tline_bottom_2a->SetLineStyle(3);
  tline_bottom_2a->Draw();
  TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  tline_bottom_2b->SetLineStyle(3);
  tline_bottom_2b->Draw();
  TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  tline_bottom_2c->SetLineStyle(3);
  tline_bottom_2c->Draw();
      
  TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  tline_bottom_3a->SetLineStyle(3);
  tline_bottom_3a->Draw();
  TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  tline_bottom_3b->SetLineStyle(3);
  tline_bottom_3b->Draw();
  TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  tline_bottom_3c->SetLineStyle(3);
  tline_bottom_3c->Draw();

  //
  // Legend & texts
  TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  ttext1a->SetTextFont(42);
  ttext1a->SetTextSize(0.04);
  ttext1a->SetTextAlign(22);
  ttext1a->Draw();
  
  TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  ttext1b->SetTextFont(42);
  ttext1b->SetTextSize(0.04);
  ttext1b->SetTextAlign(22);
  ttext1b->Draw();
  
  TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  ttext1c->SetTextFont(42);
  ttext1c->SetTextSize(0.04);
  ttext1c->SetTextAlign(22);
  ttext1c->Draw();
  
  TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  ttext1d->SetTextFont(42);
  ttext1d->SetTextSize(0.04);
  ttext1d->SetTextAlign(22);
  ttext1d->Draw();

  //
  //
  //
  Float_t legendX1 = .15; //.50;
  Float_t legendX2 = .30; //.70;
  Float_t legendY1 = .13; //.65;
  Float_t legendY2 = .28; //

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  sprintf(tempname,"Data stat uncertainties");
  catLeg1->AddEntry(searchBin_StatUncertainties,tempname);
  sprintf(tempname,"Non-closure uncertainties");
  catLeg1->AddEntry(searchBin_closureUncertainty,tempname);
  sprintf(tempname,"B-tag mistag modeling uncertainties");
  catLeg1->AddEntry(searchBin_BMistagUp,tempname);
  sprintf(tempname,"Muon efficiency uncertainties");
  catLeg1->AddEntry(searchBin_MuRecoSysDn,tempname);
  catLeg1->Draw();

  TLegend* catLeg1b = new TLegend(legendX1+0.25,legendY1,legendX2+0.25,legendY2);
  catLeg1b->SetTextSize(0.032);
  catLeg1b->SetTextFont(42);
  catLeg1b->SetFillColor(0);
  catLeg1b->SetLineColor(0);
  catLeg1b->SetBorderSize(0);

  sprintf(tempname,"Tau energy scale uncertainties");
  catLeg1b->AddEntry(searchBin_JECSysDn,tempname);
  sprintf(tempname,"MT scale uncertainties");
  catLeg1b->AddEntry(searchBin_MTSysDn,tempname);
  sprintf(tempname,"Isotrack veto uncertainties (sys)");
  catLeg1b->AddEntry(searchBin_IsoTrkVetoEffUncertaintySys,tempname);
  sprintf(tempname,"Dilepton background subtraction");
  catLeg1b->AddEntry(searchBin_DileptonUncertainty,tempname);
  catLeg1b->Draw();

  TLegend* catLeg1c = new TLegend(legendX1+0.50,legendY1,legendX2+0.50,legendY2);
  catLeg1c->SetTextSize(0.032);
  catLeg1c->SetTextFont(42);
  catLeg1c->SetFillColor(0);
  catLeg1c->SetLineColor(0);
  catLeg1c->SetBorderSize(0);

  sprintf(tempname,"M_{T} cut efficiency (stat)");
  catLeg1c->AddEntry(searchBin_MtEffStat,tempname);
  sprintf(tempname,"Acceptance (stat)");
  catLeg1c->AddEntry(searchBin_AccStat,tempname);
  sprintf(tempname,"#mu from tau correction (stat)");
  catLeg1c->AddEntry(searchBin_MuFromTauStat,tempname);
  catLeg1c->Draw();

  sprintf(tempname,"searchBin_UncertaintySummary.png");
  tc1->SaveAs(tempname);
  sprintf(tempname,"searchBin_UncertaintySummary.pdf");
  tc1->SaveAs(tempname);
    
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // 
  // Displaying errors
  //
  TCanvas *tc2 = new TCanvas("searchBin_UncertaintySummary2","searchBin_UncertaintySummary2",1200,600);
  ymax=+1.5;
  ymin=-1.0;
  yoff= 0.1;
  searchBin_box->SetMaximum(ymax);
  searchBin_box->SetMinimum(ymin);
  searchBin_box->Draw();

  searchBin_closureUncertainty->Draw("same");

  searchBin_BMistagUp->Draw("hist,same");

  searchBin_BMistagDn->Draw("hist,same");

  searchBin_MuRecoSysUp->Draw("hist,same");
  searchBin_MuRecoSysDn->Draw("hist,same");

  searchBin_JECSysUp->Draw("hist,same");
  searchBin_JECSysDn->Draw("hist,same");

  searchBin_MTSysUp->Draw("hist,same");
  searchBin_MTSysDn->Draw("hist,same");

  searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  //
  // Drawing lines
  TLine *tlzero = new TLine(1.,0.,73.,0.);
  tlzero->SetLineStyle(2);
  tlzero->SetLineColor(4);
  tlzero->Draw();

  ymax_bottom  = ymax * 0.9;
  ymax2_bottom = ymax * 0.8;
  ymax3_bottom = ymax * 0.8;
  ymax4_bottom = ymax * 0.8;
  ymin_bottom  = ymin * 0.8;

  TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  tline_bottom_1->SetLineStyle(2);
  tline_bottom_1->Draw();
  
  TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  tline_bottom_2->SetLineStyle(2);
  tline_bottom_2->Draw();

  TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  tline_bottom_1a->SetLineStyle(3);
  tline_bottom_1a->Draw();
  TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  tline_bottom_1b->SetLineStyle(3);
  tline_bottom_1b->Draw();
  TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  tline_bottom_1c->SetLineStyle(3);
  tline_bottom_1c->Draw();

  TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  tline_bottom_2a->SetLineStyle(3);
  tline_bottom_2a->Draw();
  TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  tline_bottom_2b->SetLineStyle(3);
  tline_bottom_2b->Draw();
  TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  tline_bottom_2c->SetLineStyle(3);
  tline_bottom_2c->Draw();
      
  TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  tline_bottom_3a->SetLineStyle(3);
  tline_bottom_3a->Draw();
  TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  tline_bottom_3b->SetLineStyle(3);
  tline_bottom_3b->Draw();
  TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  tline_bottom_3c->SetLineStyle(3);
  tline_bottom_3c->Draw();

  //
  // Legend & texts
  TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  ttext1a->SetTextFont(42);
  ttext1a->SetTextSize(0.04);
  ttext1a->SetTextAlign(22);
  ttext1a->Draw();
  
  TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  ttext1b->SetTextFont(42);
  ttext1b->SetTextSize(0.04);
  ttext1b->SetTextAlign(22);
  ttext1b->Draw();
  
  TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  ttext1c->SetTextFont(42);
  ttext1c->SetTextSize(0.04);
  ttext1c->SetTextAlign(22);
  ttext1c->Draw();
  
  TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  ttext1d->SetTextFont(42);
  ttext1d->SetTextSize(0.04);
  ttext1d->SetTextAlign(22);
  ttext1d->Draw();

  //
  //
  //
  Float_t legendX1 = .25; //.50;
  Float_t legendX2 = .45; //.70;
  Float_t legendY1 = .13; //.65;
  Float_t legendY2 = .28; //

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //sprintf(tempname,"Data stat uncertainties");
  //catLeg1->AddEntry(searchBin_StatUncertainties,tempname);
  sprintf(tempname,"Non-closure uncertainties");
  catLeg1->AddEntry(searchBin_closureUncertainty,tempname);
  sprintf(tempname,"B-tag mistag modeling uncertainties");
  catLeg1->AddEntry(searchBin_BMistagUp,tempname);
  sprintf(tempname,"Muon efficiency uncertainties");
  catLeg1->AddEntry(searchBin_MuRecoSysDn,tempname);
  catLeg1->Draw();

  catLeg1b->Draw();

  sprintf(tempname,"searchBin_UncertaintySummary2.png");
  tc2->SaveAs(tempname);
  sprintf(tempname,"searchBin_UncertaintySummary2.pdf");
  tc2->SaveAs(tempname);
    
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // 
  // Displaying errors
  //
  TCanvas *tc2a = new TCanvas("searchBin_UncertaintySummary2a","searchBin_UncertaintySummary2a",1200,600);
  ymax=+0.3;
  ymin=-0.3;
  yoff=0.05;
  searchBin_box->SetMaximum(ymax);
  searchBin_box->SetMinimum(ymin);
  searchBin_box->Draw();

  searchBin_closureUncertainty->Draw("same");

  searchBin_BMistagUp->Draw("hist,same");
  searchBin_BMistagDn->Draw("hist,same");

  searchBin_MuRecoSysUp->Draw("hist,same");
  searchBin_MuRecoSysDn->Draw("hist,same");

  //searchBin_JECSysUp->Draw("hist,same");
  //searchBin_JECSysDn->Draw("hist,same");

  //searchBin_MTSysUp->Draw("hist,same");
  //searchBin_MTSysDn->Draw("hist,same");

  //searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  //
  // Drawing lines
  TLine *tlzero = new TLine(1.,0.,73.,0.);
  tlzero->SetLineStyle(2);
  tlzero->SetLineColor(4);
  tlzero->Draw();

  ymax_bottom  = ymax * 0.9;
  ymax2_bottom = ymax * 0.8;
  ymax3_bottom = ymax * 0.8;
  ymax4_bottom = ymax * 0.8;
  ymin_bottom  = ymin * 0.8;

  TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  tline_bottom_1->SetLineStyle(2);
  tline_bottom_1->Draw();
  
  TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  tline_bottom_2->SetLineStyle(2);
  tline_bottom_2->Draw();

  TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  tline_bottom_1a->SetLineStyle(3);
  tline_bottom_1a->Draw();
  TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  tline_bottom_1b->SetLineStyle(3);
  tline_bottom_1b->Draw();
  TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  tline_bottom_1c->SetLineStyle(3);
  tline_bottom_1c->Draw();

  TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  tline_bottom_2a->SetLineStyle(3);
  tline_bottom_2a->Draw();
  TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  tline_bottom_2b->SetLineStyle(3);
  tline_bottom_2b->Draw();
  TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  tline_bottom_2c->SetLineStyle(3);
  tline_bottom_2c->Draw();
      
  TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  tline_bottom_3a->SetLineStyle(3);
  tline_bottom_3a->Draw();
  TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  tline_bottom_3b->SetLineStyle(3);
  tline_bottom_3b->Draw();
  TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  tline_bottom_3c->SetLineStyle(3);
  tline_bottom_3c->Draw();

  //
  // Legend & texts
  TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  ttext1a->SetTextFont(42);
  ttext1a->SetTextSize(0.04);
  ttext1a->SetTextAlign(22);
  ttext1a->Draw();
  
  TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  ttext1b->SetTextFont(42);
  ttext1b->SetTextSize(0.04);
  ttext1b->SetTextAlign(22);
  ttext1b->Draw();
  
  TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  ttext1c->SetTextFont(42);
  ttext1c->SetTextSize(0.04);
  ttext1c->SetTextAlign(22);
  ttext1c->Draw();
  
  TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  ttext1d->SetTextFont(42);
  ttext1d->SetTextSize(0.04);
  ttext1d->SetTextAlign(22);
  ttext1d->Draw();

  //
  //
  //
  catLeg1->Draw();
  //catLeg1b->Draw();

  sprintf(tempname,"searchBin_UncertaintySummary2a.png");
  tc2a->SaveAs(tempname);
  sprintf(tempname,"searchBin_UncertaintySummary2a.pdf");
  tc2a->SaveAs(tempname);
    
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // 
  // Displaying errors
  //
  TCanvas *tc2b = new TCanvas("searchBin_UncertaintySummary2b","searchBin_UncertaintySummary2b",1200,600);
  ymax=+0.3;
  ymin=-0.3;
  yoff=0.05;
  searchBin_box->SetMaximum(ymax);
  searchBin_box->SetMinimum(ymin);
  searchBin_box->Draw();

  //searchBin_closureUncertainty->Draw("same");

  //searchBin_BMistagUp->Draw("hist,same");
  //searchBin_BMistagDn->Draw("hist,same");

  //searchBin_MuRecoSysUp->Draw("hist,same");
  //searchBin_MuRecoSysDn->Draw("hist,same");

  searchBin_JECSysUp->Draw("hist,same");
  searchBin_JECSysDn->Draw("hist,same");

  searchBin_MTSysUp->Draw("hist,same");
  searchBin_MTSysDn->Draw("hist,same");

  searchBin_IsoTrkVetoEffUncertaintySys->Draw("hist,same");

  searchBin_DileptonUncertainty->Draw("hist,same");

  //
  // Drawing lines
  TLine *tlzero = new TLine(1.,0.,73.,0.);
  tlzero->SetLineStyle(2);
  tlzero->SetLineColor(4);
  tlzero->Draw();

  ymax_bottom  = ymax * 0.9;
  ymax2_bottom = ymax * 0.8;
  ymax3_bottom = ymax * 0.8;
  ymax4_bottom = ymax * 0.8;
  ymin_bottom  = ymin * 0.8;

  TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  tline_bottom_1->SetLineStyle(2);
  tline_bottom_1->Draw();
  
  TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  tline_bottom_2->SetLineStyle(2);
  tline_bottom_2->Draw();

  TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  tline_bottom_1a->SetLineStyle(3);
  tline_bottom_1a->Draw();
  TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  tline_bottom_1b->SetLineStyle(3);
  tline_bottom_1b->Draw();
  TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  tline_bottom_1c->SetLineStyle(3);
  tline_bottom_1c->Draw();

  TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  tline_bottom_2a->SetLineStyle(3);
  tline_bottom_2a->Draw();
  TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  tline_bottom_2b->SetLineStyle(3);
  tline_bottom_2b->Draw();
  TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  tline_bottom_2c->SetLineStyle(3);
  tline_bottom_2c->Draw();
      
  TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  tline_bottom_3a->SetLineStyle(3);
  tline_bottom_3a->Draw();
  TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  tline_bottom_3b->SetLineStyle(3);
  tline_bottom_3b->Draw();
  TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  tline_bottom_3c->SetLineStyle(3);
  tline_bottom_3c->Draw();

  //
  // Legend & texts
  TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  ttext1a->SetTextFont(42);
  ttext1a->SetTextSize(0.04);
  ttext1a->SetTextAlign(22);
  ttext1a->Draw();
  
  TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  ttext1b->SetTextFont(42);
  ttext1b->SetTextSize(0.04);
  ttext1b->SetTextAlign(22);
  ttext1b->Draw();
  
  TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  ttext1c->SetTextFont(42);
  ttext1c->SetTextSize(0.04);
  ttext1c->SetTextAlign(22);
  ttext1c->Draw();
  
  TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  ttext1d->SetTextFont(42);
  ttext1d->SetTextSize(0.04);
  ttext1d->SetTextAlign(22);
  ttext1d->Draw();

  TLegend* catLeg1b = new TLegend(legendX1+0.25,legendY1,legendX2+0.25,legendY2);
  catLeg1b->SetTextSize(0.032);
  catLeg1b->SetTextFont(42);
  catLeg1b->SetFillColor(0);
  catLeg1b->SetLineColor(0);
  catLeg1b->SetBorderSize(0);

  sprintf(tempname,"Tau energy scale uncertainties");
  catLeg1b->AddEntry(searchBin_JECSysDn,tempname);
  sprintf(tempname,"MT scale uncertainties");
  catLeg1b->AddEntry(searchBin_MTSysDn,tempname);
  sprintf(tempname,"Isotrack veto uncertainties (sys)");
  catLeg1b->AddEntry(searchBin_IsoTrkVetoEffUncertaintySys,tempname);
  sprintf(tempname,"Dilepton background subtraction");
  catLeg1b->AddEntry(searchBin_DileptonUncertainty,tempname);
  catLeg1b->Draw();

  TLegend* catLeg1c = new TLegend(legendX1+0.50,legendY1,legendX2+0.50,legendY2);
  catLeg1c->SetTextSize(0.032);
  catLeg1c->SetTextFont(42);
  catLeg1c->SetFillColor(0);
  catLeg1c->SetLineColor(0);
  catLeg1c->SetBorderSize(0);

  sprintf(tempname,"M_{T} cut efficiency (stat)");
  catLeg1c->AddEntry(searchBin_MtEffStat,tempname);
  sprintf(tempname,"Acceptance (stat)");
  catLeg1c->AddEntry(searchBin_AccStat,tempname);
  sprintf(tempname,"#mu from tau correction (stat)");
  catLeg1c->AddEntry(searchBin_MuFromTauStat,tempname);
  sprintf(tempname,"Isotrack veto uncertainties (stat)");
  catLeg1c->AddEntry(searchBin_IsoTrkVetoEffUncertaintyStat,tempname);
  //catLeg1c->Draw();

  //
  //
  //
  //catLeg1->Draw();
  catLeg1b->Draw();

  sprintf(tempname,"searchBin_UncertaintySummary2b.png");
  tc2b->SaveAs(tempname);
  sprintf(tempname,"searchBin_UncertaintySummary2b.pdf");
  tc2b->SaveAs(tempname);
    
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  // 
  // Displaying errors
  //
  TCanvas *tc2c = new TCanvas("searchBin_UncertaintySummary2c","searchBin_UncertaintySummary2c",1200,600);
  ymax=+0.3;
  ymin=-0.3;
  yoff=0.05;
  searchBin_box->SetMaximum(ymax);
  searchBin_box->SetMinimum(ymin);
  searchBin_box->Draw();

  //searchBin_closureUncertainty->Draw("same");

  //searchBin_BMistagUp->Draw("hist,same");
  //searchBin_BMistagDn->Draw("hist,same");

  //searchBin_MuRecoSysUp->Draw("hist,same");
  //searchBin_MuRecoSysDn->Draw("hist,same");

  //searchBin_JECSysUp->Draw("hist,same");
  //searchBin_JECSysDn->Draw("hist,same");

  //searchBin_MTSysUp->Draw("hist,same");
  //searchBin_MTSysDn->Draw("hist,same");

  //searchBin_IsoTrkVetoEffUncertaintyTot->Draw("hist,same");

  searchBin_MtEffStat->Draw("hist,same");  
  searchBin_AccStat->Draw("hist,same");
  searchBin_MuFromTauStat->Draw("hist,same");
  searchBin_IsoTrkVetoEffUncertaintyStat->Draw("hist,same");

  //
  // Drawing lines
  TLine *tlzero = new TLine(1.,0.,73.,0.);
  tlzero->SetLineStyle(2);
  tlzero->SetLineColor(4);
  tlzero->Draw();

  ymax_bottom  = ymax * 0.9;
  ymax2_bottom = ymax * 0.8;
  ymax3_bottom = ymax * 0.8;
  ymax4_bottom = ymax * 0.8;
  ymin_bottom  = ymin * 0.8;

  TLine *tline_bottom_1 = new TLine(25.,ymin_bottom,25.,ymax_bottom);
  tline_bottom_1->SetLineStyle(2);
  tline_bottom_1->Draw();
  
  TLine *tline_bottom_2 = new TLine(49.,ymin_bottom,49.,ymax_bottom);
  tline_bottom_2->SetLineStyle(2);
  tline_bottom_2->Draw();

  TLine *tline_bottom_1a = new TLine(7.,ymin_bottom,7.,ymax2_bottom);
  tline_bottom_1a->SetLineStyle(3);
  tline_bottom_1a->Draw();
  TLine *tline_bottom_1b = new TLine(13.,ymin_bottom,13.,ymax2_bottom);
  tline_bottom_1b->SetLineStyle(3);
  tline_bottom_1b->Draw();
  TLine *tline_bottom_1c = new TLine(19.,ymin_bottom,19.,ymax2_bottom);
  tline_bottom_1c->SetLineStyle(3);
  tline_bottom_1c->Draw();

  TLine *tline_bottom_2a = new TLine(31.,ymin_bottom,31.,ymax3_bottom);
  tline_bottom_2a->SetLineStyle(3);
  tline_bottom_2a->Draw();
  TLine *tline_bottom_2b = new TLine(37.,ymin_bottom,37.,ymax3_bottom);
  tline_bottom_2b->SetLineStyle(3);
  tline_bottom_2b->Draw();
  TLine *tline_bottom_2c = new TLine(43.,ymin_bottom,43.,ymax3_bottom);
  tline_bottom_2c->SetLineStyle(3);
  tline_bottom_2c->Draw();
      
  TLine *tline_bottom_3a = new TLine(55.,ymin_bottom,55.,ymax4_bottom);
  tline_bottom_3a->SetLineStyle(3);
  tline_bottom_3a->Draw();
  TLine *tline_bottom_3b = new TLine(61.,ymin_bottom,61.,ymax4_bottom);
  tline_bottom_3b->SetLineStyle(3);
  tline_bottom_3b->Draw();
  TLine *tline_bottom_3c = new TLine(67.,ymin_bottom,67.,ymax4_bottom);
  tline_bottom_3c->SetLineStyle(3);
  tline_bottom_3c->Draw();

  //
  // Legend & texts
  TLatex * ttext1 = new TLatex(13. , ymax_bottom , "4 #leq N_{jets} #leq 6");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(37. , ymax_bottom , "7 #leq N_{jets} #leq 8");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(61. , ymax_bottom , "N_{jets} #geq 9");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext1a = new TLatex(4. , ymax2_bottom-yoff , "N_{b} = 0");
  ttext1a->SetTextFont(42);
  ttext1a->SetTextSize(0.04);
  ttext1a->SetTextAlign(22);
  ttext1a->Draw();
  
  TLatex * ttext1b = new TLatex(10. , ymax2_bottom-yoff , "N_{b} = 1");
  ttext1b->SetTextFont(42);
  ttext1b->SetTextSize(0.04);
  ttext1b->SetTextAlign(22);
  ttext1b->Draw();
  
  TLatex * ttext1c = new TLatex(16. , ymax2_bottom-yoff , "N_{b} = 2");
  ttext1c->SetTextFont(42);
  ttext1c->SetTextSize(0.04);
  ttext1c->SetTextAlign(22);
  ttext1c->Draw();
  
  TLatex * ttext1d = new TLatex(22. , ymax2_bottom-yoff , "N_{b} #geq 3");
  ttext1d->SetTextFont(42);
  ttext1d->SetTextSize(0.04);
  ttext1d->SetTextAlign(22);
  ttext1d->Draw();

  //
  //
  //
  //catLeg1->Draw();
  //catLeg1b->Draw();
  catLeg1c->Draw();

  sprintf(tempname,"searchBin_UncertaintySummary2c.png");
  tc2c->SaveAs(tempname);
  sprintf(tempname,"searchBin_UncertaintySummary2c.pdf");
  tc2c->SaveAs(tempname);
    
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
  searchBin_closureUncertainty->Add(searchBin_one);       searchBin_closureUncertainty->Write();
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
  searchBin_MTSysUp->Add(searchBin_one);                  searchBin_MTSysUp->Write("searchBin_MTSysUp");
  searchBin_MTSysDn->Add(searchBin_one);                  searchBin_MTSysDn->Write("searchBin_MTSysDn");
  searchBin_MtEffStat->Add(searchBin_one);                searchBin_MtEffStat->Write();  
  searchBin_IsoTrkVetoEffUncertaintyStat->Add(searchBin_one); searchBin_IsoTrkVetoEffUncertaintyStat->Write();
  searchBin_IsoTrkVetoEffUncertaintySys->Add(searchBin_one);  searchBin_IsoTrkVetoEffUncertaintySys->Write();
  searchBin_AccStat->Add(searchBin_one);                  searchBin_AccStat->Write();
  searchBin_MuFromTauStat->Add(searchBin_one);            searchBin_MuFromTauStat->Write();
  searchBin_DileptonUncertainty->Add(searchBin_one);      searchBin_DileptonUncertainty->Write();
  
  //
  hPredHTMHT0b_nominal->Write();
  hPredHTMHTwb_nominal->Write();
  hPredNJetBins_nominal->Write();
  hPredNbBins_nominal->Write();

  //
  QCDBin_HiDphi_nominal->Write();
  QCDBin_HiDphi_nominal_fullstatuncertainty->Write();
  QCDBin_HiDphi_StatUncertainties->Write();
  QCDBin_HiDphi_closureUncertainty->Add(QCDBin_one);       QCDBin_HiDphi_closureUncertainty->Write();
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
  QCDBin_HiDphi_MTSysUp->Add(QCDBin_one);                  QCDBin_HiDphi_MTSysUp->Write("QCDBin_HiDphi_MTSysUp");
  QCDBin_HiDphi_MTSysDn->Add(QCDBin_one);                  QCDBin_HiDphi_MTSysDn->Write("QCDBin_HiDphi_MTSysDn");
  QCDBin_HiDphi_MtEffStat->Add(QCDBin_one);                QCDBin_HiDphi_MtEffStat->Write();  
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_HiDphi_AccStat->Add(QCDBin_one);                  QCDBin_HiDphi_AccStat->Write();
  QCDBin_HiDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_HiDphi_MuFromTauStat->Write();
  QCDBin_HiDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_HiDphi_DileptonUncertainty->Write();

  //
  QCDBin_LowDphi_nominal->Write();
  QCDBin_LowDphi_nominal_fullstatuncertainty->Write();
  QCDBin_LowDphi_StatUncertainties->Write();
  QCDBin_LowDphi_closureUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_closureUncertainty->Write();
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
  QCDBin_LowDphi_MTSysUp->Add(QCDBin_one);                 QCDBin_LowDphi_MTSysUp->Write("QCDBin_LowDphi_MTSysUp");
  QCDBin_LowDphi_MTSysDn->Add(QCDBin_one);                 QCDBin_LowDphi_MTSysDn->Write("QCDBin_LowDphi_MTSysDn");
  QCDBin_LowDphi_MtEffStat->Add(QCDBin_one);                QCDBin_LowDphi_MtEffStat->Write();  
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_LowDphi_AccStat->Add(QCDBin_one);                  QCDBin_LowDphi_AccStat->Write();
  QCDBin_LowDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_LowDphi_MuFromTauStat->Write();
  QCDBin_LowDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_DileptonUncertainty->Write();

  HadTauEstimation_OutputFile.Close();

}

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
void effMapStatErrPropagation(TH1* input, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinError(ibin+1)/input->GetBinContent(ibin+1));
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

void isoTrkVetoErrPropagation(TH1* input, double syserr, TH1* one,
			      TH1* output_tot, TH1* output_stat, TH1* output_sys){

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
