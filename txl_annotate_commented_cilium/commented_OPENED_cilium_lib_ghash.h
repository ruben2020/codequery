/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2002-2020 Authors of the Linux kernel */
/* Copyright Authors of Cilium */

#ifndef __GHASH_H_
#define __GHASH_H_

#include <bpf/ctx/ctx.h>
#include <bpf/api.h>

/*
 * This hash multiplies the input by a large odd number and takes the
 * high bits.  Since multiplication propagates changes to the most
 * significant end only, it is essential that the high bits of the
 * product be used for the hash value.
 *
 * Chuck Lever verified the effectiveness of this technique:
 * http://www.citi.umich.edu/techreports/reports/citi-tr-00-1.pdf
 *
 * Although a random odd number will do, it turns out that the golden
 * ratio phi = (sqrt(5)-1)/2, or its negative, has particularly nice
 * properties.  (See Knuth vol 3, section 6.4, exercise 9.)
 *
 * These are the negative, (1 - phi) = phi**2 = (3 - sqrt(5))/2,
 * which is very slightly easier to multiply by and makes no
 * difference to the hash distribution.
 */
#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ghash.h
 Startline: 31
 Endline: 35 
 Funcname: hash_32 
 Input: (__u32 val, __u32 bits) 
 Output: __u32 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __u32 hash_32(__u32 val, __u32 bits)
{
	/* High bits are more random, so use them. */
	return (val * GOLDEN_RATIO_32) >> (32 - bits);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ghash.h
 Startline: 37
 Endline: 40 
 Funcname: hash_64 
 Input: (__u64 val, __u32 bits) 
 Output: __u32 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __u32 hash_64(__u64 val, __u32 bits)
{
	return (val * GOLDEN_RATIO_64) >> (64 - bits);
}

#endif /* __GHASH_H_ */
