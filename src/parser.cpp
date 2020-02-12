#include <algorithm>
#include <stdexcept>
#include <utility>

#include "parser.h"

namespace enjoy {

    parser::parser(
        lexer input,
        std::multimap<unsigned, std::vector<unsigned>> definitions
    ) noexcept:
        definitions(std::move(definitions)),
        deny(),
        ignore(),
        inputLexer(std::move(input)),
        lookahead(nullptr),
        stack(),
        stackTypes()
    {
        populateDenyList();
    }

    parser::parser(
        lexer input,
        std::multimap<unsigned, std::vector<unsigned>> definitions,
        std::vector<unsigned> ignore
    ) noexcept:
        definitions(std::move(definitions)),
        deny(),
        ignore(std::move(ignore)),
        inputLexer(std::move(input)),
        lookahead(nullptr),
        stack(),
        stackTypes()
    {
        populateDenyList();
    }

    node* parser::parse(scanner& input) {
        updateLookahead(input);
        parseLoop(input, first);
        parseLoop(input, second);
        parseLoop(input, third);
        return stack.top();
    }

    parser::result parser::canReduce(const bool& checkLookahead) noexcept {
        result output {0, 0 };
        if (stackTypes.empty()) {
            return output;
        }
        // Check if can currently reduce
        for (auto it = stackTypes.begin(); it != stackTypes.end(); ++it) {
            std::vector<unsigned> checkValues {it, stackTypes.end()};
            for (const auto& definition: definitions) {
                if (definition.second.size() != checkValues.size()) {
                    continue;
                }
                if (definition.second == checkValues) {
                    output.size = definition.second.size();
                    output.type = definition.first;
                }
            }
            if (output.size != 0) {
                if (checkLookahead && deny.find(stackTypes.back()) != deny.end()) {
                    for (const auto& denyValue: deny.at(stackTypes.back())) {
                        if (denyValue == lookahead->type) {
                            output.size = 0;
                            output.type = 0;
                        }
                    }
                }
                break;
            }
        }
        return output;
    }

    void parser::parseLoop(scanner& input, const stage& stage) {
        result test {0, 0};
        do {
            test = canReduce(true);
            if (test.size != 0) {
                reduce(test);
                continue;
            }
            if (stage != third) {
                shift(input);
            } else {
                throw std::runtime_error("Can't parse input");
            }
        } while (stage == first ? lookahead->type != 0 : stage == second ? test.size != 0 : stack.size() > 1);
    }

    void parser::populateDenyList(void) noexcept {
        for (const auto& definition: definitions) {
            auto last = definition.second.size() - 1;
            for (unsigned i = 0; i < last; ++i) {
                deny[definition.second[i]].push_back(definition.second[i + 1]);
            }
        }
    }

    void parser::reduce(const result& result) noexcept {
        std::vector<node*> nodes;
        for (unsigned i = 0; i < result.size; ++i) {
            nodes.push_back(stack.top());
            stack.pop();
            stackTypes.pop_back();
        }
        std::reverse(nodes.begin(), nodes.end());
        node* output = new node {result.type, nodes.front()->position};
        for (node* child: nodes) {
            output->addChild(child);
        }
        stack.push(output);
        stackTypes.push_back(stack.top()->type);
    }

    void parser::shift(scanner& input) {
        stack.push(lookahead);
        stackTypes.push_back(stack.top()->type);
        updateLookahead(input);
    }

    void parser::updateLookahead(scanner& input) {
        lookahead = inputLexer.lex(input);
        auto result = std::find(ignore.begin(), ignore.end(), lookahead->type);
        if (result != ignore.end()) {
            updateLookahead(input);
        }
    }
}