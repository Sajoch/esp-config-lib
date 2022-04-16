#pragma once
#include <Wire.h>

#include <cinttypes>
#include <string>

class ConfigImpl {
 public:
  ConfigImpl(TwoWire* peripherialBus, uint8_t address, uint8_t version);
  bool IsValid();
  std::string ReadString(uint16_t dataAddress);
  std::string ReadSecureString(uint16_t dataAddress);
  uint16_t ReadUint16(uint16_t dataAddress);

 private:
  uint16_t ReadContent(uint16_t dataAddress, uint8_t* output, uint16_t size);
  bool ReadByte(uint16_t dataAddress, uint8_t* output);
  bool ReadUint16(uint16_t dataAddress, uint16_t* output);

  TwoWire* peripherialBus_;
  uint8_t address_;
  const uint8_t version_;
  const uint8_t secureByte_;
};