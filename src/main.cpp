#include "Test.hpp"

bool jestParzysta( unsigned number ){
	return !( number % 2 );
}

TEST(parzysta)
	unsigned number;
	GETVALUE( parzysta1, unsigned, number )
	EXPECT_TRUE( jestParzysta( number ) );

	GETVALUE( kolejna_zmienna, unsigned, number );
	EXPECT_TRUE( jestParzysta( number ) );
ENDTEST(parzysta)

TEST(nieparzysta)
	unsigned number;
	GETVALUE( nieparzysta, unsigned, number )
	EXPECT_TRUE( !jestParzysta( number ) );

	std::string zm;
	GETVALUE( kolejna_zmienna, std::string, zm );
	ASSERT_EQ( std::string( "abc" ), zm );

ENDTEST(nieparzysta)

int main( int argc, char** argv ){

	suboxTest::useConfigFile( "dupa.ini" );

	suboxTest::runTests( argc, argv );
	
	return 0;
}
