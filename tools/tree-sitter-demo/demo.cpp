#include <cpp-tree-sitter.h>

#include <iostream>
#include <fstream>
#include <string>

#include <map>
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

namespace Visitor {

struct Visitor;
struct Node {
    virtual void accept(Visitor &) = 0;
};

struct ConfigurationNode;
struct ConstantsNode;
struct VariablesNode;
struct PerPlayerNode;
struct PerAudienceNode;
struct RulesNode;

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
    void accept(Visitor &visitor) override;
};

struct ConfigurationNode : public Node {
    std::unique_ptr<StringNode> name;
    std::unique_ptr<BooleanNode> hasAudience;
    std::unique_ptr<RangeNode> playerRange;

    ConfigurationNode(
        std::unique_ptr<StringNode> name,
        std::unique_ptr<BooleanNode> hasAudience,
        std::unique_ptr<RangeNode> playerRange);
    void accept(Visitor &visitor) override;
};

struct ConstantsNode : public Node {
    ConstantsNode();
    void accept(Visitor &visitor) override;
};

struct VariablesNode : public Node {
    VariablesNode();
    void accept(Visitor &visitor) override;
};

struct PerPlayerNode : public Node {
    PerPlayerNode();
    void accept(Visitor &visitor) override;
};

struct PerAudienceNode : public Node {
    PerAudienceNode();
    void accept(Visitor &visitor) override;
};

struct RulesNode : public Node {
    RulesNode();
    void accept(Visitor &visitor) override;
};

struct StringNode : public Node {
    std::string value;

    StringNode(std::string value);
    void accept(Visitor &visitor) override;
};

struct BooleanNode : public Node {
    bool value;

    BooleanNode(bool value);
    void accept(Visitor &visitor) override;
};

struct RangeNode : public Node {
    std::pair<int, int> value;

    RangeNode(std::pair<int, int> value);
    void accept(Visitor &visitor) override;
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
    virtual void visit(const StringNode &node) = 0;
    virtual void visit(const BooleanNode &node) = 0;
    virtual void visit(const RangeNode &node) = 0;
};

struct Printer : public Visitor {
    void visit(const Node &node) override {
        std::cout << "TODO" << std::endl;
    }
    void visit(const GameNode &node) override {
        std::cout << "Game" << std::endl;
        node.configuration->accept(*this);
        node.constants->accept(*this);
        node.variables->accept(*this);
        node.perPlayer->accept(*this);
        node.perAudience->accept(*this);
        node.rules->accept(*this);
    }
    void visit(const ConfigurationNode &node) override {
        std::cout << "Configuration" << std::endl;
        node.name->accept(*this);
        node.hasAudience->accept(*this);
        node.playerRange->accept(*this);
    }
    void visit(const ConstantsNode &node) override {
        std::cout << "Constants" << std::endl;
    }
    void visit(const VariablesNode &node) override {
        std::cout << "Variables" << std::endl;
    }
    void visit(const PerPlayerNode &node) override {
        std::cout << "Per-Player" << std::endl;
    }
    void visit(const PerAudienceNode &node) override {
        std::cout << "Per-Audience" << std::endl;
    }
    void visit(const RulesNode &node) override {
        std::cout << "Rules" << std::endl;
    }
    void visit(const StringNode &node) override {
        std::cout << "String " << node.value << std::endl;
    }
    void visit(const BooleanNode &node) override {
        std::cout << "Boolean " << (node.value ? "true" : "false") << std::endl;
    }
    void visit(const RangeNode &node) override {
        std::cout << "Range (" << node.value.first << ", " << node.value.second << ")" << std::endl;
    }
};

};

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

void Visitor::GameNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::ConfigurationNode::ConfigurationNode(
        std::unique_ptr<StringNode> name,
        std::unique_ptr<BooleanNode> hasAudience,
        std::unique_ptr<RangeNode> playerRange) :
    name{std::move(name)},
    hasAudience{std::move(hasAudience)},
    playerRange{std::move(playerRange)} {}

void Visitor::ConfigurationNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::ConstantsNode::ConstantsNode() {

}

void Visitor::ConstantsNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::VariablesNode::VariablesNode() {

}

void Visitor::VariablesNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::PerPlayerNode::PerPlayerNode() {

}

void Visitor::PerPlayerNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::PerAudienceNode::PerAudienceNode() {

}

void Visitor::PerAudienceNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

Visitor::RulesNode::RulesNode() {

}

void Visitor::RulesNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


Visitor::StringNode::StringNode(std::string value) :
    value{value} {}

void Visitor::StringNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


Visitor::BooleanNode::BooleanNode(bool value) :
    value{value} {}

void Visitor::BooleanNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


Visitor::RangeNode::RangeNode(std::pair<int, int> value) :
    value{value} {}

void Visitor::RangeNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

namespace Visitor {

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
    std::unique_ptr<StringNode> visitString(const ts::Node &);
    std::unique_ptr<BooleanNode> visitBoolean(const ts::Node &);
    std::unique_ptr<RangeNode> visitRange(const ts::Node &);
};

};

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

    ts::Cursor cursor = node.getCursor();
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();
            if (child.getSymbol() == 92) {
                visit(child);
            }
        } while (cursor.gotoNextSibling());
    }

    return std::make_unique<ConfigurationNode>(
            std::move(name),
            std::move(hasAudience),
            std::move(playerRange));
}


std::unique_ptr<Visitor::ConstantsNode>
Visitor::Parser::visitConstants(const ts::Node &node) {
    return std::make_unique<ConstantsNode>();
}

std::unique_ptr<Visitor::VariablesNode>
Visitor::Parser::visitVariables(const ts::Node &node) {
    return std::make_unique<VariablesNode>();
}

std::unique_ptr<Visitor::PerPlayerNode>
Visitor::Parser::visitPerPlayer(const ts::Node &node) {
    return std::make_unique<PerPlayerNode>();
}

std::unique_ptr<Visitor::PerAudienceNode>
Visitor::Parser::visitPerAudience(const ts::Node &node) {
    return std::make_unique<PerAudienceNode>();
}

std::unique_ptr<Visitor::RulesNode>
Visitor::Parser::visitRules(const ts::Node &node) {
    return std::make_unique<RulesNode>();
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
