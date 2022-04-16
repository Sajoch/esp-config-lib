#include "ConfigImpl.h"

const uint8_t VERSION = 1;

ConfigImpl::ConfigImpl(TwoWire* peripherialBus, uint8_t address,
                       uint8_t version, uint8_t secureByte)
    : peripherialBus_(peripherialBus), address_(address), version_(version),
    secureByte_(secureByte) {}

bool ConfigImpl::IsValid() {
  uint8_t header[8] = {0};
  if (ReadContent(0, header, 8) != 8) return false;
  if (header[0] != 0x80 || header[1] != 0 || header[2] != 0x81 ||
      header[3] != 0 || header[4] != version_)
    return false;
  return true;
}

uint16_t ConfigImpl::ReadContent(uint16_t dataAddress, uint8_t* output,
                                 uint16_t size) {
  uint16_t realAddress;
  if (!ReadUint16(dataAddress, &realAddress)) return 0;

  uint16_t length;
  if (!ReadUint16(realAddress, &length)) return 0;

  if (size < length || output == 0) return length;

  for (auto i = 0; i < length; ++i) {
    if (!ReadByte(realAddress + i + 2, &output[i])) return 0;
  }

  return length;
}

bool ConfigImpl::ReadByte(uint16_t dataAddress, uint8_t* output) {
  peripherialBus_->beginTransmission(address_);

  if (peripherialBus_->write(dataAddress) != sizeof(dataAddress)) return false;

  if (peripherialBus_->endTransmission() != 0) return false;

  if (peripherialBus_->requestFrom(address_, (uint8_t)1) != 1) return false;

  delay(1);

  if (!peripherialBus_->available()) return false;

  *output = (uint8_t)peripherialBus_->read();
  return true;
}

bool ConfigImpl::ReadUint16(uint16_t dataAddress, uint16_t* output) {
  uint8_t data;

  if (!ReadByte(dataAddress, &data)) return false;
  *output = (data << 8);

  if (!ReadByte(dataAddress + 1, &data)) return false;
  *output |= data;

  return true;
}

std::string ConfigImpl::ReadString(uint16_t dataAddress) {
  auto size = ReadContent(dataAddress, 0, 0);
  if (size == 0) return "";
  std::string data(size, 0);
  if (ReadContent(dataAddress, (uint8_t*)&data[0], size) != size) return "";
  return data;
}

std::string ConfigImpl::ReadSecureString(uint16_t dataAddress) {
  auto size = ReadContent(dataAddress, 0, 0);
  if (size == 0) return "";
  std::string data(size, 0);
  if (ReadContent(dataAddress, (uint8_t*)&data[0], size) != size) return "";

  auto xorByte = secureByte;
  for (auto it = data.begin() ; it != data.end(); ++it) {
        std::cout << *it << ' ';
    auto plainCharacter = (*it) ^ xorByte;
    xorByte = (*it) * xorByte;
    *it = plainCharacter;
  }
  
  return data;
}

uint16_t ConfigImpl::ReadUint16(uint16_t dataAddress) {
  uint16_t realAddress;
  if (!ReadUint16(dataAddress, &realAddress)) return 0;

  uint16_t data;
  if (!ReadUint16(realAddress, &data)) return 0;
  return data;
}