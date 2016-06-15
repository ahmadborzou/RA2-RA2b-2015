//This is a root macro. Run it with root -l -b -q forCSA14.C++
//

#include "tophead.h"
#include "tdrstyle.C"
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<vector<int> > >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#endif

using namespace std;

const double cutCSVS = 0.679;
const double defaultMETcut = 200;
const double highMETcut = 350;

static const int minnJetsSelBagged = 1, maxnJetsSelBagged = -1;

const double minMETcut = 200, maxMETcut = -1;

static const int nMuonsSel = 0, nElectronsSel = 0;

static const int nJetsSel = 5, nJetsSelPt30Eta24 = 5, nJetsSelPt50Eta24 = 4, nJetsSelPt70Eta24 = 2;
static const double minJetPt = 30;
//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta24Arr[] = {   -1,       2.4,      30,    -1  };
static const double pt50Eta24Arr[] = {   -1,       2.4,      50,    -1  };
static const double pt70Eta24Arr[] = {   -1,       2.4,      70,    -1  };
static const double      dphiArr[] = {   -1,       4.7,      30,    -1  };
static const double      bTagArr[] = {   -1,       2.4,      30,    -1  };

static const double pt50Arr[] = {   -1,        -1,      50,    -1  };
static const double pt70Arr[] = {   -1,        -1,      70,    -1  };

const string      mcStr[] = {   "TTbar_8TeV", "TTbar_PUS14", "TTbar_PU20bx25" };
const double    xSecArr[] = {            234,      234,            234        };
const int       nEvtArr[] = {       21671578,  1000000,        1000000        };
static const int colors[] = {         kBlack,    kBlue,           kRed        };

const int nMC = sizeof(mcStr)/sizeof(mcStr[0]);

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt);

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS = 0.679, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi = 3, const double minAbsEta = -1, const double maxAbsEta = 4.7, const double minPt = 30, const double maxPt = -1);

TRandom3 *picker =0;

//const TString treeStrT = "stdStop_histAndTree"; TString extraDrawStrT = "";
const TString treeStrT = "stopTreeMaker"; TString extraDrawStrT = "";

static const double dataLumi = (803.197 + 82.136 + 4.385*1000 + 6.397*1000 + 495.003 + 7.266*1000)/1000.; // in fb-1
const double scaledToDataLumi = dataLumi; // in fb-1

double dataScale = 1.0;

vector<double> scalesVec;

vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

double template_cntEventsWeighted =0, template_cntEventsWeightedSquared =0;
double template_cntAftBaselineWeighted =0, template_cntAftBaselineWeightedSquared =0;

double template_cntEventsWeightedScaledMC = 0, template_cntEventsWeightedErrorScaledMC = 0; 
double template_cntAftBaselineWeightedScaledMC = 0, template_cntAftBaselineWeightedErrorScaledMC = 0;

vector<string> keyStringCachedVec;
vector<double> scaleMCCachedVec;
vector<int> colorCachedVec;

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH1D*> &h1_inputVec, const int nRebin =1, const TString optDrawStr ="");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH2D*> &h2_inputVec);

string keyStrForTheStage = "optimization";

std::vector<ULong64_t> evtIDvec;
std::vector<bool> evtPassBasicvec, evtPassStopvec;

TStopwatch timer;

vector<TH1D*> h1_metVec, h1_metphiVec;
vector<TH1D*> h1_met_allhadVec, h1_metphi_allhadVec;
vector<TH1D*> h1_met_leptonicVec, h1_metphi_leptonicVec;

vector<TH1D*> h1_nJetsVec, h1_nJets_allhadVec, h1_nJets_leptonicVec;
vector<TH1D*> h1_vtxSizeVec;

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

void initMCscales(){

   dataScale = scaledToDataLumi/dataLumi;
   cout<<"\nscaledToDataLumi : "<<scaledToDataLumi<<"  dataLumi : "<<dataLumi<<"  dataScale : "<<dataScale<<endl;
   for(int ib=0; ib<nMC; ib++){
      double perScale = scaledToDataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
      scalesVec.push_back(perScale);
      printf("%30s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
   }
   cout<<endl;
}

class histClass{
double * a;
TH1D * b_hist;
public:
void fill(int Nhists, double * eveinfarr_, TH1D * hist_){
a = eveinfarr_;
b_hist=hist_;
(*b_hist).Fill(*a);
for(int i=1; i<=Nhists ; i++){
(*(b_hist+i)).Fill(*(a+i),*a);
}
}
};


bool bg_type(string bg_ ,vector<TLorentzVector> * pvec){
if(bg_=="allEvents"){return 1;}

} //end of function bg_type


//void templatePlotsFunc(std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar", int verbose=0){
class templatePlotsFunc{///this is the main class

///Some variables
TString sampleKeyStringT;

int template_run, template_event, template_lumi, template_nm1, template_n0, template_np1, template_vtxSize;
double template_avg_npv, template_tru_npv;
int template_nJets , nbtag ;
double template_evtWeight;
double template_met, template_metphi;
double template_mht, template_ht, template_mhtphi;     
int template_nMuons, template_nElectrons,nLeptons;
double dPhi0, dPhi1, dPhi2; /// delta phi of first three jet with respect to MHT?????????
char tempname[200];
vector<TH1D > vec;
map<int, string> cutname;
map<int, string> eventType;
map<string , vector<TH1D> > cut_histvec_map;
map<string, map<string , vector<TH1D> > > map_map;
map<string, histClass> histobjmap;
histClass histObj;
string Process;
int Nhists,n_elec_mu,n_elec_mu_tot,n_tau_had,n_tau_had_tot,nLostLepton;


int loose_nIsoTrks; // number of isolated tracks with Pt>5 GeV and relIso < 0.5
vector<double> *loose_isoTrks_charge; // charge of the loose isolated tracks (see loose_nIsoTrks)
vector<double> *loose_isoTrks_iso; // isolation values (divided by Pt to get relIso) for the loose isolated tracks
vector<int> *loose_isoTrks_pdgId; // pdg id of the loose isolated tracks
vector<TLorentzVector> *loose_isoTrksLVec; // TLorentzVector of the loose isolated tracks (see loose_nIsoTrks)
//
int nIsoTrks_CUT; // number of isolated tracks with Pt>10 GeV, dR<0.3, dz<0.05 and relIso<0.1
vector<int> *forVetoIsoTrksidx; // indices of the isolated tracks (see nIsoTrks_CUT) (pointing to pfCandidate collection)
//
vector<double> *trksForIsoVeto_charge; // charges of the charged tracks for isolated veto studies
vector<int> *trksForIsoVeto_pdgId; // pdg id of the charged tracks for isolated veto studies
vector<int> *trksForIsoVeto_idx; // indices of the charged tracks for isolated veto studies (pointing to pfCandidate collection)
//
vector<double> *trksForIsoVeto_dz; // dz of the charged tracks for isolated veto studies
vector<double> *loose_isoTrks_dz; // dz of the loose isolated tracks
vector<double> *loose_isoTrks_mtw; // MT of the loose isolated tracks and MET
vector<int> *loose_isoTrks_idx; // indices of the loose isolated tracks (pointing to pfCandidate collection)
vector<TLorentzVector> *trksForIsoVetoLVec; // TLorentzVector of the charged tracks for isolated veto studies


//define different cuts here
bool threejet(){if(template_nJets>=3)return true; return false;}
bool ht(){if(template_ht>=500) return true; return false;}
bool mht(){if(template_mht>=200)return true; return false;}
bool dphi(){if(dPhi0>0.5 && dPhi1>0.3 && dPhi2>0.3)return true; return false;}
bool nolep(){if(template_nElectrons==0 && template_nMuons==0)return true; return false;}
bool fourjet(){if(template_nJets >= 4)return true; return false;}
bool fivejet(){if(template_nJets >= 5)return true; return false;}
bool sixjet(){if(template_nJets >= 6)return true; return false;}
bool highMht(){if(template_mht>=1000)return true; return false;}
bool highHt(){if(template_ht>=2500)return true; return false;}



///apply the cuts here
bool checkcut(string ss){
if(ss == cutname[0])return true;
if(ss== cutname[1]){if(threejet())return true;}
if(ss== cutname[2]){if(threejet() && ht())return true;}
if(ss== cutname[3]){if(threejet()&&ht()&&mht())return true;}
if(ss== cutname[4]){if(threejet()&&ht()&&mht()&&dphi())return true;}
if(ss== cutname[5]){if(threejet()&&ht()&&mht()&&dphi()&&nolep())return true;}
if(ss== cutname[6]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&fourjet())return true;}
if(ss== cutname[7]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&fivejet())return true;}
if(ss== cutname[8]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&sixjet())return true;}
if(ss== cutname[9]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&sixjet()&&highMht())return true;}
if(ss== cutname[10]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&sixjet()&&highHt())return true;}
if(ss== cutname[11]){if(threejet()&&ht()&&mht()&&dphi()&&nolep()&&sixjet()&&highHt()&&highMht())return true;}
return false;
}


public:
//constructor
templatePlotsFunc(std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar", int verbose=0, string Outdir="Results", string inputnumber="00"){
Process=sampleKeyString;
sampleKeyStringT=sampleKeyString;
  keyStringCachedVec.push_back(sampleKeyString);
  double sampleScaleMC = 1.0; int sampleColor = 1;
  for(int ib=0; ib<nMC; ib++){
     TString permcStrT(mcStr[ib]);
     if( permcStrT.Contains(sampleKeyString) ) { sampleColor = colors[ib]; } 
  }
  scaleMCCachedVec.push_back( sampleScaleMC );
  colorCachedVec.push_back( sampleColor );

  template_cntEventsWeightedScaledMC = 0;
  template_cntEventsWeightedErrorScaledMC = 0; 
  template_cntAftBaselineWeightedScaledMC = 0; template_cntAftBaselineWeightedErrorScaledMC = 0;

//build a vector of histograms
TH1D weight_hist = TH1D("weight", "Weight Distribution", 5,0,5);
vec.push_back(weight_hist);
TH1D RA2HT_hist = TH1D("HT","HT Distribution",50,0,5000);
vec.push_back(RA2HT_hist);
TH1D RA2MHT_hist = TH1D("MHT","MHT Distribution",100,0,5000);
vec.push_back(RA2MHT_hist);
TH1D RA2NJet_hist = TH1D("NJet","Number of Jets Distribution",10,0,20);
vec.push_back(RA2NJet_hist);
TH1D RA2NBtag_hist = TH1D("NBtag","Number of Btag Distribution",20,0,20);
vec.push_back(RA2NBtag_hist);
TH1D NLostLep_hist = TH1D("NLostLep","Number of Lost Lepton Distribution",20,0,20);
vec.push_back(NLostLep_hist);
TH1D nGenTauHad_hist = TH1D("nGenTauHad","Number of Gen. Had. Tau",20,0,20);
vec.push_back(nGenTauHad_hist);


Nhists=((int)(vec.size())-1);//-1 is because weight shouldn't be counted.
//initialize a map between string=cutnames and histvecs. copy one histvec into all of them. The histograms, though, will be filled differently.
cutname[0]="RA2nocut";cutname[1]="RA23Jetcut";cutname[2]="RA2HT500cut" ;cutname[3]="RA2MHT200cut" ;cutname[4]="RA2delphicut" ;cutname[5]="RA2noleptoncut" ;cutname[6]="RA24Jetcut" ;cutname[7]="RA25Jetcut" ;cutname[8]="RA26Jetcut" ;cutname[9]="RA2allbutHT2500cut" ;cutname[10]="RA2allbutMHT1000cut";cutname[11]= "RA2allcut";
for(int i=0; i<(int) cutname.size();i++){
cut_histvec_map[cutname[i]]=vec;
}


eventType[0]="allEvents";

//initialize a map between string and maps. copy the map of histvecs into each
for(int i=0; i< eventType.size();i++){
map_map[eventType[i]]=cut_histvec_map;
}
//initialize histobjmap
for(map<string , vector<TH1D> >::iterator it=cut_histvec_map.begin(); it!=cut_histvec_map.end();it++){
histobjmap[it->first]=histObj;
}

///////////////// Hongxuan
  TH1D *template_h1_met = new TH1D(sampleKeyStringT+"_h1_met", sampleKeyStringT+":  met; met", 100, 0, 1000); template_h1_met->Sumw2();
  TH1D *template_h1_metphi = new TH1D(sampleKeyStringT+"_h1_metphi", sampleKeyStringT+":  metphi; metphi", 100, -3.2, 3.2); template_h1_metphi->Sumw2();

  TH1D *template_h1_met_allhad = new TH1D(sampleKeyStringT+"_h1_met_allhad", sampleKeyStringT+":  met_allhad; met_allhad", 100, 0, 1000); template_h1_met_allhad->Sumw2();
  TH1D *template_h1_metphi_allhad = new TH1D(sampleKeyStringT+"_h1_metphi_allhad", sampleKeyStringT+":  metphi_allhad; metphi_allhad", 100, -3.2, 3.2); template_h1_metphi_allhad->Sumw2();

  TH1D *template_h1_met_leptonic = new TH1D(sampleKeyStringT+"_h1_met_leptonic", sampleKeyStringT+":  met_leptonic; met_leptonic", 100, 0, 1000); template_h1_met_leptonic->Sumw2();
  TH1D *template_h1_metphi_leptonic = new TH1D(sampleKeyStringT+"_h1_metphi_leptonic", sampleKeyStringT+":  metphi_leptonic; metphi_leptonic", 100, -3.2, 3.2); template_h1_metphi_leptonic->Sumw2();

  TH1D *template_h1_nJets = new TH1D(sampleKeyStringT+"_h1_nJets", sampleKeyStringT+":  number of jets; nJets", 20, 0, 20); template_h1_nJets->Sumw2();
  TH1D *template_h1_nJets_allhad = new TH1D(sampleKeyStringT+"_h1_nJets_allhad", sampleKeyStringT+":  number of jets; nJets_allhad", 20, 0, 20); template_h1_nJets_allhad->Sumw2();
  TH1D *template_h1_nJets_leptonic = new TH1D(sampleKeyStringT+"_h1_nJets_leptonic", sampleKeyStringT+":  number of jets; nJets_leptonic", 20, 0, 20); template_h1_nJets_leptonic->Sumw2();
  TH1D *template_h1_vtxSize = new TH1D(sampleKeyStringT+"_h1_vtxSize", sampleKeyStringT+":  number of vertices; vtxSize", 100, 0, 100); template_h1_vtxSize->Sumw2();
////////////////////////////////////////////////////////////////////////////////////
//


  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){
 
     bool isData = false;

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = 1.0;
//     for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ){ scaleMC = scalesVec[ib]; } }
     TString keyStringT(keyString);
//     if( keyStringT.Contains("Data") ){ scaleMC = dataScale; isData = true; }


     vector<TLorentzVector> *template_oriJetsVec = new vector<TLorentzVector>(); vector<double> *template_recoJetsBtagCSVS = new vector<double>();

     vector<TLorentzVector> *template_genDecayLVec =0;
     vector<int> *template_genDecayPdgIdVec =0, *template_genDecayIdxVec =0, *template_genDecayMomIdxVec =0;
     vector<string> *template_genDecayStrVec =0, *template_smsModelFileNameStrVec =0, *template_smsModelStrVec =0;
     double template_smsModelMotherMass, template_smsModelDaughterMass;

     vector<TLorentzVector> *template_genJetsLVec_myak5GenJetsNoNu =0, *template_genJetsLVec_myak5GenJetsNoNuNoStopDecays =0, *template_genJetsLVec_myak5GenJetsNoNuOnlyStopDecays =0;

     TTree * template_AUX = treeVec[ist];
     template_AUX->SetBranchStatus("*", 0);

///the variables
     template_AUX->SetBranchStatus("run", 1); template_AUX->SetBranchAddress("run", &template_run);
     template_AUX->SetBranchStatus("event", 1); template_AUX->SetBranchAddress("event", &template_event);
     template_AUX->SetBranchStatus("lumi", 1); template_AUX->SetBranchAddress("lumi", &template_lumi);
     template_AUX->SetBranchStatus("avg_npv", 1); template_AUX->SetBranchAddress("avg_npv", &template_avg_npv);
     template_AUX->SetBranchStatus("nm1", 1); template_AUX->SetBranchAddress("nm1", &template_nm1);
     template_AUX->SetBranchStatus("n0", 1); template_AUX->SetBranchAddress("n0", &template_n0);
     template_AUX->SetBranchStatus("np1", 1); template_AUX->SetBranchAddress("np1", &template_np1);
     template_AUX->SetBranchStatus("tru_npv", 1); template_AUX->SetBranchAddress("tru_npv", &template_tru_npv);
     template_AUX->SetBranchStatus("vtxSize", 1); template_AUX->SetBranchAddress("vtxSize", &template_vtxSize);
     template_AUX->SetBranchStatus("nJets", 1); template_AUX->SetBranchAddress("nJets", &template_nJets);
     template_AUX->SetBranchStatus("jetsLVec", 1); template_AUX->SetBranchAddress("jetsLVec", &template_oriJetsVec);
     template_AUX->SetBranchStatus("recoJetsBtag_0", 1); template_AUX->SetBranchAddress("recoJetsBtag_0", &template_recoJetsBtagCSVS);
     template_AUX->SetBranchStatus("evtWeight", 1); template_AUX->SetBranchAddress("evtWeight", &template_evtWeight);
     template_AUX->SetBranchStatus("met", 1); template_AUX->SetBranchAddress("met", &template_met);
     template_AUX->SetBranchStatus("metphi", 1); template_AUX->SetBranchAddress("metphi", &template_metphi);
     template_AUX->SetBranchStatus("mhtphi", 1); template_AUX->SetBranchAddress("mhtphi", &template_mhtphi);
     template_AUX->SetBranchStatus("nMuons_CUT", 1); template_AUX->SetBranchAddress("nMuons_CUT", &template_nMuons);
     template_AUX->SetBranchStatus("nElectrons_CUT", 1); template_AUX->SetBranchAddress("nElectrons_CUT", &template_nElectrons);

     template_AUX->SetBranchStatus("genDecayLVec", 1); template_AUX->SetBranchAddress("genDecayLVec", &template_genDecayLVec);
     template_AUX->SetBranchStatus("genDecayPdgIdVec", 1); template_AUX->SetBranchAddress("genDecayPdgIdVec", &template_genDecayPdgIdVec);
     template_AUX->SetBranchStatus("genDecayIdxVec", 1); template_AUX->SetBranchAddress("genDecayIdxVec", &template_genDecayIdxVec);
     template_AUX->SetBranchStatus("genDecayMomIdxVec", 1); template_AUX->SetBranchAddress("genDecayMomIdxVec", &template_genDecayMomIdxVec);
     template_AUX->SetBranchStatus("genDecayStrVec", 1); template_AUX->SetBranchAddress("genDecayStrVec", &template_genDecayStrVec);

template_AUX->SetBranchStatus("ht", 1); template_AUX->SetBranchAddress("ht", &template_ht);
template_AUX->SetBranchStatus("mht", 1); template_AUX->SetBranchAddress("mht", &template_mht);    


template_AUX->SetBranchStatus("loose_nIsoTrks", "1");template_AUX->SetBranchAddress("loose_nIsoTrks", &loose_nIsoTrks);
template_AUX->SetBranchStatus("nIsoTrks_CUT", "1");template_AUX->SetBranchAddress("nIsoTrks_CUT", &nIsoTrks_CUT);
template_AUX->SetBranchStatus("trksForIsoVeto_charge","1");template_AUX->SetBranchAddress("trksForIsoVeto_charge", &trksForIsoVeto_charge);
//template_AUX->SetBranchStatus("trksForIsoVeto_dz","1");template_AUX->SetBranchAddress("trksForIsoVeto_dz", &trksForIsoVeto_dz);
template_AUX->SetBranchStatus("loose_isoTrks_charge","1");template_AUX->SetBranchAddress("loose_isoTrks_charge", &loose_isoTrks_charge);
//template_AUX->SetBranchStatus("loose_isoTrks_dz","1");template_AUX->SetBranchAddress("loose_isoTrks_dz", &loose_isoTrks_dz);
template_AUX->SetBranchStatus("loose_isoTrks_iso","1");template_AUX->SetBranchAddress("loose_isoTrks_iso", &loose_isoTrks_iso);
//template_AUX->SetBranchStatus("loose_isoTrks_mtw", "1");template_AUX->SetBranchAddress("loose_isoTrks_mtw", &loose_isoTrks_mtw);
template_AUX->SetBranchStatus("trksForIsoVeto_pdgId","1");template_AUX->SetBranchAddress("trksForIsoVeto_pdgId", &trksForIsoVeto_pdgId);
//template_AUX->SetBranchStatus("trksForIsoVeto_idx","1");template_AUX->SetBranchAddress("trksForIsoVeto_idx", &trksForIsoVeto_idx);
template_AUX->SetBranchStatus("loose_isoTrks_pdgId","1");template_AUX->SetBranchAddress("loose_isoTrks_pdgId", &loose_isoTrks_pdgId);
//template_AUX->SetBranchStatus("loose_isoTrks_idx","1");template_AUX->SetBranchAddress("loose_isoTrks_idx", &loose_isoTrks_idx);
template_AUX->SetBranchStatus("forVetoIsoTrksidx","1");template_AUX->SetBranchAddress("forVetoIsoTrksidx", &forVetoIsoTrksidx);
//template_AUX->SetBranchStatus("trksForIsoVetoLVec","1");template_AUX->SetBranchAddress("trksForIsoVetoLVec", &trksForIsoVetoLVec);
template_AUX->SetBranchStatus("loose_isoTrksLVec","1");template_AUX->SetBranchAddress("loose_isoTrksLVec", &loose_isoTrksLVec);

 

     int template_Entries = template_AUX->GetEntries();
     std::cout<<"\n\n"<<keyString.c_str()<<"_Entries : "<<template_Entries<<"  scaleMC : "<<scaleMC<<std::endl;
   
     ofstream evtlistFile;
     if( keyStringT.Contains("Data") ) evtlistFile.open("evtlistData_aftAllCuts.txt");

     template_cntEventsWeighted =0; template_cntEventsWeightedSquared =0;
     template_cntAftBaselineWeighted =0; template_cntAftBaselineWeightedSquared =0;

n_elec_mu_tot=0;
n_tau_had_tot=0;
////Loop over all events
     for(int ie=0; ie<template_Entries; ie++){

        template_AUX->GetEntry(ie);
 
        if( ie==0 || ie == template_Entries-1 || ie%(template_Entries/10) == 0 ) std::cout<<"\n   Processing the "<<ie<<"th event ..."<<std::endl;

        double puWeight = 1.0;
        if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
//           puWeight = weightTruNPV(NumPUInteractions);
        }

        if( template_oriJetsVec->size() != template_recoJetsBtagCSVS->size() ){
           std::cout<<"template_oriJetsVec->size : "<<template_oriJetsVec->size()<<"  template_recoJetsBtagCSVS : "<<template_recoJetsBtagCSVS->size()<<std::endl;
        }

        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(template_met, 0, template_metphi, 0);

        int cntCSVS = countCSVS((*template_oriJetsVec), (*template_recoJetsBtagCSVS), cutCSVS, bTagArr);
        int cntNJetsPt30 = countJets((*template_oriJetsVec), pt30Arr);
        int cntNJetsPt30Eta24 = countJets((*template_oriJetsVec), pt30Eta24Arr);
        int cntNJetsPt50Eta24 = countJets((*template_oriJetsVec), pt50Eta24Arr);
        int cntNJetsPt70Eta24 = countJets((*template_oriJetsVec), pt70Eta24Arr);
//        vector<double> dPhiVec = calcDPhi((*template_oriJetsVec), template_metphi, 3, dphiArr);
        vector<double> dPhiVec = calcDPhi((*template_oriJetsVec), template_mhtphi, 3, dphiArr);

        dPhi0 = dPhiVec[0]; dPhi1 = dPhiVec[1]; dPhi2 = dPhiVec[2];

        bool passExtraCuts = true;
        bool passnJets = true, passdPhis = true, passBJets = true, passMET = true;

        if( cntNJetsPt70Eta24 < nJetsSelPt70Eta24 ){ passExtraCuts = false; passnJets = false; }
        if( cntNJetsPt50Eta24 < nJetsSelPt50Eta24 ){ passExtraCuts = false; passnJets = false; }
        if( cntNJetsPt30Eta24 < nJetsSelPt30Eta24 ){ passExtraCuts = false; passnJets = false; }

        if( dPhi0 < 0.5 || dPhi1 < 0.5 || dPhi2 < 0.3 ){ passExtraCuts = false; passdPhis = false; }
        if( !( ( minnJetsSelBagged == -1 || cntCSVS >= minnJetsSelBagged) && (maxnJetsSelBagged ==-1 || cntCSVS < maxnJetsSelBagged ) ) ){ passExtraCuts = false; passBJets = false; }

        if( !( (minMETcut == -1 || template_met >= minMETcut ) && (maxMETcut == -1 || template_met < maxMETcut) ) ){ passExtraCuts = false; passMET = false; }

// Parsing the gen information ...
        int cntgenTop = 0, cntleptons =0;

        for(int iv=0; iv<(int)template_genDecayLVec->size(); iv++){
           int pdgId = template_genDecayPdgIdVec->at(iv);
           if( abs(pdgId) == 6 ) cntgenTop ++;
           if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15 ) cntleptons++;
        }

        template_cntEventsWeighted += template_evtWeight * puWeight;
        template_cntEventsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

        template_h1_vtxSize->Fill(template_vtxSize, template_evtWeight * puWeight * scaleMC);

        template_h1_nJets->Fill(cntNJetsPt30Eta24, template_evtWeight * puWeight * scaleMC);

        template_h1_met->Fill(template_met, template_evtWeight * puWeight * scaleMC);
        template_h1_metphi->Fill(template_metphi, template_evtWeight * puWeight * scaleMC);

        if( cntleptons ==0 ){
           template_h1_nJets_allhad->Fill(cntNJetsPt30Eta24, template_evtWeight * puWeight * scaleMC);
           template_h1_met_allhad->Fill(template_met, template_evtWeight * puWeight * scaleMC);
           template_h1_metphi_allhad->Fill(template_metphi, template_evtWeight * puWeight * scaleMC);
        }else{
           template_h1_nJets_leptonic->Fill(cntNJetsPt30Eta24, template_evtWeight * puWeight * scaleMC);
           template_h1_met_leptonic->Fill(template_met, template_evtWeight * puWeight * scaleMC);
           template_h1_metphi_leptonic->Fill(template_metphi, template_evtWeight * puWeight * scaleMC);
        }

        if( verbose >=1 ){
           std::cout<<"\nie : "<<ie<<std::endl; 
           std::cout<<"genDecayStr : "<<template_genDecayStrVec->front().c_str()<<std::endl;
           printf("((pdgId,index/MomIndex):(E/Pt)) \n");
           for(int iv=0; iv<(int)template_genDecayLVec->size(); iv++){
              int pdgId = template_genDecayPdgIdVec->at(iv);
              printf("((%d,%d/%d):(%6.2f/%6.2f))  ", pdgId, template_genDecayIdxVec->at(iv), template_genDecayMomIdxVec->at(iv), template_genDecayLVec->at(iv).E(), template_genDecayLVec->at(iv).Pt());
           }
        }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////
//Isolated track section
if(ie<100){
cout << "event#: " << ie << endl;
printf("loose_nIsoTrks: %d, nIsoTrks_CUT: %d, trksForIsoVeto_charge.size(): %d, loose_isoTrks_charge.size(): %d, loose_isoTrks_iso.size(): %d, trksForIsoVeto_pdgId->size(): %d, loose_isoTrks_pdgId->size(): %d, forVetoIsoTrksidx->size(): %d, loose_isoTrksLVec->size(): %d  \n",loose_nIsoTrks,nIsoTrks_CUT,trksForIsoVeto_charge->size(),loose_isoTrks_charge->size(),loose_isoTrks_iso->size(),trksForIsoVeto_pdgId->size(),loose_isoTrks_pdgId->size(), forVetoIsoTrksidx->size(),loose_isoTrksLVec->size());
}


///In this part we would like to identify lost leptons and hadronic taus. To do so we use the generator truth information. We first check how many leptons(e and mu) are in the event and compare with the isolated+reconstructed ones. 

n_elec_mu=0;
for(int iv=0; iv<(int)template_genDecayLVec->size(); iv++){
int pdgId = template_genDecayPdgIdVec->at(iv);
if( abs(pdgId) == 11 || abs(pdgId) == 13 ) n_elec_mu++;
}
n_elec_mu_tot+=n_elec_mu;

/*if(ie < 100){
printf("event#: %d, #recElec: %d, #recMu: %d, #trueElecMu: %d \n", ie , template_nElectrons , template_nMuons , n_elec_mu);
}*/
nLostLepton=n_elec_mu-template_nElectrons-template_nMuons;
//cout << " event #: " << ie << endl;
n_tau_had=0;
for(int iv=0; iv<(int)template_genDecayLVec->size(); iv++){
int pdgId = template_genDecayPdgIdVec->at(iv);
if( abs(pdgId) == 15 ){
int index=template_genDecayIdxVec->at(iv);
for(int ivv=0; ivv<(int)template_genDecayLVec->size(); ivv++){
int MomIndex=template_genDecayMomIdxVec->at(ivv);
int secpdg = template_genDecayPdgIdVec->at(ivv);
if(MomIndex==index && secpdg > 40){
//printf("This is a tau. TauIndex: %d, TauDaughterID: %d \n",MomIndex, secpdg);
n_tau_had++;
}
}
}
}
n_tau_had_tot+=n_tau_had;

nbtag=0;
//Number of B-jets
for(int i=0; i<template_recoJetsBtagCSVS->size();i++){
if(template_recoJetsBtagCSVS->at(i) > 0.679)nbtag+=1;
}//end of the loop 

nLeptons= (int)(template_nElectrons+template_nMuons);
//build and array that contains the quantities we need a histogram for. Here order is important and must be the same as RA2nocutvec
double eveinfvec[] = {template_evtWeight, template_ht, template_mht , cntNJetsPt50Eta24, nbtag,nLostLepton,n_tau_had }; //the last one gives the RA2 defined number of jets.

//loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"
for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map.begin(); itt!=map_map.end();itt++){//this will be terminated after the cuts

//determine what type of background should pass
if(bg_type(itt->first , template_genDecayLVec)==true){//all the cuts are inside this

//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts

////loop over cut names and fill the histograms
for(map<string , vector<TH1D> >::iterator ite=cut_histvec_map.begin(); ite!=cut_histvec_map.end();ite++){
if(checkcut(ite->first)==true){histobjmap[ite->first].fill(Nhists,&eveinfvec[0] ,&itt->second[ite->first][0]);}
}//end of loop over cut names


////EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts


}//end of bg_type determination
}//end of loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"

}////end of loop over all events
cout << "# of hadronic tau: " << n_tau_had_tot << endl;
cout << "# of True Elec+Muon: " << n_elec_mu_tot <<endl;
//open a file to write the histograms
sprintf(tempname,"%s/results_%s_%s.root",Outdir.c_str(),Process.c_str(),inputnumber.c_str());
TFile *resFile = new TFile(tempname, "RECREATE");
TDirectory *cdtoitt;
TDirectory *cdtoit;

// Loop over different event categories (e.g. "All events, Wlnu, Zll, Zvv, etc")
 for(int iet=0;iet<(int)eventType.size();iet++){
 for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map.begin(); itt!=map_map.end();itt++){
 if (eventType[iet]==itt->first){
 //KH
 //std::cout << (itt->first).c_str() << std::endl;
 cdtoitt = resFile->mkdir((itt->first).c_str());
 cdtoitt->cd();

for(int i=0; i< (int)cutname.size();i++){
for(map<string , vector<TH1D> >::iterator it=itt->second.begin(); it!=itt->second.end();it++){
if (cutname[i]==it->first){
cdtoit = cdtoitt->mkdir((it->first).c_str());
cdtoit->cd();
int nHist = it->second.size();
for(int i=0; i<nHist; i++){//since we only have 4 type of histograms 
sprintf(tempname,"%s_%s_%s",it->second[i].GetName(),(it->first).c_str(),(itt->first).c_str());
it->second[i].Write(tempname);
}
cdtoitt->cd();
}
}
}
}
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     template_cntEventsWeightedScaledMC += template_cntEventsWeighted * scaleMC;
     template_cntEventsWeightedErrorScaledMC += template_cntEventsWeighted * scaleMC * scaleMC;

  } 

  template_cntEventsWeightedErrorScaledMC = sqrt(template_cntEventsWeightedErrorScaledMC);

  std::cout<<sampleKeyString.c_str()<<"_cntEventsWeightedScaledMC : "<<template_cntEventsWeightedScaledMC<<" +- "<<template_cntEventsWeightedErrorScaledMC<<std::endl;

  h1_metVec.push_back((TH1D*)template_h1_met->Clone());
  h1_metphiVec.push_back((TH1D*)template_h1_metphi->Clone());

  h1_met_allhadVec.push_back((TH1D*)template_h1_met_allhad->Clone());
  h1_metphi_allhadVec.push_back((TH1D*)template_h1_metphi_allhad->Clone());

  h1_met_leptonicVec.push_back((TH1D*)template_h1_met_leptonic->Clone());
  h1_metphi_leptonicVec.push_back((TH1D*)template_h1_metphi_leptonic->Clone());

  h1_nJetsVec.push_back((TH1D*)template_h1_nJets->Clone()); h1_nJets_allhadVec.push_back((TH1D*)template_h1_nJets_allhad->Clone()); h1_nJets_leptonicVec.push_back((TH1D*)template_h1_nJets_leptonic->Clone());

  h1_vtxSizeVec.push_back((TH1D*)template_h1_vtxSize->Clone());

}//end of class constructor templatePlotsFunc
};//end of class templatePlotsFunc


void forCSA14(){
 
  std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

  picker = new TRandom3(glbfSeed);

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  initMCscales(); ///scale different samples

  initPUinput("PUData_dist.root", "pileup"); //Handles pileup. Coudln't figure out how. For later ???????????????????????????????

  std::vector<TTree*> treeVec;
  std::vector<std::string> subSampleKeysVec;

  char filenames[500], names[500];
  std::vector<std::string> filesTTJets_8TeVVec, filesTTJets_PUS14Vec, filesTTJets_PU20bx25Vec;

///read the file names from the .txt files and load them to a vector.
  ifstream finTTJets_8TeV("filelist_TTJets_8TeV.txt"); while( finTTJets_8TeV.getline(filenames, 500) ){ filesTTJets_8TeVVec.push_back(filenames); }
  ifstream finTTJets_PUS14("filelist_TTJets_PUS14.txt"); while( finTTJets_PUS14.getline(filenames, 500) ){ filesTTJets_PUS14Vec.push_back(filenames); }
  ifstream finTTJets_PU20bx25("filelist_TTJets_PU20bx25.txt"); while( finTTJets_PU20bx25.getline(filenames, 500) ){ filesTTJets_PU20bx25Vec.push_back(filenames); }

/*
// TTJets
  std::cout<<"\nProcessing TTJets ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *TTJets_AUX = new TChain(treeStrT+"/AUX");
  for(unsigned int in=0; in<filesTTJetsVec.size(); in++){ TTJets_AUX->Add(filesTTJetsVec.at(in).c_str()); }
  treeVec.push_back(TTJets_AUX); subSampleKeysVec.push_back("TTbar");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar",0,"Results","00");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
*/

// TTJets_PUS14
  std::cout<<"\nProcessing TTJets_PUS14 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *TTJets_PUS14_AUX = new TChain(treeStrT+"/AUX");
  for(unsigned int in=0; in<filesTTJets_PUS14Vec.size(); in++){ TTJets_PUS14_AUX->Add(filesTTJets_PUS14Vec.at(in).c_str()); }
  treeVec.push_back(TTJets_PUS14_AUX); subSampleKeysVec.push_back("TTbar_PUS14");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar_PUS14",0,"Results","00");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

// TTJets_PU20bx25
  std::cout<<"\nProcessing TTJets_PU20bx25 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *TTJets_PU20bx25_AUX = new TChain(treeStrT+"/AUX");
  for(unsigned int in=0; in<filesTTJets_PU20bx25Vec.size(); in++){ TTJets_PU20bx25_AUX->Add(filesTTJets_PU20bx25Vec.at(in).c_str()); }
  treeVec.push_back(TTJets_PU20bx25_AUX); subSampleKeysVec.push_back("TTbar_PU20bx25");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar_PU20bx25",0,"Results","00");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

// TTJets_8TeV
  std::cout<<"\nProcessing TTJets_8TeV ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *TTJets_8TeV_AUX = new TChain(treeStrT+"/AUX");
  for(unsigned int in=0; in<filesTTJets_8TeVVec.size(); in++){ TTJets_8TeV_AUX->Add(filesTTJets_8TeVVec.at(in).c_str()); }
  treeVec.push_back(TTJets_8TeV_AUX); subSampleKeysVec.push_back("TTbar_8TeV");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar_8TeV",0,"Results","00");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

// Plotting

     //  For the frame
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(kBlack);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(kBlack);
  tdrStyle->SetFrameLineStyle(0);
  tdrStyle->SetFrameLineWidth(1);

     //  For the Pad
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

     //  For the axis
  tdrStyle->SetAxisColor(1,"XYZ");
  tdrStyle->SetTickLength(0.03,"XYZ");
  tdrStyle->SetNdivisions(505,"XYZ");
  tdrStyle->SetPadTickX(1);
  tdrStyle->SetPadTickY(1);
  tdrStyle->SetStripDecimals(kFALSE);

  tdrStyle->SetLabelSize(0.050, "XYZ");

  tdrStyle->SetPadTopMargin(0.1); tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadLeftMargin(0.15); tdrStyle->SetPadRightMargin(0.15);

  tdrStyle->SetOptStat(1111);

  tdrStyle->SetHistLineWidth(1);

  tdrStyle->SetPaintTextFormat("4.2f");

  unsigned int ntype = keyStringCachedVec.size();
  std::cout<<"\nntype : "<<ntype<<std::endl;

  TCanvas *cs = new TCanvas("cs", "cs", 1200, 900);
  int divW=2, divH=2;
  cs->Divide(divW, divH);

  cs->Print("allINone_"+treeStrT+".pdf[");
  draw1DallINone(cs, divW*divH, h1_metVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_metphiVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_met_allhadVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_metphi_allhadVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_met_leptonicVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_metphi_leptonicVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_nJetsVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nJets_allhadVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nJets_leptonicVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_vtxSizeVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  cs->Print("allINone_"+treeStrT+".pdf]");

  TFile * outfile = new TFile("out_CSA14.root", "RECREATE");
  for(unsigned int iv=0; iv<h1_metVec.size(); iv++){
     h1_metVec[iv]->Write(); h1_metphiVec[iv]->Write();
     h1_met_allhadVec[iv]->Write(); h1_metphi_allhadVec[iv]->Write();
     h1_met_leptonicVec[iv]->Write(); h1_metphi_leptonicVec[iv]->Write();

     h1_nJetsVec[iv]->Write(); h1_nJets_allhadVec[iv]->Write(); h1_nJets_leptonicVec[iv]->Write();
     h1_vtxSizeVec[iv]->Write();
  }
  outfile->Write(); outfile->Close();
}

// Handle my default value of trunpv==-1, this is the case for data.
double weightTruNPV(int trunpv){
      
   double w = 1.;
   if (trunpv < static_cast<int> (puWeights_.size()) && trunpv >=0) {
      w = puWeights_.at(trunpv);
   } else if( trunpv >= static_cast<int> (puWeights_.size()) ) {
      std::cerr << "WARNING ... number of PU vertices = " << trunpv
            << " out of histogram binning." << std::endl;
   }  
      
   return w;
}     
      
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName){

   puWeights_.clear();

   TFile *puDatafile = new TFile(puDatafileName.c_str());
   TH1D *data_npu_estimated = (TH1D*) puDatafile->Get(puDatahistName.c_str());

   double Summer12_S10[60] = {
      2.560E-06,
      5.239E-06,
      1.420E-05,
      5.005E-05,
      1.001E-04,
      2.705E-04,
      1.999E-03,
      6.097E-03,
      1.046E-02,
      1.383E-02,
      1.685E-02,
      2.055E-02,
      2.572E-02,
      3.262E-02,
      4.121E-02,
      4.977E-02,
      5.539E-02,
      5.725E-02,
      5.607E-02,
      5.312E-02,
      5.008E-02,
      4.763E-02,
      4.558E-02,
      4.363E-02,
      4.159E-02,
      3.933E-02,
      3.681E-02,
      3.406E-02,
      3.116E-02,
      2.818E-02,
      2.519E-02,
      2.226E-02,
      1.946E-02,
      1.682E-02,
      1.437E-02,
      1.215E-02,
      1.016E-02,
      8.400E-03,
      6.873E-03,
      5.564E-03,
      4.457E-03,
      3.533E-03,
      2.772E-03,
      2.154E-03,
      1.656E-03,
      1.261E-03,
      9.513E-04,
      7.107E-04,
      5.259E-04,
      3.856E-04,
      2.801E-04,
      2.017E-04,
      1.439E-04,
      1.017E-04,
      7.126E-05,
      4.948E-05,
      3.405E-05,
      2.322E-05,
      1.570E-05,
      5.005E-06
   };

   puWeights_.resize(60);
   double s = 0.0;
   for (int npu = 0; npu < 60; ++npu) {
      double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));
      puWeights_[npu] = npu_estimated / Summer12_S10[npu];
      s += npu_estimated;
   }

   // normalize weights such that the total sum of weights over the whole sample is 1.0, i.e., sum_i  puWeights_[i] * Summer2012[i] should be 1.0 (!)
   for (int npu = 0; npu < 60; ++npu) {
      puWeights_[npu] /= s;
   }

   return 1;
}

void draw2DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH2D*> &h2_inputVec){

  int cntPadIdx = 0;
  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){
     if( it == 0 ) cntPadIdx = 0;
     TString keyStringT(keyStringCachedVec[it]); 
     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" ) continue; 
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(0);
     h2_inputVec[it]->Draw("colz");
  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); }
}


void draw1DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH1D*> &h1_inputVec, const int nRebin, const TString optDrawStr){
  
  int statusLogy = 0;
  if( optDrawStr.Contains("SetLogy") ) statusLogy =1;
  int doNormalization = 0;
  if( optDrawStr.Contains("normalization") ) doNormalization =1;

  int cntPadIdx = 0;
  vector<TH1D*> h1_stackedVec; TH1D * h1_dataCached =0, * h1_signal1Cached =0, *h1_signal2Cached =0;

  vector<TString> stackedKeyStringTVec;

  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){

     TString keyStringT(keyStringCachedVec[it]); 

     if( it == 0 ){ cntPadIdx = 0; h1_stackedVec.clear(); }

     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" ) continue; 
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(statusLogy);

     TH1D *tmpHist = (TH1D*) h1_inputVec[it]->Clone(); 

     tmpHist->Rebin(nRebin); tmpHist->Scale(scaleMCCachedVec[it]); 

     tmpHist->SetLineColor(colorCachedVec[it]); tmpHist->SetMarkerColor(colorCachedVec[it]); tmpHist->SetMarkerStyle(6); tmpHist->SetMarkerSize(0.25);

     if( keyStringT.Contains("Data") ){ tmpHist->SetLineColor(kBlack); tmpHist->SetMarkerColor(kBlack); }

     if( !statusLogy ){
        tmpHist->SetAxisRange(0, tmpHist->GetMaximum()*1.5, "Y");
     }else{
        tmpHist->SetAxisRange(1e-1, tmpHist->GetMaximum()*32*5, "Y");
     }

     if( keyStringT.Contains("Data") || keyStringT.Contains("Signal") ){
        tmpHist->Draw("e");
     }else{
        tmpHist->Draw("hist");
     }
     
     if( !keyStringT.Contains("Data") && !keyStringT.Contains("Signal") ){
        stackedKeyStringTVec.push_back(keyStringT);
        if( h1_stackedVec.empty() ){
           h1_stackedVec.push_back( (TH1D*) tmpHist->Clone() ); 
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] ); 
        }else{
           TH1D *tmpSumHist = (TH1D*) tmpHist->Clone();
           tmpSumHist->Add(h1_stackedVec.back(), 1); 
           h1_stackedVec.push_back( (TH1D*) tmpSumHist->Clone() ); 
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] ); 
        }
     }
     if( keyStringT.Contains("Data") ){ h1_dataCached = (TH1D*) tmpHist->Clone(); }
     if( keyStringT.Contains("Signal") ){
        if( !h1_signal1Cached ) h1_signal1Cached = (TH1D*) tmpHist->Clone();
        else h1_signal2Cached = (TH1D*) tmpHist->Clone();
     }

  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); pad->SetLogy(statusLogy); }

  Float_t legendX1 = .60;
  Float_t legendX2 = .85;
  Float_t legendY1 = .55;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.030);

  cs->cd(lastPadIdx);
/*
  if( !statusLogy ){
     h1_dataCached->SetAxisRange(0, h1_dataCached->GetMaximum()*1.5, "Y");
  }else{
     h1_dataCached->SetAxisRange(1e-1, h1_dataCached->GetMaximum()*32, "Y");
  }
*/

  if( h1_dataCached ){
     double dataIntegral = h1_dataCached->Integral();
     double sumTotIntegral = h1_stackedVec.back()->Integral();
     double normScale = dataIntegral/sumTotIntegral;
   
     h1_dataCached->Draw("e");
     catLeg1->AddEntry(h1_dataCached, "Data");
     for(unsigned int is=0; is<h1_stackedVec.size(); is++){
        unsigned int ivsIdx = h1_stackedVec.size()-is-1;
        TH1D * tmpStacked = (TH1D*) h1_stackedVec[ivsIdx]->Clone();
        if( doNormalization ) tmpStacked->Scale(normScale);
        tmpStacked->Draw("hist same");
        catLeg1->AddEntry(tmpStacked, stackedKeyStringTVec[ivsIdx]);
     }
     h1_signal1Cached->SetLineColor(kRed); h1_signal1Cached->SetMarkerColor(kRed); h1_signal1Cached->SetLineStyle(3); 
     h1_signal1Cached->Draw("same");
     h1_dataCached->Draw("same e");
   
     catLeg1->AddEntry(h1_signal1Cached, "Signal");
     catLeg1->SetFillColor(kWhite);
     catLeg1->SetBorderSize(0);
     catLeg1->Draw();
   
     TPad *lastPad = (TPad*) cs->GetPad(lastPadIdx);
     lastPad->RedrawAxis();
  }

}

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   return countJets(inputJets, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){
         cntNJets ++;
      }
   }
   return cntNJets;
}

vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr){
   return calcDPhi(inputJets, metphi, nDPhi, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   vector<double> outDPhiVec(nDPhi, 999);
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( cntNJets < nDPhi ){
            double perDPhi = fabs(TVector2::Phi_mpi_pi( inputJets[ij].Phi() - metphi ));
            outDPhiVec[cntNJets] = perDPhi;
         }
         cntNJets ++;
      }
   }

   return outDPhiVec;///this is a vector whose components are delta phi of each jet with met.
}

int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr){
   return countCSVS(inputJets, inputCSVS, CSVS, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( inputCSVS[ij] > CSVS ) cntNJets ++;
      }
   }
   return cntNJets;
}

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr){
   return adjustbJetsCSVS(inputJets, inputCSVS, CSVS, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int adjustJetIdx = -1;
   double maxCSVS = -1;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( adjustJetIdx == -1 ){ maxCSVS = inputCSVS[ij]; adjustJetIdx = ij; }
         else if( maxCSVS < inputCSVS[ij] ){ maxCSVS = inputCSVS[ij]; adjustJetIdx = ij; }
      }
   }
   if( maxCSVS >= CSVS ){
      std::cout<<"ERROR ... adjustbJetsCSVS : already found jets with maxCSVS : "<<maxCSVS<<"  larger than the CSVS cut : "<<CSVS<<std::endl;
      return -1;
   }
   if( adjustJetIdx != -1 ){
      inputCSVS[adjustJetIdx] = 1.0;
   }

   return adjustJetIdx;
}
