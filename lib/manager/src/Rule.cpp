include "Rule.hpp"

RuleStatus For::execute(std::map<std::string, ElementPtr> gameState) {
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
			for (auto& rule = currentRuleMap[element]; rule != subRules.end(); rule++) {
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
		// Check if there are still incomplete rules
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

RuleStatus InParallel::execute(std::map<std::string, ElementPtr> gameState) { // Stud function for now. Implement later
	return RuleStatus::DONE;
}

RuleStatus Match::execute(std::map<std::string, ElementPtr> gameState) {
	std::map<std::string, ElementPtr> currentGameState = gameState;
	bool conditionValue = condition();

	auto candidateIt = candidateMap.find(conditionValue);
	for (auto rule = candidateIt->second.begin(); rule != candidateIt->second.end(), rule++) {
		RuleStatus currentStatus = (*rule)->execute(currentGameState);
		this->ownGame->updateGameState(*rule, currentGameState);
		currentGameState = this->ownGame->getGameState();

		if (currentStatus == RuleStatus::WAITING) {
			this->setStatus(RuleStatus::WAITING);
			return this->getStatus();
		}
	}

	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus Extend::execute(std::map<std::string, ElementPtr> gameState) {
	list->extend(content);
	this->ownGame->updateGameState(*rule, gameState);
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus Reverse::execute(std::map<std::string, ElementPtr> gameState) {
	list->reverse();
	this->ownGame->updateGameState(*rule, gameState);
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus Shuffle::execute(std::map<std::string, ElementPtr> gameState) {
	list->shuffle();
	this->ownGame->updateGameState(this, gameState);
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus Discard::execute(std::map<std::string, ElementPtr> gameState) {
	list->discard(amount);
	this->ownGame->updateGameState(this, gameState);
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus InputChoice::execute(std::map<std::string, ElementPtr> gameState) {
	if (player->getRole() != UserRole::Player) {
		this->setStatus(RuleStatus::FAILED);
		return this->getStatus();
	}

	// Need to adapt to networking later

	std::cout << prompt << std::endl;

	for (size_t i = 0; i < choices.size(); ++i) {
		std::cout << i + 1 << ". " << choices[i]->getData<std::string>() << std::endl;
	}

	int choice;
	std::cout << "Enter your choice (1-" << choices.size() << "): ";
	std::cin >> choice;

	// Validate the input
	if (choice < 1 || choice > choices.size()) {
		std::cout << "Invalid choice. Please choose a number between 1 and " << choices.size() << std::endl;
		this->setStatus(RuleStatus::FAILED);
		return this->getStatus();
	}

	(*target)[player] = choices[choice - 1];

	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}

RuleStatus Message::execute(std::map<std::string, ElementPtr> gameState) {
	for (const auto& user : users) {
		// Replace this with the actual message handling logic
		std::string userPrompt = user->getName().append(prompt);

		std::cout << userPrompt << std::endl;

	}
	this->setStatus(RuleStatus::DONE);
	return this->getStatus();
}



