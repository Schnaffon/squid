#include "squid.h"
#include "parser/Tokenizer.h"

#include <cerrno>
#if HAVE_CTYPE_H
#include <ctype.h>
#endif
#if HAVE_STDINT_H
#include <stdint.h>
#endif
#ifndef INT64_MIN
/* Native 64 bit system without strtoll() */
#if defined(LONG_MIN) && (SIZEOF_LONG == 8)
#define INT64_MIN LONG_MIN
#else
/* 32 bit system */
#define INT64_MIN       -9223372036854775807L-1L
#endif
#endif

#ifndef INT64_MAX
/* Native 64 bit system without strtoll() */
#if defined(LONG_MAX) && (SIZEOF_LONG == 8)
#define INT64_MAX LONG_MAX
#else
/* 32 bit system */
#define INT64_MAX       9223372036854775807L
#endif
#endif

bool
Parser::Tokenizer::token(SBuf &returnedToken, const CharacterSet &delimiters)
{
    const SBuf savebuf(buf_);
    skip(delimiters);
    const SBuf::size_type tokenLen = buf_.findFirstOf(delimiters); // not found = npos => consume to end
    if (tokenLen == SBuf::npos && !delimiters['\0']) {
        // no delimiter found, nor is NUL/EOS/npos acceptible as one
        buf_ = savebuf;
        return false;
    }
    const SBuf retval = buf_.consume(tokenLen);
    skip(delimiters);
    returnedToken = retval;
    return true;
}

bool
Parser::Tokenizer::prefix(SBuf &returnedToken, const CharacterSet &tokenChars, const SBuf::size_type limit)
{
    const SBuf::size_type prefixLen = buf_.substr(0,limit).findFirstNotOf(tokenChars);
    if (prefixLen == 0)
        return false;
    returnedToken = buf_.consume(prefixLen);
    return true;
}

bool
Parser::Tokenizer::skip(const CharacterSet &tokenChars)
{
    const SBuf::size_type prefixLen = buf_.findFirstNotOf(tokenChars);
    if (prefixLen == 0)
        return false;
    buf_.consume(prefixLen);
    return true;
}

bool
Parser::Tokenizer::skip(const SBuf &tokenToSkip)
{
    if (buf_.startsWith(tokenToSkip)) {
        buf_.consume(tokenToSkip.length());
        return true;
    }
    return false;
}

bool
Parser::Tokenizer::skip(const char tokenChar)
{
    if (buf_[0] == tokenChar) {
        buf_.consume(1);
        return true;
    }
    return false;
}

/* reworked from compat/strtoll.c */
bool
Parser::Tokenizer::int64(int64_t & result, int base)
{
    if (buf_.isEmpty())
        return false;

    //fixme: account for buf_.size()
    bool neg = false;
    const char *s = buf_.rawContent();
    const char *end = buf_.rawContent() + buf_.length();

    if (*s == '-') {
        neg = true;
        ++s;
    } else if (*s == '+') {
        ++s;
    }
    if (s >= end) return false;
    if (( base == 0 || base == 16) && *s == '0' && (s+1 <= end ) &&
            tolower(*(s+1)) == 'x') {
        s += 2;
        base = 16;
    }
    if (base == 0) {
        if ( *s == '0') {
            base = 8;
            ++s;
        } else {
            base = 10;
        }
    }
    if (s >= end) return false;

    uint64_t cutoff;

    cutoff = neg ? -static_cast<uint64_t>(INT64_MIN) : INT64_MAX;
    const int cutlim = cutoff % static_cast<int64_t>(base);
    cutoff /= static_cast<uint64_t>(base);

    int any = 0, c;
    int64_t acc = 0;
    for (c = *s++; s <= end; c = *s++) {
        if (xisdigit(c)) {
            c -= '0';
        } else if (xisalpha(c)) {
            c -= xisupper(c) ? 'A' - 10 : 'a' - 10;
        } else {
            break;
        }
        if (c >= base)
            break;
        if (any < 0 || static_cast<uint64_t>(acc) > cutoff || (static_cast<uint64_t>(acc) == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    if (any == 0) // nothing was parsed
        return false;
    if (any < 0) {
        acc = neg ? INT64_MIN : INT64_MAX;
        errno = ERANGE;
        return false;
    } else if (neg)
        acc = -acc;

    result = acc;
    buf_.consume(s - buf_.rawContent() -1);
    return true;
}
