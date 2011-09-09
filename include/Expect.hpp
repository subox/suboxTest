#ifndef __SUBOXTESTRUNTEST_HPP__
#define	__SUBOXTESTRUNTEST_HPP__

#include <iostream>
#include <string>

#define EXPECT_TRUE( boolean ) suboxTest::Expect::print( __FILE__, __LINE__, boolean )
#define EXPECT_FALSE( boolean ) suboxTest::Expect::print( __FILE__, __LINE__, !boolean )
#define EXPECT_EQ( expected, actual ) suboxTest::Expect::eq( __FILE__, __LINE__, expected, actual );
#define EXPECT_NE( expected, actual ) suboxTest::Expect::ne( __FILE__, __LINE__, expected, actual );
#define EXPECT_ANY_EXCEPTION( X ) bool boolean(false); try{ X;}catch( ... ){ boolean = true; } suboxTest::Expect::print( __FILE__, __LINE__, boolean );
#define EXPECT_EXCEPTION( X, EXCP ) bool boolean(false); try{ X;}catch( EXCP ){ boolean = true; } suboxTest::Expect::print( __FILE__, __LINE__, boolean );
#define EXPECT_EQ_COLLECTIONS( begin1, end1, begin2, end2 ) suboxTest::Expect::checkCollections( __FILE__, __LINE__, begin1, end1, begin2, end2 );
#define ASSERT_TRUE( boolean ) suboxTest::Expect::print( __FILE__, __LINE__, boolean, true )
#define ASSERT_FALSE( boolean ) suboxTest::Expect::print( __FILE__, __LINE__, !boolean, true )
#define ASSERT_EQ( expected, actual ) suboxTest::Expect::eq( __FILE__, __LINE__, expected, actual, true );
#define ASSERT_NE( expected, actual ) suboxTest::Expect::ne( __FILE__, __LINE__, expected, actual, true );
#define ASSERT_ANY_EXCEPTION( X ) bool boolean(false); try{ X;}catch( ... ){ boolean = true; } suboxTest::Expect::print( __FILE__, __LINE__, boolean, true );
#define ASSERT_EXCEPTION( X, EXCP ) bool boolean(false); try{ X;}catch( EXCP ){ boolean = true; } suboxTest::Expect::print( __FILE__, __LINE__, boolean, true );

namespace suboxTest{

struct Expect{

/**
 * 	\brief Wypisuje na ekran błąd z daną linią z pliku
 * 	\param file nazwa pliku
 * 	\param line numer linii
 * 	\param boolean czy test został spełniony
 * 	\param shouldAbort czy test przerwał pozostałe testy
 */
static void print(
		std::string const& file,
		int line,
		bool boolean,
		bool shouldAbort = false );

/**
 * 	\brief Porównuje dwie kolekcje
 * 	\param file nazwa pliku
 * 	\param line numer linii
 * 	\param begin1 iterator rozpoczynający kolekcję numer 1
 * 	\param end1 iterator wskazujący na end kolekcję numer 1
 * 	\param begin2 iterator rozpoczynający kolekcję numer 2
 * 	\param end2 iterator wskazujący na end kolekcję numer 2
 */
template < typename T >
static void checkCollections(
		std::string const& file,
		int line,
		T begin1,
		T end1,
		T begin2,
		T end2 ){
	T it = begin1, it2 = begin2;
	for( ;it != end1 || it2 != end2; ++it, ++it2 ){
		print( file, line, *it2 == *it );
	}

	if( it != end1 || it2 != end2 )
		print( file, line, false, true );
}

/**
 * 	\brief Porównuje dwie wartości
 * 	\param file nazwa pliku
 * 	\param line numer linii
 * 	\param expected oczekiwana wartość
 * 	\param actual aktualna wartość
 * 	\param shouldAbort czy test przerwał pozostałe testy
 */
template < typename T >
static void eq(
		std::string const& file,
		int line,
		T const& expected,
		T const& actual,
		bool shouldAbort = false ){
	print( file, line, expected == actual, shouldAbort );
}

/**
 * 	\brief Sprawdza, czy dwie wartości są różne
 * 	\param file nazwa pliku
 * 	\param line numer linii
 * 	\param expected oczekiwana wartość
 * 	\param actual aktualna wartość
 * 	\param shouldAbort czy test przerwał pozostałe testy
 */
template < typename T >
static void ne(
		std::string const& file,
		int line,
		T const& expected,
		T const& actual,
		bool shouldAbort = false ){
	print( file, line, expected != actual, shouldAbort );
}

};

}

#endif

