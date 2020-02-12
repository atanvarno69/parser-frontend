#ifndef PARSER_FRONTEND_AUTOMATONFACTORY_H
#define PARSER_FRONTEND_AUTOMATONFACTORY_H

#include <utility>

#include "automaton.h"

namespace enjoy {
    class automatonFactory {
      public:
        automatonFactory(void) = delete;
        automatonFactory(const automatonFactory&) = delete;
        automatonFactory(automatonFactory&&) = delete;
        ~automatonFactory(void) = delete;
        automatonFactory& operator=(const automatonFactory&) = delete;
        automatonFactory& operator=(automatonFactory&&) = delete;
        static automaton* singleSymbol(const codepoint&);
        static automaton* singleSymbol(const matcher&);
    };
}

#endif