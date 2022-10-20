/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __BPF_CSUM_H_
#define __BPF_CSUM_H_

#include "compiler.h"
#include "helpers.h"

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/csum.h
 Startline: 10
 Endline: 15 
 Funcname: csum_fold 
 Input: (__wsum csum) 
 Output: __sum16 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __sum16 csum_fold(__wsum csum)
{
	csum = (csum & 0xffff) + (csum >> 16);
	csum = (csum & 0xffff) + (csum >> 16);
	return (__sum16)~csum;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/csum.h
 Startline: 17
 Endline: 20 
 Funcname: csum_unfold 
 Input: (__sum16 csum) 
 Output: __wsum 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __wsum csum_unfold(__sum16 csum)
{
	return (__wsum)csum;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/csum.h
 Startline: 22
 Endline: 26 
 Funcname: csum_add 
 Input: (__wsum csum, __wsum addend) 
 Output: __wsum 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __wsum csum_add(__wsum csum, __wsum addend)
{
	csum += addend;
	return csum + (csum < addend);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/csum.h
 Startline: 28
 Endline: 31 
 Funcname: csum_sub 
 Input: (__wsum csum, __wsum addend) 
 Output: __wsum 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __wsum csum_sub(__wsum csum, __wsum addend)
{
	return csum_add(csum, ~addend);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/csum.h
 Startline: 33
 Endline: 52 
 Funcname: csum_diff 
 Input: (const void *from, __u32 size_from, const void *to, __u32 size_to, __u32 seed) 
 Output: __wsum 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __wsum csum_diff(const void *from, __u32 size_from,
					const void *to,   __u32 size_to,
					__u32 seed)
{
	if (__builtin_constant_p(size_from) &&
	    __builtin_constant_p(size_to)) {
		/* Optimizations for frequent hot-path cases that are tiny to just
		 * inline into the code instead of calling more expensive helper.
		 */
		if (size_from == 4 && size_to == 4 &&
		    __builtin_constant_p(seed) && seed == 0)
			return csum_add(~(*(__u32 *)from), *(__u32 *)to);
		if (size_from == 4 && size_to == 4)
			return csum_add(seed,
					csum_add(~(*(__u32 *)from),
						 *(__u32 *)to));
	}

	return csum_diff_external(from, size_from, to, size_to, seed);
}

#endif /* __BPF_CSUM_H_ */
