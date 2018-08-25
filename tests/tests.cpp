#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LL1.hpp"


using namespace LL1;


template <typename T>
struct input_stream_dummy
{
    constexpr input_stream_dummy(typename std::char_traits<T>::int_type tok)
        : tok{ tok }
    { }

    constexpr auto peek() noexcept
    {
        return tok;
    }

    constexpr auto get() noexcept
    {
        return tok;
    }

    typename std::char_traits<T>::int_type tok;

};


namespace LL1
{
    template <typename T>
    struct input_source_traits<input_stream_dummy<T>>
    {
        using char_type = T;

        static char_type look_ahead(input_stream_dummy<T>& ins) noexcept
        {
            return ins.peek();
        }

        static constexpr char_type read(input_stream_dummy<T>& ins) noexcept
        {
            return ins.get();
        }

        static constexpr bool is_end(input_stream_dummy<T>& ins) noexcept
        {
            return ins.peek() == std::char_traits<T>::eof();
        }
    };
}


TEST_CASE( "testing is function overloads", "[is]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE(is(ins,     'a'      ) == true );
        REQUIRE(is(ins,     'b'      ) == false);
        REQUIRE(is(ins,     character) == true );
        REQUIRE(is(ins,     end      ) == false);
        REQUIRE(is(ins_eof, 'a'      ) == false);
        REQUIRE(is(ins_eof, character) == false);
        REQUIRE(is(ins_eof, end      ) == true );
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE(is(ins,     L'a'     ) == true );
        REQUIRE(is(ins,     L'b'     ) == false);
        REQUIRE(is(ins,     character) == true );
        REQUIRE(is(ins,     end      ) == false);
        REQUIRE(is(ins_eof, L'a'     ) == false);
        REQUIRE(is(ins_eof, character) == false);
        REQUIRE(is(ins_eof, end      ) == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE(is(ins,     u'a') == true );
        REQUIRE(is(ins,     u'b') == false);
        REQUIRE(is(ins,     end ) == false);
        REQUIRE(is(ins_eof, end ) == true );
        REQUIRE(is(ins_eof, u'a') == false);
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE(is(ins,     U'a') == true );
        REQUIRE(is(ins,     U'b') == false);
        REQUIRE(is(ins,     end ) == false);
        REQUIRE(is(ins_eof, end ) == true );
        REQUIRE(is(ins_eof, U'a') == false);
    }
}

TEST_CASE( "testing is_not function overloads", "[is-not]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE(is(ins,     not_('a')) == false);
        REQUIRE(is(ins,     not_('b')) == true );
        REQUIRE(is(ins,     not_(end)) == true );
        REQUIRE(is(ins_eof, not_(end)) == false);
        REQUIRE(is(ins_eof, not_('a')) == true );
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE(is(ins,     not_(L'a')) == false);
        REQUIRE(is(ins,     not_(L'b')) == true );
        REQUIRE(is(ins,     not_(end )) == true );
        REQUIRE(is(ins_eof, not_(end )) == false);
        REQUIRE(is(ins_eof, not_(L'a')) == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE(is(ins,     not_(u'a')) == false);
        REQUIRE(is(ins,     not_(u'b')) == true );
        REQUIRE(is(ins,     not_(end )) == true );
        REQUIRE(is(ins_eof, not_(end )) == false);
        REQUIRE(is(ins_eof, not_(u'a')) == true );
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE(is(ins,     not_(U'a')) == false);
        REQUIRE(is(ins,     not_(U'b')) == true );
        REQUIRE(is(ins,     not_(end )) == true );
        REQUIRE(is(ins_eof, not_(end )) == false);
        REQUIRE(is(ins_eof, not_(U'a')) == true );
    }
}


TEST_CASE( "testing is_one_of function overloads", "[is-one-of]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE(is(ins,     one_of('a')) == true );
        REQUIRE(is(ins,     one_of('b')) == false);

        REQUIRE(is(ins_eof, one_of('a')) == false);
        
        REQUIRE(is(ins,     one_of('a', 'b')) == true );
        REQUIRE(is(ins,     one_of('b', 'a')) == true );
        REQUIRE(is(ins,     one_of('b', 'c')) == false);

        REQUIRE(is(ins_eof, one_of('a', 'b')) == false);
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE(is(ins,     one_of(L'a')) == true );
        REQUIRE(is(ins,     one_of(L'b')) == false);

        REQUIRE(is(ins_eof, one_of(L'a')) == false);

        REQUIRE(is(ins,     one_of(L'a', L'b')) == true );
        REQUIRE(is(ins,     one_of(L'b', L'a')) == true );
        REQUIRE(is(ins,     one_of(L'b', L'c')) == false);

        REQUIRE(is(ins_eof, one_of(L'a', L'b')) == false);
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE(is(ins,     one_of(u'a')) == true );
        REQUIRE(is(ins,     one_of(u'b')) == false);

        REQUIRE(is(ins_eof, one_of(u'a')) == false);

        REQUIRE(is(ins,     one_of(u'a', u'b')) == true );
        REQUIRE(is(ins,     one_of(u'b', u'a')) == true );
        REQUIRE(is(ins,     one_of(u'b', u'c')) == false);

        REQUIRE(is(ins_eof, one_of(u'a', u'b')) == false);
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE(is(ins,     one_of(U'a')) == true );
        REQUIRE(is(ins,     one_of(U'b')) == false);

        REQUIRE(is(ins_eof, one_of(U'a')) == false);

        REQUIRE(is(ins,     one_of(U'a', U'b')) == true );
        REQUIRE(is(ins,     one_of(U'b', U'a')) == true );
        REQUIRE(is(ins,     one_of(U'b', U'c')) == false);

        REQUIRE(is(ins_eof, one_of(U'a', U'b')) == false);
    }
}

TEST_CASE( "testing is_none_of function overloads", "[is-none-of]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE(is(ins,     none_of('a')) == false);
        REQUIRE(is(ins,     none_of('b')) == true );

        REQUIRE(is(ins_eof, none_of('a')) == true );

        REQUIRE(is(ins,     none_of('a', 'b')) == false);
        REQUIRE(is(ins,     none_of('b', 'a')) == false);
        REQUIRE(is(ins,     none_of('b', 'c')) == true );

        REQUIRE(is(ins_eof, none_of('a', 'b')) == true );
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE(is(ins,     none_of(L'a')) == false);
        REQUIRE(is(ins,     none_of(L'b')) == true );

        REQUIRE(is(ins_eof, none_of(L'a')) == true );

        REQUIRE(is(ins,     none_of(L'a', L'b')) == false);
        REQUIRE(is(ins,     none_of(L'b', L'a')) == false);
        REQUIRE(is(ins,     none_of(L'b', L'c')) == true );

        REQUIRE(is(ins_eof, none_of(L'a', L'b')) == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE(is(ins,     none_of(u'a')) == false);
        REQUIRE(is(ins,     none_of(u'b')) == true );

        REQUIRE(is(ins_eof, none_of(u'a')) == true );

        REQUIRE(is(ins,     none_of(u'a', u'b')) == false);
        REQUIRE(is(ins,     none_of(u'b', u'a')) == false);
        REQUIRE(is(ins,     none_of(u'b', u'c')) == true );

        REQUIRE(is(ins_eof, none_of(u'a', u'b')) == true );
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE(is(ins,     none_of(U'a')) == false);
        REQUIRE(is(ins,     none_of(U'b')) == true );

        REQUIRE(is(ins_eof, none_of(U'a')) == true );

        REQUIRE(is(ins,     none_of(U'a', U'b')) == false);
        REQUIRE(is(ins,     none_of(U'b', U'a')) == false);
        REQUIRE(is(ins,     none_of(U'b', U'c')) == true );

        REQUIRE(is(ins_eof, none_of(U'a', U'b')) == true );
    }
}



