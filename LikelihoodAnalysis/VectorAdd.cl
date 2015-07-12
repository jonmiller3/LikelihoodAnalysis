//#include <OpenCL/OpenCL.h>

// this is similar to NVIDIA version

kernel void VectorAdd(global const float*a, global const float*b, global const  float* c, int num)
{
    
    
    int iGD = get_global_id(0);
    
    if (iGD >= num) return;
    
    c[iGD] = a[iGD] + b[iGD];
    
}

