{'annotate_only': 'True', 'function_name': 'bpf_xdp_entry', 'src_dir': 'cilium', 'txl_op_dir': 'brown_txl_annotate_cilium'}
All necessary commands found...
All necessary asset files found...
here
Running:  cscope -cb -k
Running:  ctags --fields=+i -n -L ./cscope.files
Running:  cqmakedb -s test.db -c ./cscope.out -t ./tags -p
Read cscope files and generate function annotation ...
Running:  readlink -f cilium/bpf_alignchecker.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_alignchecker.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_alignchecker.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_alignchecker.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_alignchecker.c.xml /home/sayandes/codequery/cilium/bpf_alignchecker.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_alignchecker.c.out /home/sayandes/codequery/cilium/bpf_alignchecker.c asset/c-extract-struct.txl
Running:  readlink -f cilium/bpf_xdp.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_xdp.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_xdp.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_xdp.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_xdp.c.xml /home/sayandes/codequery/cilium/bpf_xdp.c  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_xdp.c.xml /home/sayandes/codequery/cilium/bpf_xdp.c  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_xdp.c.out /home/sayandes/codequery/cilium/bpf_xdp.c asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_xdp.c.out /home/sayandes/codequery/cilium/bpf_xdp.c asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/bpf_network.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_network.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_network.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_network.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_network.c.xml /home/sayandes/codequery/cilium/bpf_network.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_network.c.out /home/sayandes/codequery/cilium/bpf_network.c asset/c-extract-struct.txl
Running:  readlink -f cilium/bpf_overlay.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_overlay.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_overlay.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_overlay.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_overlay.c.xml /home/sayandes/codequery/cilium/bpf_overlay.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_overlay.c.out /home/sayandes/codequery/cilium/bpf_overlay.c asset/c-extract-struct.txl
Running:  readlink -f cilium/bpf_lxc.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_lxc.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_lxc.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_lxc.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_lxc.c.xml /home/sayandes/codequery/cilium/bpf_lxc.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_lxc.c.out /home/sayandes/codequery/cilium/bpf_lxc.c asset/c-extract-struct.txl
Running:  readlink -f cilium/bpf_sock.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_sock.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_sock.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_sock.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_sock.c.xml /home/sayandes/codequery/cilium/bpf_sock.c  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_sock.c.xml /home/sayandes/codequery/cilium/bpf_sock.c  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_sock.c.out /home/sayandes/codequery/cilium/bpf_sock.c asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_sock.c.out /home/sayandes/codequery/cilium/bpf_sock.c asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/bpf_host.c
File to annotate -  /home/sayandes/codequery/cilium/bpf_host.c output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_host.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_host.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_host.c.xml /home/sayandes/codequery/cilium/bpf_host.c  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_host.c.xml /home/sayandes/codequery/cilium/bpf_host.c  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_host.c.out /home/sayandes/codequery/cilium/bpf_host.c asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_host.c.out /home/sayandes/codequery/cilium/bpf_host.c asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/cilium-probe-kernel-hz.c
File to annotate -  /home/sayandes/codequery/cilium/cilium-probe-kernel-hz.c output in brown_txl_annotate_cilium/annotate_func_cilium_cilium-probe-kernel-hz.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_cilium-probe-kernel-hz.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_cilium-probe-kernel-hz.c.xml /home/sayandes/codequery/cilium/cilium-probe-kernel-hz.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_cilium-probe-kernel-hz.c.out /home/sayandes/codequery/cilium/cilium-probe-kernel-hz.c asset/c-extract-struct.txl
Running:  readlink -f cilium/sockops/bpf_sockops.c
File to annotate -  /home/sayandes/codequery/cilium/sockops/bpf_sockops.c output in brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_sockops.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_sockops.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_sockops.c.xml /home/sayandes/codequery/cilium/sockops/bpf_sockops.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_sockops.c.out /home/sayandes/codequery/cilium/sockops/bpf_sockops.c asset/c-extract-struct.txl
Running:  readlink -f cilium/sockops/bpf_redir.c
File to annotate -  /home/sayandes/codequery/cilium/sockops/bpf_redir.c output in brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_redir.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_redir.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_redir.c.xml /home/sayandes/codequery/cilium/sockops/bpf_redir.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_redir.c.out /home/sayandes/codequery/cilium/sockops/bpf_redir.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/ipv6_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/ipv6_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_ipv6_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_ipv6_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_ipv6_test.c.xml /home/sayandes/codequery/cilium/tests/ipv6_test.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_ipv6_test.c.out /home/sayandes/codequery/cilium/tests/ipv6_test.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/conntrack_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/conntrack_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_conntrack_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_conntrack_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_conntrack_test.c.xml /home/sayandes/codequery/cilium/tests/conntrack_test.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_conntrack_test.c.out /home/sayandes/codequery/cilium/tests/conntrack_test.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/bpf_ct_tests.c
File to annotate -  /home/sayandes/codequery/cilium/tests/bpf_ct_tests.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_bpf_ct_tests.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_bpf_ct_tests.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_bpf_ct_tests.c.xml /home/sayandes/codequery/cilium/tests/bpf_ct_tests.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_bpf_ct_tests.c.out /home/sayandes/codequery/cilium/tests/bpf_ct_tests.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/jhash_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/jhash_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_jhash_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_jhash_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_jhash_test.c.xml /home/sayandes/codequery/cilium/tests/jhash_test.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_jhash_test.c.out /home/sayandes/codequery/cilium/tests/jhash_test.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/nat_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/nat_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_nat_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_nat_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_nat_test.c.xml /home/sayandes/codequery/cilium/tests/nat_test.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_nat_test.c.out /home/sayandes/codequery/cilium/tests/nat_test.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/drop_notify_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/drop_notify_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_drop_notify_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_drop_notify_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_drop_notify_test.c.xml /home/sayandes/codequery/cilium/tests/drop_notify_test.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_drop_notify_test.c.out /home/sayandes/codequery/cilium/tests/drop_notify_test.c asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/xdp_nodeport_lb4_test.c
File to annotate -  /home/sayandes/codequery/cilium/tests/xdp_nodeport_lb4_test.c output in brown_txl_annotate_cilium/annotate_func_cilium_tests_xdp_nodeport_lb4_test.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_xdp_nodeport_lb4_test.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_xdp_nodeport_lb4_test.c.xml /home/sayandes/codequery/cilium/tests/xdp_nodeport_lb4_test.c  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_xdp_nodeport_lb4_test.c.xml /home/sayandes/codequery/cilium/tests/xdp_nodeport_lb4_test.c  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_xdp_nodeport_lb4_test.c.out /home/sayandes/codequery/cilium/tests/xdp_nodeport_lb4_test.c asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_xdp_nodeport_lb4_test.c.out /home/sayandes/codequery/cilium/tests/xdp_nodeport_lb4_test.c asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/custom/bpf_custom.c
File to annotate -  /home/sayandes/codequery/cilium/custom/bpf_custom.c output in brown_txl_annotate_cilium/annotate_func_cilium_custom_bpf_custom.c.xml brown_txl_annotate_cilium/annotate_struct_cilium_custom_bpf_custom.c.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_custom_bpf_custom.c.xml /home/sayandes/codequery/cilium/custom/bpf_custom.c  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_custom_bpf_custom.c.out /home/sayandes/codequery/cilium/custom/bpf_custom.c asset/c-extract-struct.txl
Running:  readlink -f cilium/node_config.h
File to annotate -  /home/sayandes/codequery/cilium/node_config.h output in brown_txl_annotate_cilium/annotate_func_cilium_node_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_node_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_node_config.h.xml /home/sayandes/codequery/cilium/node_config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_node_config.h.out /home/sayandes/codequery/cilium/node_config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/filter_config.h
File to annotate -  /home/sayandes/codequery/cilium/filter_config.h output in brown_txl_annotate_cilium/annotate_func_cilium_filter_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_filter_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_filter_config.h.xml /home/sayandes/codequery/cilium/filter_config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_filter_config.h.out /home/sayandes/codequery/cilium/filter_config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/bpf_features.h
File to annotate -  /home/sayandes/codequery/cilium/bpf_features.h output in brown_txl_annotate_cilium/annotate_func_cilium_bpf_features.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_bpf_features.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_bpf_features.h.xml /home/sayandes/codequery/cilium/bpf_features.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_bpf_features.h.out /home/sayandes/codequery/cilium/bpf_features.h asset/c-extract-struct.txl
Running:  readlink -f cilium/netdev_config.h
File to annotate -  /home/sayandes/codequery/cilium/netdev_config.h output in brown_txl_annotate_cilium/annotate_func_cilium_netdev_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_netdev_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_netdev_config.h.xml /home/sayandes/codequery/cilium/netdev_config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_netdev_config.h.out /home/sayandes/codequery/cilium/netdev_config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/ep_config.h
File to annotate -  /home/sayandes/codequery/cilium/ep_config.h output in brown_txl_annotate_cilium/annotate_func_cilium_ep_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_ep_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_ep_config.h.xml /home/sayandes/codequery/cilium/ep_config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_ep_config.h.out /home/sayandes/codequery/cilium/ep_config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/sockops/sockops_config.h
File to annotate -  /home/sayandes/codequery/cilium/sockops/sockops_config.h output in brown_txl_annotate_cilium/annotate_func_cilium_sockops_sockops_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_sockops_sockops_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_sockops_sockops_config.h.xml /home/sayandes/codequery/cilium/sockops/sockops_config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_sockops_sockops_config.h.out /home/sayandes/codequery/cilium/sockops/sockops_config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/sockops/bpf_sockops.h
File to annotate -  /home/sayandes/codequery/cilium/sockops/bpf_sockops.h output in brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_sockops.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_sockops.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_sockops_bpf_sockops.h.xml /home/sayandes/codequery/cilium/sockops/bpf_sockops.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_sockops_bpf_sockops.h.out /home/sayandes/codequery/cilium/sockops/bpf_sockops.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/builtin_memcpy.h
File to annotate -  /home/sayandes/codequery/cilium/tests/builtin_memcpy.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memcpy.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memcpy.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memcpy.h.xml /home/sayandes/codequery/cilium/tests/builtin_memcpy.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memcpy.h.out /home/sayandes/codequery/cilium/tests/builtin_memcpy.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/builtin_memmove.h
File to annotate -  /home/sayandes/codequery/cilium/tests/builtin_memmove.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memmove.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memmove.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memmove.h.xml /home/sayandes/codequery/cilium/tests/builtin_memmove.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memmove.h.out /home/sayandes/codequery/cilium/tests/builtin_memmove.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/builtin_test.h
File to annotate -  /home/sayandes/codequery/cilium/tests/builtin_test.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_test.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_test.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_test.h.xml /home/sayandes/codequery/cilium/tests/builtin_test.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_test.h.out /home/sayandes/codequery/cilium/tests/builtin_test.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/builtin_memzero.h
File to annotate -  /home/sayandes/codequery/cilium/tests/builtin_memzero.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memzero.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memzero.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memzero.h.xml /home/sayandes/codequery/cilium/tests/builtin_memzero.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memzero.h.out /home/sayandes/codequery/cilium/tests/builtin_memzero.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/common.h
File to annotate -  /home/sayandes/codequery/cilium/tests/common.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_common.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_common.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_common.h.xml /home/sayandes/codequery/cilium/tests/common.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_common.h.out /home/sayandes/codequery/cilium/tests/common.h asset/c-extract-struct.txl
Running:  readlink -f cilium/tests/builtin_memcmp.h
File to annotate -  /home/sayandes/codequery/cilium/tests/builtin_memcmp.h output in brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memcmp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memcmp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_tests_builtin_memcmp.h.xml /home/sayandes/codequery/cilium/tests/builtin_memcmp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_tests_builtin_memcmp.h.out /home/sayandes/codequery/cilium/tests/builtin_memcmp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/l4.h
File to annotate -  /home/sayandes/codequery/cilium/lib/l4.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_l4.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_l4.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_l4.h.xml /home/sayandes/codequery/cilium/lib/l4.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_l4.h.out /home/sayandes/codequery/cilium/lib/l4.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/pcap.h
File to annotate -  /home/sayandes/codequery/cilium/lib/pcap.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_pcap.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_pcap.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_pcap.h.xml /home/sayandes/codequery/cilium/lib/pcap.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_pcap.h.xml /home/sayandes/codequery/cilium/lib/pcap.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_pcap.h.out /home/sayandes/codequery/cilium/lib/pcap.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_pcap.h.out /home/sayandes/codequery/cilium/lib/pcap.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/hash.h
File to annotate -  /home/sayandes/codequery/cilium/lib/hash.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_hash.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_hash.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_hash.h.xml /home/sayandes/codequery/cilium/lib/hash.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_hash.h.out /home/sayandes/codequery/cilium/lib/hash.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/conntrack_map.h
File to annotate -  /home/sayandes/codequery/cilium/lib/conntrack_map.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_conntrack_map.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_conntrack_map.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_conntrack_map.h.xml /home/sayandes/codequery/cilium/lib/conntrack_map.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_conntrack_map.h.out /home/sayandes/codequery/cilium/lib/conntrack_map.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/egress_policies.h
File to annotate -  /home/sayandes/codequery/cilium/lib/egress_policies.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_egress_policies.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_egress_policies.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_egress_policies.h.xml /home/sayandes/codequery/cilium/lib/egress_policies.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_egress_policies.h.out /home/sayandes/codequery/cilium/lib/egress_policies.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/trace.h
File to annotate -  /home/sayandes/codequery/cilium/lib/trace.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_trace.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_trace.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_trace.h.xml /home/sayandes/codequery/cilium/lib/trace.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_trace.h.xml /home/sayandes/codequery/cilium/lib/trace.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_trace.h.out /home/sayandes/codequery/cilium/lib/trace.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_trace.h.out /home/sayandes/codequery/cilium/lib/trace.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/overloadable_skb.h
File to annotate -  /home/sayandes/codequery/cilium/lib/overloadable_skb.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_skb.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_skb.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_skb.h.xml /home/sayandes/codequery/cilium/lib/overloadable_skb.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_skb.h.xml /home/sayandes/codequery/cilium/lib/overloadable_skb.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_skb.h.out /home/sayandes/codequery/cilium/lib/overloadable_skb.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_skb.h.out /home/sayandes/codequery/cilium/lib/overloadable_skb.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/host_firewall.h
File to annotate -  /home/sayandes/codequery/cilium/lib/host_firewall.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_host_firewall.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_host_firewall.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_host_firewall.h.xml /home/sayandes/codequery/cilium/lib/host_firewall.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_host_firewall.h.out /home/sayandes/codequery/cilium/lib/host_firewall.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/l3.h
File to annotate -  /home/sayandes/codequery/cilium/lib/l3.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_l3.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_l3.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_l3.h.xml /home/sayandes/codequery/cilium/lib/l3.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_l3.h.out /home/sayandes/codequery/cilium/lib/l3.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/static_data.h
File to annotate -  /home/sayandes/codequery/cilium/lib/static_data.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_static_data.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_static_data.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_static_data.h.xml /home/sayandes/codequery/cilium/lib/static_data.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_static_data.h.out /home/sayandes/codequery/cilium/lib/static_data.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/overloadable.h
File to annotate -  /home/sayandes/codequery/cilium/lib/overloadable.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable.h.xml /home/sayandes/codequery/cilium/lib/overloadable.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable.h.out /home/sayandes/codequery/cilium/lib/overloadable.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/signal.h
File to annotate -  /home/sayandes/codequery/cilium/lib/signal.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_signal.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_signal.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_signal.h.xml /home/sayandes/codequery/cilium/lib/signal.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_signal.h.out /home/sayandes/codequery/cilium/lib/signal.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/conntrack.h
File to annotate -  /home/sayandes/codequery/cilium/lib/conntrack.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_conntrack.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_conntrack.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_conntrack.h.xml /home/sayandes/codequery/cilium/lib/conntrack.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_conntrack.h.xml /home/sayandes/codequery/cilium/lib/conntrack.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_conntrack.h.out /home/sayandes/codequery/cilium/lib/conntrack.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_conntrack.h.out /home/sayandes/codequery/cilium/lib/conntrack.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/metrics.h
File to annotate -  /home/sayandes/codequery/cilium/lib/metrics.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_metrics.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_metrics.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_metrics.h.xml /home/sayandes/codequery/cilium/lib/metrics.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_metrics.h.out /home/sayandes/codequery/cilium/lib/metrics.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/jhash.h
File to annotate -  /home/sayandes/codequery/cilium/lib/jhash.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_jhash.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_jhash.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_jhash.h.xml /home/sayandes/codequery/cilium/lib/jhash.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_jhash.h.out /home/sayandes/codequery/cilium/lib/jhash.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/maps.h
File to annotate -  /home/sayandes/codequery/cilium/lib/maps.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_maps.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_maps.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_maps.h.xml /home/sayandes/codequery/cilium/lib/maps.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_maps.h.xml /home/sayandes/codequery/cilium/lib/maps.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_maps.h.out /home/sayandes/codequery/cilium/lib/maps.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_maps.h.out /home/sayandes/codequery/cilium/lib/maps.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/eps.h
File to annotate -  /home/sayandes/codequery/cilium/lib/eps.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_eps.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_eps.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_eps.h.xml /home/sayandes/codequery/cilium/lib/eps.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_eps.h.out /home/sayandes/codequery/cilium/lib/eps.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/tailcall.h
File to annotate -  /home/sayandes/codequery/cilium/lib/tailcall.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_tailcall.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_tailcall.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_tailcall.h.xml /home/sayandes/codequery/cilium/lib/tailcall.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_tailcall.h.out /home/sayandes/codequery/cilium/lib/tailcall.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/nodeport.h
File to annotate -  /home/sayandes/codequery/cilium/lib/nodeport.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_nodeport.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_nodeport.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_nodeport.h.xml /home/sayandes/codequery/cilium/lib/nodeport.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_nodeport.h.xml /home/sayandes/codequery/cilium/lib/nodeport.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_nodeport.h.out /home/sayandes/codequery/cilium/lib/nodeport.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_nodeport.h.out /home/sayandes/codequery/cilium/lib/nodeport.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/identity.h
File to annotate -  /home/sayandes/codequery/cilium/lib/identity.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_identity.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_identity.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_identity.h.xml /home/sayandes/codequery/cilium/lib/identity.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_identity.h.out /home/sayandes/codequery/cilium/lib/identity.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/ipv4.h
File to annotate -  /home/sayandes/codequery/cilium/lib/ipv4.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_ipv4.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_ipv4.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_ipv4.h.xml /home/sayandes/codequery/cilium/lib/ipv4.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_ipv4.h.out /home/sayandes/codequery/cilium/lib/ipv4.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/overloadable_xdp.h
File to annotate -  /home/sayandes/codequery/cilium/lib/overloadable_xdp.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_xdp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_xdp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_xdp.h.xml /home/sayandes/codequery/cilium/lib/overloadable_xdp.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_overloadable_xdp.h.xml /home/sayandes/codequery/cilium/lib/overloadable_xdp.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_xdp.h.out /home/sayandes/codequery/cilium/lib/overloadable_xdp.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_overloadable_xdp.h.out /home/sayandes/codequery/cilium/lib/overloadable_xdp.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/ipv6.h
File to annotate -  /home/sayandes/codequery/cilium/lib/ipv6.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_ipv6.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_ipv6.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_ipv6.h.xml /home/sayandes/codequery/cilium/lib/ipv6.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_ipv6.h.out /home/sayandes/codequery/cilium/lib/ipv6.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/utils.h
File to annotate -  /home/sayandes/codequery/cilium/lib/utils.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_utils.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_utils.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_utils.h.xml /home/sayandes/codequery/cilium/lib/utils.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_utils.h.out /home/sayandes/codequery/cilium/lib/utils.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/time.h
File to annotate -  /home/sayandes/codequery/cilium/lib/time.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_time.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_time.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_time.h.xml /home/sayandes/codequery/cilium/lib/time.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_time.h.out /home/sayandes/codequery/cilium/lib/time.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/qm.h
File to annotate -  /home/sayandes/codequery/cilium/lib/qm.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_qm.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_qm.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_qm.h.xml /home/sayandes/codequery/cilium/lib/qm.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_qm.h.xml /home/sayandes/codequery/cilium/lib/qm.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_qm.h.out /home/sayandes/codequery/cilium/lib/qm.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_qm.h.out /home/sayandes/codequery/cilium/lib/qm.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/proxy_hairpin.h
File to annotate -  /home/sayandes/codequery/cilium/lib/proxy_hairpin.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_proxy_hairpin.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_proxy_hairpin.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_proxy_hairpin.h.xml /home/sayandes/codequery/cilium/lib/proxy_hairpin.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_proxy_hairpin.h.out /home/sayandes/codequery/cilium/lib/proxy_hairpin.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/mono.h
File to annotate -  /home/sayandes/codequery/cilium/lib/mono.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_mono.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_mono.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_mono.h.xml /home/sayandes/codequery/cilium/lib/mono.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_mono.h.out /home/sayandes/codequery/cilium/lib/mono.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/edt.h
File to annotate -  /home/sayandes/codequery/cilium/lib/edt.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_edt.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_edt.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_edt.h.xml /home/sayandes/codequery/cilium/lib/edt.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_edt.h.xml /home/sayandes/codequery/cilium/lib/edt.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_edt.h.out /home/sayandes/codequery/cilium/lib/edt.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_edt.h.out /home/sayandes/codequery/cilium/lib/edt.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/policy.h
File to annotate -  /home/sayandes/codequery/cilium/lib/policy.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_policy.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_policy.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_policy.h.xml /home/sayandes/codequery/cilium/lib/policy.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_policy.h.out /home/sayandes/codequery/cilium/lib/policy.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/common.h
File to annotate -  /home/sayandes/codequery/cilium/lib/common.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_common.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_common.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_common.h.xml /home/sayandes/codequery/cilium/lib/common.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_common.h.xml /home/sayandes/codequery/cilium/lib/common.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_common.h.out /home/sayandes/codequery/cilium/lib/common.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_common.h.out /home/sayandes/codequery/cilium/lib/common.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/drop.h
File to annotate -  /home/sayandes/codequery/cilium/lib/drop.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_drop.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_drop.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_drop.h.xml /home/sayandes/codequery/cilium/lib/drop.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_drop.h.out /home/sayandes/codequery/cilium/lib/drop.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/dbg.h
File to annotate -  /home/sayandes/codequery/cilium/lib/dbg.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_dbg.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_dbg.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_dbg.h.xml /home/sayandes/codequery/cilium/lib/dbg.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_dbg.h.xml /home/sayandes/codequery/cilium/lib/dbg.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_dbg.h.out /home/sayandes/codequery/cilium/lib/dbg.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_dbg.h.out /home/sayandes/codequery/cilium/lib/dbg.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/encrypt.h
File to annotate -  /home/sayandes/codequery/cilium/lib/encrypt.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_encrypt.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_encrypt.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_encrypt.h.xml /home/sayandes/codequery/cilium/lib/encrypt.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_encrypt.h.out /home/sayandes/codequery/cilium/lib/encrypt.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/proxy.h
File to annotate -  /home/sayandes/codequery/cilium/lib/proxy.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_proxy.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_proxy.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_proxy.h.xml /home/sayandes/codequery/cilium/lib/proxy.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_proxy.h.xml /home/sayandes/codequery/cilium/lib/proxy.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_proxy.h.out /home/sayandes/codequery/cilium/lib/proxy.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_proxy.h.out /home/sayandes/codequery/cilium/lib/proxy.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/lxc.h
File to annotate -  /home/sayandes/codequery/cilium/lib/lxc.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_lxc.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_lxc.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_lxc.h.xml /home/sayandes/codequery/cilium/lib/lxc.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_lxc.h.xml /home/sayandes/codequery/cilium/lib/lxc.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_lxc.h.out /home/sayandes/codequery/cilium/lib/lxc.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_lxc.h.out /home/sayandes/codequery/cilium/lib/lxc.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/fib.h
File to annotate -  /home/sayandes/codequery/cilium/lib/fib.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_fib.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_fib.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_fib.h.xml /home/sayandes/codequery/cilium/lib/fib.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_fib.h.xml /home/sayandes/codequery/cilium/lib/fib.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_fib.h.out /home/sayandes/codequery/cilium/lib/fib.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_fib.h.out /home/sayandes/codequery/cilium/lib/fib.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/encap.h
File to annotate -  /home/sayandes/codequery/cilium/lib/encap.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_encap.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_encap.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_encap.h.xml /home/sayandes/codequery/cilium/lib/encap.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_encap.h.xml /home/sayandes/codequery/cilium/lib/encap.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_encap.h.out /home/sayandes/codequery/cilium/lib/encap.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_encap.h.out /home/sayandes/codequery/cilium/lib/encap.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/ids.h
File to annotate -  /home/sayandes/codequery/cilium/lib/ids.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_ids.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_ids.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_ids.h.xml /home/sayandes/codequery/cilium/lib/ids.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_ids.h.out /home/sayandes/codequery/cilium/lib/ids.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/stubs.h
File to annotate -  /home/sayandes/codequery/cilium/lib/stubs.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_stubs.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_stubs.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_stubs.h.xml /home/sayandes/codequery/cilium/lib/stubs.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_stubs.h.out /home/sayandes/codequery/cilium/lib/stubs.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/icmp6.h
File to annotate -  /home/sayandes/codequery/cilium/lib/icmp6.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_icmp6.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_icmp6.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_icmp6.h.xml /home/sayandes/codequery/cilium/lib/icmp6.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_icmp6.h.xml /home/sayandes/codequery/cilium/lib/icmp6.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_icmp6.h.out /home/sayandes/codequery/cilium/lib/icmp6.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_icmp6.h.out /home/sayandes/codequery/cilium/lib/icmp6.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/lb.h
File to annotate -  /home/sayandes/codequery/cilium/lib/lb.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_lb.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_lb.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_lb.h.xml /home/sayandes/codequery/cilium/lib/lb.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_lb.h.xml /home/sayandes/codequery/cilium/lib/lb.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_lb.h.out /home/sayandes/codequery/cilium/lib/lb.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_lb.h.out /home/sayandes/codequery/cilium/lib/lb.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/arp.h
File to annotate -  /home/sayandes/codequery/cilium/lib/arp.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_arp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_arp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_arp.h.xml /home/sayandes/codequery/cilium/lib/arp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_arp.h.out /home/sayandes/codequery/cilium/lib/arp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/csum.h
File to annotate -  /home/sayandes/codequery/cilium/lib/csum.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_csum.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_csum.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_csum.h.xml /home/sayandes/codequery/cilium/lib/csum.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_csum.h.out /home/sayandes/codequery/cilium/lib/csum.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/endian.h
File to annotate -  /home/sayandes/codequery/cilium/lib/endian.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_endian.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_endian.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_endian.h.xml /home/sayandes/codequery/cilium/lib/endian.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_endian.h.out /home/sayandes/codequery/cilium/lib/endian.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/policy_log.h
File to annotate -  /home/sayandes/codequery/cilium/lib/policy_log.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_policy_log.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_policy_log.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_policy_log.h.xml /home/sayandes/codequery/cilium/lib/policy_log.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_policy_log.h.xml /home/sayandes/codequery/cilium/lib/policy_log.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_policy_log.h.out /home/sayandes/codequery/cilium/lib/policy_log.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_policy_log.h.out /home/sayandes/codequery/cilium/lib/policy_log.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/eth.h
File to annotate -  /home/sayandes/codequery/cilium/lib/eth.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_eth.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_eth.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_eth.h.xml /home/sayandes/codequery/cilium/lib/eth.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_eth.h.out /home/sayandes/codequery/cilium/lib/eth.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/time_cache.h
File to annotate -  /home/sayandes/codequery/cilium/lib/time_cache.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_time_cache.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_time_cache.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_time_cache.h.xml /home/sayandes/codequery/cilium/lib/time_cache.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_time_cache.h.out /home/sayandes/codequery/cilium/lib/time_cache.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/nat_46x64.h
File to annotate -  /home/sayandes/codequery/cilium/lib/nat_46x64.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_nat_46x64.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_nat_46x64.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_nat_46x64.h.xml /home/sayandes/codequery/cilium/lib/nat_46x64.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_nat_46x64.h.out /home/sayandes/codequery/cilium/lib/nat_46x64.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/config.h
File to annotate -  /home/sayandes/codequery/cilium/lib/config.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_config.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_config.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_config.h.xml /home/sayandes/codequery/cilium/lib/config.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_config.h.out /home/sayandes/codequery/cilium/lib/config.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/events.h
File to annotate -  /home/sayandes/codequery/cilium/lib/events.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_events.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_events.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_events.h.xml /home/sayandes/codequery/cilium/lib/events.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_events.h.out /home/sayandes/codequery/cilium/lib/events.h asset/c-extract-struct.txl
Running:  readlink -f cilium/lib/nat.h
File to annotate -  /home/sayandes/codequery/cilium/lib/nat.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_nat.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_nat.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_nat.h.xml /home/sayandes/codequery/cilium/lib/nat.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_nat.h.xml /home/sayandes/codequery/cilium/lib/nat.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_nat.h.out /home/sayandes/codequery/cilium/lib/nat.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_nat.h.out /home/sayandes/codequery/cilium/lib/nat.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/lib/ghash.h
File to annotate -  /home/sayandes/codequery/cilium/lib/ghash.h output in brown_txl_annotate_cilium/annotate_func_cilium_lib_ghash.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_lib_ghash.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_lib_ghash.h.xml /home/sayandes/codequery/cilium/lib/ghash.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_lib_ghash.h.out /home/sayandes/codequery/cilium/lib/ghash.h asset/c-extract-struct.txl
Running:  readlink -f cilium/custom/bytecount.h
File to annotate -  /home/sayandes/codequery/cilium/custom/bytecount.h output in brown_txl_annotate_cilium/annotate_func_cilium_custom_bytecount.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_custom_bytecount.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_custom_bytecount.h.xml /home/sayandes/codequery/cilium/custom/bytecount.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_custom_bytecount.h.out /home/sayandes/codequery/cilium/custom/bytecount.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/socket.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/socket.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_socket.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_socket.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_socket.h.xml /home/sayandes/codequery/cilium/include/linux/socket.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_socket.h.out /home/sayandes/codequery/cilium/include/linux/socket.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/in.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/in.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_in.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_in.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_in.h.xml /home/sayandes/codequery/cilium/include/linux/in.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_in.h.out /home/sayandes/codequery/cilium/include/linux/in.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/byteorder.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/byteorder.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder.h.xml /home/sayandes/codequery/cilium/include/linux/byteorder.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder.h.out /home/sayandes/codequery/cilium/include/linux/byteorder.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/swab.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/swab.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_swab.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_swab.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_swab.h.xml /home/sayandes/codequery/cilium/include/linux/swab.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_swab.h.out /home/sayandes/codequery/cilium/include/linux/swab.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/icmpv6.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/icmpv6.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_icmpv6.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_icmpv6.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_icmpv6.h.xml /home/sayandes/codequery/cilium/include/linux/icmpv6.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_icmpv6.h.out /home/sayandes/codequery/cilium/include/linux/icmpv6.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/in6.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/in6.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_in6.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_in6.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_in6.h.xml /home/sayandes/codequery/cilium/include/linux/in6.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_in6.h.out /home/sayandes/codequery/cilium/include/linux/in6.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/icmp.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/icmp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_icmp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_icmp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_icmp.h.xml /home/sayandes/codequery/cilium/include/linux/icmp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_icmp.h.out /home/sayandes/codequery/cilium/include/linux/icmp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/if_packet.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/if_packet.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_packet.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_packet.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_packet.h.xml /home/sayandes/codequery/cilium/include/linux/if_packet.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_packet.h.out /home/sayandes/codequery/cilium/include/linux/if_packet.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/ip.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/ip.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_ip.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_ip.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_ip.h.xml /home/sayandes/codequery/cilium/include/linux/ip.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_ip.h.out /home/sayandes/codequery/cilium/include/linux/ip.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/ipv6.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/ipv6.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_ipv6.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_ipv6.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_ipv6.h.xml /home/sayandes/codequery/cilium/include/linux/ipv6.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_ipv6.h.out /home/sayandes/codequery/cilium/include/linux/ipv6.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/if_ether.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/if_ether.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_ether.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_ether.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_ether.h.xml /home/sayandes/codequery/cilium/include/linux/if_ether.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_ether.h.out /home/sayandes/codequery/cilium/include/linux/if_ether.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/bpf.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/bpf.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_bpf.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_bpf.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_bpf.h.xml /home/sayandes/codequery/cilium/include/linux/bpf.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_bpf.h.out /home/sayandes/codequery/cilium/include/linux/bpf.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/types.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/types.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_types.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_types.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_types.h.xml /home/sayandes/codequery/cilium/include/linux/types.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_types.h.out /home/sayandes/codequery/cilium/include/linux/types.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/bpf_common.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/bpf_common.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_bpf_common.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_bpf_common.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_bpf_common.h.xml /home/sayandes/codequery/cilium/include/linux/bpf_common.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_bpf_common.h.out /home/sayandes/codequery/cilium/include/linux/bpf_common.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/if_arp.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/if_arp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_arp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_arp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_if_arp.h.xml /home/sayandes/codequery/cilium/include/linux/if_arp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_if_arp.h.out /home/sayandes/codequery/cilium/include/linux/if_arp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/udp.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/udp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_udp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_udp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_udp.h.xml /home/sayandes/codequery/cilium/include/linux/udp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_udp.h.out /home/sayandes/codequery/cilium/include/linux/udp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/tcp.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/tcp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_tcp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_tcp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_tcp.h.xml /home/sayandes/codequery/cilium/include/linux/tcp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_tcp.h.out /home/sayandes/codequery/cilium/include/linux/tcp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/byteorder/big_endian.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/byteorder/big_endian.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder_big_endian.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder_big_endian.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder_big_endian.h.xml /home/sayandes/codequery/cilium/include/linux/byteorder/big_endian.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder_big_endian.h.out /home/sayandes/codequery/cilium/include/linux/byteorder/big_endian.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/linux/byteorder/little_endian.h
File to annotate -  /home/sayandes/codequery/cilium/include/linux/byteorder/little_endian.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder_little_endian.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder_little_endian.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_linux_byteorder_little_endian.h.xml /home/sayandes/codequery/cilium/include/linux/byteorder/little_endian.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_linux_byteorder_little_endian.h.out /home/sayandes/codequery/cilium/include/linux/byteorder/little_endian.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/loader.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/loader.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_loader.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_loader.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_loader.h.xml /home/sayandes/codequery/cilium/include/bpf/loader.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_loader.h.out /home/sayandes/codequery/cilium/include/bpf/loader.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/api.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/api.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_api.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_api.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_api.h.xml /home/sayandes/codequery/cilium/include/bpf/api.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_api.h.out /home/sayandes/codequery/cilium/include/bpf/api.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/features.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/features.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features.h.xml /home/sayandes/codequery/cilium/include/bpf/features.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features.h.out /home/sayandes/codequery/cilium/include/bpf/features.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/helpers_skb.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/helpers_skb.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers_skb.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers_skb.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers_skb.h.xml /home/sayandes/codequery/cilium/include/bpf/helpers_skb.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers_skb.h.out /home/sayandes/codequery/cilium/include/bpf/helpers_skb.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/builtins.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/builtins.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_builtins.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_builtins.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_builtins.h.xml /home/sayandes/codequery/cilium/include/bpf/builtins.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_builtins.h.xml /home/sayandes/codequery/cilium/include/bpf/builtins.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_builtins.h.out /home/sayandes/codequery/cilium/include/bpf/builtins.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_builtins.h.out /home/sayandes/codequery/cilium/include/bpf/builtins.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/include/bpf/tailcall.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/tailcall.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_tailcall.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_tailcall.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_tailcall.h.xml /home/sayandes/codequery/cilium/include/bpf/tailcall.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_tailcall.h.out /home/sayandes/codequery/cilium/include/bpf/tailcall.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/features_skb.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/features_skb.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features_skb.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features_skb.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features_skb.h.xml /home/sayandes/codequery/cilium/include/bpf/features_skb.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features_skb.h.out /home/sayandes/codequery/cilium/include/bpf/features_skb.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/types_mapper.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/types_mapper.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_types_mapper.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_types_mapper.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_types_mapper.h.xml /home/sayandes/codequery/cilium/include/bpf/types_mapper.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_types_mapper.h.out /home/sayandes/codequery/cilium/include/bpf/types_mapper.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/stddef.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/stddef.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_stddef.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_stddef.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_stddef.h.xml /home/sayandes/codequery/cilium/include/bpf/stddef.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_stddef.h.out /home/sayandes/codequery/cilium/include/bpf/stddef.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/helpers.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/helpers.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers.h.xml /home/sayandes/codequery/cilium/include/bpf/helpers.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers.h.out /home/sayandes/codequery/cilium/include/bpf/helpers.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/verifier.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/verifier.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_verifier.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_verifier.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_verifier.h.xml /home/sayandes/codequery/cilium/include/bpf/verifier.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_verifier.h.out /home/sayandes/codequery/cilium/include/bpf/verifier.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/features_xdp.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/features_xdp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features_xdp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features_xdp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_features_xdp.h.xml /home/sayandes/codequery/cilium/include/bpf/features_xdp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_features_xdp.h.out /home/sayandes/codequery/cilium/include/bpf/features_xdp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/section.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/section.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_section.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_section.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_section.h.xml /home/sayandes/codequery/cilium/include/bpf/section.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_section.h.out /home/sayandes/codequery/cilium/include/bpf/section.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/access.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/access.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_access.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_access.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_access.h.xml /home/sayandes/codequery/cilium/include/bpf/access.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_access.h.out /home/sayandes/codequery/cilium/include/bpf/access.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/csum.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/csum.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_csum.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_csum.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_csum.h.xml /home/sayandes/codequery/cilium/include/bpf/csum.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_csum.h.out /home/sayandes/codequery/cilium/include/bpf/csum.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/errno.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/errno.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_errno.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_errno.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_errno.h.xml /home/sayandes/codequery/cilium/include/bpf/errno.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_errno.h.out /home/sayandes/codequery/cilium/include/bpf/errno.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/compiler.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/compiler.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_compiler.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_compiler.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_compiler.h.xml /home/sayandes/codequery/cilium/include/bpf/compiler.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_compiler.h.out /home/sayandes/codequery/cilium/include/bpf/compiler.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/helpers_xdp.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/helpers_xdp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers_xdp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers_xdp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_helpers_xdp.h.xml /home/sayandes/codequery/cilium/include/bpf/helpers_xdp.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_helpers_xdp.h.out /home/sayandes/codequery/cilium/include/bpf/helpers_xdp.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/ctx/unspec.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/ctx/unspec.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_unspec.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_unspec.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_unspec.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/unspec.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_unspec.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/unspec.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/ctx/skb.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/ctx/skb.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_skb.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_skb.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_skb.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/skb.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_skb.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/skb.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_skb.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/skb.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_skb.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/skb.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/include/bpf/ctx/common.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/ctx/common.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_common.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_common.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_common.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/common.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_common.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/common.h asset/c-extract-struct.txl
Running:  readlink -f cilium/include/bpf/ctx/xdp.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/ctx/xdp.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_xdp.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_xdp.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_xdp.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/xdp.h  asset/c-extract-functions.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_xdp.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/xdp.h  asset/c-extract-functions.txl  Exiting...
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_xdp.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/xdp.h asset/c-extract-struct.txl
Failed while running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_xdp.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/xdp.h asset/c-extract-struct.txl  Exiting...
Running:  readlink -f cilium/include/bpf/ctx/ctx.h
File to annotate -  /home/sayandes/codequery/cilium/include/bpf/ctx/ctx.h output in brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_ctx.h.xml brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_ctx.h.out
Running:  txl -o brown_txl_annotate_cilium/annotate_func_cilium_include_bpf_ctx_ctx.h.xml /home/sayandes/codequery/cilium/include/bpf/ctx/ctx.h  asset/c-extract-functions.txl
Running:  txl -o brown_txl_annotate_cilium/annotate_struct_cilium_include_bpf_ctx_ctx.h.out /home/sayandes/codequery/cilium/include/bpf/ctx/ctx.h asset/c-extract-struct.txl
