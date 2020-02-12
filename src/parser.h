#ifndef PARSER_FRONTEND_PARSER_H
#define PARSER_FRONTEND_PARSER_H

#include "lexer.h"
#include "node.h"
#include "scanner.h"

#include <map>
#include <stack>
#include <utility>
#include <vector>

namespace enjoy {
    class parser {
      private:
        std::multimap<unsigned, std::vector<unsigned>> definitions;
        std::map<unsigned, std::vector<unsigned>> deny;
        std::vector<unsigned> ignore;
        lexer inputLexer;
        node* lookahead;
        std::stack<node*> stack;
        std::vector<unsigned> stackTypes;
        struct result {
            unsigned size;
            unsigned type;
        };
      public:
        parser(void) = delete;
        parser(
            lexer,
            std::multimap<unsigned, std::vector<unsigned>>
        ) noexcept;
        parser(
            lexer,
            std::multimap<unsigned, std::vector<unsigned>>,
            std::vector<unsigned>
        ) noexcept;
        parser(const parser&) = default;
        parser(parser&&) noexcept = default;
        ~parser(void) = default;
        parser& operator=(const parser&) = default;
        parser& operator=(parser&&) noexcept = default;
        node* parse(scanner&);
      private:
        enum stage {
            first,
            second,
            third,
        };
        result canReduce(const bool&) noexcept;
        void parseLoop(scanner&, const stage&);
        void populateDenyList(void) noexcept;
        void reduce(const result&) noexcept;
        void shift(scanner&);
        void updateLookahead(scanner&);
    };
}

#endif