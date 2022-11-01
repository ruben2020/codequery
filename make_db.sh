##TODO: Move to python

echo "Cleaning old files ..."
rm cscope.files cscope.out tags myproject.db 

function_name=${1}
codedir=${1:-'katran'}
opdir_default="txl_annotate_$codedir"
copdir_default="txl_annotate_commented_$codedir"
opdir=${2:-$opdir_default}
copdir=${3:-$copdir_default}


echo "Creating new file index ..."
find -L $codedir -name '*.c' >cscope.files
find -L $codedir -name '*.h' >>cscope.files
echo "Running cscope & ctags ..."
cscope -cb -k 
ctags --fields=+i -n -L ./cscope.files
echo "Making DBs ..."
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p

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

#Command to generate function call graph 
echo "Running cqsearch for $function_name and outputting dependencies to func.out"
status=`cqsearch -s ./myproject.db -t $function_name  -p 7  -l 100 -k 10 -e > func.out`
status=$?
if [ $status -eq 0 ]
then
	echo "Successful cqsearch for function: $function_name" >> $logfile
else
	echo "Failed cqsearch for function: $function_name" >> $logfile
fi

echo "Success"
echo "DELETE DUPLICATES in func.out for code extraction step to proceed!!"

