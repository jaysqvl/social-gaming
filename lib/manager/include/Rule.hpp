#pragma once 

#include "Element.hpp"
#include "GameManager.hpp"  // Need to implement this

enum RuleStatus {
	NEW,
	DONE, 
	WAITING // For input
}

class Rule;
using std::shared_ptr<Rule> = RulePtr;
using std::shared_ptr<GameManager> = GameManagerPtr;

class Rule {
private:
	GameManagerPtr ownGame;
	RuleStatus status;
public:
	Rule() : ownGame(nullptr), status(RuleStatus::NEW) {}
	virtual ~Rule() {}
	void setStatus(RuleStatus newStatus) { status = newStatus; }
	RuleStatus getStatus() const { return status; }
	virtual RuleStatus execute(std::map<std::string, ElementPtr> gameState) = 0;
}

class For final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
public:
	For(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
	
}

class Loop final : public Rule {
private:
	bool conditionValue; // initialized to false 
	std::vector<RulePtr> subRules;
public:
	Loop(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules), conditionValue(false) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
	void updateConditionValue(std::map<std::string, ElementPtr> gameState); 
}

class ParallelFor final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
	std::map<ElementPtr, std::vector<RulePtr>::iterator> currentRuleMap;
	std::map<ElementPtr, RuleStatus>> currentStatusMap;
	
public:
	ParallelFor(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
}

class InParallel final : public Rule {
private:
	std::vector<RulePtr> subRules;
public:
	InParallel(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
}

class Match final : public Rule {
private:
	ElementPtr target;
	std::map<ElementPtr, std::vector<RulePtr>> candidateMap;
public:
	For(GameManagerPtr game, ElementPtr newTarget, std::vector<RulePtr>> map) : ownGame(game), target(newTarget), candidateMap(map) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
}
