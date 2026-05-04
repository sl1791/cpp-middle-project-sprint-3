#include <gtest/gtest.h>

#include <algorithm>
#include <string_view>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

TEST(BookTest, CreateBookWithGenre)
{
    Book book{"1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190};

    EXPECT_EQ(book.title, "1984");
    EXPECT_EQ(book.author, "George Orwell");
    EXPECT_EQ(book.year, 1949);
    EXPECT_EQ(book.genre, Genre::SciFi);
    EXPECT_DOUBLE_EQ(book.rating, 4.0);
    EXPECT_EQ(book.read_count, 190);
}

TEST(BookDatabaseTest, EmplaceBackAddsBooks)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);

    ASSERT_EQ(db.size(), 2);
    EXPECT_FALSE(db.empty());

    EXPECT_EQ(db.GetBooks()[0].title, "1984");
    EXPECT_EQ(db.GetBooks()[1].title, "Animal Farm");
}

TEST(BookDatabaseTest, ClearRemovesBooksAndAuthors)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);

    db.Clear();

    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST(ComparatorsTest, SortsByAuthor)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);

    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    EXPECT_EQ(db.GetBooks()[0].author, "Aldous Huxley");
    EXPECT_EQ(db.GetBooks()[1].author, "George Orwell");
}

TEST(ComparatorsTest, SortByPopularityDescending)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Low", "Author A", 2000, Genre::Fiction, 4.0, 10);
    db.EmplaceBack("High", "Author B", 2001, Genre::Fiction, 4.0, 100);

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});

    EXPECT_EQ(db.GetBooks()[0].title, "High");
}

TEST(FiltersTest, FiltersBooksByYearAndRating)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Old Good", "A", 1950, Genre::Fiction, 4.8, 10);
    db.EmplaceBack("Old Bad", "B", 1950, Genre::Fiction, 3.0, 10);
    db.EmplaceBack("New Good", "C", 2020, Genre::Fiction, 4.9, 10);

    auto filtered = filterBooks(db.begin(), db.end(), 
                                all_of(YearBetween(1900, 1999), 
                                RatingAbove(4.5)));

    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Old Good");
}

TEST(StatisticsTest, CalculatesAverageRating)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Book A", "Author A", 2000, Genre::Fiction, 4.0, 10);
    db.EmplaceBack("Book B", "Author B", 2001, Genre::Fiction, 5.0, 10);

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 4.5);
}

TEST(StatisticsTest, CalculateGenreRatings)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Book A", "Author A", 2000, Genre::Fiction, 4.0, 10);
    db.EmplaceBack("Book B", "Author B", 2001, Genre::Fiction, 5.0, 10);
    db.EmplaceBack("Book C", "Author C", 2002, Genre::SciFi, 3.0, 10);

    auto ratings = calculateGenreRatings(db.begin(), db.end());

    EXPECT_DOUBLE_EQ(ratings[Genre::Fiction], 4.5);
    EXPECT_DOUBLE_EQ(ratings[Genre::SciFi], 3.0);
}

TEST(StatisticsTest, BuildsAuthorHistogram)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);

    auto histogram = buildAuthorHistogramFlat(db);

    EXPECT_EQ(histogram["George Orwell"], 2);
    EXPECT_EQ(histogram["Aldous Huxley"], 1);
}

TEST(StatisticsTest, GetsTopNByRating)
{
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Low", "A", 2000, Genre::Fiction, 3.0, 10);
    db.EmplaceBack("High", "B", 2001, Genre::Fiction, 5.0, 10);
    db.EmplaceBack("Middle", "C", 2002, Genre::Fiction, 4.0, 10);

    auto top = getTopNBy(db, 2, comp::LessByRating{});

    ASSERT_EQ(top.size(), 2);
    ASSERT_EQ(top[0].get().title, "High");
    ASSERT_EQ(top[1].get().title, "Middle");
}

TEST(BookTest, CreateBookWithStringGenre)
{
    Book book{"1984", "George Orwell", 1949, "SciFi", 4.0, 190};

    EXPECT_EQ(book.title, "1984");
    EXPECT_EQ(book.author, "George Orwell");
    EXPECT_EQ(book.genre, Genre::SciFi);
}