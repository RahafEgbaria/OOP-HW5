#ifndef PART2_UTILITIES_H
#define PART2_UTILITIES_H

#include "UtilitiesAux.h"

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

#endif // PART2_UTILITIES_H