#include "SyntaxGenerator.hpp"


SyntaxGenerator::SyntaxGenerator(void) {

}
std::unique_ptr<SyntaxNode> SyntaxGenerator::generate(
        const std::string & name, const ts::Node &node) {
    return std::make_unique<SyntaxNode>("program", name, 0);
}
