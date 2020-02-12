#include "index.h"

namespace enjoy {

    index::index(void) noexcept: line(1), column(1) {}

    index::index(const unsigned& line, const unsigned& column) noexcept:
        line(line), column(column)
    {}

    std::string index::toString(void) const noexcept {
        return std::to_string(line) + ":" + std::to_string(column);
    }
}