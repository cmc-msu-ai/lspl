#ifndef _LSPL_TEXT_WRITERS_STREAMTEXTWRITER_H_
#define _LSPL_TEXT_WRITERS_STREAMTEXTWRITER_H_

#include "../../utils/Conversion.h"
#include "TextWriter.h"
#include <iostream>

namespace lspl { namespace text { namespace writers {
class StreamTextWriter {
    lspl::utils::Conversion conversion;
    std::ostream* os;
public:
    StreamTextWriter(std::ostream* os, const std::string& writeEncoding, const std::string& sourceEncoding = lspl::utils::Conversion::DEFAULT_ENCODING)
        : os(os)
        , conversion(sourceEncoding, writeEncoding) {};

    template<typename T>
    StreamTextWriter& operator<<(const T& data) {
        *os << data;
        return *this;
    }
    StreamTextWriter& operator<<(const std::string& data) {
        *os << conversion.convert(data);
        return *this;
    }
};
}}}
#endif /*_LSPL_TEXT_WRITERS_STREAMTEXTWRITER_H_*/
