#include "salsa.h"

void Salsa::QuarterRound(const std::vector<uint8_t>& order,
		     	         std::vector<uint32_t>& state){
  state[order[1]] ^= LeftBitRotation((state[order[0]] + state[order[3]]), 7);
  state[order[2]] ^= LeftBitRotation((state[order[1]] + state[order[0]]), 9);
  state[order[3]] ^= LeftBitRotation((state[order[2]] + state[order[1]]), 13);
  state[order[0]] ^= LeftBitRotation((state[order[3]] + state[order[2]]), 18);
}

void Salsa::DoubleRound(std::vector<uint32_t>& state){
  std::vector<uint8_t> row_order {0, 1, 2, 3, 5, 6, 7, 4,
                                  10, 11, 8, 9, 15, 12, 13, 14};
  std::vector<uint8_t> column_order {0, 4, 8, 12, 5, 9 , 13, 1,
   				                     10, 14, 2, 6, 15, 3, 7, 11};
  CustomRound(column_order, state);
  CustomRound(row_order, state);
}

void Salsa::CustomRound(const std::vector<uint8_t>& order, 
		                std::vector<uint32_t>& state){
  auto it = order.begin();
  while(it != order.end()){
    QuarterRound({it, it+4}, state);
    it += 4;
  }
}

uint32_t Salsa::LeftBitRotation(const uint32_t value, 
                                const uint32_t bits) const {
  return ((value << bits) | (value >> (32 - bits)));
}

uint32_t Salsa::LittleEndian(const std::vector<uint8_t>& sequence) const{
  uint32_t word = 0;
  for(unsigned int index = 0; index < 4; index++){
    word |= sequence[index] << (index * 8);
  }
  return word;
}

std::vector<uint8_t> Salsa::ReverseLittleEndian(const uint32_t word) const{
  std::vector<uint8_t> sequence;
  sequence.push_back((word & 0x000000ff));
  sequence.push_back((word & 0x0000ff00) >> 8);
  sequence.push_back((word & 0x00ff0000) >> 16);
  sequence.push_back((word & 0xff000000) >> 24);
  return sequence;
}