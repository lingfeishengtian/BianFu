lexer grammar BFLexer;

// These are all supported lexer sections:

// Lexer file header. Appears at the top of h + cpp files. Use e.g. for copyrights.
@lexer::header {/* lexer header section */}

// Appears before any #include in h + cpp files.
@lexer::preinclude {/* lexer precinclude section */}

// Follows directly after the standard #includes in h + cpp files.
@lexer::postinclude {
/* lexer postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Directly preceds the lexer class declaration in the h file (e.g. for additional types etc.).
@lexer::context {/* lexer context section */}

// Appears in the public part of the lexer in the h file.
@lexer::members {/* public lexer declarations section */
bool canTestFoo() { return true; }
bool isItFoo() { return true; }
bool isItBar() { return true; }
void debugMessage() { std::cout << "tst" << std::endl; }

void myFooLexerAction() { /* do something*/ };
void myBarLexerAction() { /* do something*/ };
}

// Appears in the private part of the lexer in the h file.
@lexer::declarations {/* private lexer declarations/members section */}

// Appears in line with the other class member definitions in the cpp file.
@lexer::definitions {/* lexer definitions section */}

channels { CommentsChannel, DirectiveChannel }

tokens {
	DUMMY
}

Comment : '#' ~[\r\n]* '\r'? '\n' -> channel(CommentsChannel);
WS: [ \t\r\n]+ -> channel(99);

Return: 'return';
Continue: 'continue';

// Default functions
FPrint: '出';

// Keywords!
KInt: '整数';
KDecimal: '小数';
KChar: '字';
KVar: '变量';

KVoid: '无';

KPublic: '公';
KPrivate: '母';

KClass: '类';

LessThan: '<';
GreaterThan:  '>';
Equal: '=';
And: '&&';

Colon: ':';
Semicolon: ';';
Plus: '+';
Minus: '-';
Star: '*';
Divide: '/';
OpenPar: '(';
ClosePar: ')';
OpenCurly: '{';
CloseCurly: '}';
QuestionMark: '?';
Comma: ',';
Dollar: '$';
Ampersand: '&' -> type(DUMMY);

INT: DIGIT+;
FLOAT: INT Dot INT;
ID: LETTER (LETTER | '0'..'9')*;

String: '"' .*? '"';
Char: '\'' .? '\'';

fragment Baz: 'Baz';
fragment LETTER : [a-zA-Z\u0080-\u{10FFFF}];
fragment DIGIT : [0-9];

Dot: '.';