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

        REQUIRE((is('a'      )(ins    )) == true );
        REQUIRE((is('b'      )(ins    )) == false);
        REQUIRE((is(character)(ins    )) == true );
        REQUIRE((is(end      )(ins    )) == false);
        REQUIRE((is('a'      )(ins_eof)) == false);
        REQUIRE((is(character)(ins_eof)) == false);
        REQUIRE((is(end      )(ins_eof)) == true );
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE((is(L'a'     )(ins    )) == true );
        REQUIRE((is(L'b'     )(ins    )) == false);
        REQUIRE((is(character)(ins    )) == true );
        REQUIRE((is(end      )(ins    )) == false);
        REQUIRE((is(L'a'     )(ins_eof)) == false);
        REQUIRE((is(character)(ins_eof)) == false);
        REQUIRE((is(end      )(ins_eof)) == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE((is(u'a'     )(ins    )) == true );
        REQUIRE((is(u'b'     )(ins    )) == false);
        REQUIRE((is(character)(ins    )) == true );
        REQUIRE((is(end      )(ins    )) == false);
        REQUIRE((is(u'a'     )(ins_eof)) == false);
        REQUIRE((is(character)(ins_eof)) == false);
        REQUIRE((is(end      )(ins_eof)) == true );
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE((is(U'a'     )(ins    )) == true );
        REQUIRE((is(U'b'     )(ins    )) == false);
        REQUIRE((is(character)(ins    )) == true );
        REQUIRE((is(end      )(ins    )) == false);
        REQUIRE((is(U'a'     )(ins_eof)) == false);
        REQUIRE((is(character)(ins_eof)) == false);
        REQUIRE((is(end      )(ins_eof)) == true );
    }
}

TEST_CASE( "testing is_not function overloads", "[is-not]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE((is_not('a'      )(ins    )) == false);
        REQUIRE((is_not('b'      )(ins    )) == true );
        REQUIRE((is_not(character)(ins    )) == false);
        REQUIRE((is_not(end      )(ins    )) == true );
        REQUIRE((is_not('a'      )(ins_eof)) == true );
        REQUIRE((is_not(character)(ins_eof)) == true );
        REQUIRE((is_not(end      )(ins_eof)) == false);
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE((is_not(L'a'      )(ins    )) == false);
        REQUIRE((is_not(L'b'      )(ins    )) == true );
        REQUIRE((is_not(character)(ins     )) == false);
        REQUIRE((is_not(end       )(ins    )) == true );
        REQUIRE((is_not(L'a'      )(ins_eof)) == true );
        REQUIRE((is_not(character )(ins_eof)) == true );
        REQUIRE((is_not(end       )(ins_eof)) == false);
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE((is_not(u'a'      )(ins    )) == false);
        REQUIRE((is_not(u'b'      )(ins    )) == true );
        REQUIRE((is_not(character )(ins    )) == false);
        REQUIRE((is_not(end       )(ins    )) == true );
        REQUIRE((is_not(u'a'      )(ins_eof)) == true );
        REQUIRE((is_not(character )(ins_eof)) == true );
        REQUIRE((is_not(end       )(ins_eof)) == false);
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE((is_not(U'a'      )(ins    )) == false);
        REQUIRE((is_not(U'b'      )(ins    )) == true );
        REQUIRE((is_not(character )(ins    )) == false);
        REQUIRE((is_not(end       )(ins    )) == true );
        REQUIRE((is_not(U'a'      )(ins_eof)) == true );
        REQUIRE((is_not(character )(ins_eof)) == true );
        REQUIRE((is_not(end       )(ins_eof)) == false);
    }
}


TEST_CASE( "testing is_one_of function overloads", "[is-one-of]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE((is_one_of('a'     )(ins    )) == true );
        REQUIRE((is_one_of('b'     )(ins    )) == false);
        REQUIRE((is_one_of('a', 'b')(ins    )) == true );
        REQUIRE((is_one_of('b', 'a')(ins    )) == true );
        REQUIRE((is_one_of('b', 'c')(ins    )) == false);
        REQUIRE((is_one_of('a'     )(ins_eof)) == false);
        REQUIRE((is_one_of('a', 'b')(ins_eof)) == false);


    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE((is_one_of(L'a'      )(ins    )) == true );
        REQUIRE((is_one_of(L'b'      )(ins    )) == false);
        REQUIRE((is_one_of(L'a', L'b')(ins    )) == true );
        REQUIRE((is_one_of(L'b', L'a')(ins    )) == true );
        REQUIRE((is_one_of(L'b', L'c')(ins    )) == false);
        REQUIRE((is_one_of(L'a'      )(ins_eof)) == false);
        REQUIRE((is_one_of(L'a', L'b')(ins_eof)) == false);
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE((is_one_of(u'a'      )(ins    )) == true );
        REQUIRE((is_one_of(u'b'      )(ins    )) == false);
        REQUIRE((is_one_of(u'a', u'b')(ins    )) == true );
        REQUIRE((is_one_of(u'b', u'a')(ins    )) == true );
        REQUIRE((is_one_of(u'b', u'c')(ins    )) == false);
        REQUIRE((is_one_of(u'a'      )(ins_eof)) == false);
        REQUIRE((is_one_of(u'a', u'b')(ins_eof)) == false);
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE((is_one_of(U'a'      )(ins    )) == true );
        REQUIRE((is_one_of(U'b'      )(ins    )) == false);
        REQUIRE((is_one_of(U'a', U'b')(ins    )) == true );
        REQUIRE((is_one_of(U'b', U'a')(ins    )) == true );
        REQUIRE((is_one_of(U'b', U'c')(ins    )) == false);
        REQUIRE((is_one_of(U'a'      )(ins_eof)) == false);
        REQUIRE((is_one_of(U'a', U'b')(ins_eof)) == false);
    }
}

TEST_CASE( "testing is_none_of function overloads", "[is-none-of]" )
{
    SECTION("char tests")
    {
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

        REQUIRE((is_none_of('a'     )(ins    ))  == false);
        REQUIRE((is_none_of('b'     )(ins    ))  == true );
        REQUIRE((is_none_of('a', 'b')(ins    ))  == false);
        REQUIRE((is_none_of('b', 'a')(ins    ))  == false);
        REQUIRE((is_none_of('b', 'c')(ins    ))  == true );
        REQUIRE((is_none_of('a'     )(ins_eof))  == true );
        REQUIRE((is_none_of('a', 'b')(ins_eof))  == true );
    }

    SECTION("wchar_t tests")
    {
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

        REQUIRE((is_none_of(L'a'      )(ins    ))  == false);
        REQUIRE((is_none_of(L'b'      )(ins    ))  == true );
        REQUIRE((is_none_of(L'a', L'b')(ins    ))  == false);
        REQUIRE((is_none_of(L'b', L'a')(ins    ))  == false);
        REQUIRE((is_none_of(L'b', L'c')(ins    ))  == true );
        REQUIRE((is_none_of(L'a'      )(ins_eof))  == true );
        REQUIRE((is_none_of(L'a', L'b')(ins_eof))  == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE((is_none_of(u'a'      )(ins    ))  == false);
        REQUIRE((is_none_of(u'b'      )(ins    ))  == true );
        REQUIRE((is_none_of(u'a', u'b')(ins    ))  == false);
        REQUIRE((is_none_of(u'b', u'a')(ins    ))  == false);
        REQUIRE((is_none_of(u'b', u'c')(ins    ))  == true );
        REQUIRE((is_none_of(u'a'      )(ins_eof))  == true );
        REQUIRE((is_none_of(u'a', u'b')(ins_eof))  == true );
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE((is_none_of(U'a'      )(ins    ))  == false);
        REQUIRE((is_none_of(U'b'      )(ins    ))  == true );
        REQUIRE((is_none_of(U'a', U'b')(ins    ))  == false);
        REQUIRE((is_none_of(U'b', U'a')(ins    ))  == false);
        REQUIRE((is_none_of(U'b', U'c')(ins    ))  == true );
        REQUIRE((is_none_of(U'a'      )(ins_eof))  == true );
        REQUIRE((is_none_of(U'a', U'b')(ins_eof))  == true );
    }
}



