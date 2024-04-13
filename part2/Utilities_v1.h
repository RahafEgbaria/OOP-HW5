#ifndef PART2_UTILITIES_H
#define PART2_UTILITIES_H

/********************** Conditional begin **********************/
template<bool, typename, typename>
struct Conditional;

template<typename T, typename E>
struct Conditional<true, T, E>{
    typedef T value;
};

template<typename T, typename E>
struct Conditional<false, T, E>{
    typedef E value;
};
/********************** Conditional end **********************/
/********************** ConditionalInteger begin **********************/
template<bool, int, int>
struct ConditionalInteger;

template<int i, int j>
struct ConditionalInteger<true, i, j>{
    static constexpr int value = i;
};

template<int i, int j>
struct ConditionalInteger<false, i, j>{
    static constexpr int value = j;
};
/********************** ConditionalInteger end **********************/

#endif //PART2_UTILITIES_H
