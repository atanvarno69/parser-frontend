#include "ebnfAstGenerator.h"

namespace enjoy {

    node* ebnfAstGenerator::visit(node* host) noexcept {
        std::stack<node*> stack;
        populatePostOrder(host, stack);
        node* output = new node {host->type, host->position};
        std::stack<node*> outputChildren;
        while (!stack.empty()) {
            if (stack.top()->type == 0)  {
                outputChildren.push(stack.top());
            }
            if (stack.top()->type == 35 || stack.top()->type == 36) {
                outputChildren.push(stack.top());
            }
            if (stack.top()->type == 34) {
                // replace(current, implode(current, 33, 34));
            }
            stack.pop();
        }
        while (!outputChildren.empty()) {
            output->addChild(outputChildren.top());
            outputChildren.pop();
        }
        return output;
    }
}