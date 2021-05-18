
#ifndef DATA_TYPES_BINARY_H
#define DATA_TYPES_BINARY_H

#include "abstractbuffer.h"
#include "base16.h"
//#include "base16/decoder.h"
//
//#include "binary/bytereader.h"
//#include "binary/digest.h"
//#include "binary/lensize.h"
#include "utils/strencodings.h"
#include "algo/digest.h"


namespace Buffer
{
class Binary : public AbstractBuffer
{
   public:
   Binary() = default;
   Binary(const std::string& data);
   std::string validatedDataTypeValue(const std::string& data) override;
   std::string raw();
   int sizeInBytes();
   int sizeInBits();
   CAlgo::Digest hash();

   //    Base16 base16();
   //    Base64 base64();
   //    Buffer::Bitwise bitwise();

   //    CPPBinary::Bytereader read();

   //    Binary* getBinary();
};
}    // namespace Buffer
#endif    // DATA_TYPES_BINARY_H
