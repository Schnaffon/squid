/*
 * Copyright (C) 1996-2014 The Squid Software Foundation and contributors
 *
 * Squid software is distributed under GPLv2+ license and includes
 * contributions from numerous individuals and organizations.
 * Please see the COPYING and CONTRIBUTORS files for details.
 */

#ifndef _SQUID_SSL_PEEKER_H
#define _SQUID_SSL_PEEKER_H

#include "base/AsyncJob.h"
#include "base/CbcPointer.h"
#include "comm/forward.h"
#include "HttpRequest.h"
#include "ip/Address.h"

class ConnStateData;
class store_client;

namespace Ssl
{

/**
  \ingroup ServerProtocolSSLAPI
 * Maintains bump-server-first related information.
 */
class ServerBump
{
public:
    explicit ServerBump(HttpRequest *fakeRequest, StoreEntry *e = NULL, Ssl::BumpMode mode = Ssl::bumpServerFirst);
    ~ServerBump();

    /// faked, minimal request; required by server-side API
    HttpRequest::Pointer request;
    StoreEntry *entry; ///< for receiving Squid-generated error messages
    Ssl::X509_Pointer serverCert; ///< HTTPS server certificate
    Ssl::CertErrors *sslErrors; ///< SSL [certificate validation] errors
    Ssl::BumpMode mode; ///< The SSL server bump mode
    Ssl::BumpStep step; ///< The SSL server bumping step
    SBuf clientSni; ///< the SSL client SNI name

private:
    store_client *sc; ///< dummy client to prevent entry trimming

    CBDATA_CLASS2(ServerBump);
};

} // namespace Ssl

#endif
