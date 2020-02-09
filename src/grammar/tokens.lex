LINE_TERMINATOR -=- [\r\n]|(\r\n)
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
OCT_INT_LITERAL -=- 0[0-9]*
DEC_INT_LITERAL -=- 0|[1-9][0-9]*
HEX_INT_LITERAL -=- 0x[0-9a-fA-F]+
BOOLEANLITERAL -=- (true)|(false)
CHAR_LITERAL -=- '[^'\\\r\n]|(\\[nr\\'tbf"])'
STRING_LITERAL -=- "([^"\\\r\n]|\\[btnfr"'\\])*"
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
TILDA -=- ~
QUESTION -=- \?
COLON -=- :
EQUAL -=- ==
LESSER_OR_EQUAL -=- <=
GREATER_OR_EQUAL -=- >=
NOT_EQUAL -=- !=
AND -=- &&
OR -=- \|\|
INCREMENT -=- ++
DECREMENT -=- --
ADDITION -=- +
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
PLUS_EQUALS -=- +=
MINUS_EQUALS -=- -=
MULTIPLY_EQUALS -=- *=
DIVISION_EQUALS -=- /=
BITWISEAND_EQUALS -=- &=
BITWISEOR_EQUALS -=- \|=
BITWISEXOR_EQUALS -=- \^=
MODULUS_EQUALS -=- %=
LEFTSHIFT_EQUALS -=- <<=
RIGHTSHIFT_EQUALS -=- >>=
UNSIGNED_RIGHTSHIFT_EQUALS -=- >>>=
IDENTIFIER -=- [a-zA-Z_$][a-zA-Z0-9_$]*
