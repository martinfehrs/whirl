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

        REQUIRE(LL1::is(ins,     'a'     ) == true );
        REQUIRE(LL1::is(ins,     'b'     ) == false);
        REQUIRE(LL1::is(ins,     eof     ) == false);
        REQUIRE(LL1::is(ins_eof, eof     ) == true );
        REQUIRE(LL1::is(ins_eof, 'a'     ) == false);

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

TEST_CASE( "testing is_not function overloads", "[is-not]" )
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


TEST_CASE( "testing is_one_of function overloads", "[is-one-of]" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::one_of('a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of('b')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof)) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof)) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of('a')) == false);
        
        REQUIRE(LL1::is(ins,     LL1::one_of('a', 'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of('b', 'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of('b', 'c')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of('a', eof)) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(eof, 'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of('b', eof)) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof, 'b')) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of('a', eof)) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof, 'a')) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of('a', 'b')) == false);
    }

    SECTION("wchar_t tests")
    {
        static constexpr auto eof = std::char_traits<wchar_t>::eof();
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::one_of(L'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(L'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof )) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(L'a')) == false);

        REQUIRE(LL1::is(ins,     LL1::one_of(L'a', L'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(L'b', L'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(L'b', L'c')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(L'a', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  L'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(L'b', eof )) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  L'b')) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(L'a', eof)) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof,  L'a')) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(L'a', L'b')) == false);
    }

    SECTION("char16_t tests")
    {
        static constexpr auto eof = std::char_traits<char16_t>::eof();
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::one_of(u'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(u'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof )) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(u'a')) == false);

        REQUIRE(LL1::is(ins,     LL1::one_of(u'a', u'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(u'b', u'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(u'b', u'c')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(u'a', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  u'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(u'b', eof )) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  u'b')) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(u'a', eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof,  u'a')) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(u'a', u'b')) == false);
    }

    SECTION("char32_t tests")
    {
        static constexpr auto eof = std::char_traits<char32_t>::eof();
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::one_of(U'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(U'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof )) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(U'a')) == false);

        REQUIRE(LL1::is(ins,     LL1::one_of(U'a', U'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(U'b', U'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(U'b', U'c')) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(U'a', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  U'a')) == true );
        REQUIRE(LL1::is(ins,     LL1::one_of(U'b', eof )) == false);
        REQUIRE(LL1::is(ins,     LL1::one_of(eof,  U'b')) == false);

        REQUIRE(LL1::is(ins_eof, LL1::one_of(U'a', eof )) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(eof,  U'a')) == true );
        REQUIRE(LL1::is(ins_eof, LL1::one_of(U'a', U'b')) == false);
    }
}

TEST_CASE( "testing is_none_of function overloads", "[is-none-of]" )
{
    SECTION("char tests")
    {
        static constexpr auto eof = std::char_traits<char>::eof();
        input_stream_dummy<char> ins('a');
        input_stream_dummy<char> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::none_of('a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of('b')) == true );

        REQUIRE(LL1::is(ins,     LL1::none_of(eof)) == true );
        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof)) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of('a')) == true );

        REQUIRE(LL1::is(ins,     LL1::none_of('a', 'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of('b', 'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of('b', 'c')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of('a', eof)) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(eof, 'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of('b', eof)) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof, 'b')) == true );


        REQUIRE(LL1::is(ins_eof, LL1::none_of('a', eof)) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof, 'a')) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of('a', 'b')) == true );

    }

    SECTION("wchar_t tests")
    {
        static constexpr auto eof = std::char_traits<wchar_t>::eof();
        input_stream_dummy<wchar_t> ins(L'a');
        input_stream_dummy<wchar_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::none_of(L'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(L'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof )) == true );

        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(L'a')) == true );

        REQUIRE(LL1::is(ins,     LL1::none_of(L'a', L'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(L'b', L'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(L'b', L'c')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(L'a', eof )) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  L'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(L'b', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  L'b')) == true );

        REQUIRE(LL1::is(ins_eof, LL1::none_of(L'a', eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof,  L'a')) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(L'a', L'b')) == true );

    }

    SECTION("char16_t tests")
    {
        static constexpr auto eof = std::char_traits<char16_t>::eof();
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::none_of(u'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(u'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof )) == true );

        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(u'a')) == true );

        REQUIRE(LL1::is(ins,     LL1::none_of(u'a', u'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(u'b', u'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(u'b', u'c')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(u'a', eof )) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  u'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(u'b', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  u'b')) == true );


        REQUIRE(LL1::is(ins_eof, LL1::none_of(u'a', eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof,  u'a')) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(u'a', u'b')) == true );
    }

    SECTION("char32_t tests")
    {
        static constexpr auto eof = std::char_traits<char32_t>::eof();
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(eof);

        REQUIRE(LL1::is(ins,     LL1::none_of(U'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(U'b')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof )) == true );

        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(U'a')) == true );

        REQUIRE(LL1::is(ins,     LL1::none_of(U'a', U'b')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(U'b', U'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(U'b', U'c')) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  U'a')) == false);
        REQUIRE(LL1::is(ins,     LL1::none_of(U'b', eof )) == true );
        REQUIRE(LL1::is(ins,     LL1::none_of(eof,  U'b')) == true );

        REQUIRE(LL1::is(ins_eof, LL1::none_of(U'a', eof )) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(eof,  U'a')) == false);
        REQUIRE(LL1::is(ins_eof, LL1::none_of(U'a', U'b')) == true );
    }
}



