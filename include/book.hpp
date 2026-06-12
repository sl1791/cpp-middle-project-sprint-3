#pragma once

#include <format>
#include <stdexcept>
#include <string_view>
#include <array>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr std::string_view s_Fiction = "Fiction";
constexpr std::string_view s_NonFiction = "NonFiction";
constexpr std::string_view s_SciFi = "SciFi";
constexpr std::string_view s_Biography = "Biography";
constexpr std::string_view s_Mystery = "Mystery";

constexpr Genre GenreFromString(std::string_view s) 
{
    constexpr std::array<std::pair<std::string_view, Genre>, 5> 
        genres = 
        {{
            {s_Fiction, Genre::Fiction} ,
            {s_NonFiction, Genre::NonFiction},
            {s_SciFi, Genre::SciFi},
            {s_Biography, Genre::Biography},
            {s_Mystery, Genre::Mystery},
        }};

    auto it = std::find_if(genres.begin(), genres.end(),
        [s](const auto& pair) { return pair.first == s; });

    if (it != genres.end())
        return it->second;
    return Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string title;
    std::string_view author;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(std::string a_title,
                    std::string_view a_author,
                    int a_year,
                    Genre a_genre,
                    double a_rating,
                    int a_read_count) :
    title(std::move(a_title)),
    author(a_author),
    year(a_year),
    genre(a_genre),
    rating(a_rating),
    read_count(a_read_count)
    { }

    constexpr Book(std::string a_title,
                std::string_view a_author,
                int a_year,
                std::string a_genre,
                double a_rating,
                int a_read_count) :
    title(std::move(a_title)),
    author(a_author),
    year(a_year),
    genre(GenreFromString(a_genre)),
    rating(a_rating),
    read_count(a_read_count)
    { }
}; // Book

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        return format_to(fc.out(), "{} {} {} {} {} {}", 
            b.author,
            b.title,
            b.year,
            b.genre,
            b.rating,
            b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
