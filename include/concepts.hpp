#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T t) 
{
    typename T::value_type;
    requires std::is_same_v<typename T::value_type, Book>;
    t.begin();
    t.end();
    { *t.begin() } -> std::convertible_to<const Book&>;
};

template <typename T>
concept BookIterator = requires(T t) 
{
    t.operator ++();
    { *t } -> std::convertible_to<typename T::value_type>;
};

template <typename S, typename I>
concept BookSentinel = requires(S s, I i) {
    { i != s } -> std::convertible_to<bool>;
    { s != i } -> std::convertible_to<bool>;
    { i == s } -> std::convertible_to<bool>;
    { s == i } -> std::convertible_to<bool>;
};

template <typename P>
concept BookPredicate = requires(P p, const Book& book) 
{
    { p(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator= requires(C c, const Book& book, const Book& book2) 
{
    { c(book, book2) } -> std::convertible_to<bool>;
};

}  // namespace bookdb