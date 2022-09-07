#include<stdio.h>

struct abc {
	int x;
} xyz;

struct {
	__uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
	__type(key, __u32);
	__type(value, struct decap_stats);
	__uint(max_entries, STATS_MAP_SIZE);
	__uint(map_flags, NO_FLAGS);
} decap_counters SEC(".maps");

struct bpf_map_def SEC("maps") flow_table_v4 = {
#if USE_PERCPU_HASH
    .type = BPF_MAP_TYPE_PERCPU_HASH,
#else
    .type = BPF_MAP_TYPE_HASH,
#endif
    .key_size = sizeof(struct flowv4_keys),
    .value_size = sizeof(struct pair),
    .max_entries = 32768,
};


int main(int val){
	 p = 0;
	return 0;
}
