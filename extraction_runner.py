import os
import re
import subprocess
import glob
import command
import shutil
import code_commentor as cmt

def check_if_cmd_available():
    commands = ['txl', 'cscope', 'ctags', 'cqmakedb', 'cqsearch']
    for cmd in commands:
        if shutil.which(cmd) is None:
            print("Command: ",cmd," unavailable.. ", "Exiting")
            return False
    print("All necessary commands found...")
    return True

def check_if_file_available():
    files = [r'asset/c-extract-functions.txl', r'asset/c-extract-struct.txl', r'asset/c.grm.1', r'asset/bom.grm', r'asset/helper_hookpoint_map.json']
    for fl in files:
        if os.path.isfile(fl) is False:
            print("File: ",fl," unavailable.. ", "Exiting")
            return False
    print("All necessary asset files found...")
    return True
#1. make cscope db
#2. do txl annotation
#3. comment generation
#4. cqsearch

#rm cscope.files cscope.out tags myproject.db 
def clean_intermediate_files(file_list):
    for my_file in file_list:
        if os.path.isfile(myfile):
            os.remove(myfile)
        else:
            print("File Not Found: ",my_file)

def run_cmd(cmd):
    print("Running: ",cmd)
    status, output = subprocess.getstatusoutput(cmd)
    if(status != 0):
        print("Failed while running: ",cmd," Exiting...")
        exit(1)
    return output

def create_directories(dir_list):
    for dr in dir_list:
        if not os.path.exists(dr):
            os.mkdir(dr)

'''
echo "Creating new file index ..."
find -L $codedir -name '*.c' >cscope.files
find -L $codedir -name '*.h' >>cscope.files
echo "Running cscope & ctags ..."
cscope -cb -k 
ctags --fields=+i -n -L ./cscope.files
echo "Making DBs ..."
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
'''            
#cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
def make_cscope_db(db_name,code_dir):
    op_file = open('cscope.files','w')
    files = glob.glob(code_dir + '/**/*.c', recursive=True)
    for f in files:
        op_file.write(f)
        op_file.write("\n")

    files = glob.glob(code_dir + '/**/*.h', recursive=True)
    for f in files:
        op_file.write(f)
        op_file.write("\n")
    op_file.close()        
    run_cmd("cscope -cb -k")
    run_cmd("ctags --fields=+i -n -L ./cscope.files")
    run_cmd("cqmakedb -s "+ db_name+ " -c ./cscope.out -t ./tags -p")

def create_txl_annotation(cscope_file, opdir):
    print("Read cscope files and generate function annotation ...")
    txl_dict = {}
    code_f = open(cscope_file,'r')
    for line in code_f.readlines():
        line = line.strip()
        full_line = run_cmd("readlink -f "+line)
        line = re.sub('^\.','',line) 
        line = re.sub('\/','_',line) 
        line = re.sub('^__','',line) 

        opfile_function_annotate= opdir+"/annotate_func_"+line+".xml"
        opfile_struct_annotate= opdir+"/annotate_struct_"+line+".out"
        logfile= opdir+"/LOG"

        print("File to annotate - ",full_line,"output in",opfile_function_annotate,opfile_struct_annotate)
        op = run_cmd("txl -o "+ opfile_function_annotate+" "+full_line+"  asset/c-extract-functions.txl")
        op = run_cmd("txl -o "+opfile_struct_annotate+" "+full_line +" asset/c-extract-struct.txl")
        txl_dict[full_line] = opfile_function_annotate
    return txl_dict

def create_code_comments(txl_dict, bpf_helper_file, opdir):
    map_update_fn = ["bpf_sock_map_update", "bpf_map_delete_elem", "bpf_map_update_elem","bpf_map_pop_elem", "bpf_map_push_elem"]
    map_read_fn = ["bpf_map_peek_elem", "bpf_map_lookup_elem", "bpf_map_pop_elem"]
    helperdict = cmt.load_bpf_helper_map(bpf_helper_file)  
    for srcFile,txlFile in txl_dict.items():
        opFile = opdir+'/'+os.path.basename(srcFile)
        xmlFile = open(txlFile,'r')
        cmt.parseTXLFunctionOutputFileForComments(xmlFile, opFile, srcFile, helperdict, map_update_fn, map_read_fn)
        xmlFile.close()
    return

def create_cqmakedb(db_file, cscope_file, tags_folder):
    run_cmd("cqmakedb -s "+db_file+" -c "+cscope_file+" -t "+tags_folder+" -p")
    return

def search_function(function_name, db_file):
    print("Running cqsearch for ",function_name," and outputting dependencies to func.out")
    status=run_cmd("cqsearch -s "+db_file+" -t "+function_name+"  -p 7  -l 100 -k 10 -e > func.out")

if __name__ == "__main__":

    if(not check_if_cmd_available() or not check_if_file_available()):
        exit(1)
    print("here")
    dir_list = ["commented","txl_annotate"]
    db_file = "test.db"
    cscope_files = "cscope.files"
    cscope_out = "cscope.out"
    tags_folder = "./tags"
    function_name = "xdpdecap"
    create_directories(dir_list)
    make_cscope_db(db_file,"katran")
    txl_dict = create_txl_annotation(cscope_files,"txl_annotate")
    create_code_comments(txl_dict, "asset/helper_hookpoint_map.json", "commented")
    create_cqmakedb(db_file, cscope_out, tags_folder)
    search_function(function_name, db_file)
  #  run_cqsearch()
