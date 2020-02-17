#include <stdexcept>
#include <utility>

#include "node.h"
#include "visitor.h"

namespace enjoy {

    node::node(void) noexcept:
        parent(nullptr),
        children(),
        position(),
        type(0),
        value()
    {}

    node::node(const unsigned& typeId, const index& location) noexcept:
        parent(nullptr),
        children(),
        position(location),
        type(typeId),
        value()
    {}

    node::node(
        const unsigned& typeId,
        const index& location,
        std::vector<codepoint> valueList
    ) noexcept:
        parent(nullptr),
        children(),
        position(location),
        type(typeId),
        value(std::move(valueList))
    {}

    node::node(const node& that) noexcept:
        parent(nullptr),
        children(that.children),
        position(that.position),
        type(that.type),
        value(that.value)
    {}

    node::node(node&& that) noexcept:
        parent(nullptr),
        children(std::move(that.children)),
        position(that.position),
        type(that.type),
        value(std::move(that.value))
    {}

    node::~node(void) noexcept {
        for (node* child: children) {
            delete child;
        }
    }

    void node::addChild(node* child) noexcept {
        children.push_back(child);
        child->setParent(this);
    }

    void node::setParent(node* newParent) noexcept {
        parent = newParent;
    }

    std::string node::typeString(
        const std::map<unsigned, std::string>& dictionary
    ) const noexcept {
        std::string output;
        try {
            output = dictionary.at(type);
        } catch (std::out_of_range& error) {
            output = "UNIDENTIFIED";
        }
        return output;
    }

    std::string node::valueString(void) const noexcept {
        std::string output;
        unsigned length = value.size();
        for (unsigned i = 0; i < length; ++i) {
            output += toString(value[i]);
            if (i != length - 1) {
                output += " ";
            }
        }
        return output;
    }
}
