#include <json/json.h>

namespace NJson {

// helper type for the visitor #4
template <class... Ts>
struct TOverloaded : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
TOverloaded(Ts...) -> TOverloaded<Ts...>;

std::ostream& operator<<(std::ostream& out, const TJson& obj) {
    std::visit(TOverloaded{
                   [&](int64_t value) { out << value; },
                   [&](double value) { out << value; },
                   [&](bool value) { out << value; },
                   [&](std::nullopt_t) { out << "null"; },
                   [&](const std::string& value) { out << '"' << value << '"'; },  // Skip escaping for simplicity
                   [&](const TJson::TArray& value) {
                       out << '[';
                       for (auto it = value.begin(); it != value.end();) {
                           out << *it;
                           ++it;
                           if (it != value.end()) {
                               out << ',';
                           }
                       }
                       out << ']';
                   },
                   [&](const TJson::TMap& value) {
                       out << '{';
                       for (auto it = value.begin(); it != value.end();) {
                           out << '"' << it->first << '"' << ':' << it->second;
                           ++it;
                           if (it != value.end()) {
                               out << ',';
                           }
                       }
                       out << '}';
                   },
               },
               obj.Data);
    return out;
}

}  // namespace NJson
