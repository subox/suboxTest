#include "IniFileReader.hpp"
#include <boost/regex.hpp>

using namespace suboxTest;

bool IniFileReader::parseFile( std::string const& fileName ){

	if( fileName.empty() ){
		return false;
	}

	std::ifstream file( fileName.c_str() );
	if( file.is_open() ){
		m_useParams = readParams( file );
		file.close();
	}else{
		std::cerr << "Open params file failed: " << fileName << std::endl;
	}

	return m_useParams;
}

bool IniFileReader::isParamsRed(){
	return m_useParams;
}

bool IniFileReader::readParams( std::ifstream & file ){
	boost::regex const pattern( "^\\s*([^;#]*[\\w]+)([\\s]*[\\=][\\s]*)((\")?([^\"]*)(\")?)\\s*" );
	boost::regex const sectionPattern( "^\\s*([^#]|\\[(\\w+)\\])\\s*" );
	boost::smatch matches;
	std::string buffer;
	std::string section;
	while( !std::getline( file, buffer ).eof() ){
		if( boost::regex_match( buffer, matches, sectionPattern ) ){
			section = matches[ 2 ];
		} else if( boost::regex_match( buffer, matches, pattern ) ){
			std::string const name = std::string( matches[ 1 ] );
			std::string const value = std::string( matches[ 5 ] );
			paramsVector.push_back( boost::make_tuple( section, name, value ) );
		}
	}

	return !paramsVector.empty();
}

