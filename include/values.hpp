#ifndef __VALUES_HPP__
#define __VALUES_HPP__


#include <ostream>


namespace whirl
{

    class Nothing {};

    enum class Sign
    {
        positive = 1,
        negative = -1
    };

    template <typename D>
    class Digit
    {

    public:

        constexpr Digit() noexcept
            : m_data{ 0 }
        { }

        explicit constexpr Digit(D data) noexcept
            : m_data{ data }
        { }

        constexpr auto data() const noexcept
        {
            return m_data;
        }

    private:

        D m_data;

    };

    template <typename D>
    class UnsignedNumber
    {

    public:

        constexpr UnsignedNumber() noexcept
            : m_data{ 0 }
        { }

        explicit constexpr UnsignedNumber(D data) noexcept
            : m_data{ data }
        { }

        constexpr auto data() const noexcept
        {
            return m_data;
        }

    private:

        D  m_data;

    };

    template <typename D>
    std::ostream& operator<<(std::ostream& os, UnsignedNumber<D> unum)
    {
        os << unum.data();

        return os;
    }


    template <typename D>
    class SignedNumber
    {

    public:

        constexpr SignedNumber() noexcept
            : m_data{ 0 }
        { }

        explicit constexpr SignedNumber(D data) noexcept
            : m_data{ data }
        { }

        constexpr auto data() const noexcept
        {
            return m_data;
        }

    private:

        D  m_data;

    };

    template <typename D>
    std::ostream& operator<<(std::ostream& os, SignedNumber<D> snum)
    {
        os << snum.data();

        return os;
    }

    template <typename D>
    constexpr bool operator==(SignedNumber<D> snum, D num) noexcept
    {
        return snum.data() == num;
    }


    constexpr auto concat(Nothing, Nothing) noexcept
    {
        return Nothing{};
    }

    template <typename T>
    constexpr auto concat(T val, Nothing) noexcept
    {
        return val;
    }

    template <typename T>
    constexpr auto concat(Nothing, T val) noexcept
    {
        return val;
    }

    template <typename T>
    constexpr auto concat(Sign sign, Digit<T> digit) noexcept
    {
        return SignedNumber<T>{ static_cast<T>(sign) * digit.data() };
    }

    template <typename T>
    constexpr auto concat(Digit<T> digit1, Digit<T> digit2) noexcept
    {
        return UnsignedNumber<T>{ digit1.data() * 10 + digit2.data() };
    }

    template <typename T>
    constexpr auto concat(UnsignedNumber<T> unum, Digit<T> digit) noexcept
    {
        return UnsignedNumber<T>{ unum.data() * 10 + digit.value() };
    }

    template <typename T>
    constexpr auto concat(SignedNumber<T> snum1, Digit<T> digit) noexcept
    {
        return SignedNumber<T>{ snum1.data() * 10 + digit.data() };
    }

    template <typename T>
    constexpr auto concat(Sign sign, UnsignedNumber<T> num) noexcept
    {
        return SignedNumber<T>{ static_cast<T>(sign) * num.data() };
    }

}


#endif /*__WHIRL_HPP__*/