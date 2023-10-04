include "Rule.hpp"

RuleStatus For::execute(std::map<std::string, ElementPtr> gameState) {'
	std::map<std::string, ElementPtr> currentGameState = gameState;
	for (auto element : list) {
		for (auto rule : subRules) {
			RuleStatus currentStatus = rule->execute(currentGameState);
			
			this->ownGame->updateGameState(rule, currentGameState);
			currentGameState = this->ownGame->getGameState();
			
			if (currentStatus == RuleStatus::WAITING) {
				this->setStatus(RuleStatus::WAITING);
				return this->getStatus();
			}
		}
	}
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

void RuleStatus::updateConditionValue(std::map<std::string, ElementPtr> gameState) {
	
}

RuleStatus Loop::execute(std::map<std::string, ElementPtr> gameState) {
	std::map<std::string, ElementPtr> currentGameState = gameState;
	while (!conditionValue) {
		for (auto rule : subRules) {
			RuleStatus currentStatus = rule->execute(currentGameState);
			
			this->ownGame->updateGameState(rule, currentGameState);
			currentGameState = this->ownGame->getGameState();
			
			this->updateConditionValue(currentGameState);
			
			if (currentStatus == RuleStatus::WAITING) {
				this->setStatus(RuleStatus::WAITING);
				return this->getStatus();
			}
		}
	}
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus ParallelFor::execute(std::map<std::string, ElementPtr> gameState) {
	std::map<std::string, ElementPtr> currentGameState = gameState;
	bool running = true;
	if (running) {
		for (auto element : list) {
			for (auto& rule = currentRuleMap[element], rule != subRules.end(); rule++) {
				RuleStatus currentStatus = (*rule)->execute(currentGameState);
			
				this->ownGame->updateGameState(*rule, currentGameState);
				currentGameState = this->ownGame->getGameState();
				currentRuleMap[element] = rule;
				currentStatusMap[element] = currentStatus;
			
				if (currentStatus == RuleStatus::WAITING) {
				
					break;
				}
			}
		}
		auto& incompleteIter = std::find_if(currentStatusMap.begin(), currentStatusMap.end(), [](const pair<ElementPtr, RuleStatus>& p) 
		{
			return p.second == RuleStatus::WAITING;
		});
		if (incompleteIter == currentStatusMap.end())
			running = false;
	}
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus InParallel::execute(std::map<std::string, ElementPtr> gameState) {

}

RuleStatus Match::execute(std::map<std::string, ElementPtr> gameState) {

}


