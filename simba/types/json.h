#pragma once

#include <json/json.h>

namespace NSimba::NSBEReader {

template <typename T>
::NJson::TJson ToJson(const T& object) {
    return ::NJson::TJson(object);
}

template <typename T>
::NJson::TJson ToJson(const std::vector<T>& object) {
    std::vector<::NJson::TJson> data;
    data.reserve(object.size());
    for (const auto& elem : object) {
        data.push_back(ToJson(elem));
    }
    return ::NJson::TJson(data);
}

template <typename T>
::NJson::TJson ToJson(const std::optional<T>& object) {
    if (!object) {
        return ::NJson::TJson(std::nullopt);
    }
    return ToJson(*object);
}

}  // namespace NSimba::NSBEReader
