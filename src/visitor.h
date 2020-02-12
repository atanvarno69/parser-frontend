#ifndef PARSER_FRONTEND_VISITOR_H
#define PARSER_FRONTEND_VISITOR_H

#include <stack>

#include "node.h"

namespace enjoy {

    class visitor {
      public:
        virtual node* visit(node*) noexcept = 0;
      protected:
        static node* implode(node*, const unsigned&, const unsigned&) noexcept;
        static void populatePostOrder(node*, std::stack<node*>&) noexcept;
        static void populatePreOrder(node*, std::stack<node*>&) noexcept;
        static void replace(node*, node*) noexcept;
    };
}

#endif
