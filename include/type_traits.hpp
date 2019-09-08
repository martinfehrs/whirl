#ifndef __TYPE_TRAITS_HPP__
#define __TYPE_TRAITS_HPP__


#include <type_traits>
#include <istream>

namespace whirl
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // SFINEA utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename... Ts>
    using requires_t = std::enable_if_t<std::conjunction_v<Ts...>>;

    template <typename T1, typename T2>
    using requires_type_t = requires_t<std::is_same<T1, T2>>;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // character type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_character_type
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<T, char>,
                std::is_same<T, wchar_t>,
                std::is_same<T, char16_t>,
                std::is_same<T, char32_t>
            >
        >
    { };

    template <typename... Ts>
    struct are_character_types : std::bool_constant<std::conjunction_v<is_character_type<Ts>...>>
    { };


    template <typename T1, typename T2, typename = void>
    struct equality_comparable : std::false_type
    { };

    template <typename T1, typename T2>
    struct equality_comparable<
        T1, T2, std::void_t<decltype(std::declval<T1>() == std::declval<T2>())>
    > : std::true_type
    { };


    template <typename T1, typename T2>
    struct is_compatible_character_type
        : std::bool_constant<
            std::conjunction_v<
                is_character_type<T1>,
                is_character_type<T2>,
                equality_comparable<T1, T2>
            >
        >
    { };

    template <typename T>
    constexpr auto is_character_type_v = is_character_type<T>::value;

    template <typename... Ts>
    constexpr auto are_character_types_v = are_character_types<Ts...>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_character_type_v = is_compatible_character_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // input source type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename = void>
    struct input_source_traits;

    template <typename T, typename T2, typename = void>
    struct is_input_source_trait_class_type_impl : std::false_type { };

    template <typename T1, typename T2>
    struct is_input_source_trait_class_type_impl<T1, T2, std::void_t<
        requires_t<is_character_type<typename T1::char_type>>,
        requires_type_t<decltype(T1::look_ahead(std::declval<T2&>())), typename T1::char_type>,
        requires_type_t<decltype(T1::read(std::declval<T2&>())), typename T1::char_type>,
        requires_type_t<decltype(T1::ignore(std::declval<T2&>())), void>,
        requires_type_t<decltype(T1::is_end(std::declval<T2&>())), bool>
    >>
        : is_character_type<decltype(T1::read(std::declval<T2&>()))>
    { };

    template <typename T>
    struct is_input_source_trait_class_type : std::false_type { };

    template <template <typename...> typename TT, typename T, typename... Ts>
    struct is_input_source_trait_class_type<TT<T, Ts...>>
        : is_input_source_trait_class_type_impl<TT<T, Ts...>, T>
    { };

    template <typename T>
    struct is_input_source_type
        : std::bool_constant<is_input_source_trait_class_type<input_source_traits<T>>::value>
    { };

    template <typename T1>
    constexpr auto is_input_source_type_v = is_input_source_type<T1>::value;

    template <typename T1, typename T2, typename = void>
    struct is_compatible_input_source_type : std::false_type {};

    template <typename T1, typename T2>
    struct is_compatible_input_source_type<T1, T2, std::enable_if_t<is_input_source_type_v<T1>>>
        : is_compatible_character_type<typename input_source_traits<T1>::char_type, T2>
    { };

    template <typename T1, typename T2>
    constexpr auto is_compatible_input_source_type_v =
        is_compatible_input_source_type<T1, T2>::value;


    template <template <typename...> typename TT, typename T, typename... Ts>
    struct input_source_traits<
        TT<T, Ts...>, requires_t<std::is_base_of<std::basic_istream<T, Ts...>, TT<T, Ts...>>>
    >
    {
        using char_type = T;
        using stream_type = TT<T, Ts...>;

        static char_type look_ahead(std::basic_istream<T, Ts...>& ins)
        {
            return ins.peek();
        }

        static char_type read(std::basic_istream<T, Ts...>& ins)
        {
            return ins.get();
        }

        static void ignore(std::basic_istream<T, Ts...>& ins)
        {
            return ins.ignore();
        }

        static auto is_end(std::basic_istream<T, Ts...>& ins)
        {
            return ins.peek() == std::char_traits<T>::eof();
        }
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound predicate type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <typename T1, typename T2>
        using return_type_t = decltype(std::declval<T1>()(std::declval<std::basic_istream<T2>&>()));

        template<typename, typename, typename = void>
        struct is_bound_predicate_impl : std::false_type {};


        template <typename T1, typename T2>
        struct is_bound_predicate_impl<
            T1,
            T2,
            requires_type_t<return_type_t<T1, T2>, bool>
        >
            : std::true_type
        {};

    }

    template <typename T>
    struct is_bound_predicate
        : std::disjunction<
            detail::is_bound_predicate_impl<T, char>,
            detail::is_bound_predicate_impl<T, wchar_t>,
            detail::is_bound_predicate_impl<T, char16_t>,
            detail::is_bound_predicate_impl<T, char32_t>
        >
    {};

    template <typename T>
    constexpr auto is_bound_predicate_v = is_bound_predicate<T>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // transformator type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <typename T, typename I, typename = void>
        struct is_transformator_impl : std::false_type
        {};

        template <typename T, typename I>
        struct is_transformator_impl<
            T, I, std::void_t<decltype(std::declval<T>()(std::declval<I>()))>
        > : std::negation<std::is_same<decltype(std::declval<T>()(std::declval<I>())), void>>
        {};
    }

    template <typename T>
    struct is_transformator
        : std::disjunction<
            detail::is_transformator_impl<T, char>,
            detail::is_transformator_impl<T, wchar_t>,
            detail::is_transformator_impl<T, char16_t>,
            detail::is_transformator_impl<T, char32_t>
        >
    {};

    template <typename T>
    constexpr auto is_transformator_v = is_transformator<T>::value;

}

#endif /*__TYPE_TRAITS_HPP__*/