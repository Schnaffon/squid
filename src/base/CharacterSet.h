#ifndef _SQUID_SRC_PARSER_CHARACTERSET_H
#define _SQUID_SRC_PARSER_CHARACTERSET_H

#include <vector>

/// optimized set of C chars, with quick membership test and merge support
class CharacterSet
{
public:
    typedef std::vector<uint8_t> Storage;

    /// define a character set with the given label ("anonymous" if NULL)
    ///  with specified initial contents
    CharacterSet(const char *label, const char * const initial);

    /// whether a given character exists in the set
    bool operator[](unsigned char c) const {return chars_[static_cast<uint8_t>(c)] != 0;}

    /// add a given character to the character set
    CharacterSet & add(const unsigned char c);

    /// add all characters from the given CharacterSet to this one
    const CharacterSet &operator +=(const CharacterSet &src);

    /// optional set label for debugging (default: "anonymous")
    const char * name;

private:
    /** index of characters in this set
     *
     * \note guaranteed to be always 256 slots big, as forced in the
     *  constructor. This assumption is relied upon in operator[], add,
     *  operator+=
     */
   Storage chars_;
};

#endif /* _SQUID_SRC_PARSER_CHARACTERSET_H */
