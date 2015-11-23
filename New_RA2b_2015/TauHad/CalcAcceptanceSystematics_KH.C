#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <numeric>      // std::accumulate
#include <vector>
#include <algorithm>

//
// inner product
template <class InputIterator1, class InputIterator2, class T>
   T inner_product (InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, T init)
{
  while (first1!=last1) {
    init = init + (*first1)*(*first2);
               // or: init = binary_op1 (init, binary_op2(*first1,*first2));
    ++first1; ++first2;
  }
  return init;
}

//
// accumulate
template <class InputIterator, class T>
   T accumulate (InputIterator first, InputIterator last, T init)
{
  while (first!=last) {
    init = init + *first;  // or: init=binary_op(init,*first) for the binary_op version
    ++first;
  }
  return init;
}

//
// main function
void CalcAcceptanceSystematics_KH(){

  char tempname[200];
  TFile * infile = new TFile("LostLepton2_MuonEfficienciesFromAll_Elog408_.root","R");
  TH1* hAccPass, * hAccAll, * hAccPass_lowDphi, * hAccAll_lowDphi;
  TH1* hSumofSquareOfDev , * hSumofSquareOfDev_lowDphi;
  vector<TH1*> hAccVec, hAcc_lowDphiVec, hAcc_DeviationFromNomVec, hAcc_DeviationFromNom_lowDphiVec;

  //
  // Loop over all MC replicas for PDF uncertainties (and one default NNPDF)
  // -------------------------
  for(int iPDF=0; iPDF < 101; iPDF++){// 101 pdf 

    // get the pass
    sprintf(tempname,"Systematics/hAccPass_%d",iPDF);
    hAccPass = (TH1*) infile->Get(tempname)->Clone();

    // get the all
    sprintf(tempname,"Systematics/hAccAll_%d",iPDF);
    hAccAll = (TH1*) infile->Get(tempname)->Clone();    
    
    // divide the pass and all to get acc
    sprintf(tempname,"hAccVec_%d",iPDF);
    hAccVec.push_back(static_cast<TH1*>(hAccPass->Clone(tempname)));
    hAccVec[iPDF]->Divide(hAccPass,hAccAll,1,1,"B");

    cout << " bin45=> acc: " << hAccVec[iPDF]->GetBinContent(45) << endl;

    // lowDphi
    sprintf(tempname,"Systematics/hAccPass_lowDphi_%d",iPDF);
    hAccPass_lowDphi = (TH1*) infile->Get(tempname)->Clone();

    sprintf(tempname,"Systematics/hAccAll_lowDphi_%d",iPDF);
    hAccAll_lowDphi = (TH1*) infile->Get(tempname)->Clone();

    sprintf(tempname,"hAcc_lowDphiVec_%d",iPDF);
    hAcc_lowDphiVec.push_back(static_cast<TH1*>(hAccPass_lowDphi->Clone(tempname)));
    hAcc_lowDphiVec[iPDF]->Divide(hAccPass_lowDphi,hAccAll_lowDphi,1,1,"B");

    cout << " bin45=> acc: " << hAcc_lowDphiVec[iPDF]->GetBinContent(45) << endl;

    // calculate the deviation from nominal acceptance
    /*
    hAcc_DeviationFromNomVec.push_back(static_cast<TH1*>(hAccVec[iPDF]->Clone("hAcc_DeviationFromNomVec")));// copy
    hAcc_DeviationFromNomVec[iPDF]->Add(hAccVec[0],-1.0); // subtract the nominal from each acceptance: Acc - Acc_nom

    hAcc_DeviationFromNomVec[iPDF]->Multiply(hAcc_DeviationFromNomVec[iPDF]); // (Acc - Acc_nom)^2
    if(iPDF==0){
     hSumofSquareOfDev = static_cast<TH1*>(hAccAll->Clone("hSumofSquareOfDev"));
     hSumofSquareOfDev->Reset();
    }
    hSumofSquareOfDev->Add(hAcc_DeviationFromNomVec[iPDF]); // sum{ (Acc - Acc_nom)^2 }
    */

    // do the same for lowDphi
    /*
    hAcc_DeviationFromNom_lowDphiVec.push_back(static_cast<TH1*>(hAcc_lowDphiVec[iPDF]->Clone("hAcc_DeviationFromNom_lowDphiVec")));// copy
    hAcc_DeviationFromNom_lowDphiVec[iPDF]->Add(hAcc_lowDphiVec[0],-1.0); // subtract the nominal from each acceptance: Acc - Acc_nom

    hAcc_DeviationFromNom_lowDphiVec[iPDF]->Multiply(hAcc_DeviationFromNom_lowDphiVec[iPDF]); // (Acc - Acc_nom)^2
    if(iPDF==0){
      hSumofSquareOfDev_lowDphi = static_cast<TH1*>(hAccAll->Clone("hSumofSquareOfDev_lowDphi"));
      hSumofSquareOfDev_lowDphi->Reset();
    }
    hSumofSquareOfDev_lowDphi->Add(hAcc_DeviationFromNom_lowDphiVec[iPDF]); // sum{ (Acc - Acc_nom)^2 }
    */

  }// end of loop over pdf

  TH1 * hAccSysRMS = static_cast<TH1*>(hAccAll->Clone("hAccSysRMS"));
  hAccSysRMS->Reset();
  TH1 * hAccSysUp = static_cast<TH1*>(hAccAll->Clone("hAccSysUp"));
  hAccSysUp->Reset();
  TH1 * hAccSysDn = static_cast<TH1*>(hAccAll->Clone("hAccSysDn"));
  hAccSysDn->Reset();
  TH1 * hAccSysUp2 = static_cast<TH1*>(hAccAll->Clone("hAccSysUp2"));
  hAccSysUp2->Reset();
  TH1 * hAccSysDn2 = static_cast<TH1*>(hAccAll->Clone("hAccSysDn2"));
  hAccSysDn2->Reset();

  TH1 * hAccSysRMS_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSysRMS_lowDphi"));
  hAccSysRMS_lowDphi->Reset();
  TH1 * hAccSysUp_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSysUp_lowDphi"));
  hAccSysUp_lowDphi->Reset();
  TH1 * hAccSysDn_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSysDn_lowDphi"));
  hAccSysDn_lowDphi->Reset();
  TH1 * hAccSysUp2_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSysUp2_lowDphi"));
  hAccSysUp2_lowDphi->Reset();
  TH1 * hAccSysDn2_lowDphi = static_cast<TH1*>(hAccAll_lowDphi->Clone("hAccSysDn2_lowDphi"));
  hAccSysDn2_lowDphi->Reset();

  //
  // Loop over all acceptance bins
  // [and for each acceptance bin, compute the PDF variation]
  // ------------------------------
  std::vector<double> accPDFVariations;
  std::vector<double> accPDFVariations_lowDphi;
  std::vector<double> accPDFVariations_sort;
  std::vector<double> accPDFVariations_lowDphi_sort;
  printf("-------------------------------------------------------------------------------------------------------\n");
  printf("\nFor events passing delphi cut                                 | for lowDphi cut\n");
  printf("-------------------------------------------------------------------------------------------------------\n");
  printf("iacc bin, acc[0] (mean[1-100]), RMS,      max/min \n");
  printf("-------------------------------------------------------------------------------------------------------\n");
  for(int iacc=1; iacc<=hAccSysRMS->GetNbinsX(); iacc++){

    //cout << "bincontent: " <<  hAccSys->GetBinContent(iacc) << endl;
    accPDFVariations.clear();
    accPDFVariations_lowDphi.clear();
    accPDFVariations_sort.clear();
    accPDFVariations_lowDphi_sort.clear();
    for (int iPDF=1;iPDF<=100;iPDF++){
      accPDFVariations.push_back(hAccVec[iPDF]->GetBinContent(iacc));
      accPDFVariations_lowDphi.push_back(hAcc_lowDphiVec[iPDF]->GetBinContent(iacc));
      //if (iPDF==1||iPDF==2) std::cout << hAccVec[iPDF]->GetBinContent(iacc) << std::endl;
    }  
    accPDFVariations_sort=accPDFVariations;
    accPDFVariations_lowDphi_sort=accPDFVariations_lowDphi;
    sort(accPDFVariations_sort.begin(),accPDFVariations_sort.end());
    sort(accPDFVariations_lowDphi_sort.begin(),accPDFVariations_lowDphi_sort.end());

    double sum = accumulate(accPDFVariations.begin(), accPDFVariations.end(), 0.0);
    double mean = sum / accPDFVariations.size();

    double sq_sum = inner_product(accPDFVariations.begin(), accPDFVariations.end(), accPDFVariations.begin(), 0.0);
    double stdev = sqrt(sq_sum / accPDFVariations.size() - mean * mean);

    double max_dev = *max_element(accPDFVariations.begin(),accPDFVariations.end()) - hAccVec[0]->GetBinContent(iacc);
    double min_dev = *min_element(accPDFVariations.begin(),accPDFVariations.end()) - hAccVec[0]->GetBinContent(iacc);
    double max2_dev = accPDFVariations_sort[accPDFVariations_sort.size()-2]-hAccVec[0]->GetBinContent(iacc);
    double min2_dev = accPDFVariations_sort[1]-hAccVec[0]->GetBinContent(iacc);
  
    double sum_lowDphi = accumulate(accPDFVariations_lowDphi.begin(), accPDFVariations_lowDphi.end(), 0.0);
    double mean_lowDphi = sum_lowDphi / accPDFVariations_lowDphi.size();

    double sq_sum_lowDphi = inner_product(accPDFVariations_lowDphi.begin(), accPDFVariations_lowDphi.end(), accPDFVariations_lowDphi.begin(), 0.0);
    double stdev_lowDphi = sqrt(sq_sum_lowDphi / accPDFVariations_lowDphi.size() - mean_lowDphi * mean_lowDphi);
  
    double max_dev_lowDphi = *max_element(accPDFVariations_lowDphi.begin(),accPDFVariations_lowDphi.end()) - hAcc_lowDphiVec[0]->GetBinContent(iacc);
    double min_dev_lowDphi = *min_element(accPDFVariations_lowDphi.begin(),accPDFVariations_lowDphi.end()) - hAcc_lowDphiVec[0]->GetBinContent(iacc);
    double max2_dev_lowDphi = accPDFVariations_lowDphi_sort[accPDFVariations_lowDphi_sort.size()-2]        - hAcc_lowDphiVec[0]->GetBinContent(iacc);
    double min2_dev_lowDphi = accPDFVariations_lowDphi_sort[1]                                             - hAcc_lowDphiVec[0]->GetBinContent(iacc);

    printf("%5d %8.3f(%8.3f) +- %6.3f(RMS) +%6.3f%6.3f(max/min),+%6.3f%6.3f(2n max/min), %8.3f(%8.3f) +- %6.3f +%6.3f%6.3f(max/min)+%6.3f%6.3f(2nd max/min)\n",
	   iacc,
	   hAccVec[0]->GetBinContent(iacc),mean,stdev,max_dev,min_dev,max2_dev,min2_dev,
	   hAcc_lowDphiVec[0]->GetBinContent(iacc),mean_lowDphi,stdev_lowDphi,max_dev_lowDphi,min_dev_lowDphi,max2_dev_lowDphi,min2_dev_lowDphi
	   );

    hAccSysRMS->SetBinContent(iacc,stdev);
    hAccSysUp->SetBinContent(iacc,max_dev);
    hAccSysDn->SetBinContent(iacc,min_dev);
    hAccSysUp2->SetBinContent(iacc,max2_dev);
    hAccSysDn2->SetBinContent(iacc,min2_dev);

    hAccSysRMS_lowDphi->SetBinContent(iacc,stdev_lowDphi);  // sqrt[ sum{ (Acc - Acc_nom)^2 } ]
    hAccSysUp_lowDphi->SetBinContent(iacc,max_dev_lowDphi); 
    hAccSysDn_lowDphi->SetBinContent(iacc,min_dev_lowDphi); 
    hAccSysUp2_lowDphi->SetBinContent(iacc,max2_dev_lowDphi); 
    hAccSysDn2_lowDphi->SetBinContent(iacc,min2_dev_lowDphi); 
  
  }

  TFile * outFile = new TFile("AcceptanceSystematics_AllSamples.root","RECREATE");

  hAccSysRMS->Write();
  hAccSysUp->Write();
  hAccSysDn->Write();
  hAccSysUp2->Write();
  hAccSysDn2->Write();

  hAccSysRMS_lowDphi->Write();
  hAccSysUp_lowDphi->Write();
  hAccSysDn_lowDphi->Write();
  hAccSysUp2_lowDphi->Write();
  hAccSysDn2_lowDphi->Write();

  //hAccSysRMS->Print("all");
  
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
