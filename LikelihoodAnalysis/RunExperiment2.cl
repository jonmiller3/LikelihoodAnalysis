#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#define DOUBLEFAIL
#endif

//#include <OpenCL/OpenCL.h>

// float or double?
#ifdef DOUBLEFAIL
kernel void RunExperiment(global const float* pdf, constant int* data, constant int* inputs, global const float* t0val, global float* llh)
#else
kernel void RunExperiment(global const double* pdf, constant int* data, constant int* inputs, global const float* t0val, global double* llh)
#endif
{
   

    
    
    int numdata=inputs[0];
    int numbins=inputs[1];
    
    // data is bin number
    int iGD = get_global_id(0);
    
    // this is for 1024 workgroup size
    int i8=iGD%8;
    int i64=((iGD-i8)/8)%8;
    int i512=((iGD-i64*8-i8)/64)%8;
    ///int i1024=((iGD-i512*64-i64*8-i8)/512)%8;
    
    // this is for 1024 workgroup size
    //int i16=iGD%16;
    //int i256=((iGD-i16)/16)%16;
    
    
    float tval[13];
    
    // background is strength 1 always
    tval[0]=1;
    
    float np=4.;
    
    
    
#ifdef DOUBLEFAIL
    float* l= new float[np*2];
#else
    double* l= new float[np*2];
#endif
    
    
    // initial settings
    tval[1]=t0val[0];
    tval[2]=t0val[1];
    tval[3]=t0val[2];
    tval[4]=t0val[3];
    tval[5]=t0val[4];
    
    tval[6]=t0val[5];
    tval[7]=t0val[6];
    tval[8]=t0val[7];
    
    tval[9]=t0val[8];
    tval[10]=t0val[9];
    tval[11]=t0val[10];

    tval[12]=t0val[11];

    
    
    //for (int iUcore=-np; iUcore<np; iUcore++){
    int iUcore=i8-np;
    tval[1]=(np+iUcore);
    //for (int iUmantle=-np; iUmantle<np; iUmantle++){
    int iUmantle=i64-np;
    tval[2]=t0val[1]*(1+iUmantle/np);

    int iUcrust=i512-np;
    tval[3]=t0val[2]*(1+iUcrust/np);
    
    for (int iUocean=-np; iUocean<np; iUocean++){
        l[iUocean+np]=0;
        tval[4]=t0val[4]*(1+iUocean/np);
    
    for (int i=0; i<numdata; i++){
        
        int databin=data[i];
        for (int modelnumber=0; modelnumber<13; modelnumber++){
            l[iUocean+np]+=(pdf[modelnumber*numbins+databin]*tval[modelnumber]);
        }
        
    }

        llh[iGD+iUocean+np]=l[iUocean+np];
    
    }
    
    
    
    // the output should be 512 or 1024x8 or something floats/doubles

    
}



