#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <flat_map>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using size_type = BookContainer::size_type;
    using value_type = Book;

    // Ваш код здесь

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> bookList)
    {  
        for(const Book& it : bookList)
        {
            PushBack(it);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator end() const { return books_.end(); }
    size_type size() const { return books_.size(); }

    //
    template<typename ... Arg>
    void EmplaceBack(Arg&& ... a_Arg)
    {
        auto &newBook = books_.emplace_back(std::forward<Arg>(a_Arg)...);
        auto [author, _] = authors_.emplace(newBook.author);
        newBook.author = *author;
    }

    void PushBack(const Book& a_Book)
    {
        auto &newBook = books_.emplace_back(a_Book);
        auto [author, _] = authors_.emplace(newBook.author);
        newBook.author = *author;
    }

    void PushBack(Book&& a_Book)
    {
        auto &newBook = books_.emplace_back(std::move(a_Book));
        auto [author, _] = authors_.emplace(newBook.author);
        newBook.author = *author;
    }

    std::span<const Book> GetBooks() const
    {
        return std::span{books_};
    }
    std::span<Book> GetBooks()
    {
        return std::span{books_};
    }

    [[nodiscard]] const AuthorContainer &GetAuthors()
    {
        return authors_;
    }
    [[nodiscard]] const AuthorContainer &GetAuthors() const
    {
        return authors_;
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
/*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)
*/

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <typename Comparator>
struct formatter<std::flat_map<std::string_view, size_t, Comparator>> {
    template <typename FormatContext>
    auto format(const std::flat_map<std::string_view, size_t, Comparator> &histogram, FormatContext &fc) const {
        format_to(fc.out(), "\n");
        for (const auto &[author, val] : histogram) {
            format_to(fc.out(), "- {} - {}\n", author, val);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
