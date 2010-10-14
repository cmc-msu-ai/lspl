/*
 * AotGraphan.cpp
 *
 *  Created on: May 9, 2009
 *      Author: alno
 */
#include "../base/BaseInternal.h"

#include "AotGraphan.h"

#include <stdexcept>
#include <iostream>

#include "aot/Source/GraphanLib/GraphmatFile.h"

#ifdef WIN32
#define PD "\\"
#else
#define PD "/"
#endif

namespace lspl { namespace graphan {

const char * AotGraphan::findRml() {
	static std::string candidates[] = {"data",".." PD "data","deps" PD "aot",".." PD "deps" PD "aot",".." PD ".." PD "deps" PD "aot"};

	for ( int i = 0; i < 5; ++ i ) {
		std::string rmlIni = candidates[i] + "/Bin/rml.ini";

		if ( FILE * fi = fopen(rmlIni.c_str(),"r") ) {
			fclose( fi );

			return candidates[i].c_str();
		}
	}

	return 0;
}

void AotGraphan::setupRml() {
	if ( getenv("RML") )
		return;

	if ( const char * rml = findRml() ) {
#ifdef WIN32
			putenv( (std::string( "RML=") + rml).c_str() );
#else
			setenv( "RML", rml, 1 );
#endif
	} else {
		std::cerr << "Error: No suitable RML path found!" << std::endl;
	}
}

AotGraphan::AotGraphan() {
}

AotGraphan::~AotGraphan() {
}

void AotGraphan::analyzeString( const std::string & str, boost::ptr_vector<Unit> & units ) {
	setupRml();

	try {
		CGraphmatFile file;

#ifdef MSVC
		if ( !file.LoadDicts() ) // Загружаем словари
			throw std::logic_error( file.GetLastError() );

		if ( !file.LoadStringToGraphan( str ) ) // Загружаем файл
			throw std::logic_error( file.GetLastError() );
#else
		if ( !file.LoadDicts() ) // Загружаем словари
			throw std::logic_error( file.GetLastError() );

		if ( !file.LoadStringToGraphan( str ) ) // Загружаем файл
			throw std::logic_error( file.GetLastError() );
#endif

		foreach ( const CGraLine & line, file.GetUnits() ) {
			units.push_back( new Unit( line.GetToken(), line.GetTokenLength(), line.GetInputOffset(), line.IsWordOrNumberOrAbbr() ? Unit::WORD : line.IsPunct() ? Unit::PUNCT : Unit::UNKNOWN ) );
		}
	} catch ( const std::exception & e ) {
		throw e;
	} catch ( const CExpc & e ) {
		throw std::logic_error( "Couldn't init morphology: " + e.m_strCause );
	} catch (...) {
		throw std::logic_error( "Couldn't init morphology due to unknown error" );
	}
}

} }
