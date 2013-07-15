

# Automatically web-translate ts files.
#
# This license applies only to this file:
#
# Copyright (c) 2013 ruben2020
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#

use strict;
use HTTP::Request::Common qw(POST);  
use LWP::UserAgent; 

my @langfiles;
open(FIL, "./tslist.txt");
while(<FIL>)
{
	chomp;
	if (/\{[ \t]*\"(.+)\"[ \t]*,[ \t]*\"(.+)\"[ \t]*\}/)
	{
		push @langfiles, $2;
	}
}
close(FIL);

foreach(@langfiles)
{
	system("lupdate -no-obsolete ../*.cpp ../ui/*.ui -ts ./$_.ts");
}

my $langout;
my $langfil;
foreach(@langfiles)
{
	$langfil = "./$_.ts";
	if ($_ =~ /codequery\_([A-Za-z0-9-]+)/) {$langout = $1;}
	if ($langout eq 'en') {next;}
	print "Web translating $langfil $langout ...\n";
	&translatedoc($langfil, $langout);
}


#############################################
sub translatedoc
{
my ($fn, $lang) = @_;
my $ua = LWP::UserAgent->new();  
my $req = POST 'http://maille.tk/translate/index-curl.php',
              Content_Type => 'multipart/form-data',
              Content => [ 
             "bingkey" => "689AC7C9ABE8CF4C53B930FE1A0019F73C0C8AB5",
             "languagein" => "en",
             "languageout" => $lang,
             "output" => "pofile",
             "pofile" => [$fn]
]; 
my $resp = $ua->request($req);
my @respxml;
if ($resp->is_success)
{
	open(FILO, ">$fn");
	@respxml = split /\n/, $resp->content;
	$respxml[0] =~ s/^[ \t]*<\?xml/<?xml/;
	print FILO join("\n", @respxml);
	close(FILO);
}
else {print $resp->as_string;}

}


