/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __LIB_PROXY_HAIRPIN_H_
#define __LIB_PROXY_HAIRPIN_H_

#include "common.h"
#include "utils.h"
#include "ipv6.h"
#include "ipv4.h"
#include "eth.h"
#include "dbg.h"
#include "trace.h"
#include "csum.h"
#include "l4.h"

#if defined(HOST_IFINDEX_MAC) && defined(HOST_IFINDEX)
/**
 * ctx_redirect_to_proxy_hairpin redirects to the proxy by hairpining the
 * packet out the incoming interface
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/proxy_hairpin.h
 Startline: 22
 Endline: 63 
 Funcname: ctx_redirect_to_proxy_hairpin 
 Input: (struct __ctx_buff *ctx, __be16 proxy_port, const bool is_ipv6) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int
ctx_redirect_to_proxy_hairpin(struct __ctx_buff *ctx, __be16 proxy_port, const bool is_ipv6)
{
#if defined(ENABLE_IPV4) || defined(ENABLE_IPV6)
	union macaddr host_mac = HOST_IFINDEX_MAC;
	union macaddr router_mac = NODE_MAC;
#endif
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct iphdr *ip4;
	int ret = 0;

	ctx_store_meta(ctx, CB_PROXY_MAGIC,
		       MARK_MAGIC_TO_PROXY | (proxy_port << 16));
	bpf_barrier(); /* verifier workaround */

	if (!revalidate_data(ctx, &data, &data_end, &ip4))
		return DROP_INVALID;

	if (is_ipv6) {
#ifdef ENABLE_IPV6
		ret = ipv6_l3(ctx, ETH_HLEN, (__u8 *)&router_mac, (__u8 *)&host_mac,
			      METRIC_EGRESS);
#endif
	} else {
#ifdef ENABLE_IPV4
		ret = ipv4_l3(ctx, ETH_HLEN, (__u8 *)&router_mac, (__u8 *)&host_mac,
			      ip4);
#endif
	}
	if (IS_ERR(ret))
		return ret;

	cilium_dbg(ctx, DBG_CAPTURE_PROXY_PRE, proxy_port, 0);

	/* Note that the actual __ctx_buff preparation for submitting the
	 * packet to the proxy will occur in a subsequent program via
	 * ctx_redirect_to_proxy_first().
	 */

	return ctx_redirect(ctx, HOST_IFINDEX, 0);
}

#ifdef ENABLE_IPV4
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/proxy_hairpin.h
 Startline: 66
 Endline: 70 
 Funcname: ctx_redirect_to_proxy_hairpin_ipv4 
 Input: (struct __ctx_buff *ctx, __be16 proxy_port) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int
ctx_redirect_to_proxy_hairpin_ipv4(struct __ctx_buff *ctx, __be16 proxy_port)
{
	return ctx_redirect_to_proxy_hairpin(ctx, proxy_port, false);
}
#endif

#ifdef ENABLE_IPV6
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/proxy_hairpin.h
 Startline: 74
 Endline: 78 
 Funcname: ctx_redirect_to_proxy_hairpin_ipv6 
 Input: (struct __ctx_buff *ctx, __be16 proxy_port) 
 Output: int 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline int
ctx_redirect_to_proxy_hairpin_ipv6(struct __ctx_buff *ctx, __be16 proxy_port)
{
	return ctx_redirect_to_proxy_hairpin(ctx, proxy_port, true);
}
#endif

#endif /* HOST_IFINDEX_MAC && HOST_IFINDEX */

#endif /* __LIB_PROXY_HAIRPIN_H_ */
