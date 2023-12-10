#include "addressBook.hpp"
#include "myFunctions.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

addressBook::addressBook(char* csvPath){
  csvPathStr = csvPath;
  loadCsv(csvPath);
  makePictureList();
}

std::string addressBook::getCsvPathStr(){
  return csvPathStr;
}

std::vector<personalData> addressBook::getPeopleList(){
  return peopleList;
};

std::map<std::string, std::string> addressBook::getPictureList(){
  return pictureList;
};

bool addressBook::getIsPictureExist(){
  return isPictureExist;
}

int addressBook::loadCsv(char* csvPathStr){
  int nameColumnNumber = -1;
  int addressColumnNumber = -1;
  int postalCodeColumnNumber = -1;
  int pictureColumnNumber = -1;
  std::vector<std::vector<std::string>> strVector;
  strVector = csvTo2dVector(csvPathStr);
  NumberOfPeople = strVector.size() - 1;
  peopleList.reserve(NumberOfPeople);

  for(int i=0; i < strVector[0].size(); i++){
    if(strVector[0][i] == "氏名" || strVector[0][i] == "名前" || strVector[0][i] == "name"){
      nameColumnNumber = i;
    }else if(strVector[0][i] == "住所" || strVector[0][i] == "自宅住所" || strVector[0][i] == "address"){
      addressColumnNumber = i;
    }else if(strVector[0][i] == "郵便番号" || strVector[0][i] == "自宅郵便番号" || strVector[0][i] == "postal code"){
      postalCodeColumnNumber = i;
    }else if(strVector[0][i] == "画像" || strVector[0][i] == "文面" || strVector[0][i] == "裏面" || strVector[0][i] == "picture" || strVector[0][i] == "image"){
      pictureColumnNumber = i;
      isPictureExist = true;
    }
  }
  if(nameColumnNumber == -1){
    outputLog("ERROR", "名前の列がありません。 name column not found.");
    exit(EXIT_FAILURE);
  }
  if(addressColumnNumber == -1){
    outputLog("ERROR", "住所の列がありません。 address column not found.");
    exit(EXIT_FAILURE);
  }
  if(postalCodeColumnNumber == -1){
    outputLog("ERROR","郵便番号の列がありません。 postal code column not found.");
    exit(EXIT_FAILURE);
  }
  if(pictureColumnNumber == -1){
    outputLog("INFO ","画像(裏面)の列がありませんでした。 picture column not found.");
    isPictureExist = false;
  }

  if(isPictureExist){
    for(int i=1; i <= NumberOfPeople; i++){
      this->peopleList.push_back(personalData(strVector[i].at(nameColumnNumber), strVector[i].at(addressColumnNumber), strVector[i].at(postalCodeColumnNumber), strVector[i].at(pictureColumnNumber)));
    }
  }else{
    for(int i=1; i <= NumberOfPeople; i++){
      this->peopleList.push_back(personalData(strVector[i].at(nameColumnNumber), strVector[i].at(addressColumnNumber), strVector[i].at(postalCodeColumnNumber), ""));
    }
  }
  outputLog("INFO ", "住所録の読み込み完了 csv loading complete");
  return 0;
}

int addressBook::makePictureList(){
  if(!isPictureExist) return 0;
  for(int i=1; i < NumberOfPeople; i++){
    if(pictureList.count(peopleList[i].getPicturePathStr()) == 0 && peopleList[i].getPicturePathStr() != ""){
      pictureList[peopleList[i].getPicturePathStr()] = pathToBase64(peopleList[i].getPicturePathStr());
    }
  }
  return 0;
}