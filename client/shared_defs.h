/**
 * \file
 * Macros shared between the server and client.
 *
 * \note The server's parser is kinda sensitive about the format of this file,
 * don't use any fancy preprocessor stuff.
 *
 * (only lines matching \verbatim /^\s*#define\s[A-Z_0-9]+\s\S+.*$/ \endverbatim are used).
 */

#ifndef BONSAI_CLIENT_SHARED_DEFS_H
#define BONSAI_CLIENT_SHARED_DEFS_H

#define DEFAULT_PORT 23232
#define CLIENT_VERSION_MAJOR 0
#define CLIENT_VERSION_MINOR 1
#define CLIENT_VERSION_REV 0

#endif
