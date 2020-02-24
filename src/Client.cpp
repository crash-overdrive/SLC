#include "Client.hpp"

bool Client::preProcess(std::istream &IStream, const std::string &FileName) {
  (void)IStream;
  (void)FileName;
  return true;
}

bool Client::verifyFileName(const std::string &FileName) {
  const std::string Ext(".java");
  if (FileName.length() < Ext.length()) {
    return false;
  }
  size_t Position = FileName.find(".");
  return FileName.compare(Position, Ext.size(), Ext) == 0;
}

bool Client::scan(std::istream &IStream) {
  (void)IStream;
  return true;
}

bool Client::scan(std::istream &IStream, std::ostream &OStream) {
  (void)IStream;
  (void)OStream;
  return true;
}
