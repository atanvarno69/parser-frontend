#include "automatonFactory.h"
#include "ebnfParserFactory.h"
#include "matcherFactory.h"
#include "state.h"

namespace enjoy {

    lexer* ebnfParserFactory::getLexer(void) {
        // EOF
        matcher mEOF = matcherFactory::alternate({0x0000, 0x001A});
        automaton aEOF = *automatonFactory::singleSymbol(mEOF);

        // COMMENT
        matcher mRoundOpen = matcherFactory::is(0x0028);
        matcher mRoundClose = matcherFactory::is(0x0029);
        matcher mAsterisk = matcherFactory::is(0x002A);
        matcher mCommentCharA = matcherFactory::alternate({
            matcherFactory::range(0x0020, 0x0029),
            matcherFactory::range(0x002B, 0x007E),
        });
        matcher mCommentCharB = matcherFactory::alternate({
            matcherFactory::range(0x0020, 0x0028),
            matcherFactory::range(0x002B, 0x007E),
        });
        state sComment0 {
            {1, {mRoundOpen, state::cont}}
        };
        state sComment1 {
            {2, {mAsterisk, state::cont}}
        };
        state sComment2 {
            {2, {mCommentCharA, state::cont}},
            {3, {mAsterisk, state::cont}},
        };
        state sComment3 {
            {2, {mCommentCharB, state::cont}},
            {3, {mAsterisk, state::cont}},
            {0, {mRoundClose, state::accept}},
        };
        automaton aComment {
            {0, sComment0},
            {1, sComment1},
            {2, sComment2},
            {3, sComment3},
        };

        // WHITESPACE
        matcher mWS = matcherFactory::alternate({
            matcherFactory::range(0x0009, 0x000D),
            matcherFactory::is(0x0020),
        });
        state sWS0 {
            {0, {mWS, state::contacc}},
        };
        automaton aWS {
            {0, sWS0}
        };

        // IDENTIFIER_NONTERMINAL
        matcher mAlphaLower = matcherFactory::range(0x0061, 0x007A);
        matcher mAlphaUpper = matcherFactory::range(0x0041, 0x005A);
        matcher mUnderscore = matcherFactory::is(0x005F);
        matcher mCharacterIdentifier = matcherFactory::alternate({
            mAlphaLower, mAlphaUpper, mUnderscore
        });
        state sIdN0 {
            {1, {mAlphaLower, state::contacc}},
        };
        state sIdN1 {
            {1, {mCharacterIdentifier, state::contacc}},
        };
        automaton aIdN {
            {0, sIdN0},
            {1, sIdN1},
        };

        // IDENTIFIER_TERMINAL
        state sIdT0 {
            {1, {mAlphaUpper, state::contacc}},
        };
        state sIdT1 {
            {1, {mCharacterIdentifier, state::contacc}},
        };
        automaton aIdT {
            {0, sIdT0},
            {1, sIdT1},
        };

        // LITERAL_UNICODE
        matcher mUu = matcherFactory::alternate({0x0055, 0x0075});
        matcher mPlus = matcherFactory::is(0x002B);
        matcher mDigitDec = matcherFactory::range(0x0030, 0x0039);
        matcher mDigitHex = matcherFactory::alternate({
            mDigitDec,
            matcherFactory::range(0x0041, 0x0046),
            matcherFactory::range(0x0061, 0x0066),
        });
        state sUnicode0 {
            {1, {mUu, state::cont}}
        };
        state sUnicode1 {
            {2, {mPlus, state::cont}}
        };
        state sUnicode2 {
            {3, {mDigitHex, state::contacc}}
        };
        state sUnicode3 {
            {4, {mDigitHex, state::contacc}}
        };
        state sUnicode4 {
            {5, {mDigitHex, state::contacc}}
        };
        state sUnicode5 {
            {6, {mDigitHex, state::contacc}}
        };
        state sUnicode6 {
            {7, {mDigitHex, state::contacc}}
        };
        state sUnicode7 {
            {0, {mDigitHex, state::accept}}
        };
        automaton aUnicode {
            {0, sUnicode0},
            {1, sUnicode1},
            {2, sUnicode2},
            {3, sUnicode3},
            {4, sUnicode4},
            {5, sUnicode5},
            {6, sUnicode6},
            {7, sUnicode7},
        };

        // SPECIAL_DIRECTIVE
        matcher mQuestion = matcherFactory::is(0x003F);
        matcher mCharacterSpecial = matcherFactory::alternate({
            matcherFactory::range(0x0020, 0x003E),
            matcherFactory::range(0x0040, 0x007E),
        });
        state sSpecial0 {
            {1, {mQuestion, state::cont}}
        };
        state sSpecial1 {
            {1, {mCharacterSpecial, state::cont}},
            {0, {mQuestion, state::accept}},
        };
        automaton aSpecial {
            {0, sSpecial0},
            {1, sSpecial1},
        };

        // LITERAL_STRING
        matcher mQuote = matcherFactory::is(0x0022);
        matcher mApostrophe = matcherFactory::is(0x0027);
        matcher mCharacterSingleQuoted = matcherFactory::alternate({
            matcherFactory::range(0x0020, 0x0026),
            matcherFactory::range(0x0028, 0x007E),
        });
        matcher mCharacterDoubleQuoted = matcherFactory::alternate({
            matcherFactory::range(0x0020, 0x0021),
            matcherFactory::range(0x0023, 0x007E),
        });
        state sStr0 {
            {1, {mApostrophe, state::cont}},
            {2, {mQuote, state::cont}},
        };
        state sStr1 {
            {1, {mCharacterSingleQuoted, state::cont}},
            {0, {mApostrophe, state::accept}},
        };
        state sStr2 {
            {2, {mCharacterDoubleQuoted, state::cont}},
            {0, {mQuote, state::accept}},
        };
        automaton aStr {
            {0, sStr0},
            {1, sStr1},
            {2, sStr2},
        };

        // ALTERNATOR
        automaton aAlt = *automatonFactory::singleSymbol(0x007C);

        // CONCATENATION
        automaton aConcat  = *automatonFactory::singleSymbol(0x002C);

        // DEFINE
        automaton aDef  = *automatonFactory::singleSymbol(0x003D);

        // EXCEPT
        automaton aExc = *automatonFactory::singleSymbol(0x002D);

        // RANGE
        matcher mDot = matcherFactory::is(0x002E);
        state sRng0 {
            {1, {mDot, state::cont}}
        };
        state sRng1 {
            {2, {mDot, state::cont}}
        };
        state sRng2 {
            {0, {mDot, state::accept}}
        };
        automaton aRng {
            {0, sRng0},
            {1, sRng1},
            {2, sRng2},
        };

        // REPEAT
        automaton aRpt = *automatonFactory::singleSymbol(mAsterisk);

        // TERMINATOR
        automaton aTerm = *automatonFactory::singleSymbol(0x003B);

        // GROUP_OPEN
        automaton aGrpO = *automatonFactory::singleSymbol(mRoundOpen);

        // GROUP_CLOSE
        automaton aGrpC = *automatonFactory::singleSymbol(mRoundClose);

        // OPTION_OPEN
        automaton aOptO = *automatonFactory::singleSymbol(0x005B);

        // OPTION_CLOSE
        automaton aOptC = *automatonFactory::singleSymbol(0x005D);

        // REPEAT_OPEN
        automaton aRptO= *automatonFactory::singleSymbol(0x007B);

        // REPEAT_CLOSE
        automaton aRptC = *automatonFactory::singleSymbol(0x007D);

        // UNDEFINED
        matcher mUndef = matcherFactory::alternate({
            matcherFactory::range(0x0001, 0x0008),
            matcherFactory::range(0x000E, 0x0019),
            matcherFactory::range(0x001B, 0x001F),
            matcherFactory::is(0x007F),
        });
        state sUndef0 {
            {0, {mUndef, state::accept}}
        };
        automaton aUndef {
            {0, sUndef0}
        };

        // INTEGER
        state sInt0 {
            {0, {mDigitDec, state::contacc}}
        };
        automaton aInt {
            {0, sInt0}
        };

        // Return
        return new lexer {
            {0, aEOF},
            {1, aComment},
            {2, aWS},
            {3, aIdN},
            {4, aIdT},
            {5, aUnicode},
            {6, aSpecial},
            {7, aStr},
            {8, aAlt},
            {9, aConcat},
            {10, aDef},
            {11, aExc},
            {12, aRng},
            {13, aRpt},
            {14, aTerm},
            {15, aGrpO},
            {16, aGrpC},
            {17, aOptO},
            {18, aOptC},
            {19, aRptO},
            {20, aRptC},
            {21, aInt},
            {40, aUndef},
        };
    }

    std::map<unsigned, std::string>* ebnfParserFactory::getDictionary(void) {
        return new std::map<unsigned, std::string> {
            {0, "EOF"},
            {1, "COMMENT"},
            {2, "WHITESPACE"},
            {3, "IDENTIFIER_NONTERMINAL"},
            {4, "IDENTIFIER_TERMINAL"},
            {5, "LITERAL_UNICODE"},
            {6, "SPECIAL_DIRECTIVE"},
            {7, "LITERAL_STRING"},
            {8, "ALTERNATOR"},
            {9, "CONCATENATION"},
            {10, "DEFINE"},
            {11, "EXCEPT"},
            {12, "RANGE"},
            {13, "REPEAT"},
            {14, "TERMINATOR"},
            {15, "GROUP_OPEN"},
            {16, "GROUP_CLOSE"},
            {17, "OPTION_OPEN"},
            {18, "OPTION_CLOSE"},
            {19, "REPEAT_OPEN"},
            {20, "REPEAT_CLOSE"},
            {21, "INTEGER"},
            {22, "unicode_value"},
            {23, "sequence_grouped"},
            {24, "sequence_repeated"},
            {25, "sequence_optional"},
            {26, "bracket_sequence"},
            {27, "identifier"},
            {28, "literal"},
            {29, "primary"},
            {30, "factor"},
            {31, "exception"},
            {32, "term"},
            {33, "definition"},
            {34, "definition_list"},
            {35, "rule_nonterminal"},
            {36, "rule_terminal"},
            {37, "rule"},
            {38, "rule_list"},
            {39, "grammar"},
            {40, "UNDEFINED"},

        };
    }

    std::vector<unsigned>* ebnfParserFactory::getIgnoreList(void) {
        return new std::vector<unsigned> { 1, 2 };
    }

    parser* ebnfParserFactory::getParser(void) {
        std::multimap<unsigned, std::vector<unsigned>> definitions {
            {22, std::vector<unsigned> {5}},
            {22, std::vector<unsigned> {22, 12, 5}},
            {23, std::vector<unsigned> {15, 34, 16}},
            {24, std::vector<unsigned> {19, 34, 20}},
            {25, std::vector<unsigned> {17, 34, 18}},
            {26, std::vector<unsigned> {23}},
            {26, std::vector<unsigned> {24}},
            {26, std::vector<unsigned> {25}},
            {27, std::vector<unsigned> {3}},
            {27, std::vector<unsigned> {4}},
            {28, std::vector<unsigned> {7}},
            {28, std::vector<unsigned> {22}},
            {29, std::vector<unsigned> {26}},
            {29, std::vector<unsigned> {27}},
            {29, std::vector<unsigned> {28}},
            {30, std::vector<unsigned> {29}},
            {30, std::vector<unsigned> {21, 13, 29}},
            {31, std::vector<unsigned> {11, 30}},
            {32, std::vector<unsigned> {30}},
            {32, std::vector<unsigned> {32, 31}},
            {33, std::vector<unsigned> {32}},
            {33, std::vector<unsigned> {33, 9, 32}},
            {34, std::vector<unsigned> {33}},
            {34, std::vector<unsigned> {34, 8, 33}},
            {35, std::vector<unsigned> {3, 10, 34, 14}},
            {35, std::vector<unsigned> {3, 6, 10, 34, 14}},
            {36, std::vector<unsigned> {4, 10, 34, 14}},
            {36, std::vector<unsigned> {4, 6, 10, 34, 14}},
            {37, std::vector<unsigned> {35}},
            {37, std::vector<unsigned> {36}},
            {38, std::vector<unsigned> {37}},
            {38, std::vector<unsigned> {38, 37}},
            {39, std::vector<unsigned> {38, 0}},
            {39, std::vector<unsigned> {0}},

        };
        return new parser { *getLexer(), definitions, *getIgnoreList() };
    }
}