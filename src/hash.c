/*
 * Copyright (c) 2017 Neil Madden.
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

#include "hash.h"
#include <bearssl_hash.h>
#include <bearssl_hmac.h>

#define UNUSED(x) (void)(x)

/*
 * bearssl sha224 data ?data...?
 *
 *      Calculates the SHA-224 hash of (the concatenation of) the input arguments. Both inputs
 *      and outputs are byte array objects.
 */
int sha224_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    br_sha224_context ctx;
    int length;
    int i;
    unsigned char *data;
    unsigned char hash[br_sha224_SIZE];

    if (objc < 2) {
        Tcl_SetObjResult(interp, 
                Tcl_NewStringObj("wrong # args: should be \"bearssl sha224 value ?value...?\"", -1));
        return TCL_ERROR;
    }

    br_sha224_init(&ctx);
    for (i = 1; i < objc; ++i) {
        data = Tcl_GetByteArrayFromObj(objv[i], &length);
        br_sha224_update(&ctx, data, length);
    }
    br_sha224_out(&ctx, hash);

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(hash, br_sha224_SIZE));
    return TCL_OK;
}

/*
 * bearssl sha256 data ?data...?
 *
 *      Calculates the SHA-256 hash of (the concatenation of) the input arguments. Both inputs
 *      and outputs are byte array objects.
 */
int sha256_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    br_sha256_context ctx;
    int length;
    int i;
    unsigned char *data;
    unsigned char hash[br_sha256_SIZE];

    if (objc < 2) {
        Tcl_SetObjResult(interp, 
                Tcl_NewStringObj("wrong # args: should be \"bearssl sha256 value ?value...?\"", -1));
        return TCL_ERROR;
    }

    br_sha256_init(&ctx);
    for (i = 1; i < objc; ++i) {
        data = Tcl_GetByteArrayFromObj(objv[i], &length);
        br_sha256_update(&ctx, data, length);
    }
    br_sha256_out(&ctx, hash);

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(hash, br_sha256_SIZE));
    return TCL_OK;
}
/*
 * bearssl sha384 data ?data...?
 *
 *      Calculates the SHA-384 hash of (the concatenation of) the input arguments. Both inputs
 *      and outputs are byte array objects.
 */
int sha384_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    br_sha384_context ctx;
    int length;
    int i;
    unsigned char *data;
    unsigned char hash[br_sha384_SIZE];

    if (objc < 2) {
        Tcl_SetObjResult(interp, 
                Tcl_NewStringObj("wrong # args: should be \"bearssl sha384 value ?value...?\"", -1));
        return TCL_ERROR;
    }

    br_sha384_init(&ctx);
    for (i = 1; i < objc; ++i) {
        data = Tcl_GetByteArrayFromObj(objv[i], &length);
        br_sha384_update(&ctx, data, length);
    }
    br_sha384_out(&ctx, hash);

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(hash, br_sha384_SIZE));
    return TCL_OK;
}



/*
 * bearssl sha512 data ?data...?
 *
 *      Calculates the SHA-512 hash of (the concatenation of) the input arguments. Both inputs
 *      and outputs are byte array objects.
 */
int sha512_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    br_sha512_context ctx;
    int length;
    int i;
    unsigned char *data;
    unsigned char hash[br_sha512_SIZE];

    if (objc < 2) {
        Tcl_SetObjResult(interp, 
                Tcl_NewStringObj("wrong # args: should be \"bearssl sha512 value ?value...?\"", -1));
        return TCL_ERROR;
    }

    br_sha512_init(&ctx);
    for (i = 1; i < objc; ++i) {
        data = Tcl_GetByteArrayFromObj(objv[i], &length);
        br_sha512_update(&ctx, data, length);
    }
    br_sha512_out(&ctx, hash);

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(hash, br_sha512_SIZE));
    return TCL_OK;
}

int hmac_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    UNUSED(objv);

    if (objc < 4) {
        Tcl_SetObjResult(interp, 
                Tcl_NewStringObj("wrong # args: should be \"bearssl hmac (sha224|sha256|sha384|sha512) key data ?data...?\"", -1));
        return TCL_ERROR;
    }

    return TCL_OK;
}
