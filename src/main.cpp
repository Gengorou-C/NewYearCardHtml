#include <iostream>
#include <string>
#include "myFunctions.hpp"
#include "personalData.hpp"
#include "addressBook.hpp"

int main(int argc, char** argv){
  if(argc == 1){
    outputLog("Error", "住所録のCSVをください input address book.");
    exit(EXIT_FAILURE);
  }
  outputLog(" log ", "target csv name \"" + std::string(argv[1]) + "\"");

  class addressBook book = addressBook(argv[1]);
  generateHtml(book);
  return 0;
}