#include <iostream>
#include <string>
#include <filesystem>
#include "myFunctions.hpp"
#include "personalData.hpp"
#include "addressBook.hpp"

int main(int argc, char** argv){
  if(argc == 1){
    outputLog("Error", "住所録のCSVをください input address book.");
    exit(EXIT_FAILURE);
  }
  outputLog(" log ", "target csv name \"" + std::filesystem::path(argv[1]).u8string() + "\"");

  class addressBook book = addressBook(argv[1]);
  generateHtml(book);

  outputLog(" log ", "Complete.\n");
  return 0;
}