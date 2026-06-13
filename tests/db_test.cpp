#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;
constexpr bool IsZero(double val)
{
    return abs(val) < 0.0000001;
} 

constexpr bool IsEQ(const Book& L, const Book& R)
{
    return
        std::tie(L.title, /*L.author,*/ L.year, L.genre, L.read_count) == 
            std::tie(R.title, /*R.author,*/ R.year, R.genre, R.read_count) &&
        IsZero(L.rating - R.rating);
}


TEST(BookDatabase, FillDatabase) 
{ 
    BookDatabase<std::vector<Book>> db;
    EXPECT_EQ(db.size(), 0); 
    constexpr std::string_view title = "1984";
    constexpr std::string_view author = "George Orwell";
    constexpr int year = 1949;
    constexpr Genre genre = Genre::SciFi;
    constexpr double rating = 4.;
    constexpr int read_count = 190;

    db.EmplaceBack(title, author, year, genre, rating, read_count);

    EXPECT_EQ(db.size(), 1); 

    Book& book = *db.begin();
    EXPECT_EQ(book.title, title); 
    EXPECT_EQ(book.author, author); 
    EXPECT_EQ(book.year, year); 
    EXPECT_EQ(book.genre, genre); 
    GTEST_EXPECT_TRUE(IsZero(book.rating-rating)); 
    EXPECT_EQ(book.read_count, read_count); 
}

TEST(BookDatabase, EmptyDatabase) 
{ 
    BookDatabase<std::vector<Book>> db;
    EXPECT_EQ(db.size(), 0); 
    EXPECT_NO_THROW(std::sort(db.begin(), db.end(), comp::LessByAuthor{}));
    EXPECT_NO_THROW(std::sort(db.begin(), db.end(), comp::LessByRating{}));
    EXPECT_NO_THROW(buildAuthorHistogramFlat(db));
    EXPECT_NO_THROW(calculateGenreRatings(db.begin(), db.end()));
    EXPECT_NO_THROW(calculateAverageRating(db));
    EXPECT_NO_THROW(filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5))));
    EXPECT_NO_THROW(getTopNBy(db, 3, comp::LessByRating{}));
    EXPECT_NO_THROW(std::find_if(db.begin(), db.end(), 
        [](const auto &v) { return v.author == "George Orwell"; }));
    
    auto histogram = buildAuthorHistogramFlat(db);
    GTEST_EXPECT_TRUE(histogram.empty());
    
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    GTEST_EXPECT_TRUE(genreRatings.empty()); 
    
    auto avrRating = calculateAverageRating(db);
    GTEST_EXPECT_TRUE(IsZero(avrRating)); 

    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    GTEST_EXPECT_TRUE(filtered.empty()); 

    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    GTEST_EXPECT_TRUE(topBooks.empty()); 

    auto orwellBookIt = std::find_if(db.begin(), db.end(), 
        [](const auto &v) { return v.author == "George Orwell"; });
    GTEST_EXPECT_TRUE(orwellBookIt == db.end()); 
}

TEST(BookDatabase, InitializerList) 
{ 
    const Book bk1 = {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const Book bk2 = {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    BookDatabase<std::vector<Book>> db = {bk1, bk2};
    EXPECT_EQ(db.size(), 2); 
    GTEST_EXPECT_TRUE(IsEQ(*db.begin(), bk1)); 
    GTEST_EXPECT_TRUE(IsEQ(*(db.begin() + 1), bk2)); 
}