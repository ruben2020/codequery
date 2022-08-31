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

opdir="txl_annotate"
mkdir $opdir
echo "Read cscope files and generate function annotation ..."
while read -r line; do
    echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g' | sed -e 's/^__//g'
    x=$(echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g'| sed -e 's/^__//g')
    echo "${x}"
    
	opfile="$opdir/$x.xml"
	logfile="$opdir/LOG"
	echo "File to annotate - $line ouput in $opfile"
	txl -o $opfile $line c-extract-functions.txl
	status=$?
	if [ $status -eq 0 ] 
	then
		echo "Successful annotation for file: $line" >> $logfile
	else
		echo "Failed annotation for file: $line" >> $logfile
	fi
done < "cscope.files"

#Command to generate function call graph cqsearch -s ./myproject.db -t process_packet  -p 7  -l 100 -k 10 -e > a1
