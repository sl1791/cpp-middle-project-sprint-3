#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <random>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <boost/container/flat_map.hpp>

#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T>& cont, Comparator comp = {})
{
    boost::container::flat_map<std::string_view, std::size_t, Comparator> result{comp};

    for(const auto& book : cont)
    {
        ++result[book.author];
    }

    return result;
}

template <BookIterator Iterator>
auto calculateGenreRatings(Iterator first, Iterator last)
{
    std::map<Genre, std::pair<double, std::size_t>> grouped;

    for(; first != last; ++first)
    {
        auto& [sum, count] = grouped[first->genre];
        sum += first->rating;
        ++count;
    }

    std::map<Genre, double> result;

    for(const auto& [genre, data] : grouped)
    {
        const auto& [sum, count] = data;
        result[genre] = sum / static_cast<double>(count);
    }

    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T>& cont)
{
    if(cont.empty())
    {
        return 0.0;
    }

    double sum = 0.0;

    for(const auto& book : cont)
    {
        sum += book.rating;
    }

    return sum / static_cast<double>(cont.size());
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T>& cont, std::size_t n, Comparator comp)
{
    std::vector<std::reference_wrapper<const Book>> result;

    if(n == 0 || cont.empty())
    {
        return result;
    }

    std::sort(cont.begin(), cont.end(), comp);

    const auto limit = std::min(n, cont.size());
    result.reserve(limit);

    auto it = cont.begin();
    for(std::size_t i = 0; i < limit; ++i, ++it)
    {
        result.emplace_back(*it);
    }

    return result;
}

}  // namespace bookdb
