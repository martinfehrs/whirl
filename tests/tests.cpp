#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LLk.hpp"


template <typename T>
struct input_stream_dummy
{
	T c;

	constexpr auto peek() noexcept
	{
		return c;
	}

	constexpr auto get() noexcept
	{
		return c;
	}
};

template <typename T>
constexpr input_stream_dummy<T> make_input_stream_dummy(T chr) noexcept
{
	return { chr };
}

//TEST_CASE("testing static_look_ahead_buffer", "static_look_ahead_buffer") {
//
//    LL1::static_look_ahead_buffer<char, 1> buf1;
//
//    buf1[0] = 'a';
//
//    SECTION("subscribtion operator") {
//        REQUIRE(buf1[0] == 'a');
//    }
//
//    SECTION("size tests") {
//        REQUIRE(LLk::static_look_ahead_buffer<char, 1>{}.size() == 0);
//        REQUIRE(LLk::static_look_ahead_buffer<char, 1>{'a'}.size() == 1);
//        REQUIRE(LLk::static_look_ahead_buffer<char, 2>{}.size() == 0);
//        REQUIRE(LLk::static_look_ahead_buffer<char, 2>{'a'}.size() == 1);
//        REQUIRE(LL1::static_look_ahead_buffer<char, 2>{'a', 'b'}.size() == 2);
//    }
//
//}

TEST_CASE( "testing is function overloads", "[is]" ) {

	SECTION("char tests") {
		static constexpr auto eof = std::char_traits<char>::eof();
		auto ins = make_input_stream_dummy('a');
		auto ins_eof = make_input_stream_dummy(eof);
        //LL1::static_look_ahead_buffer<char, 1> buf;

		REQUIRE(LLk::is('a',     'a'     ) == true );
		REQUIRE(LLk::is('a',     'b'     ) == false);
		REQUIRE(LLk::is(eof,     eof     ) == true );
		REQUIRE(LLk::is('a',     eof     ) == false);
		REQUIRE(LLk::is(eof,     'a'     ) == false);
		REQUIRE(LLk::is(ins,     'a'     ) == true );
		REQUIRE(LLk::is(ins,     'b'     ) == false);
		REQUIRE(LLk::is(ins,     eof     ) == false);
		REQUIRE(LLk::is(ins_eof, eof     ) == true );
		REQUIRE(LLk::is(ins_eof, 'a'     ) == false);
		//REQUIRE(LLk::is(ins,     buf, 'a') == true );
		//REQUIRE(LLk::is(ins,     buf, 'b') == false);
		//REQUIRE(LLk::is(ins,     buf, 'a') == true );
		//REQUIRE(LLk::is(ins,     buf, eof) == false);
		//REQUIRE(LLk::is(ins_eof, buf, eof) == true );
		//REQUIRE(LLk::is(ins_eof, buf, 'a') == false);
	}

	SECTION("wchar_t tests") {
		constexpr auto eof = std::char_traits<wchar_t>::eof();
		auto ins = make_input_stream_dummy(L'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is(L'a',    L'a') == true );
		REQUIRE(LLk::is(L'a',    L'b') == false);
		REQUIRE(LLk::is(eof,     eof ) == true );
		REQUIRE(LLk::is(L'a',    eof ) == false);
		REQUIRE(LLk::is(eof,     L'a') == false);
		REQUIRE(LLk::is(ins,     L'a') == true );
		REQUIRE(LLk::is(ins,     L'b') == false);
		REQUIRE(LLk::is(ins,     eof ) == false);
		REQUIRE(LLk::is(ins_eof, eof ) == true );
		REQUIRE(LLk::is(ins_eof, L'a') == false);
	}

	SECTION("char16_t tests") {
		constexpr auto eof = std::char_traits<char16_t>::eof();
		auto ins = make_input_stream_dummy(u'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE( LLk::is(u'a',    u'a') == true );
		REQUIRE(LLk::is(u'a',    u'b') == false);
		REQUIRE(LLk::is(eof,     eof ) == true );
		REQUIRE(LLk::is(u'a',    eof ) == false);
		REQUIRE(LLk::is(eof,     u'a') == false);
		REQUIRE(LLk::is(ins,     u'a') == true );
		REQUIRE(LLk::is(ins,     u'b') == false);
		REQUIRE(LLk::is(ins,     eof ) == false);
		REQUIRE(LLk::is(ins_eof, eof ) == true );
		REQUIRE(LLk::is(ins_eof, u'a') == false);
	}

	SECTION("char32_t tests") {
		constexpr auto eof = std::char_traits<char32_t>::eof();
		auto ins = make_input_stream_dummy(U'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is(U'a',    U'a') == true );
		REQUIRE(LLk::is(U'a',    U'b') == false);
		REQUIRE(LLk::is(eof,     eof ) == true );
		REQUIRE(LLk::is(U'a',    eof ) == false);
		REQUIRE(LLk::is(eof,     U'a') == false);
		REQUIRE(LLk::is(ins,     U'a') == true );
		REQUIRE(LLk::is(ins,     U'b') == false);
		REQUIRE(LLk::is(ins,     eof ) == false);
		REQUIRE(LLk::is(ins_eof, eof ) == true );
		REQUIRE(LLk::is(ins_eof, U'a') == false);
	}
}

TEST_CASE( "testing is_not function overloads", "[is_not]" ) {

	SECTION("char tests") {
		static constexpr auto eof = std::char_traits<char>::eof();
		auto ins = make_input_stream_dummy('a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_not('a',     'a') == false);
		REQUIRE(LLk::is_not('a',     'b') == true );
		REQUIRE(LLk::is_not(eof,     eof) == false);
		REQUIRE(LLk::is_not('a',     eof) == true );
		REQUIRE(LLk::is_not(eof,     'a') == true );
		REQUIRE(LLk::is_not(ins,     'a') == false);
		REQUIRE(LLk::is_not(ins,     'b') == true );
		REQUIRE(LLk::is_not(ins,     eof) == true );
		REQUIRE(LLk::is_not(ins_eof, eof) == false);
		REQUIRE(LLk::is_not(ins_eof, 'a') == true );
	}

	SECTION("wchar_t tests") {
		constexpr auto eof = std::char_traits<wchar_t>::eof();
		auto ins = make_input_stream_dummy(L'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_not(L'a',    L'a') == false);
		REQUIRE(LLk::is_not(L'a',    L'b') == true );
		REQUIRE(LLk::is_not(eof,     eof ) == false);
		REQUIRE(LLk::is_not(L'a',    eof ) == true );
		REQUIRE(LLk::is_not(eof,     L'a') == true );
		REQUIRE(LLk::is_not(ins,     L'a') == false);
		REQUIRE(LLk::is_not(ins,     L'b') == true );
		REQUIRE(LLk::is_not(ins,     eof ) == true );
		REQUIRE(LLk::is_not(ins_eof, eof ) == false);
		REQUIRE(LLk::is_not(ins_eof, L'a') == true );
	}

	SECTION("char16_t tests") {
		constexpr auto eof = std::char_traits<char16_t>::eof();
		auto ins = make_input_stream_dummy(u'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_not(u'a',    u'a') == false);
		REQUIRE(LLk::is_not(u'a',    u'b') == true );
		REQUIRE(LLk::is_not(eof,     eof ) == false);
		REQUIRE(LLk::is_not(u'a',    eof ) == true );
		REQUIRE(LLk::is_not(eof,     u'a') == true );
		REQUIRE(LLk::is_not(ins,     u'a') == false);
		REQUIRE(LLk::is_not(ins,     u'b') == true );
		REQUIRE(LLk::is_not(ins,     eof ) == true );
		REQUIRE(LLk::is_not(ins_eof, eof ) == false);
		REQUIRE(LLk::is_not(ins_eof, u'a') == true );
	}

	SECTION("char32_t tests") {
		constexpr auto eof = std::char_traits<char32_t>::eof();
		auto ins = make_input_stream_dummy(U'a');
		auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_not(U'a',    U'a') == false);
		REQUIRE(LLk::is_not(U'a',    U'b') == true );
		REQUIRE(LLk::is_not(eof,     eof ) == false);
		REQUIRE(LLk::is_not(U'a',    eof ) == true );
		REQUIRE(LLk::is_not(eof,     U'a') == true );
		REQUIRE(LLk::is_not(ins,     U'a') == false);
		REQUIRE(LLk::is_not(ins,     U'b') == true );
		REQUIRE(LLk::is_not(ins,     eof ) == true );
		REQUIRE(LLk::is_not(ins_eof, eof ) == false);
		REQUIRE(LLk::is_not(ins_eof, U'a') == true );
	}
}

TEST_CASE( "testing is_one_of function overloads", "[is_one_of]" ) {

	SECTION("char tests") {
		static constexpr auto eof = std::char_traits<char>::eof();
		//auto ins = make_input_stream_dummy('a');
		//auto ins_eof = make_input_stream_dummy(eof);
		//auto tok_set = make_token_set

		REQUIRE(LLk::is_one_of('a', 'a')      == true );
		REQUIRE(LLk::is_one_of('a', 'b')      == false);
		REQUIRE(LLk::is_one_of('a', 'a', 'b') == true );
		REQUIRE(LLk::is_one_of('a', 'b', 'a') == true );
		REQUIRE(LLk::is_one_of('a', 'b', 'c') == false);
		REQUIRE(LLk::is_one_of(eof, eof)      == true );
		REQUIRE(LLk::is_one_of(eof, 'a')      == false);
		REQUIRE(LLk::is_one_of('a', eof)      == false);
		REQUIRE(LLk::is_one_of(eof, 'a', eof) == true );
		REQUIRE(LLk::is_one_of(eof, eof, 'a') == true );
	}

	SECTION("wchar_t tests") {
		//constexpr auto eof = std::char_traits<wchar_t>::eof();
		//auto ins = make_input_stream_dummy(L'a');
		//auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_one_of(L'a', L'a'      ) == true );
		REQUIRE(LLk::is_one_of(L'a', L'b'      ) == false);
		REQUIRE(LLk::is_one_of(L'a', L'a', L'b') == true );
		REQUIRE(LLk::is_one_of(L'a', L'b', L'a') == true );
		REQUIRE(LLk::is_one_of(L'a', L'b', L'c') == false);
	}

	SECTION("char16_t tests") {
		//constexpr auto eof = std::char_traits<char16_t>::eof();
		//auto ins = make_input_stream_dummy(u'a');
		//auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_one_of(u'a', u'a'      ) == true );
		REQUIRE(LLk::is_one_of(u'a', u'b'      ) == false);
		REQUIRE(LLk::is_one_of(u'a', u'a', u'b') == true );
		REQUIRE(LLk::is_one_of(u'a', u'b', u'a') == true );
		REQUIRE(LLk::is_one_of(u'a', u'b', u'c') == false);
	}

	SECTION("char32_t tests") {
		//constexpr auto eof = std::char_traits<char32_t>::eof();
		//auto ins = make_input_stream_dummy(U'a');
		//auto ins_eof = make_input_stream_dummy(eof);

		REQUIRE(LLk::is_one_of(U'a', U'a'      ) == true );
		REQUIRE(LLk::is_one_of(U'a', U'b'      ) == false);
		REQUIRE(LLk::is_one_of(U'a', U'a', U'b') == true );
		REQUIRE(LLk::is_one_of(U'a', U'b', U'a') == true );
		REQUIRE(LLk::is_one_of(U'a', U'b', U'c') == false);
	}
}

