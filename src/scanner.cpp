#include <utility>

#include "scanner.h"

namespace enjoy {

    scanner::scanner(void) noexcept:
        encoded(utf8),
        memory(),
        stream(nullptr),
        scan([this](void)->codepoint{return this->getUtf8();})
    {
        detect();
    }

    scanner::scanner(std::istream* stream) noexcept:
        encoded(utf8),
        memory(),
        stream(stream),
        scan([this](void)->codepoint{return this->getUtf8();})
    {
        detect();
    }

    scanner::scanner(const scanner& that) noexcept:
        encoded(that.encoded),
        memory(that.memory),
        stream(that.stream),
        scan([this](void)->codepoint{return this->getUtf8();})
    {
        setup();
    }

    scanner::scanner(scanner&& that) noexcept:
        encoded(that.encoded),
        memory(std::move(that.memory)),
        stream(that.stream),
        scan()
    {
        setup();
    }

    scanner& scanner::operator=(const scanner& that) noexcept {
        encoded = that.encoded;
        memory = that.memory;
        stream = that.stream;
        setup();
        return *this;
    }

    scanner& scanner::operator=(scanner&& that) noexcept {
        encoded = that.encoded;
        memory = std::move(that.memory);
        stream = that.stream;
        setup();
        return *this;
    }

    void scanner::detect(void) noexcept {
        byte bytes [4];
        for (unsigned char& byte: bytes) {
            byte = readStream();
            memory.push(byte);
        }
        switch (bytes[0]) {
          case 0:
            if (bytes[1] == 0 && bytes[2] == 0xfe && bytes[3] == 0xff) {
                encoded = utf32be;
            }
            break;
          case 0xff:
            if (bytes[1] == 0xfe) {
                encoded = (bytes[2] == 0 && bytes[3] == 0) ? utf32le : utf16le;
            }
            break;
          case 0xfe:
            if (bytes[1] == 0xff) {
                encoded = utf16be;
            }
            break;
          case 0xef:
            if (bytes[1] == 0xbb && bytes[2] == 0xbf) {
                encoded = utf8;
            }
            break;
          default:
            encoded = utf8;
            break;
        }
    }

    void scanner::setup(void) noexcept {
        switch (encoded) {
          default:
            // no break
          case utf8:
            scan = [this](void)->codepoint{return this->getUtf8();};
            break;
          case utf16be:
            scan = [this](void)->codepoint{return this->getUtf16(true);};
            break;
          case utf16le:
            scan = [this](void)->codepoint{return this->getUtf16(false);};
            break;
          case utf32be:
            scan = [this](void)->codepoint{return this->getUtf32(true);};
            break;
          case utf32le:
            scan = [this](void)->codepoint{return this->getUtf32(false);};
            break;
        }
    }

    scanner::byte scanner::readStream(void) noexcept {
        if (stream == nullptr) {
            return 0;
        }
        int output = stream->get();
        return (output == std::istream::traits_type::eof()) ? 0 : output;
    }

    scanner::byte scanner::getNextByte(void) noexcept {
        if (!memory.empty()) {
            byte output = memory.front();
            memory.pop();
            return output;
        }
        return readStream();
    }

    scanner::codeunit scanner::getCodeunit(const bool& bigEndian) noexcept {
        byte bytes [2] { getNextByte(), getNextByte() };
        codeunit output = 0;
        if (bigEndian) {
            output = (bytes[0] << 8) | bytes[1];
        } else {
            output = (bytes[1] << 8) | bytes[0];
        }
        return output;
    }

    codepoint scanner::getUtf8(void) noexcept {
        byte leading = getNextByte();
        if (leading < 0x7F) {
            return leading;
        }
        codepoint output = 0;
        int following = 0;
        if (leading >= 0xC0 && leading <= 0xDF) {
            output = leading & 0x1F;
            following = 1;
        }
        if (leading >= 0xE0 && leading <= 0xEF) {
            output = leading & 0x0F;
            following = 2;
        }
        if (leading >= 0xF0 && leading <= 0xF7) {
            output = leading & 0x07;
            following = 3;
        }
        for (int i = 0; i < following; ++i) {
            output = (output << 6) | (getNextByte() & 0x3F);
        }
        return output;
    }

    codepoint scanner::getUtf16(const bool& bigEndian) noexcept {
        codeunit leading = getCodeunit(bigEndian);
        if (
            (leading >= 0x0000 && leading <= 0xD7FF)
         || (leading >= 0xE000 && leading <= 0xFFFF)
        ) {
            return leading;
        }
        codepoint output = leading - 0xD800;
        return ((output << 10) | (getCodeunit(bigEndian) - 0xDC00)) + 0x10000;
    }

    codepoint scanner::getUtf32(const bool& bigEndian) noexcept {
        byte bytes [4] {
            getNextByte(), getNextByte(), getNextByte(), getNextByte()
        };
        codepoint output = 0;
        if (bigEndian) {
            for (byte byte: bytes) {
                output = (output << 8) | byte;
            }
        } else {
            for (int i = 3; i >= 0; --i) {
                output = (output << 8) | bytes[i];
            }
        }
        return output;
    }
}