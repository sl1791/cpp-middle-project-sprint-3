#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s)
 {
    if(s == "Fiction")
    {
        return Genre::Fiction;
    }
    if(s == "NonFiction")
    {
        return Genre::NonFiction;
    }
    if(s == "SciFi")
    {
        return Genre::SciFi;
    }
    if(s == "Biography")
    {
        return Genre::Biography;
    }
    if(s == "Mystery")
    {
        return Genre::Mystery;
    }
    return Genre::Unknown;
}

constexpr std::string_view GenreToString(Genre genre)
{
    using enum Genre;

    switch (genre)
    {
    case Fiction:
        return "Fiction";
    case NonFiction:
        return "NonFiction";
    case SciFi:
        return "SciFi";
    case Biography:
        return "Biography";
    case Mystery:
        return "Mystery";
    case Unknown:
        return "Unknown";
    }
    
    return "Unknown";
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(std::string_view title_, 
                   std::string_view author_, 
                   int year_, 
                   Genre genre_, 
                   double rating_,
                   int read_count_) :
                   author(author_),
                   title(title_),
                   year(year_),
                   genre(genre_),
                   rating(rating_),
                   read_count(read_count_) {}

    constexpr Book(std::string_view title_,
                   std::string_view author_,
                   int year_,
                   std::string_view genre_,
                   double rating_,
                   int read_count_) :
                   Book(title_, author_, year_, GenreFromString(genre_), rating_, read_count_) {}
};
}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::Genre, char>
{
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext& fc) const
    {
        return format_to(fc.out(), "{}", bookdb::GenreToString(g));
    }

    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
};

template <>
struct formatter<bookdb::Book, char> 
{
    template <typename FormatContext>
    auto format(const bookdb::Book& book, FormatContext &fc) const {
        return format_to(fc.out(),
                         "\"{}\" by {}, {}, genre: {}, rating: {}, read count: {}",
                         book.title,
                         book.author,
                         book.year,
                         book.genre,
                         book.rating,
                         book.read_count);
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin(); 
    }
};

}  // namespace std
