#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fpga_api.h"


// g++ -I./include test.cpp ./src/fpga_api.cpp -o run.exe && sudo ./run.exe

// g++ -I./include test.cpp ./src/fpga_api_pseudo.cpp -o run.exe && sudo ./run.exe

typedef union {
  float f;
  unsigned int i;
}foo;

int main(int argc, char** argv)
{
  foo container;

  float* flat = new float[SIZE * (SIZE + 1)];
  float* input = flat;
  float* mat = flat + SIZE;  
  float* output = new float[SIZE];

  for(int i = 0 ; i < SIZE*(SIZE+1) ; ++i)
  {
    flat[i] = ((float)rand()) / RAND_MAX;
  }

  for (int i = 0; i < SIZE; i++)
    output[i] = 0.0f;

  // computation
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      output[i] += input[j] * mat[SIZE*i + j];

  // FPGA offloading
  // memory load
  FPGA dev(0x40000000, 0x43c00000);
  memcpy(dev.vector(), input, sizeof(float)*SIZE);
  memcpy(dev.matrix(), mat, sizeof(float)*SIZE*SIZE);
  const float* output_fpga = dev.run();

  // display
  printf("%-10s%-10s%-10s%-10s\n", "index", "CPU", "FPGA", "FPGA(hex)");
  for (int i = 0; i < SIZE; i++)
  {
    container.f = output_fpga[i];
    printf("%-10d%-10f%-10f%-10X\n", i, output[i], output_fpga[i], container.i);
  }

  delete[] flat;
  delete[] output;
  return 0;
}
