##TODO: Move to python

echo "Cleaning old files ..."
rm cscope.files cscope.out tags myproject.db 

echo "Creating new file index ..."
find -L $1 -name '*.c' >cscope.files
find -L $1 -name '*.h' >>cscope.files
echo "Running cscope & ctags ..."
cscope -cb -k 
ctags --fields=+i -n -L ./cscope.files
echo "Making DBs ..."
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p

#opdir="txl_annotate"
opdir=$2
if [ -d "$opdir" ]; then rm -Rf $opdir; fi
mkdir $opdir

echo "Read cscope files and generate function annotation ..."
while read -r line; do
	
    echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g' | sed -e 's/^__//g'
    x=$(echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g'| sed -e 's/^__//g')
    echo "${x}"
    
	opfile_function_annotate="$opdir/annotate_func_$x.xml"
	opfile_struct_annotate="$opdir/annotate_struct_$x.out"
	logfile="$opdir/LOG"
	full_line=`readlink -f $line`
	echo "File to annotate - $full_line ouput in $opfile_function_annotate $opfile_struct_annotate"
	`txl -o $opfile_function_annotate $full_line c-extract-functions.txl`
	status=$?
	if [ $status -eq 0 ] 
	then
		echo "Successful function annotation for file: $full_line" >> $logfile
	else
		echo "Failed function annotation for file: $full_line" >> $logfile
	fi
	`txl -o $opfile_struct_annotate $full_line c-extract-struct.txl`
	status=$?
	if [ $status -eq 0 ] 
	then
		echo "Successful struct annotation for file: $full_line" >> $logfile
	else
		echo "Failed struct annotation for file: $full_line" >> $logfile
	fi

done < "cscope.files"

#Command to generate function call graph cqsearch -s ./myproject.db -t process_packet  -p 7  -l 100 -k 10 -e > a1
