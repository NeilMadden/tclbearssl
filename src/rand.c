/*
 * Copyright (c) 2018 Neil Madden.
 *
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "rand.h"
#include <bearssl_rand.h>
#include <bearssl_hash.h>

#define UNUSED(x) (void)(x)

static Tcl_ThreadDataKey threadDrbgCtx;
static const char * SEED_PERSONALISATION = "Tcl_BearSSL";

typedef struct {
    br_hmac_drbg_context drbgCtx;
    int initialized;
} random_context_t;

int rand_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    random_context_t *ctx;
    br_prng_seeder seeder = 0;
    int numBytes;
    unsigned char *buf;
    char *subCmd;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "numBytes|reseed");
        return TCL_ERROR;
    }

    /* Get thread-local DRBG context and initialize if necessary. */
    ctx = Tcl_GetThreadData(&threadDrbgCtx, sizeof(random_context_t));
    if (!ctx->initialized) {
        br_hmac_drbg_init(&(ctx->drbgCtx), &br_sha512_vtable, SEED_PERSONALISATION, sizeof(SEED_PERSONALISATION));
        seeder = br_prng_seeder_system(NULL);
        if (seeder != 0) {
            (seeder)(&(ctx->drbgCtx.vtable));
        }
        ctx->initialized = 1;
    }

    subCmd = Tcl_GetStringFromObj(objv[1], &numBytes);
    if (strncmp("reseed", subCmd, numBytes) == 0) {
        if (seeder == 0) {
            seeder = br_prng_seeder_system(NULL);
        }
        if (seeder != 0) {
            (seeder)(&(ctx->drbgCtx.vtable));
            return TCL_OK;
        } else {
            Tcl_SetObjResult(interp, Tcl_NewStringObj("unable to reseed DRBG", -1));
            return TCL_ERROR;
        }
    }

    
    if (Tcl_GetIntFromObj(interp, objv[1], &numBytes) != TCL_OK) {
        return TCL_ERROR;
    }

    if (numBytes <= 0 || numBytes > 65536) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("numBytes must be greater than 0 and at most 65536", -1));
        return TCL_ERROR;
    }

        buf = (unsigned char *) ckalloc(numBytes);
    br_hmac_drbg_generate(&(ctx->drbgCtx), buf, numBytes);

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(buf, numBytes));
    ckfree((void *) buf);
    return TCL_OK;
}
