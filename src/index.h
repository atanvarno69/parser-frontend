#ifndef PARSER_FRONTEND_INDEX_H
#define PARSER_FRONTEND_INDEX_H

#include <string>

namespace enjoy {
    class index {
      public:
        unsigned line;
        unsigned column;
        index(void) noexcept;
        index(const unsigned&, const unsigned&) noexcept;
        index(const index&) = default;
        index(index&&) noexcept = default;
        ~index(void) = default;
        index& operator=(const index&) = default;
        index& operator=(index&&) noexcept = default;
        [[nodiscard]] std::string toString(void) const noexcept;
    };
}

#endif