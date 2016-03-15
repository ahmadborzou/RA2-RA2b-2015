#include "TH1.h"
#include "TH2.h"

namespace bootstrapUtils{


  /* KH
  Copy contents of histogram filled for each event to another histogram accumulated
  over the entire sample.
  Separate functions are prepared for TH1 and TH2.
  For some histogram, histogram without weights (i.e. one entry in a histogram bin
  for a event, if an event contribute to this bin even with a very small probability)
  are made, and the functions with four aurguments can be used. _noW means "no weight".  

  void HistogramFillForEventTH1(TH1* h, TH1* h_noW, TH1* h_evt, TH1* h_noW_evt)
  void HistogramFillForEventTH2(TH2* h, TH2* h_noW, TH2* h_evt, TH2* h_noW_evt){
  void HistogramFillForEventTH1(TH1* h, TH1* h_evt)
  void HistogramFillForEventTH2(TH2* h, TH2* h_evt)

  */
/*
  An example:
  
  Assume there is one event and bootstrap is on. A histgram will be filled 2x50 times. Assume there is only one bin in the hist and it gets filled 
  with the same weight every time. So the weight is 0.01 and the content is 1=2x50x0.01
  The uncertainty for that bin is therefore sqrt( 2x50x(0.01)^2 ) = 0.1
  Bootstrap should not change the uncertainty value but is a way to better read the template. So, we should see how the uncertainty is when bootstrap  is off. In that case the bin gets filled once with weight one. so uncertinty is sqrt( 1x(1)^2)=1
  which is different than 0.1 and is the correct value. 
  To avoid this error we introduce a new hist and fill the corresponding bin with a weight equal to the content of the origianl hist's bin. 
  Therefore the hist will be filled once ( there is one bin ) with weight 1 so the uncertainty is also 1

*/

  // Copy histogram contents for each event - 1D
  void HistogramFillForEventTH1(TH1* h, TH1* h_noW, TH1* h_evt, TH1* h_noW_evt){

      for (int ibin=0;ibin<h_evt->GetNbinsX()+2;ibin++){
        double bin     = h_evt->GetBinCenter(ibin);
        double content = h_evt->GetBinContent(ibin);
        double entry   = h_noW_evt->GetBinContent(ibin);
        //KH std::cout << "bin content: " << bin << " " << content << " " << entry << std::endl;
        if (entry!=0.){
          h->Fill(bin,content);
          h_noW->Fill(bin);
        }
      }

      h_evt->Reset();
      h_noW_evt->Reset();

  }

  // Copy histogram contents for each event - 2D
  void HistogramFillForEventTH2(TH2* h, TH2* h_noW, TH2* h_evt, TH2* h_noW_evt){

      for (int ibin=0;ibin<h_evt->GetNbinsX()+2;ibin++){
        for (int jbin=0;jbin<h_evt->GetNbinsY()+2;jbin++){
          double binX     = h_evt->GetXaxis()->GetBinCenter(ibin);
          double binY     = h_evt->GetYaxis()->GetBinCenter(jbin);
          double content  = h_evt->GetBinContent(ibin,jbin);
          double entry    = h_noW_evt->GetBinContent(ibin,jbin);
          if (entry!=0.){
            h->Fill(binX,binY,content);
            h_noW->Fill(binX,binY);
          }
        }
      }

      h_evt->Reset();
      h_noW_evt->Reset();

  }

  // Copy histogram contents for each event - 1D
  void HistogramFillForEventTH1(TH1* h, TH1* h_evt){

      for (int ibin=0;ibin<h_evt->GetNbinsX()+2;ibin++){
        double bin     = h_evt->GetBinCenter(ibin);
        double content = h_evt->GetBinContent(ibin);
        //KH double entry   = h_noW_evt->GetBinContent(ibin);
        //KH std::cout << "bin content: " << bin << " " << content << " " << entry << std::endl;
        if (content != 0.){
          h->Fill(bin,content);
          //KH h_noW->Fill(bin);
        }
      }

      h_evt->Reset();
      //h_noW_evt->Reset();

  }

  //KH-Feb2016-starts
  // Copy histogram contents for each event - 1D
  void HistogramFillForEventCorrelation(TH2* h, TH1* h_evt){

      for (int ibin=0;ibin<h_evt->GetNbinsX()+2;ibin++){

        double binX     = h_evt->GetBinCenter(ibin);
        double contentX = h_evt->GetBinContent(ibin);
        if (contentX != 0.){
          h->Fill(binX,binX,contentX); // diagonal entries
        }

	for (int jbin=0;jbin<ibin;jbin++){
	  double binY     = h_evt->GetBinCenter(jbin);
	  double contentY = h_evt->GetBinContent(jbin);	  
	  double content = TMath::Min(contentX,contentY); // how this line works in MC events with negative weights
	                                                  // is not clear yet. Needs to be revisited.
	  if (contentX != 0. && contentY != 0.){
	    h->Fill(binX,binY,contentY); // diagonal entries
	    h->Fill(binY,binX,contentX); // diagonal entries
	  }
	}

      }

      //h_evt->Reset();

  }
  //KH-Feb2016-ends

  // Copy histogram contents for each event - 2D
  void HistogramFillForEventTH2(TH2* h, TH2* h_evt){

      for (int ibin=0;ibin<h_evt->GetNbinsX()+2;ibin++){
        for (int jbin=0;jbin<h_evt->GetNbinsY()+2;jbin++){
          double binX     = h_evt->GetXaxis()->GetBinCenter(ibin);
          double binY     = h_evt->GetYaxis()->GetBinCenter(jbin);
          double content  = h_evt->GetBinContent(ibin,jbin);
          //KH double entry    = h_noW_evt->GetBinContent(ibin,jbin);
          if (content != 0.){
            h->Fill(binX,binY,content);
            //KH h_noW->Fill(binX,binY);
          }
        }
      }

      h_evt->Reset();
      //h_noW_evt->Reset();

  }



}
