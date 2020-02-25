#include "Client.hpp"
#include "Config.hpp"
#include <fstream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: missing input file" << std::endl;
    return 1;
  }
  bool OutputToken(false);
  bool OutputParse(false);
  bool InputToken(false);
  std::string FileName;
  for (int i = 1; i < argc; ++i) {
    std::string Arg(argv[i]);
    if (Arg == "--input-tokens") {
      InputToken = true;
    } else if (Arg == "--output-tokens") {
      OutputToken = true;
    } else if (Arg == "--output-parse") {
      OutputParse = true;
    } else if (FileName.empty() && Arg[0] != '-') {
      FileName = Arg;
    } else {
      std::cerr << "error: unrecognize flag: " << Arg << std::endl;
      return 1;
    }
  }
  if (OutputToken && OutputParse) {
    std::cerr << "error: unable to dump token and parse at same time"
              << std::endl;
    return 1;
  }
  if (FileName.empty()) {
    std::cerr << "error: missing fileName" << std::endl;
    return 1;
  }
  std::ifstream IStream;
  IStream.open(FileName);
  if (IStream.fail()) {
    std::cerr << "error: file " << FileName << " missing" << std::endl;
    return 1;
  }

  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  if (InputToken && OutputToken) {
    std::cout << IStream.rdbuf();
    return 0;
  }
  if (InputToken) {
    std::cerr << "Parsing tokens";
    return Parser.parse(IStream) ? 0 : 42;
  }

  if (OutputToken) {
  } else {
    return Client::preProcess(IStream, FileName) ? 0 : 42;
  }
}
