#pragma once

#include <for_each_macro.h>

///////////////////////////////////////////////////////////////////////////
// FlagSet
#define M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER(name, value) \
    name = value,

#define M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER_HELPER(arg) \
    M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER arg

#define M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_CHECKER(arg, value) \
    bool Is##arg() { return Value & (1uLL << value); }

#define M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_CHECKER_HELPER(arg) \
    M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_CHECKER arg

#define M_CREATE_SIMBA_SET(base, name, ...)                                           \
    struct name {                                                                     \
        enum class EFlag : uint8_t {                                                  \
            FOR_EACH(M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER_HELPER, __VA_ARGS__) \
        };                                                                            \
        decltype(std::declval<TSBEReader>().Read<base>()) Value;                      \
        FOR_EACH(M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_CHECKER_HELPER, __VA_ARGS__)     \
    };                                                                                \
    template <>                                                                       \
    inline auto TSBEReader::Read<name>() {                                            \
        auto value = Read<base>();                                                    \
        return name{.Value = value};                                                  \
    }                                                                                 \
    template <>                                                                       \
    inline ::NJson::TJson ToJson(const name& value) {                                 \
        return ::NJson::TJson(value.Value);                                           \
    }
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Enum
#define M_CREATE_SIMBA_ENUM(name, base, ...)                                                          \
    enum class name : decltype(std::declval<TSBEReader>().Read<base>()) {                             \
        FOR_EACH(M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER_HELPER, __VA_ARGS__)                     \
    };                                                                                                \
    template <>                                                                                       \
    inline auto TSBEReader::Read<name>() {                                                            \
        return static_cast<name>(Read<base>());                                                       \
    }                                                                                                 \
    template <>                                                                                       \
    inline ::NJson::TJson ToJson(const name& value) {                                                 \
        return ::NJson::TJson(static_cast<decltype(std::declval<TSBEReader>().Read<base>())>(value)); \
    }

#define M_CREATE_SIMBA_NULL_ENUM(name, base, ...)                                                                 \
    enum class name : decltype(std::declval<TSBEReader>().Read<base>())::value_type {                             \
        FOR_EACH(M__HELPER_CREATE_SIMBA_ENUM_ELEMENT_BUILDER_HELPER, __VA_ARGS__)                                 \
    };                                                                                                            \
    template <>                                                                                                   \
    inline auto TSBEReader::Read<name>() {                                                                        \
        auto inner = Read<base>();                                                                                \
        if (!inner) {                                                                                             \
            return std::optional<name>(std::nullopt);                                                             \
        }                                                                                                         \
        return std::optional<name>(static_cast<name>(*inner));                                                    \
    }                                                                                                             \
    template <>                                                                                                   \
    inline ::NJson::TJson ToJson(const name& value) {                                                             \
        return ::NJson::TJson(static_cast<decltype(std::declval<TSBEReader>().Read<base>())::value_type>(value)); \
    }
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Struct
#define M__HELPER_CREATE_SIMBA_STRUCT_FIELD(name, type) \
    decltype(std::declval<TSBEReader>().Read<type>()) name;

#define M__HELPER_CREATE_SIMBA_STRUCT_FIELD_HELPER(arg) \
    M__HELPER_CREATE_SIMBA_STRUCT_FIELD arg

#define M__HELPER_CREATE_SIMBA_STRUCT_FIELD_READ(name, type) \
    result.name = Read<type>();

#define M__HELPER_CREATE_SIMBA_STRUCT_FIELD_READ_HELPER(arg) \
    M__HELPER_CREATE_SIMBA_STRUCT_FIELD_READ arg

#define M__HELPER_MEMBER_ACCESS(object, field) object.field

#define M__HELPER_SIMBA_JSON_WRITE_FIELD(name, type) \
    .Field(#name, ToJson(M__HELPER_MEMBER_ACCESS(value, name)))

#define M__HELPER_SIMBA_JSON_WRITE_FIELD_HELPER(arg) \
    M__HELPER_SIMBA_JSON_WRITE_FIELD arg

#define M_CREATE_SIMBA_STRUCT(name, ...)                                                           \
    struct name {                                                                                  \
        FOR_EACH(M__HELPER_CREATE_SIMBA_STRUCT_FIELD_HELPER, __VA_ARGS__)                          \
    };                                                                                             \
    template <>                                                                                    \
    inline auto TSBEReader::Read<name>() {                                                         \
        name result;                                                                               \
        FOR_EACH(M__HELPER_CREATE_SIMBA_STRUCT_FIELD_READ_HELPER, __VA_ARGS__)                     \
        return result;                                                                             \
    }                                                                                              \
    template <>                                                                                    \
    inline ::NJson::TJson ToJson(const decltype(std::declval<TSBEReader>().Read<name>())& value) { \
        return std::move(::NJson::TJsonStructWriter(#name)                                         \
                             FOR_EACH(M__HELPER_SIMBA_JSON_WRITE_FIELD_HELPER, __VA_ARGS__))       \
            .Json();                                                                               \
    }
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Message
#define M_CREATE_SIMBA_MESSAGE(id, name, ...)                                                      \
    struct name {                                                                                  \
        static constexpr uint32_t kId = id;                                                        \
        FOR_EACH(M__HELPER_CREATE_SIMBA_STRUCT_FIELD_HELPER, __VA_ARGS__)                          \
    };                                                                                             \
    template <>                                                                                    \
    inline auto TSBEReader::Read<name>() {                                                         \
        name result;                                                                               \
        FOR_EACH(M__HELPER_CREATE_SIMBA_STRUCT_FIELD_READ_HELPER, __VA_ARGS__)                     \
        return result;                                                                             \
    }                                                                                              \
    template <>                                                                                    \
    inline ::NJson::TJson ToJson(const decltype(std::declval<TSBEReader>().Read<name>())& value) { \
        (void)value;                                                                               \
        return std::move(::NJson::TJsonStructWriter(#name)                                         \
                             FOR_EACH(M__HELPER_SIMBA_JSON_WRITE_FIELD_HELPER, __VA_ARGS__))       \
            .Json();                                                                               \
    }
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// SBEMessage
#define M__HELPER_SIMBA_SBE_MESSAGE_CASE(name) \
    case name::kId:                            \
        return TSBEMessage(Read<name>());

#define M_DECLARE_SIMBA_SBE_MESSAGE(...)                                                                              \
    using TSBEMessage = std::variant<__VA_ARGS__>;                                                                    \
    template <>                                                                                                       \
    inline auto TSBEReader::Read<TSBEMessage>() {                                                                     \
        auto header = Read<TMessageHeader>();                                                                         \
        switch (header.TemplateId) {                                                                                  \
            FOR_EACH(M__HELPER_SIMBA_SBE_MESSAGE_CASE, __VA_ARGS__)                                                   \
            default:                                                                                                  \
                AllowPartialRead();                                                                                   \
                throw std::invalid_argument(std::format("ERROR: Unknown variant {}, please fix", header.TemplateId)); \
        }                                                                                                             \
    }
///////////////////////////////////////////////////////////////////////////
