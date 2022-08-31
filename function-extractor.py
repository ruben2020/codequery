from lxml import etree
import re

def dump_to_file(f,appt):
    str = "//" + appt.get('file')+ " " + appt.get('startline')+ " "+ appt.get('endline') + " \n"
    f.write(str)
    f.write(appt.text)

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
        print(appt.get('startline'))
        print(appt.get('endline'))

def parseFunctionList(ifile):
    for line in ifile.readlines():
        print(line)
        ct = 0
        m = re.match(r"[{}]",line)
        if m:
            print("Ignoring",line)
            ct = ct + 1
        else:
            if ct >= 2:
                return
            #print("Processing", line)
            tokens = line.split(',')
            fnName = tokens[0]
            count = tokens[1]

            src = tokens[2]
            #remove leading [
            src = src[1:]

            startLine = tokens[3]
            #remove end ]
            startLine = startLine[:-1]

            print(fnName,count,src,startLine)
            
if __name__ == "__main__":
    ifile = open('func.out','r')
    parseFunctionList(ifile)
    ifile.close()
    f = open('extracted.c','w')
    #parseXML("op.xml",f)
    f.close()
