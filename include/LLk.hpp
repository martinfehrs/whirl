#include <istream>
#include <vector>
#include <optional>
#include <array>
#include <deque>
#include <algorithm>

#include <iostream>

namespace LLk
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // aliases, improving readability
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    using int_type_t = typename std::char_traits<T>::int_type;

    template <typename T>
    using optional_token_t = std::optional<int_type_t<T>>;

    template <typename T, size_t N>
    using token_set_t = std::array<T, N>;

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
    // look ahead buffers
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct dynamic_look_ahead_buffer
    {

    public:

        using int_type = int_type_t<T>;


        void push_back(int_type chr)
        {
            this->storage.push_back(chr);
        }

        int_type pop_front()
        {
            auto tok = this->storage.front();
            this->storage.pop_back();
            return tok;
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

        std::deque<int_type> storage;

    };

    template <typename T, size_t N>
    struct static_look_ahead_buffer
    {

        using int_type = int_type_t<T>;


        static_look_ahead_buffer() = default;

        template <typename... Ts>
        constexpr static_look_ahead_buffer(Ts... toks)
            : storage{toks...}
        { }

        constexpr void push_back(int_type tok)
        {
            this->end_index++;
            this->storage[this->end_index] = tok;
        }

        constexpr int_type pop_front()
        {
            auto tok = this->storage[this->begin_index];
            this->begin_index++;

            return tok;
        }

        constexpr const int_type& operator[](size_t i) const noexcept
        {
            return this->storage[(this->begin_index + i) % N];
        }

        constexpr int_type& operator[](size_t i) noexcept
        {
            return this->storage[(this->begin_index + i) % N];
        }

        constexpr size_t size() const noexcept
        {
            return end_index - begin_index;
        }

    private:

        int_type storage[N];
        int begin_index = -1;
        int end_index = -1;

    };

    template <typename T>
    struct static_look_ahead_buffer<T, 1>
    {

        using int_type = int_type_t<T>;


        static_look_ahead_buffer() = default;

        constexpr static_look_ahead_buffer(int_type tok)
            : tok{tok}
            , ptr{&this->tok}
        { }

        constexpr static_look_ahead_buffer(const static_look_ahead_buffer& other)
            : tok{other.tok}
            , ptr{other.ptr}
        {
        }

        constexpr static_look_ahead_buffer& operator=(static_look_ahead_buffer& other)
        {
            this->tok = other.tok;
            this->ptr = other.ptr;

            return *this;
        }

        constexpr void push_back(int_type tok)
        {
            this->tok = tok;
            this->ptr = &this->tok;
        }

        constexpr int_type pop_front()
        {
            this->ptr = nullptr;
            return tok;
        }

        constexpr const int_type& operator[](size_t i) const noexcept
        {
            return *ptr;
        }

        constexpr size_t size() const noexcept
        {
            return ptr ? 1 : 0;
        }

    private:

        int_type tok;
        int_type* ptr = nullptr;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_char_type
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
    struct is_int_type
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<T, int_type_t<char>>,
                std::is_same<T, int_type_t<wchar_t>>,
                std::is_same<T, int_type_t<char16_t>>,
                std::is_same<T, int_type_t<char32_t>>
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

    template <typename T1, typename T2>
    struct is_compatible_token_type
        : std::bool_constant<
            std::conjunction_v<
                are_token_types<T1, T2>,
                std::disjunction<
                    std::is_same<T1, T2>,
                    std::is_same<T1, int_type_t<T2>>,
                    std::is_same<int_type_t<T1>, T2>
                >
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

    template <typename... Ts>
    constexpr bool are_compatible_token_types_v = are_compatible_token_types<Ts...>::value;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined tokens
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    constexpr T lf_v = 0xA;

    template <typename T>
    constexpr T cr_v = 0xD;

    template <typename T>
    constexpr T space_v = 0x20;

    template <typename T>
    constexpr T tab_v = 0x9;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined token sets
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    constexpr token_set_t<T, 3> space{ ' ', '\t', '\n' };

    template <typename T>
    constexpr token_set_t<T, 2> blank{ ' ', '\t' };

    template <typename T>
    constexpr token_set_t<T, 10> digit{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is(T1 tok, T2 cmp) noexcept
    {
        return tok == cmp;
    }

    template <
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is(T1 tok, const token_set_t<T2, N>& cmp) noexcept
    {
        return std::find(std::begin(cmp), std::end(cmp), tok) != std::end(cmp);
    }
    
    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is(TT<T1>& bis, T2 cmp)
    {
        return is(bis.peek(), cmp);
    }

    //template <
    //    template <typename...> typename TT1,
    //    template <typename...> typename TT2,
    //    typename T1,
    //    typename T2,
    //    typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    //>
    //constexpr bool is(TT1<T1>& bis, TT2<T1>& buf, T2 cmp)
    //{
    //    buf.push_back(bis.get());
    //
    //    return is(buf[0], cmp);
    //}

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        return std::find(std::begin(cmp), std::end(cmp), bis.peek()) != std::end(cmp);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_not(T1 tok, T2 cmp) noexcept
    {
        return !is(tok, cmp);
    }

    template <
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_not(T1 tok, const token_set_t<T2, N>& cmp) noexcept
    {
        return !is(tok, cmp);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_not(TT<T1>& bis, T2 cmp)
    {
        return !is(bis, cmp);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        return !is(bis, cmp);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_one_of(T1 tok, T2 cmp1) noexcept
    {
        return is(tok, cmp1);
    }

    template <
        typename T1,
        typename T2,
        typename... Ts,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2, Ts...>>
    >
    constexpr bool is_one_of(T1 tok, T2 cmp1, Ts... cmp) noexcept
    {
        return is(tok, cmp1) || is_one_of(tok, cmp...);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_one_of(TT<T>& bis, Ts... cmp)
    {
        return is_one_of(bis.peek(), cmp...);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_none_of(T tok, Ts... cmp) noexcept
    {
        return !is_one_of(tok, cmp...);
    }

    template <
    template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_none_of(TT<T>& bis, Ts... cmp)
    {
        return !is_one_of(bis, cmp...);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
    constexpr auto read(TT<T>& bis)
    {
        return bis.get();
    }

    template <
       template <typename...> typename TT,
       typename T,
       typename = std::enable_if_t<are_token_types_v<T>>
    >
    constexpr auto read(TT<T>& bis, code_position& pos)
    {
       auto tok = bis.get();
    
       if(tok == lf_v<T>)
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
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, T2 cmp)
    {
        if(is(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        if(is(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, T2 cmp)
    {
        if(is_not(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is_not(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            return read(bis, pos);
        else
            return std::nullopt;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_none_of(TT<T>& bis, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_none_of(bis, pos, cmp...))
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
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, T2 cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, code_position& pos, T2 cmp)
    {        
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis, pos));
            
        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis, pos));
            
        return tokseq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, T2 cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis, pos));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis, pos));

        return tokseq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_one_of(TT<T>& bis, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
        
        while(is_one_of(bis, cmp...))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
        
        while(is_one_of(bis, cmp...))
            tokseq.push_back(read(bis, pos));
            
        return tokseq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
       template <typename...> typename TT,
       typename T,
       typename... Ts,
       typename = std::enable_if_t<sizeof...(Ts) != 0>,
       typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_none_of(TT<T>& bis, Ts... cmp)
    {
       dynamic_token_sequence<T> tokseq;

       while(is_none_of(bis, cmp...))
           tokseq.push_back(read(bis));

       return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
    
        while(is_none_of(bis, cmp...))
            tokseq.push_back(read(bis, pos));
    
        return tokseq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
       constexpr void ignore(TT<T>& bis)
    {
        read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
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
       typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if(TT<T1>& bis, T2 cmp)
    {
       if(is(bis, cmp))
           ignore(bis);
    }

    template <
       template <typename...> typename TT,
       typename T1,
       typename T2,
       size_t N,
       typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
       if(is(bis, cmp))
           ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is(bis, cmp))
            ignore(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is(bis, cmp))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if_not(TT<T1>& bis, T2 cmp)
    {
        if(is_not(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            ignore(bis);
    
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if_not(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is_not(bis, cmp))
            ignore(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_if_not(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////    

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            ignore(bis, pos);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_none_of(input_stream_t<T>& bis, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, T2 cmp)
    {
        while(is(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        while(is(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        while(is(bis, cmp))
            ignore(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        while(is(bis, cmp))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, T2 cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_one_of(TT<T>& bis, Ts... cmp)
    {
        while(is_one_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        while(is_one_of(bis, cmp...))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_none_of(TT<T>& bis, Ts... cmp)
    {
        while(is_none_of(bis, cmp...))
            ignore(bis);
    }
    
    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        while(is_none_of(bis, cmp...))
            ignore(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, T2 cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};
        
        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, T2 cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, code_position& pos, T2 cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        size_t N,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, code_position& pos, const token_set_t<T2, N>& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);    
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_not_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_not_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_none_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis, pos);
    }

}



