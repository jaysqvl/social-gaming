#pragma once 

#include "Element.hpp"
#include "GameManager.hpp"  // Need to implement this

enum class RuleStatus {
	NEW,
	DONE, 
	WAITING // For input
};

class Rule;
class GameManager;
using RulePtr = std::shared_ptr<Rule>;
using GameManagerPtr = std::shared_ptr<GameManager>;

class Rule {
private:
	RuleStatus status;
protected:
	GameManagerPtr ownGame;
public:
	Rule(GameManagerPtr ownGame) : ownGame(ownGame), status(RuleStatus::NEW) {}
	virtual ~Rule() {}
	void setStatus(RuleStatus newStatus) { status = newStatus; }
	RuleStatus getStatus() const { return status; }
	virtual RuleStatus execute(std::map<std::string, ElementPtr> gameState) = 0;
};

class For final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
public:
	For(GameManagerPtr game, std::vector<RulePtr> rules) : Rule(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
	
};

class Loop final : public Rule {
private:
	bool conditionValue; // initialized to false 
	std::vector<RulePtr> subRules;
public:
	Loop(GameManagerPtr game, std::vector<RulePtr> rules) : Rule(game), subRules(rules), conditionValue(false) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
	void updateConditionValue(std::map<std::string, ElementPtr> gameState); 
};

class ParallelFor final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
	std::map<ElementPtr, std::vector<RulePtr>::iterator> currentRuleMap;
	std::map<ElementPtr, RuleStatus> currentStatusMap;
	
public:
	ParallelFor(GameManagerPtr game, std::vector<RulePtr> rules) : Rule(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class InParallel final : public Rule {
private:
	std::vector<RulePtr> subRules;
public:
	InParallel(GameManagerPtr game, std::vector<RulePtr> rules) : Rule(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Match final : public Rule {
private:
	ElementPtr target;
	std::map<ElementPtr, std::vector<RulePtr>> candidateMap;
public:
	Match(GameManagerPtr game, ElementPtr newTarget, std::map<ElementPtr, std::vector<RulePtr>> map) : Rule(game), target(newTarget), candidateMap(map) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};
