/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#ifndef __LIB_IDENTITY_H_
#define __LIB_IDENTITY_H_

#include "dbg.h"

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 9
 Endline: 12 
 Funcname: identity_in_range 
 Input: (__u32 identity, __u32 range_start, __u32 range_end) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool identity_in_range(__u32 identity, __u32 range_start, __u32 range_end)
{
	return range_start <= identity && identity <= range_end;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 14
 Endline: 32 
 Funcname: identity_is_remote_node 
 Input: (__u32 identity) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool identity_is_remote_node(__u32 identity)
{
	/* KUBE_APISERVER_NODE_ID is the reserved identity that corresponds to
	 * the labels 'reserved:remote-node' and 'reserved:kube-apiserver'. As
	 * such, if it is ever used for determining the identity of a node in
	 * the cluster, then routing decisions and so on should be made the
	 * same way as for REMOTE_NODE_ID. If we ever assign unique identities
	 * to each node in the cluster, then we'll probably need to convert
	 * the implementation here into a map to select any of the possible
	 * identities. But for now, this is good enough to capture the notion
	 * of 'remote nodes in the cluster' for routing decisions.
	 *
	 * Note that kube-apiserver policy is handled entirely separately by
	 * the standard policymap enforcement logic and has no relationship to
	 * the identity as used here. If the apiserver is outside the cluster,
	 * then the KUBE_APISERVER_NODE_ID case should not ever be hit.
	 */
	return identity == REMOTE_NODE_ID || identity == KUBE_APISERVER_NODE_ID;
}

/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 34
 Endline: 37 
 Funcname: identity_is_node 
 Input: (__u32 identity) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool identity_is_node(__u32 identity)
{
	return identity == HOST_ID || identity_is_remote_node(identity);
}

/**
 * identity_is_reserved is used to determine whether an identity is one of the
 * reserved identities that are not handed out to endpoints.
 *
 * Specifically, it should return true if the identity is one of these:
 * - IdentityUnknown
 * - ReservedIdentityHost
 * - ReservedIdentityWorld
 * - ReservedIdentityRemoteNode
 * - ReservedIdentityKubeAPIServer
 *
 * The following identities are given to endpoints so return false for these:
 * - ReservedIdentityUnmanaged
 * - ReservedIdentityHealth
 * - ReservedIdentityInit
 *
 * Identities 128 and higher are guaranteed to be generated based on user input.
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 57
 Endline: 60 
 Funcname: identity_is_reserved 
 Input: (__u32 identity) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool identity_is_reserved(__u32 identity)
{
	return identity < UNMANAGED_ID || identity_is_remote_node(identity);
}

/**
 * identity_is_cluster is used to determine whether an identity is assigned to
 * an entity inside the cluster.
 *
 * This function will return false for:
 * - ReservedIdentityWorld
 * - an identity in the CIDR range
 *
 * This function will return true for:
 * - ReservedIdentityHost
 * - ReservedIdentityUnmanaged
 * - ReservedIdentityHealth
 * - ReservedIdentityInit
 * - ReservedIdentityRemoteNode
 * - ReservedIdentityKubeAPIServer
 * - ReservedIdentityIngress
 * - all other identifies
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 80
 Endline: 90 
 Funcname: identity_is_cluster 
 Input: (__u32 identity) 
 Output: bool 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline bool identity_is_cluster(__u32 identity)
{
	if (identity == WORLD_ID)
		return false;

	if (identity_in_range(identity, CIDR_IDENTITY_RANGE_START,
			      CIDR_IDENTITY_RANGE_END))
		return false;

	return true;
}

#if __ctx_is == __ctx_skb
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/identity.h
 Startline: 93
 Endline: 137 
 Funcname: inherit_identity_from_host 
 Input: (struct __ctx_buff *ctx, __u32 *identity) 
 Output: __u32 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline __u32 inherit_identity_from_host(struct __ctx_buff *ctx, __u32 *identity)
{
	__u32 magic = ctx->mark & MARK_MAGIC_HOST_MASK;

	/* Packets from the ingress proxy must skip the proxy when the
	 * destination endpoint evaluates the policy. As the packet would loop
	 * and/or the connection be reset otherwise.
	 */
	if (magic == MARK_MAGIC_PROXY_INGRESS) {
		*identity = get_identity(ctx);
		ctx->tc_index |= TC_INDEX_F_SKIP_INGRESS_PROXY;
	/* (Return) packets from the egress proxy must skip the redirection to
	 * the proxy, as the packet would loop and/or the connection be reset
	 * otherwise.
	 */
	} else if (magic == MARK_MAGIC_PROXY_EGRESS) {
		*identity = get_identity(ctx);
		ctx->tc_index |= TC_INDEX_F_SKIP_EGRESS_PROXY;
	} else if (magic == MARK_MAGIC_IDENTITY) {
		*identity = get_identity(ctx);
	} else if (magic == MARK_MAGIC_HOST) {
		*identity = HOST_ID;
	} else if (magic == MARK_MAGIC_ENCRYPT) {
		*identity = get_identity(ctx);
#if defined(ENABLE_L7_LB)
	} else if (magic == MARK_MAGIC_PROXY_EGRESS_EPID) {
		*identity = get_epid(ctx); /* endpoint identity, not security identity! */
#endif
	} else {
		*identity = WORLD_ID;
	}

	/* Reset packet mark to avoid hitting routing rules again */
	ctx->mark = 0;

#if defined(ENABLE_L7_LB)
	/* Caller tail calls back to source endpoint egress in this case,
	 * do not log the (world) identity.
	 */
	if (magic != MARK_MAGIC_PROXY_EGRESS_EPID)
#endif
		cilium_dbg(ctx, DBG_INHERIT_IDENTITY, *identity, 0);

	return magic;
}
#endif /* __ctx_is == __ctx_skb */

#endif
