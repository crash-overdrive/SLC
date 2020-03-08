#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("AST built from ParseTree", "[ASTBuilder][!hide]") {
  Lex::Scanner Scanner;
  std::ifstream ScannerStream;
  ScannerStream.open(TokensLexFile);
  ScannerStream >> Scanner;

  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;


  SECTION("J1_forMethodUpdate") {
    std::ifstream FileStream;
    FileStream.open(TestDataDir + "/java/a1/J1_forMethodUpdate.java");
    Scanner.scan(FileStream);
    Parser.parse(Scanner.getTokens());
    Parse::Tree T = Parser.buildTree();

    const Parse::Node &ParseStart =  T.getRoot();
    AST::Start ASTStart;
    AST::dispatch(ParseStart, ASTStart);

    std::ostringstream OStream;
    AST::PrintVisitor Visitor(OStream);
    ASTStart.accept(Visitor);

    std::ifstream ASTStream;
    ASTStream.open(TestDataDir + "/ast/J1_forMethodUpdate.ast");
    std::string ASTString(std::istreambuf_iterator<char>(ASTStream), {});
    REQUIRE(OStream.str() == ASTString);
  }
}
