#include "Client.hpp"
#include "Config.hpp"
#include <array>
#include <fstream>
#include <set>
#include <string>
#include <vector>

const int errorCode = 42;

const std::vector<std::array<std::string, 2>> usageArray{
    {"--output-tokens", "outputs scanned tokens for each class"},
    {"--output-parse", "outputs parse tree for each class"},
    {"--output-ast", "outputs abstract syntax tree for each class"},
    {"--output-joostype", "outputs joos type for each class"},
    {"--output-localvariable",
     "outputs local variable analysis for each class"},
    {"--output-codegen", "outputs code generation for each class"},
    {"--std-lib2", "includes Java standard libarary from Stdlib2Files"},
    {"--std-lib3", "includes Java standard libarary from Stdlib3Files"},
    {"--std-lib4", "includes Java standard libarary from Stdlib4Files"},
    {"--std-lib5", "includes Java standard libarary from Stdlib5Files"},
};

const std::unordered_map<std::string, Client::BreakPointType> outputMap{
    {"--output-tokens", Client::BreakPointType::Scan},
    {"--output-parse", Client::BreakPointType::Parse},
    {"--output-ast", Client::BreakPointType::Ast},
    {"--output-joostype", Client::BreakPointType::TypeDeclaration},
    {"--output-localvariable", Client::BreakPointType::LocalVariable},
    {"--output-codegen", Client::BreakPointType::CodeGen},
};

const std::unordered_map<std::string, std::vector<std::string>> stdlibMap{
    {"--std-lib2", stdlib2Files},
    {"--std-lib3", stdlib3Files},
    {"--std-lib4", stdlib4Files},
    {"--std-lib5", stdlib5Files},
};

void usage() {
  std::cout << "Usage: ./joosc [options] [file-names]\n"
            << "options:\n";
  for (const auto &usage : usageArray) {
    std::cout << usage[0] << '\t' << usage[1] << '\n';
  }
}

int main(int argc, char *argv[]) {
  std::unique_ptr<Lex::Scanner> scanner = std::make_unique<Lex::Scanner>();
  std::unique_ptr<Parse::DFA> parser = std::make_unique<Parse::DFA>();

  std::ifstream scannerStream;
  std::ifstream parserStream;

  scannerStream.open(tokensLexFile);
  if (!scannerStream.is_open()) {
    std::cerr << "Error initialising scanner, " << tokensLexFile
              << " could not be opened" << std::endl;
    return errorCode;
  }
  parserStream.open(joosLRFile);
  if (!parserStream.is_open()) {
    std::cerr << "Error initialising parser, " << joosLRFile
              << " could not be opened" << std::endl;
    return errorCode;
  }

  scannerStream >> *scanner;
  parserStream >> *parser;

  Client client(std::move(scanner), std::move(parser));

  if (argc < 2) {
    std::cerr << "Error: missing input file\n";
    usage();
    return errorCode;
  }

  std::vector<std::string> files;
  for (int i = 1; i < argc; ++i) {
    std::string argument(argv[i]);

    auto outputIt = outputMap.find(argument);
    if (outputIt != outputMap.end()) {
      client.addPrintPoint(outputIt->second);
      continue;
    }
    auto stdlibIt = stdlibMap.find(argument);
    if (stdlibIt != stdlibMap.end()) {
      files.insert(files.end(), stdlibIt->second.begin(),
                   stdlibIt->second.end());
      continue;
    }
    if (argument.at(0) != '-') {
      files.emplace_back(std::move(argument));
      continue;
    }
    std::cerr << "Error: Incorrect flag passed" << std::endl;
    usage();
    return errorCode;
  }

  if (files.size() == 0) {
    std::cerr << "Error: no files provided to compile.." << std::endl;
    return errorCode;
  }

  bool compileResult = client.compile(files);

  if (!compileResult) {
    std::cerr << "Failure in compilation" << std::endl;
    return errorCode;
  }
}
