import re
import os
import json
import np
import argparse

def load_bpf_helper_map(fname):
    with open(fname, 'r') as f:
        data = json.load(f)
    return data


def check_and_return_helper_present(my_dict,line):
    for key in my_dict.keys():
        if line.find(key)>=0:
            return key
    return None

def get_helper_encoding(lines):
    helper_set= set()
    for line in lines:
        present=check_and_return_helper_present(helperdict,line)
        if present != None:
            helper_set.add(present)
    str =  ""
    for helper in helper_set:
        str = str + helper +","
    return str


def set_to_string(my_set):
    str =  ""
    for elem in my_set:
        str = str + elem +","
    return str


def get_read_maps(lines):
    map_read_set=set()
    for line in lines:
        mapname= check_map_access(map_read_fn,line)
        if mapname != None:
            map_read_set.add(mapname)
    return set_to_string(map_read_set)
            
def get_update_maps(lines):
    map_update_set=set()
    for line in lines:
        mapname= check_map_access(map_update_fn,line)
        if mapname != None:
            map_update_set.add(mapname)
    return set_to_string(map_update_set)

def dump_comment(fname,startLineDict, ofname):
    if fname  == "":
        return
    ifile = open(fname,'r')
    #ofname = fname+"-OPENED"
    print("Dumping for: ",fname," outputFile: ",ofname)
    ofile = open(ofname,'w')
    ct = 0
    for line in ifile.readlines():
        ct=ct + 1
        ofile.write(line)
        nextLine = ct + 1
        if nextLine in startLineDict:
            ofile.write(startLineDict.get(nextLine))
    ofile.flush()
    ofile.close()
    ifile.close()
            
def check_map_access(my_arr,line):
    for func in my_arr:
        idx = line.find(func)
        if idx>=0:
            chunks = line[len(func)+idx:].replace('(','')
            first_entry_end = chunks.find(',')
            return chunks[:first_entry_end].replace("&","")
    return None


def generate_comment(srcFile,funcName,startLine,endLine,funcArgs,output,encoding,read_maps,update_maps):
    comment="/* \n OPENED COMMENT BEGIN \n { \n File: "+srcFile + ",\n Startline: "+ str(startLine) + ",\n Endline: "+str(endLine) + ",\n Funcname: "+funcName + ",\n Input: ("+ funcArgs + "),\n Output: "+output + ",\n Helpers: [" + encoding + "]" + ",\n Read_maps: [" + read_maps + "],\n Update_maps: [" + update_maps + "],\n Func Description: TO BE ADDED, \n } \n OPENED COMMENT END \n */ \n"
    #print("COMMENT File: ",srcFile, " startline: ",startLine," endline: ",endLine," funcname: ",funcName, "Input: (", funcArgs, ") Output: ",output, "Helpers: [",encoding,"]", "read_maps: [",read_maps,"] update_maps: [",update_maps,"]")
    return comment

def add_function_size(srcFile,funcName,startLine,endLine,func_sizes):
    key = srcFile+":"+funcName
    size = endLine - startLine + 1
    func_sizes[key]= size
    

# parses output from c-extract-function.txl
def parseTXLFunctionOutputFileForMetrics(inputFile, opFile):
    srcSeen=False
    lines = []
    startLineDict ={}
    funcName=""
    funcArgs=""
    output=""
    startLine = -1
    endLine = -1
    for line in inputFile.readlines():
        ending = re.match(r"</source",line)
        if ending:
            srcSeen = False;
            #dump to file
            #print(lines)
            continue;
        if srcSeen:
            continue;
        starting = re.match(r"<source",line)
        if starting:
            #print("Starting",line)
            srcSeen = True
        
            line = line.replace("funcheader","")
            line = line.replace("startline","")
            line = line.replace("endline","")
            line = line.replace(">","")
            line = line.replace("\n","")
            line = line.replace("\"","")
            tokens = line.split('=')
            #print("len",len(tokens),"tokens",tokens)

            funcHeader=tokens[2]
            #print("funcHeader: ",funcHeader)
            funcArgs = funcHeader.split('(')[-1]
            funcArgs = funcArgs.split(')')[0]
            #funcArgs = funcArgs.replace(" ","")
            #print("args ",funcArgs)
            
            srcFile = tokens[-4]
            srcFile = srcFile.replace(" ","")

            funcName = tokens[-3].replace(" (","(")
            output= funcName.split('(')[-2].split(" ")[-2]
            output = output.replace(" ","")
            #print("funcName: ",funcName)
            funcName = funcName.split('(')[-2].split(" ")[-1]

            startLine = int(tokens[-2])
            endLine = int(tokens[-1])
            #print("File: ",srcFile, " startline: ",startLine," endline: ",endLine," funcname: ",funcName, "Input: (", funcArgs, ") Output: ",output)
            #key=funcName+":"+srcFile+":"+str(startLine)
            key=funcName+":"+srcFile
            #print("Checking if need to extract",key
            print("Going to call add_function_size for: "+srcFile)
            add_function_size(srcFile,funcName,startLine,endLine,func_sizes)
                    
if __name__ =="__main__":
    parser = argparse.ArgumentParser(description='Code commentor')
    parser.add_argument('-o','--opFile', type=str,required=True,
            help='output file to dump commented code ')

    parser.add_argument('-i','--txlDir', type=str,required=True,
            help='TXL annotated files with function and map listings')


    args = parser.parse_args()

    print("Args",args)

    opFile=args.opFile

    txlDir=args.txlDir

    func_sizes={}

    for txlFile in os.listdir(txlDir):
        print("File: ",txlFile)
        if txlFile.endswith(".xml"):
            map_update_fn = ["bpf_sock_map_update", "bpf_map_delete_elem", "bpf_map_update_elem","bpf_map_pop_elem", "bpf_map_push_elem"]
            map_read_fn = ["bpf_map_peek_elem", "bpf_map_lookup_elem", "bpf_map_pop_elem"]
            path = txlDir + "/" + txlFile
            xmlFile = open(path,'r')
            parseTXLFunctionOutputFileForMetrics(xmlFile, opFile)
            xmlFile.close()


    mean = np.array(list(func_sizes.values())).mean()
    print(mean)
