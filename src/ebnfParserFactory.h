#ifndef PARSER_FRONTEND_EBNFPARSERFACTORY_H
#define PARSER_FRONTEND_EBNFPARSERFACTORY_H

#include <map>
#include <string>
#include <vector>

#include "parser.h"

namespace enjoy {
    class ebnfParserFactory {
      public:
        ebnfParserFactory(void) = delete;
        ebnfParserFactory(const ebnfParserFactory&) = delete;
        ebnfParserFactory(ebnfParserFactory&&) = delete;
        ~ebnfParserFactory(void) = delete;
        ebnfParserFactory& operator=(const ebnfParserFactory&) = delete;
        ebnfParserFactory& operator=(ebnfParserFactory&&) noexcept = delete;
        static lexer* getLexer(void);
        static std::map<unsigned, std::string>* getDictionary(void);
        static std::vector<unsigned>* getIgnoreList(void);
        static parser* getParser(void);
    };
}

#endif