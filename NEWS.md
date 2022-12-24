Changelog
=========

Version 0.26.0 - Dec 19, 2022
-----------------------------
Bugfix: File path label with too large contents extending window beyond viewport GUI bug fixed          
Enhancement: Default external editor configuration for MacOS added

Version 0.25.0 - Apr 17, 2022
-----------------------------
Enhancement: Workaround for pycscope trailer start offset bug running on Python 3.8         
Enhancement: Updated Scintilla to 3.21.1        
Enhancement: Removed warnings for Qt 5.15         
Enhancement: Windows installer now includes Qt 5.15.2 and sqlite 3.38.2 binaries       
Enhancement: Moved from Travis CI to Github Actions (for Linux, MacOS and Windows builds)        

Version 0.24.0 - Jan 17, 2021
-----------------------------
Enhancement: Improved German translation for CodeQuery GUI, contributed by stweise       

Version 0.23.0 - Apr 15, 2020
-----------------------------
The CMake build directory is now moveable and renameable, which is useful for Debian source package creation      

Version 0.22.0 - Apr 12, 2020
-----------------------------
Enhancement: Added build option for no GUI      
Enhancement: Updated Scintilla to 3.11.2      
Enhancement: Performance improvement by using QString instead of STL String for search through GUI      
Enhancement: Added ability to general call function and class inheritance graph through cqsearch CLI      
Enhancement: Added ability to filter by path added to cqsearch CLI      
Enhancement: Changing query type on the GUI dropbox will start a search immediately      
Enhancement: Clicking on a line in the GUI file viewer, will now highlight the appropriate function in the function list      
Enhancement: Now Qt5 becomes the default build option in CMakeLists.txt     
Enhancement: Windows installer changed from InnoSetup to Wix Toolset, to generate MSI installer     
Enhancement: Now Windows installer will be in two versions: Qt5, 64-bit or Qt4, 32-bit     
Bugfix: Default font selection for GUI file viewer, instead of becoming empty     
Bugfix: Improved annotation accuracy in GUI     
Bugfix: Improved brace highlighting in GUI     
Bugfix: Fixed the bug where function list next to GUI file viewer doesn't work properly for grep     

Version 0.21.1 - Sept 16, 2018
------------------------------
Fixed a build error due to new version of Qt5 (5.11)    

Version 0.21.0 - Apr 30, 2017
-----------------------------
Fixes in the ctags file parser     
"List of files" button added to the GUI     
Improved "full path for file" results list in GUI      

Version 0.20.0 - Apr 9, 2017
----------------------------
Major bug fix for a performance issue in the GUI, wrt clearing of the search results list    
Other minor bug fixes    

Version 0.19.0 - Apr 2, 2017
----------------------------
Migrated from GPLv2 to MPLv2 because of incompatibility with Qt5's license    
Now officially supporting Qt5 for Linux and Mac, but Qt4 for Windows    
Replaced QScintilla (by Riverbank Computing) with Scintilla (by Neil Hodgson), to avoid GPLv3    
Unicode UTF-8 files now viewable on CodeQuery GUI    
Source code preview text line length now unlimited in database    
CodeQuery CLI now has feature to change the source code preview text line truncation length    
Golang syntax highlighting added    
Travis script now also builds with clang and Qt5 for Mac build testing    
Fixed a build bug in CMake scripts for Arch Linux    
Debian and tarball packages would now be separated into Qt5 and Qt4    

Version 0.18.0 - Oct 9, 2016
----------------------------
Migrated from GPLv3 to GPLv2 because some companies ban GPLv3 software    
List of functions in the file viewer area is a major new feature    
Now the selection of one or two levels has been added for function call graphs    
Replaced Dipperstein's optlist with Sittler's getopt replacement (MIT licensed), to avoid LGPLv3    
Updated showgraph (now BSD licensed), with rebase from the git repo    
Various bug fixes for the file viewer    

Version 0.17.0 - Sept 25, 2016
------------------------------
Now Javascript is officially supported through starscope    
The full file path option is added to the command-line version of the tool for vim support    
HOWTO documentation updated for compatibility with Mac OS    
CodeQuery now officially built using Qt5 for Mac OS, but still in Qt4 for Windows and Linux    
Travis script updated to include Coverity scanning    

Version 0.16.0 - May 2, 2015
----------------------------
Brace matching on the file viewer    
Better handling for current line highlight    
Annotation in file viewer to quickly and easily identify the selected item    
Changed version to the semantic versioning scheme MAJOR.MINOR.PATCH    
Qt4 (default) and Qt5 now supported by CMake build option    

Version 0.15 - Aug 12, 2014
---------------------------
Added support for Mac OS    
Allowed -q in cscope.out header    
Added info to HOWTOs on how to exclude standard include paths    

Version 0.14 - Mar 9, 2014
--------------------------
Fixed Java and Ruby syntax highlight bug    
Fixed bug where database file combobox becomes too large due to very long filename    

Version 0.13 - Mar 1, 2014
--------------------------
Grep feature added    
Last search history stored persistently    

Version 0.12 - Feb 23, 2014
---------------------------
QScintilla used for code editing widget    
Imported a number of syntax highlight themes from Notepad++    
Ruby and Go now officially supported through starscope    
RPM packages will now be generated also    

Version 0.11 - Dec 31, 2013
---------------------------
File path filter    
New query type: Calls of this function    
Search results traversing within same file    
Improvements on graph's initial size and zoom    
Bug fix on external editor settings text    

Version 0.10 - Aug 10, 2013
---------------------------
ctags file processing optimization    
New GUI features: Header files only, Functions inside this file, new About box    
New GUI features: Ability to change file viewer font and tab space width    
Autocomplete made asynchronous and optimized    

Version 0.09 - Apr 20, 2013
---------------------------
Fixed some cscope.out parser and SQL handling bugs    
Added font resize buttons for file viewer    

Version 0.08 - Apr 14, 2013
---------------------------
Fixed Windows app icon and taskbar bugs    

Version 0.07 - Apr 13, 2013
---------------------------
Added ability to parse cscope.out generated by pycscope (for Python support)    
Added syntax highlighting for Python and Java    
File path can now be selected and copied in the GUI tool    

Version 0.06 - Feb 27, 2013
---------------------------
Fixed syntax highlight bug, added user warning if CQ database is older than viewed source file,    
removed the extra console window appearing alongside the GUI app in Windows    

Version 0.05 - Feb 25, 2013
---------------------------
Fixed file path bug in Windows and initial version of cqsearch CLI tool    

Version 0.04 - Feb 24, 2013
---------------------------
Initial version of visualization for function call graph and class inheritance is now working.    

Version 0.03 - Feb 22, 2013
---------------------------
Added wildcard search * and ?. Added previous and next history browsing for search.    

Version 0.02 - Feb 19, 2013
---------------------------
Fixed external editor file path recognition bug for Windows    
(could not recognize spaces e.g. C:\Program Files).    

Version 0.01 - Feb 18, 2013
---------------------------
Initial release    

