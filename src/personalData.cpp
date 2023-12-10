#include "personalData.hpp"
#include "myFunctions.hpp"
#include <iostream>
#include <regex>
#include <filesystem>

personalData::personalData(){
    name.reserve(20);
    name = "";
    title.reserve(16);
    title = "";
    address.reserve(150);
    address = "";
    postalCode.reserve(16);
    postalCode = "";
    picturePathStr.reserve(200);
    picturePathStr = "";
    for(int i=0; i<5; i++){
      jointSignatureNames[i].reserve(20);
      jointSignatureNames[i] = "";
      jointSignatureTitles[i].reserve(16);
      jointSignatureTitles[i] = "";
    }
}

personalData::personalData(std::string name, std::string title, std::string address, std::string postalCode, std::string picturePath, std::string* jointSignatureNames, std::string* jointSignatureTitles){
  set(name, title, address, postalCode, picturePath, jointSignatureNames, jointSignatureTitles);
}

int personalData::set(std::string name, std::string title, std::string address, std::string postalCode, std::string picturePath, std::string* jointSignatureNames, std::string* jointSignatureTitles){
  setName(name);
  setTitle(title);
  setAddress(address);
  setPostalCode(postalCode);
  setPicturePathStr(picturePath);
  for(int i=0; i < 5; i++){
    setJointSignature(i, jointSignatureNames[i], jointSignatureTitles[i]);
  }
  return 0;
}

int personalData::setName(std::string name){
  if(name.empty()){
    name = "";
  }
  name = std::regex_replace(name, std::regex(R"(　)"), " ");
  this->name = name;
  return 0;
}

int personalData::setTitle(std::string title){
  if(title.empty()){
    outputLog("WARN ", name+"さんの敬称がありません。 title not found.");
    title = "　";
  }
  this->title = title;
  return 0;
}

int personalData::setAddress(std::string address){
  if(address.empty()){
    address = "";
  }
  address = std::regex_replace(address, std::regex(R"(-|−)"), "‐");
  this->address = address;
  return 0;
}

int personalData::setPostalCode(std::string postalCode){
  if(postalCode.empty()){
    postalCode = "";
  }
  this->postalCode = std::regex_replace(postalCode, std::regex(R"(\D)"), "");
  return 0;
}

int personalData::setPicturePathStr(std::string picturePathStr){
  if(picturePathStr.empty()){
    picturePathStr = "";
  }else{
    picturePathStr = std::filesystem::absolute(std::filesystem::u8path(picturePathStr)).string();
  }
  this->picturePathStr = picturePathStr;
  return 0;
}

int personalData::setJointSignature(int i, std::string firstName, std::string title){
  if(i < 0 || 5 <= i){
    return 1;
  }
  if(!firstName.empty() && title == ""){
    outputLog("WARN ", firstName+"さんの敬称がありません。 title not found.");
    title = "　";
  }
  jointSignatureNames[i] = firstName;
  jointSignatureTitles[i] = title;
  return 0;
}

std::string personalData::getName(){
  return name;
}
std::string personalData::getFamilyName(){
  return name.substr(0, name.find(" "));
}
std::string personalData::getFirstName(){
  return name.erase(0, name.rfind(" ")+1);
}
std::string personalData::getTitle(){
  return title;
}
std::string personalData::getAddress(){
  return address;
}
std::string personalData::getPostalCode(){
  return postalCode;
}

std::string personalData::getPicturePathStr(){
  return picturePathStr;
}
std::string personalData::getJointSignatureName(int i){
  return jointSignatureNames[i];
}
std::string personalData::getJointSignatureTitle(int i){
  return jointSignatureTitles[i];
}