
#include "binary.h"
//#include "base64.h"
//#include "binary/lensize.h"
//#include "bitwise.h"

Buffer::Binary::Binary(const std::string& data) : AbstractBuffer(data) {}

std::string Buffer::Binary::validatedDataTypeValue(const std::string& data)
{
   return data.empty() ? "" : data;
}

std::string Buffer::Binary::raw()
{
   return this->value().empty() ? "" : this->value();
}
int Buffer::Binary::sizeInBytes()
{
   return getBufferData().length();
}
int Buffer::Binary::sizeInBits()
{
   return getBufferData().length() * 8;
}

//TODO:: ONCE BASE16 IS WRITTEN WTF
//Buffer::Base16 Buffer::Binary::base16()
//{
//   return Base16(binToHex(this->raw()));
//}

// TODO :: Dependency on base64
// Buffer::Base64 Buffer::Binary::base64()
//{
//    return Buffer::Base64(Utils::base64_encode(this->raw()));
// }

// TODO :: Dependency on base16
// Buffer::Bitwise Buffer::Binary::bitwise()
//{
//    std::string hexits = this->base16().value();
//    if (hexits.empty())
//       throw std::string{" Base16 Buffer is null or empty "} + std::strerror(errno);
//    int expectedBits = hexits.length() * 4;
//    std::string bitwise = CPPBcMath::BcMath::BaseConvert(hexits, 16, 2);
//    if (bitwise.length() < expectedBits) {
//       while (bitwise.length() < expectedBits)
//          bitwise = bitwise + "0";
//    }
//    return Buffer::Bitwise(bitwise);
// }


CAlgo::Digest Buffer::Binary::hash()
{
   return CAlgo::Digest(*this);
}
