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
#include <TH1F.h>
#include <TH1.h>

#include <TH2D.h>
#include <TH2F.h>
#include <TH2.h>

#include <TH3D.h>
#include <TH3F.h>
#include <TH3.h>

#include <stdio.h>
#include <iostream>

#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#else
#include <stdio.h>
#include <string.h>
#include "CL/cl.h"
#define FALSE false
#define TRUE true
#endif

using namespace std;


// I think you do it like this:
#define MAX_GPU_COUNT 16

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


class TH1FSpec: public TH1F{
    
public:
    TH1FSpec();
    TH1FSpec(const char *name,const char *title,Int_t nbinsx,Double_t xlow,Double_t xup): TH1F(name,title,nbinsx,xlow,xup){}
    TH1FSpec(const char *name,const char *title,Int_t nbinsx,const Float_t  *xbins): TH1F(name,title,nbinsx,xbins){}
    TH1FSpec(const char *name,const char *title,Int_t nbinsx,const Double_t *xbins) : TH1F(name,title,nbinsx,xbins){}
    //TH1FSpec(const TVectorF &v) : TH1F ( *v ){}
    //TH1FSpec(const TH1FSpec &h1f) : TH1F( &hlf ){}
    virtual ~TH1FSpec(){}
    
    double GetRandomSpec(int);
    Bool_t AddFast(const TH1FSpec *, const TH1FSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const float* GetArray() const { return fArray;}
    
    
    
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



class TH2FSpec: public TH2F{
    
public:
    TH2FSpec();
    TH2FSpec(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins): TH2F(name,title,nbinsx,xbins,nbinsy,ybins){}
    TH2FSpec(const char *name,const char *title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins): TH2F(name,title,nbinsx,xbins,nbinsy,ybins){}
    TH2FSpec(const char *name,const char *title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, Double_t ylow, Double_t yup) : TH2F(name,title,nbinsx,xbins,nbinsy,ylow,yup){}
    TH2FSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, const Double_t* ybins) : TH2F(name,title,nbinsx,xlow,xup,nbinsy,ybins){}
    TH2FSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup) : TH2F(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup){}
    //TH1FSpec(const TVectorF &v) : TH1F ( *v ){}
    //TH1FSpec(const TH1FSpec &h1f) : TH1F( &hlf ){}
    virtual ~TH2FSpec(){}
    
    void GetRandom2Spec(Double_t &, Double_t &, int);
    
    Bool_t AddFast(const TH2FSpec *, const TH2FSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const float* GetArray() const { return fArray;}
    
    
};



class TH3DSpec: public TH3D{
    
public:
    TH3DSpec();
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins): TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins): TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    TH3DSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup): TH3D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup){}
    
    virtual ~TH3DSpec(){}
    
    void GetRandom3Spec(Double_t &, Double_t &, Double_t&, int);
    Int_t GetRandom3Spec(int);

    
    Bool_t AddFast(const TH3DSpec *, const TH3DSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const Double_t* GetArray() const { return fArray;}


    
};



class TH3FSpec: public TH3F{
    
public:
    TH3FSpec();
    TH3FSpec(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins): TH3F(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    
    TH3FSpec(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins): TH3F(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins){}
    TH3FSpec(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup): TH3F( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup){}
    
    virtual ~TH3FSpec(){}
    
    void GetRandom3Spec(Double_t &, Double_t &, Double_t&, int);
    Int_t GetRandom3Spec(int);
    
    
    Bool_t AddFast(const TH3FSpec *, const TH3FSpec *, Double_t, Double_t);
    Int_t GetNCells(){ return fNcells;}
    const float* GetArray() const { return fArray;}
    
    
    
};



// this is a tool from NVIDIA
//////////////////////////////////////////////////////////////////////////////
//! Loads a Program file and prepends the cPreamble to the code.
//!
//! @return the source string if succeeded, 0 otherwise
//! @param cFilename        program filename
//! @param cPreamble        code that is prepended to the loaded file, typically a set of #defines or a header
//! @param szFinalLength    returned length of the code string
//////////////////////////////////////////////////////////////////////////////
char* oclLoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
    // locals
    FILE* pFileStream = NULL;
    size_t szSourceLength;
    
    
    // open the OpenCL source code file
#ifdef _WIN32   // Windows version
    if(fopen_s(&pFileStream, cFilename, "rb") != 0)
    {
        return NULL;
    }
#else           // Linux version
    
    pFileStream = fopen(cFilename, "rb");
    if(pFileStream == 0)
    {
      std::cout<<" falied to open file load program "<<cFilename<<std::endl;
        return NULL;
    }
#endif
    
    size_t szPreambleLength = strlen(cPreamble);
    
    // get the length of the source code
    fseek(pFileStream, 0, SEEK_END);
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET);
    
    // allocate a buffer for the source code string and read it in
    char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1);
    memcpy(cSourceString, cPreamble, szPreambleLength);
    if (fread((cSourceString) + szPreambleLength, szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }
    
    // close the file and return the total length of the combined (preamble + source) string
    fclose(pFileStream);
    if(szFinalLength != 0)
    {
        *szFinalLength = szSourceLength + szPreambleLength;
    }
    cSourceString[szSourceLength + szPreambleLength] = '\0';

    return cSourceString;
}


// should should be after I initialize, but before I use the GPU devices
// I need to do this once per device?
int CompileOCLKernel(const cl_device_id* cdDevice, cl_context       cxGPUContext,
                     const char *ocl_source_filename, cl_program *cpProgram){
    
    
    
    // this is adapted from Nvidia example code
    
    cl_int          ciErrNum;
    
    
    size_t program_length=0;
    program_length=strlen(ocl_source_filename);
    // argv[0] should be some setting... I can just set it
    
    char *source = oclLoadProgSource(ocl_source_filename,"",&program_length);
    
    *cpProgram = clCreateProgramWithSource(cxGPUContext, 1, (const char **)&source,
                                           &program_length, &ciErrNum);
    
    if (ciErrNum != CL_SUCCESS) {
        std::cout<<"Error: Failed to create program\n"<<std::endl;
        return ciErrNum;
    } else {
        std::cout<<"clCreateProgramWithSource "<<source<<" succeeded, program_length="<<program_length<<std::endl;
    }
    free(source);
    
    // OK, we created program, now it needs to be built
    cl_build_status build_status;
    
    // I think I need to include the devices built here...
    //ciErrNum = clBuildProgram(*cpProgram, 0, NULL, "-cl-fast-relaxed-math -cl-nv-verbose", NULL, NULL);

    ciErrNum = clBuildProgram(*cpProgram, 2, cdDevice, "-cl-fast-relaxed-math -cl-nv-verbose", NULL, NULL);

    //ciErrNum = clBuildProgram(*cpProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
    if (ciErrNum != CL_SUCCESS)
    {
        // write out standard error, Build Log and PTX, then return error
        std::cout<<" problem with building program"<<ciErrNum<<std::endl;
	/*
	size_t len;
	char buffer[20480];
	ciErrNum=clGetProgramBuildInfo(*cpProgram, cdDevice[devi], CL_PROGRAM_BUILD_LOG, NULL, NULL, &len);
	ciErrNum=clGetProgramBuildInfo(*cpProgram, cdDevice[devi], CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
	//std::cout<<" buffer result "<<buffer<<std::endl
	std::cout<<" getting log result "<<ciErrNum<<std::endl;
	printf("buffer is %s\n", buffer);
	*/
	clGetProgramBuildInfo(*cpProgram, cdDevice[0], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);
	std::cout<<" build status "<<build_status<<std::endl;
	//printf("%s\n", build_status);
	//clGetProgramBuildInfo(*cpProgram, cdDevice, CL_PROGRAM_BUILD_OPTIONS, 2048*sizeof(char), buffer, &len);
	//printf("%s\n", buffer);

    } else {
        ciErrNum = clGetProgramBuildInfo(*cpProgram, cdDevice[0], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);
        //shrLog("clGetProgramBuildInfo returned: ");
        if (build_status == CL_SUCCESS) {
            std::cout<<"CL_SUCCESS "<<ciErrNum<<" "<<build_status<<std::endl;
        } else {
            std::cout<<"CLErrorNumber = "<<ciErrNum<<std::endl;
        }
	

	/*

size_t len;
	char buffer[20480];
	ciErrNum=clGetProgramBuildInfo(*cpProgram, cdDevice[0], CL_PROGRAM_BUILD_LOG, NULL, NULL, &len);
	ciErrNum=clGetProgramBuildInfo(*cpProgram, cdDevice[0], CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
	//std::cout<<" buffer result "<<buffer<<std::endl
	std::cout<<" getting log result "<<ciErrNum<<std::endl;
	printf("buffer is %s\n", buffer);
	*/
	
    }

    size_t logSize;
    char *programLog;
    clGetProgramBuildInfo(*cpProgram, cdDevice[0],
			  CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    programLog = (char*) calloc (logSize+1, sizeof(char));
    clGetProgramBuildInfo(*cpProgram, cdDevice[0],
			  CL_PROGRAM_BUILD_LOG, logSize+1, programLog, NULL);
    printf("Build failed; error=%d, status=%d, programLog:nn%s",ciErrNum, build_status, programLog);
    free(programLog);

    
    // print out the build log, note in the case where there is nothing shown, some OpenCL PTX->SASS caching has happened
    /*
     {
     char *build_log;
     size_t ret_val_size;
     ciErrNum = clGetProgramBuildInfo(*cpProgram, cdDevices, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
     if (ciErrNum != CL_SUCCESS) {
     shrLog("clGetProgramBuildInfo device %d, failed to get the log size at line %d\n", cdDevices, __LINE__);
     }
     build_log = (char *)malloc(ret_val_size+1);
     ciErrNum = clGetProgramBuildInfo(*cpProgram, cdDevices, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
     if (ciErrNum != CL_SUCCESS) {
     shrLog("clGetProgramBuildInfo device %d, failed to get the build log at line %d\n", cdDevices, __LINE__);
     }
     // to be carefully, terminate with \0
     // there's no information in the reference whether the string is 0 terminated or not
     build_log[ret_val_size] = '\0';
     shrLog("%s\n", build_log );
     }
     */
    return 0;
    
}



#endif /* defined(__LikelihoodAnalysis__Helper__) */
