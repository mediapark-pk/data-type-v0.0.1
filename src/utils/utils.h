//
// Created by umar on 23/12/20.
//

#ifndef DATA_TYPES_UTILS_H
#define DATA_TYPES_UTILS_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <iomanip>

namespace Utils {
    /** IT will work if ltrim don't have whitespaces will fix it later **/
    static std::string ltrim(const std::string &untrimmed, const std::string pattern) {
        /** If pattern length is greater than the string error and return **/
        if (pattern.length() > untrimmed.length())
            return untrimmed;

        int i = 0;
        bool isWhiteSpaceFound = false;
        /** if there is any whitespace in the start remove it and return **/
        while (isspace(untrimmed[i]) && i < untrimmed.length()) {
            ++i;
            isWhiteSpaceFound = true;
        }
        /** whitespace found, Remove and return **/
        if (isWhiteSpaceFound) {
            return untrimmed.substr(i);
        } else { /** whitespace Not found, Look for pattern **/
            std::string tempString = untrimmed.substr(0, untrimmed.find_first_of(' '));
            if (untrimmed[0] == pattern[0] && !untrimmed.empty()) {
                if (tempString.length() < pattern.length()) {
                    return untrimmed;
                }
                for (int i = 0; i < untrimmed.length();) {
                    // Check
                    if (pattern == tempString.substr(i, pattern.length())) {
                        i += pattern.length();
                        continue;
                    } else {
                        for (; pattern.find(tempString[i]) != std::string::npos;) {
                            ++i;
                        };
                        return untrimmed.substr(i);
                    }
                }
            } else {
                for (int i = 0; i < untrimmed.length();) {
                    // Check
                    if (pattern == tempString.substr(i, pattern.length())) {
                        i += pattern.length();
                        continue;
                    } else {
                        for (; pattern.find(tempString[i]) != std::string::npos;) {
                            ++i;
                        };
                        return untrimmed.substr(i);
                    }
                }
            }
            return untrimmed;
        }
    }

    static std::string rtrim(std::string untrimmed, std::string pattern) {
        std::reverse(untrimmed.begin(), untrimmed.end());
        std::reverse(pattern.begin(),pattern.end());
        untrimmed = ltrim(untrimmed, pattern);
        std::reverse(untrimmed.begin(), untrimmed.end());
        return untrimmed;
    }

    static std::vector<std::string> explode(const char delimiter, const std::string &string) {
        std::vector<std::string> splits;
        std::string buffer = "";
        std::stringstream stream(string);
        while (std::getline(stream, buffer, delimiter)) {
            splits.push_back(buffer);
        }
        return splits;
    }    // namespace Utils

    static std::string deciToBin(const std::string &string, int numOfBits) {
        int iterator = 0;
        std::vector<int> asciValue(string.length());
        for (const auto ptr : string) {
            asciValue[iterator] = int(ptr);
            ++iterator;
        }
        std::vector<std::string> binaryFromAsci(asciValue.size());
    }
//todo
    /*static std::string base64_encode(std::string data) {
        static constexpr char sEncodingTable[] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

        size_t in_len = data.size();
        size_t out_len = 4 * ((in_len + 2) / 3);
        std::string ret(out_len, '\0');
        size_t i;
        char *p = const_cast<char *>(ret.c_str());

        for (i = 0; i < in_len - 2; i += 3) {
            *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
            *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
            *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int) (data[i + 2] & 0xC0) >> 6)];
            *p++ = sEncodingTable[data[i + 2] & 0x3F];
        }
        if (i < in_len) {
            *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
            if (i == (in_len - 1)) {
                *p++ = sEncodingTable[((data[i] & 0x3) << 4)];
                *p++ = '=';
            } else {
                *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
                *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
            }
            *p++ = '=';
        }

        return ret;
    }*/

    static std::string base64_encode(const std::string &in) {
        std::string out;
        static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        int val = 0, valb = -6;
        for (char c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(b[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) out.push_back(b[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() % 4) out.push_back('=');
        return out;
    }

    /*static std::string base64_decode(const std::string &input) {
        std::string out;
        static constexpr unsigned char kDecodingTable[] = {
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62,
                64, 64, 64, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0,
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                23, 24, 25, 64, 64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};

        size_t in_len = input.size();
        if (in_len % 4 != 0)
            return "Input data size is not a multiple of 4";

        size_t out_len = in_len / 4 * 3;
        if (input[in_len - 1] == '=')
            out_len--;
        if (input[in_len - 2] == '=')
            out_len--;
        out.resize(out_len);
        for (size_t i = 0, j = 0; i < in_len;) {
            uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
            uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
            uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
            uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

            uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

            if (j < out_len)
                out[j++] = (triple >> 2 * 8) & 0xFF;
            if (j < out_len)
                out[j++] = (triple >> 1 * 8) & 0xFF;
            if (j < out_len)
                out[j++] = (triple >> 0 * 8) & 0xFF;
        }
        return out;
    }*/

    static std::string base64_decode(const std::string &in) {

        std::string out;
        static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++) T[b[i]] = i;

        int val = 0, valb = -8;
        for (char c : in) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    enum PAD_TYPE {
        STR_PAD_RIGHT,
        STR_PAD_LEFT
    };

    static std::string
    str_pad(const std::string &input, const int pad_length, const std::string &pad_string, PAD_TYPE padType) {
        std::string tempString;
        switch (padType) {
            case PAD_TYPE::STR_PAD_LEFT:
                if (pad_string.length() > input.length())
                    return input;
                while (tempString.length() <= pad_length) {
                    if (tempString.length() + input.length() + pad_string.length() <= pad_length) {
                        tempString.append(pad_string);
                    } else {
                        for (int i = 0;; ++i) {
                            if (input.length() + tempString.length() < pad_length) {
                                tempString.push_back(pad_string[i]);
                            } else {
                                return tempString.append(input);
                            }
                        }
                    }
                }
                return tempString.append(input);

            case PAD_TYPE::STR_PAD_RIGHT:
                if (pad_string.length() > input.length())
                    return input;
                while (tempString.length() <= pad_length) {
                    if (tempString.length() + input.length() + pad_string.length() <= pad_length) {
                        tempString.append(pad_string);
                    } else {
                        for (int i = 0;; ++i) {
                            if (input.length() + tempString.length() < pad_length) {
                                tempString.push_back(pad_string[i]);
                            } else {
                                return std::string(input).append(tempString);
                            }
                        }
                    }
                }
                return std::string(input).append(tempString);
        }
    }

    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base *>(ptr) != nullptr;
    }

    static std::string decToHex(const int decimalValue) {
        std::stringstream ss;
        ss << std::hex << decimalValue; // int decimal_value
        std::string res(ss.str());
        return res;
    }

    static int hexToDec(const std::string &hexValue) {
        std::stringstream ss;
        int decimalValue = 0;
        ss << hexValue; // std::string hex_value
        ss >> std::hex >> decimalValue; //int decimal_value
        return decimalValue;
    }

    static std::string chunk_split(std::string &sString, int length, char seperator) {
        std::string outstring = "";
        outstring = sString.substr(0, length);
        for (int loc = length; loc < sString.size(); loc += length) {
            outstring = outstring + seperator + sString.substr(loc, length);
        }
        return outstring;
    }

    static std::string number_format(float num, int decimals, char deci_sep = '.', char thous_sep = ',') {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(decimals) << num;
        std::string s = stream.str();
        return s;
    }
    static std::string trim(const std::string &string) {
        if (!string.empty()) {
            int start = 0, end = string.length() - 1;
            while (string[start] != '\0') {
                if (!isspace(string[start]))
                    break;
                ++start;
            }
            if (start == end && end == -1)
                return "";
            while (end >= 0) {
                if (!isspace(string[end]))
                    break;
                --end;
            }
            return string.substr(start, end + 1);
        }
        return string;
    }

};    // namespace Utils

#endif    // DATA_TYPES_UTILS_H
