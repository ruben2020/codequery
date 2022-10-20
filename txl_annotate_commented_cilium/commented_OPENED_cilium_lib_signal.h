/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __LIB_SIGNAL_H_
#define __LIB_SIGNAL_H_

#include <bpf/api.h>

struct {
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
	__uint(key_size, sizeof(__u32));
	__uint(value_size, sizeof(__u32));
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, __NR_CPUS__);
} SIGNAL_MAP __section_maps_btf;

enum {
	SIGNAL_NAT_FILL_UP = 0,
	SIGNAL_CT_FILL_UP,
};

enum {
	SIGNAL_PROTO_V4 = 0,
	SIGNAL_PROTO_V6,
};

struct signal_msg {
	__u32 signal_nr;
	union {
		struct {
			__u32 proto;
		};
	};
};

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/signal.h
 Startline: 36
 Endline: 41 
 Funcname: send_signal 
 Input: (struct __ctx_buff *ctx, struct signal_msg *msg) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void send_signal(struct __ctx_buff *ctx,
					struct signal_msg *msg)
{
	ctx_event_output(ctx, &SIGNAL_MAP, BPF_F_CURRENT_CPU,
			 msg, sizeof(*msg));
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/signal.h
 Startline: 43
 Endline: 52 
 Funcname: send_signal_nat_fill_up 
 Input: (struct __ctx_buff *ctx, __u32 proto) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void send_signal_nat_fill_up(struct __ctx_buff *ctx,
						    __u32 proto)
{
	struct signal_msg msg = {
		.signal_nr	= SIGNAL_NAT_FILL_UP,
		.proto		= proto,
	};

	send_signal(ctx, &msg);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/signal.h
 Startline: 54
 Endline: 63 
 Funcname: send_signal_ct_fill_up 
 Input: (struct __ctx_buff *ctx, __u32 proto) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void send_signal_ct_fill_up(struct __ctx_buff *ctx,
						   __u32 proto)
{
	struct signal_msg msg = {
		.signal_nr	= SIGNAL_CT_FILL_UP,
		.proto		= proto,
	};

	send_signal(ctx, &msg);
}

#endif /* __LIB_SIGNAL_H_ */
