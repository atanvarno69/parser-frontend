#include <set>
#include <stdexcept>
#include <utility>

#include "lexer.h"

namespace enjoy {

    tracker::tracker(void): crFlag(false), position() {}

    void tracker::incrementColumn(void) noexcept {
        ++position.column;
    }

    void tracker::incrementLine(void) noexcept {
        ++position.line;
        position.column = 1;
    }

    void tracker::increment(const codepoint& input) {
        if ((input >= 0x000a && input <= 0x000d)
            || input == 0x0085
            || input == 0x2028
            || input == 0x2029
            ) {
            if (crFlag) {
                if (input != 0x000a) {
                    incrementLine();
                }
                crFlag = false;
            } else {
                if (input == 0x000d) {
                    crFlag = true;
                }
                incrementLine();
            }
        } else {
            crFlag = false;
            incrementColumn();
        }
    }

    lexer::lexer(
        std::unordered_map<unsigned, automaton>  automata
    ) noexcept:
        automata(std::move(automata)),
        locationManager(),
        memory()
    {}

    lexer::lexer(
        std::initializer_list<std::pair<const unsigned, automaton>> il
    ) noexcept:
        automata(il),
        locationManager(),
        memory()
    {}

    token* lexer::lex(scanner& input) {
        std::set<unsigned> continuing;
        for (auto const& value: automata) {
            continuing.insert(value.first);
        }
        std::vector<codepoint> collection;
        std::set<unsigned> lastAccepting;
        const index startingLocation {locationManager.position};
        while (!continuing.empty()) {
            codepoint next = peek(input);
            // Skip byte order mark
            if (
                locationManager.position.line == 1
             && locationManager.position.column == 1
             && next == 0xfeff
            ) {
                pop(input, false);
                continue;
            }
            std::set<uint> accepting;
            std::set<uint> deleting;
            for (const auto& tokenType: continuing) {
                state::result res = automata.at(tokenType).accept(next);
                switch (res) {
                    case state::result::fail:
                        deleting.insert(tokenType);
                        break;
                    case state::result::cont:
                        break;
                    case state::result::accept:
                        accepting.insert(tokenType);
                        deleting.insert(tokenType);
                        break;
                    case state::result::contacc:
                        accepting.insert(tokenType);
                        break;
                }
            }
            for (const auto& tokenType: deleting) {
                continuing.erase(tokenType);
            }
            if (!continuing.empty() || !accepting.empty()) {
                collection.push_back(pop(input, true));
            }
            if (!accepting.empty()) {
                lastAccepting.clear();
                lastAccepting.insert(accepting.begin(), accepting.end());
            }
        }
        if (lastAccepting.size() != 1) {
            std::string msg = "Lex error at " + startingLocation.toString();
            throw std::runtime_error(msg);
        }
        token* value = new token(
            *(lastAccepting.begin()), startingLocation, collection
        );
        return value;
    }

    codepoint lexer::peek(scanner& input) noexcept {
        if (!memory.empty()) {
            return memory.front();
        }
        codepoint output = input.scan();
        memory.push(output);
        return output;
    }

    codepoint lexer::pop(scanner& input, const bool& record) noexcept {
        codepoint output = 0;
        if (!memory.empty()) {
            output = memory.front();
            memory.pop();
        } else {
            output = input.scan();
        }
        if (record) {
            locationManager.increment(output);
        }
        return output;
    }
}