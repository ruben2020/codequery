% Null transform - format output according to grammar
include "c.grm.1"

% Ignore byte order marks on source files
include "C18/bom.grm"

% Uncomment this line to approximately parse and preserve comments
% include "C18/c-comments.grm"
% TODO: uncommenting is leading to issues with multiline '//' comments, so disabled

define begin_marker
 [NL] '<[SPOFF] 'struct '>[SPON] [NL]
end define

define end_marker
 '<[SPOFF] '/struct '>[SPON] [NL]
end define


redefine function_definition_or_declaration
	[function_definition]
    |	[opt begin_marker] [declaration] [opt end_marker]
#ifdef PREPROCESSOR
    |	[preprocessor]
#endif
    |	[block] [NL]	% sometimes, function headers are ifdefed out
#ifdef GNU
    |	[macro_declaration_or_statement]
#endif
    |	[unknown_declaration_or_statement]
end redefine


rule replaceStruct0
	replace [function_definition_or_declaration]
	  T [struct_or_union_specifier] S [semi] 

	construct BEG [begin_marker]
	 '< 'struct  '>

	construct END [end_marker]
	 '<  '/struct '>

	by
	   BEG  T S END
end rule

rule replaceStruct1
	replace [function_definition_or_declaration]
	  T [struct_or_union_specifier] P [reference_id] S [semi] 

	construct BEG [begin_marker]
	 '< 'struct '>

	construct END [end_marker]
	 '< '/struct '>

	by
	   BEG  T P S END
end rule

rule replaceStruct2
	replace [function_definition_or_declaration]
	  T [struct_or_union_specifier] P [reference_id] U [decl_qualifier_or_type_specifier] S [semi]  

	construct BEG [begin_marker]
	 '< 'struct '>

	construct END [end_marker]
	 '< '/struct '>

	by
	   BEG T P U S END 
end rule

rule replaceStruct3
	replace [function_definition_or_declaration]
	  T [struct_or_union_specifier]  M [macro_specifier] P [init_declarator]  S [semi]  

	construct BEG [begin_marker]
	 '< 'struct '>

	construct END [end_marker]
	 '< '/struct '>

	by
	 BEG  T  M   P  S END
end rule

function main 
	replace [program] 
		P  [program]
	by
	 	P [replaceStruct0]
		  [replaceStruct1]
		  [replaceStruct2]
		  [replaceStruct3]
end function
