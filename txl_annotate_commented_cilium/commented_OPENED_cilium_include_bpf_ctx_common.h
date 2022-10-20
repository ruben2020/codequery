/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __BPF_CTX_COMMON_H_
#define __BPF_CTX_COMMON_H_

#include <linux/types.h>
#include <linux/bpf.h>

#include "../compiler.h"
#include "../errno.h"

#define __ctx_skb		1
#define __ctx_xdp		2

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/ctx/common.h
 Startline: 16
 Endline: 19 
 Funcname: *ctx_data 
 Input: (const struct __ctx_buff *ctx) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void *ctx_data(const struct __ctx_buff *ctx)
{
	return (void *)(unsigned long)ctx->data;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/ctx/common.h
 Startline: 21
 Endline: 24 
 Funcname: *ctx_data_meta 
 Input: (const struct __ctx_buff *ctx) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void *ctx_data_meta(const struct __ctx_buff *ctx)
{
	return (void *)(unsigned long)ctx->data_meta;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/ctx/common.h
 Startline: 26
 Endline: 29 
 Funcname: *ctx_data_end 
 Input: (const struct __ctx_buff *ctx) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void *ctx_data_end(const struct __ctx_buff *ctx)
{
	return (void *)(unsigned long)ctx->data_end;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/include/bpf/ctx/common.h
 Startline: 31
 Endline: 34 
 Funcname: ctx_no_room 
 Input: (const void *needed, const void *limit) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool ctx_no_room(const void *needed, const void *limit)
{
	return unlikely(needed > limit);
}

#endif /* __BPF_CTX_COMMON_H_ */
