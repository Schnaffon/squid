/*
 * Copyright (C) 1996-2015 The Squid Software Foundation and contributors
 *
 * Squid software is distributed under GPLv2+ license and includes
 * contributions from numerous individuals and organizations.
 * Please see the COPYING and CONTRIBUTORS files for details.
 */

#ifndef SQUID_REFRESHPATTERN_H_
#define SQUID_REFRESHPATTERN_H_

#include "compat/GnuRegex.h"

/// a representation of a refresh pattern. Currently a POD.
class RefreshPattern
{
public:
    const char *pattern;
    regex_t compiled_pattern;
    time_t min;
    double pct;
    time_t max;
    RefreshPattern *next;

    struct {
        bool icase;
        bool refresh_ims;
        bool store_stale;
#if USE_HTTP_VIOLATIONS
        bool override_expire;
        bool override_lastmod;
        bool reload_into_ims;
        bool ignore_reload;
        bool ignore_no_store;
        bool ignore_must_revalidate;
        bool ignore_private;
#endif
    } flags;
    int max_stale;

    // statistics about how many matches this pattern has had
    mutable struct stats_ {
        uint64_t matchTests;
        uint64_t matchCount;
        // TODO: some stats to indicate how useful/less the flags are would be nice.
    } stats;
};

#endif /* SQUID_REFRESHPATTERN_H_ */

