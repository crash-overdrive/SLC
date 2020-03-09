#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Config.hpp"

void usage() {
  std::cout << "Usage: ./joosc [options] [file-names]" << std::endl;
  std::cout << "options:" << std::endl;
  std::cout << "--output-tokens \t outputs scanned tokens for each class" << std::endl;
  std::cout << "--output-parse \t outputs parse tree for each class" << std::endl;
  std::cout << "--output-ast \t outputs abstract syntax tree for each class" << std::endl;
  std::cout << "--output-fileheader \t outputs file header for each class" << std::endl;
  std::cout << "--std-lib2 \t includes Java standard libarary from Stdlib2Files"
            << std::endl;
  std::cout << "--std-lib3 \t includes Java standard libarary from Stdlib3Files"
            << std::endl;
  std::cout << "--std-lib4 \t includes Java standard libarary from Stdlib4Files"
            << std::endl;
  std::cout << "--std-lib5 \t includes Java standard libarary from Stdlib5Files"
            << std::endl;
}

int main(int argc, char *argv[]) {
  bool outputToken = false;
  bool outputParse = false;
  bool outputAst = false;
  bool outputFileHeader = false;
  // add more flags here for debuging
  std::vector<std::string> files;
  std::set<std::string> finalFiles;

  if (argc < 2) {
    std::cerr << "Error: missing input file" << std::endl;
    usage();
    return 42;
  }

  for (int i = 1; i < argc; ++i) {
    std::string argument(argv[i]);

    if (argument == "--output-tokens") {
      outputToken = true;
    } else if (argument == "--output-parse") {
      outputParse = true;
    } else if (argument == "--output-ast") {
      outputAst = true;
    } else if(argument == "--output-fileheader") {
      outputFileHeader = true;
    } else if (argument == "--std-lib2") {
      for (auto const &file : Stdlib2Files) {
        files.push_back(file);
      }
    } else if (argument == "--std-lib3") {
      for (auto const &file : Stdlib3Files) {
        files.push_back(file);
      }
    } else if (argument == "--std-lib4") {
      for (auto const &file : Stdlib4Files) {
        files.push_back(file);
      }
    } else if (argument == "--std-lib5") {
      for (auto const &file : Stdlib5Files) {
        files.push_back(file);
      }
    } else if (argument[0] != '-') {
      files.push_back(argument);
    } else {
      std::cerr << "Error: Incorrect flag passed" << std::endl;
      usage();
      return 42;
    }
  }

  if (files.size() == 0) {
    std::cerr << "Error: no files provided to compile.." << std::endl;
    return 42;
  }

  for (auto const &file : files) {
    if (finalFiles.find(file) != finalFiles.end()) {
      std::cerr << "Error: " << file << " included multiple times.."
                << std::endl;
      return 42;
    }
    finalFiles.insert(file);
  }

  std::unique_ptr<Lex::Scanner> scanner = std::make_unique<Lex::Scanner>();
  std::unique_ptr<Parse::DFA> parser = std::make_unique<Parse::DFA>();

  std::ifstream scannerStream;
  std::ifstream parserStream;

  scannerStream.open(TokensLexFile);
  if (!scannerStream.is_open()) {
    std::cerr << "Error initialising scanner, " << TokensLexFile
              << " could not be opened" << std::endl;
    return 42;
  }
  parserStream.open(JoosLRFile);
  if (!parserStream.is_open()) {
    std::cerr << "Error initialising parser, " << JoosLRFile
              << " could not be opened" << std::endl;
    return 42;
  }

  scannerStream >> *scanner;
  parserStream >> *parser;

  Client client(std::move(scanner), std::move(parser));
  client.addJavaFiles(std::move(finalFiles));
  client.outputToken = outputToken;
  client.outputParse = outputParse;
  client.outputAst = outputAst;
  client.outputFileHeader = outputFileHeader;

  bool compileResult = client.compile();

  if (!compileResult) {
    std::cerr << "Failure in compilation" << std::endl;
    return 42;
  }
}
