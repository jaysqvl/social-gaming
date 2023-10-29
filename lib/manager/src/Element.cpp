// #include "Element.hpp"
// //#include "User.hpp"
// #include "Rule.hpp"

// void Element<std::map<std::string, ElementPtr>>::setElement(std::string key, ElementPtr element) {
//     data[key] = element;
// }

// // Modified this function since there is no suitable operator[] defined for const std::map<std::string, ElementPtr>
// ElementPtr Element<std::map<std::string, ElementPtr>>::getElement(std::string key) const {
//     auto it = data.find(key);
//     if (it != data.end())
//         return it->second;
//     else 
//         LOG(INFO) << "Key doesn't exist!\n";
//         return nullptr;
// }

// void Element<std::map<std::string, ElementPtr>>::updateElement(std::string key, ElementPtr element) {
//     if (data.find(key) != data.end())
//     	data[key] = element;
//     else 
//     	LOG(INFO) << "Key doesn't exist!\n";
// }

// void Element<std::map<std::string, ElementPtr>>::removeKey(std::string key) {
//     data.erase(key);
// }

// bool Element<std::map<std::string, ElementPtr>>::containKey(std::string key) {
//     if (data.find(key) != data.end())
//     	return true;
//     else
//     	return false;
// }

// void Element<std::vector<ElementPtr>>::extend(ElementPtr element) {
//     data.emplace_back(element);
// }

// void Element<std::vector<ElementPtr>>::reverse() {
//     std::reverse(data.begin(), data.end());
// }

// void Element<std::vector<ElementPtr>>::shuffle() {
//     auto rd = std::random_device {};
//     auto rng = std::default_random_engine { rd() };
//     std::shuffle(data.begin(), data.end(), rng);
// }

// void Element<std::vector<ElementPtr>>::discard(size_t amount) {
//     data.erase(data.end() - amount, data.end());
// }

