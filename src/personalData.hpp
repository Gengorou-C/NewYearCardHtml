#ifndef PERSONALDATA_HPP
#define PERSONALDATA_HPP

#include <string>

class personalData{
  private:
    std::string name;
    std::string address;
    std::string postalCode;
    std::string picturePathStr;

  public:
    personalData(std::string name, std::string address, std::string postalCode, std::string picturePathStr);

    int set(std::string name, std::string address, std::string postalCode, std::string picturePathStr);
    int setName(std::string name);
    int setAddress(std::string address);
    int setPostalCode(std::string postalCode);
    int setPicturePathStr(std::string picturePathStr);
    std::string getName();
    std::string getAddress();
    std::string getPostalCode();
    std::string getPicturePathStr();
};

#endif