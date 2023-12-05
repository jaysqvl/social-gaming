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

Visitor::RulesSetNode::RulesSetNode(std::unique_ptr<BodyNode> bodyNode) 
    : body(std::move(bodyNode)) {}

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
                // Todo: create a ForLoopNode? or just some way of putting the data into something manageable
                break;
            case 111:
                // Todo: DiscardNode
                break;
            case 118:
                // Todo: Message handling/visitation
                break;
            case 102:
                // Todo: parallel for
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

Visitor::DiscardNode::DiscardNode(std::unique_ptr<StringNode> discardContent, std::unique_ptr<StringNode> target) 
    : discardContent(std::move(discardContent)), target(std::move(target)) {}

void Visitor::DiscardNode::accept(Visitor &visitor) const {
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
    std::unique_ptr<BodyNode> rulesBodyNode = 
        visitRulesBody(node.getChildByFieldName("body"));
    
    std::unique_ptr<MessageNode> messageNode = visitMessage(node);

    std::cout << "Message recipient: " << messageNode->recipients->value << "\n";
    std::cout << "Message: " << messageNode->messageContent->value << "\n";

    std::unique_ptr<DiscardNode> discardNode = visitDiscard(node);
    std::cout << "Discard target: " << discardNode->target->value << "\n";
    std::cout << "Discarding: " << discardNode->discardContent->value << "\n";

    rulesBodyNode->handleGameRuleNodes();
    //TODO: have constructor of rules node taking in the node of the body -- Complete
    //Todo - Store and use the message and discard nodes in some way
    std::unique_ptr<RulesSetNode> ruleSetNode =  std::make_unique<RulesSetNode>(std::move(rulesBodyNode));
    return ruleSetNode;
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

            if (messageKid.getSymbol() == 130) { // Sym player set
                
            } else if (messageKid.getSymbol() == 120){ // Sym expression

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

void processScore(const ts::Node &scoreNode) {
    ts::Cursor scoreCursor = scoreNode.getCursor();
    if (scoreCursor.gotoFirstChild()) {
        do {
            ts::Node scoreKid = scoreCursor.getCurrentNode();
            std::cout << "  " << scoreKid.getType() << " " << scoreKid.getSymbol() << std::endl << std::endl;
            if (scoreKid.getSymbol() == 126) { // List literals
                // TODO: Handle 'keys' which is of type list_literal
            }
        } while (scoreCursor.gotoNextSibling());
    }
}

void processExtend(const ts::Node &extendNode) {
    ts::Cursor extendCursor = extendNode.getCursor();
    if (extendCursor.gotoFirstChild()) {
        do {
            ts::Node extendKid = extendCursor.getCurrentNode();
            std::cout << "  " << extendKid.getType() << " " << extendKid.getSymbol() << std::endl << std::endl;
            if (extendKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            } else if (extendKid.getSymbol() == 120) { // expression
                // TODO: Handle expression
            }
        } while (extendCursor.gotoNextSibling());
    }
}

void processAssignment(const ts::Node &assignmentNode) {
    // stub
    ts::Cursor assignmentCursor = assignmentNode.getCursor();
    if (assignmentCursor.gotoFirstChild()) {
        do {
            ts::Node assignmentKid = assignmentCursor.getCurrentNode();
            std::cout << "  " << assignmentKid.getType() << " " << assignmentKid.getSymbol() << std::endl << std::endl;
            if (assignmentKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            } else if (assignmentKid.getSymbol() == 120) { // expression
                // TODO: Handle expression
            }
        } while (assignmentCursor.gotoNextSibling());
    }
}

void processSort(const ts::Node &sortNode) {
    ts::Cursor sortCursor = sortNode.getCursor();
    if (sortCursor.gotoFirstChild()) {
        do {
            ts::Node sortKid = sortCursor.getCurrentNode();
            std::cout << "  " << sortKid.getType() << " " << sortKid.getSymbol() << std::endl << std::endl;
            if (sortKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            } else {
                if (sortKid.getSymbol() == 120) { // OPTIONAL: Expression
                    // TODO: Handle expression
                }
            }
        } while (sortCursor.gotoNextSibling());
    }
}

void processDeal(const ts::Node &dealNode) {
    ts::Cursor dealCursor = dealNode.getCursor();
    if (dealCursor.gotoFirstChild()) {
        do {
            ts::Node dealKid = dealCursor.getCurrentNode();
            std::cout << "  " << dealKid.getType() << " " << dealKid.getSymbol() << std::endl << std::endl;
            if (dealKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            } else if (dealKid.getSymbol() == 130) { // player_set
                // TODO: Player Set
            } else if (dealKid.getSymbol() == 120) {
                // TODO: Handle expression
            }
        } while (dealCursor.gotoNextSibling());
    }
}

void processTimer(const ts::Node &timerNode) {
    ts::Cursor timerCursor = timerNode.getCursor();
    if (timerCursor.gotoFirstChild()) {
        do {
            ts::Node timerKid = timerCursor.getCurrentNode();
            std::cout << "  " << timerKid.getType() << " " << timerKid.getSymbol() << std::endl << std::endl;
            if (timerKid.getSymbol() == 120) { // expression
                // TODO: Handle qualified identifier
            } else if (timerKid.getSymbol() == 131) { // sym body
                processRulesBody(timerKid);
            } else {    // START OPTIONAL KIDS:
                if (timerKid.getSymbol() == 114) { // OPTIONAL: Choice field
                    // TODO: HANDLE 'at most' and 'exactly' choices (see grammar.js line 235)
                } else if (timerKid.getSymbol() == 129) { // OPTIONAL: Qualified identifier
                    
                }
            }
        } while (timerCursor.gotoNextSibling());
    }
}

void processReverse(const ts::Node &reverseNode) {
    ts::Cursor reverseCursor = reverseNode.getCursor();
    if (reverseCursor.gotoFirstChild()) {
        do {
            ts::Node reverseKid = reverseCursor.getCurrentNode();
            std::cout << "  " << reverseKid.getType() << " " << reverseKid.getSymbol() << std::endl << std::endl;
            if (reverseKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            }
        } while (reverseCursor.gotoNextSibling());
    }
}

void processShuffle(const ts::Node &shuffleNode) {
    ts::Cursor shuffleCursor = shuffleNode.getCursor();
    if (shuffleCursor.gotoFirstChild()) {
        do {
            ts::Node shuffleKid = shuffleCursor.getCurrentNode();
            std::cout << "  " << shuffleKid.getType() << " " << shuffleKid.getSymbol() << std::endl << std::endl;
            if (shuffleKid.getSymbol() == 129) { // qualified identifier
                // TODO: Handle qualified identifier
            }
        } while (shuffleCursor.gotoNextSibling());
    }
}

void processRuleBodyType(const ts::Node &ruleBodyNode) {
    ts::Node rulesType = ruleBodyNode.getChild(0);
    auto rulesTypeSymbol = rulesType.getSymbol();

    switch (rulesTypeSymbol) {
        case 111:
            processDiscard(rulesType);
            break;
        case 118:
            processMessage(rulesType);
            break;
        case 102:
            processParallelFor(rulesType);
            break;
        case 100:
            processForLoop(rulesType);
            break;
        case 119:
            processScore(rulesType);
            break;
        case 104:
            processMatch(rulesType);
            break;
        case 106:
            processExtend(rulesType);
            break;
        case 112:
            processAssignment(rulesType);
            break;
        case 109:
            processSort(rulesType);
            break;
        case 110:
            processDeal(rulesType);
            break;
        case 113:
            processTimer(rulesType);
            break;
        case 107:
            processReverse(rulesType);
            break;
        case 108:
            processShuffle(rulesType);
            break;
        default:
            break;
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

// recursively traverse through the rule tree to find message statements
std::unique_ptr<Visitor::StringNode> 
Visitor::Parser::findNode(const ts::Node &node, std::string keyword) {
    for(int i = 0; i < node.getNumChildren(); i++) {
        std::string currWord = std::string(node.getChild(i).getSourceRange(source));

        // Check if currWord is not empty before extracting the first word
        if (!currWord.empty()) {
            std::string firstWord;
            std::istringstream(currWord) >> firstWord;

            if(firstWord == keyword) {
                // std::cout << "found message! " << currWord << '\n';
                std::unique_ptr<StringNode> stringNode = std::make_unique<StringNode>(std::move(std::string(currWord)));
                return stringNode;
            }
        }

        auto result = findNode(node.getChild(i), keyword);
        if(result) {
            return result;
        }
    }
    return nullptr;
}

std::unique_ptr<Visitor::MessageNode> 
Visitor::Parser::visitMessage(const ts::Node &node) {
    std::unique_ptr<StringNode> messageContentNode;
    std::unique_ptr<StringNode> recipentsNode;
    ts::Cursor cursor = node.getCursor();

    std::unique_ptr<StringNode> messageNode = findNode(node, "message");
    if (messageNode) {
        size_t messagePos = messageNode->value.find("message");

        messagePos += 7; // length of "message"
        size_t start = messageNode->value.find_first_not_of(" ", messagePos);
        size_t end = messageNode->value.find(' ', start);

        if (start != std::string::npos && end != std::string::npos) {
            std::string recipents = messageNode->value.substr(start, end - start);
            recipentsNode = std::make_unique<StringNode>(std::move(std::string(recipents)));
        } 
        
        else {
            std::cerr << "Error: recipients not found in message node." << std::endl;
            return nullptr;
        }

        size_t quoteStart = messageNode->value.find("\"");
        size_t quoteEnd = messageNode->value.find("\"", quoteStart + 1);

        if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
            std::string msg = messageNode->value.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            messageContentNode = std::make_unique<StringNode>(std::move(std::string(msg)));
        } 
        
        else {
            std::cerr << "Error: message content not found in message node." << std::endl;
            return nullptr;
        }

        return std::make_unique<MessageNode>(std::move(messageContentNode), std::move(recipentsNode));
    }

    std::cerr << "Error: Message node not found." << std::endl;
    return nullptr;
}


std::unique_ptr<Visitor::DiscardNode> 
Visitor::Parser::visitDiscard(const ts::Node &node) {
    std::unique_ptr<StringNode> discardContentNode;
    std::unique_ptr<StringNode> discardTargetNode;
    ts::Cursor cursor = node.getCursor();

    std::unique_ptr<StringNode> discardNode = findNode(node, "discard");

    if(discardNode) {
        // std::cout << "discard content: " << discardNode->value << std::endl;

        size_t discardPos = discardNode->value.find("discard");

        if (discardPos != std::string::npos) {
            std::string afterDiscard = discardNode->value.substr(discardPos + 8);

            std::istringstream iss(afterDiscard);
            std::string discardContent;
            iss >> discardContent;

            discardContentNode = std::make_unique<StringNode>(std::move(std::string(discardContent)));

        } else {
            std::cout << "'Error: discard' not found in Discard node." << std::endl;
            return nullptr;
        }

        size_t fromPos = discardNode->value.find("from");

        if (fromPos != std::string::npos) {
            std::string afterFrom = discardNode->value.substr(fromPos + 5);

            std::istringstream iss(afterFrom);
            std::string discardTarget;
            iss >> discardTarget;

            discardTargetNode = std::make_unique<StringNode>(std::move(std::string(discardTarget)));
        } 

        else {
            std::cout << "Error: 'from' not found in Discard node." << std::endl;
            return nullptr;
        }

        return std::make_unique<DiscardNode>(std::move(discardContentNode), std::move(discardTargetNode));
    }
    std::cerr << "Error: Discard node not found." << std::endl;
    return nullptr;
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
