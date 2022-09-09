/* SPDX-License-Identifier: GPL-2.0 */
#define RECORD_FLOW_INFO
#ifndef SWAB_H
#define SWAB_H
#include "swab.h"
#endif 

#include <linux/types.h>
#include <asm/bitsperlong.h>
#include <asm/swab.h>
#ifndef BPF_ENDIAN_H
#define BPF_ENDIAN_H
#include "bpf_endian.h"
#endif 

#include <linux/swab.h>
#ifndef CDEFS_H
#define CDEFS_H
#include "cdefs.h"
#endif 

#include <bits/wordsize.h>
#include <bits/long-double.h>
#ifndef STRING_H
#define STRING_H
#include "string.h"
#endif 

//#include <bits/libc-header-start.h>
#include <stddef.h>
#ifndef HANDLE_ICMP_H
#define HANDLE_ICMP_H
#include "handle_icmp.h"
#endif 

#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#ifndef BALANCER_CONSTS_H
#define BALANCER_CONSTS_H
#include "balancer_consts.h"
#endif 

#ifndef BALANCER_HELPERS_H
#define BALANCER_HELPERS_H
#include "balancer_helpers.h"
#endif 

#ifndef BALANCER_STRUCTS_H
#define BALANCER_STRUCTS_H
#include "balancer_structs.h"
#endif 

#ifndef BPF_H
#define BPF_H
#include "bpf.h"
#endif 

#ifndef BPF_ENDIAN_H
#define BPF_ENDIAN_H
#include "bpf_endian.h"
#endif 

#ifndef CSUM_HELPERS_H
#define CSUM_HELPERS_H
#include "csum_helpers.h"
#endif 

#include <linux/in.h>
#include <linux/udp.h>
#include <stdbool.h>
#ifndef BPF_HELPERS_H
#define BPF_HELPERS_H
#include "bpf_helpers.h"
#endif 

#ifndef BALANCER_HELPERS_H
#define BALANCER_HELPERS_H
#include "balancer_helpers.h"
#endif 

#ifndef CONTROL_DATA_MAPS_H
#define CONTROL_DATA_MAPS_H
#include "control_data_maps.h"
#endif 

#ifndef CSUM_HELPERS_H
#define CSUM_HELPERS_H
#include "csum_helpers.h"
#endif 

#ifndef INTROSPECTION_H
#define INTROSPECTION_H
#include "introspection.h"
#endif 

#ifndef PCKT_ENCAP_H
#define PCKT_ENCAP_H
#include "pckt_encap.h"
#endif 

#include <string.h>
#ifndef ENCAP_HELPERS_H
#define ENCAP_HELPERS_H
#include "encap_helpers.h"
#endif 

#ifndef FLOW_DEBUG_H
#define FLOW_DEBUG_H
#include "flow_debug.h"
#endif 

#ifndef PCKT_PARSING_H
#define PCKT_PARSING_H
#include "pckt_parsing.h"
#endif 

#ifndef PCKT_PARSING_H
#define PCKT_PARSING_H
#include "pckt_parsing.h"
#endif 

#include <linux/ptrace.h>
#include <linux/tcp.h>
#include <linux/version.h>
#ifndef FLOW_DEBUG_HELPERS_H
#define FLOW_DEBUG_HELPERS_H
#include "flow_debug_helpers.h"
#endif 

#ifndef FLOW_DEBUG_MAPS_H
#define FLOW_DEBUG_MAPS_H
#include "flow_debug_maps.h"
#endif 

#ifndef INTROSPECTION_H
#define INTROSPECTION_H
#include "introspection.h"
#endif 

#ifndef VERSION_H
#define VERSION_H
#include "version.h"
#endif 

#ifndef JHASH_H
#define JHASH_H
#include "jhash.h"
#endif 

#ifndef BALANCER_MAPS_H
#define BALANCER_MAPS_H
#include "balancer_maps.h"
#endif 

#ifndef CONTROL_DATA_MAPS_H
#define CONTROL_DATA_MAPS_H
#include "control_data_maps.h"
#endif 

#ifndef DECAP_MAPS_H
#define DECAP_MAPS_H
#include "decap_maps.h"
#endif 

#ifndef FLOW_DEBUG_MAPS_H
#define FLOW_DEBUG_MAPS_H
#include "flow_debug_maps.h"
#endif 

#ifdef  INLINE_DECAP_GENERIC
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 233 endLine: 255
 */ 
__attribute__((__always_inline__)) static inline int
check_decap_dst(struct packet_description* pckt, bool is_ipv6, bool* pass) {
  struct address dst_addr = {};
  struct lb_stats* data_stats;

  if (is_ipv6) {
    memcpy(dst_addr.addrv6, pckt->flow.dstv6, 16);
  } else {
    dst_addr.addr = pckt->flow.dst;
  }
  __u32* decap_dst_flags = bpf_map_lookup_elem(&decap_dst, &dst_addr);

  if (decap_dst_flags) {
    *pass = false;
    __u32 stats_key = MAX_VIPS + REMOTE_ENCAP_CNTRS;
    data_stats = bpf_map_lookup_elem(&stats, &stats_key);
    if (!data_stats) {
      return XDP_DROP;
    }
    data_stats->v1 += 1;
  }
  return FURTHER_PROCESSING;
}
#endif 
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 444 endLine: 457
 */ 
__attribute__((__always_inline__)) static inline void
increment_quic_cid_version_stats(int host_id) {
  __u32 quic_version_stats_key = MAX_VIPS + QUIC_CID_VERSION_STATS;
  struct lb_stats* quic_version =
      bpf_map_lookup_elem(&stats, &quic_version_stats_key);
  if (!quic_version) {
    return;
  }
  if (host_id > QUIC_CONNID_VERSION_V1_MAX_VAL) {
    quic_version->v2 += 1;
  } else {
    quic_version->v1 += 1;
  }
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 470 endLine: 478
 */ 
__attribute__((__always_inline__)) static inline void increment_quic_cid_drop_real_0() {
  __u32 quic_drop_stats_key = MAX_VIPS + QUIC_CID_DROP_STATS;
  struct lb_stats* quic_drop =
      bpf_map_lookup_elem(&stats, &quic_drop_stats_key);
  if (!quic_drop) {
    return;
  }
  quic_drop->v2 += 1;
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 459 endLine: 468
 */ 
__attribute__((__always_inline__)) static inline void
increment_quic_cid_drop_no_real() {
  __u32 quic_drop_stats_key = MAX_VIPS + QUIC_CID_DROP_STATS;
  struct lb_stats* quic_drop =
      bpf_map_lookup_elem(&stats, &quic_drop_stats_key);
  if (!quic_drop) {
    return;
  }
  quic_drop->v1 += 1;
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 133 endLine: 156
 */ 
__attribute__((__always_inline__)) static inline void connection_table_lookup(
    struct real_definition** real,
    struct packet_description* pckt,
    void* lru_map,
    bool isGlobalLru) {
  struct real_pos_lru* dst_lru;
  __u64 cur_time;
  __u32 key;
  dst_lru = bpf_map_lookup_elem(lru_map, &pckt->flow);
  if (!dst_lru) {
    return;
  }
  if (!isGlobalLru && pckt->flow.proto == IPPROTO_UDP) {
    cur_time = bpf_ktime_get_ns();
    if (cur_time - dst_lru->atime > LRU_UDP_TIMEOUT) {
      return;
    }
    dst_lru->atime = cur_time;
  }
  key = dst_lru->pos;
  pckt->real_index = key;
  *real = bpf_map_lookup_elem(&reals, &key);
  return;
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 24 endLine: 50
 */ 
__attribute__((__always_inline__)) 
	static inline bool is_under_flood(
    __u64* cur_time) {
  __u32 conn_rate_key = MAX_VIPS + NEW_CONN_RATE_CNTR;
  struct lb_stats* conn_rate_stats =
      bpf_map_lookup_elem(&stats, &conn_rate_key);
  if (!conn_rate_stats) {
    return true;
  }
  *cur_time = bpf_ktime_get_ns();
  // we are going to check that new connections rate is less than predefined
  // value; conn_rate_stats.v1 contains number of new connections for the last
  // second, v2 - when last time quanta started.
  if ((*cur_time - conn_rate_stats->v2) > ONE_SEC) {
    // new time quanta; reseting counters
    conn_rate_stats->v1 = 1;
    conn_rate_stats->v2 = *cur_time;
  } else {
    conn_rate_stats->v1 += 1;
    if (conn_rate_stats->v1 > MAX_CONN_RATE) {
      // we are exceding max connections rate. bypasing lru update and
      // source routing lookup
      return true;
    }
  }
  return false;
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 818 endLine: 827
 */ 
__attribute__((__always_inline__)) static inline __u32 get_packet_hash(struct packet_description* pckt,bool hash_16bytes) {
  if (hash_16bytes) {
    return jhash_2words(
        jhash(pckt->flow.srcv6, 16, INIT_JHASH_SEED_V6),
        pckt->flow.ports,
        INIT_JHASH_SEED);
  } else {
    return jhash_2words(pckt->flow.src, pckt->flow.ports, INIT_JHASH_SEED);
  }
}
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 52 endLine: 131
 */ 
__attribute__((__always_inline__)) 
	static inline bool get_packet_dst(
    struct real_definition** real,
    struct packet_description* pckt,
    struct vip_meta* vip_info,
    bool is_ipv6,
    void* lru_map) {
  // to update lru w/ new connection
  struct real_pos_lru new_dst_lru = {};
  bool under_flood = false;
  bool src_found = false;
  __u32* real_pos;
  __u64 cur_time = 0;
  __u32 hash;
  __u32 key;

  under_flood = is_under_flood(&cur_time);

#ifdef LPM_SRC_LOOKUP
  if ((vip_info->flags & F_SRC_ROUTING) && !under_flood) {
    __u32* lpm_val;
    if (is_ipv6) {
      struct v6_lpm_key lpm_key_v6 = {};
      lpm_key_v6.prefixlen = 128;
      memcpy(lpm_key_v6.addr, pckt->flow.srcv6, 16);
      lpm_val = bpf_map_lookup_elem(&lpm_src_v6, &lpm_key_v6);
    } else {
      struct v4_lpm_key lpm_key_v4 = {};
      lpm_key_v4.addr = pckt->flow.src;
      lpm_key_v4.prefixlen = 32;
      lpm_val = bpf_map_lookup_elem(&lpm_src_v4, &lpm_key_v4);
    }
    if (lpm_val) {
      src_found = true;
      key = *lpm_val;
    }
    __u32 stats_key = MAX_VIPS + LPM_SRC_CNTRS;
    struct lb_stats* data_stats = bpf_map_lookup_elem(&stats, &stats_key);
    if (data_stats) {
      if (src_found) {
        data_stats->v2 += 1;
      } else {
        data_stats->v1 += 1;
      }
    }
  }
#endif
  if (!src_found) {
    bool hash_16bytes = is_ipv6;

    if (vip_info->flags & F_HASH_DPORT_ONLY) {
      // service which only use dst port for hash calculation
      // e.g. if packets has same dst port -> they will go to the same real.
      // usually VoIP related services.
      pckt->flow.port16[0] = pckt->flow.port16[1];
      memset(pckt->flow.srcv6, 0, 16);
    }
    hash = get_packet_hash(pckt, hash_16bytes) % RING_SIZE;
    key = RING_SIZE * (vip_info->vip_num) + hash;

    real_pos = bpf_map_lookup_elem(&ch_rings, &key);
    if (!real_pos) {
      return false;
    }
    key = *real_pos;
  }
  pckt->real_index = key;
  *real = bpf_map_lookup_elem(&reals, &key);
  if (!(*real)) {
    return false;
  }
  if (lru_map && !(vip_info->flags & F_LRU_BYPASS) && !under_flood) {
    if (pckt->flow.proto == IPPROTO_UDP) {
      new_dst_lru.atime = cur_time;
    }
    new_dst_lru.pos = key;
    bpf_map_update_elem(lru_map, &pckt->flow, &new_dst_lru, BPF_ANY);
  }
  return true;
}
#ifdef  GLOBAL_LRU_LOOKUP
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 261 endLine: 277
 */ 
__attribute__((__always_inline__)) static inline bool reals_have_same_addr(
    struct real_definition* a,
    struct real_definition* b) {
  if (a->flags != b->flags) {
    return false;
  }
  if (a->flags & F_IPV6) {
    for (int i = 0; i < 4; i++) {
      if (a->dstv6[i] != b->dstv6[i]) {
        return false;
      }
      return true;
    }
  } else {
    return a->dst == b->dst;
  }
}
#endif 
#ifdef  GLOBAL_LRU_LOOKUP
/* Extracted from 
 /home/sayandes/codequery/test/balancer_kern.c 
 startLine: 279 endLine: 335
 */ 
__attribute__((__always_inline__)) static inline int perform_global_lru_lookup(
    struct real_definition** dst,
    struct packet_description* pckt,
    __u32 cpu_num,
    struct vip_meta* vip_info,
    bool is_ipv6) {
  // lookup in the global cache
  void* g_lru_map = bpf_map_lookup_elem(&global_lru_maps, &cpu_num);
  __u32 global_lru_stats_key = MAX_VIPS + GLOBAL_LRU_CNTR;

  struct lb_stats* global_lru_stats =
      bpf_map_lookup_elem(&stats, &global_lru_stats_key);
  if (!global_lru_stats) {
    return XDP_DROP;
  }

  if (!g_lru_map) {
    // We were not able to retrieve the global lru for this cpu.
    // This counter should never be anything except 0 in prod.
    // We are going to use it for monitoring.
    global_lru_stats->v1 += 1; // global lru map doesn't exist for this cpu
    g_lru_map = &fallback_glru;
  }

  connection_table_lookup(dst, pckt, g_lru_map, /*isGlobalLru=*/true);
  if (*dst) {
    global_lru_stats->v2 += 1; // we routed a flow using global lru

    // Find the real that we route the packet to if we use consistent hashing
    struct real_definition* dst_consistent_hash = NULL;
    if (get_packet_dst(
            &dst_consistent_hash,
            pckt,
            vip_info,
            is_ipv6,
            /*lru_map=*/NULL)) {
      __u32 global_lru_mismatch_stats_key = MAX_VIPS + GLOBAL_LRU_MISMATCH_CNTR;

      struct lb_stats* global_lru_mismatch_stats =
          bpf_map_lookup_elem(&stats, &global_lru_mismatch_stats_key);

      if (dst_consistent_hash && global_lru_mismatch_stats) {
        if (reals_have_same_addr(dst_consistent_hash, *dst)) {
          // We route to the same real as that indicated by the consistent
          // hash
          global_lru_mismatch_stats->v1++;
        } else {
          // We route to a real different from that indicated by the
          // consistent hash
          global_lru_mismatch_stats->v2++;
        }
      }
    }
  }

  return FURTHER_PROCESSING;
}
#endif 
char _license[] SEC("license") = "GPL";
