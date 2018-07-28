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

TEST_CASE( "testing is function overloads", "is" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);
        //LL1::static_look_ahead_buffer<char, 1> buf;

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

        REQUIRE(LL1::is(ins,     U'a') == true );
        REQUIRE(LL1::is(ins,     U'b') == false);
        REQUIRE(LL1::is(ins,     eof ) == false);
        REQUIRE(LL1::is(ins_eof, eof ) == true );
        REQUIRE(LL1::is(ins_eof, U'a') == false);
    }
}

TEST_CASE( "testing is_not function overloads", "is-not" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::not_('a')) == false);
        REQUIRE(LL1::is(ins,     LL1::not_('b')) == true );
        REQUIRE(LL1::is(ins,     LL1::not_(eof)) == true );
        REQUIRE(LL1::is(ins_eof, LL1::not_(eof)) == false);
        REQUIRE(LL1::is(ins_eof, LL1::not_('a')) == true );
    }

    SECTION("wchar_t tests")
    {
        constexpr auto eof = std::char_traits<wchar_t>::eof();
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::not_(L'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::not_(L'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::not_(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::not_(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::not_(L'a')) == true );
    }

    SECTION("char16_t tests")
    {
        constexpr auto eof = std::char_traits<char16_t>::eof();
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::not_(u'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::not_(u'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::not_(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::not_(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::not_(u'a')) == true );
    }

    SECTION("char32_t tests") {
        constexpr auto eof = std::char_traits<char32_t>::eof();
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::not_(U'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::not_(U'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::not_(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::not_(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::not_(U'a')) == true );
    }
}


TEST_CASE( "testing is_one_of function overloads", "is-one-of" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        //auto ins = make_input_stream_dummy('a');
        //auto ins_eof = make_input_stream_dummy(eof);
        //auto tok_set = make_token_set

    }

    SECTION("wchar_t tests")
    {
        //constexpr auto eof = std::char_traits<wchar_t>::eof();
        //auto ins = make_input_stream_dummy(L'a');
        //auto ins_eof = make_input_stream_dummy(eof);
    }

    SECTION("char16_t tests")
    {
        //constexpr auto eof = std::char_traits<char16_t>::eof();
        //auto ins = make_input_stream_dummy(u'a');
        //auto ins_eof = make_input_stream_dummy(eof);
    }

    SECTION("char32_t tests")
    {
        //constexpr auto eof = std::char_traits<char32_t>::eof();
        //auto ins = make_input_stream_dummy(U'a');
        //auto ins_eof = make_input_stream_dummy(eof);
    }
}

