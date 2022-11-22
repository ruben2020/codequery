CLANG=/home/sayandes/katran/_build//deps/clang/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang 
LLC=/home/sayandes/katran/_build//deps/clang/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/llc
INLCUDE_SYS=./usr/include
INCLUDE_LOC=./include
XDP_TARGET=extracted

$CLANG -I$INLCUDE_SYS -I$INCLUDE_LOC   \
	-DDEBUG -D__KERNEL__ -Wno-unused-value -Wno-pointer-sign \
        -Wno-compare-distinct-pointer-types \
	-O2 -emit-llvm -c -g $XDP_TARGET.c -o -| $LLC -march=bpf -filetype=obj -o $XDP_TARGET.o
