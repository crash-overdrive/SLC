#include "Client.hpp"
#include "Config.hpp"
#include <fstream>

static bool hasExtension(const std::string &FullString,
                      const std::string &Ext) {
  if (FullString.length() < Ext.length()) {
    return false;
  }
  size_t Position = FullString.find(".");
  return FullString.compare(Position, Ext.size(), Ext) == 0;
}

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
  if (InputToken && OutputParse) {
      return Client::parse(Parser, IStream, std::cerr) ? 0 : 42;
  }

  std::string extension(".java");
  if (!hasExtension(FileName, extension)) {
    std::cerr << "error: expect " << FileName << " to end with .java"
              << std::endl;
    return 42;
  }
  if (OutputToken) {
    return Client::scan(IStream, std::cerr) ? 0 : 42;
  } else {
    return Client::preProcess(IStream) ? 0 : 42;
  }
}
