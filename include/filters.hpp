#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
  constexpr auto YearBetween(int start, int end)
  {
    return [start, end](const Book& book)
            { return start <= book.year && book.year <= end;};
  }

  constexpr auto RatingAbove(double min_rating)
  {
    return [min_rating](const Book& book)
            { return min_rating < book.rating; };
  }

  constexpr auto GenreIs(Genre genre)
  {
    return [genre](const Book& book)
            { return genre == book.genre;};
  }
  
  template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
  auto filterBooks(It first, Sent last, Pred pred)
  {
    std::vector<std::reference_wrapper<const Book>> res;
    std::copy_if(first, last, std::back_inserter(res), pred);
    return res;
  }
  
  template <BookPredicate... Preds>
  constexpr auto all_of(Preds... preds)
  {
    return [preds...](const Book& book)
        {
            return (preds(book) && ...);
        };
  }
  
  template <BookPredicate... Preds>
  constexpr auto any_of(Preds... preds)
  {
    return [preds...](const Book& book)
        {
            return (preds(book) || ...);
        };
  }

}  // namespace bookdb