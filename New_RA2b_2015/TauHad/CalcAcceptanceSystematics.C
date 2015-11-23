#include <vector>

void CalcAcceptanceSystematics(){

  char tempname[200];
  TFile * infile = new TFile("LostLepton2_MuonEfficienciesFromAll_Elog408_.root","R");
  TH1* hAccPass, * hAccAll, * hAccPass_lowDphi, * hAccAll_lowDphi;
  TH1* hSumofSquareOfDev , * hSumofSquareOfDev_lowDphi;
  vector<TH1*> hAccVec, hAcc_lowDphiVec, hAcc_DeviationFromNomVec, hAcc_DeviationFromNom_lowDphiVec;


  for(int iacc=0; iacc < 101; iacc++){// 101 pdf 

    // get the pass
    sprintf(tempname,"Systematics/hAccPass_%d",iacc);
    hAccPass = (TH1*) infile->Get(tempname)->Clone();

    // get the all
    sprintf(tempname,"Systematics/hAccAll_%d",iacc);
    hAccAll = (TH1*) infile->Get(tempname)->Clone();    
    
    // divide the pass and all to get acc
    sprintf(tempname,"hAccVec_%d",iacc);
    hAccVec.push_back(static_cast<TH1*>(hAccPass->Clone(tempname)));
    hAccVec[iacc]->Divide(hAccPass,hAccAll,1,1,"B");

    cout << " bin5=> acc: " << hAccVec[iacc]->GetBinContent(5) << endl;

    // lowDphi
    sprintf(tempname,"Systematics/hAccPass_lowDphi_%d",iacc);
    hAccPass_lowDphi = (TH1*) infile->Get(tempname)->Clone();

    sprintf(tempname,"Systematics/hAccAll_lowDphi_%d",iacc);
    hAccAll_lowDphi = (TH1*) infile->Get(tempname)->Clone();

    sprintf(tempname,"hAcc_lowDphiVec_%d",iacc);
    hAcc_lowDphiVec.push_back(static_cast<TH1*>(hAccPass_lowDphi->Clone(tempname)));
    hAcc_lowDphiVec[iacc]->Divide(hAccPass_lowDphi,hAccAll_lowDphi,1,1,"B");


    // calculate the deviation from nominal acceptance
    hAcc_DeviationFromNomVec.push_back(static_cast<TH1*>(hAccVec[iacc]->Clone("hAcc_DeviationFromNomVec")));// copy
    hAcc_DeviationFromNomVec[iacc]->Add(hAccVec[0],-1.0); // subtract the nominal from each acceptance: Acc - Acc_nom


    hAcc_DeviationFromNomVec[iacc]->Multiply(hAcc_DeviationFromNomVec[iacc]); // (Acc - Acc_nom)^2
    if(iacc==0){
      hSumofSquareOfDev = static_cast<TH1*>(hAccAll->Clone("hSumofSquareOfDev"));
      hSumofSquareOfDev->Reset();
    }

    hSumofSquareOfDev->Add(hAcc_DeviationFromNomVec[iacc]); // sum{ (Acc - Acc_nom)^2 }

    // do the same for lowDphi
    hAcc_DeviationFromNom_lowDphiVec.push_back(static_cast<TH1*>(hAcc_lowDphiVec[iacc]->Clone("hAcc_DeviationFromNom_lowDphiVec")));// copy
    hAcc_DeviationFromNom_lowDphiVec[iacc]->Add(hAcc_lowDphiVec[0],-1.0); // subtract the nominal from each acceptance: Acc - Acc_nom
    hAcc_DeviationFromNom_lowDphiVec[iacc]->Multiply(hAcc_DeviationFromNom_lowDphiVec[iacc]); // (Acc - Acc_nom)^2

    if(iacc==0){
      hSumofSquareOfDev_lowDphi = static_cast<TH1*>(hAccAll->Clone("hSumofSquareOfDev_lowDphi"));
      hSumofSquareOfDev_lowDphi->Reset();
    }

    hSumofSquareOfDev_lowDphi->Add(hAcc_DeviationFromNom_lowDphiVec[iacc]); // sum{ (Acc - Acc_nom)^2 }

  }// end of loop over pdf



  TH1 * hAccSys = static_cast<TH1*>(hAccAll->Clone("hAccSys"));
  hAccSys->Reset();
  TH1 * hAccSys_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSys_lowDphi"));
  hAccSys_lowDphi->Reset();

  for(int ibin=0; ibin < hAccSys->GetNbinsX()+2; ibin++){
    //cout << "bincontent: " <<  hAccSys->GetBinContent(ibin) << endl;

   

    hAccSys->SetBinContent(ibin,pow(hSumofSquareOfDev->GetBinContent(ibin),0.5)); // sqrt[ sum{ (Acc - Acc_nom)^2 } ]
    hAccSys_lowDphi->SetBinContent(ibin,pow(hSumofSquareOfDev_lowDphi->GetBinContent(ibin),0.5)); // sqrt[ sum{ (Acc - Acc_nom)^2 } ]

  
  }



  TFile * outFile = new TFile("AcceptanceSystematics_AllSamples.root","RECREATE");
  hAccSys->Write();
  hAccSys_lowDphi->Write();

  /*
  for(int iacc=0; iacc < PDFsize ; iacc++){
    hAccAllVec[iacc]->Write();
    hAccPassVec[iacc]->Write();
    hAccVec[iacc]->Write();
    hAccAll_lowDphiVec[iacc]->Write();
    hAccPass_lowDphiVec[iacc]->Write();
    hAcc_lowDphiVec[iacc]->Write();

  }
  */

  outFile->Close();

}
