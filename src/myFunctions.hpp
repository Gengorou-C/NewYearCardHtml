#ifndef MYFUNCTIONS_HPP
#define MYFUNCTIONS_HPP

#include <string>
#include "addressBook.hpp"

std::string getDatetimeStr();

int outputLog(std::string level, std::string content);

std::string pathToBase64(std::string path);

int searchSeparatorPosition(std::string obj);

std::vector<std::vector<std::string>> csvTo2dVector(char* csvPathStr);

std::string pathToBase64(std::string pathStr);

int generateHtml(addressBook book);

#endif