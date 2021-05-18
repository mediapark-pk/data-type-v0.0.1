//
// Created by umar on 10/12/20.
//

#ifndef DATA_TYPES_BASE16_H
#define DATA_TYPES_BASE16_H

#include "abstractbuffer.h"
#include "utils/strencodings.h"
#include "validator.h"

#include <memory>

namespace Buffer
{
class Binary;

class Base16 : public Buffer::AbstractBuffer
{
   private:
   //        CPPBase16::Decoder *m_decoder;
   ////        std::string m_data;    // added to remove error

   public:
   Base16() = default;

   Base16(const std::string& data);    // check required
   std::string validatedDataTypeValue(const std::string& data) override;
   std::string hexits(bool prefix = false);
//   std::shared_ptr<Buffer::Binary> binary();
//   std::unique_ptr<CPPBase16::Decoder> decode();
};

};    // namespace Buffer

#endif    // DATA_TYPES_BASE16_H
