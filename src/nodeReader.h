#ifndef PARSER_FRONTEND_NODEREADER_H
#define PARSER_FRONTEND_NODEREADER_H

#include <map>
#include <string>

#include "visitor.h"

namespace enjoy {
    class nodeReader: public visitor {
      private:
        std::map<unsigned, std::string> dictionary;
      public:
        nodeReader(void) = default;
        explicit nodeReader(std::map<unsigned, std::string>) noexcept;
        nodeReader(const nodeReader&) = default;
        nodeReader(nodeReader&&) noexcept = default;
        ~nodeReader(void) = default;
        nodeReader& operator=(const nodeReader&) = default;
        nodeReader& operator=(nodeReader&&) noexcept = default;
        node* visit(node*) noexcept override;
      private:
        void printNode(node*, unsigned) noexcept;
    };
}

#endif