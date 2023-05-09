#include <iostream>

#include "salsa.h"

int main(){
  std::vector<uint8_t> state = {211,159, 13,115, 76, 55, 82,183, 3,117,222, 37,191,187,234,136,
		 49,237,179, 48, 1,106,178,219,175,199,166, 48, 86, 16,179,207,
		 31,240, 32, 63, 15, 83, 93,161,116,147, 48,113,238, 55,204, 36,
		 79,201,235, 79, 3, 81,156, 47,203, 26,244,243, 88,118,104, 54};
  Salsa salsa(state, 20);
  std::vector<uint8_t> output_salsa = salsa.Hash<uint8_t>();
  std::cout.flags(std::ios::dec);
  for(auto it = output_salsa.begin(); it != output_salsa.end(); ++it){
    std::cout << int(*it) << " ";
  }
  std::cout << std::endl;
  return 0;
}