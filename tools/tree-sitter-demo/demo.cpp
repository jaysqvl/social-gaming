#include "demo.h"

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

//todo: move code from visit rules body into here
void Visitor::BodyNode::handleGameRuleNodes() {
    for (auto node : gameRuleNodes) {
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
    }
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

void Visitor::ForLoopNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::IdentifierNode::IdentifierNode(std::unique_ptr<StringNode> sn) 
    : identifierValue(std::move(sn)) {}

void Visitor::IdentifierNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::MessageNode::MessageNode(std::unique_ptr<StringNode> messageContent, std::unique_ptr<StringNode> recipients) 
    : messageContent(std::move(messageContent)), recipients(std::move(recipients)) {}

void Visitor::MessageNode::accept(Visitor &visitor) const {
    visitor.visit(*this);
}

Visitor::ExpressionNode::ExpressionNode(std::unique_ptr<StringNode> sn) 
    : expressionValue(std::move(sn)) {}

void Visitor::ExpressionNode::accept(Visitor &visitor) const {
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
    
    std::unique_ptr<MessageNode> messageNode = visitMessage(node);
    rulesNode->handleGameRuleNodes();
    //TODO: have constructor of rules node taking in the node of the body
    return std::make_unique<RulesSetNode>();
}

// Helper Functions for visitRulesBody
// void printNodes(const std::vector<ts::Node> &nodes) {
//     std::cout << "nodes going into a body node:" << std::endl;
//     for (const auto &node : nodes) {
//         std::cout << node.getType() << " " << node.getSymbol() << std::endl;
//     }
// }

//TODO: handle the expression node (child idx 1) and the qualified identifier (child idx 3) - amos
void processDiscard(const ts::Node &discardNode) {
    ts::Cursor discardCursor = discardNode.getCursor();
     do {
        ts::Node discardKid = discardCursor.getCurrentNode();
        std::cout << "  " << discardKid.getChild(0).getType() << " " << discardKid.getChild(0).getSymbol() << std::endl << std::endl;
        std::cout << "  " << discardKid.getChild(1).getType() << " " << discardKid.getChild(1).getSymbol() << std::endl << std::endl;
        std::cout << "  " << discardKid.getChild(2).getType() << " " << discardKid.getChild(2).getSymbol() << std::endl << std::endl;
        std::cout << "  " << discardKid.getChild(3).getType() << " " << discardKid.getChild(3).getSymbol() << std::endl << std::endl;
    } while (discardCursor.gotoNextSibling());
}

//TODO: handle the player_set node (child idx 1) and qualified identifier (child idx 2) -amos
// void processMessage(const ts::Node &messageNode) {
//     ts::Cursor messageCursor = messageNode.getCursor();
//     do {
//         ts::Node messageKid = messageCursor.getCurrentNode();
//         std::cout << "  " << messageKid.getChild(0).getType() << " " << messageKid.getChild(0).getSymbol() << std::endl << std::endl;
//         std::cout << "  " << messageKid.getChild(1).getType() << " " << messageKid.getChild(1).getSymbol() << std::endl << std::endl;
//         std::cout << "  " << messageKid.getChild(2).getType() << " " << messageKid.getChild(2).getSymbol() << std::endl << std::endl;
//     } while (messageCursor.gotoNextSibling());
// }

void processMessage(const ts::Node &messageNode) {
    ts::Cursor messageCursor = messageNode.getCursor();
    std::unique_ptr<Visitor::StringNode> messageContentNode;

    if (messageCursor.gotoFirstChild()) {
        do {
            ts::Node messageKid = messageCursor.getCurrentNode();
            std::cout << "  " << messageKid.getType() << " " << messageKid.getSymbol() << std::endl << std::endl;
            
            //process message identifier
            if(messageKid.getSymbol() == 118) {

            }
            
            //players to message
            else if(messageKid.getSymbol() == 130) {
                
            }
            
            //the actual message
            else if(messageKid.getSymbol() == 120){

            }

        } while (messageCursor.gotoNextSibling());
    }
}

//TODO: handle parallel for (if needed - child idx 0), identifier (child idx 1), expression (3) and body (4) - jay
void processParallelFor(const ts::Node &parallelNode) {
    ts::Cursor parallelCursor = parallelNode.getCursor();
    do {
        ts::Node parallelKid = parallelCursor.getCurrentNode();
        std::cout << "  " << parallelKid.getChild(0).getType() << " " << parallelKid.getChild(0).getSymbol() << std::endl << std::endl;
        std::cout << "  " << parallelKid.getChild(1).getType() << " " << parallelKid.getChild(1).getSymbol() << std::endl << std::endl;
        std::cout << "  " << parallelKid.getChild(2).getType() << " " << parallelKid.getChild(2).getSymbol() << std::endl << std::endl;
        std::cout << "  " << parallelKid.getChild(3).getType() << " " << parallelKid.getChild(3).getSymbol() << std::endl << std::endl;
        std::cout << "  " << parallelKid.getChild(4).getType() << " " << parallelKid.getChild(4).getSymbol() << std::endl << std::endl;
    } while (parallelCursor.gotoNextSibling());
}

void processMatch(const ts::Node &matchNode) {
    ts::Cursor matchCursor = matchNode.getCursor();
    if (matchCursor.gotoFirstChild()) {
        do {
            ts::Node matchKid = matchCursor.getCurrentNode();
            std::cout << "  " << matchKid.getType() << " " << matchKid.getSymbol() << std::endl << std::endl;
            if (matchKid.getSymbol() == 120) { // expression
                //todo: try implementing something similar to the visitExpression function in Parser class
            } else if (matchKid.getSymbol() == 105) { //match entry (can be multiple)
                processMatchEntry(matchKid);
            }
        } while (matchCursor.gotoNextSibling());
    }
}
 //helper function of the helper to get all the match entries
void processMatchEntry(const ts::Node &matchEntryNode) {
    ts::Cursor matchEntryCursor = matchEntryNode.getCursor();
    if (matchEntryCursor.gotoFirstChild()) {
        do {
            ts::Node matchEntryKid = matchEntryCursor.getCurrentNode();
            std::cout << "    " << matchEntryKid.getType() << " " << matchEntryKid.getSymbol() << std::endl << std::endl;
            if (matchEntryKid.getSymbol() == 120) { // expression
                //todo: try implementing something similar to the visitExpression function in Parser class
            } else if (matchEntryKid.getSymbol() == 131) { // body
                std::cout << "START MATCH ENTRY BODY" << std::endl;
                processRulesBody(matchEntryKid);
                std::cout << "END MATCH ENTRY BODY" << std::endl;
            }
        } while (matchEntryCursor.gotoNextSibling());
    }
}

void processRuleBodyType(const ts::Node &ruleBodyNode) {
    ts::Node rulesType = ruleBodyNode.getChild(0);

    if (rulesType.getSymbol() == 111) { // Discard
        processDiscard(rulesType);
    } else if (rulesType.getSymbol() == 118) { // Message
        processMessage(rulesType);
    } else if (rulesType.getSymbol() == 102) { // Parallel For
        processParallelFor(rulesType);
    } else if (rulesType.getSymbol() == 100) { // Regular For
        processForLoop(rulesType);
    } else if (rulesType.getSymbol() == 119) { // Scores
        //TODO: make a func to process scores - david
    } else if (rulesType.getSymbol() == 104) { // Match
        processMatch(rulesType);
    } else if (rulesType.getSymbol() == 106) { // Extend
        //TODO
    } else if (rulesType.getSymbol() == 112) { // Assignment
        //TODO
    }
}

void processRulesBody(const ts::Node &rulesBodyNode) {
    std::cout << "loop rules body" << std::endl;
    ts::Cursor rulesCursor = rulesBodyNode.getCursor();
    if (rulesCursor.gotoFirstChild()) {
        do {
            ts::Node rulesKid = rulesCursor.getCurrentNode();
            std::cout << rulesKid.getType() << " " << rulesKid.getSymbol() << std::endl << std::endl;
            if (rulesKid.getSymbol() == 99) {
                std::cout << "  " << rulesKid.getChild(0).getType() << " " << rulesKid.getChild(0).getSymbol() << std::endl << std::endl;
                processRuleBodyType(rulesKid);
            }
        } while (rulesCursor.gotoNextSibling());
        std::cout << "end for loop body" << std::endl << std::endl;
    }
}

void processForLoop(const ts::Node &forLoopNode) {
    std::cout << "for loop found" << std::endl;
    ts::Cursor forLoopCursor = forLoopNode.getCursor();

    if (forLoopCursor.gotoFirstChild()) { //Handles keyword, ident., in, expression, and body
    do {
        ts::Node forLoopKid = forLoopCursor.getCurrentNode();
        std::cout << forLoopKid.getType() << " " << forLoopKid.getSymbol() << std::endl << std::endl;
        if (forLoopKid.getSymbol() == 131) {
            processRulesBody(forLoopKid);
        }
        } while (forLoopCursor.gotoNextSibling());
    }
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
            if (child.getSymbol() == 99) {
                std::cout << "rule found" << std::endl;
                std::cout << child.getChild(0).getType() << " " << child.getChild(0).getSymbol() << std::endl;
                gameRules.push_back(child);
                ts::Node ruleTypeNode = child.getChild(0);
                if (ruleTypeNode.getSymbol() == 100) {
                    ts::Node identifierNode = ruleTypeNode.getChild(1);
                    ts::Node expressionNode = ruleTypeNode.getChild(3);
                    processForLoop(ruleTypeNode);
                    visitIdentifier(identifierNode);
                    visitExpression(expressionNode);
                }

                else if(ruleTypeNode.getSymbol() == 118) {
                    std::cout << ruleTypeNode.getType() << "poop " << ruleTypeNode.getSymbol() << std::endl;
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

std::unique_ptr<Visitor::GameRuleNode> 
Visitor::Parser::visitGameRule(const ts::Node &node) {
    //TODO: add more.
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

std::unique_ptr<Visitor::IdentifierNode> 
Visitor::Parser::visitIdentifier(const ts::Node &node) {
    std::unique_ptr<StringNode> newStringNode = visitString(node);
    std::cout << "loop identifier: " << newStringNode->value << std::endl;
    return std::make_unique<IdentifierNode>(std::move(newStringNode));
}

std::unique_ptr<Visitor::MessageNode> 
Visitor::Parser::visitMessage(const ts::Node &node) {
    ts::Node messageContent = node;
    ts::Node recipients = node;
    ts::Cursor cursor = node.getCursor();

    if (cursor.gotoFirstChild()) {
        do {
            ts::Node child = cursor.getCurrentNode();    
            std::cout << child.getType() << "asd " << child.getSymbol() << std::endl << std::endl;

            if(child.getSymbol() == 118) {
                std::cout << "msg found" << std::endl;
                messageContent = child;
            }

            else if(child.getSymbol() == 130) {
                std::cout << "player set found" << std::endl;
                recipients = child;
            }
        } while (cursor.gotoNextSibling());
    }

    std::unique_ptr<StringNode> messageContentNode = visitString(messageContent);
    std::cout << "message content: " << messageContentNode->value << std::endl;

    std::unique_ptr<StringNode> recipientsNode = visitString(recipients);
    std::cout << "recipients: " << recipientsNode->value << std::endl;

    return std::make_unique<MessageNode>(std::move(messageContentNode), std::move(recipientsNode));
}

std::unique_ptr<Visitor::ExpressionNode> 
Visitor::Parser::visitExpression(const ts::Node &node) {
    ts::Cursor cursor = node.getCursor();
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node expressionKid = cursor.getCurrentNode();    
            std::cout << expressionKid.getType() << " " << expressionKid.getSymbol() << std::endl << std::endl;
            //TODO: handle "builtin" and "argument_list" nodes. the entire expression is already in a string form.
            if (expressionKid.getSymbol() == 121) { //builtin

            }
            if (expressionKid.getSymbol() == 122) { //argument_list

            }
        } while (cursor.gotoNextSibling());
    }
    std::unique_ptr<StringNode> newStringNode = visitString(node);
    std::cout << "loop expression: " << newStringNode->value << std::endl;
    return std::make_unique<ExpressionNode>(std::move(newStringNode));
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
