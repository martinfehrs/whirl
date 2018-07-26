#include <istream>
#include <vector>
#include <optional>
#include <array>
#include <deque>
#include <algorithm>
#include <tuple>
#include <iostream>


namespace LL1
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // aliases, improving readability
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    using int_type_t = typename std::char_traits<T>::int_type;

    template <typename T>
    using optional_token_t = std::optional<int_type_t<T>>;

    template <typename T>
    using input_stream_t = std::basic_istream<T>;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // error handling
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct code_position
    {
        unsigned row;
        unsigned col;
    };

    struct unexpected_token
    {
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token sequences
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct dynamic_token_sequence
    {

    public:

        using int_type = int_type_t<T>;


        void push_back(int_type chr)
        {
            this->storage.push_back(chr);
        }

        dynamic_token_sequence& operator +=(int_type tok)
        {
            this->storage.push_back(tok);

            return *this;
        }

        dynamic_token_sequence& operator+=(const dynamic_token_sequence& other)
        {
            this->storage.insert(
                std::end(this->storage),
                std::begin(other.storage),
                std::end(other.storage)
            );

            return *this;
        }

        const int_type& operator[](size_t i) const
        {
            return this->storage[0];
        }

        auto size() const
        {
            return this->storage.size();
        }

    private:

        std::vector<int_type> storage;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // negated token
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct negated_token
    {
        explicit constexpr negated_token(T tok)
            : tok{tok}
        { }

        constexpr T token() const
        {
            return this->tok;
        }

    private:

        T tok;
    };

    template <typename T1, typename T2>
    constexpr bool operator==(negated_token<T1> tok1, negated_token<T2> tok2)
    {
        return tok1.token() == tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(negated_token<T1> tok1, negated_token<T2> tok2)
    {
        return tok1.token() != tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator==(negated_token<T1> tok1, T2 tok2)
    {
        return tok1.token() != tok2;
    }

    template <typename T1, typename T2>
    constexpr bool operator==(T1 tok1, negated_token<T2> tok2)
    {
        return tok1 != tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(T1 tok1, negated_token<T2> tok2)
    {
        return tok1 == tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(negated_token<T1> tok1, T2 tok2)
    {
        return tok1.token() == tok2;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_char_type
        : std::bool_constant<
        std::disjunction_v<
        std::is_same<T, char>,
        std::is_same<T, wchar_t>,
        std::is_same<T, char16_t>,
        std::is_same<T, char32_t>,
        std::is_same<T, negated_token<char>>,
        std::is_same<T, negated_token<wchar_t>>,
        std::is_same<T, negated_token<wchar_t>>,
        std::is_same<T, negated_token<char16_t>>,
        std::is_same<T, negated_token<char32_t>>
        >
        >
    { };

    template <typename T>
    struct is_int_type
        : std::bool_constant<
        std::disjunction_v<
        std::is_same<T, int_type_t<char>>,
        std::is_same<T, int_type_t<wchar_t>>,
        std::is_same<T, int_type_t<char16_t>>,
        std::is_same<T, int_type_t<char32_t>>,
        std::is_same<T, negated_token<int_type_t<char>>>,
        std::is_same<T, negated_token<int_type_t<wchar_t>>>,
        std::is_same<T, negated_token<int_type_t<wchar_t>>>,
        std::is_same<T, negated_token<int_type_t<char16_t>>>,
        std::is_same<T, negated_token<int_type_t<char32_t>>>
        >
        >
    { };

    template <typename T>
    struct is_token_type
        : std::bool_constant<
        std::disjunction_v<
        is_char_type<T>,
        is_int_type<T>
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
    struct equality_comparable: std::false_type
    { };

    template <typename T1, typename T2>
    struct equality_comparable<
        T1, T2, std::void_t<decltype(std::declval<T1>() == std::declval<T2>())>
    >: std::true_type
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

    template <typename T, typename... Ts>
    struct are_compatible_token_types
        : std::bool_constant<
        std::conjunction_v<
        is_compatible_token_type<T, Ts>...
        >
        >
    { };

    template <typename T>
    constexpr bool is_char_type_v = is_char_type<T>::value;

    template <typename T>
    constexpr bool is_int_type_v = is_int_type<T>::value;

    template <typename T>
    constexpr bool is_token_type_v = is_token_type<T>::value;

    template <typename... Ts>
    constexpr bool are_token_types_v = are_token_types<Ts...>::value;

    template <typename T1, typename T2>
    constexpr bool is_compatible_token_type_v = is_compatible_token_type<T1, T2>::value;

    template <typename... Ts>
    constexpr bool are_compatible_token_types_v = are_compatible_token_types<Ts...>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // negated token set
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct negated_token_set
    {

        explicit constexpr negated_token_set(const T& set)
            : set_{set}
        { }

        constexpr const auto set() const
        {
            return set_;
        }

    private:

        T set_;

    };

    template <typename T, size_t N>
    struct negated_token_set<T[N]>
    {

        explicit constexpr negated_token_set(const T(&set)[N])
        {
            std::copy_n(set, N, this->set_);
        }

        constexpr const auto& set() const
        {
            return set_;
        }

    private:

        T set_[N];

    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set search functions
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    constexpr bool disjunction(T expr)
    {
        return expr ? true : false;
    }

    template <typename T, typename... Ts>
    constexpr bool disjunction(T expr, Ts... exprs)
    {
        return disjunction(expr) || disjunction(exprs...);
    }

    template <
        typename T1,
        typename T2,
        size_t... I,
        typename = std::enable_if<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool contains_impl(const T1& set, T2 tok, std::index_sequence<I...>)
    {
        return disjunction((std::get<I>(set) == tok)...);
    }

    template <
        typename... Ts,
        typename T,
        typename = std::enable_if<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool contains(const std::tuple<Ts...>& set, T tok)
    {
        return contains_impl(set, tok, std::index_sequence_for<Ts...>{});
    }

    template <
        typename T1,
        size_t N,
        typename T2,
        typename = std::enable_if<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool contains(const std::array<T1, N>& set, T2 tok)
    {
        return contains_impl(set, tok, std::make_index_sequence<N>{});
    }

    template <
        typename T1,
        size_t N,
        typename T2,
        typename = std::enable_if<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool contains(const T1(&set)[N], T2 tok)
    {
        return std::find(std::begin(set), std::end(set), tok) != std::end(set);
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool contains(const negated_token_set<T1>& negated_set, T2 tok)
    {
        return !contains(negated_set.set(), tok);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool contains(const negated_token_set<T1[N]>& negated_set, T2 tok)
    {
        return !contains(negated_set.set(), tok);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_token_set_type: std::false_type
    { };

    template <typename... Ts>
    struct is_token_set_type<std::tuple<Ts...>>: std::true_type
    { };

    template <typename T1, typename T2>
    struct is_token_set_type<std::pair<T1, T2>>: std::true_type
    { };

    template <typename T, size_t N>
    struct is_token_set_type<std::array<T, N>>: std::true_type
    { };

    template <typename T, size_t N>
    struct is_token_set_type<T[N]>: std::true_type
    { };

    template <typename T>
    struct is_token_set_type<negated_token_set<T>>: std::true_type
    { };

    template <typename T1, typename T2, typename = void>
    struct is_compatible_token_set_type: std::false_type
    { };

    template <typename T1, typename T2>
    struct is_compatible_token_set_type<
        T1, T2, std::void_t<decltype(contains(std::declval<T2>(), std::declval<T1>()))>
    >: std::true_type
    { };

    template <typename T>
    constexpr bool is_token_set_type_v = is_token_set_type<T>::value;

    template <typename T1, typename T2>
    constexpr bool is_compatible_token_set_type_v = is_compatible_token_set_type<T1, T2>::value;


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
        constexpr char space[]{' ', '\t', '\n'};

        constexpr char blank[]{' ', '\t'};

        constexpr char digit[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // logical token and token set operations
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename = std::enable_if_t<is_token_type_v<T>>>
    constexpr auto not_(T tok)
    {
        return negated_token<T>{ tok };
    }

    template <typename T, typename = std::enable_if_t<is_token_set_type_v<T>>>
    constexpr auto not_(const T& set)
    {
        return negated_token_set<T>{ set };
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set factories
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename... Ts, typename = std::enable_if_t<are_token_types_v<Ts...>>>
    constexpr auto one_of(Ts... toks)
    {
        return std::make_tuple(toks...);
    }

    template <typename... Ts, typename = std::enable_if_t<are_token_types_v<Ts...>>>
    constexpr auto none_of(Ts... toks)
    {
        return not_(one_of(toks...));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // comparison type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1, typename T2>
    struct is_compatible_comparison_type
        : std::bool_constant<
        std::disjunction_v<
        std::conditional_t<
        is_token_type_v<T2>,
        is_compatible_token_type<T1, T2>,
        std::false_type
        >,
        is_compatible_token_set_type<T1, T2>
        >
        >
    { };

    template <typename T1, typename T2>
    constexpr bool is_compatible_comparison_type_v = is_compatible_comparison_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // input steam type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1, typename = void>
    struct is_compatible_input_stream_type: std::false_type
    { };

    template <typename T1>
    struct is_compatible_input_stream_type<T1, std::void_t<decltype(std::declval<T1>().get())>>
        : is_int_type<decltype(std::declval<T1>().get())>::type
    { };

    template <typename T1>
    constexpr bool is_compatible_input_stream_type_v = is_compatible_input_stream_type<T1>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_token_type_v<T1>>,
        typename = std::enable_if_t<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool is(T1 tok, T2 cmp) noexcept
    {
        return tok == cmp;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_token_type_v<T1>>,
        typename = std::enable_if_t<is_compatible_token_set_type_v<T1, T2>>
    >
    constexpr bool is(T1 tok, const T2& cmp) noexcept
    {
        return contains(cmp, tok);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr bool is(TT<T1>& bis, const T2& cmp)
    {
        return is(bis.peek(), cmp);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T>>>,
        typename = std::enable_if_t<is_token_type_v<T>>
    >
    constexpr auto read(TT<T>& bis)
    {
        return bis.get();
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T>>>,
        typename = std::enable_if_t<is_token_type_v<T>>
    >
    constexpr auto read(TT<T>& bis, code_position& pos)
    {
        auto tok = bis.get();

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
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, const T2& cmp)
    {
        if (is(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        if (is(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, const T2& cmp)
    {
        dynamic_token_sequence<T1> tokseq;

        while (is(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        dynamic_token_sequence<T1> tokseq;

        while (is(bis, cmp))
            tokseq.push_back(read(bis, pos));

        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T>>>,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
    constexpr void ignore(TT<T>& bis)
    {
        read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T>>>,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
    constexpr void ignore(TT<T>& bis, code_position& pos)
    {
        read(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto ignore_if(TT<T1>& bis, const T2& cmp)
    {
        return is(bis, cmp) ? ignore(bis), true : false;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto ignore_if(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        return is(bis, cmp) ? ignore(bis, pos), true : false;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto ignore_while(TT<T1>& bis, const T2& cmp)
    {
        unsigned count = 0;

        while (is(bis, cmp))
        {
            ignore(bis);
            count++;
        }

        return count;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto ignore_while(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        unsigned count = 0;

        while(is(bis, cmp))
        {
            ignore(bis, pos);
            count++;
        }

        return count;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, const T2& cmp)
    {
        if(is(bis, not_(cmp)))
            throw unexpected_token{};
        
        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, code_position& pos, const T2& cmp)
    {    
        if(is(bis, not_(cmp)))
            throw unexpected_token{};

        return read(bis, pos);
    }

}
