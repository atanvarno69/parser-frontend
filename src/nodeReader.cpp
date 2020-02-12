#include "nodeReader.h"

#include <iostream>
#include <utility>

namespace enjoy {

    nodeReader::nodeReader(
        std::map<unsigned, std::string>  dictionary
    ) noexcept:
        dictionary(std::move(dictionary))
    {}

    node* nodeReader::visit(node* host) noexcept {
        printNode(host, 0);
        return host;
    }

    void nodeReader::printNode(node* host, unsigned indent) noexcept {
        if (indent > 0) {
            for (unsigned i = 1; i < indent; ++i) {
                std::cout << " │";
            }
            std::cout << " ├ ";
        }
        std::cout << host->position.toString() << ": "
                  << dictionary[host->type] << std::endl;
        for (node* child: host->children) {
            printNode(child, indent + 1);
        }
    }


}