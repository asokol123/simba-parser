#pragma once

#include <optional>
#include <ostream>
#include <unordered_map>
#include <variant>
#include <vector>

namespace NJson {

struct TJson {
    using TMap = std::unordered_map<std::string, TJson>;
    using TArray = std::vector<TJson>;
    std::variant<
        int64_t,
        double,
        bool,
        std::nullopt_t,
        std::string,
        TArray,
        TMap>
        Data;

    TJson(const TJson& other) = default;
    TJson(TJson&& other) = default;
    TJson& operator=(const TJson& other) = default;
    TJson& operator=(TJson&& other) = default;

    explicit TJson(unsigned data) : Data(int64_t(data)) {}
    explicit TJson(int data) : Data(int64_t(data)) {}
    explicit TJson(int64_t data) : Data(data) {}
    explicit TJson(uint64_t data) : Data(int64_t(data)) {}
    explicit TJson(double data) : Data(std::move(data)) {}

    explicit TJson(bool data) : Data(std::move(data)) {}
    explicit TJson(std::nullopt_t data) : Data(std::move(data)) {}
    explicit TJson(std::string data) : Data(std::move(data)) {}
    explicit TJson(std::string_view data) : Data(std::string(data)) {}
    explicit TJson(TArray data) : Data(std::move(data)) {}
    explicit TJson(TMap data) : Data(std::move(data)) {}

    template <typename T>
    explicit TJson(std::optional<T> obj) : Data(std::nullopt) {
        if (obj) {
            *this = TJson(std::move(*obj));
        }
    }

    template <typename T>
    static TJson ToJson(const T& obj) {
        return TJson(obj);
    }

    static TJson Map(TMap data = {}) {
        return TJson(data);
    }
};
std::ostream& operator<<(std::ostream& out, const TJson& obj);

struct TJsonStructWriter {
    TJsonStructWriter(std::string name)
        : Name(std::move(name)), Data() {
    }

    template <typename T>
    TJsonStructWriter& Field(std::string name, T value) {
        Data.insert_or_assign(std::move(name), TJson(std::move(value)));
        return *this;
    }

    TJson Json() && {
        return TJson::Map({{"type", TJson(std::move(Name))},
                           {"data", TJson(std::move(Data))}});
    }

    std::string Name;
    TJson::TMap Data;
};

}  // namespace NJson
