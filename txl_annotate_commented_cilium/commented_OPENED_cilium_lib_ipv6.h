/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __LIB_IPV6__
#define __LIB_IPV6__

#include <linux/ipv6.h>

#include "dbg.h"

#define IPV6_FLOWINFO_MASK              bpf_htonl(0x0FFFFFFF)
#define IPV6_FLOWLABEL_MASK             bpf_htonl(0x000FFFFF)
#define IPV6_FLOWLABEL_STATELESS_FLAG   bpf_htonl(0x00080000)

#define IPV6_TCLASS_MASK (IPV6_FLOWINFO_MASK & ~IPV6_FLOWLABEL_MASK)
#define IPV6_TCLASS_SHIFT       20

/* Number of extension headers that can be skipped */
#define IPV6_MAX_HEADERS 4

#define NEXTHDR_HOP             0       /* Hop-by-hop option header. */
#define NEXTHDR_TCP             6       /* TCP segment. */
#define NEXTHDR_UDP             17      /* UDP message. */
#define NEXTHDR_IPV6            41      /* IPv6 in IPv6 */
#define NEXTHDR_ROUTING         43      /* Routing header. */
#define NEXTHDR_FRAGMENT        44      /* Fragmentation/reassembly header. */
#define NEXTHDR_GRE             47      /* GRE header. */
#define NEXTHDR_ESP             50      /* Encapsulating security payload. */
#define NEXTHDR_AUTH            51      /* Authentication header. */
#define NEXTHDR_ICMP            58      /* ICMP for IPv6. */
#define NEXTHDR_NONE            59      /* No next header */
#define NEXTHDR_DEST            60      /* Destination options header. */
#define NEXTHDR_SCTP            132     /* SCTP message. */
#define NEXTHDR_MOBILITY        135     /* Mobility header. */

#define NEXTHDR_MAX             255

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 38
 Endline: 41 
 Funcname: ipv6_optlen 
 Input: (const struct ipv6_opt_hdr *opthdr) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_optlen(const struct ipv6_opt_hdr *opthdr)
{
	return (opthdr->hdrlen + 1) << 3;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 43
 Endline: 46 
 Funcname: ipv6_authlen 
 Input: (const struct ipv6_opt_hdr *opthdr) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_authlen(const struct ipv6_opt_hdr *opthdr)
{
	return (opthdr->hdrlen + 2) << 2;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 48
 Endline: 85 
 Funcname: ipv6_hdrlen 
 Input: (struct __ctx_buff *ctx, __u8 *nexthdr) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_hdrlen(struct __ctx_buff *ctx, __u8 *nexthdr)
{
	int i, len = sizeof(struct ipv6hdr);
	struct ipv6_opt_hdr opthdr __align_stack_8;
	__u8 nh = *nexthdr;

#pragma unroll
	for (i = 0; i < IPV6_MAX_HEADERS; i++) {
		switch (nh) {
		case NEXTHDR_NONE:
			return DROP_INVALID_EXTHDR;

		case NEXTHDR_FRAGMENT:
			return DROP_FRAG_NOSUPPORT;

		case NEXTHDR_HOP:
		case NEXTHDR_ROUTING:
		case NEXTHDR_AUTH:
		case NEXTHDR_DEST:
			if (ctx_load_bytes(ctx, ETH_HLEN + len, &opthdr, sizeof(opthdr)) < 0)
				return DROP_INVALID;

			nh = opthdr.nexthdr;
			if (nh == NEXTHDR_AUTH)
				len += ipv6_authlen(&opthdr);
			else
				len += ipv6_optlen(&opthdr);
			break;

		default:
			*nexthdr = nh;
			return len;
		}
	}

	/* Reached limit of supported extension headers */
	return DROP_INVALID_EXTHDR;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 87
 Endline: 92 
 Funcname: ipv6_addr_copy 
 Input: (union v6addr *dst, const union v6addr *src) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void ipv6_addr_copy(union v6addr *dst,
					   const union v6addr *src)
{
	dst->d1 = src->d1;
	dst->d2 = src->d2;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 94
 Endline: 103 
 Funcname: ipv6_addrcmp 
 Input: (const union v6addr *a, const union v6addr *b) 
 Output: __u64 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __u64 ipv6_addrcmp(const union v6addr *a,
					  const union v6addr *b)
{
	__u64 tmp;

	tmp = a->d1 - b->d1;
	if (!tmp)
		tmp = a->d2 - b->d2;
	return tmp;
}

/* Only works with contiguous masks. */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 106
 Endline: 116 
 Funcname: ipv6_addr_in_net 
 Input: (const union v6addr *addr, const union v6addr *net, const union v6addr *mask) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_addr_in_net(const union v6addr *addr,
					    const union v6addr *net,
					    const union v6addr *mask)
{
	return ((addr->p1 & mask->p1) == net->p1)
		&& (!mask->p2
		    || (((addr->p2 & mask->p2) == net->p2)
			&& (!mask->p3
			    || (((addr->p3 & mask->p3) == net->p3)
				&& (!mask->p4 || ((addr->p4 & mask->p4) == net->p4))))));
}

#define GET_PREFIX(PREFIX)						\
	bpf_htonl(PREFIX <= 0 ? 0 : PREFIX < 32 ? ((1<<PREFIX) - 1) << (32-PREFIX)	\
			      : 0xFFFFFFFF)

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 122
 Endline: 132 
 Funcname: ipv6_addr_clear_suffix 
 Input: (union v6addr *addr, int prefix) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void ipv6_addr_clear_suffix(union v6addr *addr,
						   int prefix)
{
	addr->p1 &= GET_PREFIX(prefix);
	prefix -= 32;
	addr->p2 &= GET_PREFIX(prefix);
	prefix -= 32;
	addr->p3 &= GET_PREFIX(prefix);
	prefix -= 32;
	addr->p4 &= GET_PREFIX(prefix);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 134
 Endline: 144 
 Funcname: ipv6_match_prefix_64 
 Input: (const union v6addr *addr, const union v6addr *prefix) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_match_prefix_64(const union v6addr *addr,
						const union v6addr *prefix)
{
	int tmp;

	tmp = addr->p1 - prefix->p1;
	if (!tmp)
		tmp = addr->p2 - prefix->p2;

	return !tmp;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 146
 Endline: 161 
 Funcname: ipv6_dec_hoplimit 
 Input: (struct __ctx_buff *ctx, int off) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_dec_hoplimit(struct __ctx_buff *ctx, int off)
{
	__u8 hl;

	if (ctx_load_bytes(ctx, off + offsetof(struct ipv6hdr, hop_limit),
			   &hl, sizeof(hl)) < 0)
		return DROP_INVALID;

	if (hl <= 1)
		return 1;
	hl--;
	if (ctx_store_bytes(ctx, off + offsetof(struct ipv6hdr, hop_limit),
			    &hl, sizeof(hl), BPF_F_RECOMPUTE_CSUM) < 0)
		return DROP_WRITE_ERROR;
	return 0;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 163
 Endline: 168 
 Funcname: ipv6_load_saddr 
 Input: (struct __ctx_buff *ctx, int off, union v6addr *dst) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_load_saddr(struct __ctx_buff *ctx, int off,
					   union v6addr *dst)
{
	return ctx_load_bytes(ctx, off + offsetof(struct ipv6hdr, saddr), dst->addr,
			      sizeof(((struct ipv6hdr *)NULL)->saddr));
}

/* Assumes that caller fixes checksum csum_diff() and l4_csum_replace() */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 171
 Endline: 175 
 Funcname: ipv6_store_saddr 
 Input: (struct __ctx_buff *ctx, __u8 *addr, int off) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_store_saddr(struct __ctx_buff *ctx, __u8 *addr,
					    int off)
{
	return ctx_store_bytes(ctx, off + offsetof(struct ipv6hdr, saddr), addr, 16, 0);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 177
 Endline: 182 
 Funcname: ipv6_load_daddr 
 Input: (struct __ctx_buff *ctx, int off, union v6addr *dst) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_load_daddr(struct __ctx_buff *ctx, int off,
					   union v6addr *dst)
{
	return ctx_load_bytes(ctx, off + offsetof(struct ipv6hdr, daddr), dst->addr,
			      sizeof(((struct ipv6hdr *)NULL)->daddr));
}

/* Assumes that caller fixes checksum csum_diff() and l4_csum_replace() */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 185
 Endline: 189 
 Funcname: ipv6_store_daddr 
 Input: (struct __ctx_buff *ctx, const __u8 *addr, int off) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int
ipv6_store_daddr(struct __ctx_buff *ctx, const __u8 *addr, int off)
{
	return ctx_store_bytes(ctx, off + offsetof(struct ipv6hdr, daddr), addr, 16, 0);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 191
 Endline: 196 
 Funcname: ipv6_load_nexthdr 
 Input: (struct __ctx_buff *ctx, int off, __u8 *nexthdr) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_load_nexthdr(struct __ctx_buff *ctx, int off,
					     __u8 *nexthdr)
{
	return ctx_load_bytes(ctx, off + offsetof(struct ipv6hdr, nexthdr), nexthdr,
			      sizeof(__u8));
}

/* Assumes that caller fixes checksum csum_diff() and l4_csum_replace() */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 199
 Endline: 204 
 Funcname: ipv6_store_nexthdr 
 Input: (struct __ctx_buff *ctx, __u8 *nexthdr, int off) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_store_nexthdr(struct __ctx_buff *ctx, __u8 *nexthdr,
					      int off)
{
	return ctx_store_bytes(ctx, off + offsetof(struct ipv6hdr, nexthdr), nexthdr,
			      sizeof(__u8), 0);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 206
 Endline: 211 
 Funcname: ipv6_load_paylen 
 Input: (struct __ctx_buff *ctx, int off, __be16 *len) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_load_paylen(struct __ctx_buff *ctx, int off,
					    __be16 *len)
{
	return ctx_load_bytes(ctx, off + offsetof(struct ipv6hdr, payload_len),
			      len, sizeof(*len));
}

/* Assumes that caller fixes checksum csum_diff() and l4_csum_replace() */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 214
 Endline: 219 
 Funcname: ipv6_store_paylen 
 Input: (struct __ctx_buff *ctx, int off, __be16 *len) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_store_paylen(struct __ctx_buff *ctx, int off,
					     __be16 *len)
{
	return ctx_store_bytes(ctx, off + offsetof(struct ipv6hdr, payload_len),
			       len, sizeof(*len), 0);
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 221
 Endline: 237 
 Funcname: ipv6_store_flowlabel 
 Input: (struct __ctx_buff *ctx, int off, __be32 label) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_store_flowlabel(struct __ctx_buff *ctx, int off,
						__be32 label)
{
	__be32 old;

	/* use traffic class from packet */
	if (ctx_load_bytes(ctx, off, &old, 4) < 0)
		return DROP_INVALID;

	old &= IPV6_TCLASS_MASK;
	old = bpf_htonl(0x60000000) | label | old;

	if (ctx_store_bytes(ctx, off, &old, 4, BPF_F_RECOMPUTE_CSUM) < 0)
		return DROP_WRITE_ERROR;

	return 0;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 239
 Endline: 252 
 Funcname: ipv6_pseudohdr_checksum 
 Input: (struct ipv6hdr *hdr, __u8 next_hdr, __u16 payload_len, __be32 sum) 
 Output: __be32 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __be32 ipv6_pseudohdr_checksum(struct ipv6hdr *hdr,
						      __u8 next_hdr,
						      __u16 payload_len, __be32 sum)
{
	__be32 len = bpf_htonl((__u32)payload_len);
	__be32 nexthdr = bpf_htonl((__u32)next_hdr);

	sum = csum_diff(NULL, 0, &hdr->saddr, sizeof(struct in6_addr), sum);
	sum = csum_diff(NULL, 0, &hdr->daddr, sizeof(struct in6_addr), sum);
	sum = csum_diff(NULL, 0, &len, sizeof(len), sum);
	sum = csum_diff(NULL, 0, &nexthdr, sizeof(nexthdr), sum);

	return sum;
}

/*
 * Ipv4 mapped address - 0:0:0:0:0:FFFF::/96
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/ipv6.h
 Startline: 257
 Endline: 260 
 Funcname: ipv6_addr_is_mapped 
 Input: (const union v6addr *addr) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int ipv6_addr_is_mapped(const union v6addr *addr)
{
	return addr->p1 == 0 && addr->p2 == 0 && addr->p3 == 0xFFFF0000;
}
#endif /* __LIB_IPV6__ */
