#ifndef STREAM_CIPHERS_INCLUDE_SALSA_H_
#define STREAM_CIPHERS_INCLUDE_SALSA_H_

#include <iostream>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

class Salsa{
  public:
    template<typename input_type = uint8_t>
    Salsa(const std::vector<input_type>& state, const uint32_t rounds);
    template<typename output_type = uint8_t>
    std::vector<output_type> Hash();
  private:
    template<typename input_type>
    void StateInitialization(const std::vector<input_type>& state);
    template<typename output_type>
    std::vector<output_type> OutputState(const std::vector<uint32_t>& state) const;
    void QuarterRound(const std::vector<uint8_t>& order,
		     	            std::vector<uint32_t>& state);
    void DoubleRound(std::vector<uint32_t>& state);
    void CustomRound(const std::vector<uint8_t>& order, 
		                 std::vector<uint32_t>& state);
    uint32_t LeftBitRotation(const uint32_t value, const uint32_t bits) const;
    uint32_t LittleEndian(const std::vector<uint8_t>& sequence) const;
    std::vector<uint8_t> ReverseLittleEndian(const uint32_t word) const;
    std::vector<uint32_t> state_;
    uint32_t rounds_;
};
template<typename input_type>
Salsa::Salsa(const std::vector<input_type>& state, const uint32_t rounds): rounds_(rounds){
  try{
    // check the `input_type`
    assert(typeid(input_type) == typeid(uint8_t) || typeid(input_type) == typeid(uint32_t)
           && "warning: Invalid input data type for Salsa object.");
    // initialize 'state_'
    StateInitialization<input_type>(state);
  }
  catch( const std::out_of_range& e ){
    std::cerr << e.what() << std::endl;
  }
}
template<typename input_type>
void Salsa::StateInitialization(const std::vector<input_type>& state){
  // make a copy if the `input_type` is uint32_t
  if( typeid(input_type) == typeid(uint32_t) ){
    state_ = std::vector<uint32_t>(state.begin(), state.end());
    return;
  }
  // otherwise can be uint8_t
  // check the number of elements in vector
  std::size_t size = state.size();
  if( size != 64 ){
    throw std::out_of_range("error: The input state vector have " + std::to_string(size) + 
                            "/64 byte sequence.");
  }
  // convert state as sequence of words of 4-bytes
  auto it = state.begin();
  std::vector<uint32_t> words;
  while( it != state.end() ){
    std::vector<uint8_t> sequence(it, it+4);
    words.push_back(LittleEndian(sequence));
    it += 4;
  }
  state_ = words;
}
template<typename output_type>
std::vector<output_type> Salsa::Hash(){
  // check the `output_type`
  assert(typeid(output_type) == typeid(uint8_t) || typeid(output_type) == typeid(uint32_t)
         && "warning: Invalid output data type for Salsa object.");
  // generate state
  std::vector<uint32_t> state_copy(state_);
  for( unsigned int index=0; index < rounds_; index += 2 ){
    DoubleRound(state_copy);
  }
  // return final state
  return OutputState<output_type>(state_copy);
}
template<typename output_type>
std::vector<output_type> Salsa::OutputState(const std::vector<uint32_t>& state) const{
  // if `output_type` is  uint32_t, return the casted state
  if( typeid(output_type) == typeid(uint32_t) ){
    return reinterpret_cast<const std::vector<output_type>&>(state);
  }
  // otherwise convert to uint8_t sequence using the reverse of little endian method
  std::vector<uint8_t> output;
  for( unsigned int index = 0; index < 16; index++ ){
    std::vector<uint8_t> sequence = ReverseLittleEndian(state[index] + state_[index]);
    for( auto it : sequence ){
      output.push_back(it);
    }
  }
  return output;
}
#endif  // STREAM_CIPHERS_INCLUDE_SALSA_H_