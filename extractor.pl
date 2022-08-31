#!/usr/bin/perl
#use warnings;
#use strict;
use File::Path;
use File::Copy;
use File::Compare;

my $ip_cscope_files = 'cscope.files';
my $ip_filename = 'x';
my $op_filename = 'x.c';
my $op_dir_name = "test";
my $main_fn_lic_str = "/* SPDX-License-Identifier: GPL-2.0 */";
my $main_fn_stub_str = "
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC(\"xdp\")
int  xdp_prog_simple(struct xdp_md *ctx)
{
  return xdpdecap(ctx);
//return XDP_PASS;
}

char _license[] SEC(\"license\") = \"GPL\";
";

my %files_hash = (); 
open(FH, '<', $ip_filename) or die $!;

while($line = <FH>){
	if($line !~ "{" && $line !~ "}"){
		@tokens = split(/,/,$line);
		$files_hash{$tokens[1]} = 1;
	}
}
close(FH);

#create a folder
rmtree $op_dir_name;
if(-e $op_dir_name){
	die "Directory '$dir' still exists";
}
else{
	print "Directory '$dir' deleted.\n";
}

mkdir $op_dir_name or die "error creating directory: $op_dir_name";
open(OFH, '>', "$op_dir_name/$op_filename") or die $!;
print OFH "$main_fn_lic_str\n"; 

#copy cscope.h files to op_dir_name
my $include_str = "";
open(IFH, '<', $ip_cscope_files) or die $!;
while($line = <IFH>){
    chomp $line;
    #print "Processing: $line \n";
	#print "Header: $line\n";
	if($line !~ /bpf/ && $line !~ /linux_includes/){
		next;
	}
       if(-f "$line") {
	    @f_tokens = split(/\//,$line);
	    $fname = $f_tokens[$#f_tokens];
	    $opf = "$op_dir_name/$fname";
	    print "Copying $line to $opf\n";
	    copy "$line", $opf;
	    if($line =~ /\.c/){
		    $include_str = "$include_str #include \"$fname\"\n";
	    }elsif($line =~ /\.h/){
		    $include_str = "#include \"$fname\"\n$include_str";
	    }else{
		    print "ignoring $fname\n";
	    }

	} else{
	    die "cannot find $line\n"
	}
} 
print OFH $include_str;
print OFH $main_fn_stub_str;
close(IFH);
=begin comment
#copy code files into the folder
#make a c file with stub code
foreach my $key (keys(%files_hash)) {
	if(-f "$key" ) {
		#Check with -f only for files (no directories)
		@f_tokens = split(/\//,$key);
		$fname = $f_tokens[$#f_tokens];
		$opf = "$op_dir_name/$fname";
		print "Copying $key to $opf\n";
		copy "$key", $opf;
		if(compare($key, $opf) != 0){
		       die "Files do not match";
	       }else{
		       print "Successful\n";
	       }
		print OFH "#include \"$fname\"\n";
	}else{
		die "cannot find $key\n";
	}
}
=end comment
#print OFH $main_fn_stub_str;
close(OFH);


