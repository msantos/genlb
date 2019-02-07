/*
 * Copyright (C) 2012-2013 Codership Oy <info@codership.com>
 *
 * $Id$
 */

#ifndef _glb_cnf_h_
#define _glb_cnf_h_

#include "glb_dst.h"
#include "glb_time.h"

#include <stdbool.h>    // for bool
#include <stdio.h>      // for FILE and fprintf()

typedef enum glb_policy
{
    GLB_POLICY_LEAST = 0, /* least connected     */
    GLB_POLICY_ROUND,     /* round-robin         */
    GLB_POLICY_SINGLE,    /* single dest. with the top weight */
    GLB_POLICY_RANDOM,    /* random choice       */
    GLB_POLICY_SOURCE     /* same dest. for same source */
} glb_policy_t;

#define GLB_POLICY_MAX (GLB_POLICY_SOURCE + 1)

typedef struct glb_cnf
{
    glb_sockaddr_t inc_addr;     // IP to bind listener for incoming connecitons
    glb_sockaddr_t ctrl_addr;    // network control interface
    const char*    watchdog;     // watchdog spec string
    glb_time_t     interval;     // health check interval (nanoseconds)
    glb_time_t     extra;        // extra check interval (nanoseconds)
#ifdef GLBD
    const char*    fifo_name;    // FIFO file name
    int            n_threads;    // number of routing threads (1 .. oo)
    int            max_conn;     // max allowed client connections
    bool           nodelay;      // use TCP_NODELAY?
    bool           keepalive;    // use SO_KEEPALIVE?
    bool           linger;       // use SO_LINGER?
    bool           reuseaddr;    // use SO_REUSEADDR?
    bool           defer_accept; // use TCP_DEFER_ACCEPT?
    bool           daemonize;    // become a daemon?
    bool           synchronous;  // connect synchronously
#endif /* GLBD */
    bool           verbose;      // be verbose?
    bool           discover;     // automatically discover new destinations
    bool           top;          // only use top weighted destinations
    bool           ctrl_set;     // was set? (false)
    int            lat_factor;   // smoothe latency over that many samples
    glb_policy_t   policy;       // algorithm to use for load-balancing
    size_t         n_dst;        // number of destinations
    glb_dst_t      dst[];        // destination descriptions
} glb_cnf_t;

extern glb_cnf_t*
glb_cnf_init ();

/*!
 * parses array list of destinations, returns updated cnf structure.
 */
extern glb_cnf_t*
glb_parse_dst_list (const char* const dst_list[],
                    int         const n_dst,
                    uint16_t    const default_port,
                    glb_cnf_t*  const in);

/*!
 * parses [addr:]port
 *
 * depending on the purpose default address can be e.g. 127.0.0.1 or 0.0.0.0
 * (for listening socket)
 */
extern int
glb_parse_addr (glb_sockaddr_t* addr,
                const char*     str,
                const char*     default_addr);

extern void
glb_print_version (FILE* out);

extern void
glb_cnf_print (FILE* out, const glb_cnf_t* cnf);

#endif // _glb_cnf_h_
