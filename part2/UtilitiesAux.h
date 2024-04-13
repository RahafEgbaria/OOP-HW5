#ifndef PART2_UTILITIES_AUX_H
#define PART2_UTILITIES_AUX_H

/********************** NestedConditional begin **********************/
// like conditional plus applies value on Conditional result
// add inheritance??
template <bool, typename, typename>
struct NestedConditional;

template <typename T, typename F>
struct NestedConditional<true, T, F>
{
    typedef typename T::value value;
};
template <typename T, typename F>
struct NestedConditional<false, T, F>
{
    typedef typename F::value value;
};

template <typename B>
struct NestedConditionalValueWrapper
{
    typedef B value;
};

template <bool B>
struct NestedConditionalBoolWrapper
{
    constexpr static bool result = B;
};
/********************** NestedConditional end **********************/

#endif // PART2_UTILITIES_AUX_H