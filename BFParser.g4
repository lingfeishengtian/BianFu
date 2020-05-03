parser grammar BFParser;

options {
	tokenVocab = BFLexer;
}

// These are all supported parser sections:

// Parser file header. Appears at the top in all parser related files. Use e.g. for copyrights.
@parser::header {/* parser/listener/visitor header section */}

// Appears before any #include in h + cpp files.
@parser::preinclude {/* parser precinclude section */}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Directly preceeds the parser class declaration in the h file (e.g. for additional types etc.).
@parser::context {/* parser context section */}

// Appears in the private part of the parser in the h file.
// The function bodies could also appear in the definitions section, but I want to maximize
// Java compatibility, so we can also create a Java parser from this grammar.
// Still, some tweaking is necessary after the Java file generation (e.g. bool -> boolean).
@parser::members {
/* public parser declarations/members section */
bool myAction() { return true; }
bool doesItBlend() { return true; }
void cleanUp() {}
void doInit() {}
void doAfter() {}
}

// Appears in the public part of the parser in the h file.
@parser::declarations {/* private parser declarations section */}

// Appears in line with the other class member definitions in the cpp file.
@parser::definitions {/* parser definitions section */}

// Additionally there are similar sections for (base)listener and (base)visitor files.
@parser::listenerpreinclude {/* listener preinclude section */}
@parser::listenerpostinclude {/* listener postinclude section */}
@parser::listenerdeclarations {/* listener public declarations/members section */}
@parser::listenermembers {/* listener private declarations/members section */}
@parser::listenerdefinitions {/* listener definitions section */}

@parser::baselistenerpreinclude {/* base listener preinclude section */}
@parser::baselistenerpostinclude {/* base listener postinclude section */}
@parser::baselistenerdeclarations {/* base listener public declarations/members section */}
@parser::baselistenermembers {/* base listener private declarations/members section */}
@parser::baselistenerdefinitions {/* base listener definitions section */}

@parser::visitorpreinclude {/* visitor preinclude section */}
@parser::visitorpostinclude {/* visitor postinclude section */}
@parser::visitordeclarations {/* visitor public declarations/members section */}
@parser::visitormembers {/* visitor private declarations/members section */}
@parser::visitordefinitions {/* visitor definitions section */}

@parser::basevisitorpreinclude {/* base visitor preinclude section */}
@parser::basevisitorpostinclude {/* base visitor postinclude section */}
@parser::basevisitordeclarations {/* base visitor public declarations/members section */}
@parser::basevisitormembers {/* base visitor private declarations/members section */}
@parser::basevisitordefinitions {/* base visitor definitions section */}

// Actual grammar start.
main: stat* EOF;

// Unused rule to demonstrate some of the special features.
unused[double input = 111] returns [double calculated] locals [int _a, double _b, int _c] @init{ doInit(); } @after { doAfter(); } :
	stat
;
catch [...] {
  // Replaces the standard exception handling.
}
finally {
  cleanUp();
}

unused2:
	(unused[1] .)+ (Colon | Semicolon | Plus)? ~Semicolon
;

classStat: assignment
    | functionDeclaration
;

stat: expr
    | assignment
    | classDeclaration
    | flowControl
;

//TODO: FIX SPECIFYING INT OR CLASS NAME
assignment: (KVar | ID) expr Equal expr
    | expr Equal expr
;

expr: expr op=(Star | Divide) expr
    | expr op=(Plus | Minus) expr
    | expr Dot ID OpenPar ClosePar //TODO: ADD PARAMETER LIST SUPPORT LATER
    | OpenPar expr ClosePar
    | id = ID
    | <assoc = right> expr QuestionMark expr Colon expr
    | typeDef
    | array
    | defaultFunctions
;

defaultFunctions: FPrint OpenPar expr ClosePar
    | FPrint OpenPar ClosePar
;

classDeclaration: KClass id=ID OpenCurly classStat* CloseCurly;

//TODO: ADD CONSTRUCTORS
functionDeclaration: id=ID OpenPar el += functionParameter (Comma el += functionParameter)* ClosePar block
    | id=ID OpenPar ClosePar block
    | (typeDef | ID | KVoid) functionDeclaration
    | (KPublic | KPrivate) functionDeclaration
;

block: OpenCurly stat+ CloseCurly;

// SUPPORT no type, just ID: Defaults to any object type.
//TODO: Change TYPE to KType because you're searching for keywords!
functionParameter: (typeDef | ID) ID | ID;

operatorDeclaration: ; // WORK IN PROGRESS!!!!

flowControl:
	Return expr # Return
	| Return # Return
	| Continue # Continue
;

typeKeywords: ;

typeDef : FLOAT
    | INT
    | String
    | Char
    | ID OpenPar ClosePar //TODO: Add parameter list!
;
array : OpenCurly el += expr (Comma el += expr)* CloseCurly
    | OpenCurly CloseCurly;
any: t = .;