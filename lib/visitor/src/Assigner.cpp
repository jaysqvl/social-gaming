#include "Assigner.hpp"

void Visitor::Assigner::operator()(const None& none) {

}

void Visitor::Assigner::operator()(Integer& integer, int newValue) {
	integer.value = newValue;
}

void Visitor::Assigner::operator()(Double& floating, double newValue) {
	floating.value = newValue;
}

void Visitor::Assigner::operator()(Boolean& boolean, bool newValue) {
	boolean.value = newValue;
}

void Visitor::Assigner::operator()(String& string, const string& newValue) {
	string.value = newValue;
}

void Visitor::Assigner::operator()(Identifier& identifier, const string& newValue) {
	// TODO: May need to avoid duplicating an existing identifier
	identifier.value = newValue;
}

void Visitor::Assigner::operator()(Range& range, int newBegin, int newEnd) {
	if (newBegin < newEnd) {
		range.begin = newBegin;
		range.end = newEnd;
	}
}

void Visitor::Assigner::operator()(Pair& pair, const Visitor::Data& newFirst, const Visitor::Data& newSecond) {
	pair.first = std::make_shared<Visitor::Data>(newFirst);
	pair.second = std::make_shared<Visitor::Data>(newSecond);
}

void Visitor::Assigner::operator()(List& list, const std::vector< Visitor::Data>& newValue) {
	list.value = newValue;
}

void Visitor::Assigner::operator()(Dictionary& dict, const std::map<std::string, Visitor::Data>& newValue) {
	dict.value = newValue;
}