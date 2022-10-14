import re
import os

# parses output from c-extract-function.txl
def parseTXLFunctionOutputFileForComments(inputFile):
    srcSeen=False
    lines = []
    for line in inputFile.readlines():
        ending = re.match(r"</source",line)
        if ending:
            srcSeen = False;
            #dump to file
            #print(lines)
            lines = []
            continue;
        if srcSeen:
            lines.append(line)
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
            #print("args ",funcArgs)
   
            
            srcFile = tokens[-4]
            srcFile = srcFile.replace(" ","")

            funcName = tokens[-3].replace(" (","(")
            output= funcName.split('(')[-2].split(" ")[-2]
            #print("funcName: ",funcName)
            funcName = funcName.split('(')[-2].split(" ")[-1]

            startLine = int(tokens[-2])
            endLine = int(tokens[-1])
            print("File: ",srcFile, " startline: ",startLine," endline: ",endLine," funcname: ",funcName, "Input: (", funcArgs, ") Output: ",output)
            #key=funcName+":"+srcFile+":"+str(startLine)
            key=funcName+":"+srcFile
            #print("Checking if need to extract",key)

if __name__ =="__main__":

    TXLDir = "./txl_annotate"
    xmlFiles = []
    
    #Parse TXL annotated files
    for fName in os.listdir(TXLDir):
        #print("annotatedFile: ",fName)
        path = TXLDir + "/" + fName
        if fName.endswith(".xml"):
            ##print("annotatedXmlFile: ",fName)
            xmlFiles.append(path)
    for path in xmlFiles:
        xmlFile = open(path,'r')
        parseTXLFunctionOutputFileForComments(xmlFile)
        xmlFile.close()

    '''
    ifile = open("./txl_annotate/annotate_func_test_decap_kern.c.xml",'r')
    parseTXLFunctionOutputFileForComments(ifile)
    ifile.close()
    '''
