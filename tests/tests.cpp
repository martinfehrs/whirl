#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LL1.hpp"


template <typename T>
struct input_stream_dummy
{
    constexpr input_stream_dummy(LL1::int_type_t<T> c)
        : c{ c }
    { }

    constexpr auto peek() noexcept
    {
        return c;
    }

    constexpr auto get() noexcept
    {
        return c;
    }

    LL1::int_type_t<T> c;

};

TEST_CASE( "testing is function overloads", "[is]" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);
        //LL1::static_look_ahead_buffer<char, 1> buf;

        REQUIRE(LL1::is('a',     'a'     ) == true );
        REQUIRE(LL1::is('a',     'b'     ) == false);
        REQUIRE(LL1::is(eof,     eof     ) == true );
        REQUIRE(LL1::is('a',     eof     ) == false);
        REQUIRE(LL1::is(eof,     'a'     ) == false);
        REQUIRE(LL1::is(ins,     'a'     ) == true );
        REQUIRE(LL1::is(ins,     'b'     ) == false);
        REQUIRE(LL1::is(ins,     eof     ) == false);
        REQUIRE(LL1::is(ins_eof, eof     ) == true );
        REQUIRE(LL1::is(ins_eof, 'a'     ) == false);
        //REQUIRE(LL1::is(ins,     buf, 'a') == true );
        //REQUIRE(LL1::is(ins,     buf, 'b') == false);
        //REQUIRE(LL1::is(ins,     buf, 'a') == true );
        //REQUIRE(LL1::is(ins,     buf, eof) == false);
        //REQUIRE(LL1::is(ins_eof, buf, eof) == true );
        //REQUIRE(LL1::is(ins_eof, buf, 'a') == false);
    }

    SECTION("wchar_t tests")
    {
        constexpr auto eof = std::char_traits<wchar_t>::eof();
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(eof);

        REQUIRE(LL1::is(L'a',    L'a') == true );
        REQUIRE(LL1::is(L'a',    L'b') == false);
        REQUIRE(LL1::is(eof,     eof ) == true );
        REQUIRE(LL1::is(L'a',    eof ) == false);
        REQUIRE(LL1::is(eof,     L'a') == false);
        REQUIRE(LL1::is(ins,     L'a') == true );
        REQUIRE(LL1::is(ins,     L'b') == false);
        REQUIRE(LL1::is(ins,     eof ) == false);
        REQUIRE(LL1::is(ins_eof, eof ) == true );
        REQUIRE(LL1::is(ins_eof, L'a') == false);
    }

    SECTION("char16_t tests")
    {
        constexpr auto eof = std::char_traits<char16_t>::eof();
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(eof);

        REQUIRE( LL1::is(u'a',   u'a') == true );
        REQUIRE(LL1::is(u'a',    u'b') == false);
        REQUIRE(LL1::is(eof,     eof ) == true );
        REQUIRE(LL1::is(u'a',    eof ) == false);
        REQUIRE(LL1::is(eof,     u'a') == false);
        REQUIRE(LL1::is(ins,     u'a') == true );
        REQUIRE(LL1::is(ins,     u'b') == false);
        REQUIRE(LL1::is(ins,     eof ) == false);
        REQUIRE(LL1::is(ins_eof, eof ) == true );
        REQUIRE(LL1::is(ins_eof, u'a') == false);
    }

    SECTION("char32_t tests")
    {
        constexpr auto eof = std::char_traits<char32_t>::eof();
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(eof);

        REQUIRE(LL1::is(U'a',    U'a') == true );
        REQUIRE(LL1::is(U'a',    U'b') == false);
        REQUIRE(LL1::is(eof,     eof ) == true );
        REQUIRE(LL1::is(U'a',    eof ) == false);
        REQUIRE(LL1::is(eof,     U'a') == false);
        REQUIRE(LL1::is(ins,     U'a') == true );
        REQUIRE(LL1::is(ins,     U'b') == false);
        REQUIRE(LL1::is(ins,     eof ) == false);
        REQUIRE(LL1::is(ins_eof, eof ) == true );
        REQUIRE(LL1::is(ins_eof, U'a') == false);
    }
}

TEST_CASE( "testing is_not function overloads", "[is_not]" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);

        REQUIRE(LL1::is_not('a',     'a') == false);
        REQUIRE(LL1::is_not('a',     'b') == true );
        REQUIRE(LL1::is_not(eof,     eof) == false);
        REQUIRE(LL1::is_not('a',     eof) == true );
        REQUIRE(LL1::is_not(eof,     'a') == true );
        REQUIRE(LL1::is_not(ins,     'a') == false);
        REQUIRE(LL1::is_not(ins,     'b') == true );
        REQUIRE(LL1::is_not(ins,     eof) == true );
        REQUIRE(LL1::is_not(ins_eof, eof) == false);
        REQUIRE(LL1::is_not(ins_eof, 'a') == true );
    }

    SECTION("wchar_t tests")
    {
        constexpr auto eof = std::char_traits<wchar_t>::eof();
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(eof);

        REQUIRE(LL1::is_not(L'a',    L'a') == false);
        REQUIRE(LL1::is_not(L'a',    L'b') == true );
        REQUIRE(LL1::is_not(eof,     eof ) == false);
        REQUIRE(LL1::is_not(L'a',    eof ) == true );
        REQUIRE(LL1::is_not(eof,     L'a') == true );
        REQUIRE(LL1::is_not(ins,     L'a') == false);
        REQUIRE(LL1::is_not(ins,     L'b') == true );
        REQUIRE(LL1::is_not(ins,     eof ) == true );
        REQUIRE(LL1::is_not(ins_eof, eof ) == false);
        REQUIRE(LL1::is_not(ins_eof, L'a') == true );
    }

    SECTION("char16_t tests")
    {
        constexpr auto eof = std::char_traits<char16_t>::eof();
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(eof);

        REQUIRE(LL1::is_not(u'a',    u'a') == false);
        REQUIRE(LL1::is_not(u'a',    u'b') == true );
        REQUIRE(LL1::is_not(eof,     eof ) == false);
        REQUIRE(LL1::is_not(u'a',    eof ) == true );
        REQUIRE(LL1::is_not(eof,     u'a') == true );
        REQUIRE(LL1::is_not(ins,     u'a') == false);
        REQUIRE(LL1::is_not(ins,     u'b') == true );
        REQUIRE(LL1::is_not(ins,     eof ) == true );
        REQUIRE(LL1::is_not(ins_eof, eof ) == false);
        REQUIRE(LL1::is_not(ins_eof, u'a') == true );
    }

    SECTION("char32_t tests") {
        constexpr auto eof = std::char_traits<char32_t>::eof();
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(eof);

        REQUIRE(LL1::is_not(U'a',    U'a') == false);
        REQUIRE(LL1::is_not(U'a',    U'b') == true );
        REQUIRE(LL1::is_not(eof,     eof ) == false);
        REQUIRE(LL1::is_not(U'a',    eof ) == true );
        REQUIRE(LL1::is_not(eof,     U'a') == true );
        REQUIRE(LL1::is_not(ins,     U'a') == false);
        REQUIRE(LL1::is_not(ins,     U'b') == true );
        REQUIRE(LL1::is_not(ins,     eof ) == true );
        REQUIRE(LL1::is_not(ins_eof, eof ) == false);
        REQUIRE(LL1::is_not(ins_eof, U'a') == true );
    }
}

TEST_CASE( "testing is_one_of function overloads", "[is_one_of]" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        //auto ins = make_input_stream_dummy('a');
        //auto ins_eof = make_input_stream_dummy(eof);
        //auto tok_set = make_token_set

        REQUIRE(LL1::is_one_of('a', 'a')      == true );
        REQUIRE(LL1::is_one_of('a', 'b')      == false);
        REQUIRE(LL1::is_one_of('a', 'a', 'b') == true );
        REQUIRE(LL1::is_one_of('a', 'b', 'a') == true );
        REQUIRE(LL1::is_one_of('a', 'b', 'c') == false);
        REQUIRE(LL1::is_one_of(eof, eof)      == true );
        REQUIRE(LL1::is_one_of(eof, 'a')      == false);
        REQUIRE(LL1::is_one_of('a', eof)      == false);
        REQUIRE(LL1::is_one_of(eof, 'a', eof) == true );
        REQUIRE(LL1::is_one_of(eof, eof, 'a') == true );
    }

    SECTION("wchar_t tests")
    {
        //constexpr auto eof = std::char_traits<wchar_t>::eof();
        //auto ins = make_input_stream_dummy(L'a');
        //auto ins_eof = make_input_stream_dummy(eof);

        REQUIRE(LL1::is_one_of(L'a', L'a'      ) == true );
        REQUIRE(LL1::is_one_of(L'a', L'b'      ) == false);
        REQUIRE(LL1::is_one_of(L'a', L'a', L'b') == true );
        REQUIRE(LL1::is_one_of(L'a', L'b', L'a') == true );
        REQUIRE(LL1::is_one_of(L'a', L'b', L'c') == false);
    }

    SECTION("char16_t tests")
    {
        //constexpr auto eof = std::char_traits<char16_t>::eof();
        //auto ins = make_input_stream_dummy(u'a');
        //auto ins_eof = make_input_stream_dummy(eof);

        REQUIRE(LL1::is_one_of(u'a', u'a'      ) == true );
        REQUIRE(LL1::is_one_of(u'a', u'b'      ) == false);
        REQUIRE(LL1::is_one_of(u'a', u'a', u'b') == true );
        REQUIRE(LL1::is_one_of(u'a', u'b', u'a') == true );
        REQUIRE(LL1::is_one_of(u'a', u'b', u'c') == false);
    }

    SECTION("char32_t tests")
    {
        //constexpr auto eof = std::char_traits<char32_t>::eof();
        //auto ins = make_input_stream_dummy(U'a');
        //auto ins_eof = make_input_stream_dummy(eof);

        REQUIRE(LL1::is_one_of(U'a', U'a'      ) == true );
        REQUIRE(LL1::is_one_of(U'a', U'b'      ) == false);
        REQUIRE(LL1::is_one_of(U'a', U'a', U'b') == true );
        REQUIRE(LL1::is_one_of(U'a', U'b', U'a') == true );
        REQUIRE(LL1::is_one_of(U'a', U'b', U'c') == false);
    }
}

