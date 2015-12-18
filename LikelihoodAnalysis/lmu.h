#ifndef LMU_H
#define LMU_H

#include <TH1D.h>
#include <TH1F.h>
#include <TH1.h>

#include <TH2D.h>
#include <TH2F.h>
#include <TH2.h>

#include <TH3D.h>
#include <TH3F.h>
#include <TH3.h>

#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>

#include <iostream>
//#include <thread>


#include "Helper.h"

using namespace std;

struct model {
    double signal;
    
};


struct modelpdf {
    TH1FSpec* signal;
    
    
};

bool operator<(model a, model m){

  double total=m.signal;
  double ta=a.signal;

  return ta<total;


}




class lmu{

private:
  modelpdf fs;
  TH1FSpec* fb;
  TH1FSpec* ft;

    
    int nbinsx;
    int nbinsy;
    int nbinsz;
    double maxx;
    double maxy;
    double maxz;

public:
  lmu(modelpdf hs, TH1FSpec* hb){fs=hs;fb=hb;
    nbinsx=fb->GetXaxis()->GetNbins();
    maxx=fb->GetXaxis()->GetXmax();}

    ~lmu(){delete fb;}

    int* calc(double*, model, int, float&);
    int calc(int*, model, int, float&);
  int* getrandom(model,int,double*);
    
    int getncells(){return fb->GetNCells();}
    
    // but I think that double* is better?
    float* getmodelarray();
    TH1FSpec* getfb(){return fb;}
    TH1FSpec* getfs1(){return fs.signal;}
    
    double getintegral(model);
    
    
};

// but I think I want double*
float* lmu::getmodelarray(){
    
    // problem is that there are underflow and overflow bins here
    int ncells = fb->GetNCells();
    
    
    // I think that doing this is unncessary
    const float* barray = fb->GetArray();
    const float* s1array = fs.signal->GetArray();
    
    // res should be 13*nbins
    float* res = new float[ncells*2];
    
    copy(barray, barray+ncells, res);
    copy(s1array, s1array+ ncells, res + ncells);
 
    for (int i=0; i<ncells*13; i++) {
        if (isnan(res[i])) std::cout<<" test "<<res[i]<<std::endl;
    }
    
    return res;
    
}


int* lmu::calc(double* phi, model mu, int nobs, float& totalcontent){
  
    int* binres = new int[nobs];
    
  ft=0;

  ft = new TH1FSpec("ft","ft",nbinsx,0,maxx);

    ft->Add(fs.signal,fb,mu.signal,1);

  totalcontent=0;
  for (int i=0; i<nobs; i++){

      // I need to check order on this
    int binx=1+(int)(phi[i]*nbinsx/maxx);
    int bin = ft->GetBin(binx);

      binres[i]=bin;
      
    double content = ft->GetBinContent(bin);

    if (content!=0) {
        
        content = TMath::Log(content);
        
        
    } else {
        cout<<" test content "<<content<<" at bin "<<bin<<endl;
      content = -1000;
    }

    totalcontent+=content;
  }

  delete ft;
  //return totalcontent;

    return binres;
    
}


int lmu::calc(int* binres, model mu, int nobs, float& totalcontent){
    
    ft=0;
    
    ft = new TH1FSpec("ft","ft",nbinsx,0,maxx);
    
    ft->Add(fs.signal,fb,mu.signal,1);
    
    totalcontent=0;
    for (int i=0; i<nobs; i++){
        

        
        double content = ft->GetBinContent(binres[i]);
        
        if (content!=0) {
            
            content = TMath::Log(content);
            
            
        } else {
            cout<<" test content "<<content<<" at bin "<<binres[i]<<endl;
            content = -1000;
        }
        
        totalcontent+=content;
    }
    
    delete ft;
    //return totalcontent;
    
    return 1;
    
}

int* lmu::getrandom(model mu, int nobs, double* phi){
    int* binres = new int[nobs];


  ft=0;
  ft = new TH1FSpec("ft","ft",nbinsx,0,maxx);
    
    ft->Add(fs.signal,fb,mu.signal,1);

  for (int i=0; i<nobs; i++){
    double out1=0;
      double out2=0;
      double out3=0;

    out1 = ft->GetRandomSpec(0);

    phi[i]=(out1);


      // I need to check order on this
      int binx=1+(int)(phi[i]*nbinsx/maxx);
      int bin = ft->GetBin(binx);
      
      binres[i]=bin;


  }

  delete ft;
return binres;

}

double lmu::getintegral(model mu){
    
    double res=0;
    
    
    
    ft=0;
    ft = new TH1FSpec("ft","ft",nbinsx,0,maxx);
    
    
    ft->Add(fs.signal,fb,mu.signal,1);
    
    res=ft->Integral();
    
    delete ft;
    
    return res;
}


#endif
