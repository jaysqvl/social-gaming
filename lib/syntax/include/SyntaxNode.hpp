#pragma once

#include <string>
#include <vector>
#include <memory>

class SyntaxNode {
public:
    SyntaxNode(const std::string &type, const std::string &text, int number);
    std::string getType(void);
    std::string getText(void);
    int getNumber(void);

    size_t size(void);
    SyntaxNode &getChild(size_t index);
    void append(std::unique_ptr<SyntaxNode> child);

    void print(size_t depth);
private:
    std::string type;
    std::string text;
    int number;

    std::vector<std::unique_ptr<SyntaxNode>> children;
};
