#include "Utils.h"

#include "lspl/utils/Conversion.h"

namespace lspl { namespace java {

lspl::utils::Conversion inputConverter( "UTF-8", "CP1251" );
lspl::utils::Conversion outputConverter( "CP1251", "UTF-8" );

std::string in( JNIEnv * env, jstring str ) {
	const char * buff = env->GetStringUTFChars( str, NULL );	
	std::string result = inputConverter.convert( buff );
	env->ReleaseStringUTFChars( str, buff );
	
	return result;
}

jstring out( JNIEnv * env, const std::string & str ) {
	return env->NewStringUTF( outputConverter.convert( str ).c_str() );
}

jstring out( JNIEnv * env, const char * str ) {
	return env->NewStringUTF( outputConverter.convert( str ).c_str() );
}

} }
