/* SPDX-License-Identifier: GPL-2.0 */
#define RECORD_FLOW_INFO
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <stdbool.h>
#include <stddef.h>
#ifndef BALANCER_CONSTS_H_OPENED_FRAMEWORK
#define BALANCER_CONSTS_H_OPENED_FRAMEWORK
#include "balancer_consts.h"
#endif 

#ifndef BPF_H_OPENED_FRAMEWORK
#define BPF_H_OPENED_FRAMEWORK
#include "bpf.h"
#endif 

#ifndef BPF_HELPERS_H_OPENED_FRAMEWORK
#define BPF_HELPERS_H_OPENED_FRAMEWORK
#include "bpf_helpers.h"
#endif 

#ifndef DECAP_MAPS_H_OPENED_FRAMEWORK
#define DECAP_MAPS_H_OPENED_FRAMEWORK
#include "decap_maps.h"
#endif 

#ifndef PCKT_ENCAP_H_OPENED_FRAMEWORK
#define PCKT_ENCAP_H_OPENED_FRAMEWORK
#include "pckt_encap.h"
#endif 

#ifndef PCKT_PARSING_H_OPENED_FRAMEWORK
#define PCKT_PARSING_H_OPENED_FRAMEWORK
#include "pckt_parsing.h"
#endif 

#include <string.h>
#ifndef BALANCER_HELPERS_H_OPENED_FRAMEWORK
#define BALANCER_HELPERS_H_OPENED_FRAMEWORK
#include "balancer_helpers.h"
#endif 

#ifndef BALANCER_MAPS_H_OPENED_FRAMEWORK
#define BALANCER_MAPS_H_OPENED_FRAMEWORK
#include "balancer_maps.h"
#endif 

#ifndef BALANCER_STRUCTS_H_OPENED_FRAMEWORK
#define BALANCER_STRUCTS_H_OPENED_FRAMEWORK
#include "balancer_structs.h"
#endif 

#ifndef HANDLE_ICMP_H_OPENED_FRAMEWORK
#define HANDLE_ICMP_H_OPENED_FRAMEWORK
#include "handle_icmp.h"
#endif 

#ifndef JHASH_H_OPENED_FRAMEWORK
#define JHASH_H_OPENED_FRAMEWORK
#include "jhash.h"
#endif 

#ifndef BALANCER_MAPS_H_OPENED_FRAMEWORK
#define BALANCER_MAPS_H_OPENED_FRAMEWORK
#include "balancer_maps.h"
#endif 

#ifndef CONTROL_DATA_MAPS_H_OPENED_FRAMEWORK
#define CONTROL_DATA_MAPS_H_OPENED_FRAMEWORK
#include "control_data_maps.h"
#endif 

#ifndef DECAP_MAPS_H_OPENED_FRAMEWORK
#define DECAP_MAPS_H_OPENED_FRAMEWORK
#include "decap_maps.h"
#endif 

#ifndef FLOW_DEBUG_HELPERS_H_OPENED_FRAMEWORK
#define FLOW_DEBUG_HELPERS_H_OPENED_FRAMEWORK
#include "flow_debug_helpers.h"
#endif 

#include <linux/if_ether.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#ifndef FLOW_DEBUG_MAPS_H_OPENED_FRAMEWORK
#define FLOW_DEBUG_MAPS_H_OPENED_FRAMEWORK
#include "flow_debug_maps.h"
#endif 

#ifndef FLOW_DEBUG_MAPS_H_OPENED_FRAMEWORK
#define FLOW_DEBUG_MAPS_H_OPENED_FRAMEWORK
#include "flow_debug_maps.h"
#endif 

#ifndef FLOW_DEBUG_H_OPENED_FRAMEWORK
#define FLOW_DEBUG_H_OPENED_FRAMEWORK
#include "flow_debug.h"
#endif 

#ifndef PCKT_PARSING_H_OPENED_FRAMEWORK
#define PCKT_PARSING_H_OPENED_FRAMEWORK
#include "pckt_parsing.h"
#endif 

#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/ptrace.h>
#include <linux/version.h>

/* Extracted from 
 /home/sayandes/codequery/test/decap_kern.c 
 startLine: 34 endLine: 83
 */ 
__attribute__((__always_inline__)) static inline int process_l3_headers(
    struct packet_description* pckt,
    __u8* protocol,
    __u64 off,
    __u16* pkt_bytes,
    void* data,
    void* data_end,
    bool is_ipv6) {
  __u64 iph_len;
  struct iphdr* iph;
  struct ipv6hdr* ip6h;
  if (is_ipv6) {
    ip6h = data + off;
    if (ip6h + 1 > data_end) {
      return XDP_DROP;
    }

    iph_len = sizeof(struct ipv6hdr);
    *protocol = ip6h->nexthdr;
    pckt->flow.proto = *protocol;
    *pkt_bytes = bpf_ntohs(ip6h->payload_len);
    off += iph_len;
    if (*protocol == IPPROTO_FRAGMENT) {
      // we drop fragmented packets
      return XDP_DROP;
    }
  } else {
    iph = data + off;
    if (iph + 1 > data_end) {
      return XDP_DROP;
    }
    // ihl contains len of ipv4 header in 32bit words
    if (iph->ihl != 5) {
      // if len of ipv4 hdr is not equal to 20bytes that means that header
      // contains ip options, and we dont support em
      return XDP_DROP;
    }

    *protocol = iph->protocol;
    pckt->flow.proto = *protocol;
    *pkt_bytes = bpf_ntohs(iph->tot_len);
    off += IPV4_HDR_LEN_NO_OPT;

    if (iph->frag_off & PCKT_FRAGMENTED) {
      // we drop fragmented packets.
      return XDP_DROP;
    }
  }
  return FURTHER_PROCESSING;
}
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
 /home/sayandes/codequery/test/decap_kern.c 
 startLine: 85 endLine: 120
 */ 
__attribute__((__always_inline__)) static inline int process_encaped_ipip_pckt(
    void** data,
    void** data_end,
    struct xdp_md* xdp,
    bool* is_ipv6,
    struct packet_description* pckt,
    __u8* protocol,
    __u64 off,
    __u16* pkt_bytes) {
  if (*protocol == IPPROTO_IPIP) {
    if (*is_ipv6) {
      if ((*data + sizeof(struct ipv6hdr) + sizeof(struct ethhdr)) >
          *data_end) {
        return XDP_DROP;
      }
      if (!decap_v6(xdp, data, data_end, true)) {
        return XDP_DROP;
      }
    } else {
      if ((*data + sizeof(struct iphdr) + sizeof(struct ethhdr)) > *data_end) {
        return XDP_DROP;
      }
      if (!decap_v4(xdp, data, data_end)) {
        return XDP_DROP;
      }
    }
  } else if (*protocol == IPPROTO_IPV6) {
    if ((*data + sizeof(struct ipv6hdr) + sizeof(struct ethhdr)) > *data_end) {
      return XDP_DROP;
    }
    if (!decap_v6(xdp, data, data_end, false)) {
      return XDP_DROP;
    }
  }
  return FURTHER_PROCESSING;
}
#ifdef  INLINE_DECAP_GUE
/* Extracted from 
 /home/sayandes/codequery/test/decap_kern.c 
 startLine: 123 endLine: 161
 */ 
__attribute__((__always_inline__)) static inline int process_encaped_gue_pckt(
    void** data,
    void** data_end,
    struct xdp_md* xdp,
    bool is_ipv6) {
  int offset = 0;
  if (is_ipv6) {
    __u8 v6 = 0;
    offset =
        sizeof(struct ipv6hdr) + sizeof(struct ethhdr) + sizeof(struct udphdr);
    // 1 byte for gue v1 marker to figure out what is internal protocol
    if ((*data + offset + 1) > *data_end) {
      return XDP_DROP;
    }
    v6 = ((__u8*)(*data))[offset];
    v6 &= GUEV1_IPV6MASK;
    if (v6) {
      // inner packet is ipv6 as well
      if (!gue_decap_v6(xdp, data, data_end, false)) {
        return XDP_DROP;
      }
    } else {
      // inner packet is ipv4
      if (!gue_decap_v6(xdp, data, data_end, true)) {
        return XDP_DROP;
      }
    }
  } else {
    offset =
        sizeof(struct iphdr) + sizeof(struct ethhdr) + sizeof(struct udphdr);
    if ((*data + offset) > *data_end) {
      return XDP_DROP;
    }
    if (!gue_decap_v4(xdp, data, data_end)) {
      return XDP_DROP;
    }
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
/* Extracted from 
 /home/sayandes/codequery/test/decap_kern.c 
 startLine: 164 endLine: 221
 */ 
__attribute__((__always_inline__)) static inline int process_packet(
    void* data,
    __u64 off,
    void* data_end,
    bool is_ipv6,
    struct xdp_md* xdp) {
  struct packet_description pckt = {};
  struct decap_stats* data_stats;
  __u32 key = 0;
  __u8 protocol;

  int action;
  __u16 pkt_bytes;
  action = process_l3_headers(
      &pckt, &protocol, off, &pkt_bytes, data, data_end, is_ipv6);
  if (action >= 0) {
    return action;
  }
  protocol = pckt.flow.proto;

  data_stats = bpf_map_lookup_elem(&decap_counters, &key);
  if (!data_stats) {
    return XDP_PASS;
  }

  data_stats->total += 1;
  if (protocol == IPPROTO_IPIP || protocol == IPPROTO_IPV6) {
    if (is_ipv6) {
      data_stats->decap_v6 += 1;
    } else {
      data_stats->decap_v4 += 1;
    }
    action = process_encaped_ipip_pckt(
        &data, &data_end, xdp, &is_ipv6, &pckt, &protocol, off, &pkt_bytes);
    if (action >= 0) {
      return action;
    }
  }
#ifdef INLINE_DECAP_GUE
  else if (protocol == IPPROTO_UDP) {
    if (!parse_udp(data, data_end, is_ipv6, &pckt)) {
      return XDP_PASS;
    }
    if (pckt.flow.port16[1] == bpf_htons(GUE_DPORT)) {
      if (is_ipv6) {
        data_stats->decap_v6 += 1;
      } else {
        data_stats->decap_v4 += 1;
      }
      action = process_encaped_gue_pckt(&data, &data_end, xdp, is_ipv6);
      if (action >= 0) {
        return action;
      }
    }
  }
#endif // INLINE_DECAP_GUE
  return XDP_PASS;
}
/* Extracted from 
 /home/sayandes/codequery/test/decap_kern.c 
 startLine: 223 endLine: 247
 */ 
SEC("decap")
int xdpdecap(struct xdp_md* ctx) {
  void* data = (void*)(long)ctx->data;
  void* data_end = (void*)(long)ctx->data_end;
  struct ethhdr* eth = data;
  __u32 eth_proto;
  __u32 nh_off;
  nh_off = sizeof(struct ethhdr);

  if (data + nh_off > data_end) {
    // bogus packet, len less than minimum ethernet frame size
    return XDP_DROP;
  }

  eth_proto = eth->h_proto;

  if (eth_proto == BE_ETH_P_IP) {
    return process_packet(data, nh_off, data_end, false, ctx);
  } else if (eth_proto == BE_ETH_P_IPV6) {
    return process_packet(data, nh_off, data_end, true, ctx);
  } else {
    // pass to tcp/ip stack
    return XDP_PASS;
  }
}
char _license[] SEC("license") = "GPL";
