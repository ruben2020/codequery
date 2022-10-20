/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

/*
 * Data metrics collection functions
 *
 */
#ifndef __LIB_METRICS__
#define __LIB_METRICS__

#include "common.h"
#include "utils.h"
#include "maps.h"
#include "dbg.h"

/**
 * update_metrics
 * @direction:	1: Ingress 2: Egress
 * @reason:	reason for forwarding or dropping packet.
 *		reason is 0 if packet is being forwarded, else reason
 *		is the drop error code.
 * Update the metrics map.
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/metrics.h
 Startline: 24
 Endline: 43 
 Funcname: update_metrics 
 Input: (__u64 bytes, __u8 direction, __u8 reason) 
 Output: void 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline void update_metrics(__u64 bytes, __u8 direction,
					   __u8 reason)
{
	struct metrics_value *entry, new_entry = {};
	struct metrics_key key = {};

	key.reason = reason;
	key.dir    = direction;


	entry = map_lookup_elem(&METRICS_MAP, &key);
	if (entry) {
		entry->count += 1;
		entry->bytes += bytes;
	} else {
		new_entry.count = 1;
		new_entry.bytes = bytes;
		map_update_elem(&METRICS_MAP, &key, &new_entry, 0);
	}
}

/**
 * ct_to_metrics_dir
 * @direction:	1: Ingress 2: Egress 3: Service
 * Convert a CT direction into the corresponding one for metrics.
 */
/* 
 OPENED COMMENT BEGIN 
 File: /home/sayandes/codequery/cilium/lib/metrics.h
 Startline: 50
 Endline: 62 
 Funcname: ct_to_metrics_dir 
 Input: (enum ct_dir ct_dir) 
 Output: metric_dir 
 Helpers: [] 
 Read_maps: [] 
 Update_maps: [] 
 Func Description: TO BE ADDED 
 OPENED COMMENT END 
 */ 
static __always_inline enum metric_dir ct_to_metrics_dir(enum ct_dir ct_dir)
{
	switch (ct_dir) {
	case CT_INGRESS:
		return METRIC_INGRESS;
	case CT_EGRESS:
		return METRIC_EGRESS;
	case CT_SERVICE:
		return METRIC_SERVICE;
	default:
		return 0;
	}
}

#endif /* __LIB_METRICS__ */
