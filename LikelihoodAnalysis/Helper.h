//
//  Helper.h
//  LikelihoodAnalysis
//
//  Created by Jonathan Miller on 7/10/15.
//  Copyright (c) 2015 Jonathan Miller. All rights reserved.
//

#ifndef __LikelihoodAnalysis__Helper__
#define __LikelihoodAnalysis__Helper__

#include <TH1D.h>
#include <TH1.h>

#include <TH2D.h>
#include <TH2.h>

#include <TH3D.h>
#include <TH3.h>

#include <stdio.h>

#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>


// goal of this class is to take 1D-3D root histograms
// and change them into 3D memory structures
// when you add 2 of them, they need to produce a 3rd
// and the elements need to be added
class helper{

private:
    int nbinsx;
    int nbinsy;
    int nbinsz;
    double maxx;
    double maxy;
    double maxz;
    
    double* mem;
    int dim;
    
public:
    helper(TH1D*);
    helper(TH3D*);
    helper(TH2D*);
   
    // I am not sure if this is the best
    // adds this mini histogram
    helper Add(helper,double);
    
    // this could also be done on GPU, but it is really
    // simple, just look up (3D)
    double* GetContent(double*);
    

};



class TH1DSpec: public TH1D{
    
public:
    TH1DSpec();
    TH1DSpec(const char *name,const char *title,Int_t nbinsx,Double_t xlow,Double_t xup): TH1D(name,title,nbinsx,xlow,xup){}
    TH1DSpec(const char *name,const char *title,Int_t nbinsx,const Float_t  *xbins): TH1D(name,title,nbinsx,xbins){}
    TH1DSpec(const char *name,const char *title,Int_t nbinsx,const Double_t *xbins) : TH1D(name,title,nbinsx,xbins){}
    //TH1FSpec(const TVectorF &v) : TH1F ( *v ){}
    //TH1FSpec(const TH1FSpec &h1f) : TH1F( &hlf ){}
    virtual ~TH1DSpec(){}
    
    double GetRandomSpec(int);
    Bool_t AddFast(const TH1DSpec *, const TH1DSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const Double_t* GetArray() const { return fArray;}
    
    
    
};


class TH2DSpec: public TH2D{
    
public:
    TH2DSpec();
    TH2DSpec(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins): TH2D(name,title,nbinsx,xbins,nbinsy,ybins){}
    TH2DSpec(const char *name,const char *title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins): TH2D(name,title,nbinsx,xbins,nbinsy,ybins){}
    TH2DSpec(const char *name,const char *title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, Double_t ylow, Double_t yup) : TH2D(name,title,nbinsx,xbins,nbinsy,ylow,yup){}
    TH2DSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, const Double_t* ybins) : TH2D(name,title,nbinsx,xlow,xup,nbinsy,ybins){}
    TH2DSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup) : TH2D(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup){}
    //TH1FSpec(const TVectorF &v) : TH1F ( *v ){}
    //TH1FSpec(const TH1FSpec &h1f) : TH1F( &hlf ){}
    virtual ~TH2DSpec(){}
    
    void GetRandom2Spec(Double_t &, Double_t &, int);
    
    Bool_t AddFast(const TH2DSpec *, const TH2DSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const Double_t* GetArray() const { return fArray;}

    
};

class TH3DSpec: public TH3D{
    
public:
    TH3DSpec();
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins): TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins): TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup): TH3D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup){}
    
    virtual ~TH3DSpec(){}
    
    void GetRandom3Spec(Double_t &, Double_t &, Double_t&, int);
    
    Bool_t AddFast(const TH3DSpec *, const TH3DSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const Double_t* GetArray() const { return fArray;}


    
};


#endif /* defined(__LikelihoodAnalysis__Helper__) */
