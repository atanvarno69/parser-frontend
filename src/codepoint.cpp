#include <iomanip>
#include <sstream>

#include "codepoint.h"

namespace enjoy {
    std::string toString(const codepoint& input) noexcept {
        std::ostringstream output {""};
        output << "U+" << std::hex << std::uppercase << std::setw(4)
               << std::setfill('0') << input;
        return output.str();
    }
}