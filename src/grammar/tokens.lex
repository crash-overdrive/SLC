#README:
#Regular patterns are expressed in such a way that they are unix compatible
#When in doubt, try to grep with these patterns via -P
#to be separated with the -=- sign, this is unique enough that java token patterns likely wouldnt include them

#regexr.com -> really good resource for regex validation

#UNICODE -=- \u[a-fA-F0-9]{4}

LINE_TERMINATOR -=- [\r\n]|(\r\n)

#\s is regex's treatment of regular expression
#Not very helpful, admittedly, so this should match space, \t, \r and \n.

WHITESPACE -=- [ \t\r\n]

COMMENT -=- (\/\/[^\n]*)|\/\*[^*]*\*([^\/][^*]*\*)*\/

ABSTRACT -=- abstract

BOOLEAN -=- boolean

BREAK -=- break

BYTE -=- byte

CASE -=- case

CATCH -=- catch

CHAR -=- char

CLASS -=- class

CONST -=- const

CONTINUE -=- continue

DEFAULT -=- default

DO -=- do

DOUBLE -=- double

ELSE -=- else

EXTENDS -=- extends

FINAL -=- final

FINALLY -=- finally

FLOAT -=- float

FOR -=- for

GOTO -=- goto

IF -=- if

IMPLEMENTS -=- implements

IMPORT -=- import

INSTANCEOF -=- instanceof

INT -=- int

INTERFACE -=- interface

LONG -=- long

NATIVE -=- native

NEW -=- new

PACKAGE -=- package

PRIVATE -=- private

PROTECTED -=- protected

PUBLIC -=- public

RETURN -=- return

SHORT -=- short

STATIC -=- static

STRICTFP -=- strictfp

SUPER -=- super

SWITCH -=- switch

SYNCHRONIZED -=- synchronized

THIS -=- this

THROW -=- throw

THROWS -=- throws

TRANSIENT -=- transient

TRY -=- try

VOID -=- void

VOLATILE -=- volatile

WHILE -=- while

OCT_INT_LITERAL -=- 0[0-9]+

DEC_INT_LITERAL -=- 0|[1-9][0-9]*

HEX_INT_LITERAL -=- 0x[0-9a-fA-F]+

BOOLEAN_LITERAL -=- (true)|(false)

CHAR_LITERAL -=- '([^'\\\r\n]|(\\[nNrR\\'tTbBfF"])|(\\([0-3]?[0-7][0-7]?)))'

STRING_LITERAL -=- "([^"\\\r\n]|\\[btnfrBTNFR"'\\]|(\\[0-3]?[0-7][0-7]?))*"

NULL_LITERAL -=- null

LPAREN -=- \(

RPAREN -=- \)

LBRKT -=- \[

RBRKT -=- \]

LBRAC -=- {

RBRAC -=- }

SEMI -=- ;

COMMA -=- ,

DOT -=- \.

ASSIGN -=- =

GREATER -=- >

LESSER -=- <

EXCLAIMATION -=- !

#Note: Bitwise compliment
TILDA -=- ~

QUESTION -=- \?

COLON -=- :

EQUAL -=- ==

LESSER_OR_EQUAL -=- <=

GREATER_OR_EQUAL -=- >=

NOT_EQUAL -=- !=

AND -=- &&

OR -=- \|\|

INCREMENT -=- \+\+

DECREMENT -=- --

ADDITION -=- \+

SUBTRACTION -=- -

MULTIPLICATION -=- \*

DIVISION -=- /

BITWISEAND -=- &

BITWISEOR -=- \|

BITWISEXOR -=- \^

MODULUS -=- %

LEFT_SHIFT -=- <<

RIGHT_SHIFT -=- >>

UNSIGNED_RIGHT_SHIFT -=- >>>

PLUS_EQUALS -=- \+=

MINUS_EQUALS -=- -=

MULTIPLY_EQUALS -=- \*=

DIVISION_EQUALS -=- /=

BITWISEAND_EQUALS -=- &=

BITWISEOR_EQUALS -=- \|=

BITWISEXOR_EQUALS -=- \^=

MODULUS_EQUALS -=- %=

LEFTSHIFT_EQUALS -=- <<=

RIGHTSHIFT_EQUALS -=- >>=

UNSIGNED_RIGHTSHIFT_EQUALS -=- >>>=

IDENTIFIER -=- [a-zA-Z_$][a-zA-Z0-9_$]*
