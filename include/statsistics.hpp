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
    boost::container::flat_map<Genre, std::pair<double, std::size_t>> grouped;

    for(; first != last; ++first)
    {
        auto& [sum, count] = grouped[first->genre];
        sum += first->rating;
        ++count;
    }

    boost::container::flat_map<Genre, double> result;

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

    const double sum = std::accumulate(cont.begin(), cont.end(), 0.0,
                                       [](double acc, const Book& book)
                                       {
                                            return acc + book.rating;
                                       });

    return sum / static_cast<double>(cont.size());
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T>& cont, std::size_t n, Comparator comp)
{
    std::vector<std::reference_wrapper<const Book>> result;

    if (n == 0 || cont.empty())
    {
        return result;
    }

    const auto limit = std::min(n, cont.size());

    std::partial_sort(
        cont.begin(),
        cont.begin() + static_cast<std::ptrdiff_t>(limit),
        cont.end(),
        comp);

    result.reserve(limit);

    auto it = cont.begin();
    for (std::size_t i = 0; i < limit; ++i, ++it)
    {
        result.emplace_back(*it);
    }

    return result;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T>& cont, std::size_t num)
{
    std::vector<std::reference_wrapper<const Book>> result;

    if (num == 0 || cont.empty())
    {
        return result;
    }

    result.reserve(cont.size());

    for (const auto& book : cont)
    {
        result.emplace_back(book);
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(result.begin(), result.end(), gen);

    if (result.size() > num)
    {
        result.resize(num);
    }

    return result;
}

}  // namespace bookdb
