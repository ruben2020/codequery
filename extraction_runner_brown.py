import os
import re
import subprocess
import glob
import command
import shutil
import code_commentor as cmt
import argparse


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
        #exit(1) ## TODO:Uncomment
    return output

def create_directories(dir_list):
    for dr in dir_list:
        if not os.path.exists(dr):
            os.mkdir(dr)
            
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


def create_cqmakedb(db_file, cscope_file, tags_folder):
    run_cmd("cqmakedb -s "+db_file+" -c "+cscope_file+" -t "+tags_folder+" -p")
    return

def search_function(function_name, db_file):
    print("Running cqsearch for ",function_name," and outputting dependencies to func.out")
    status=run_cmd("cqsearch -s "+db_file+" -t "+function_name+"  -p 7  -l 100 -k 10 -e > func.out")

if __name__ == "__main__":

    my_parser = argparse.ArgumentParser()
    my_parser.add_argument('-annotate_only',
                       action='store',
                           default=False)
    my_parser.add_argument('-f','--function_name',action='store',required=True)
    my_parser.add_argument('-s','--src_dir',action='store',required=True)
    my_parser.add_argument('-o','--txl_op_dir',action='store',required=True)
    
    args = my_parser.parse_args()
    print(vars(args))
    if(not check_if_cmd_available() or not check_if_file_available()):
        exit(1)
    print("here")
    #dir_list = ["commented","txl_annotate"]
    dir_list = ["commented"]
    db_file = "test.db"
    cscope_files = "cscope.files"
    cscope_out = "cscope.out"
    tags_folder = "./tags"
    #function_name = "xdpdecap"
    function_name= args.function_name
    src_dir = args.src_dir
    txl_op_dir = args.txl_op_dir
    dir_list.append(txl_op_dir)
    create_directories(dir_list)
    #make_cscope_db(db_file,"katran")
    make_cscope_db(db_file,src_dir)
    
    txl_dict = create_txl_annotation(cscope_files,txl_op_dir)
    if args.annotate_only:
        exit(0)
    create_cqmakedb(db_file, cscope_out, tags_folder)
    search_function(function_name, db_file)

