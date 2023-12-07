#include "personalData.hpp"
#include <regex>
#include <filesystem>

personalData::personalData(std::string name, std::string address, std::string postalCode, std::string picturePath){
  set(name, address, postalCode, picturePath);
}

int personalData::set(std::string name, std::string address, std::string postalCode, std::string picturePath){
  setName(name);
  setAddress(address);
  setPostalCode(postalCode);
  setPicturePathStr(picturePath);
  return 0;
}

int personalData::setName(std::string name){
  if(name.empty()){
    name = "";
  }
  this->name = name;
  return 0;
}

int personalData::setAddress(std::string address){
  if(address.empty()){
    address = "";
  }
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

std::string personalData::getName(){
  return name;
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