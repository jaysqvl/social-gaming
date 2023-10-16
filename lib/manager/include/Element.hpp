#pragma once 

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <glog/logging.h>
#include <random>
#include <memory>

class User;
class UserRole;
class Rule;
class RuleStatus;

enum class ElementType {
	UNKNOWN,
	INT, 
	DOUBLE, 
	STRING,
	VECTOR,
	MAP,
	BOOL, 
	USER,
	RULE,
}; 

// An interface for all specialized elements that allow a list to contain heterogeneous data types
class BaseElement {
protected: //Changed from 'private' to 'protected' to make the 'type' member variable accessible below
	ElementType type;
public:
	virtual ~BaseElement() {}
	
	ElementType getType() const { return type; }
};

using ElementPtr = std::shared_ptr<BaseElement>;

// A generic data container 
template <typename T>
class Element final : public BaseElement {
private:
	T data;
public:
	Element(T newData) : data(newData) { type = ElementType::UNKNOWN; }
	
	//Default setter and getter
	void setData(T newData) { data = newData; }
	T getData() const { return data; }
	
	//Default size
	int getSize() const { return 0; }
};

template<>
class Element<int> final : public BaseElement {
private: 
	int data;
public:
	Element(int newData) : data(newData) { type = ElementType::INT; }
	void setData(int newData) { data = newData; }
	int getData() const { return data; }	
};

template<>
class Element<double> final : public BaseElement {
private:
	double data;
public:
	Element(double newData) : data(newData) { type = ElementType::DOUBLE; }
	void setData(double newData) { data = newData; }
	double getData() const { return data; }	
};

template<>
class Element<std::string> final : public BaseElement {
private:
	std::string data;
public:
	Element(std::string newData) : data(newData) { type = ElementType::STRING; }
	void setData(std::string newData) { data = newData; }
	std::string getData() const { return data; }
	size_t getSize() const { return data.size(); }
};

template<>
class Element<std::map<std::string, ElementPtr>> final : public BaseElement {
private:
	std::map<std::string, ElementPtr> data;
public:
	Element(const std::map<std::string, ElementPtr>& newData) : data(newData) { type = ElementType::MAP; }
	void setData(const std::map<std::string, ElementPtr>& newData) { data = newData; }
	std::map<std::string, ElementPtr> getData() const { return data; }
	size_t getSize() const { return data.size(); }
	
	void setElement(std::string key, ElementPtr element);
	ElementPtr getElement(std::string key) const;
	void updateElement(std::string key, ElementPtr element);
	void removeKey(std::string key);
	bool containKey(std::string key);
};

template<>
class Element<std::vector<ElementPtr>> final : public BaseElement {
private:
	std::vector<ElementPtr> data;
public:
	Element(const std::vector<ElementPtr>& newData) : data(newData) { type = ElementType::VECTOR; }
	void setData(const std::vector<ElementPtr>& newData) { data = newData; }
	std::vector<ElementPtr> getData() const { return data; }
	size_t getSize() const { return data.size(); }
	
	//List Operations 
	void extend(ElementPtr element);
	void reverse();
	void shuffle();
	//void sort(); Implement this later
	//void deal(); Implement this later
	void discard(size_t amount);
};

template <>
class Element<bool> final : public BaseElement {
private:
    bool data;

public:
    Element(bool newData) : data(newData) { type = ElementType::BOOL; }
    void setData(bool newData) { data = newData; }
    bool getData() const { return data; }
    std::string getStringData() const { // Renamed the function. C++ does not allow overloading functions based solely on differences in return types.
        if (data)
            return "true";
        else
            return "false";
    }
    void negateData() { data = !data; }
};

template <>
class Element<std::shared_ptr<User>> final : public BaseElement {
private:
	std::shared_ptr<User> data;

public:
	Element(std::shared_ptr<User> newData) : data(newData) { type = ElementType::USER; }
	void setData(std::shared_ptr<User> newData) { data = newData; }
	User getData() const { return *data; }
	std::string getName() const { return data->getName(); }
	uint32_t getId() const { return data->getId(); }
	std::map<std::string, ElementPtr> getMap() const { return data->getMap(); }
	void setRole(UserRole newRole) { data->setRole(newRole); }
	UserRole getRole() const { return data->getRole; }
};

template <>
class Element<std::shared_ptr<Rule>> final : public BaseElement {
private:
	std::shared_ptr<Rule> data;

public:
	Element(std::shared_ptr<Rule> newData) : data(newData) { type = ElementType::RULE; }
	void setData(std::shared_ptr<Rule> newData) { data = newData; }
	Rule getData() const { return *data; }
	void setStatus(RuleStatus newStatus) { data->setStatus(newStatus); }
	RuleStatus getStatus() const { return status; }
	virtual RuleStatus execute(std::map<std::string, ElementPtr> gameState) { data->execute(gameState); }
};

	
	
	
	
