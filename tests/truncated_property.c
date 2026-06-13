// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * libfdt - Flat Device Tree manipulation
 *	Testcase for misbehaviour on a truncated property
 * Copyright (C) 2006 David Gibson, IBM Corporation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libfdt.h>

#include "tests.h"

int main(int argc, char *argv[])
{
	void *fdt;
	const void *prop;
	int len;

	test_init(argc, argv);
	fdt = load_blob_arg(argc, argv);

	prop = fdt_getprop(fdt, 0, "truncated", &len);
	if (prop)
		FAIL("fdt_getprop() succeeded on truncated property");
	if (len != -FDT_ERR_BADSTRUCTURE)
		FAIL("fdt_getprop() failed with \"%s\" instead of \"%s\"",
		     fdt_strerror(len), fdt_strerror(-FDT_ERR_BADSTRUCTURE));

	PASS();
}
