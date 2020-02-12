#ifndef PARSER_FRONTEND_EBNFASTGENERATOR_H
#define PARSER_FRONTEND_EBNFASTGENERATOR_H

#include "visitor.h"

namespace enjoy {
    class ebnfAstGenerator: public visitor {
      public:
        ebnfAstGenerator(void) = default;
        ebnfAstGenerator(const ebnfAstGenerator&) = default;
        ebnfAstGenerator(ebnfAstGenerator&&) noexcept = default;
        ~ebnfAstGenerator(void) = default;
        ebnfAstGenerator& operator=(const ebnfAstGenerator&) = default;
        ebnfAstGenerator& operator=(ebnfAstGenerator&&) noexcept = default;
        node* visit(node*) noexcept override;
    };
}

#endif