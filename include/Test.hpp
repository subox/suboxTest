//#ifndef __SUBOXTESTRUNTEST_HPP__
//#define	__SUBOXTESTRUNTEST_HPP__

#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <chrono>
#include "Expect.hpp"

#include "IniFileReader.hpp"

#define TEST( NAME ) void suboxTest_##NAME(){
#define ENDTEST( NAME ) } char var_suboxTest_##NAME = suboxTest::runTest::addFunction( #NAME, boost::bind( suboxTest_##NAME ) );

#define ENDSUITETEST( NAME, SUITE ) } char var_suboxTest_##NAME = suboxTest::runTest::addFunction( #NAME, boost::bind( suboxTest_##NAME ), #SUITE );

namespace suboxTest{

void runTests( int argc, char** argv );

void useConfigFile( std::string const& filename );

struct runTest{
	typedef std::pair< std::string, std::vector< unsigned long > > suitePair;
	typedef std::vector< suitePair > suiteVector;
private:
	static suiteVector::iterator findSuite( std::string const& suite );

public:
	/**
	 * 	\brief Przeładowany konstruktor uruchamiający wszystkie testy
	 * 	\param repeat powtarza testy określoną ilość razy
	 * 	\param random gdy jest true, testy uruchamiane są w losowej kolejności
	 */
	runTest( unsigned repeat = 1, bool random = false );

	/**
	 * 	\brief Przeladowany konstruktor uruchamiajacy jeden test określoną ilość razy
	 * 	\param testName nazwa testu
	 * 	\param repeat powtarza testy okreslona ilosc razy
	 */
	runTest( std::string const& testName, unsigned repeat = 1 );

	/**
	 * 	\brief Przeladowany konstruktor uruchamiajacy zestaw testów określoną ilość razy z możliwością mieszania
	 * 	\param listOfTestNames wektor przetrzymujący nazwy testów
	 * 	\param repeat powtarza testy okreslona ilosc razy
	 * 	\param random gdy jest true, testy uruchamiane są w losowej kolejności
	 */
	runTest(
			std::vector< std::string > const& listOfTestNames,
			unsigned repeat = 1,
			bool random = false );

	typedef boost::function< void( void ) > bfun;
	typedef boost::tuple< unsigned long, std::string, runTest::bfun > ftuple;
	typedef std::vector< ftuple > bfunMap;

	template < typename T >
	static char addFunction( std::string const& name, T func, std::string const& suite = "" ){
		suiteVector::iterator it;
		if( ( it = findSuite( suite ) ) != suites.end() ){
			it->second.push_back( testsMap.size() );
		} else {
			suites.push_back( suitePair( suite, std::vector< unsigned long >( 1, testsMap.size() ) ) );
		}

		testsMap.push_back( ftuple( testsMap.size(), name, func ) );
		return 1;
	}

	static void printAllTests();

	static void printAllSuites();

	static unsigned expectFailed;

private:

	void shuffle();

	void executeTest( ftuple const& tuple );

	bool findTest( std::string const& testName );

	bfunMap::iterator findTestIndex( unsigned long index );

	void printSummary( unsigned countTest );

	void localeTest( unsigned& countTest );

	static bfunMap testsMap;

	static suiteVector suites;

	static bool coreDump;

	static unsigned testsFailed;

	std::chrono::high_resolution_clock::time_point startTime, stopTime;
};

}

//#endif

