#ifndef DDC_internal_error_h
#define DDC_internal_error_h


enum DDCErrorEnum
{	errUnknown = 0,
	errReadOccurrenceFile	= 1,
	errProcessMorphology	= 2,
	errUnknownPath	= 3,
	errParseError = 4,
	errReadSourceFile	= 5,
	errTimeoutElapsed	= 6
};


inline string GetDDCErrorString(DDCErrorEnum ErrorCode)
{
	switch (ErrorCode) {
		case errUnknown: return "No error";
		case errReadOccurrenceFile: return "cannot read an index file";
		case errProcessMorphology: return "an exception in morphology";
		case errUnknownPath: return "bad path for index files";
		case errParseError: return "cannot parse the input query";
		case errReadSourceFile: return "cannot read a source file";
		case errTimeoutElapsed: return "timeout elapsed";
	};
	return "Unknown error code";

};

#endif
