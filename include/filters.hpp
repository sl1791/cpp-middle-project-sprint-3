#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

    inline auto YearBetween(int from, int to)
    {
        return [from, to](const Book& book)
        {
            return book.year >= from && book.year <= to;
        };
    }

    inline auto RatingAbove(double rating)
    {
        return [rating](const Book& book)
        {
            return book.rating >= rating;
        };
    }

    inline auto GenreIs(Genre genre)
    {
        return [genre](const Book& book)
        {
            return book.genre == genre;
        };
    }

    template <typename... Predicates>
    auto all_of(Predicates... predicates)
    {
        return [=](const Book& book)
        {
            return (predicates(book) && ...);
        };
    }

    template <typename... Predicates>
    auto any_of(Predicates... predicates)
    {
        return [=](const Book& book)
        {
            return (predicates(book) || ...);
        };
    }

    template <BookIterator iterator, typename Predicate>
    auto filterBooks(iterator first, iterator last, Predicate predicate)
    {
        std::vector<std::reference_wrapper<const Book>> result;

        for(; first != last; ++first)
        {
            if(predicate(*first))
            {
                result.emplace_back(*first);
            }
        }

        return result;
    }


}  // namespace bookdb