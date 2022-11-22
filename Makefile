# SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause)

XDP_TARGETS := extracted
#USER_TARGETS := xdp_pass_user

LLC ?= llc
CLANG ?= clang
CC := gcc

BASE_DIR = /home/sayandes/xdp-tutorial
LIBBPF_DIR = $(BASE_DIR)/libbpf/src/
COMMON_DIR = $(BASE_DIR)/common/

include $(COMMON_DIR)/common.mk
COMMON_OBJS := $(COMMON_DIR)/common_params.o
