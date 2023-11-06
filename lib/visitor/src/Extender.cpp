#include "Extender.hpp"

void Visitor::Extender::operator()(const None& none) {

}

void Visitor::Extender::operator()(const Integer& integer, int addition) {
	if (addition > 0) {
		integer.value += addition;
	}
}

void Visitor::Extender::operator()(const Double& floating, double addition) {
	if (addition > 0) {
		floating.value += addition;
	}
}

void Visitor::Extender::operator()(const Boolean& boolean, bool addition) {
	boolean.value = boolean.value && addition;
}

void Visitor::Extender::operator()(const String& string, const string& addition) {
	string.value.append(addition);
}

void Visitor::Extender::operator()(const String& string, char addition) {
	string.value.append(addition);
}

void Visitor::Extender::operator()(const Identifier& identifier, const string& addition) {
	identifier.value.append(addition);
}

//PRE: additionBegin and additionEnd need to be positive
//POST: range.begin moves backward by an amount of additionBegin. range.end moves forward by an amount of additionEnd
void Visitor::Extender::operator()(const Range& range, int additionBegin, int additionEnd) {
	if (additionBegin >= 0 and additionEnd >= 0) {
		range.begin -= additionBegin;
		range.end += additionEnd;
	}
}

void Visitor::Extender::operator()(const Pair& pair) {

}

void Visitor::Extender::operator()(const List& list, const Visitor::Data& newValue) {
	list.value.emplace_back(newValue);
}

void Visitor::Extender::operator()(const Dictionary& dict, std::string additionKey, Visitor::Data& additionValue) {
	dict.value.emplace(additionKey, additionValue);
}
