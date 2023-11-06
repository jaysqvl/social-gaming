#include "Sorter.hpp"


void Visitor::Sorter::operator()(const Visitor::None& none) {
    
}

void Visitor::Sorter::operator()(const Visitor::Integer& integer) {
    
}

void Visitor::Sorter::operator()(const Visitor::String& string) {
   
}

void Visitor::Sorter::operator()(const Visitor::Double& floating) {
    
}

void Visitor::Sorter::operator()(const Visitor::Boolean& boolean) {
    
}

void Visitor::Sorter::operator()(const Visitor::Identifier& identifier) {
    
}

void Visitor::Sorter::operator()(const Visitor::Range& range) {
    
}

void Visitor::Sorter::operator()(const Visitor::Pair& pair) {
  
}

void Visitor::Sorter::operator()(const Visitor::List& list) {
    // May need a visitor class to compare two Data objects 
    std::sort(list.value.begin(), list.value.end(), [](const Visitor::Data& x, const Visitor::Data& y) {

        });
}

void Visitor::Sorter::operator()(const Visitor::Dictionary& dict) {
    std::sort(dict.value.begin(), dict.value.end(), [](const Visitor::Data& x, const Visitor::Data& y) {

        });
}