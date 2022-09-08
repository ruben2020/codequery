# string to search in file

def create_preprocessor_map(filename):
        fp = open(filename, 'r')
        # read all lines using readline()
        lc = 1
        lines = fp.readlines()

        stack = []
        pres=[]

        for line in lines:
            # check if string present on a current line
            ifdefstring = '#ifdef'
            elsestring = '#else'
            endifstring = '#endif'
            elifstring = '#elif'
            ifndefstring = '#ifndef'
            # find() method returns -1 if the value is not found,
            # if found it return 0

            if line.find(ifdefstring) == 0:
                print('string #ifdef exists in file')
                print('line Number:', lc,line)
                tokens = line.split()
                name = tokens[1]
                stack.append((ifdefstring,name,lc))

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

        return pres

# checks if function/struct in filename between st_line and end_line needs to be guarded with IFDEF MACROS
def contained_in_preprocessor(pres, st_line, end_line):
        print("checking containment")
        for (t,defName,start,end) in pres:
                if st_line >= start and end_line <= end:
                        return (t,defName,start,end)
        return (None,None,None,None)
if __name__ == "__main__":
        out=create_preprocessor_map('/home/sayandes/codequery/test/balancer_kern.c')
        print(out)
        val = None
        (val,name,start,end) = contained_in_preprocessor(out,233,256)
        if val:
            print(val,name,start,end)
        (val,name,start,end) = contained_in_preprocessor(out,24,50)
        if val:
            print(val,name,start,end)
