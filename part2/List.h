#ifndef PART2_LIST_H
#define PART2_LIST_H

/********************** list begin **********************/
template <typename...>
struct List;

template <typename T, typename... TT>
struct List<T, TT...>
{
    typedef T head;
    typedef List<TT...> next;
    static constexpr int size = sizeof...(TT) + 1;
};

template <>
struct List<>
{
    static constexpr int size = 0;
};
/********************** List end **********************/
/********************** PrependList begin **********************/
template <typename... TT>
struct PrependList;

template <typename T, typename... TT>
struct PrependList<T, List<TT...>>
{
    typedef List<T, TT...> list;
};
/********************** PrependList end **********************/
/********************** GetAtIndex begin **********************/
template <int N, typename T>
struct GetAtIndex
{
};

template <int N, typename T, typename... TT>
struct GetAtIndex<N, List<T, TT...>>
{
    typedef typename GetAtIndex<N - 1, typename List<T, TT...>::next>::value value;
};

template <typename T, typename... TT>
struct GetAtIndex<0, List<T, TT...>>
{
    typedef T value;
};
/********************** GetAtIndex end **********************/
/********************** SetAtIndex begin **********************/
template <int N, typename type, typename T>
struct SetAtIndex;

template <int N, typename type, typename T, typename... TT>
struct SetAtIndex<N, type, List<T, TT...>>
{
    typedef typename PrependList<T, typename SetAtIndex<N - 1, type, List<TT...>>::list>::list list;
};

template <typename type, typename T, typename... TT>
struct SetAtIndex<0, type, List<T, TT...>>
{
    typedef typename PrependList<type, List<TT...>>::list list;
};
/********************** SetAtIndex end **********************/

#endif // PART2_LIST_H
