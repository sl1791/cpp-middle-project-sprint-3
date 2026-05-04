#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T container, Book book)
{
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    typename T::size_type;

    requires std::same_as<typename T::value_type, Book>;

    { container.begin() } -> std::same_as<typename T::iterator>;
    { container.end() } -> std::same_as<typename T::iterator>;

    { std::as_const(container).begin() } -> std::same_as<typename T::const_iterator>;
    { std::as_const(container).end() } -> std::same_as<typename T::const_iterator>;

    { container.size() } -> std::same_as<typename T::size_type>;
    { container.empty() } -> std::convertible_to<bool>;

    container.clear();
    container.emplace_back(book);
};

template <typename T>
concept BookIterator = 
    std::input_iterator<T> && 
    std::same_as<std::iter_value_t<T>, Book>;

template <typename S, typename I>
concept BookSentinel = 
    BookIterator<I> &&
    std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P predicate, const Book& book)
{
    { predicate(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator =  requires(C comparator, const Book& lhs, const Book& rhs)
{
    { comparator(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb