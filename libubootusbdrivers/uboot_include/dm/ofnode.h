/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2017 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 */

#ifndef _DM_OFNODE_H
#define _DM_OFNODE_H

#include <dm/of.h>

/* Enable checks to protect against invalid calls */
#undef OF_CHECKS

struct resource;

/**
 * typedef union ofnode_union ofnode - reference to a device tree node
 *
 * This union can hold either a straightforward pointer to a struct device_node
 * in the live device tree, or an offset within the flat device tree. In the
 * latter case, the pointer value is just the integer offset within the flat DT.
 *
 * Thus we can reference nodes in both the live tree (once available) and the
 * flat tree (until then). Functions are available to translate between an
 * ofnode and either an offset or a `struct device_node *`.
 *
 * The reference can also hold a null offset, in which case the pointer value
 * here is NULL. This corresponds to a struct device_node * value of
 * NULL, or an offset of -1.
 *
 * There is no ambiguity as to whether ofnode holds an offset or a node
 * pointer: when the live tree is active it holds a node pointer, otherwise it
 * holds an offset. The value itself does not need to be unique and in theory
 * the same value could point to a valid device node or a valid offset. We
 * could arrange for a unique value to be used (e.g. by making the pointer
 * point to an offset within the flat device tree in the case of an offset) but
 * this increases code size slightly due to the subtraction. Since it offers no
 * real benefit, the approach described here seems best.
 *
 * For now these points use constant types, since we don't allow writing
 * the DT.
 *
 * @np: Pointer to device node, used for live tree
 * @of_offset: Pointer into flat device tree, used for flat tree. Note that this
 *	is not a really a pointer to a node: it is an offset value. See above.
 */
typedef union ofnode_union {
	const struct device_node *np;
	long of_offset;
} ofnode;

struct ofnode_phandle_args {
	ofnode node;
	int args_count;
	uint32_t args[OF_MAX_PHANDLE_ARGS];
};

/**
 * struct ofprop - reference to a property of a device tree node
 *
 * This struct hold the reference on one property of one node,
 * using struct ofnode and an offset within the flat device tree or either
 * a pointer to a struct property in the live device tree.
 *
 * Thus we can reference arguments in both the live tree and the flat tree.
 *
 * The property reference can also hold a null reference. This corresponds to
 * a struct property NULL pointer or an offset of -1.
 *
 * @node: Pointer to device node
 * @offset: Pointer into flat device tree, used for flat tree.
 * @prop: Pointer to property, used for live treee.
 */

struct ofprop {
	ofnode node;
	union {
		int offset;
		const struct property *prop;
	};
};

/**
 * ofnode_null() - Obtain a null ofnode
 *
 * This returns an ofnode which points to no node. It works both with the flat
 * tree and livetree.
 */
static inline ofnode ofnode_null(void)
{
	ofnode node;

	if (of_live_active())
		node.np = NULL;
	else
		node.of_offset = -1;

	return node;
}

/* Stubs for routines we do not rely upon */

#define ofnode_for_each_subnode(node, parent)	while(false)

#define ofnode_read_u32(...)		 			0


#endif