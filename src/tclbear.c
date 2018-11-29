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

#include <tcl.h>
#include <bearssl.h>

#include "hash.h"
#include "rand.h"

#define PACKAGE_NAME 	"bearssl"
#define PACKAGE_VERSION "0.4.0"

int DLLEXPORT
Tclbearssl_Init(Tcl_Interp *interp)
{
    Tcl_Namespace *namespace;
    Tcl_Command ensemble;

    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
        return TCL_ERROR;
    }

    namespace = Tcl_CreateNamespace(interp, PACKAGE_NAME, NULL, NULL);
    if (namespace == NULL) {
        return TCL_ERROR;
    }

    /* Register commands */
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::md5", hash_cmd, (ClientData) &br_md5_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::sha1", hash_cmd, (ClientData) &br_sha1_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::sha224", hash_cmd, (ClientData) &br_sha224_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::sha256", hash_cmd, (ClientData) &br_sha256_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::sha384", hash_cmd, (ClientData) &br_sha384_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::sha512", hash_cmd, (ClientData) &br_sha512_vtable, NULL);
    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::hmac", hmac_cmd, NULL, NULL);

    Tcl_CreateObjCommand(interp, PACKAGE_NAME "::rand", rand_cmd, NULL, NULL);

    if (Tcl_Export(interp, namespace, "*", 1) != TCL_OK) {
        return TCL_ERROR;
    }

    ensemble = Tcl_CreateEnsemble(interp, "::" PACKAGE_NAME, namespace, 0);

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    return TCL_OK;
}
