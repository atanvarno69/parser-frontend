#include <algorithm>

#include "visitor.h"

namespace enjoy {

    node* visitor::implode(
        node* root,
        const unsigned& pluralType,
        const unsigned& collectionType
    ) noexcept {
        node* output = new node {collectionType, root->position};
        std::stack<node*> stack;
        populatePreOrder(root, stack);
        while (!stack.empty()) {
            if (stack.top()->type == pluralType) {
                output->addChild(stack.top());
            }
            stack.pop();
        }
        return output;
    }

    void visitor::populatePostOrder(
        node* root, std::stack<node*>& stack
    ) noexcept {
        stack.push(root);
        for (node* child: root->children) {
            populatePostOrder(child, stack);
        }
    }

    void visitor::populatePreOrder(
        node* root, std::stack<node*>& stack
    ) noexcept {
        for (node* child: root->children) {
            populatePostOrder(child, stack);
        }
        stack.push(root);
    }

    void visitor::replace(node* old, node* replacement) noexcept {
        replacement->parent = old->parent;
        std::replace(old->parent->children.begin(), old->parent->children.end(), old, replacement);
    }
}