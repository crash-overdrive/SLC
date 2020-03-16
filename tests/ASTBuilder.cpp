#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("AST built from ParseTree", "[ASTBuilder]") {
  Lex::Scanner scanner;
  std::ifstream scannerStream;
  scannerStream.open(tokensLexFile);
  scannerStream >> scanner;

  Parse::DFA parser;
  std::ifstream parserStream;
  parserStream.open(joosLRFile);
  parserStream >> parser;

  SECTION("J1_forMethodUpdate") {
    std::ifstream fileStream;
    fileStream.open(testDataDir + "/java/a1/J1_forMethodUpdate.java");
    scanner.scan(fileStream);
    parser.parse(scanner.getTokens());
    Parse::Tree t = parser.buildTree();

    const Parse::Node &parseStart = t.getRoot();
    AST::Start astStart;
    AST::dispatch(parseStart, astStart);

    std::ostringstream oStream;
    AST::TrackVisitor visitor;
    visitor.setLog(oStream);
    astStart.accept(visitor);

    std::ifstream astStream;
    astStream.open(testDataDir + "/ast/J1_forMethodUpdate.ast");
    std::string astString(std::istreambuf_iterator<char>(astStream), {});
    REQUIRE(oStream.str() == astString);
  }
}
