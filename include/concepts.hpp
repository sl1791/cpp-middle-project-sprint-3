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
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept BookIterator = std::input_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

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