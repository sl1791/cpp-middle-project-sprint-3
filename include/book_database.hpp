#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <initializer_list>
#include <utility>
#include <deque>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases

    using value_type = typename BookContainer::value_type;
    using container_type = BookContainer;

    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;

    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;

    using pointer = typename BookContainer::pointer;
    using const_pointer = typename BookContainer::const_pointer;

    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;


    using AuthorContainer = std::deque<std::string>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books)
    {
        for(const auto& book : books)
        {
            PushBack(book);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const Book& book)
    {
        authors_.emplace_back(book.author);

        Book stored_book = book;
        stored_book.author = authors_.back();

        books_.emplace_back(std::move(stored_book));
    }

    void PushBack(Book&& book)
    {
        authors_.emplace_back(book.author);

        book.author = authors_.back();

        books_.emplace_back(std::move(book));
    }

    template <typename... Args>
    reference EmplaceBack(Args&&... args)
    {
        Book book{std::forward<Args>(args)...};
        PushBack(std::move(book));
        return books_.back();
    }

    iterator begin()
    {
        return books_.begin();
    }

    iterator end()
    {
        return books_.end();
    }

    const_iterator begin() const
    {
        return books_.cbegin();
    }

    const_iterator end() const
    {
        return books_.cend();
    }
    
    const_iterator cbegin() const
    {
        return books_.cbegin();
    }

    const_iterator cend() const
    {
        return books_.cend();
    }

    bool empty() const
    {
        return books_.empty();
    }

    size_type size() const
    {
        return books_.size();
    }

    const BookContainer& GetBooks() const
    {
        return books_;
    }

    const AuthorContainer& GetAuthors() const
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
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for(const auto& book : db.GetBooks())
        {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for(const auto& author : db.GetAuthors())
        {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
