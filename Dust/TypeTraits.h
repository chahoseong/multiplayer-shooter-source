#pragma once

template <typename Type, template <typename...> typename Reference>
struct is_specialization : std::false_type { };

template <template <typename...> typename Reference, typename... Args>
struct is_specialization<Reference<Args...>, Reference> : std::true_type { };

template <typename T, template <typename...> typename Reference>
inline constexpr bool is_specialization_v = is_specialization<std::decay_t<T>, Reference>::value;