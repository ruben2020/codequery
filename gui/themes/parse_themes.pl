#!/usr/bin/perl

# Process theme XML files, to generate C++ code
#
# This license applies only to this file:
#
# Copyright (c) 2014 ruben2020
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

use XML::LibXML;

my $debug = 0;

my $themelist;
my $numOfThemes = 0;
my $lexstyle;

my $cppstyle;
my $javastyle;
my $pythonstyle;
my $rubystyle;

my $langname;
my $langdesc;
my $themename;
my $themename2;
my @themes;
my $defaultbgcolor;
my $defaultfgcolor;
my $stylename;
my $styleid;
my $fgcolor;
my $bgcolor;
my $fontstyle;

my %acceptedlang =
(
	'C++' => 'cpp',
	'Java' => 'java',
	'Python' => 'python',
	'Ruby' => 'ruby'
);

my $filename;
my $parser;
my $xmldoc;

$themelist .= "static const char* themelist[] = {\n";

$cppstyle .= "static const langstyle cppstyle[] = {\n";
$javastyle .= "static const langstyle javastyle[] = {\n";
$pythonstyle .= "static const langstyle pythonstyle[] = {\n";
$rubystyle .= "static const langstyle rubystyle[] = {\n";

opendir(DIR1, '.');
while($filename = readdir(DIR1))
{
	$themename = '';
	if ((-f $filename )&&($filename =~ /\.xml$/i))
	{
		$themename = $filename;
		$themename =~ s/\.xml$//i;
		$themename2 = $themename;
		$themename2 =~ s/_/ /g;
		print "\n ==> $themename\n" if ($debug);
		push @themes, $themename2;
		processfile();
		$numOfThemes++;
	}
}
close(DIR1);
@themes = sort {lc $a cmp lc $b} @themes;
foreach my $them(@themes)
{
	$themelist .= "\t\"$them\",\n";
}
$themelist .= "};\n\n";


$cppstyle    .= "\t{ NULL, NULL, NULL, 0 }\n};\n\n";
$javastyle   .= "\t{ NULL, NULL, NULL, 0 }\n};\n\n";
$pythonstyle .= "\t{ NULL, NULL, NULL, 0 }\n};\n\n";
$rubystyle   .= "\t{ NULL, NULL, NULL, 0 }\n};\n\n";
open(FILO,">themes_gen.cpp");
print FILO "\n /* THIS FILE WAS AUTO_GENERATED USING parse_themes.pl */";
print FILO "\n /* DO NOT CHANGE BY HAND                              */\n\n";
print FILO "#define NUM_OF_THEMES   $numOfThemes\n\n";
print FILO $themelist;
print FILO $lexstyle."\n";
print FILO $cppstyle;
print FILO $javastyle;
print FILO $pythonstyle;
print FILO $rubystyle;
close(FILO);

################################################
sub processfile
{

$parser = XML::LibXML->new();
$xmldoc = $parser->parse_file($filename);
my $lstyle;
my $numofstyles=0;

foreach my $sample ( $xmldoc->findnodes('/NotepadPlus/LexerStyles/LexerType') )
{
	$langname = '';
	$langdesc = '';
	$langname = $sample->getAttribute('name') if ($sample->hasAttribute('name'));
	$langdesc = $sample->getAttribute('desc') if ($sample->hasAttribute('desc'));
	next if (!(defined($acceptedlang{$langdesc})));

	print "\n$langname, $langdesc\n" if ($debug);
	$lexstyle .= "\nstatic const lexstyle ".$langname."_".$themename."[] = {\n";
	$defaultbgcolor = '';
	$defaultfgcolor = '';
	$numofstyles = 0;
	foreach my $child ( $sample->getChildnodes )
	{
		if (( $child->nodeType() == XML_ELEMENT_NODE )&&($child->nodeName() eq 'WordsStyle'))
		{
			$stylename = '';
			$styleid = '';
			$fgcolor = '';
			$bgcolor = '';
			$fontstyle = '';
			$stylename = $child->getAttribute('name') if ($child->hasAttribute('name'));
			$styleid = $child->getAttribute('styleID') if ($child->hasAttribute('styleID'));
			$fgcolor = $child->getAttribute('fgColor') if ($child->hasAttribute('fgColor'));
			$bgcolor = $child->getAttribute('bgColor') if ($child->hasAttribute('bgColor'));
			$fontstyle = $child->getAttribute('fontStyle') if ($child->hasAttribute('fontStyle'));
			$defaultbgcolor = $bgcolor if ($stylename eq 'DEFAULT');
			$defaultfgcolor = $fgcolor if ($stylename eq 'DEFAULT');
			print "$stylename, $styleid, $fgcolor, $bgcolor, $fontstyle\n" if ($debug);
			$lexstyle .= "\t{ $styleid, \"$fgcolor\", \"$bgcolor\", $fontstyle }, // $stylename\n";
			$numofstyles++;
        	}
	}
	$lexstyle .= "};\n";
	$lstyle = "	{ \"$themename2\", \"$defaultfgcolor\", \"$defaultbgcolor\", ".$langname."_".$themename.", $numofstyles },\n";
	$cppstyle    .= $lstyle if ($langdesc eq 'C++');
	$javastyle   .= $lstyle if ($langdesc eq 'Java');
	$pythonstyle .= $lstyle if ($langdesc eq 'Python');
	$rubystyle   .= $lstyle if ($langdesc eq 'Ruby');
}

}

