

#ifndef DATA_TYPES_ABSTRACTBUFFER_H
#define DATA_TYPES_ABSTRACTBUFFER_H

#include <regex>
#include <string>
#include "utils/utils.h"
#include "configexception.h"

namespace Buffer
{
class AbstractBuffer
{
   std::string m_data;
   int m_len;
   int m_size;
   bool m_readOnly;

   public:
   AbstractBuffer();
   AbstractBuffer(std::string data);

   std::string serialize();
   void unSerialize(const std::string& serializeString);
   void setBufferData(const std::string& validatedData);

   std::string value(const int start = 0, const int length = 0);
   int getSize();
   int len();

   AbstractBuffer* append(AbstractBuffer& data);
   AbstractBuffer* prepend(AbstractBuffer& data);
   AbstractBuffer* readOnly(bool set);
   AbstractBuffer* set(const std::string data = "");

   virtual std::string validatedDataTypeValue(const std::string& data) = 0;
};
};    // namespace Buffer

#endif    // DATA_TYPES_ABSTRACTBUFFER_H