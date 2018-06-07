#ifndef _FPGA_API_H_
#define _FPGA_API_H_

#include <sys/types.h>

// matrix vector multiplicator
// matrix M: SIZE by SIZE
// vector V: SIZE
// output = M * V

#define SIZE 64 // input,output vector size

class FPGA
{
private:
    int fd_;	
    float* data_;
    unsigned int* api_;
	
public:
    FPGA(off_t data_addr, off_t api_addr);
    ~FPGA();
	
	// return internal pointer for the data
	float* matrix(void);
	float* vector(void);
	
	// perform matrix multiplication and return output array pointer
    const float* run();	
	
	// input vector size: M
	// matrix size: N by M
	// output vector size: N
	// O = M * I
	void largeMV(const float* mat, const float* input, 
		float* output, int M, int N);    
};

#endif

