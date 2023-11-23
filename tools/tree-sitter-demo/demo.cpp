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

// namespace Visitor {

// struct Visitor;
// struct Node {
//     virtual void accept(Visitor &) const = 0;
// };

// struct ConfigurationNode;
// struct ConstantsNode;
// struct VariablesNode;
// struct PerPlayerNode;
// struct PerAudienceNode;

// struct RulesNode;
// struct BodyNode;

// struct SetupRuleNode;
// struct ValueMapNode;

// struct StringNode;
// struct BooleanNode;
// struct RangeNode;

// struct GameNode : public Node {
//     std::unique_ptr<ConfigurationNode> configuration;
//     std::unique_ptr<ConstantsNode> constants;
//     std::unique_ptr<VariablesNode> variables;
//     std::unique_ptr<PerPlayerNode> perPlayer;
//     std::unique_ptr<PerAudienceNode> perAudience;
//     std::unique_ptr<RulesNode> rules;

//     GameNode(
//         std::unique_ptr<ConfigurationNode> configuration,
//         std::unique_ptr<ConstantsNode> constants,
//         std::unique_ptr<VariablesNode> variables,
//         std::unique_ptr<PerPlayerNode> perPlayer,
//         std::unique_ptr<PerAudienceNode> perAudience,
//         std::unique_ptr<RulesNode> rules);
//     void accept(Visitor &visitor) const override;
// };

// struct ConfigurationNode : public Node {
//     std::unique_ptr<StringNode> name;
//     std::unique_ptr<BooleanNode> hasAudience;
//     std::unique_ptr<RangeNode> playerRange;
//     std::vector<std::unique_ptr<SetupRuleNode>> setupRules;

//     ConfigurationNode(
//         std::unique_ptr<StringNode> name,
//         std::unique_ptr<BooleanNode> hasAudience,
//         std::unique_ptr<RangeNode> playerRange);
//     void accept(Visitor &visitor) const override;
// };

// struct ConstantsNode : public Node {
//     std::unique_ptr<ValueMapNode> valueMap;
//     ConstantsNode(std::unique_ptr<ValueMapNode> valueMap);
//     void accept(Visitor &visitor) const override;
// };

// struct VariablesNode : public Node {
//     std::unique_ptr<ValueMapNode> valueMap;
//     VariablesNode(std::unique_ptr<ValueMapNode> valueMap);
//     void accept(Visitor &visitor) const override;
// };

// struct PerPlayerNode : public Node {
//     std::unique_ptr<ValueMapNode> valueMap;
//     PerPlayerNode(std::unique_ptr<ValueMapNode> valueMap);
//     void accept(Visitor &visitor) const override;
// };

// struct PerAudienceNode : public Node {
//     std::unique_ptr<ValueMapNode> valueMap;
//     PerAudienceNode(std::unique_ptr<ValueMapNode> valueMap);
//     void accept(Visitor &visitor) const override;
// };

// struct RulesNode : public Node {
//     std::unique_ptr<BodyNode> rulesBody;
//     RulesNode();
//     void accept(Visitor &visitor) const override;
// };

// struct BodyNode : public Node {
//     BodyNode(void);
//     void accept(Visitor &visitor) const override;
// };

// struct SetupRuleNode : public Node {
//     std::unique_ptr<StringNode> kind;
//     std::unique_ptr<StringNode> prompt;
//     std::unique_ptr<RangeNode> range;
//     std::unique_ptr<Node> choices;
//     std::unique_ptr<Node> defaultValue;

//     SetupRuleNode(std::unique_ptr<StringNode> kind,
//     std::unique_ptr<StringNode> prompt,
//     std::unique_ptr<RangeNode> range,
//     std::unique_ptr<Node> choices,
//     std::unique_ptr<Node> defaultValue);
//     void accept(Visitor &visitor) const override;
// };

// struct ValueMapNode : public Node {
//     std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values;
//     ValueMapNode(void);
//     ValueMapNode(std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values);
//     void accept(Visitor &visitor) const override;
// };


// struct StringNode : public Node {
//     std::string value;

//     StringNode(std::string value);
//     void accept(Visitor &visitor) const override;
// };

// struct BooleanNode : public Node {
//     bool value;

//     BooleanNode(bool value);
//     void accept(Visitor &visitor) const override;
// };

// struct RangeNode : public Node {
//     std::pair<int, int> value;

//     RangeNode(std::pair<int, int> value);
//     void accept(Visitor &visitor) const override;
// };

// struct Visitor {
//     virtual void visit(const Node &node) = 0;
//     virtual void visit(const GameNode &node) = 0;
//     virtual void visit(const ConfigurationNode &node) = 0;
//     virtual void visit(const ConstantsNode &node) = 0;
//     virtual void visit(const VariablesNode &node) = 0;
//     virtual void visit(const PerPlayerNode &node) = 0;
//     virtual void visit(const PerAudienceNode &node) = 0;
//     virtual void visit(const RulesNode &node) = 0;
//     virtual void visit(const BodyNode & node) = 0;
//     virtual void visit(const SetupRuleNode &node) = 0;
//     virtual void visit(const ValueMapNode &node) = 0;
//     virtual void visit(const StringNode &node) = 0;
//     virtual void visit(const BooleanNode &node) = 0;
//     virtual void visit(const RangeNode &node) = 0;
// };

// struct Printer : public Visitor {
//     void visit(const Node &node) override {
//         printDepth();
//         std::cout << "TODO" << std::endl;
//     }
//     void visit(const GameNode &node) override {
//         printDepth();
//         std::cout << "Game" << std::endl;
//         depth += 2;
//         node.configuration->accept(*this);
//         node.constants->accept(*this);
//         node.variables->accept(*this);
//         node.perPlayer->accept(*this);
//         node.perAudience->accept(*this);
//         node.rules->accept(*this);
//         depth -= 2;
//     }
//     void visit(const ConfigurationNode &node) override {
//         printDepth();
//         std::cout << "Configuration" << std::endl;
//         depth += 2;
//         node.name->accept(*this);
//         node.hasAudience->accept(*this);
//         node.playerRange->accept(*this);

//         for (size_t i = 0; i < node.setupRules.size(); i++) {
//             node.setupRules[i]->accept(*this);
//         }

//         depth -= 2;
//     }
    
//     void visitNodeWithValueMap(const std::string& nodeName, const ValueMapNode& valueMapNode) {
//         printDepth();
//         std::cout << nodeName << std::endl;
//         depth += 2;
//         valueMapNode.accept(*this);

//         for (const auto &pair : valueMapNode.values) {
//             pair.first->accept(*this); // identifier
//             pair.second->accept(*this); // expression
//         }

//         depth -= 2;
//     }

//     void visit(const ConstantsNode &node) override {
//          visitNodeWithValueMap("Constants", *node.valueMap);
//     }

//     void visit(const VariablesNode &node) override {
//          visitNodeWithValueMap("Variables", *node.valueMap);
//     }

//     void visit(const PerPlayerNode &node) override {
//         visitNodeWithValueMap("Per-Player", *node.valueMap);
//     }

//     void visit(const PerAudienceNode &node) override {
//         visitNodeWithValueMap("Per-Audience", *node.valueMap);
//     }

//     void visit(const RulesNode &node) override {
//         printDepth();
//         std::cout << "Rules" << std::endl;
//     }

//     void visit(const BodyNode & node) override {
//         printDepth();
//         std::cout << "RulesBody" << std::endl;
//     }

//     void visit(const SetupRuleNode &node) override {
//         printDepth();
//         std::cout << "Setup Rule" << std::endl;
//         depth += 2;
//         if (node.kind != nullptr) node.kind->accept(*this);
//         if (node.prompt != nullptr) node.prompt->accept(*this);
//         if (node.range != nullptr) node.range->accept(*this);
//         if (node.choices != nullptr) node.choices->accept(*this);
//         if (node.defaultValue != nullptr) node.defaultValue->accept(*this);
//         depth -= 2;
//     }
    
//     void visit(const ValueMapNode &node) override {
//         printDepth();
//         std::cout << "ValueMap" << std::endl;
//     }

//     void visit(const StringNode &node) override {
//         printDepth();
//         std::cout << "String " << node.value << std::endl;
//     }
//     void visit(const BooleanNode &node) override {
//         printDepth();
//         std::cout << "Boolean " << (node.value ? "true" : "false") << std::endl;
//     }
//     void visit(const RangeNode &node) override {
//         printDepth();
//         std::cout << "Range (" << node.value.first << ", " << node.value.second << ")" << std::endl;
//     }

//     size_t depth = 0;
//     void printDepth(void) {
//         for (size_t i = 0; i < depth; i++) {
//             std::cout << " ";
//         }
//     }
// };

// };

Visitor::GameNode::GameNode(
        std::unique_ptr<ConfigurationNode> configuration,
        std::unique_ptr<ConstantsNode> constants,
        std::unique_ptr<VariablesNode> variables,
        std::unique_ptr<PerPlayerNode> perPlayer,
        std::unique_ptr<PerAudienceNode> perAudience,
        std::unique_ptr<RulesNode> rules) :
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

Visitor::RulesNode::RulesNode() {

}

void Visitor::RulesNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::BodyNode::BodyNode() {

}

void Visitor::BodyNode::accept(Visitor &visitor) const {
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

Visitor::ValueMapNode::ValueMapNode(void) {

}

Visitor::ValueMapNode::ValueMapNode(std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values) : 
values(std::move(values)) {

}

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
    std::unique_ptr<RulesNode> rules =
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

std::unique_ptr<Visitor::RulesNode>
Visitor::Parser::visitRules(const ts::Node &node) {
    std::unique_ptr<BodyNode> rulesNode = 
        visitRulesBody(node.getChildByFieldName("body"));
    //TODO: have constructor of rules node taking in the node of the body
    return std::make_unique<RulesNode>();
}

//body is the field of rules; rule is child of body
std::unique_ptr<Visitor::BodyNode>
Visitor::Parser::visitRulesBody(const ts::Node &node) {
    ts::Cursor cursor = node.getCursor();
    //std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values;
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();
            std::cout << child.getType() << " " << child.getSymbol() << std::endl << std::endl;
            // if a rule is found, add to the body by getting the child
            if (child.getSymbol() == 99) {
                std::cout << child.getChild(0).getType() << " " << child.getChild(0).getSymbol() << std::endl << std::endl;
                //body->children.push_back(visit(child.getChild(0)));
                // std::unique_ptr<StringNode> identifier = visitString(key);

                // const ts::Node value = child.getChildByFieldName("value");
                // std::unique_ptr<StringNode> expression = visitString(value);

                // values.insert(std::make_pair(std::move(identifier), std::move(expression)));

                // // // temp
                // // for(auto it = values.cbegin(); it != values.cend(); ++it) {
                // //     std::cout << it->second->value << "\n";
                // // }
            }
        } while (cursor.gotoNextSibling());
    }
    return std::make_unique<BodyNode>();
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

                // // temp
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
