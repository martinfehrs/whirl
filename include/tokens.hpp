#ifndef __TOKENS_HPP__
#define __TOKENS_HPP__


#include <ostream>


namespace whirl
{

    enum class Sign
    {
        positive = 1,
        negative = -1
    };

    template <typename V>
    class SignedDigitSequence
    {

    public:

        constexpr SignedDigitSequence() noexcept
            : m_count{ 0 }
            , m_value{ 0 }
        { }

        explicit constexpr SignedDigitSequence(V value) noexcept
            : m_count{ 1 }
            , m_value{ value }
        { }

        constexpr SignedDigitSequence(int count, V value) noexcept
            : m_count{ count }
            , m_value{ value }
        { }

        constexpr auto count() const noexcept
        {
            return m_count;
        }

        constexpr auto value() const noexcept
        {
            return m_value;
        }

        explicit constexpr operator V() const noexcept
        {
            return this->value();
        }

    private:

        int m_count;
        V m_value;

    };

    template <typename D>
    std::ostream& operator<<(std::ostream& os, SignedDigitSequence<D> sdseq)
    {
        os << sdseq.data();

        return os;
    }


    template <typename V>
    class DigitSequence
    {

    public:

        constexpr DigitSequence() noexcept
            : count_{ 0 }
            , value_{ 0 }
        {}

        constexpr explicit DigitSequence(V value) noexcept
            : count_{ 1 }
            , value_{ value }
        {}

        constexpr auto count() const noexcept
        {
            return this->count_;
        }

        constexpr auto value() const noexcept
        {
            return this->value_;
        }

        explicit constexpr operator V() const noexcept
        {
            return this->value();
        }

        constexpr auto as_signed() const noexcept
        {
            return SignedDigitSequence<V>(this->count_, this->value_);
        }

        constexpr operator SignedDigitSequence<V>() const noexcept
        {
            return this->as_signed();
        }

    private:

        int count_;
        V value_;

    };

    template <typename V>
    std::ostream& operator<<(std::ostream& os, DigitSequence<V> dseq)
    {
        os << dseq.value();

        return os;
    }


    template <typename T>
    constexpr auto concat(Sign sign, DigitSequence<T> dseq) noexcept
    {
        return SignedDigitSequence<T>{ static_cast<T>(sign) * dseq.value() };
    }

    template <typename T>
    constexpr auto concat(DigitSequence<T> dseq1, DigitSequence<T> dseq2) noexcept
    {
        return DigitSequence<T>{ dseq1.value() * dseq1.count() * 10 + dseq2.value() };
    }

    template <typename T>
    constexpr auto concat(SignedDigitSequence<T> sdseq1, DigitSequence<T> dseq2) noexcept
    {
        return SignedDigitSequence<T>{ sdseq1.value() * sdseq1.count() * 10 + dseq2.value() };
    }

}


#endif /*__TOKENS_HPP__*/