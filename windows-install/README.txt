CodeQuery
=========

This is a tool to index, then query or search C, C++ and Java source code (and probably also C#).

Support is best in the following order: (best support) C, C++, Java, C# (least support)

It builds upon the databases of cscope and ctags.

The databases of *cscope* and *ctags* would be processed by the *cqmakedb* tool to generate the CodeQuery database file.

The CodeQuery database tool can be viewed and queried in the *codequery* GUI tool.

Website: [CodeQuery website](https://github.com/ruben2020/codequery)


## What are the advantages compared to cscope and ctags?

cscope is more C-centric, but is fuzzy enough to cover C++ and Java, but not very well for e.g. it doesn't understand destructors and class member instantiations. cscope can do "functions that call this functions" and "functions called by this function". But it can't provide relationships of inheritance and membership.

ctags can do many languages well and understands destructors, member instantiations and inheritance, but doesn't do "functions that call this functions" or "functions called by this function". From ctags, we can find out "members and methods of this class", "class which owns this member or method", "parent of this class", "child of this class" etc.

CodeQuery is a project that attempts to combine the best of these, provide faster database access compared to cscope (because it uses sqlite) and provide a nice GUI tool as well.

* Combines the best of both cscope and ctags
* Faster due to the use of sqlite for the CodeQuery database
* Cross-platform GUI tool
* Auto-complete feature
* File viewer with syntax highlighting
* Ability to open in an external editor


## What types of query can I make?

* Symbol
* Function or macro
* Class or struct
* Functions calling this function
* Functions called by this function
* Class which owns this member or method
* Members and methods of this class
* Parent of this class (inheritance)
* Children of this class (inheritance)
* Files including this file
* Full path for file


## What does it cost? How is it licensed?

It's freeware and free open source software.

It's licensed under the [GNU GPL v3](http://www.gnu.org/licenses/gpl.html). See [LICENSE.md](LICENSE.md) or [LICENSE.txt](windows-install/LICENSE.txt).


## Can I use it in a commercial environment without purchasing, for an unlimited time?

Yes. However, donations are welcomed.


## Which platforms are supported?

It has been tested on Ubuntu 32-bit, Windows XP and Windows 7.

Help is welcomed to attempt ports to Mac OS and other operating systems.


## Is the software available in multiple languages?

Yes. This applies only to the GUI tool.

Help is welcomed to update and contribute translations.


## How to install it?

On Linux, please read the [INSTALL-LINUX](doc/INSTALL-LINUX.md) file. At present, the only way is to build the code.

On Windows, an EXE setup package will be provided here:
[CodeQuery@sourceforge downloads](https://sourceforge.net/projects/codequery/files/)


## How do I use it?

Please read the HOWTO file provided for each platform. The workflow looks like this:
![CodeQuery workflow](doc/workflow.png)

On Linux: [HOWTO-LINUX](doc/HOWTO-LINUX.md)

On Windows: [HOWTO-WINDOWS](windows-install/HOWTO-WINDOWS.txt)


## How do I contact the author for support, issues, bug reports, fix patches etc.?

Please see the email address below, and also the Issues tab in Github.

Email address: ![Contact address](doc/emailaddr.png)


## List of Contributors

ruben2020


## What does it look like?

![CodeQuery screenshot](doc/screenshot.png)




