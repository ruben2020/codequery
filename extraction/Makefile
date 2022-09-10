obj := .
src := .


DEBUGBPF = -DDEBUG
#DEBUGFLAGS = -O0 -g -Wall
DEBUGFLAGS = -O2 -g -Wall
PFLAGS = $(DEBUGFLAGS)

INCLUDEFLAGS = -I$(obj)/usr/include \
	       -I$(obj)/include \



always = extracted.o

HOSTCFLAGS += $(INCLUDEFLAGS) $(PFLAGS)
HOSTCFLAGS_bpf_load.o += $(INCLUDEFLAGS) $(PFLAGS) -Wno-unused-variable


# Allows pointing LLC/CLANG to a LLVM backend with bpf support, redefine on cmdline:
#  make samples/bpf/ LLC=~/git/llvm/build/bin/llc CLANG=~/git/llvm/build/bin/clang
LLC ?= llc
CLANG ?= clang

# Trick to allow make to be run from this directory
all: $(always)
	$(MAKE) -C .. $$PWD/

clean:
	$(MAKE) -C .. M=$$PWD clean
	@rm -f *~

$(obj)/%.o: $(src)/%.c
	$(CLANG) $(INCLUDEFLAGS) $(EXTRA_CFLAGS) \
	$(DEBUGBPF) -D__KERNEL__ -Wno-unused-value -Wno-pointer-sign \
		-Wno-compare-distinct-pointer-types \
		-O2 -emit-llvm -c -g $< -o -| $(LLC) -march=bpf -filetype=obj -o $@
