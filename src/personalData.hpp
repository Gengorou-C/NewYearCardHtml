#ifndef PERSONALDATA_HPP
#define PERSONALDATA_HPP

#include <string>
#include <vector>
#include <map>

class personalData{
  private:
    std::string name;
    std::string title;
    std::string address;
    std::string postalCode;
    std::string picturePathStr;
    std::string jointSignatureNames[5];
    std::string jointSignatureTitles[5];

  public:
    personalData();
    personalData(std::string name, std::string title, std::string address, std::string postalCode, std::string picturePathStr, std::string* jointSignatureNames, std::string* jointSignatureTitles);

    int set(std::string name, std::string title, std::string address, std::string postalCode, std::string picturePathStr, std::string* jointSignatureNames, std::string* jointSignatureTitles);
    int setName(std::string name);
    int setTitle(std::string title);
    int setAddress(std::string address);
    int setPostalCode(std::string postalCode);
    int setPicturePathStr(std::string picturePathStr);
    int setJointSignature(int i, std::string firstName, std::string title);
    std::string getName();
    std::string getFamilyName();
    std::string getFirstName();
    std::string getTitle();
    std::string getAddress();
    std::string getPostalCode();
    std::string getPicturePathStr();
    std::string getJointSignatureName(int i);
    std::string getJointSignatureTitle(int i);
};

#endif