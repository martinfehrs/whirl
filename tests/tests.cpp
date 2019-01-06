#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LL1.hpp"
#include "sequential.hpp"


namespace LL1
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// testing utilities
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct bound_predicate_dummy
    {
        bool operator()(const std::istream&);
    };

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

        constexpr void ignore() noexcept
        {
        }

        typename std::char_traits<T>::int_type tok;

    };

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

        static constexpr void ignore(input_stream_dummy<T>& ins) noexcept
        {
            ins.ignore();
        }

        static constexpr bool is_end(input_stream_dummy<T>& ins) noexcept
        {
            return ins.peek() == std::char_traits<T>::eof();
        }
    };

    using bound_predicate_conjunction_dummy_t =
        bound_predicate_conjunction<bound_predicate_dummy, bound_predicate_dummy>;

    using bound_predicate_disjunction_dummy_t =
        bound_predicate_disjunction<bound_predicate_dummy, bound_predicate_dummy>;


////////////////////////////////////////////////////////////////////////////////////////////////////
// compile-time checks
////////////////////////////////////////////////////////////////////////////////////////////////////

    static_assert(is_character_type<char>::value);
    static_assert(is_character_type<wchar_t>::value);
    static_assert(is_character_type<char16_t>::value);
    static_assert(is_character_type<char32_t>::value);

    static_assert(are_character_types<char, wchar_t, char16_t, char32_t>::value);

    static_assert(equality_comparable<int, int>::value);

    static_assert(is_compatible_character_type<char, char>::value);
    static_assert(is_compatible_character_type<wchar_t, wchar_t>::value);
    static_assert(is_compatible_character_type<char16_t, char16_t>::value);
    static_assert(is_compatible_character_type<char32_t, char32_t>::value);

    static_assert(is_character_type_v<char>);
    static_assert(is_character_type_v<wchar_t>);
    static_assert(is_character_type_v<char16_t>);
    static_assert(is_character_type_v<char32_t>);

    static_assert(are_character_types_v<char, wchar_t, char16_t, char32_t>);

    static_assert(is_compatible_character_type_v<char, char>);
    static_assert(is_compatible_character_type_v<wchar_t, wchar_t>);
    static_assert(is_compatible_character_type_v<char16_t, char16_t>);
    static_assert(is_compatible_character_type_v<char32_t, char32_t>);

    static_assert(is_input_source_type<std::basic_istream<char>>::value);
    static_assert(is_input_source_type<std::basic_istream<wchar_t>>::value);

    static_assert(is_input_source_type_v<std::basic_istream<char>>);
    static_assert(is_input_source_type_v<std::basic_istream<wchar_t>>);
    static_assert(is_input_source_type_v<std::basic_istream<char16_t>>);
    static_assert(is_input_source_type_v<std::basic_istream<char32_t>>);

    static_assert(is_compatible_input_source_type<std::basic_istream<char>, char>::value);
    static_assert(is_compatible_input_source_type<std::basic_istream<wchar_t>, wchar_t>::value);
    static_assert(is_compatible_input_source_type<std::basic_istream<char16_t>, char16_t>::value);
    static_assert(is_compatible_input_source_type<std::basic_istream<char32_t>, char32_t>::value);

    static_assert(is_compatible_input_source_type_v<std::istream, char>);
    static_assert(is_compatible_input_source_type_v<std::wistream, wchar_t>);

    static_assert(detail::is_bound_predicate_impl<bound_predicate_dummy, char>::value);
    static_assert(is_bound_predicate<bound_predicate_dummy>::value);
    static_assert(is_bound_predicate_v<bound_predicate_dummy>);

    static_assert(is_bound_predicate_v<bound_is_predicate<char>>);
    static_assert(is_bound_predicate_v<bound_is_predicate<wchar_t>>);
    static_assert(is_bound_predicate_v<bound_is_predicate<char16_t>>);
    static_assert(is_bound_predicate_v<bound_is_predicate<char32_t>>);
    static_assert(is_bound_predicate_v<bound_predicate_conjunction_dummy_t>);
    static_assert(is_bound_predicate_v<bound_predicate_disjunction_dummy_t>);


////////////////////////////////////////////////////////////////////////////////////////////////////
// run-time checks
////////////////////////////////////////////////////////////////////////////////////////////////////

    TEST_CASE( "testing is function overloads", "[is]" )
    {
        SECTION("char tests")
        {
            input_stream_dummy<char> ins('a');
            input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

            REQUIRE((is('a')(ins    )) == true );
            REQUIRE((is('b')(ins    )) == false);
            REQUIRE((is('a')(ins_eof)) == false);
        }

        SECTION("wchar_t tests")
        {
            input_stream_dummy<wchar_t> ins(L'a');
            input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

            REQUIRE((is(L'a')(ins    )) == true );
            REQUIRE((is(L'b')(ins    )) == false);
            REQUIRE((is(L'a')(ins_eof)) == false);
        }

        SECTION("char16_t tests")
        {
            input_stream_dummy<char16_t> ins(u'a');
            input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

            REQUIRE((is(u'a')(ins    )) == true );
            REQUIRE((is(u'b')(ins    )) == false);
            REQUIRE((is(u'a')(ins_eof)) == false);
        }

        SECTION("char32_t tests")
        {
            input_stream_dummy<char32_t> ins(U'a');
            input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

            REQUIRE((is(U'a')(ins    )) == true );
            REQUIRE((is(U'b')(ins    )) == false);
            REQUIRE((is(U'a')(ins_eof)) == false);
        }
    }

    TEST_CASE( "testing is_not function overloads", "[is-not]" )
    {
        SECTION("char tests")
        {
            input_stream_dummy<char> ins('a');
            input_stream_dummy<char> ins_eof(std::char_traits<char>::eof());

            REQUIRE((is_not('a')(ins    )) == false);
            REQUIRE((is_not('b')(ins    )) == true );
            REQUIRE((is_not('a')(ins_eof)) == true );
        }

        SECTION("wchar_t tests")
        {
            input_stream_dummy<wchar_t> ins(L'a');
            input_stream_dummy<wchar_t> ins_eof(std::char_traits<wchar_t>::eof());

            REQUIRE((is_not(L'a')(ins    )) == false);
            REQUIRE((is_not(L'b')(ins    )) == true );
            REQUIRE((is_not(L'a')(ins_eof)) == true );
        }

        SECTION("char16_t tests")
        {
            input_stream_dummy<char16_t> ins(u'a');
            input_stream_dummy<char16_t> ins_eof(std::char_traits<char16_t>::eof());

            REQUIRE((is_not(u'a')(ins    )) == false);
            REQUIRE((is_not(u'b')(ins    )) == true );
            REQUIRE((is_not(u'a')(ins_eof)) == true );
        }

        SECTION("char32_t tests")
        {
            input_stream_dummy<char32_t> ins(U'a');
            input_stream_dummy<char32_t> ins_eof(std::char_traits<char32_t>::eof());

            REQUIRE((is_not(U'a')(ins    )) == false);
            REQUIRE((is_not(U'b')(ins    )) == true );
            REQUIRE((is_not(U'a')(ins_eof)) == true );
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

    TEST_CASE("testing sequential example", "[sequential]")
    {
        SECTION("valid input")
        {
            static constexpr int expected_result[] = {
                -3, -3, -2, -1, 0, -1, 0, 1, 1, 2, 2, 2, 4, 5, 8
            };

            LL1::code_position pos{ 1, 1 };

            std::ifstream ifs("sequential.inp");

            REQUIRE(ifs.is_open());

            const auto result = sequential::read_data(ifs, pos);

            REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected_result)));
        }

        SECTION("invalid input")
        {
            LL1::code_position pos{ 1, 1 };

            std::ifstream ifs("sequential_invalid.inp");

            REQUIRE_THROWS(sequential::read_data(ifs, pos));
        }

        SECTION("empty input")
        {
            LL1::code_position pos{ 1, 1 };

            std::istringstream iss;

            const auto result = sequential::read_data(iss, pos);

            REQUIRE(result.empty());
        }
    }

}