#include "Test.hpp"

#include <iostream>
#include <algorithm>
#include <ctime>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

using namespace suboxTest;

runTest::bfunMap runTest::testsMap;
runTest::suiteVector runTest::suites;
bool runTest::coreDump( false );
unsigned runTest::expectFailed( 0 );
unsigned runTest::testsFailed( 0 );

void suboxTest::runTests( int argc, char** argv ){
	boost::program_options::options_description d( "Allowed options for suboxTest library" );
	d.add_options()
		( "tests",
         "List all tests")
		( "suites",
         "List all suites")
		( "run",
         boost::program_options::value< std::string >(),
         "Run specific tests. Use comma without whitespace to run more tests." )
		( "runs",
         boost::program_options::value< std::string >(),
         "Run specific suites. Use comma without whitespace to run more suites." )
		( "repeat",
         boost::program_options::value< unsigned long >(),
         "Run the tests repeatedly")
		( "shuffle",
         "Randomize tests order")
		( "help",
        "produce this help message" );

	boost::program_options::variables_map m;
	boost::program_options::store( boost::program_options::parse_command_line( argc, argv, d ), m );
	boost::program_options::notify( m );

	bool run( true );
	unsigned long repeat( 1 );
	bool shuffle( false );
	std::vector< std::string > tests;
	std::vector< std::string > suites;

	if( m.count( "tests" ) ){
		std::cout << "All availble tests:" << std::endl;
		runTest::printAllTests();
		run = false;
	} else if( m.count( "suites" ) ){
		std::cout << "All availble tests:" << std::endl;
		runTest::printAllSuites();
		run = false;
	} else if( m.count( "help" ) ){
		std::cout << d << "\n";
		run = false;
	}

	if( !run ){
		exit( 0 );
	}

	if( m.count( "run" ) ){
		boost::split( tests, m[ "run" ].as< std::string >(), boost::is_any_of("," ));
	} else if( m.count( "runs" ) ) {
		boost::split( suites, m[ "runs" ].as< std::string >(), boost::is_any_of("," ));
	}

	if( m.count( "repeat" ) ){
		repeat = m[ "repeat" ].as< unsigned long >();
	}

	if( m.count( "shuffle" ) ){
		shuffle = true;
	}

	if( !tests.empty() ){
		runTest( tests, repeat, shuffle );
	} else if( !suites.empty() ){
	} else {
		runTest( repeat, shuffle );
	}
}

void suboxTest::useConfigFile( std::string const& filename ){
	IniFileReader::getInstance().parseFile( filename );
}

runTest::runTest( unsigned repeat, bool random ){
	GETVALUE( repeat, unsigned, repeat )
	GETVALUE( random, bool, random )
	GETVALUE( coredump, bool, coreDump )

	if( random ){
		shuffle();
	}

	unsigned countTest( 0 );
	startTime = std::chrono::high_resolution_clock::now();
	while( repeat-- ){
		localeTest( countTest );
	}
	stopTime = std::chrono::high_resolution_clock::now();

	printSummary( countTest );
}

void runTest::localeTest( unsigned& countTest ){
	if( suites.size() == 1 && suites.back().first.empty() ){
		for( bfunMap::iterator it( testsMap.begin() ); it != testsMap.end(); ++it ){
			++countTest;
			std::cout << "\033[0;32m[ Run      ]\033[0m ";
			expectFailed = 0;
			executeTest( *it );
			std::cout << "\033[0;32m[ -------- ]\033[0m" << std::endl;
		}
	}else{
		bfunMap::iterator funIt;
		for( suiteVector::iterator it( suites.begin() ); it != suites.end(); ++it ){
			if( !it->first.empty() )
				std::cout << "\033[0;33m[ SUITE    ] " << it->first << "\033[0m"<< std::endl;
			for( std::vector< unsigned long >::iterator iter( it->second.begin() ); iter != it->second.end(); ++iter ){
				if( ( funIt = findTestIndex( *iter ) ) != testsMap.end() ){
					++countTest;
					std::cout << "\033[0;32m[ Run      ]\033[0m ";
					expectFailed = 0;
					executeTest( *funIt );
					std::cout << "\033[0;32m[ -------- ]\033[0m" << std::endl;
				}
			}
			if( !it->first.empty() ) std::cout << "\033[0;33m[ ENDSUITE ]\033[0m\n\n";
		}
	}
}

runTest::runTest(
		std::string const& testName,
		unsigned repeat ){
	unsigned countTest( 0 );
	GETVALUE( repeat, unsigned, repeat )
	while( repeat-- ){
		++countTest;
		std::cout << "\033[0;32m[ Run      ]\033[0m ";
		findTest( testName );
		std::cout << "\033[0;32m[ -------- ]\033[0m" << std::endl;
	}

	printSummary( countTest );
}

runTest::runTest(
		std::vector< std::string > const& listOfTestNames,
		unsigned repeat,
		bool random ){

	IniFileReader::getInstance().parseFile( "dupa.ini" );

	GETVALUE( repeat, unsigned, repeat )
	GETVALUE( random, bool, random )
	GETVALUE( coredump, bool, coreDump )

	if( random ){
		shuffle();
	}

	unsigned countTest( 0 );
	startTime = std::chrono::high_resolution_clock::now();
	while( repeat-- ){
		for( unsigned i( 0 ); i < listOfTestNames.size(); ++i ){
			++countTest;
			std::cout << "\033[0;32m[ Run      ]\033[0m ";
			expectFailed = 0;
			findTest( listOfTestNames[ i ] );
			std::cout << "\033[0;32m[ -------- ]\033[0m" << std::endl;
		}
	}

	stopTime = std::chrono::high_resolution_clock::now();

	printSummary( countTest );
}

void runTest::printSummary( unsigned countTest ){
	std::cout << "\n\033[0;33mTests result:\033[0m\nDuration of tests: "
			<< std::chrono::duration_cast< std::chrono::milliseconds >( stopTime - startTime ).count() << " ms" << std::endl;
	if( testsFailed ){
		std::cout << "\033[0;31m" << testsFailed << " tests FAILED\033[0m\n\033[0;32m" << countTest - testsFailed << " tests";
	} else {
		std::cout << "\033[0;32mAll of " << countTest;
	}

	std::cout << " ended succesfully\033[0m" << std::endl;
}

void runTest::shuffle(){
	srand ( unsigned( time( 0 ) ) );
	std::random_shuffle( testsMap.begin(), testsMap.end() );
}

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

void runTest::executeTest( ftuple const& tuple ){
	std::cout << "\033[1;37m" << tuple.get< 1 >() << "\033[0m" << std::endl;
	bool interupted( false );
	std::chrono::high_resolution_clock::time_point t0, t1;
	try{
		t0 = std::chrono::high_resolution_clock::now();
		tuple.get< 2 >()();
		t1 = std::chrono::high_resolution_clock::now();
	}catch( std::exception const&  ){
		t1 = std::chrono::high_resolution_clock::now();
		interupted = true;
	}

	std::cout << "\033[0;32m[ End";
	if( expectFailed ){
		++testsFailed;
		std::cout << "\033[0m\033[0;31m  NOK \033[0m\033[0;32m]\033[0m with " << expectFailed << " unexpected \033[0;31mFAILED";
		interupted && std::cout << " and INTERUPTED";
		std::cout << "\t";
	} else {
		std::cout << "   OK ] ";
	}
	std::cout << "\033[0m[ " << std::chrono::duration_cast< std::chrono::milliseconds >( t1 - t0 ).count()
				<< " ms ]"<< std::endl;
}

namespace{
	bool findTestName( runTest::ftuple const& pair, std::string const& testName ){
		return pair.get< 1 >() == testName;
	}

	bool findSuiteName( runTest::suitePair const& pair, std::string const& suite ){
		return pair.first == suite;
	}

	bool findTestByIndex( runTest::ftuple const& pair, unsigned long index ){
		return pair.get< 0 >() == index;
	}
}

bool runTest::findTest( std::string const& testName ){
	bool status( false );
	bfunMap::iterator it = std::find_if(
									testsMap.begin(),
									testsMap.end(),
									boost::bind( findTestName, _1, testName ) );

	if( it != testsMap.end() ){
		executeTest( *it );
		status = true;
	}else{
		++runTest::testsFailed;
		std::cerr << "\033[0;31mError:\033[0m Unknown test name: " << testName << std::endl;
	}

	return status;
}

runTest::suiteVector::iterator runTest::findSuite( std::string const& suite ){
	suiteVector::iterator it = std::find_if(
									suites.begin(),
									suites.end(),
									boost::bind( findSuiteName, _1, suite ) );
	return it;
}

runTest::bfunMap::iterator runTest::findTestIndex( unsigned long index ){
	bfunMap::iterator it = std::find_if(
									testsMap.begin(),
									testsMap.end(),
									boost::bind( findTestByIndex, _1, index ) );
	return it;
}

void runTest::printAllTests(){
	for( bfunMap::iterator it( testsMap.begin() ); it != testsMap.end(); ++it ){
			std::cout << it->get< 1 >() << std::endl;
		}
}

void runTest::printAllSuites(){
	for( suiteVector::iterator it( suites.begin() ); it != suites.end(); ++it ){
			if( !it->first.empty() )
				std::cout << it->first << std::endl;
		}
}

