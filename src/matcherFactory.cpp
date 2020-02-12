#include <vector>

#include "matcherFactory.h"

namespace enjoy {

    matcher matcherFactory::is(const codepoint value) noexcept {
        return [value](const codepoint& input)->codepoint {
            return input == value;
        };
    }

    matcher matcherFactory::range(
        const codepoint min,
        const codepoint max
    ) noexcept {
        return [min, max](const codepoint& input)->codepoint {
            return input >= min && input <= max;
        };
    }

    matcher matcherFactory::neg(const matcher& value) noexcept {
        return [value](const codepoint& input)->codepoint {
            return !value(input);
        };
    }

    matcher matcherFactory::conjunction(
        std::initializer_list<matcher> il
    ) noexcept {
        std::vector<matcher> values {il};
        return [values](const codepoint& input)->codepoint {
            for (const auto& value: values) {
                if (!value(input)) {
                    return false;
                }
            }
            return true;
        };
    }

    matcher matcherFactory::alternate(
        std::initializer_list<codepoint> il
    ) noexcept {
        std::vector<codepoint> values {il};
        return [values](const codepoint& input)->codepoint {
            for (const auto& value: values) {
                if (input == value) {
                    return true;
                }
            }
            return false;
        };
    }

    matcher matcherFactory::alternate(
        std::initializer_list<matcher> il
    ) noexcept {
        std::vector<matcher> values {il};
        return [values](const codepoint& input)->codepoint {
            for (const auto& value: values) {
                if (value(input)) {
                    return true;
                }
            }
            return false;
        };
    }
}