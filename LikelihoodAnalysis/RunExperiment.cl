#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#define DOUBLEFAIL
#endif

//#include <OpenCL/OpenCL.h>

// a bit silly, I probably should just stick in the value below into the proper place
//inline float modelvalue( const float* pdf , int modelnumber, int numbins, float theoryval, int databin){
//    return (float)( pdf[modelnumber*numbins+databin]*theoryval);
//}

// I should put in the basic model in
//

// float or double?
#ifdef DOUBLEFAIL
kernel void RunExperiment(global const float* pdf, constant int* data, constant int* inputs, global float* maxl)
#else
kernel void RunExperiment(global const double* pdf, constant int* data, constant int* inputs, global double* maxl)
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
    int i1024=((iGD-i512*64-i64*8-i8)/512)%8;

    //printf(" first %i %i %i %i %i \n",i8,i64,i512,i1024);
    
    
    // max should be -1000 or 0?
    maxl[iGD]=0;
    float tval[13];
    float t0val[13];

    t0val[0]=1;
    t0val[1]=1;
    t0val[2]=1;
    t0val[3]=1;
    t0val[4]=1;
    t0val[5]=1;
    t0val[6]=1;
    t0val[7]=1;
    t0val[8]=1;
    t0val[9]=1;
    t0val[10]=1;
    t0val[11]=1;
    t0val[12]=1;
    
    // background is strength 1 always
    tval[0]=1;
    
    float np=4.;
    
    tval[1]=t0val[1];
    tval[2]=t0val[2];
    tval[3]=t0val[3];
    tval[4]=t0val[4];
    tval[5]=t0val[5];
    
    tval[6]=t0val[6];
    tval[7]=t0val[7];
    tval[8]=t0val[8];
    
    for (int iKcore=-np; iKcore<np; iKcore++){
        tval[9]=(np+iKcore)*1e4;
        for (int iKmantle=-np; iKmantle<np; iKmantle++){
            tval[10]=t0val[10]*(1+iKmantle/np);
            for (int iKcrust=-np; iKcrust<np; iKcrust++){
                tval[11]=t0val[11]*(1+iKcrust/np);
                for (int iKocean=-np; iKocean<np; iKocean++){
                    
                    tval[12]=t0val[12]*(1+iKocean/np);
   
                                                 
#ifdef DOUBLEFAIL
   float l=0;
#else
   double l=0;
#endif
                    
                    for (int i=0; i<numdata; i++){
    
                        int databin=data[i];
                        for (int modelnumber=0; modelnumber<13; modelnumber++){
                            if (pdf[modelnumber*numbins+databin]>0) printf(" test data %f %i",pdf[modelnumber*numbins+databin],modelnumber*numbins+databin);
                            l+=(pdf[modelnumber*numbins+databin]*tval[modelnumber]);
                        }
                        
                    }
                                                    
                    if (l>maxl[iGD]) maxl[iGD]=l;
                                                    
                                                    
                }
            }
        }
    }
    
    
    
    
    printf(" out %i %f \n",iGD,maxl[iGD]);
    
    
}