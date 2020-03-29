#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include <fstream>

const std::unordered_map<unsigned int, std::vector<std::string>> stdLibMap{
    {1, stdlib2Files}, {2, stdlib2Files}, {3, stdlib3Files},
    {4, stdlib4Files}, {5, stdlib5Files},
};

Client createClient() {
  std::unique_ptr<Lex::Scanner> scanner = std::make_unique<Lex::Scanner>();
  std::ifstream scannerStream;
  scannerStream.open(tokensLexFile);
  scannerStream >> *scanner;

  std::unique_ptr<Parse::DFA> parser = std::make_unique<Parse::DFA>();
  std::ifstream parserStream;
  parserStream.open(joosLRFile);
  parserStream >> *parser;

  return Client(std::move(scanner), std::move(parser));
}

std::vector<std::string>
createMarmosetTest(unsigned int num,
                   const std::vector<std::string> &fileNames) {
  std::vector<std::string> files;
  for (const auto &fileName : fileNames) {
    files.emplace_back(testDataDir + "/java/a" + std::to_string(num) + "/" +
                       fileName);
  }
  auto it = stdLibMap.find(num);
  files.insert(files.end(), it->second.begin(), it->second.end());
  return files;
}
