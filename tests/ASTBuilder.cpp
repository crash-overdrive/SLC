#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("AST built from ParseTree", "[ASTBuilder][!hide]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  SECTION("J1_finalclass.tokens") {
    TokenStream.open(TestDataDir + "/tokens/J1_finalclass.tokens");
    Client::parse(Parser, TokenStream);
    Parse::Tree T = Parser.buildTree();

    const Parse::Node &ParseStart =  T.getRoot();
    AST::Start ASTStart;
    AST::dispatch(ASTStart, ParseStart);

    std::ostringstream OStream;
    AST::PrintVisitor Visitor(OStream);
    ASTStart.accept(Visitor);

    std::ifstream ASTStream;
    ASTStream.open(TestDataDir + "/ast/J1_finalclass.ast");
    std::string ASTString(std::istreambuf_iterator<char>(ASTStream), {});
    REQUIRE(OStream.str() == ASTString);
  }
}
