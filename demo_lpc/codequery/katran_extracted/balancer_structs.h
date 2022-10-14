#ifndef __BALANCER_STRUCTS_H
#define __BALANCER_STRUCTS_H
struct flow_key {
    union {
        __be32 src;
        __be32 srcv6 [4];
    };
    union {
        __be32 dst;
        __be32 dstv6 [4];
    };
    union {
        __u32 ports;
        __u16 port16 [2];
    };
    __u8 proto;
};
struct packet_description {
    struct flow_key flow;
    __u32 real_index;
    __u8 flags;
    __u8 tos;
};
struct ctl_value {
    union {
        __u64 value;
        __u32 ifindex;
        __u8 mac [6];
    };
};
struct vip_definition {
    union {
        __be32 vip;
        __be32 vipv6 [4];
    };
    __u16 port;
    __u8 proto;
};
struct vip_meta {
    __u32 flags;
    __u32 vip_num;
};
struct real_pos_lru {
    __u32 pos;
    __u64 atime;
};
struct real_definition {
    union {
        __be32 dst;
        __be32 dstv6 [4];
    };
    __u8 flags;
};
struct lb_stats {
    __u64 v1;
    __u64 v2;
};
struct v4_lpm_key {
    __u32 prefixlen;
    __be32 addr;
};
struct v6_lpm_key {
    __u32 prefixlen;
    __be32 addr [4];
};
struct address {
    union {
        __be32 addr;
        __be32 addrv6 [4];
    };
};
#ifdef KATRAN_INTROSPECTION
struct event_metadata {
    __u32 event;
    __u32 pkt_size;
    __u32 data_len;
} __attribute__ ((__packed__));
#endif
#ifdef RECORD_FLOW_INFO
struct flow_debug_info {
    union {
        __be32 l4_hop;
        __be32 l4_hopv6 [4];
    };
    union {
        __be32 this_hop;
        __be32 this_hopv6 [4];
    };
};
#endif // of RECORD_FLOW_INFO
#endif // of _BALANCER_STRUCTS
