#include <istream>
#include <vector>
#include <optional>
#include <algorithm>
#include <tuple>


namespace LL1
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // SFINEA utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename... Ts>
    using requires_t = std::enable_if_t<std::conjunction_v<Ts...>>;

    template <typename T1, typename T2>
    using requires_type_t = requires_t<std::is_same<T1, T2>>;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // parsing error handling
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct code_position
    {
        unsigned row;
        unsigned col;
    };

    struct unexpected_input { };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // special token types
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct end_token {};
    struct any_character {};


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token type traits
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

    template <typename T>
    struct is_special_token_type
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<T, end_token>,
                std::is_same<T, any_character>
            >
        >
    { };

    template <typename T>
    struct is_token_type
        : std::bool_constant<
            std::disjunction_v<
                is_character_type<T>,
                is_special_token_type<T>
            >
        >
    { };

    template <typename... Ts>
    struct are_character_types
        : std::bool_constant<
            std::disjunction_v<
                is_character_type<Ts>...
            >
        >
    { };

    template <typename... Ts>
    struct are_token_types
        : std::bool_constant<
            std::disjunction_v<
                is_token_type<Ts>...
            >
        >
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
        are_character_types<T1, T2>,
        equality_comparable<T1, T2>
        >
        >
    { };

    template <typename T1, typename T2>
    struct is_compatible_token_type
        : std::bool_constant<
            std::conjunction_v<
                are_token_types<T1, T2>,
                equality_comparable<T1, T2>
            >
        >
    { };


    template <typename T>
    constexpr auto is_character_type_v = is_character_type<T>::value;

    template <typename T>
    constexpr auto is_special_token_type_v = is_special_token_type<T>::value;

    template <typename T>
    constexpr auto is_token_type_v = is_token_type<T>::value;

    template <typename... Ts>
    constexpr auto are_character_types_v = are_character_types<Ts...>::value;

    template <typename... Ts>
    constexpr auto are_token_types_v = are_token_types<Ts...>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_character_type_v = is_compatible_character_type<T1, T2>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_token_type_v = is_compatible_token_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // special tokens
    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr end_token end;
    constexpr any_character character;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // comparison type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_comparison_type : std::bool_constant<is_token_type_v<T>>
    { };

    template <typename T1, typename T2>
    struct is_compatible_comparison_type
        : std::bool_constant<
            std::disjunction_v<
                std::conditional_t<
                    is_character_type_v<T2>,
                    is_compatible_character_type<T1, T2>,
                    std::false_type
                >,
                is_special_token_type<T2>
            >
        >
    { };

    template <typename T>
    constexpr auto is_comparison_type_v = is_comparison_type<T>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_comparison_type_v = is_compatible_comparison_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // input source type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename = void>
    struct input_source_traits;

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

        static auto is_end(std::basic_istream<T, Ts...>& ins)
        {
            return ins.peek() == std::char_traits<T>::eof();
        }
    };

    template <typename T, typename T2, typename = void>
    struct is_input_source_trait_class_type_impl : std::false_type { };

    template <typename T1, typename T2>
    struct is_input_source_trait_class_type_impl<T1, T2, std::void_t<
        requires_t<is_token_type<typename T1::char_type>>,
        requires_type_t<decltype(T1::look_ahead(std::declval<T2&>())), typename T1::char_type>,
        requires_type_t<decltype(T1::read(std::declval<T2&>())), typename T1::char_type>,
        requires_type_t<decltype(T1::is_end(std::declval<T2&>())), bool>
    >>
        : is_token_type<decltype(T1::read(std::declval<T2&>()))>
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

    template <typename T1, typename T2, typename = void>
    struct is_compatible_input_source_type : std::false_type {};

    template <typename T1, typename T2>
    struct is_compatible_input_source_type<T1, T2, std::enable_if_t<is_input_source_type<T1>::value>>
	    : is_compatible_comparison_type<typename input_source_traits<T1>::char_type, T2>
    { };

    template <typename T1>
    constexpr auto is_input_source_type_v = is_input_source_type<T1>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_input_source_type_v =
        is_compatible_input_source_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound predicates
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename P1, typename P2>
    struct bound_predicate_conjunction
    {

        explicit constexpr bound_predicate_conjunction(P1 p1, P2 p2)
            : p1{ p1 }
            , p2{ p2 }
        { }
        
        template <typename T>
        constexpr auto operator()(T& ins) const
        {
            return this->p1(ins) && this->p2(ins);
        }
        
        P1 p1;
        P2 p2;

    };

    template <typename P1, typename P2>
    struct bound_predicate_disjunction
    {

        explicit constexpr bound_predicate_disjunction(const P1& pred1, const P2& pred2)
            : pred1{ pred1 }
            , pred2{ pred2 }
        { }
        
        template <typename I>
        constexpr auto operator()(I& ins) const
        {
            return this->p1(ins) || this->p2(ins);
        }
        
        P1 pred1, pred2;

    };

    template <typename P>
    struct bound_predicate_negation
    {

        explicit constexpr bound_predicate_negation(const P& pred)
            : pred{ pred }
        { }
        
        template <typename T2>
        constexpr auto operator()(T2& ins) const
        {
            return !this->pred(ins);
        }
        
        P pred;

    };

    template <typename C>
    struct bound_is_predicate
    {

        explicit constexpr bound_is_predicate(const C& cmp)
            : cmp{ cmp }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, C>>>
        constexpr auto operator()(I& ins) const
        {
            if constexpr(std::is_same_v<C, any_character>)
                return !input_source_traits<I>::is_end(ins);
            else if constexpr(std::is_same_v<C, end_token>)
                return input_source_traits<I>::is_end(ins);
            else
                return input_source_traits<I>::look_ahead(ins) == this->cmp;
        }

        C cmp;

    };

    template <typename C>
    struct bound_is_not_predicate
    {

        explicit constexpr bound_is_not_predicate(const C& cmp)
            : cmp{ cmp }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, C>>>
        constexpr auto operator()(I& ins) const
        {
            if constexpr(std::is_same_v<C, any_character>)
                return input_source_traits<I>::is_end(ins);
            else if constexpr(std::is_same_v<C, end_token>)
                return !input_source_traits<I>::is_end(ins);
            else
                return input_source_traits<I>::look_ahead(ins) != this->cmp;
        }

        C cmp;

    };

    template <typename... Cs>
    struct bound_is_one_of_predicate
    {

        explicit constexpr bound_is_one_of_predicate(const Cs&... cmps)
            : cmps{ cmps... }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, Cs>...>>
        constexpr auto operator()(I& ins) const
        {
            return std::apply(
                [&ins](const auto&... cmps) {
                    return ((input_source_traits<I>::look_ahead(ins) == cmps) || ...);
                },
                this->cmps
            );
        }

        std::tuple<Cs...> cmps;

    };

    template <typename... Cs>
    struct bound_is_none_of_predicate
    {

        explicit constexpr bound_is_none_of_predicate(const Cs&... cmps)
            : cmps{ cmps... }
        { }

        template <
            typename I,
            typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<I, Cs>...>>
        >
        constexpr auto operator()(I& ins) const
        {
            return std::apply(
                [&ins](const auto&... cmps) {
                    return ((input_source_traits<I>::look_ahead(ins) != cmps) && ...);
                },
                this->cmps
            );
        }

        std::tuple<Cs...> cmps;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound predicate type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename, typename, typename = void>
    struct is_bound_predicate_impl : std::false_type {};

 
    template <typename T1, typename T2>
    struct is_bound_predicate_impl<
        T1,
        T2,
        requires_type_t<decltype(std::declval<T1>()(std::declval<std::basic_istream<T2>>())), bool>
    >
        : std::true_type
    {};

    template <typename T>
    struct is_bound_predicate
        : std::conjunction<
            is_bound_predicate_impl<T, char>,
            is_bound_predicate_impl<T, wchar_t>,
            is_bound_predicate_impl<T, char16_t>,
            is_bound_predicate_impl<T, char32_t>
        >
    {};

    template <typename T>
    constexpr auto is_bound_predicate_v = is_bound_predicate<T>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // universal logical bound predicate predicate operations
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename P, typename = requires_t<is_bound_predicate<P>>>
    constexpr auto operator!(const P& p)
    {
        return bound_predicate_negation(p);
    }

    template <
        typename P1,
        typename P2,
        typename = requires_t<is_bound_predicate<P1>>,
        typename = requires_t<is_bound_predicate<P2>>
    >
    constexpr auto operator&&(const P1& p1, const P2& p2)
    {
        return bound_predicate_conjunction{ p1, p2 };
    }

    template <
        typename P1,
        typename P2,
        typename = requires_t<is_bound_predicate<P1>>,
        typename = requires_t<is_bound_predicate<P2>>
    >
    constexpr auto operator||(const P1& p1, const P2& p2)
    {
        return bound_predicate_disjunction{ p1, p2 };
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // specialzed optimized logical bound predicate operations
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename C, typename = requires_t<is_comparison_type<C>>>
    constexpr auto operator!(const bound_is_predicate<C>& p)
    {
        return bound_is_not_predicate<T>{ p.cmp };
    }

    template <typename C, typename = requires_t<is_comparison_type<C>>>
    constexpr auto operator!(const bound_is_not_predicate<C>& p)
    {
        return bound_is_predicate<T>{ p.cmp };
    }

    template <typename... Cs, typename = requires_t<is_comparison_type<Cs>...>>
    constexpr auto operator!(const bound_is_one_of_predicate<Cs...>& p)
    {
        return std::apply(
            [](const auto&... cmps) {
                return bound_is_none_of_predicate<Ts...>{ cmps... };
            },
            p.cmps
        );
    }

    template <typename... Cs, typename = requires_t<is_comparison_type<Cs>...>>
    constexpr auto operator!(const bound_is_none_of_predicate<Cs...>& p)
    {
        return std::apply(
            [](const auto&... cmps) {
                return bound_is_one_of_predicate<Ts...>{ cmps... };
            },
            p.cmps
        );
    }

    template <
        typename C1,
        typename C2,
        typename = requires_t<is_comparison_type<C1>>,
        typename = requires_t<is_comparison_type<C2>>
    >
    constexpr auto operator||(const bound_is_predicate<C1>& lhs, const bound_is_predicate<C2>& rhs)
    {
        return bound_is_one_of_predicate{ lhs.cmp, rhs.cmp };
    }

    template <
        typename... Cs,
        typename C,
        typename = requires_t<is_comparison_type<C>>,
        typename = requires_t<is_comparison_type<Cs>...>
    >
    constexpr auto operator||(
        const bound_is_one_of_predicate<Cs...>& lhs, const bound_is_predicate<C>& rhs
    )
    {
        return std::apply(
            [&rhs](const auto&... cmps) {
                return bound_is_one_of_predicate{ cmps..., rhs.cmp };
            },
            lhs.cmps
        );
    }

    template <
        typename C,
        typename... Cs,
        typename = requires_t<is_comparison_type<C>>,
        typename = requires_t<is_comparison_type<Cs>...>
    >
    constexpr auto operator||(
        const bound_is_predicate<C>& lhs, const bound_is_one_of_predicate<Cs...>& rhs
    )
    {
        return rhs || lhs;
    }

    template <
        typename... Cs1,
        typename... Cs2,
        typename = requires_t<is_comparison_type<Cs1>...>,
        typename = requires_t<is_comparison_type<Cs2>...>
    >
    constexpr auto operator||(
        const bound_is_one_of_predicate<Cs1...>& lhs, const bound_is_one_of_predicate<Cs2...>& rhs
    )
    {
        return std::apply(
            [&rhs](const auto&... cmps) {
                return (rhs || ... || cmps);
            },
            lhs.cmps
        );
    }

    template <
        typename C1,
        typename C2,
        typename = requires_t<is_comparison_type<C1>>,
        typename = requires_t<is_comparison_type<C2>>
    >
    constexpr auto operator&&(
        const bound_is_not_predicate<C1>& lhs, const bound_is_not_predicate<C2>& rhs
    )
    {
        return bound_is_none_of_predicate{ lhs.cmp, rhs.cmp };
    }

    template <
        typename... Cs,
        typename C,
        typename = requires_t<is_comparison_type<Cs>...>,
        typename = requires_t<is_comparison_type<C>>
    >
    constexpr auto operator&&(
        const bound_is_none_of_predicate<Cs...>& lhs, const bound_is_not_predicate<C>& rhs
    )
    {
        return std::apply(
            [&rhs](const auto&... cmps) {
                return bound_is_one_of_predicate<Ts..., T>{ cmps..., rhs.cmp };
            },
            lhs.cmps
        );
    }

    template <
        typename C,
        typename... Cs,
        typename = requires_t<is_comparison_type<C>>,
        typename = requires_t<is_comparison_type<Cs>...>
    >
    constexpr auto operator&&(
        const bound_is_not_predicate<C>& lhs, const bound_is_none_of_predicate<Cs...>& rhs
    )
    {
        return rhs || lhs;
    }

    template <
        typename... Cs1,
        typename... Cs2,
        typename = requires_t<is_comparison_type<Cs1>...>,
        typename = requires_t<is_comparison_type<Cs2>...>
    >
    constexpr auto operator&&(
        const bound_is_none_of_predicate<Cs1...>& lhs,
        const bound_is_none_of_predicate<Cs2...>& rhs
    )
    {
        return std::apply(
            [&rhs](const auto&... cmps) {
                return (rhs || ... || cmps);
            },
            lhs.cmps
        );
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound predicate factories
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename C, typename = requires_t<is_comparison_type<C>>>
    constexpr auto is(const C& cmp)
    {
        return bound_is_predicate{ cmp };
    }

    template <typename C, typename = requires_t<is_comparison_type<C>>>
    constexpr auto is_not(const C& cmp)
    {
        return bound_is_not_predicate{ cmp };
    }

    template <typename... Cs, typename = requires_t<is_comparison_type<Cs>...>>
    constexpr auto is_one_of(const Cs&... cmp)
    {
        return bound_is_one_of_predicate{ cmp... };
    }

    template <typename... Cs, typename = requires_t<is_comparison_type<Cs>...>>
    constexpr auto is_none_of(const Cs&... cmp)
    {
        return bound_is_none_of_predicate{ cmp... };
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr auto read(I& ins)
    {
        if (input_source_traits<I>::is_end(ins))
            throw unexpected_input{};

        return ins.get();
    }

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr auto read(I& ins, code_position& pos)
    {
        auto tok = read(ins);

        if (tok == '\n')
        {
            pos.row++;
            pos.col = 0;
        }
        else
        {
            pos.col++;
        }

        return tok;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr std::optional<typename I::char_type> read_if(const P& pred, I& ins)
    {
        if (pred(ins))
            return read(ins);
        else
            return std::nullopt;
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr std::optional<typename I::char_type> read_if(
        const P& pred, I& ins, code_position& pos
    )
    {
        if (pred(ins))
            return read(ins, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto read_while(const P& pred, I& ins)
    {
        std::basic_string<typename I::char_type> tokseq;

        while (is(ins, cmp))
            tokseq.push_back(read(ins));

        return tokseq;
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto read_while(const P& pred, I& ins, code_position& pos)
    {
        std::basic_string<typename I::char_type> tokseq;

        while (is(ins, cmp))
            tokseq.push_back(read(ins, pos));

        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr void next(I& ins)
    {
        read(ins);
    }

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr void next(I& ins, code_position& pos)
    {
        read(ins, pos);
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next(const P& pred, I& ins)
    {
        if(!pred(ins))
            throw unexpected_input{};
        
        return read(ins);
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next(const P& pred, I& ins, code_position& pos)
    {
        if(!pred(ins))
            throw unexpected_input{};

        return read(ins, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next_if(const P& pred, I& ins)
    {
        return pred(ins) ? next(ins), true : false;
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next_if(const P& pred, I& ins, code_position& pos)
    {
        return pred(ins) ? next(ins, pos), true : false;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next_while(const P& pred, I& ins)
    {
        unsigned count = 0;

        while (pred(ins))
        {
            ignore(ins);
            count++;
        }

        return count;
    }

    template <
        typename P,
        typename I,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next_while(const P& pred, I& ins, code_position& pos)
    {
        unsigned count = 0;

        while(pred(ins))
        {
            next(ins, pos);
            count++;
        }

        return count;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined bound predicates
    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr auto is_character      = is(character);
    constexpr auto is_blank          = is_one_of(' ', '\t');
    constexpr auto is_space          = is_one_of(' ', '\t', '\n');
    constexpr auto is_zero           = is('0');
    constexpr auto is_non_zero_digit = is_one_of('1', '2', '3', '4', '5', '6', '7', '8', '9');
    constexpr auto is_digit          = is_zero || is_non_zero_digit;

}
