#include <cpp-tree-sitter.h>

#include <iostream>
#include <fstream>
#include <string>

#include <map>
#include <vector>
#include <memory>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

std::string loadFile(const std::string &filename);

namespace Visitor {

struct Visitor;
struct Node {
    virtual void accept(Visitor &) const = 0;
};

struct ConfigurationNode;
struct ConstantsNode;
struct VariablesNode;
struct PerPlayerNode;
struct PerAudienceNode;

struct RulesNode;
struct BodyNode;

struct SetupRuleNode;
struct ValueMapNode;

struct StringNode;
struct BooleanNode;
struct RangeNode;

struct GameNode : public Node {
    std::unique_ptr<ConfigurationNode> configuration;
    std::unique_ptr<ConstantsNode> constants;
    std::unique_ptr<VariablesNode> variables;
    std::unique_ptr<PerPlayerNode> perPlayer;
    std::unique_ptr<PerAudienceNode> perAudience;
    std::unique_ptr<RulesNode> rules;

    GameNode(
        std::unique_ptr<ConfigurationNode> configuration,
        std::unique_ptr<ConstantsNode> constants,
        std::unique_ptr<VariablesNode> variables,
        std::unique_ptr<PerPlayerNode> perPlayer,
        std::unique_ptr<PerAudienceNode> perAudience,
        std::unique_ptr<RulesNode> rules);
    void accept(Visitor &visitor) const override;
};

struct ConfigurationNode : public Node {
    std::unique_ptr<StringNode> name;
    std::unique_ptr<BooleanNode> hasAudience;
    std::unique_ptr<RangeNode> playerRange;
    std::vector<std::unique_ptr<SetupRuleNode>> setupRules;

    ConfigurationNode(
        std::unique_ptr<StringNode> name,
        std::unique_ptr<BooleanNode> hasAudience,
        std::unique_ptr<RangeNode> playerRange);
    void accept(Visitor &visitor) const override;
};

struct ConstantsNode : public Node {
    std::unique_ptr<ValueMapNode> valueMap;
    ConstantsNode(std::unique_ptr<ValueMapNode> valueMap);
    void accept(Visitor &visitor) const override;
};

struct VariablesNode : public Node {
    std::unique_ptr<ValueMapNode> valueMap;
    VariablesNode(std::unique_ptr<ValueMapNode> valueMap);
    void accept(Visitor &visitor) const override;
};

struct PerPlayerNode : public Node {
    std::unique_ptr<ValueMapNode> valueMap;
    PerPlayerNode(std::unique_ptr<ValueMapNode> valueMap);
    void accept(Visitor &visitor) const override;
};

struct PerAudienceNode : public Node {
    std::unique_ptr<ValueMapNode> valueMap;
    PerAudienceNode(std::unique_ptr<ValueMapNode> valueMap);
    void accept(Visitor &visitor) const override;
};

struct RulesNode : public Node {
    std::unique_ptr<BodyNode> rulesBody;
    RulesNode();
    void accept(Visitor &visitor) const override;
};

struct BodyNode : public Node {
    BodyNode(void);
    void accept(Visitor &visitor) const override;
};

struct SetupRuleNode : public Node {
    std::unique_ptr<StringNode> kind;
    std::unique_ptr<StringNode> prompt;
    std::unique_ptr<RangeNode> range;
    std::unique_ptr<Node> choices;
    std::unique_ptr<Node> defaultValue;

    SetupRuleNode(std::unique_ptr<StringNode> kind,
    std::unique_ptr<StringNode> prompt,
    std::unique_ptr<RangeNode> range,
    std::unique_ptr<Node> choices,
    std::unique_ptr<Node> defaultValue);
    void accept(Visitor &visitor) const override;
};

struct ValueMapNode : public Node {
    std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values;
    ValueMapNode(void);
    ValueMapNode(std::map<std::unique_ptr<StringNode>, std::unique_ptr<StringNode>> values);
    void accept(Visitor &visitor) const override;
};


struct StringNode : public Node {
    std::string value;

    StringNode(std::string value);
    void accept(Visitor &visitor) const override;
};

struct BooleanNode : public Node {
    bool value;

    BooleanNode(bool value);
    void accept(Visitor &visitor) const override;
};

struct RangeNode : public Node {
    std::pair<int, int> value;

    RangeNode(std::pair<int, int> value);
    void accept(Visitor &visitor) const override;
};

struct Visitor {
    virtual void visit(const Node &node) = 0;
    virtual void visit(const GameNode &node) = 0;
    virtual void visit(const ConfigurationNode &node) = 0;
    virtual void visit(const ConstantsNode &node) = 0;
    virtual void visit(const VariablesNode &node) = 0;
    virtual void visit(const PerPlayerNode &node) = 0;
    virtual void visit(const PerAudienceNode &node) = 0;
    virtual void visit(const RulesNode &node) = 0;
    virtual void visit(const BodyNode & node) = 0;
    virtual void visit(const SetupRuleNode &node) = 0;
    virtual void visit(const ValueMapNode &node) = 0;
    virtual void visit(const StringNode &node) = 0;
    virtual void visit(const BooleanNode &node) = 0;
    virtual void visit(const RangeNode &node) = 0;
};

struct Printer : public Visitor {
    void visit(const Node &node) override {
        printDepth();
        std::cout << "TODO" << std::endl;
    }
    void visit(const GameNode &node) override {
        printDepth();
        std::cout << "Game" << std::endl;
        depth += 2;
        node.configuration->accept(*this);
        node.constants->accept(*this);
        node.variables->accept(*this);
        node.perPlayer->accept(*this);
        node.perAudience->accept(*this);
        node.rules->accept(*this);
        depth -= 2;
    }
    void visit(const ConfigurationNode &node) override {
        printDepth();
        std::cout << "Configuration" << std::endl;
        depth += 2;
        node.name->accept(*this);
        node.hasAudience->accept(*this);
        node.playerRange->accept(*this);

        for (size_t i = 0; i < node.setupRules.size(); i++) {
            node.setupRules[i]->accept(*this);
        }

        depth -= 2;
    }
    
    void visitNodeWithValueMap(const std::string& nodeName, const ValueMapNode& valueMapNode) {
        printDepth();
        std::cout << nodeName << std::endl;
        depth += 2;
        valueMapNode.accept(*this);

        for (const auto &pair : valueMapNode.values) {
            pair.first->accept(*this); // identifier
            pair.second->accept(*this); // expression
        }

        depth -= 2;
    }

    void visit(const ConstantsNode &node) override {
         visitNodeWithValueMap("Constants", *node.valueMap);
    }

    void visit(const VariablesNode &node) override {
         visitNodeWithValueMap("Variables", *node.valueMap);
    }

    void visit(const PerPlayerNode &node) override {
        visitNodeWithValueMap("Per-Player", *node.valueMap);
    }

    void visit(const PerAudienceNode &node) override {
        visitNodeWithValueMap("Per-Audience", *node.valueMap);
    }

    void visit(const RulesNode &node) override {
        printDepth();
        std::cout << "Rules" << std::endl;
    }

    void visit(const BodyNode & node) override {
        printDepth();
        std::cout << "RulesBody" << std::endl;
    }

    void visit(const SetupRuleNode &node) override {
        printDepth();
        std::cout << "Setup Rule" << std::endl;
        depth += 2;
        if (node.kind != nullptr) node.kind->accept(*this);
        if (node.prompt != nullptr) node.prompt->accept(*this);
        if (node.range != nullptr) node.range->accept(*this);
        if (node.choices != nullptr) node.choices->accept(*this);
        if (node.defaultValue != nullptr) node.defaultValue->accept(*this);
        depth -= 2;
    }
    
    void visit(const ValueMapNode &node) override {
        printDepth();
        std::cout << "ValueMap" << std::endl;
    }

    void visit(const StringNode &node) override {
        printDepth();
        std::cout << "String " << node.value << std::endl;
    }
    void visit(const BooleanNode &node) override {
        printDepth();
        std::cout << "Boolean " << (node.value ? "true" : "false") << std::endl;
    }
    void visit(const RangeNode &node) override {
        printDepth();
        std::cout << "Range (" << node.value.first << ", " << node.value.second << ")" << std::endl;
    }

    size_t depth = 0;
    void printDepth(void) {
        for (size_t i = 0; i < depth; i++) {
            std::cout << " ";
        }
    }
};

class Parser {
public:
    using Command = std::unique_ptr<Node> (Parser::*)(const ts::Node &);
public:
    Parser(const ts::Language &language, const std::string &source);
    std::unique_ptr<Node> visit(const ts::Node &node);
private:
    const ts::Language &language;
    const std::string &source;
    std::map<ts::Symbol, Command> visitMap;
private:
    std::unique_ptr<Node> visitChildren(const ts::Node &);

    std::unique_ptr<GameNode> visitGame(const ts::Node &);
    std::unique_ptr<ConfigurationNode> visitConfiguration(const ts::Node &);
    std::unique_ptr<ConstantsNode> visitConstants(const ts::Node &);
    std::unique_ptr<VariablesNode> visitVariables(const ts::Node &);
    std::unique_ptr<PerPlayerNode> visitPerPlayer(const ts::Node &);
    std::unique_ptr<PerAudienceNode> visitPerAudience(const ts::Node &);
    std::unique_ptr<RulesNode> visitRules(const ts::Node &);
    std::unique_ptr<BodyNode> visitRulesBody(const ts::Node &);
    std::unique_ptr<SetupRuleNode> visitSetupRule(const ts::Node &);
    std::unique_ptr<ValueMapNode> visitValueMap(const ts::Node &);
    std::unique_ptr<StringNode> visitString(const ts::Node &);
    std::unique_ptr<BooleanNode> visitBoolean(const ts::Node &);
    std::unique_ptr<RangeNode> visitRange(const ts::Node &);
};

};