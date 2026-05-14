// =============================================================================
// probtable.bin file format
// =============================================================================
// All integers little-endian. Floats are IEEE 754 binary32.
//
// Header (32 bytes):
//   offset  size  field
//   ------  ----  -------------------------------------------------------------
//      0     4    magic   = "FRKL"
//      4     4    version = 1                (uint32)
//      8     4    goal    = 10000            (uint32)
//     12     4    error   = 8000             (uint32)
//     16     4    dim_i   = (goal+error)/50  (uint32)  -- player score buckets
//     20     4    dim_j   = (goal+error)/50  (uint32)  -- opponent score buckets
//     24     4    dim_k   = error/50         (uint32)  -- turn total buckets
//     28     4    dim_d   = 6                (uint32)  -- dice remaining (1..6)
//
// Data (dim_i * dim_j * dim_k * dim_d float32s, ~475 MB):
//   Row-major, i outermost, d innermost. Offset of P(i,j,k,d):
//     flat_index   = (((i * dim_j + j) * dim_k + k) * dim_d + d)
//     byte_offset  = 32 + flat_index * 4
//   Real score = bucket_index * 50.  d is zero-indexed (d=0 means 1 die).
// =============================================================================
 
#include <iostream>
#include <fstream>
#include <cstdint>
#include "ProbabilityTable.h"
 
// Declare here so you dont put ~475 MB on the stack
ProbabilityTable P;
 
int main() {
  std::ofstream out("probtable.bin", std::ios::binary);
  if(!out) {
    std::cerr << "Failed to open probtable.bin for writing\n";
    return 1;
  }
 
  // This is just to print the header
  const char     magic[4] = {'F','R','K','L'};
  const uint32_t version  = 1;
  const uint32_t goalVal  = goal;
  const uint32_t errorVal = error;
  const uint32_t dim_i    = (goal + error) / 50;
  const uint32_t dim_j    = (goal + error) / 50;
  const uint32_t dim_k    = error / 50;
  const uint32_t dim_d    = diceNum;
 
  out.write(magic, 4);
  out.write(reinterpret_cast<const char*>(&version),  sizeof(version));
  out.write(reinterpret_cast<const char*>(&goalVal),  sizeof(goalVal));
  out.write(reinterpret_cast<const char*>(&errorVal), sizeof(errorVal));
  out.write(reinterpret_cast<const char*>(&dim_i),    sizeof(dim_i));
  out.write(reinterpret_cast<const char*>(&dim_j),    sizeof(dim_j));
  out.write(reinterpret_cast<const char*>(&dim_k),    sizeof(dim_k));
  out.write(reinterpret_cast<const char*>(&dim_d),    sizeof(dim_d));
 
  // These are just for the output at the end
  const uint64_t total = (uint64_t)dim_i * dim_j * dim_k * dim_d;
  uint64_t written = 0;
 
  // Stores it in a specific thing where d changes the fastest and i changes the slowest its cool
  for(uint32_t i = 0; i < dim_i; i++) {
    for(uint32_t j = 0; j < dim_j; j++)
      for(uint32_t k = 0; k < dim_k; k++)
        for(uint32_t d = 0; d < dim_d; d++) {
          float p = P.getProbability(i, j, k, d);
          out.write(reinterpret_cast<const char*>(&p), sizeof(float));
          ++written;
        }
 
    // After this just user progress messages
    if(i % 10 == 0)
      std::cout << "i=" << i << "/" << dim_i
                << "  (" << (100.0 * written / total) << "%)\n";
  }
 
  out.close();
  std::cout << "Done. Wrote " << written
            << " floats = " << (written * sizeof(float) / (1024 * 1024))
            << " MB to probtable.bin\n";
  return 0;
}
