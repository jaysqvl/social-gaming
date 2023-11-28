#include <cpp-tree-sitter.h>
#include "demo.h"

#include <iostream>
#include <fstream>
#include <string>

#include <map>
#include <vector>
#include <memory>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

std::string loadFile(const std::string &filename) {
    std::ifstream ifs(filename);
    return std::string(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

Visitor::GameNode::GameNode(
        std::unique_ptr<ConfigurationNode> configuration,
        std::unique_ptr<ConstantsNode> constants,
        std::unique_ptr<VariablesNode> variables,
        std::unique_ptr<PerPlayerNode> perPlayer,
        std::unique_ptr<PerAudienceNode> perAudience,
        std::unique_ptr<RulesSetNode> rules) :
    configuration{std::move(configuration)},
    constants{std::move(constants)},
    variables{std::move(variables)},
    perPlayer{std::move(perPlayer)},
    perAudience{std::move(perAudience)},
    rules{std::move(rules)} {}

void Visitor::GameNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::ConfigurationNode::ConfigurationNode(
        std::unique_ptr<StringNode> name,
        std::unique_ptr<BooleanNode> hasAudience,
        std::unique_ptr<RangeNode> playerRange) :
    name{std::move(name)},
    hasAudience{std::move(hasAudience)},
    playerRange{std::move(playerRange)},
    setupRules{} {}

void Visitor::ConfigurationNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::ConstantsNode::ConstantsNode(
        std::unique_ptr<ValueMapNode> valueMap) :
    valueMap{std::move(valueMap)} {}

void Visitor::ConstantsNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::VariablesNode::VariablesNode(
        std::unique_ptr<ValueMapNode> valueMap) :
    valueMap{std::move(valueMap)} {}

void Visitor::VariablesNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::PerPlayerNode::PerPlayerNode(
        std::unique_ptr<ValueMapNode> valueMap) :
    valueMap{std::move(valueMap)} {}

void Visitor::PerPlayerNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::PerAudienceNode::PerAudienceNode(
        std::unique_ptr<ValueMapNode> valueMap) :
    valueMap{std::move(valueMap)} {}

void Visitor::PerAudienceNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::RulesSetNode::RulesSetNode() {}

void Visitor::RulesSetNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::BodyNode::BodyNode() {}

Visitor::BodyNode::BodyNode(std::vector<ts::Node> nodes) : gameRuleNodes(nodes) {}

void Visitor::BodyNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

//TODO: implement game rule constructors and accept functions!
Visitor::GameRuleNode::GameRuleNode() {}

Visitor::GameRuleNode::GameRuleNode(std::string type, size_t symbol) 
    : ruleType(type), ruleSymbol(symbol) {}

void Visitor::GameRuleNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::SetupRuleNode::SetupRuleNode(
        std::unique_ptr<StringNode> kind,
        std::unique_ptr<StringNode> prompt,
        std::unique_ptr<RangeNode> range,
        std::unique_ptr<Node> choices,
        std::unique_ptr<Node> defaultValue) :
    kind{std::move(kind)},
    prompt{std::move(prompt)},
    range{std::move(range)},
    choices{std::move(choices)},
    defaultValue{std::move(defaultValue)} {}

void Visitor::SetupRuleNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::ValueMapNode::ValueMapNode(void) {}

Visitor::ValueMapNode::ValueMapNode(std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values) : 
values(std::move(values)) {}

void Visitor::ValueMapNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::StringNode::StringNode(std::string value) :
    value{value} {}

void Visitor::StringNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::BooleanNode::BooleanNode(bool value) :
    value{value} {}

void Visitor::BooleanNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}


Visitor::RangeNode::RangeNode(std::pair<int, int> value) :
    value{value} {}

void Visitor::RangeNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::Parser::Parser(const ts::Language &language, const std::string &source) :
    language{language}, source{source}, visitMap{
        { 90, (Command)&Parser::visitGame }
    } {}

std::unique_ptr<Visitor::Node>
Visitor::Parser::visit(const ts::Node &node) {
    auto it = visitMap.find(node.getSymbol());
    if (it != visitMap.end()) {
        return (this->*it->second)(node);
    } else {
        std::cout << node.getType() << " " << node.getSymbol() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Visitor::Node>
Visitor::Parser::visitChildren(const ts::Node &node) {
    ts::Cursor cursor = node.getCursor();
    if (cursor.gotoFirstChild()) {
        do {
            visit(cursor.getCurrentNode());
        } while (cursor.gotoNextSibling());
    }

    return nullptr;
}

std::unique_ptr<Visitor::GameNode>
Visitor::Parser::visitGame(const ts::Node &node) {
    std::unique_ptr<ConfigurationNode> configuration =
        visitConfiguration(node.getChildByFieldName("configuration"));
    std::unique_ptr<ConstantsNode> constants =
        visitConstants(node.getChildByFieldName("constants"));
    std::unique_ptr<VariablesNode> variables =
        visitVariables(node.getChildByFieldName("variables"));
    std::unique_ptr<PerPlayerNode> perPlayer =
        visitPerPlayer(node.getChildByFieldName("per_player"));
    std::unique_ptr<PerAudienceNode> perAudience =
        visitPerAudience(node.getChildByFieldName("per_audience"));
    std::unique_ptr<RulesSetNode> rules =
        visitRules(node.getChildByFieldName("rules"));

    return std::make_unique<GameNode>(
            std::move(configuration),
            std::move(constants),
            std::move(variables),
            std::move(perPlayer),
            std::move(perAudience),
            std::move(rules));
}

std::unique_ptr<Visitor::ConfigurationNode>
Visitor::Parser::visitConfiguration(const ts::Node &node) {
    std::unique_ptr<StringNode> name =
        visitString(node.getChildByFieldName("name"));
    std::unique_ptr<BooleanNode> hasAudience =
        visitBoolean(node.getChildByFieldName("has_audience"));
    std::unique_ptr<RangeNode> playerRange =
        visitRange(node.getChildByFieldName("player_range"));

    std::unique_ptr<ConfigurationNode> result =
        std::make_unique<ConfigurationNode>(
            std::move(name),
            std::move(hasAudience),
            std::move(playerRange));
// how to: handle unnamed children
    ts::Cursor cursor = node.getCursor();
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();
            if (child.getSymbol() == 92) {
                result->setupRules.push_back(visitSetupRule(child));
            }
        } while (cursor.gotoNextSibling());
    }
    return result;
}

std::unique_ptr<Visitor::ConstantsNode>
Visitor::Parser::visitConstants(const ts::Node &node) {
    std::unique_ptr<ValueMapNode> valueMap =
        visitValueMap(node.getChildByFieldName("map"));
    return std::make_unique<ConstantsNode>(std::move(valueMap));
}

std::unique_ptr<Visitor::VariablesNode>
Visitor::Parser::visitVariables(const ts::Node &node) {
    std::unique_ptr<ValueMapNode> valueMap =
        visitValueMap(node.getChildByFieldName("map"));
    return std::make_unique<VariablesNode>(std::move(valueMap));
}

std::unique_ptr<Visitor::PerPlayerNode>
Visitor::Parser::visitPerPlayer(const ts::Node &node) {
    std::unique_ptr<ValueMapNode> valueMap =
        visitValueMap(node.getChildByFieldName("map"));
    return std::make_unique<PerPlayerNode>(std::move(valueMap));
}

std::unique_ptr<Visitor::PerAudienceNode>
Visitor::Parser::visitPerAudience(const ts::Node &node) {
    std::unique_ptr<ValueMapNode> valueMap =
        visitValueMap(node.getChildByFieldName("map"));
    return std::make_unique<PerAudienceNode>(std::move(valueMap));
}

//Might not be necessary
std::unique_ptr<Visitor::RulesSetNode>
Visitor::Parser::visitRules(const ts::Node &node) {
    std::unique_ptr<BodyNode> rulesNode = 
        visitRulesBody(node.getChildByFieldName("body"));
    //TODO: have constructor of rules node taking in the node of the body
    return std::make_unique<RulesSetNode>();
}

//body is the field of rules; rule is child of body
std::unique_ptr<Visitor::BodyNode>
Visitor::Parser::visitRulesBody(const ts::Node &node) {
    ts::Cursor cursor = node.getCursor();
    std::vector<ts::Node> gameRules;
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();
            std::cout << child.getType() << " " << child.getSymbol() << std::endl;
            // if a rule is found, add to the body by getting the child
            //TODO: abstract the code in here so we can handle nested for loops
            //initial rule node handling (NEEDS TO BE ABSTRACTED)
            if (child.getSymbol() == 99) {
                std::cout << "rule found" << std::endl;
                std::cout << child.getChild(0).getType() << " " << child.getChild(0).getSymbol() << std::endl;
                gameRules.push_back(child);
                ts::Node ruleTypeNode = child.getChild(0);
                if (ruleTypeNode.getSymbol() == 100) {
                    std::cout << "for loop found" << std::endl;
                    ts::Cursor forLoopCursor = ruleTypeNode.getCursor();

                    if (forLoopCursor.gotoFirstChild()) { //Handles keyword, ident., in, expression, and body
                        do {
                            ts::Node forLoopKid = forLoopCursor.getCurrentNode();
                            std::cout << forLoopKid.getType() << " " << forLoopKid.getSymbol() << std::endl << std::endl;

                            if (child.getSymbol() == 85) { // for loop "identifier"

                            }

                            if (child.getSymbol() == 120) { //expression

                            }

                            //TODO: implement what to do with the rules body - discard, messages, and parallel for probably need to look like this
                            if (forLoopKid.getSymbol() == 131) {
                                std::cout << "for loop rules body" << std::endl;
                                ts::Cursor rulesCursor = forLoopKid.getCursor();
                                if (rulesCursor.gotoFirstChild()) {
                                    do {
                                        ts::Node rulesKid = rulesCursor.getCurrentNode();
                                        std::cout << rulesKid.getType() << " " << rulesKid.getSymbol() << std::endl << std::endl;
                                        if (rulesKid.getSymbol() == 99) {
                                            std::cout << "  " << rulesKid.getChild(0).getType() << " " << rulesKid.getChild(0).getSymbol() << std::endl << std::endl;

                                            ts::Node rulesType = rulesKid.getChild(0);

                                            // Discard
                                            if (rulesType.getSymbol() == 111) {
                                                ts::Cursor discardCursor = rulesType.getCursor();
                                                do {
                                                     ts::Node discardKid = discardCursor.getCurrentNode();
                                                    std::cout << "  " << discardKid.getChild(0).getType() << " " << discardKid.getChild(0).getSymbol() << std::endl << std::endl;
                                                    std::cout << "  " << discardKid.getChild(1).getType() << " " << discardKid.getChild(1).getSymbol() << std::endl << std::endl;
                                                } while (discardCursor.gotoNextSibling());
                                            }

                                            // Message
                                            else if (rulesType.getSymbol() == 118) {
                                                ts::Cursor messageCursor = rulesType.getCursor();
                                                do {
                                                     ts::Node messageKid = messageCursor.getCurrentNode();
                                                     std::cout << "  " << messageKid.getChild(0).getType() << " " << messageKid.getChild(0).getSymbol() << std::endl << std::endl;
                                                     std::cout << "  " << messageKid.getChild(1).getType() << " " << messageKid.getChild(1).getSymbol() << std::endl << std::endl;
                                                } while (messageCursor.gotoNextSibling());

                                            }

                                            // Parallel For
                                            else if (rulesType.getSymbol() == 102) {
                                                ts::Cursor parallelCursor = rulesType.getCursor();
                                                do {
                                                     ts::Node parallelKid = parallelCursor.getCurrentNode();
                                                     std::cout << "  " << parallelKid.getChild(0).getType() << " " << parallelKid.getChild(0).getSymbol() << std::endl << std::endl;
                                                     std::cout << "  " << parallelKid.getChild(1).getType() << " " << parallelKid.getChild(1).getSymbol() << std::endl << std::endl;
                                                     std::cout << "  " << parallelKid.getChild(2).getType() << " " << parallelKid.getChild(2).getSymbol() << std::endl << std::endl;
                                                     std::cout << "  " << parallelKid.getChild(3).getType() << " " << parallelKid.getChild(3).getSymbol() << std::endl << std::endl;
                                                     std::cout << "  " << parallelKid.getChild(4).getType() << " " << parallelKid.getChild(4).getSymbol() << std::endl << std::endl;
                                                } while (parallelCursor.gotoNextSibling());

                                            }
                                        }
                                    } while (rulesCursor.gotoNextSibling());
                                    std::cout << "end for loop body" << std::endl << std::endl;
                                }
                            }
                        } while (forLoopCursor.gotoNextSibling());
                    }
                }
            }
        } while (cursor.gotoNextSibling());
    }

    std::cout << "nodes going into a body node:" << std::endl;
    for (auto node : gameRules) {
        std::cout << node.getType() << " " << node.getSymbol() << std::endl;
    }
    return std::make_unique<BodyNode>(gameRules);
}

//might need to add more
std::unique_ptr<Visitor::GameRuleNode> 
Visitor::Parser::visitGameRule(const ts::Node &node) {
    size_t nodeSymbolNumber = node.getSymbol();
    switch (nodeSymbolNumber) {
        case 100:
            //TODO: create a ForLoopNode? or just some way of putting the data into something manageable
            break;
        case 111:
            //Todo: DiscardNode
            break;
        case 118:
            //todo: Message handling/visitation
            break;
        case 102:
            //todo: parallel for
            break;
    }
    return std::make_unique<GameRuleNode>();
}

std::unique_ptr<Visitor::SetupRuleNode>
Visitor::Parser::visitSetupRule(const ts::Node &node) {
    // How to: handle field
    std::unique_ptr<StringNode> kind =
        visitString(node.getChildByFieldName("kind"));
    std::unique_ptr<StringNode> prompt =
        visitString(node.getChildByFieldName("prompt"));

// How-To: handle optional types
    auto rangeChild = node.getChildByFieldName("range");
    auto choicesChild = node.getChildByFieldName("choices");
    auto defaultChild = node.getChildByFieldName("default");

    std::unique_ptr<RangeNode> range = rangeChild.isNull() ? nullptr : visitRange(rangeChild);
    auto choices = choicesChild.isNull() ? nullptr : visit(choicesChild);
    auto defaultValue = defaultChild.isNull() ? nullptr : visit(defaultChild);

    return std::make_unique<SetupRuleNode>(
            std::move(kind),
            std::move(prompt),
            std::move(range),
            std::move(choices),
            std::move(defaultValue));
}

std::unique_ptr<Visitor::ValueMapNode>
Visitor::Parser::visitValueMap(const ts::Node &node) {
    ts::Cursor cursor = node.getCursor();
    std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values;
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();
            std::cout << child.getType() << child.getSymbol() << std::endl << std::endl;
            if (child.getSymbol() == 127) {

                const ts::Node key = child.getChildByFieldName("key");
                std::unique_ptr<StringNode> identifier = visitString(key);

                const ts::Node value = child.getChildByFieldName("value");
                std::unique_ptr<StringNode> expression = visitString(value);

                values.insert(std::make_pair(std::move(identifier), std::move(expression)));

                // // temp loop used to print for debugging
                // for(auto it = values.cbegin(); it != values.cend(); ++it) {
                //     std::cout << it->second->value << "\n";
                // }
            }
        } while (cursor.gotoNextSibling());

       
    }
    return std::make_unique<ValueMapNode>(std::move(values));
}

std::unique_ptr<Visitor::StringNode>
Visitor::Parser::visitString(const ts::Node &node) {
    std::string value = std::string(node.getSourceRange(source));
    return std::make_unique<StringNode>(value);
}

std::unique_ptr<Visitor::BooleanNode>
Visitor::Parser::visitBoolean(const ts::Node &node) {
    std::string value = std::string(node.getSourceRange(source));
    return std::make_unique<BooleanNode>(value == "true");
}

std::unique_ptr<Visitor::RangeNode>
Visitor::Parser::visitRange(const ts::Node &node) {
    std::string first = std::string(node.getChild(1).getSourceRange(source));
    std::string second = std::string(node.getChild(3).getSourceRange(source));
    return std::make_unique<RangeNode>(std::pair<int, int>(std::stoi(first), std::stoi(second)));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::string filename = argv[1];
    std::string source = loadFile(filename);
    ts::Tree tree = parser.parseString(source);
    ts::Node node = tree.getRootNode();

    Visitor::Parser vParser{language, source};
    std::unique_ptr<Visitor::Node> vNode = vParser.visit(node);

    std::cout << std::endl << "==== Result ====" << std::endl;

    Visitor::Printer vPrinter;
    if (vNode != nullptr) {
        vNode->accept(vPrinter);
    }

    return 0;
}
