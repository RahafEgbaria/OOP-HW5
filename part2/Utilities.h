#ifndef PART2_UTILITIES_H
#define PART2_UTILITIES_H

/********************** Conditional begin **********************/
template <bool g, typename T, typename E>
struct Conditional;

template <typename T, typename E>
struct Conditional<true, T, E>
{
    typedef T value;
};

template <typename T, typename E>
struct Conditional<false, T, E>
{
    typedef E value;
};
/********************** Conditional end **********************/
/********************** ConditionalInteger begin **********************/
template <bool g, int i, int j>
struct ConditionalInteger;

template <int i, int j>
struct ConditionalInteger<true, i, j>
{
    static constexpr int value = i;
};

template <int i, int j>
struct ConditionalInteger<false, i, j>
{
    static constexpr int value = j;
};
/********************** ConditionalInteger end **********************/
/********************** ConditionalBool begin **********************/
template <bool g, bool g1, bool g2>
struct ConditionalBool;

template <bool g1, bool g2>
struct ConditionalBool<true, g1, g2>
{
    static constexpr bool value = g1;
};

template <bool g1, bool g2>
struct ConditionalBool<false, g1, g2>
{
    static constexpr bool value = g2;
};
/********************** ConditionalBool end **********************/
/* ConditionalAndValue */
/**
 * the following methods will make a LAZY evaluation (-;
 */

template <bool B>
struct ValueWrapperBool
{
    // typedef true value;
    typedef ValueWrapperBool<B> value;
    constexpr static bool result = B;
};

template <bool, typename, typename>
struct ConditionalAndValue;

template <typename T, typename F>
struct ConditionalAndValue<true, T, F>
{
    typedef typename T::value value;
};
template <typename T, typename F>
struct ConditionalAndValue<false, T, F>
{
    typedef typename F::value value;
};

#endif // PART2_UTILITIES_H