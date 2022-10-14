#ifndef __CONTROL_DATA_MAPS_H
#define __CONTROL_DATA_MAPS_H
#include "bpf.h"
#include "bpf_helpers.h"
#include "balancer_consts.h"
#include "balancer_structs.h"
struct {
    __uint (type, BPF_MAP_TYPE_ARRAY);
    __type (key, __u32);
    __type (value, struct ctl_value);
    __uint (max_entries, CTL_MAP_SIZE);
    __uint (map_flags, NO_FLAGS);
} ctl_array SEC (".maps");
#ifdef KATRAN_INTROSPECTION
struct {
    __uint (type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __type (key, int);
    __type (value, __u32);
    __uint (max_entries, MAX_SUPPORTED_CPUS);
    __uint (map_flags, NO_FLAGS);
} event_pipe SEC (".maps");
#endif
#ifdef INLINE_DECAP_GENERIC
struct {
    __uint (type, BPF_MAP_TYPE_HASH);
    __type (key, struct address);
    __type (value, __u32);
    __uint (max_entries, MAX_VIPS);
    __uint (map_flags, NO_FLAGS);
} decap_dst SEC (".maps");
struct {
    __uint (type, BPF_MAP_TYPE_PROG_ARRAY);
    __type (key, __u32);
    __type (value, __u32);
    __uint (max_entries, SUBPROGRAMS_ARRAY_SIZE);
} subprograms SEC (".maps");
#endif
#ifdef GUE_ENCAP
struct {
    __uint (type, BPF_MAP_TYPE_ARRAY);
    __type (key, __u32);
    __type (value, struct real_definition);
    __uint (max_entries, 2);
    __uint (map_flags, NO_FLAGS);
} pckt_srcs SEC (".maps");
#endif
#endif // of __CONTROL_DATA_MAPS_H
