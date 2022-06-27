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
