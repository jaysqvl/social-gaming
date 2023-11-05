#pragma once

#include <variant>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace Visitor {

struct None;
struct Integer;
struct Boolean;
struct String;
struct Identifier;
struct Range;
struct Pair;
struct List;
struct Dictionary;

//rules Data structs; add as we go
struct For;

using Data = std::variant<
    None, Integer, Boolean,
    String, Identifier, Range,
    Pair, List, Dictionary
>;

struct None {};

struct Integer {
    int value;
};

struct Boolean {
    bool value;
};

struct String {
    std::string value;
};

struct Identifier {
    std::string value;
};

struct Range {
    int begin;
    int end;
};

struct Pair {
    std::shared_ptr<Data> first;
    std::shared_ptr<Data> second;

    Pair(const Data &first, const Data &second) :
        first{std::make_shared<Data>(first)},
        second(std::make_shared<Data>(second)) {}
};

struct List {
    std::vector<Data> value;
};

struct Dictionary {
    std::map<std::string, Data> value;
};

};
