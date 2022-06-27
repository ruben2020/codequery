#Commands to get end line of map definition

 2419  cqsearch -s myproject.db -t bpf_map_lookup_elem  -p 1 -l 800| cut -d "(&" -f 2|cut -d ',' -f 1
 2420  cqsearch -s myproject.db -t bpf_map_lookup_elem  -p 1 -l 800| cut -d "&" -f 2|cut -d ',' -f 1
 2421  cqsearch -s myproject.db -t bpf_map_lookup_elem  -p 1 -l 800| cut -d "&" -f 2|cut -d ',' -f 1|uniq
 2422  cqsearch -s myproject.db -t bpf_map_lookup_elem  -p 1 -l 800
 2423  cqsearch -s myproject.db -t bpf_map_lookup_elem  -p 1 -l 800| cut -d "(" -f 2|cut -d ',' -f 1|uniq
 2424  cqsearch -s myproject.db -t server_id_map -e|grep SEC
 2425  cqsearch -s myproject.db -t server_id_map -e -u |grep SEC
