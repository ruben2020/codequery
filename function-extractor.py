from lxml import etree
import re
import os.path
import os
import shutil
from collections import defaultdict


def dump_to_file(f,appt):
    str = "//" + appt.get('file')+ " " + appt.get('startline')+ " "+ appt.get('endline') + " \n"
    f.write(str)
    f.write(appt.text)

def make_extraction_dir(path):
    # Check whether the specified path exists or not
    isExist = os.path.exists(path)

    if isExist:
        for filename in os.listdir(path):
            file_path = os.path.join(path, filename)
            try:
                if os.path.isfile(file_path) or os.path.islink(file_path):
                    os.unlink(file_path)
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)
            except Exception as e:
                print('Failed to delete %s. Reason: %s' % (file_path, e))
    # Create a new directory because it does not exist 
    else:
        os.makedirs(path)
        print("The new directory is created!")
    #os.chdir(path)

# reads cscope.files and copies headers to direction. Also adds headers to headers dict
def copy_include_files(iFile, opdir):
    iFilePtr = open(iFile,'r')
    for line in iFilePtr.readlines():
        print("cscope header: ",line)
        line=line.replace("\n","")
        if line.endswith(".h"):
            shutil.copy(line, opdir)
            headers[line]=1
            print("copying header: ",line)
    shutil.copy("Makefile",opdir)
    iFilePtr.close()

# read iFile from startLine to endLine and writes to oFile
def extractAndDump(iFile,startLine,endLine,oFile):
    print("iFile",iFile, "startline:",startLine," endline: ",endLine)
    if not os.path.exists(iFile):
        print("File Not Found: ",iFile)
        return
    iFilePtr = open(iFile,'r')
    lineCt = 1
    #ignore lines
    while lineCt < startLine:
        print("skipping line#: ",lineCt)
        iFilePtr.readline()
        lineCt = lineCt + 1;
    comment = "/* Extracted from \n "+ iFile+ " \n startLine: "+ str(startLine) + " endLine: "+ str(endLine) + "\n */ \n"
    oFile.write(comment)
    while lineCt <= endLine :
        line = iFilePtr.readline()
        print("lineCt",lineCt, " line: ",line)
        oFile.write(line)
        lineCt= lineCt + 1

    iFilePtr.close()
    return

# parses output from c-functions-extraction.txl
def parseTXLOutputFile(inputFile,f,e):
    srcSeen=False
    lines = []
    for line in inputFile.readlines():
        ending = re.match(r"</source",line)
        if ending:
            srcSeen = False;
            #dump to file
            print(lines)
            lines = []
            continue;
        if srcSeen:
            lines.append(line)
            continue;
        starting = re.match(r"<source",line)
        if starting:
            print("Starting",line)
            srcSeen = True
            line = line.replace("funcheader","")
            line = line.replace("startline","")
            line = line.replace("endline","")
            line = line.replace(">","")
            line = line.replace("\n","")
            line = line.replace("\"","")
            tokens = line.split('=')
            #print("len",len(tokens),"tokens",tokens)
            srcFile = tokens[-4]
            srcFile = srcFile.replace(" ","")
            funcName = tokens[-3].replace(" (","(")
            #print(funcName)
            funcName = funcName.split('(')[-2].split(" ")[-1]
            startLine = int(tokens[-2])
            endLine = int(tokens[-1])
            #funcHeader=tokens[2].split("=")[1]
            print(srcFile, " startline: ",startLine," endline: ",endLine," funcname: ",funcName)
            key=funcName+":"+srcFile+":"+str(startLine)
            print("Checking ",key)
            if key in fns.keys():
                if not srcFile.endswith(".h"):
                    print("Need to Extract", key)
                    op=funcName+","+srcFile+",["+str(startLine)+":"+str(endLine)+"]\n"
                    e.write(op)
                    extractAndDump(srcFile,startLine,endLine,f)
            continue;
    
def parseXML(xmlFile,f):
    """
    Parse the xml
    """
    with open(xmlFile) as fobj:
        xml = fobj.read()

    root = etree.fromstring(xml)
    for appt in root.getchildren():
        dump_to_file(f,appt)
        print("source_code\n")
        print(appt.text)
        print("FileName")
        print(appt.get('file'))
        print(appt.get('funcheader'))
        print(appt.get('startline'))
        print(appt.get('endline'))
        
# read header file and included headers to headers dict
def addDependsOn(headerFile):
    with open(headerFile) as iFile:
        for line in iFile.readlines():
            if "#include" in line:
                h  = line.split()[-1]
                #h = h.replace("<","")
                #h = h.replace(">","")
                h = h.replace("\"","")
                print("headerFile: ",headerFile," h: ",h)
                graph[h].add(headerFile)
                headers[h]=1
                headers[headerFile]=1
    iFile.close()

# Parses output from codequery search output and puts in map
def parseFunctionList(ifile):
    ct = 0
    for line in ifile.readlines():
        #print(line)
        m = re.match(r"[{}]",line)
        if m:
            print("Ignoring",line)
            ct = ct + 1
        else:
            print("ct",ct)
            if ct >= 2:
                return
            #print("Processing", line)
            line = line.replace('[','')
            line = line.replace(']','')
            tokens = line.split(',')
            fnName = tokens[0]
            count = tokens[1]
            if int(count) > 1:
                print("Duplicate Defns: ", line);
                duplicates.append(line)
                continue;
            src = tokens[2]
            if src.endswith(".h"):
                print("Header File: ",line)
                headers[src]=1
                shutil.copy(src,opdir)
                addDependsOn(src)
            startLine = tokens[3]
            #remove end ]
            startLine = startLine[:-1]
            #print(fnName,count,src,startLine)
            key=fnName+":"+src+":"+startLine
            fns[key]=1

            
if __name__ == "__main__":
    #dict containing function definitions
    fns = {}
    
    # dict containing dupcliate fn definitions
    duplicates = []

    #dict contatining header files
    headers = {}

    #dict containing header file dependencies for ordering includes in final generated file
    graph = defaultdict(set)

    #constants
    opdir="extraction"
    codequeryOutputFile="func.out"
    dupFileName=opdir+"/"+"duplicates.out"
    extractedFileName = opdir+"/"+"extracted.c"
    TXLDir ="./txl_annotate"
    extractedFunctionListFile="extractedFuncList.out"
    
    make_extraction_dir(opdir)
    copy_include_files("cscope.files", opdir)
    dupFile = open (dupFileName,'w')
    ifile = open(codequeryOutputFile,'r')
    eFile = open(extractedFunctionListFile,'w')
    
    parseFunctionList(ifile)
    ifile.close()
    #print("Dependencies: \n",graph)

    # Dump duplicate function definitions
    #print("fns: ", fns)
    #print("dups: ",duplicates)
    for dup in duplicates:
        dupFile.write(dup)
    dupFile.close()

    #print("curr: ",os.getcwd())

    #include required header files
    f = open(extractedFileName,'w')
    f.write("/* SPDX-License-Identifier: GPL-2.0 */\n");
    #print("HEADERS\n")
    for header in headers.keys():
        print(header)
        if not "<" in header:
            header = header.split('/')[-1]
            macro = header.upper()
            macro = macro.replace(".","_")
            macro = macro.replace("/","_")
            macro = macro.replace("-","_")
            cmd = "#ifndef "+ macro + "\n"
            cmd += "#define " + macro + "\n"
            cmd += "#include \"" + header + "\"\n"
            cmd += "#endif \n\n"
        else:
            cmd = "#include " + header + "\n"
            
        f.write(cmd)

    #Parse TXL annotated files
    for fName in os.listdir(TXLDir):
        print("annotatedFile: ",fName)
        if fName.endswith(".xml"):
            print("annotatedXmlFile: ",fName)
            path = TXLDir + "/" + fName
            xmlFile = open(path,'r')
            parseTXLOutputFile(xmlFile,f,eFile)
            xmlFile.close()

    f.write("char _license[] SEC(\"license\") = \"GPL\";");
    f.close()
    eFile.close()
