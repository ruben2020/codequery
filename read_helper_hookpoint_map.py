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


fname = './helper_hookpoint_map.json'
data = load_bpf_helper_map(fname)

val1 = check_and_return_helper_present(data, "sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "in sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")
else:
    print("nothing found in sufiuygfytgfyatf bpf_override_return yfgiygfaiyt")

val1 = check_and_return_helper_present(data, "sufiuygfytgfyatf  yfgiygfaiyt")
if val1 != None:
    print("found ",val1, "in sufiuygfytgfyatf  yfgiygfaiyt" )
else:
    print("nothing found in sufiuygfytgfyatf  yfgiygfaiyt")
