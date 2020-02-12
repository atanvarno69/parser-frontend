#ifndef PARSER_FRONTEND_NODE_H
#define PARSER_FRONTEND_NODE_H

#include <map>
#include <string>
#include <vector>

#include "codepoint.h"
#include "index.h"

namespace enjoy {
    class visitor;

    class node {
      public:
        node* parent;
        std::vector<node*> children;
        index position;
        unsigned type;
        std::vector<codepoint> value;
        node(void) noexcept;
        node(const unsigned&, const index&) noexcept;
        node(
            const unsigned&, const index&, std::vector<codepoint>
        ) noexcept;
        node(const node&) noexcept;
        node(node&&) noexcept;
        ~node(void) noexcept;
        node& operator=(const node&) = default;
        node& operator=(node&&) = default;
        void addChild(node*) noexcept;
        void setParent(node*) noexcept;
        [[nodiscard]] std::string typeString(
            const std::map<unsigned, std::string>&
        ) const noexcept;
        [[nodiscard]] std::string valueString(void) const noexcept;
        void accept(visitor&) noexcept;
    };
}

#endif