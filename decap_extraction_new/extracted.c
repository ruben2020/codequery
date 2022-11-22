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
char _license[] SEC("license") = "GPL";