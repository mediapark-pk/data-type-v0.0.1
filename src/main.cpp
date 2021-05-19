#include <iostream>

#include "buffer/base16.h"


int main() {
   Buffer::Base16 obj("0x21321312");
   obj.hexits(false);
   std::cout << obj.value() << std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
