#include <istream>
#include <vector>
#include <optional>
#include <algorithm>
#include <tuple>


namespace LL1
{
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

    template <typename T>
    struct negated_character
    {
        explicit constexpr negated_character(T tok)
            : tok{tok}
        { }

        constexpr auto negate() const
        {
            return this->tok;
        }

        constexpr auto operator!() const
        {
            return this->negate();
        }

    private:

        T tok;
    };

    template <typename T1, typename T2>
    constexpr auto operator==(negated_character<T1> tok1, negated_character<T2> tok2)
    {
        return !tok1 == tok2.negate();
    }

    template <typename T1, typename T2>
    constexpr auto operator!=(negated_character<T1> tok1, negated_character<T2> tok2)
    {
        return tok1.negate() != tok2.negate();
    }

    template <typename T1, typename T2>
    constexpr auto operator==(negated_character<T1> tok1, T2 tok2)
    {
        return tok1.negate() != tok2;
    }

    template <typename T1, typename T2>
    constexpr auto operator==(T1 tok1, negated_character<T2> tok2)
    {
        return tok1 != tok2.negate();
    }

    template <typename T1, typename T2>
    constexpr auto operator!=(T1 tok1, negated_character<T2> tok2)
    {
        return tok1 == tok2.negate();
    }

    template <typename T1, typename T2>
    constexpr auto operator!=(negated_character<T1> tok1, T2 tok2)
    {
        return tok1.negate() == tok2;
    }


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
                std::is_same<T, char32_t>,
                std::is_same<T, negated_character<char>>,
                std::is_same<T, negated_character<wchar_t>>,
                std::is_same<T, negated_character<wchar_t>>,
                std::is_same<T, negated_character<char16_t>>,
                std::is_same<T, negated_character<char32_t>>
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
    // token set types
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename... Ts>
    struct negated_character_set;

    template <typename... Ts>
    struct character_set
    {
    
    public:

        explicit constexpr character_set(Ts... toks)
            : toks{ toks... }
        { }

        template <typename T>
        constexpr auto contains(T tok) const
        {
            return contains<>(tok, std::index_sequence_for<Ts...>{});
        }

        constexpr auto operator!() const
        {
            return negate(std::index_sequence_for<Ts...>{});
        }

        constexpr auto negate() const
        {
            return negate(std::index_sequence_for<Ts...>{});
        }

    private:

        template <size_t... I>
        constexpr auto negate(std::index_sequence<I...>) const
        {
            return negated_character_set<Ts...>{ std::get<I>(this->toks)... };
        }

        template <typename T, size_t... I>
        constexpr auto contains(T tok, std::index_sequence<I...>) const
        {
            return ((std::get<I>(this->toks) == tok) || ...);
        }

        std::tuple<Ts...> toks;

    };


    template <typename... Ts>
    struct negated_character_set
    {

    public:

        explicit constexpr negated_character_set(Ts... toks)
            : toks{ toks... }
        { }

        template <typename T>
        constexpr auto contains(T tok) const
        {
            return contains<>(tok, std::index_sequence_for<Ts...>{});
        }

        constexpr auto operator!() const
        {
            return negate(std::index_sequence_for<Ts...>{});
        }

        constexpr auto negate() const
        {
            return negate(std::index_sequence_for<Ts...>{});
        }

    private:

        template <size_t... I>
        constexpr auto negate(std::index_sequence<I...>) const
        {
            return character_set<Ts...>{ std::get<I>(this->toks)... };
        }

        template <typename T, size_t... I>
        constexpr auto contains(T tok, std::index_sequence<I...>) const
        {
            return ((!(std::get<I>(this->toks) == tok)) && ...);
        }

        std::tuple<Ts...> toks;

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_character_set_type : std::false_type
    { };

    template <typename... Ts>
    struct is_character_set_type<character_set<Ts...>> : std::true_type
    { };

    template <typename... Ts>
    struct is_character_set_type<negated_character_set<Ts...>> : std::true_type
    { };

    template <typename T1, typename T2, typename = void>
    struct is_compatible_character_set_type : std::false_type
    { };

    template <typename T1, typename T2>
    struct is_compatible_character_set_type<
        T1, T2, std::void_t<decltype(std::declval<T2>().contains(std::declval<T1>()))>
    >: std::true_type
    { };

    template <typename T>
    constexpr auto is_character_set_type_v = is_character_set_type<T>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_character_set_type_v = is_compatible_character_set_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // special tokens
    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr end_token end;
    constexpr any_character character;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined tokens
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace tokens
    {
        constexpr char line_feed = 0xA;
        constexpr char carriage_return = 0xD;
        constexpr char space = 0x20;
        constexpr char tabulator = 0x9;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined token sets
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace sets
    {
        constexpr character_set space{ ' ', '\t', '\n' };

        constexpr character_set blank{' ', '\t'};

        constexpr character_set digit{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // logical token and token set operations
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename = std::enable_if_t<is_token_type_v<T>>
    >
    constexpr auto not_(T tok)
    {
        return negated_character<T>{ tok };
    }

    template <
        typename T,
        typename = std::enable_if_t<is_token_type_v<T>>
    >
        constexpr auto not_(negated_character<T> tok)
    {
        return tok.negate();
    }

    constexpr auto not_(end_token)
    {
        return character;
    }

    constexpr auto not_(any_character)
    {
        return end;
    }

    template <
        template<typename...> typename TT,
        typename... Ts,
        typename = std::enable_if_t<is_character_set_type_v<TT<Ts...>>>,
        typename = std::enable_if_t<are_token_types_v<Ts...>>>
    constexpr auto not_(const TT<Ts...>& set)
    {
        return set.negate();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set factories
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename... Ts, typename = std::enable_if_t<are_character_types_v<Ts...>>>
    constexpr auto one_of(Ts... toks)
    {
        return character_set{ toks... };
    }

    template <typename... Ts, typename = std::enable_if_t<are_character_types_v<Ts...>>>
    constexpr auto none_of(Ts... toks)
    {
        return not_(one_of(toks...));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // comparison type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_comparison_type
        : std::bool_constant<
            std::disjunction_v<
                is_token_type<T>,
                is_character_set_type<T>
            >
        >
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
                is_compatible_character_set_type<T1, T2>,
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
        TT<T, Ts...>,
        std::enable_if_t<std::is_base_of_v<std::basic_istream<T, Ts...>, TT<T, Ts...>>>
    >
    {
        using char_type = T;

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

    template <typename T, typename = void>
    struct is_input_source_trait_class_type : std::false_type
    { };

    template <template <typename...> typename TT, typename T, typename... Ts>
    struct is_input_source_trait_class_type<TT<T, Ts...>, std::void_t<
        std::enable_if_t<is_token_type_v<typename TT<T, Ts...>::char_type>>,
        std::enable_if_t<std::is_same_v<decltype(TT<T, Ts...>::look_ahead(std::declval<T&>())), typename TT<T, Ts...>::char_type>>,
        std::enable_if_t<std::is_same_v<decltype(TT<T, Ts...>::read(std::declval<T&>())), typename TT<T, Ts...>::char_type>>,
        std::enable_if_t<std::is_same_v<decltype(TT<T, Ts...>::is_end(std::declval<T&>())), bool>>
    >>
        : is_token_type<decltype(TT<T, Ts...>::read(std::declval<T&>()))>
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

    /*
    template <typename T1, typename T2, typename T3 = typename input_source_traits<T1>::char_type>
    struct is_compatible_input_source_type
        : std::conditional_t<
            is_input_source_type<T1>::value,
            is_compatible_comparison_type<T3, T2>,
            std::false_type
        >
    { };
    */

    template <typename T1>
    constexpr auto is_input_source_type_v = is_input_source_type<T1>::value;

    template <typename T1, typename T2>
    constexpr auto is_compatible_input_source_type_v =
        is_compatible_input_source_type<T1, T2>::value;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // bound functors
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1>
    struct bound_is
    {

    public:

        explicit constexpr bound_is(T1 cmp)
            : cmp{cmp}
        { }

        template <
            typename T2,
            typename = std::enable_if_t<is_compatible_input_source_type_v<T2, T1>>
        >
        constexpr auto operator()(T2& ins)
        {
            return is(ins, this->cmp);
        }

    private:

        T1 cmp;

    };

    template <typename T1>
    struct bound_is_not
    {

    public:

        explicit constexpr bound_is_not(T1 cmp)
            : cmp{cmp}
        { }

        template <
            typename T2,
            typename = std::enable_if_t<is_compatible_input_source_type_v<T2, T1>>
        >
        constexpr auto operator()(T2& ins)
        {
            return is_not(ins, this->cmp);
        }

    private:

        T1 cmp;

    };

    template <typename... Ts>
    struct bound_is_one_of
    {

    public:

        explicit constexpr bound_is_one_of(const Ts&... cmp)
            : cmp{cmp...}
        {
	}

        template <
            typename T,
            typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T, Ts>...>>
        >
        constexpr auto operator()(T& ins)
        {
            return (*this)(ins, std::index_sequence_for<Ts...>{});
        }

    private:

        template <
            typename T,
            std::size_t... I,
            typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T, Ts>...>>
        >
        constexpr auto operator()(T& ins, std::index_sequence<I...>)
        {
            return is_one_of(ins, std::get<I>(this->cmp)...);
        }

        std::tuple<Ts...> cmp;

    };

    template <typename... Ts>
    struct bound_is_none_of
    {

    public:

        explicit constexpr bound_is_none_of(Ts... cmp)
            : cmp{cmp...}
        { }

        template <
            typename T,
            typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T, Ts>...>>
        >
        constexpr auto operator()(T& ins)
        {
            return (*this)(ins, std::index_sequence_for<Ts...>{});
        }

    private:

        template <
            typename T,
            std::size_t... I,
            typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T, Ts>...>>
        >
        constexpr auto operator()(T& ins, std::index_sequence<I...>)
        {
            return is_none_of(ins, std::get<I>(this->cmp)...);
        }

        std::tuple<Ts...> cmp;

    };

    template <typename T>
    struct is_bound_functor;

    template <template <typename...> typename TT, typename... Ts>
    struct is_bound_functor<TT<Ts...>>
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<TT<Ts...>, bound_is<Ts...>>,
                std::is_same<TT<Ts...>, bound_is_not<Ts...>>,
                std::is_same<TT<Ts...>, bound_is_one_of<Ts...>>,
                std::is_same<TT<Ts...>, bound_is_none_of<Ts...>>
            >
        >
    { };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto is(T1& ins, T2 cmp)
    {
        if constexpr (is_character_type_v<T2>)
            return input_source_traits<T1>::look_ahead(ins) == cmp;
        else if constexpr (is_character_set_type_v<T2>)
            return cmp.contains(input_source_traits<T1>::look_ahead(ins));
        else if constexpr (std::is_same_v<T2, end_token>)
            return input_source_traits<T1>::is_end(ins);
        else if constexpr (std::is_same_v<T2, any_character>)
            return !input_source_traits<T1>::is_end(ins);
    }

    template <
        typename T,
        typename = std::enable_if_t<is_comparison_type_v<T>>
    >
    constexpr auto is(const T& cmp)
    {
        return bound_is{ cmp };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr bool is_not(T1& ins, const T2& cmp)
    {
        return !is(ins, cmp);
    }

    template <
        typename T,
        typename = std::enable_if_t<is_comparison_type_v<T>>
    >
    constexpr auto is_not(const T& cmp)
    {
        return bound_is_not{ cmp };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename... Ts,
        typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T1, Ts>...>>
    >
    constexpr bool is_one_of(T1& ins, const Ts&... cmp)
    {
        return (is(ins, cmp) || ...);
    }

    template <
        typename... Ts,
        typename = std::enable_if_t<std::conjunction_v<is_comparison_type<Ts>...>>
    >
    constexpr auto is_one_of(const Ts&... cmp)
    {
        return bound_is_one_of<Ts...>{ cmp... };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename... Ts,
        typename = std::enable_if_t<std::conjunction_v<is_compatible_input_source_type<T1, Ts>...>>
    >
    constexpr bool is_none_of(T1& ins, const Ts&... cmp)
    {
        return !is_one_of(ins, cmp...);
    }

    template <
        typename... Ts,
        typename = std::enable_if_t<std::conjunction_v<is_comparison_type<Ts>...>>
    >
    constexpr auto is_none_of(const Ts&... cmp)
    {
        return bound_is_none_of<Ts...>{ cmp... };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename = std::enable_if_t<is_input_source_type_v<T>>
    >
    constexpr auto read(T& ins)
    {
        if (input_source_traits<T>::is_end(ins))
            throw unexpected_input{};

        return ins.get();
    }

    template <
        typename T,
        typename = std::enable_if_t<is_input_source_type_v<T>>
    >
    constexpr auto read(T& ins, code_position& pos)
    {
        auto tok = read(ins);

        if (tok == tokens::line_feed)
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
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr std::optional<T1> read_if(T1& ins, const T2& cmp)
    {
        if (is(ins, cmp))
            return read(ins);
        else
            return std::nullopt;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr std::optional<T1> read_if(T1& ins, code_position& pos, const T2& cmp)
    {
        if (is(ins, cmp))
            return read(ins, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto read_while(T1& ins, const T2& cmp)
    {
        std::basic_string<T1> tokseq;

        while (is(ins, cmp))
            tokseq.push_back(read(ins));

        return tokseq;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto read_while(T1& ins, code_position& pos, const T2& cmp)
    {
        std::basic_string<T1> tokseq;

        while (is(ins, cmp))
            tokseq.push_back(read(ins, pos));

        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename = std::enable_if_t<is_input_source_type_v<T>>
    >
        constexpr void next(T& ins)
    {
        read(ins);
    }

    template <
        typename T,
        typename = std::enable_if_t<is_input_source_type_v<T>>
    >
    constexpr void next(T& ins, code_position& pos)
    {
        read(ins, pos);
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next(T1& ins, const T2& cmp)
    {
        if(is(ins, not_(cmp)))
            throw unexpected_input{};
        
        return read(ins);
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next(T1& ins, code_position& pos, const T2& cmp)
    {
        if(is(ins, not_(cmp)))
            throw unexpected_input{};

        return read(ins, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next_if(T1& ins, const T2& cmp)
    {
        return is(ins, cmp) ? next(ins), true : false;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next_if(T1& ins, code_position& pos, const T2& cmp)
    {
        return is(ins, cmp) ? next(ins, pos), true : false;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'next_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next_while(T1& ins, const T2& cmp)
    {
        unsigned count = 0;

        while (is(ins, cmp))
        {
            ignore(ins);
            count++;
        }

        return count;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_source_type_v<T1, T2>>
    >
    constexpr auto next_while(T1& ins, code_position& pos, const T2& cmp)
    {
        unsigned count = 0;

        while(is(ins, cmp))
        {
            next(ins, pos);
            count++;
        }

        return count;
    }
}
