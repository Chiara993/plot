#include <TGraph.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>
#include <TLegend.h>
#include <TChain.h>
//#include <vector.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <sstream>


void conv_root()
{
  FILE *fp = fopen("matrix_pred_test_20201120.txt","rt");
	
  Double_t t,p;

	TFile* fr = new TFile("matrix_pred_test_20201120.root","RECREATE");

	TNtuple* tr = new TNtuple("res","res","p:t");

  while(!feof(fp)) 
	{
		if(0>=fscanf(fp,"%lf %lf",&p, &t))
			continue;
		tr->Fill(p,t);
  }
	fclose(fp);

	tr->Write();
	fr->Close();
	
}

void plot(Float_t scale, Float_t minV){
  //prendo i dati dal tree
  TFile f("matrix_pred_test_20201120.root");
  TTree *myevents = new TTree();
  myevents = (TTree*)((f.Get("res")));
  //File degli histos
  TFile h("histo.root", "recreate");

 
  Int_t size =myevents->GetEntries();
  Float_t pred, test;
  //Float_t p[size],t[size];
  
  myevents->SetBranchAddress("p", &pred);
  
  myevents->SetBranchAddress("t", &test);

  
    TH1F *hpred = new TH1F("hpred","hpred;z_{pred} [#mum]; Entries",100,-1.1,0.3);
    TH1F *htest = new TH1F("htest","htest;z_{test} [#mum]; Entries",100, -1.1,0.3) ;
    TH1F *hdelta = new TH1F("hdelta","hpred; #Delta z [#mum]; Entries",100, -0.5,0.5);
    
  
  TGraph *g1 = new TGraph();
  TGraph *g2 = new TGraph();
  TF1 *fgaus = new TF1("fgaus","gaus(0)",-0.1, 0.1);

  Float_t delta[size];
  
  cout<< myevents->GetEntries() << endl;
  for (Int_t i=0; i < size ; i++){
    myevents->GetEntry(i);
    //cout<<pred<< " "<< test<< endl;
    pred = pred*scale+minV;
    test = test*scale+minV;
    hpred->Fill(pred);  
    htest->Fill(test);
    Float_t delta = pred - test;
    hdelta->Fill(pred-test);
    
    g1->SetPoint(i,test,pred);
    g2->SetPoint(i,test,delta);
  }
  TCanvas *c1 = new TCanvas("c1");

 
  g1->GetXaxis()->SetLimits(-1.1 ,0.3 );
  g1->GetYaxis()->SetRangeUser(-1.1 ,0.3);
    
  g1->SetTitle("; z_{test}[#mum]; z_{pred}[#mum]");
  g1->Draw("AP");
  g1->SetMarkerStyle(1);
  
  TCanvas *c2 = new TCanvas("c2");

  g2->GetXaxis()->SetLimits(-1.1 ,0.3 );

  g2->GetYaxis()->SetRangeUser(-1 ,1);
  g2->SetTitle("; z_{test}[#mum]; #Delta z[#mum]");
  g2->Draw("AP");
  g2->SetMarkerStyle(1);

  c1->Write();
  c2->Write();
  gStyle->SetOptFit(1111);
  fgaus->SetLineWidth(3);
  hdelta->Fit(fgaus,"R");

  hpred->SetLineColor(kBlack);
  hpred->SetLineWidth(2);
  hpred->Write();

  htest->SetLineColor(kBlack);
  htest->SetLineWidth(2);
  htest->Write();

  hdelta->SetLineColor(kBlack);
  hdelta->SetLineWidth(2);
  hdelta->Write();

  h.Close();

  
}


