#ifndef __SUBOXTESTINIFILEREADER_HPP__
#define	__SUBOXTESTINIFILEREADER_HPP__

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

#include "Singleton.hpp"

#define GETVALUE( PARAMNAME, TYPE, VARIABLE ) suboxTest::IniFileReader::getInstance().getValue< TYPE >( __FUNCTION__, #PARAMNAME, VARIABLE, #TYPE );

namespace suboxTest{

struct IniFileReader;

namespace{
	/**
	 * 	\brief Sprawdza czy parametr i funkcja znajduje się w structurze 
	 * 	\param myTuple struktura zawierająca nazwę funkcji, parametru
	 * 	\param funcName nazwa funkcji
	 * 	\param param nazwa parametru
	 * 	\return wynik sprawdzenia
	*/
	bool findValue(
			boost::tuple< std::string, std::string, std::string > const& myTuple,
			std::string const& funcName,
			std::string const& param ){
		return myTuple.get< 0 >() == funcName && myTuple.get< 1 >() == param;
	}
}

/**
 * 	\brief Klasa obslugująca wszystywanie parametrów z plików formatu ini
*/
struct IniFileReader : public Singleton< suboxTest::IniFileReader > {

	/**
	 * 	\brief Odczytuje plik i jego parametry
	 * 	\param fileName nazwa pliku
	 * 	\return wynik odczytu
	*/
	bool parseFile( std::string const& fileName );

	/**
	 * 	\brief Sprawdza, czy powiodło się odczytanie
	 * 	\return wynik odczytu
	*/
	bool isParamsRed();

	/**
	 * 	\brief Pobiera wartość z parametru uwzględniając nazwę funkcji
	 * 	\param[in] funcName nazwa funkcji
	 * 	\param[in] param nazwa parametru
	 * 	\param[out] value wartość odczytana
	 * 	\param[in] convertTo nazwa typu
	*/
	template < typename T >
	void getValue(
			std::string const& funcName,
			std::string const& param,
			T& value,
			std::string const& convertTo );

private:
	friend class Singleton< suboxTest::IniFileReader >;

	IniFileReader() : m_useParams( false ){}
	~IniFileReader(){}

	bool readParams( std::ifstream & file );

	typedef boost::tuple< std::string, std::string, std::string > tupleString;
	std::vector< tupleString > paramsVector;
	bool m_useParams;
};

template < typename T >
void IniFileReader::getValue(
		std::string const& funcName,
		std::string const& param,
		T& value,
		std::string const& convertTo ){
	std::string func = boost::algorithm::erase_first_copy( funcName, "suboxTest_" );
	std::vector< tupleString >::iterator it = std::find_if(
														paramsVector.begin(),
														paramsVector.end(),
														boost::bind(
															findValue,
															_1,
															func,
															param ) );
	if( it != paramsVector.end() ) {
		try{
			value = boost::lexical_cast< T >( it->get< 2 >() );
		} catch( boost::bad_lexical_cast const& e ) {
			std::cerr << "Exception during conversion to type " << convertTo << " from ini file parameter: \"" << param <<
							"\" with value: \"" << it->get< 2 >() << "\"\n" << e.what() << std::endl;
		} catch( ... ){
			std::cerr << "Unknown Exception during getting value" << std::endl;
		}
	}
}

}

#endif

