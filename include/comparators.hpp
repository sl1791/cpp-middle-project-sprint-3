#pragma once

#include "book.hpp"

namespace bookdb::comp {

using Book = bookdb::Book;
struct LessByAuthor 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.author < a_R.author;
    }
};

struct LessByTitle 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.title < a_R.title;
    }
};

struct LessByYear 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.year < a_R.year;
    }
};

struct LessByGenre 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.genre < a_R.genre;
    }
};

struct LessByRating 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.rating < a_R.rating;
    }
};

struct LessByRead_count 
{
    bool operator()(const Book& a_L, const Book& a_R) const
    {
        return a_L.read_count < a_R.read_count;
    }
};

}  // namespace bookdb::comp