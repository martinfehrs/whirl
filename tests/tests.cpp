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

        REQUIRE(is(ins,     'a'        ) == true );
        REQUIRE(is(ins,     'b'        ) == false);
        REQUIRE(is(ins,     character  ) == true );
        REQUIRE(is(ins,     end        ) == false);
        REQUIRE(is(ins_eof, 'a'        ) == false);
        REQUIRE(is(ins_eof, character  ) == false);
        REQUIRE(is(ins_eof, end        ) == true );

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

        REQUIRE(is(ins,     L'a'       ) == true );
        REQUIRE(is(ins,     L'b'       ) == false);
        REQUIRE(is(ins,     character  ) == true );
        REQUIRE(is(ins,     end        ) == false);
        REQUIRE(is(ins_eof, L'a'       ) == false);
        REQUIRE(is(ins_eof, character  ) == false);
        REQUIRE(is(ins_eof, end        ) == true );

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

        REQUIRE(is(ins,     u'a'       ) == true );
        REQUIRE(is(ins,     u'b'       ) == false);
        REQUIRE(is(ins,     character  ) == true );
        REQUIRE(is(ins,     end        ) == false);
        REQUIRE(is(ins_eof, u'a'       ) == false);
        REQUIRE(is(ins_eof, character  ) == false);
        REQUIRE(is(ins_eof, end        ) == true );

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

        REQUIRE(is(ins,     U'a'       ) == true );
        REQUIRE(is(ins,     U'b'       ) == false);
        REQUIRE(is(ins,     character  ) == true );
        REQUIRE(is(ins,     end        ) == false);
        REQUIRE(is(ins_eof, U'a'       ) == false);
        REQUIRE(is(ins_eof, character  ) == false);
        REQUIRE(is(ins_eof, end        ) == true );

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

        REQUIRE(is_not(ins,     'a'        ) == false);
        REQUIRE(is_not(ins,     'b'        ) == true );
        REQUIRE(is_not(ins,     character  ) == false);
        REQUIRE(is_not(ins,     end        ) == true );
        REQUIRE(is_not(ins_eof, 'a'        ) == true );
        REQUIRE(is_not(ins_eof, character  ) == true );
        REQUIRE(is_not(ins_eof, end        ) == false);

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

        REQUIRE(is_not(ins,     L'a'        ) == false);
        REQUIRE(is_not(ins,     L'b'        ) == true );
        REQUIRE(is_not(ins,     character   ) == false);
        REQUIRE(is_not(ins,     end         ) == true );
        REQUIRE(is_not(ins_eof, L'a'        ) == true );
        REQUIRE(is_not(ins_eof, character   ) == true );
        REQUIRE(is_not(ins_eof, end         ) == false);

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

        REQUIRE(is_not(ins,     u'a'        ) == false);
        REQUIRE(is_not(ins,     u'b'        ) == true );
        REQUIRE(is_not(ins,     character   ) == false);
        REQUIRE(is_not(ins,     end         ) == true );
        REQUIRE(is_not(ins_eof, u'a'        ) == true );
        REQUIRE(is_not(ins_eof, character   ) == true );
        REQUIRE(is_not(ins_eof, end         ) == false);

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

        REQUIRE(is_not(ins,     U'a'        ) == false);
        REQUIRE(is_not(ins,     U'b'        ) == true );
        REQUIRE(is_not(ins,     character   ) == false);
        REQUIRE(is_not(ins,     end         ) == true );
        REQUIRE(is_not(ins_eof, U'a'        ) == true );
        REQUIRE(is_not(ins_eof, character   ) == true );
        REQUIRE(is_not(ins_eof, end         ) == false);

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

        REQUIRE(is_one_of(ins,     'a'     ) == true );
        REQUIRE(is_one_of(ins,     'b'     ) == false);
        REQUIRE(is_one_of(ins,     'a', 'b') == true );
        REQUIRE(is_one_of(ins,     'b', 'a') == true );
        REQUIRE(is_one_of(ins,     'b', 'c') == false);
        REQUIRE(is_one_of(ins_eof, 'a'     ) == false);
        REQUIRE(is_one_of(ins_eof, 'a', 'b') == false);

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

        REQUIRE(is_one_of(ins,     L'a'      ) == true );
        REQUIRE(is_one_of(ins,     L'b'      ) == false);
        REQUIRE(is_one_of(ins,     L'a', L'b') == true );
        REQUIRE(is_one_of(ins,     L'b', L'a') == true );
        REQUIRE(is_one_of(ins,     L'b', L'c') == false);
        REQUIRE(is_one_of(ins_eof, L'a'      ) == false);
        REQUIRE(is_one_of(ins_eof, L'a', L'b') == false);

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

        REQUIRE(is_one_of(ins,     u'a'      ) == true );
        REQUIRE(is_one_of(ins,     u'b'      ) == false);
        REQUIRE(is_one_of(ins,     u'a', u'b') == true );
        REQUIRE(is_one_of(ins,     u'b', u'a') == true );
        REQUIRE(is_one_of(ins,     u'b', u'c') == false);
        REQUIRE(is_one_of(ins_eof, u'a'      ) == false);
        REQUIRE(is_one_of(ins_eof, u'a', u'b') == false);

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

        REQUIRE(is_one_of(ins,     U'a'      ) == true );
        REQUIRE(is_one_of(ins,     U'b'      ) == false);
        REQUIRE(is_one_of(ins,     U'a', U'b') == true );
        REQUIRE(is_one_of(ins,     U'b', U'a') == true );
        REQUIRE(is_one_of(ins,     U'b', U'c') == false);
        REQUIRE(is_one_of(ins_eof, U'a'      ) == false);
        REQUIRE(is_one_of(ins_eof, U'a', U'b') == false);

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

        REQUIRE(is_none_of(ins,     'a'     ) == false);
        REQUIRE(is_none_of(ins,     'b'     ) == true );
        REQUIRE(is_none_of(ins,     'a', 'b') == false);
        REQUIRE(is_none_of(ins,     'b', 'a') == false);
        REQUIRE(is_none_of(ins,     'b', 'c') == true );
        REQUIRE(is_none_of(ins_eof, 'a'     ) == true );
        REQUIRE(is_none_of(ins_eof, 'a', 'b') == true );

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

        REQUIRE(is_none_of(ins,     L'a'      ) == false);
        REQUIRE(is_none_of(ins,     L'b'      ) == true );
        REQUIRE(is_none_of(ins,     L'a', L'b') == false);
        REQUIRE(is_none_of(ins,     L'b', L'a') == false);
        REQUIRE(is_none_of(ins,     L'b', L'c') == true );
        REQUIRE(is_none_of(ins_eof, L'a'      ) == true );
        REQUIRE(is_none_of(ins_eof, L'a', L'b') == true );
    }

    SECTION("char16_t tests")
    {
        input_stream_dummy<char16_t> ins(u'a');
        input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

        REQUIRE(is_none_of(ins,     u'a'      ) == false);
        REQUIRE(is_none_of(ins,     u'b'      ) == true );
        REQUIRE(is_none_of(ins,     u'a', u'b') == false);
        REQUIRE(is_none_of(ins,     u'b', u'a') == false);
        REQUIRE(is_none_of(ins,     u'b', u'c') == true );
        REQUIRE(is_none_of(ins_eof, u'a'      ) == true );
        REQUIRE(is_none_of(ins_eof, u'a', u'b') == true );
    }

    SECTION("char32_t tests")
    {
        input_stream_dummy<char32_t> ins(U'a');
        input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

        REQUIRE(is_none_of(ins,     U'a'      ) == false);
        REQUIRE(is_none_of(ins,     U'b'      ) == true );
        REQUIRE(is_none_of(ins,     U'a', U'b') == false);
        REQUIRE(is_none_of(ins,     U'b', U'a') == false);
        REQUIRE(is_none_of(ins,     U'b', U'c') == true );
        REQUIRE(is_none_of(ins_eof, U'a'      ) == true );
        REQUIRE(is_none_of(ins_eof, U'a', U'b') == true );
    }
}



