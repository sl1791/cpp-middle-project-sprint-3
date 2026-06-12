#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>
#include <flat_map>
#include <map>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) 
{
    std::flat_map<std::string_view, size_t, TransparentStringLess> res;
    for(auto& book : cont)
        ++res[book.author];
    return res;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last)
{
    struct AverageRaiting
    {
        size_t cnt = 0; // кол-во книг
        double totalRaiting = 0.0; // суммарный рейтинг

        void AddRaiting(double a_Raiting)
        { 
            ++cnt;
            totalRaiting += a_Raiting;
        }

        double Average() const
        {
            if(cnt)
                return totalRaiting / cnt;
            return 0.0;
        }
    };

    struct LessGenre 
    {
        bool operator()(Genre L, Genre R) const
        {
            return static_cast<int>(L) < static_cast<int>(R);
        }
    };
    std::map<Genre, AverageRaiting, LessGenre> coll;
    for(It it = first; it != last; ++it)
    {
        coll[it->genre].AddRaiting(it->rating);
    }

    std::flat_map<Genre, double, LessGenre> res;
    auto extracted = std::move(res).extract();
    extracted.keys.reserve(coll.size());
    extracted.values.reserve(coll.size());
    for(const auto& [genre, averRaiting] : coll)
    {
        extracted.keys.emplace_back(genre);
        extracted.values.emplace_back(averRaiting.Average());
    }
    res.replace(std::move(extracted.keys), std::move(extracted.values));
    return res;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books)
{
    double totRaiting = std::transform_reduce(books.begin(), books.end(), 0.0, 
            std::plus<double>(), [](const Book& book){ return book.rating; });
    return books.size() ? totRaiting/books.size() : 0.0;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num)
{
    std::vector<std::reference_wrapper<const Book>> res;
    const size_t actual_num = std::min(num, cont.size());
    res.reserve(actual_num);
    std::sample(cont.begin(), cont.end(), std::back_inserter(res),
            actual_num,
            std::mt19937{std::random_device{}()});
    return res;
}

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp)
{
    n = std::min(n, cont.size());
    // в задании сказано, вернуть n элементов,
    // если результат должен быть упорядочен, 
    // нужно использовать std::partial_sort.
    std::nth_element(cont.begin(), cont.begin() + n, cont.end(), comp);
    return std::vector<std::reference_wrapper<const Book>>(
            cont.begin(), cont.begin() + n);
}

}  // namespace bookdb
