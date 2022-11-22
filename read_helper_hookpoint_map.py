import json

def load_bpf_helper_map(fname):
    with open(fname, 'r') as f:
        data = json.load(f)
    return data

def check_and_return_helper_present(my_dict,line):
    for key in my_dict.keys():
        if line.find(key)>=0:
            return key
    return None
def check_map_access(my_arr,line):
    for func in my_arr:
        idx = line.find(func)
        if idx>=0:
            chunks = line[len(func)+idx:].replace('(','')
            first_entry_end = chunks.find(',')
            return chunks[:first_entry_end]
    return None

fname = './helper_hookpoint_map.json'
data = load_bpf_helper_map(fname)
map_update_fn = ["bpf_sock_map_update", "bpf_map_delete_elem", "bpf_map_update_elem","bpf_map_pop_elem", "bpf_map_push_elem"]
map_read_fn = ["bpf_map_peek_elem", "bpf_map_lookup_elem"]
val1 = check_and_return_helper_present(data, "sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "in sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")
else:
    print("nothing found in sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")

val1 = check_and_return_helper_present(data, "sufiuygfytgfyatf bpf_map_update_elem yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "in sufiuygfytgfyatf  yfgiygfaiyt" )
else:
    print("nothing found in sufiuygfytgfyatf  yfgiygfaiyt")

val1 = check_map_access(map_update_fn, "sufiuygfytgfyatf bpf_map_update_elem ( map_name, key, value); yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "update in  bpf_map_update_elem (map_name, key, value); sufiuygfytgfyatf  yfgiygfaiyt" )
else:
    print("nothing found in sufiuygfytgfyatf bpf_map_lookup_elem(map_name, key, value); yfgiygfaiyt")

val1 = check_map_access(map_read_fn, "sufiuygfytgfyatf bpf_map_lookup_elem(map_name, key, value); yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "read in sufiuygfytgfyatf bpf_map_lookup_elem(map_name, key, value); yfgiygfaiyt" )
else:
    print("nothing found in sufiuygfytgfyatf bpf_map_lookup_elem(map_name, key, value); yfgiygfaiyt")


