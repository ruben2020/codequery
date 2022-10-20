#!/bin/bash
##TODO: Move to python

echo "Cleaning old files ..."
rm cscope.files cscope.out tags myproject.db 
bash setup_interfaces.sh
echo "Setting interfaces for testing ..."
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

if [ -d "$copdir" ]; then rm -Rf $copdir; fi
mkdir $copdir


echo "Read cscope files and generate function annotation ..."
while read -r line; do
	
    echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g' | sed -e 's/^__//g'
    x=$(echo "$line"  | sed -e 's/^\./\//g' | sed -e 's/\//_/g'| sed -e 's/^__//g')
    echo "${x}"
    
	opfile_function_annotate="$opdir/annotate_func_$x.xml"
	commented_opfile_function_annotate="$copdir/commented_OPENED_$x"
	logfile="$opdir/LOG"
	full_line=`readlink -f $line`
	echo "File to annotate - $full_line ouput in $opfile_function_annotate $opfile_struct_annotate"
	`txl -o $opfile_function_annotate $full_line c-extract-functions.txl`
	status=$?
	if [ $status -eq 0 ] 
	then
		echo "Successful function annotation for file: $full_line" >> $logfile
		echo "Generating comments " >> $logfile
		
		`python3 code-commentor.py -o $commented_opfile_function_annotate -i $opfile_function_annotate -f ./helper_hookpoint_map.json -s $full_line`
		st=$?
		if [ $st -eq 0 ] 
		then
			echo "Successful function comment generation for file: $full_line" >> $logfile
		else
			echo "Failed function cwcomment generation for file: $full_line" >> $logfile
		fi
	else
		echo "Failed function annotation for file: $full_line" >> $logfile
	fi
done < "cscope.files"

echo "Running code commentator on the first_pass txl annotated files"

