from lxml import etree
import re
import os.path
import os
import shutil
from collections import defaultdict
from collections import OrderedDict

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
# string to search in file

def create_preprocessor_map(filename):

    fp = open(filename, 'r')
        # read all lines using readline()
    lc = 1
    lines = fp.readlines()
    print("Creating preprocessor for: ",filename)
    stack = []
    pres=[]

    for line in lines:
        # check if string present on a current line
        ifdefstring = '#ifdef '
        ifstring = '#if '
        elsestring = '#else '
        endifstring = '#endif '
        elifstring = '#elif '
        ifndefstring = '#ifndef '
        # find() method returns -1 if the value is not found,
        # if found it return 0
        if line.find(ifdefstring) == 0:
            print('string #ifdef exists in file')
            print('line Number:', lc,line)
            tokens = line.split()
            name = tokens[1]
            stack.append((ifdefstring,name,lc))

        if line.find(ifstring) == 0:
            print('string #if exists in file')
            print('line Number:', lc,line)
            tokens = line.split()
            name = tokens[1]
            stack.append((ifstring,name,lc))


        if line.find(ifndefstring) == 0:
            print('string #ifndef exists in file')
            print('line Number:', lc,line)
            tokens = line.split()
            name = tokens[1]
            stack.append((ifndefstring,name,lc))


        if line.find(elifstring) == 0:
           print('string #elif exists in file')
           print('line Number:', lc,line)
           (t,name,bg) = stack.pop()
           pres.append((t,name,bg,lc-1))
           tokens = line.split()
           stack.append((ifdefstring,tokens[1],lc))

        if line.find(elsestring) == 0:
           print('string #else exists in file')
           print('line Number:', lc,line)
           (t,name,bg) = stack.pop()
           pres.append(( t,name,bg,lc-1))
           tokens = line.split()
           stack.append((ifndefstring,name,lc))


        if line.find(endifstring) == 0:
           print('string #endif exists in file')
           print('line Number:', lc,line)
           (t,name,bg) = stack.pop()
           pres.append(( t,name,bg,lc-1))

        lc = lc + 1

    print(pres)       
    return pres

# checks if function/struct in filename between st_line and end_line needs to be guarded with IFDEF MACROS
def contained_in_preprocessor(fname, pres, st_line, end_line):
        print("checking containment in: ",fname,"start: ",st_line,"end: ",end_line)
        for (t,defName,start,end) in pres:
                if st_line >= start and end_line <= end:
                    print("Fully contained ", t, defName)
                    return (t,defName,start,end)
        return (None,None,None,None)
                    
# reads cscope.files and copies headers to direction. Also adds headers to headers dict
def copy_include_files(iFile, opdir):
    iFilePtr = open(iFile,'r')
    for line in iFilePtr.readlines():
        print("cscope header: ",line)
        line=line.replace("\n","")
        if line.endswith(".h"):
            shutil.copy(line, opdir)
            #headers[line]=1
            print("copying header: ",line)
    shutil.copy("Makefile",opdir)
    iFilePtr.close()

# read iFile from startLine to endLine and writes to oFile
def extractAndDump(iFile,startLine,endLine,oFile):
    #print("iFile",iFile, "startline:",startLine," endline: ",endLine)
    if not os.path.exists(iFile):
        print("File Not Found: ",iFile)
        return
    val=None
    #check if we need to guard this func/struct with a macro
    p = presDict[iFile]
    if not p:
        print("ALERT: PREPROCESSING DICT MISSING FOR: ",iFile)
    else:
        (val,name,start,end) = contained_in_preprocessor(iFile,p, startLine, endLine)
    if val:
        cmd = val+" "+name+"\n"
        oFile.write(cmd)
    iFilePtr = open(iFile,'r')
    lineCt = 1
    #ignore lines
    while lineCt < startLine:
        #print("skipping line#: ",lineCt)
        iFilePtr.readline()
        lineCt = lineCt + 1;
    comment = "/* Extracted from \n "+ iFile+ " \n startLine: "+ str(startLine) + " endLine: "+ str(endLine) + "\n */ \n"
    oFile.write(comment)
    while lineCt <= endLine :
        line = iFilePtr.readline()
        #print("lineCt",lineCt, " line: ",line)
        oFile.write(line)
        lineCt= lineCt + 1
            
    iFilePtr.close()
    if val:
        cmd="#endif \n"
        oFile.write(cmd)
    return

#does structStr contain map name that is of interest
def doesStructContainMap(str):
    for key in maps:
        #isMap = re.match(key,str)
        if key in str.split():
            return (True,key)
        
    return (False,None)

def dumpFns(f,e):
    for key in fns:
        (funcName,srcFile,startLine,endLine) = fns[key]
        if not funcName == None and not srcFile.endswith(".h"):
            op=funcName+","+srcFile+",["+str(startLine)+":"+str(endLine)+"]\n"
            e.write(op)
            extractAndDump(srcFile,startLine,endLine,f)
            #del fns[key]
        


# parses output from c-extract-struct.txl
def parseTXLStructOutputFile(fileName,f):
    print("Parsing Struct Output FIle: ",fileName)
    iFile = open(fileName,'r')
    lineCt = 1
    inside = False;
    structStr = ""
    for line in iFile.readlines():
        print(line)
        begin=re.match(r"<struct>",line)
        end = re.match(r"</struct>",line)
        if begin:
            startLine = lineCt + 1
            inside = True;
        elif end:
            endLine = lineCt - 1
            key = fileName+":"+str(startLine)+":"+str(endLine);
            print("EXTRACT -> ",key)
            maps[key]=1
            # Write maps together from dict
            #if not ".h.out" in fileName:
            #    extractAndDump(fileName,startLine,endLine,f)
            inside = False;
            print("StructStr",structStr)
            (isMap,mapName) = doesStructContainMap(structStr)
            if isMap == True:
                head="//fileName "+fileName+" startLine: "+str(startLine)+" endLine: "+str(endLine)+"\n"
                structStr=head+structStr
                opMaps[mapName].append(structStr)
            structStr= ""
        elif inside == True:
            structStr = structStr + line
        lineCt = lineCt + 1;
    iFile.close()
            

    
# parses output from c-extract-function.txl
def parseTXLFunctionOutputFile(inputFile,f,e):
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

            if not srcFile in presDict:
                p = create_preprocessor_map(srcFile)
                presDict[srcFile]=p
            
            funcName = tokens[-3].replace(" (","(")
            #print(funcName)
            funcName = funcName.split('(')[-2].split(" ")[-1]
            startLine = int(tokens[-2])
            endLine = int(tokens[-1])
            #funcHeader=tokens[2].split("=")[1]
            #print(srcFile, " startline: ",startLine," endline: ",endLine," funcname: ",funcName)
            #key=funcName+":"+srcFile+":"+str(startLine)
            key=funcName+":"+srcFile
            print("Checking if need to extract",key)
            if key in fns.keys():
                fns.update({key:(funcName,srcFile,startLine,endLine)})
                if not srcFile.endswith(".h"):
                    print("Need to Extract", key)
                    #op=funcName+","+srcFile+",["+str(startLine)+":"+str(endLine)+"]\n"
                    #e.write(op)
                    #extractAndDump(srcFile,startLine,endLine,f)
                    #del fns[key]
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
def addDependsOn(cFile):
    with open(cFile) as iFile:
        for line in iFile.readlines():
            if "#include" in line:
                h  = line.split()[-1]
                #h = h.replace("<","")
                #h = h.replace(">","")
                h = h.replace("\"","")
                #print("cFile: ",cFile," h: ",h)
                graph[h].add(cFile)
                print("Adding Header Dep: From",cFile," to: ",h )
                headers[h]=1
               
    iFile.close()

def processFuncLine(line):
    ##print("Processing", line)
    line = line.replace('[','')
    line = line.replace(']','')
    tokens = line.split(',')
    fnName = tokens[0]
    count = tokens[1]
    if int(count) > 1:
        #print("Duplicate Defns: ", line);
        duplicates.append(line)
        return
    src = tokens[2]
    #Add headers included by .c files only
    if src.endswith(".c"):
    #if src.endswith(".h"):
        #print("Header File: ",line)
        #headers[src]=1
        #shutil.copy(src,opdir)
        addDependsOn(src)
    startLine = tokens[3]
    #remove end ]
    startLine = startLine[:-1]
    #print(fnName,count,src,startLine)
    #key=fnName+":"+src+":"+startLine
    key=fnName+":"+src
    fns[key]=(None,None,None,None)

def processMapLine(line):
    ##print("Processing", line)
    line = line.replace('[','')
    line = line.replace(']','')
    tokens = line.split(',')
    mapName = tokens[0]
    srcFile = tokens[1]
    if srcFile.endswith(".h"):
        headers[srcFile]=1
        addDependsOn(srcFile)
    startLine = tokens[2]
    isFound = tokens[3]
    #print(fnName,count,src,startLine)
    #key=fnName+":"+src+":"+startLine
    key=mapName
    maps[key]=1


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
            if ct < 2:
                processFuncLine(line)
            else:
                processMapLine(line)
            
            
if __name__ == "__main__":
    #dict containing function definitions
    #fns = {}
    fns = OrderedDict()
    
    # dict containing dupcliate fn definitions
    duplicates = []

    #dict contatining header files
    headers = {}

    #dict containing header file dependencies for ordering includes in final generated file
    graph = defaultdict(set)

    #dict containing map defns to be extracted
    maps = {}

    #dict containing maps that will be written out
    opMaps = defaultdict(list)

    #dict of dict containing pre processor directives per file
    presDict = defaultdict(list)
    
    #constants
    opdir="extraction"
    codequeryOutputFile="func.out"
    dupFileName=opdir+"/"+"duplicates.out"
    extractedFileName = opdir+"/"+"extracted.c"
    TXLDir ="./txl_annotate"
    extractedFunctionListFile="extractedFuncList.out"
    missedFunctionListFile="missedFuncList.out"

    
    make_extraction_dir(opdir)
    copy_include_files("cscope.files", opdir)
    dupFile = open (dupFileName,'w')
    ifile = open(codequeryOutputFile,'r')
    eFile = open(extractedFunctionListFile,'w')
    
    parseFunctionList(ifile)
    ifile.close()
    print("BEFORE: FNS LIST")
    print(fns)
    
    #print("Dependencies: \n",graph)

    # Dump duplicate function definitions
    #print("fns: ", fns)
    #print("dups: ",duplicates)
    for dup in duplicates:
        dupFile.write(dup)
    dupFile.close()

    
    f = open(extractedFileName,'w')

    #include required header files
    f.write("/* SPDX-License-Identifier: GPL-2.0 */\n");
    f.write("#define RECORD_FLOW_INFO\n")
    #print("HEADERS\n")
    for header in headers.keys():
        #print(header)
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

    xmlFiles = []
    structFiles = []
    #Parse TXL annotated files
    for fName in os.listdir(TXLDir):
        print("annotatedFile: ",fName)
        path = TXLDir + "/" + fName
        if fName.endswith(".xml"):
            #print("annotatedXmlFile: ",fName)
            xmlFiles.append(path)
        elif "annotate_struct" in fName:
            structFiles.append(path)
        
    for path in structFiles:
        print("structFIle: ",path)
        p = create_preprocessor_map(path)
        presDict[path]=p
        parseTXLStructOutputFile(path,f)
            
    for mapName  in opMaps:
        isDup=False
        if len(opMaps[mapName]) > 1:
            isDup=True
            cmd = "//ATTENTION BEGIN: DUPLICATE MAP DEFNS\n"
            f.write(cmd)
        for s in opMaps[mapName]:
            # Do not write structs from .h files as they will already be included
            if not ".h" in s:
                f.write(s)
            else:
                if isDup == True:
                    f.write("//DUP Defn at: ")
                    h = s.split("\n")[0]
                    h = h.replace("//","")
                    f.write(h)
                    f.write("\n")
        if isDup == True:
            f.write("//ATTENTION END \n")
            isDup=False;

    for path in xmlFiles:
        p = create_preprocessor_map(path)
        presDict[path]=p
        xmlFile = open(path,'r')
        parseTXLFunctionOutputFile(xmlFile,f,eFile)
        xmlFile.close()

    dumpFns(f,eFile)
    
    f.write("char _license[] SEC(\"license\") = \"GPL\";");
    f.close()
    eFile.close()

    outFile = open(missedFunctionListFile,'w')
    outFile.write("func:fileName,Status\n")
    for fn in fns:
        outFile.write(fn)
        srcFile = fn.split(":")[-1]
        if srcFile.endswith(".h"):
            outFile.write(",SKIPPED")
        else:
            outFile.write(",MISSED")
        outFile.write("\n")
    outFile.close()

    print("MAPS\n")
    for mapName  in opMaps:
        if len(opMaps[mapName]) > 1:
            print("DUPLICATE MAP",mapName)
        else:
            print("MAP",mapName)
        for str in opMaps[mapName]:
            print(str)

    print("PREPROCESSORS")
    for key in presDict:
        print("Name: ",key)
        print("Val: ",presDict[key])

    print("AFTER: FNS LIST")
    print(fns)
    
