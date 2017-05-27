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

static int constant_time_compare(unsigned char *a, unsigned char *b, size_t length);

/*
 * bearssl (sha224|sha256|sha384|sha512) data ?data ...?
 *
 *      Calculates the given hash of the given input arguments. Both inputs and outputs are
 *      byte array objects. Expects the clientData to be a br_hash_class pointer.
 */
int hash_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const br_hash_class *hash = (const br_hash_class *) clientData;
    br_hash_compat_context ctx;
    unsigned char output[br_sha512_SIZE]; // Allocate to largest possible size for simplicity
    unsigned char *data;
    size_t output_size;
    int i, length;

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "value ?value...?");
        return TCL_ERROR;
    }

    hash->init(&ctx.vtable);
    for (i = 1; i < objc; ++i) {
        data = Tcl_GetByteArrayFromObj(objv[i], &length);
        hash->update(&ctx.vtable, data, length);
    }
    hash->out(&ctx.vtable, output);
    output_size = (hash->desc >> BR_HASHDESC_OUT_OFF) & BR_HASHDESC_OUT_MASK;

    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(output, output_size));
    return TCL_OK;
}

int hmac_cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    UNUSED(clientData);
    br_hmac_key_context key_ctx;
    br_hmac_context ctx;
    unsigned char* key;
    unsigned char* data;
    unsigned char* expected = NULL;
    int key_length, data_length, expected_length;
    int output_size;
    unsigned char output[br_sha512_SIZE]; // Allocate for largest possible output for simplicity

    static const struct {
        char *digest_name;
        const br_hash_class *digest_vtable;
    } options[] = {
        { "sha224", &br_sha224_vtable },
        { "sha256", &br_sha256_vtable },
        { "sha384", &br_sha384_vtable },
        { "sha512", &br_sha512_vtable },
        { NULL, NULL }
    };
    int index;
    int comparison_result;

    if (objc < 4 || objc > 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "(sha224|sha256|sha384|sha512) key value ?expected?");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObjStruct(interp, objv[1], options, sizeof(options[0]), "digest", 0, &index) != TCL_OK) {
        return TCL_ERROR;
    }

    key = Tcl_GetByteArrayFromObj(objv[2], &key_length);
    data = Tcl_GetByteArrayFromObj(objv[3], &data_length);

    br_hmac_key_init(&key_ctx, options[index].digest_vtable, key, key_length);
    br_hmac_init(&ctx, &key_ctx, 0); // 0 = natural output length of the underlying hash
    br_hmac_update(&ctx, data, data_length);
    br_hmac_out(&ctx, &output);
    output_size = br_hmac_size(&ctx);

    if (objc == 5) {
        expected = Tcl_GetByteArrayFromObj(objv[4], &expected_length);
        if (expected_length > output_size) {
            Tcl_SetObjResult(interp, Tcl_NewStringObj("expected value is too big for hash size", -1));
            return TCL_ERROR;
        }
        comparison_result = constant_time_compare(output, expected, expected_length);
        Tcl_SetObjResult(interp, Tcl_NewBooleanObj(comparison_result));
    } else {
        Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(output, output_size));
    }

    return TCL_OK;
}

/**
 * Implements a constant-time comparison of two byte arrays that must both be >= length in size.
 * This is very important to avoid timing attacks on HMAC comparisons.
 */
static int constant_time_compare(unsigned char *a, unsigned char *b, size_t length)
{
    int result = 0;
    size_t i;

    for (i = 0; i < length; ++i) {
        result |= a[i] ^ b[i];
    }

    return 1 & ((result - 1) >> 8);
}
