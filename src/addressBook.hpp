#ifndef ADDRESSBOOK_HPP
#define ADDRESSBOOK_HPP

#include <vector>
#include <map>

#include "personalData.hpp"

class addressBook{
  private:
    std::string csvPathStr;
    std::vector<personalData> peopleList;
    int NumberOfPeople;
    std::map<std::string, std::string> pictureList;
    bool isPictureExist;
    int loadCsv(char* path);
    int makePictureList();

  public:
    addressBook(char* path);

    std::string getCsvPathStr();
    std::vector<personalData> getPeopleList();
    std::map<std::string, std::string> getPictureList();
    bool getIsPictureExist();
};

#endif