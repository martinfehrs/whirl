#ifndef __LL1_HPP__
#define __LL1_HPP__


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
    // basic bound predicates
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename C>
    struct bound_is_predicate
    {

        static_assert(is_character_type_v<C>);


        explicit constexpr bound_is_predicate(const C& cmp)
            : cmp{ cmp }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, C>>>
        constexpr bool operator()(I& ins) const
        {
            return input_source_traits<I>::look_ahead(ins) == this->cmp;
        }

        C cmp;

    };

    template <typename C>
    struct bound_is_not_predicate
    {

        static_assert(is_character_type_v<C>);


        explicit constexpr bound_is_not_predicate(const C& cmp)
            : cmp{ cmp }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, C>>>
        constexpr bool operator()(I& ins) const
        {
            return input_source_traits<I>::look_ahead(ins) != this->cmp;
        }

        C cmp;

    };

    template <typename... Cs>
    struct bound_is_one_of_predicate
    {

        static_assert(are_character_types_v<Cs...>);


        explicit constexpr bound_is_one_of_predicate(const Cs&... cmps)
            : cmps{ cmps... }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, Cs>...>>
        constexpr bool operator()(I& ins) const
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

        static_assert(are_character_types_v<Cs...>);


        explicit constexpr bound_is_none_of_predicate(const Cs&... cmps)
            : cmps{ cmps... }
        { }

        template <typename I, typename = requires_t<is_compatible_input_source_type<I, Cs>...>>
        constexpr bool operator()(I& ins) const
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

    struct bound_is_end_predicate
    {

        template <typename I, typename = requires_t<is_input_source_type<I>>>
        constexpr bool operator()(I& ins) const
        {
            return input_source_traits<I>::is_end(ins);
        }

    };

    struct bound_is_character_predicate
    {

        template <typename I, typename = requires_t<is_input_source_type<I>>>
        constexpr bool operator()(I& ins) const
        {
            return !input_source_traits<I>::is_end(ins);
        }

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // composed bound predicates
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename P1, typename P2>
    struct bound_predicate_conjunction
    {

	static_assert(is_bound_predicate_v<P1>);
	static_assert(is_bound_predicate_v<P2>);


	explicit constexpr bound_predicate_conjunction(P1 p1, P2 p2)
	    : p1{ p1 }
	    , p2{ p2 }
	{ }

	template <typename T>
	constexpr bool operator()(T& ins) const
	{
	    return this->p1(ins) && this->p2(ins);
	}

	P1 p1;
	P2 p2;

    };

    template <typename P1, typename P2>
    struct bound_predicate_disjunction
    {

	static_assert(is_bound_predicate_v<P1>);
	static_assert(is_bound_predicate_v<P2>);


	explicit constexpr bound_predicate_disjunction(const P1& pred1, const P2& pred2)
	    : pred1{ pred1 }
	    , pred2{ pred2 }
	{ }

	template <typename I>
	constexpr bool operator()(I& ins) const
	{
	    return this->pred1(ins) || this->pred2(ins);
	}

	P1 pred1;
	P2 pred2;

    };

    template <typename P>
    struct bound_predicate_negation
    {

	static_assert(is_bound_predicate_v<P>);


	explicit constexpr bound_predicate_negation(const P& pred)
	    : pred{ pred }
	{ }

	template <typename I>
	constexpr bool operator()(I& ins) const
	{
	    return !this->pred(ins);
	}

	P pred;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // universal logical bound predicate operations
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

    template <typename C, typename = requires_t<is_character_type<C>>>
    constexpr auto operator!(const bound_is_predicate<C>& p)
    {
        return bound_is_not_predicate<C>{ p.cmp };
    }

    template <typename C, typename = requires_t<is_character_type<C>>>
    constexpr auto operator!(const bound_is_not_predicate<C>& p)
    {
        return bound_is_predicate<C>{ p.cmp };
    }

    template <typename... Cs, typename = requires_t<are_character_types<Cs...>>>
    constexpr auto operator!(const bound_is_one_of_predicate<Cs...>& p)
    {
        return std::apply(
            [](const auto&... cmps) {
                return bound_is_none_of_predicate<Cs...>{ cmps... };
            },
            p.cmps
        );
    }

    template <typename... Cs, typename = requires_t<are_character_types<Cs...>>>
    constexpr auto operator!(const bound_is_none_of_predicate<Cs...>& p)
    {
        return std::apply(
            [](const auto&... cmps) {
                return bound_is_one_of_predicate<Cs...>{ cmps... };
            },
            p.cmps
        );
    }

    template <typename C1, typename C2, typename = requires_t<are_character_types<C1, C2>>>
    constexpr auto operator||(const bound_is_predicate<C1>& lhs, const bound_is_predicate<C2>& rhs)
    {
        return bound_is_one_of_predicate{ lhs.cmp, rhs.cmp };
    }

    template <typename... Cs, typename C, typename = requires_t<are_character_types<C, Cs...>>>
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

    template <typename C, typename... Cs, typename = requires_t<are_character_types<C, Cs...>>>
    constexpr auto operator||(
        const bound_is_predicate<C>& lhs, const bound_is_one_of_predicate<Cs...>& rhs
    )
    {
        return rhs || lhs;
    }

    template <
        typename... Cs1, typename... Cs2, typename = requires_t<are_character_types<Cs1..., Cs2...>>
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

    template <typename C1, typename C2, typename = requires_t<are_character_types<C1, C2>>>
    constexpr auto operator&&(
        const bound_is_not_predicate<C1>& lhs, const bound_is_not_predicate<C2>& rhs
    )
    {
        return bound_is_none_of_predicate{ lhs.cmp, rhs.cmp };
    }

    template <
        typename... Cs,
        typename C,
        typename = requires_t<is_character_type<Cs>...>,
        typename = requires_t<is_character_type<C>>
    >
    constexpr auto operator&&(
        const bound_is_none_of_predicate<Cs...>& lhs, const bound_is_not_predicate<C>& rhs
    )
    {
        return std::apply(
            [&rhs](const auto&... cmps) {
                return bound_is_one_of_predicate<Cs..., C>{ cmps..., rhs.cmp };
            },
            lhs.cmps
        );
    }

    template <typename C, typename... Cs, typename = requires_t<are_character_types<C, Cs...>>>
    constexpr auto operator&&(
        const bound_is_not_predicate<C>& lhs, const bound_is_none_of_predicate<Cs...>& rhs
    )
    {
        return rhs || lhs;
    }

    template <
        typename... Cs1, typename... Cs2, typename = requires_t<are_character_types<Cs1..., Cs2...>>
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

    template <typename C, typename = requires_t<is_character_type<C>>>
    constexpr auto is(const C& cmp)
    {
        return bound_is_predicate{ cmp };
    }

    template <typename C, typename = requires_t<is_character_type<C>>>
    constexpr auto is_not(const C& cmp)
    {
        return bound_is_not_predicate{ cmp };
    }

    template <typename... Cs, typename = requires_t<are_character_types<Cs...>>>
    constexpr auto is_one_of(const Cs&... cmp)
    {
        return bound_is_one_of_predicate{ cmp... };
    }

    template <typename... Cs, typename = requires_t<are_character_types<Cs...>>>
    constexpr auto is_none_of(const Cs&... cmp)
    {
        return bound_is_none_of_predicate{ cmp... };
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined bound predicates
    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr auto is_end            = bound_is_end_predicate{};
    constexpr auto is_character      = bound_is_character_predicate{};
    constexpr auto is_blank          = is_one_of(' ', '\t');
    constexpr auto is_space          = is_one_of(' ', '\t', '\n');
    constexpr auto is_zero           = is('0');
    constexpr auto is_non_zero_digit = is_one_of('1', '2', '3', '4', '5', '6', '7', '8', '9');
    constexpr auto is_digit          = is_zero || is_non_zero_digit;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // transformators
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct as_transform
    {
        constexpr explicit as_transform(const T& obj)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            : obj{ obj }
        { }

        constexpr explicit as_transform(T&& obj)
            noexcept(std::is_nothrow_move_constructible_v<T>)
            : obj{ std::move(obj) }
        { }

        template <typename C, typename = requires_t<is_character_type<C>>>
        constexpr auto operator()(const C&) const
        {
            return obj;
        }

        T obj;
    };

    struct as_is_transform
    {
        template <typename C, typename = requires_t<is_character_type<C>>>
        constexpr auto operator()(const C& chr) const
        {
            return chr;
        }
    };

    struct as_digit_transform
    {
        template <typename C, typename = requires_t<is_character_type<C>>>
        constexpr auto operator()(const C& chr) const
        {
            return chr - '0';
        }
    };

    struct as_digits_transform
    {
        template <typename N, typename C, typename = requires_t<is_character_type<C>>>
        constexpr auto operator()(const N& num, const C& chr) const
        {
            return num * 10 + chr - '0';
        }
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // transformator factories
    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr auto as_is     = as_is_transform{};
    constexpr auto as_digit  = as_digit_transform{};
    constexpr auto as_digits = as_digits_transform{};

    template <typename T>
    constexpr auto as(T&& obj)
    {
        return as_transform{ std::forward<T>(obj) };
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr void next(I& ins)
    {
        if (input_source_traits<I>::is_end(ins))
            throw unexpected_input{};

        input_source_traits<I>::ignore(ins);
    }

    template <typename I, typename T, typename = requires_t<is_input_source_type<I>>>
    constexpr auto next(I& ins, const T& trans)
    {
        if (input_source_traits<I>::is_end(ins))
            throw unexpected_input{};

        return trans(input_source_traits<I>::read(ins));
    }

    template <typename I, typename = requires_t<is_input_source_type<I>>>
    constexpr void next(I& ins, code_position& pos)
    {
        const auto tok = next(ins, as_is);

        if (tok == '\n')
        {
            pos.row++;
            pos.col = 0;
        }
        else
        {
            pos.col++;
        }
    }

    template <typename I, typename T, typename = requires_t<is_input_source_type<I>>>
    constexpr auto next(I& ins, code_position& pos, const T& trans)
    {
        const auto tok = next(ins, trans);

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

    template <
        typename I,
	typename P,
	typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr void next(I& ins, const P& pred)
    {
        if(!pred(ins))
            throw unexpected_input{};

        next(ins);
    }

    template <
        typename I,
        typename P,
        typename T,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr auto next(I& ins, const T& trans, const P& pred)
    {
        if(!pred(ins))
            throw unexpected_input{};

        return next(ins, trans);
    }

    template <
        typename I,
        typename P,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr void next(I& ins, code_position& pos, const P& pred)
    {
        if(!pred(ins))
            throw unexpected_input{};

        if constexpr(std::is_same_v<P, bound_is_end_predicate>)
            return;

        next(ins, pos);
    }

    template <
        typename P,
        typename I,
        typename T,
        typename = requires_t<is_bound_predicate<P>>,
        typename = requires_t<is_input_source_type<I>>
    >
    constexpr auto next(I& ins, code_position& pos, const P& pred, const T& trans)
    {
        if(!pred(ins))
            throw unexpected_input{};

        if constexpr(std::is_same_v<P, bound_is_end_predicate>)
            return;

        return next(ins, pos, trans);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename I,
        typename P,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr void next_if(I& ins, const P& pred)
    {
        if (pred(ins))
            next(ins);
    }

    template <
        typename I,
        typename P,
        typename T,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr auto next_if(I& ins, const P& pred, const T& trans)
        -> std::optional<decltype(next(ins, trans))>
    {
        if (pred(ins))
            return next(ins, trans);
        else
            return std::nullopt;
    }

    template <
        typename I,
        typename P,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr void next_if(I& ins, code_position& pos, const P& pred)
    {
        if (pred(ins))
           next(ins, pos);
    }

    template <
        typename I,
        typename P,
        typename T,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr std::optional<typename I::char_type> next_if(
        I& ins, code_position& pos, const P& pred, const T& trans)
    {
        if (pred(ins))
            return next(ins, pos, trans);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename I,
        typename P,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr void next_while(I& ins, const P& pred)
    {
        while (pred(ins))
            next(ins);
    }

    template <
        typename I,
        typename P,
        typename T,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr auto next_while(I& ins, const P& pred, const T& trans)
    {
        std::basic_string<typename I::char_type> tokseq;

        while (pred(ins))
            tokseq.push_back(next(ins, trans));

        return tokseq;
    }

    template <
        typename I,
        typename P,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr auto next_while(I& ins, code_position& pos, const P& pred)
    {
        while (pred(ins))
            next(ins, pos);
    }

    template <
        typename I,
        typename P,
        typename V,
        typename T,
        typename = requires_t<is_input_source_type<I>>,
        typename = requires_t<is_bound_predicate<P>>
    >
    constexpr auto next_while(
        I& ins, code_position& pos, const P& pred, const V& init, const T& trans)
    {
        V result = init;

        while (pred(ins))
            result = trans(result, next(ins, pos, as_is));

        return result;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound consumers
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct bound_transforming_read
    {

        explicit constexpr bound_transforming_read(const T& trans)
            : trans{ trans }
        { }

        template <typename I>
        constexpr auto operator()(I& ins) const
        {
            return next(ins, this->trans);
        }

        template <typename I>
        constexpr auto operator()(I& ins, code_position& pos) const
        {
            return next(ins, pos, this->trans);
        }

        T trans;

    };

    template <typename P>
    struct bound_conditional_read
    {

        static_assert(is_bound_predicate_v<P>);


        explicit constexpr bound_conditional_read(const P& pred)
            : pred{ pred }
        { }

        template <typename I>
        constexpr std::optional<typename I::char_type> operator()(I& ins) const
        {
            if (this->pred(ins))
                return next(ins);
            else
                return std::nullopt;
        }

        template <typename I>
        constexpr std::optional<typename I::char_type> operator()(I& ins, code_position& pos) const
        {
            if (this->pred(ins))
                return next(ins, pos);
            else
                return std::nullopt;
        }

        P pred;

    };

    template <typename P, typename T, typename A>
    struct bound_ord_conditional_transforming_read
    {

        static_assert(is_bound_predicate_v<P>);


        constexpr bound_ord_conditional_transforming_read(
            const P& pred, const T& trans, const A& alt)
            : pred{ pred }
            , trans{ trans }
            , alt{ alt }
        { }

        template <typename I>
        constexpr auto operator()(I& ins) const
        {
            if (this->pred(ins))
                return next(ins, this->trans);
            else
                return alt;
        }

        template <typename I>
        constexpr auto operator()(I& ins, code_position& pos) const
        {
            if (this->pred(ins))
                return next(ins, pos, this->trans);
            else
                return alt;
        }

        P pred;
        T trans;
        A alt;

    };

    template <typename P, typename T>
    struct bound_transforming_conditional_read
    {

        static_assert(is_bound_predicate_v<P>);


        explicit constexpr bound_transforming_conditional_read(const P& pred, const T& trans)
            : pred{ pred }
            , trans{ trans }
        { }

        template <typename I>
        constexpr std::optional<typename I::char_type> operator()(I& ins) const
        {
            if (this->pred(ins))
                return next(ins, this->trans);
            else
                return std::nullopt;
        }

        template <typename I>
        constexpr auto operator()(I& ins, code_position& pos) const
        {
            if (this->pred(ins))
                return next(ins, pos, this->trans);
            else
                return std::optional<decltype(next(ins, pos, this->trans))>{};
        }

        template <typename A>
        constexpr auto operator||(const A& alt)
        {
            return bound_ord_conditional_transforming_read{ this->pred, this->trans, alt };
        }

        P pred;
        T trans;

    };

    template <typename P>
    struct bound_conditional_multi_read
    {

        static_assert(is_bound_predicate_v<P>);


        explicit constexpr bound_conditional_multi_read(const P& pred)
            : pred{ pred }
        { }

        template <typename I>
        constexpr void operator()(I& ins) const
        {
            next_while(ins, this->pred);
        }

        template <typename I>
        constexpr void operator()(I& ins, code_position& pos) const
        {
            next_while(ins, pos, this->pred);
        }

        P pred;

    };

    template <typename P, typename T>
    struct bound_transforming_conditional_multi_read
    {

        static_assert(is_bound_predicate_v<P>);


        explicit constexpr bound_transforming_conditional_multi_read(const P& pred, const T& trans)
            : pred{ pred }
            , trans{ trans }
        { }

        template <typename V, typename I>
        auto operator()(I& ins, const V& init) const
        {
            return next_while(ins, this->pred, init, trans);
        }

        template <typename V, typename I>
        auto operator()(I& ins, code_position& pos, const V& init) const
        {
            return next_while(ins, pos, this->pred, init, trans);
        }

        P pred;
        T trans;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound consumer factories
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    constexpr auto next(const T& trans)
    {
        return bound_transforming_read{ trans };
    }

    template <typename P, typename = requires_t<is_bound_predicate<P>>>
    constexpr auto next_if(const P& pred)
    {
        return bound_conditional_read{ pred };
    }

    template <typename P, typename T, typename = requires_t<is_bound_predicate<P>>>
    constexpr auto next_if(const P& pred, const T& trans)
    {
        return bound_transforming_conditional_read{ pred, trans };
    }

    template <typename P, typename = requires_t<is_bound_predicate<P>>>
    constexpr auto next_while(const P& pred)
    {
        return bound_conditional_multi_read{ pred };
    }

    template <typename P, typename T, typename = requires_t<is_bound_predicate<P>>>
    constexpr auto next_while(const P& pred, const T& trans)
    {
        return bound_transforming_conditional_multi_read{ pred, trans };
    }

}


#endif /*__LL1_HPP__*/
