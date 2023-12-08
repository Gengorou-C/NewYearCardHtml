#include <iostream>
#include <string>
#include <filesystem>
#include "myFunctions.hpp"
#include "personalData.hpp"
#include "addressBook.hpp"

int main(int argc, char** argv){
  outputLog("INFO ", "NewYearCardHtml started.");
  if(argc == 1){
    outputLog("ERROR", "住所録のCSVをください input address book.");
    exit(EXIT_FAILURE);
  }
  outputLog("INFO ", "target csv name \"" + std::filesystem::path(argv[1]).u8string() + "\"");

  class addressBook book = addressBook(argv[1]);
  generateHtml(book);

  outputLog("INFO ", "Complete.");
  return 0;
}