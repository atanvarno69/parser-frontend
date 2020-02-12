#ifndef PARSER_FRONTEND_MATCHER_H
#define PARSER_FRONTEND_MATCHER_H

#include <functional>

#include "codepoint.h"

namespace enjoy {
    typedef std::function<bool(const codepoint&)> matcher;
}

#endif