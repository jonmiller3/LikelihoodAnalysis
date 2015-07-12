//
//  Helper.cpp
//  LikelihoodAnalysis
//
//  Created by Jonathan Miller on 7/10/15.
//  Copyright (c) 2015 Jonathan Miller. All rights reserved.
//

#include "Helper.h"

helper::helper(TH1D* hist){
    
    nbinsx=hist->GetXaxis()->GetNbins();
    maxx=hist->GetXaxis()->GetXmax();
    //nbinsy=hist->GetYaxis()->GetNbins();
    //maxy=hist->GetYaxis()->GetXmax();
    //nbinsz=hist->GetZaxis()->GetNbins();
    //maxz=hist->GetZaxis()->GetXmax();
    
    mem=new double[nbinsx];
    
    for (int bin=0; bin<nbinsx; ++bin){
        
        mem[bin]=hist->GetBinContent(bin);
        
    }
    
    return;
    
}

helper::helper(TH2D* hist){
    
    nbinsx=hist->GetXaxis()->GetNbins();
    maxx=hist->GetXaxis()->GetXmax();
    nbinsy=hist->GetYaxis()->GetNbins();
    maxy=hist->GetYaxis()->GetXmax();
    //nbinsz=hist->GetZaxis()->GetNbins();
    //maxz=hist->GetZaxis()->GetXmax();
    
    mem=new double[nbinsx*nbinsy];
    
    for (int bin=0; bin<nbinsx; ++bin){
        
        mem[bin]=hist->GetBinContent(bin);
        
    }
    
    return;
    
}




void TH3DSpec::GetRandom3Spec(Double_t &x, Double_t &y, Double_t &z, int seed)
{
    
    TRandom3 rndm3(seed);
    
    Int_t nbinsx = GetNbinsX();
    Int_t nbinsy = GetNbinsY();
    Int_t nbinsz = GetNbinsZ();
    Int_t nxy  = nbinsx*nbinsy;
    Int_t nbins = nxy*nbinsz;
    
    Double_t integral;
    if (fIntegral) {
        if (fIntegral[nbins+1] != fEntries) integral = ComputeIntegral();
    } else {
        integral = ComputeIntegral();
        if (integral == 0 || fIntegral == 0) return;
    }
    Double_t r1 = rndm3.Rndm();
    
    Int_t ibin = TMath::BinarySearch(nbins,fIntegral,(Double_t) r1);
    Int_t binz = ibin/nxy;
    
    Int_t biny = (ibin - nxy*binz)/nbinsx;
    Int_t binx = ibin - nbinsx*(biny + nbinsy*binz);
    
    
    x = fXaxis.GetBinLowEdge(binx+1);
    if (r1 > fIntegral[ibin]) x +=
        fXaxis.GetBinWidth(binx+1)*(r1-fIntegral[ibin])/(fIntegral[ibin+1] - fIntegral[ibin]);
    
    y = fYaxis.GetBinLowEdge(biny+1) + fYaxis.GetBinWidth(biny+1)*rndm3.Rndm();
    z = fZaxis.GetBinLowEdge(binz+1) + fZaxis.GetBinWidth(binz+1)*rndm3.Rndm();
    
    
}




void TH2DSpec::GetRandom2Spec(Double_t &x, Double_t &y, int seed)
{
    // return 2 random numbers along axis x and y distributed according
    // the cellcontents of a 2-dim histogram
    
    TRandom3 rndm3(seed);
    
    Int_t nbinsx = GetNbinsX();
    Int_t nbinsy = GetNbinsY();
    Int_t nbins  = nbinsx*nbinsy;
    Double_t integral;
    if (fIntegral) {
        if (fIntegral[nbins+1] != fEntries) integral = ComputeIntegral();
    } else {
        integral = ComputeIntegral();
        if (integral == 0 || fIntegral == 0) return;
    }
    Double_t r1 = rndm3.Rndm();
    Int_t ibin = TMath::BinarySearch(nbins,fIntegral,(Double_t) r1);
    Int_t biny = ibin/nbinsx;
    Int_t binx = ibin - nbinsx*biny;
    x = fXaxis.GetBinLowEdge(binx+1);
    if (r1 > fIntegral[ibin]) (x) +=
        fXaxis.GetBinWidth(binx+1)*(r1-fIntegral[ibin])/(fIntegral[ibin+1] - fIntegral[ibin]);
    y = fYaxis.GetBinLowEdge(biny+1) + fYaxis.GetBinWidth(biny+1)*rndm3.Rndm();
    
}




double TH1DSpec::GetRandomSpec(int seed){
    
    
    TRandom3 rndm3(seed);
    Int_t nbinsx = GetNbinsX();
    Double_t integral = 0;
    
    if (fIntegral) {
        if (fIntegral[nbinsx+1] != fEntries) integral = ((TH1*)this)->ComputeIntegral();
        else integral = fIntegral[nbinsx];
    } else {
        integral = ((TH1*)this)->ComputeIntegral();
    }
    if (integral == 0) return 0;
    
    Double_t r1 = rndm3.Rndm();
    Int_t ibin = TMath::BinarySearch(nbinsx,fIntegral,r1);
    Double_t x = GetBinLowEdge(ibin+1);
    if (r1>fIntegral[ibin]) x +=
        GetBinWidth(ibin+1)*(r1-fIntegral[ibin])/(fIntegral[ibin+1]-fIntegral[ibin]);
    return x;
    
}

/*
void GetArray	(	const TH1 * 	h,
                 float *& 	fArray,
                 double *& 	dArray
                 )
{
    fArray = 0; dArray = 0;
    const TString className = h->ClassName();
    if(className == "TH1F") fArray = ((TH1F*)h)->fArray;
    if(className == "TH2F") fArray = ((TH2F*)h)->fArray;
    if(className == "TH3F") fArray = ((TH3F*)h)->fArray;
    if(className == "TH1D") dArray = ((TH1D*)h)->fArray;
    if(className == "TH2D") dArray = ((TH2D*)h)->fArray;
    if(className == "TH3D") dArray = ((TH3D*)h)->fArray;
}


int GetArraySize	(	const TH1 * 	h	 )
{
    
    struct CellsGetter: public TH1
    {
            int NCells()
            {
                return fNcells;
                }
    };
    
    return ((CellsGetter*)h)->NCells();
    
}
 */

Bool_t TH1DSpec::AddFast(const TH1DSpec *h1, const TH1DSpec *h2, Double_t c1, Double_t c2){

    if(c1 == 0 || c2==0) return false;
    //assert(this->ClassName() == b->ClassName());
    
    const int N = fNcells; // should be able to do this due to inherentance
    //const int N = GetArraySize(h1);
    //assert(GetArraySize(b) == N);

    //float* fArrayA = 0;
    //float* fArrayB = 0;
    //float* fArrayC = 0;

    const Double_t* dArrayA = h1->GetArray();
    const Double_t* dArrayB = h2->GetArray();
    //double* dArrayC = 0;

    
    //GetArray(h1, fArrayA, dArrayA);
    //GetArray(h2, fArrayB, dArrayB);
    //GetArray(this, fArrayC, dArrayC);

    
    if(!(dArrayA && dArrayB)){

                this->Add(h1, h2, c1, c2);
                return false;
        }
    
    if(dArrayA && dArrayB) for(int n = 0; n < N; ++n) fArray[n]=dArrayA[n]*c1 + dArrayB[n]*c2;
    
    return true;

}



Bool_t TH2DSpec::AddFast(const TH2DSpec *h1, const TH2DSpec *h2, Double_t c1, Double_t c2){
    
    if(c1 == 0 || c2==0) return false;
    //assert(this->ClassName() == b->ClassName());
    
    const int N = fNcells; // should be able to do this due to inherentance
    //const int N = GetArraySize(h1);
    //assert(GetArraySize(b) == N);
    
    //float* fArrayA = 0;
    //float* fArrayB = 0;
    //float* fArrayC = 0;
    
    const Double_t* dArrayA = h1->GetArray();
    const Double_t* dArrayB = h2->GetArray();
    //double* dArrayC = 0;
    
    
    //GetArray(h1, fArrayA, dArrayA);
    //GetArray(h2, fArrayB, dArrayB);
    //GetArray(this, fArrayC, dArrayC);
    
    
    if(!(dArrayA && dArrayB)){
        
        this->Add(h1, h2, c1, c2);
        return false;
    }
    
    if(dArrayA && dArrayB) for(int n = 0; n < N; ++n) fArray[n]=dArrayA[n]*c1 + dArrayB[n]*c2;
    
    return true;
    
}



Bool_t TH3DSpec::AddFast(const TH3DSpec *h1, const TH3DSpec *h2, Double_t c1, Double_t c2){
    
    if(c1 == 0 || c2==0) return false;
    //assert(this->ClassName() == b->ClassName());
    
    const int N = fNcells; // should be able to do this due to inherentance
    //const int N = GetArraySize(h1);
    //assert(GetArraySize(b) == N);
    
    //float* fArrayA = 0;
    //float* fArrayB = 0;
    //float* fArrayC = 0;
    
    const Double_t* dArrayA = h1->GetArray();
    const Double_t* dArrayB = h2->GetArray();
    //double* dArrayC = 0;
    
    
    //GetArray(h1, fArrayA, dArrayA);
    //GetArray(h2, fArrayB, dArrayB);
    //GetArray(this, fArrayC, dArrayC);
    
    
    if(!(dArrayA && dArrayB)){
        
        this->Add(h1, h2, c1, c2);
        return false;
    }
    
    if(dArrayA && dArrayB) for(int n = 0; n < N; ++n) fArray[n]=dArrayA[n]*c1 + dArrayB[n]*c2;
    
    return true;
    
}

