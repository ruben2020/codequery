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
#ifndef PCKT_PARSING_H_OPENED_FRAMEWORK
#define PCKT_PARSING_H_OPENED_FRAMEWORK
#include "pckt_parsing.h"
#endif 
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/if_ether.h>
#include <linux/ptrace.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/version.h>
#include <linux/pkt_cls.h>

__attribute__ ((__always_inline__)) static inline int process_l3_headers (struct packet_description *pckt, __u8 *protocol, __u64 off, __u16 *pkt_bytes, void *data, void *data_end, bool is_ipv6) {
    __u64 iph_len;
    struct iphdr *iph;
    struct ipv6hdr *ip6h;
    if (is_ipv6) {
        ip6h = data + off;
        if (ip6h + 1 > data_end) {
            return TC_ACT_SHOT;
        }
        iph_len = sizeof (struct ipv6hdr);
        *protocol = ip6h->nexthdr;
        pckt->flow.proto = *protocol;
        *pkt_bytes = bpf_ntohs (ip6h->payload_len);
        off += iph_len;
        if (*protocol == IPPROTO_FRAGMENT) {
            return TC_ACT_SHOT;
        }
    }
    else {
        iph = data + off;
        if (iph + 1 > data_end) {
            return TC_ACT_SHOT;
        }
        if (iph->ihl != 5) {
            return TC_ACT_SHOT;
        }
        *protocol = iph->protocol;
        pckt->flow.proto = *protocol;
        *pkt_bytes = bpf_ntohs (iph->tot_len);
        off += IPV4_HDR_LEN_NO_OPT;
        if (iph->frag_off & PCKT_FRAGMENTED) {
            return TC_ACT_SHOT;
        }
    }
    return FURTHER_PROCESSING;
}
#ifdef  INLINE_DECAP_GENERIC

__attribute__ ((__always_inline__)) static inline int check_decap_dst (struct packet_description *pckt, bool is_ipv6, bool *pass) {
    struct address dst_addr = {};
    struct lb_stats *data_stats;
    if (is_ipv6) {
        memcpy (dst_addr.addrv6, pckt -> flow.dstv6, 16);
    }
    else {
        dst_addr.addr = pckt->flow.dst;
    }
    __u32 *decap_dst_flags = bpf_map_lookup_elem (&decap_dst, &dst_addr);
    if (decap_dst_flags) {
        *pass = false;
        __u32 stats_key = MAX_VIPS + REMOTE_ENCAP_CNTRS;
        data_stats = bpf_map_lookup_elem (&stats, &stats_key);
        if (!data_stats) {
            return TC_ACT_SHOT;
        }
        data_stats->v1 += 1;
    }
    return FURTHER_PROCESSING;
}
#endif 

__attribute__ ((__always_inline__)) static inline int process_encaped_ipip_pckt (void **data, void **data_end, struct __sk_buff *xdp, bool *is_ipv6, struct packet_description *pckt, __u8 *protocol, __u64 off, __u16 *pkt_bytes) {
    if (*protocol == IPPROTO_IPIP) {
        if (*is_ipv6) {
            if ((*data + sizeof (struct ipv6hdr) + sizeof (struct ethhdr)) > *data_end) {
                return TC_ACT_SHOT;
            }
            if (!decap_v6 (xdp, data, data_end, true)) {
                return TC_ACT_SHOT;
            }
        }
        else {
            if ((*data + sizeof (struct iphdr) + sizeof (struct ethhdr)) > *data_end) {
                return TC_ACT_SHOT;
            }
            if (!decap_v4 (xdp, data, data_end)) {
                return TC_ACT_SHOT;
            }
        }
    }
    else if (*protocol == IPPROTO_IPV6) {
        if ((*data + sizeof (struct ipv6hdr) + sizeof (struct ethhdr)) > *data_end) {
            return TC_ACT_SHOT;
        }
        if (!decap_v6 (xdp, data, data_end, false)) {
            return TC_ACT_SHOT;
        }
    }
    return FURTHER_PROCESSING;
}
#ifdef  INLINE_DECAP_GUE

__attribute__ ((__always_inline__)) static inline int process_encaped_gue_pckt (void **data, void **data_end, struct __sk_buff *xdp, bool is_ipv6) {
    int offset = 0;
    if (is_ipv6) {
        __u8 v6 = 0;
        offset = sizeof (struct ipv6hdr) + sizeof (struct ethhdr) + sizeof (struct udphdr);
        if ((*data + offset + 1) > *data_end) {
            return TC_ACT_SHOT;
        }
        v6 = ((__u8 *) (*data))[offset];
        v6 &= GUEV1_IPV6MASK;
        if (v6) {
            if (!gue_decap_v6 (xdp, data, data_end, false)) {
                return TC_ACT_SHOT;
            }
        }
        else {
            if (!gue_decap_v6 (xdp, data, data_end, true)) {
                return TC_ACT_SHOT;
            }
        }
    }
    else {
        offset = sizeof (struct iphdr) + sizeof (struct ethhdr) + sizeof (struct udphdr);
        if ((*data + offset) > *data_end) {
            return TC_ACT_SHOT;
        }
        if (!gue_decap_v4 (xdp, data, data_end)) {
            return TC_ACT_SHOT;
        }
    }
    return FURTHER_PROCESSING;
}
#endif 

__attribute__ ((__always_inline__)) static inline void increment_quic_cid_version_stats (int host_id) {
    __u32 quic_version_stats_key = MAX_VIPS + QUIC_CID_VERSION_STATS;
    struct lb_stats *quic_version = bpf_map_lookup_elem (&stats, &quic_version_stats_key);
    if (!quic_version) {
        return;
    }
    if (host_id > QUIC_CONNID_VERSION_V1_MAX_VAL) {
        quic_version->v2 += 1;
    }
    else {
        quic_version->v1 += 1;
    }
}

__attribute__ ((__always_inline__)) static inline void increment_quic_cid_drop_real_0 () {
    __u32 quic_drop_stats_key = MAX_VIPS + QUIC_CID_DROP_STATS;
    struct lb_stats *quic_drop = bpf_map_lookup_elem (&stats, &quic_drop_stats_key);
    if (!quic_drop) {
        return;
    }
    quic_drop->v2 += 1;
}

__attribute__ ((__always_inline__)) static inline void increment_quic_cid_drop_no_real () {
    __u32 quic_drop_stats_key = MAX_VIPS + QUIC_CID_DROP_STATS;
    struct lb_stats *quic_drop = bpf_map_lookup_elem (&stats, &quic_drop_stats_key);
    if (!quic_drop) {
        return;
    }
    quic_drop->v1 += 1;
}

__attribute__ ((__always_inline__)) static inline void connection_table_lookup (struct real_definition **real, struct packet_description *pckt, void *lru_map, bool isGlobalLru) {
    struct real_pos_lru *dst_lru;
    __u64 cur_time;
    __u32 key;
    dst_lru = bpf_map_lookup_elem (lru_map, &pckt->flow);
    if (!dst_lru) {
        return;
    }
    if (!isGlobalLru && pckt->flow.proto == IPPROTO_UDP) {
        cur_time = bpf_ktime_get_ns ();
        if (cur_time - dst_lru->atime > LRU_UDP_TIMEOUT) {
            return;
        }
        dst_lru->atime = cur_time;
    }
    key = dst_lru->pos;
    pckt->real_index = key;
    *real = bpf_map_lookup_elem (&reals, &key);
    return;
}

__attribute__ ((__always_inline__)) static inline bool is_under_flood (__u64 *cur_time) {
    __u32 conn_rate_key = MAX_VIPS + NEW_CONN_RATE_CNTR;
    struct lb_stats *conn_rate_stats = bpf_map_lookup_elem (&stats, &conn_rate_key);
    if (!conn_rate_stats) {
        return true;
    }
    *cur_time = bpf_ktime_get_ns ();
    if ((*cur_time - conn_rate_stats->v2) > ONE_SEC) {
        conn_rate_stats->v1 = 1;
        conn_rate_stats->v2 = *cur_time;
    }
    else {
        conn_rate_stats->v1 += 1;
        if (conn_rate_stats->v1 > MAX_CONN_RATE) {
            return true;
        }
    }
    return false;
}

__attribute__ ((__always_inline__)) static inline __u32 get_packet_hash (struct packet_description *pckt, bool hash_16bytes) {
    if (hash_16bytes) {
        return jhash_2words (jhash (pckt->flow.srcv6, 16, INIT_JHASH_SEED_V6), pckt->flow.ports, INIT_JHASH_SEED);
    }
    else {
        return jhash_2words (pckt->flow.src, pckt->flow.ports, INIT_JHASH_SEED);
    }
}

__attribute__ ((__always_inline__)) static inline bool get_packet_dst (struct real_definition **real, struct packet_description *pckt, struct vip_meta *vip_info, bool is_ipv6, void *lru_map) {
    struct real_pos_lru new_dst_lru = {};
    bool under_flood = false;
    bool src_found = false;
    __u32 *real_pos;
    __u64 cur_time = 0;
    __u32 hash;
    __u32 key;
    under_flood = is_under_flood (&cur_time);
#ifdef LPM_SRC_LOOKUP
    if ((vip_info->flags & F_SRC_ROUTING) && !under_flood) {
        __u32 *lpm_val;
        if (is_ipv6) {
            struct v6_lpm_key lpm_key_v6 = {};
            lpm_key_v6.prefixlen = 128;
            memcpy (lpm_key_v6.addr, pckt -> flow.srcv6, 16);
            lpm_val = bpf_map_lookup_elem (&lpm_src_v6, &lpm_key_v6);
        }
        else {
            struct v4_lpm_key lpm_key_v4 = {};
            lpm_key_v4.addr = pckt->flow.src;
            lpm_key_v4.prefixlen = 32;
            lpm_val = bpf_map_lookup_elem (&lpm_src_v4, &lpm_key_v4);
        }
        if (lpm_val) {
            src_found = true;
            key = *lpm_val;
        }
        __u32 stats_key = MAX_VIPS + LPM_SRC_CNTRS;
        struct lb_stats *data_stats = bpf_map_lookup_elem (&stats, &stats_key);
        if (data_stats) {
            if (src_found) {
                data_stats->v2 += 1;
            }
            else {
                data_stats->v1 += 1;
            }
        }
    }
#endif
    if (!src_found) {
        bool hash_16bytes = is_ipv6;
        if (vip_info->flags & F_HASH_DPORT_ONLY) {
            pckt->flow.port16[0] = pckt->flow.port16[1];
            memset (pckt -> flow.srcv6, 0, 16);
        }
        hash = get_packet_hash (pckt, hash_16bytes) % RING_SIZE;
        key = RING_SIZE * (vip_info->vip_num) + hash;
        real_pos = bpf_map_lookup_elem (&ch_rings, &key);
        if (!real_pos) {
            return false;
        }
        key = *real_pos;
    }
    pckt->real_index = key;
    *real = bpf_map_lookup_elem (&reals, &key);
    if (!(*real)) {
        return false;
    }
    if (lru_map && !(vip_info->flags & F_LRU_BYPASS) && !under_flood) {
        if (pckt->flow.proto == IPPROTO_UDP) {
            new_dst_lru.atime = cur_time;
        }
        new_dst_lru.pos = key;
        bpf_map_update_elem (lru_map, & pckt -> flow, & new_dst_lru, BPF_ANY);
    }
    return true;
}
#ifdef  GLOBAL_LRU_LOOKUP

__attribute__ ((__always_inline__)) static inline bool reals_have_same_addr (struct real_definition *a, struct real_definition *b) {
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
    }
    else {
        return a->dst == b->dst;
    }
}
#endif 
#ifdef  GLOBAL_LRU_LOOKUP

__attribute__ ((__always_inline__)) static inline int perform_global_lru_lookup (struct real_definition **dst, struct packet_description *pckt, __u32 cpu_num, struct vip_meta *vip_info, bool is_ipv6) {
    void *g_lru_map = bpf_map_lookup_elem (&global_lru_maps, &cpu_num);
    __u32 global_lru_stats_key = MAX_VIPS + GLOBAL_LRU_CNTR;
    struct lb_stats *global_lru_stats = bpf_map_lookup_elem (&stats, &global_lru_stats_key);
    if (!global_lru_stats) {
        return TC_ACT_SHOT;
    }
    if (!g_lru_map) {
        global_lru_stats->v1 += 1;
        g_lru_map = &fallback_glru;
    }
    connection_table_lookup (dst, pckt, g_lru_map, true);
    if (*dst) {
        global_lru_stats->v2 += 1;
        struct real_definition *dst_consistent_hash = NULL;
        if (get_packet_dst (&dst_consistent_hash, pckt, vip_info, is_ipv6, NULL)) {
            __u32 global_lru_mismatch_stats_key = MAX_VIPS + GLOBAL_LRU_MISMATCH_CNTR;
            struct lb_stats *global_lru_mismatch_stats = bpf_map_lookup_elem (&stats, &global_lru_mismatch_stats_key);
            if (dst_consistent_hash && global_lru_mismatch_stats) {
                if (reals_have_same_addr (dst_consistent_hash, *dst)) {
                    global_lru_mismatch_stats->v1++;
                }
                else {
                    global_lru_mismatch_stats->v2++;
                }
            }
        }
    }
    return FURTHER_PROCESSING;
}
#endif 

__attribute__ ((__always_inline__)) static inline int process_packet (void *data, __u64 off, void *data_end, bool is_ipv6, struct __sk_buff *xdp) {
    struct packet_description pckt = {};
    struct decap_stats *data_stats;
    __u32 key = 0;
    __u8 protocol;
    int action;
    __u16 pkt_bytes;
    action = process_l3_headers (&pckt, &protocol, off, &pkt_bytes, data, data_end, is_ipv6);
    if (action >= 0) {
        return action;
    }
    protocol = pckt.flow.proto;
    data_stats = bpf_map_lookup_elem (&decap_counters, &key);
    if (!data_stats) {
        return TC_ACT_OK;
    }
    data_stats->total += 1;
    if (protocol == IPPROTO_IPIP || protocol == IPPROTO_IPV6) {
        if (is_ipv6) {
            data_stats->decap_v6 += 1;
        }
        else {
            data_stats->decap_v4 += 1;
        }
        action = process_encaped_ipip_pckt (&data, &data_end, xdp, &is_ipv6, &pckt, &protocol, off, &pkt_bytes);
        if (action >= 0) {
            return action;
        }
    }
#ifdef INLINE_DECAP_GUE
    else if (protocol == IPPROTO_UDP) {
        if (!parse_udp (data, data_end, is_ipv6, &pckt)) {
            return TC_ACT_OK;
        }
        if (pckt.flow.port16[1] == bpf_htons (GUE_DPORT)) {
            if (is_ipv6) {
                data_stats->decap_v6 += 1;
            }
            else {
                data_stats->decap_v4 += 1;
            }
            action = process_encaped_gue_pckt (&data, &data_end, xdp, is_ipv6);
            if (action >= 0) {
                return action;
            }
        }
    }
#endif // INLINE_DECAP_GUE
    return TC_ACT_OK;
}

SEC ("decap")
int xdpdecap (struct __sk_buff *ctx) {
    void *data = (void *) (long) ctx->data;
    void *data_end = (void *) (long) ctx->data_end;
    struct ethhdr *eth = data;
    __u32 eth_proto;
    __u32 nh_off;
    nh_off = sizeof (struct ethhdr);
    if (data + nh_off > data_end) {
        return TC_ACT_SHOT;
    }
    eth_proto = ctx->protocol;
    if (eth_proto == BE_ETH_P_IP) {
        return process_packet (data, nh_off, data_end, false, ctx);
    }
    else if (eth_proto == BE_ETH_P_IPV6) {
        return process_packet (data, nh_off, data_end, true, ctx);
    }
    else {
        return TC_ACT_OK;
    }
}

char _license [] SEC ("license") = "GPL";
