#pragma once 

#include "Element.hpp"
#include "User.hpp"
#include "GameManager.hpp"  // Need to implement this
#include <functional>

enum class RuleStatus {
	NEW,
	DONE,
	WAITING, // For input
	FAILED,
};

class Rule;
using RulePtr = std::shared_ptr<Rule>;
using GameManagerPtr = std::shared_ptr<GameManager>;

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
};

class For final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
public:
	For(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;

};

class Loop final : public Rule {
private:
	bool conditionValue; // initialized to false 
	std::vector<RulePtr> subRules;
public:
	Loop(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules), conditionValue(false) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
	void updateConditionValue(std::map<std::string, ElementPtr> gameState);
};

class ParallelFor final : public Rule {
private:
	std::vector<ElementPtr> list;
	std::vector<RulePtr> subRules;
	std::map<ElementPtr, std::vector<RulePtr>::iterator> currentRuleMap;
	std::map<ElementPtr, RuleStatus >> currentStatusMap;

public:
	ParallelFor(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class InParallel final : public Rule {
private:
	std::vector<RulePtr> subRules;
public:
	InParallel(GameManagerPtr game, std::vector<RulePtr> rules) : ownGame(game), subRules(rules) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Match final : public Rule { // Implementation is based on the Rock Paper Scissor Game. 
private:                          // This is matching to only 2 cases (i.e., true or false) for now
	std::function<bool()> condition;
	std::map<bool, std::vector<RulePtr>> candidateMap;
public:
	Match(GameManagerPtr game, std::function<bool()> newCondition, std::map<bool, std::vector<RulePtr>> map) : ownGame(game), condition(newCondition), candidateMap(map) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Extend final : public Rule {
private:
	ElementPtr list; //This should have an implicit type of std::vector
	ElementPtr content;
public:
	Extend(GameManagerPtr game, ElementPtr newList, ElementPtr addedContent) : ownGame(game), list(newList), content(addedContent) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Reverse final : public Rule {
private:
	ElementPtr list; //This should have an implicit type of std::vector
public:
	Extend(GameManagerPtr game, ElementPtr newList) : ownGame(game), list(newList) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Shuffle final : public Rule {
private:
	ElementPtr list; //This should have an implicit type of std::vector
public:
	Extend(GameManagerPtr game, ElementPtr newList) : ownGame(game), list(newList) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Discard final : public Rule {
private:
	ElementPtr list; //This should have an implicit type of std::vector
	size_t amount;
public:
	Discard(GameManagerPtr game, ElementPtr newList, ElementPtr discardedContent) : ownGame(game), list(newList), content(discardedContent) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class InputChoice final : public Rule {
private:
	std::shared_ptr<User> player; // Need to validate their role before executing
	std::string prompt;
	std::vector<ElementPtr> choices;
	std::shared_ptr<std::map< std::shared_ptr<User>, ElementPtr>> target;
	unsigned int timeout;

public:
	InputChoice(GameManagerPtr game, std::shared_ptr<User> player, std::string prompt, std::vector<ElementPtr> choices, std::shared_ptr<std::map< std::shared_ptr<User>, ElementPtr>> target, unsigned int timeout)
		: ownGame(game), prompt(prompt), player(player), choices(choices), target(target), timeout(timeout) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};

class Message final : public Rule {
private:
	std::vector<std::shared_ptr<User>> users;
	std::string prompt;

public:
	InputChoice(GameManagerPtr game, std::vector<std::shared_ptr<User>> users, std::string prompt)
		: ownGame(game), users(users), prompt(prompt) {}
	RuleStatus execute(std::map<std::string, ElementPtr> gameState) final;
};
