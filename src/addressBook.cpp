#include "addressBook.hpp"
#include "myFunctions.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
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
  int titleColumnNumber = -1;
  int addressColumnNumber = -1;
  int postalCodeColumnNumber = -1;
  int pictureColumnNumber = 0;
  int jointSignatureNameColumnNumbers[5] = {0};
  int jointSignatureTitleColumnNumbers[5] = {0};
  std::vector<std::vector<std::string>> strVector;
  strVector = csvTo2dVector(csvPathStr);
  NumberOfPeople = strVector.size() - 1;
  peopleList = std::vector<personalData>(NumberOfPeople, personalData());
  for(int i=0; i<NumberOfPeople+1; i++){
    strVector[i].insert(strVector[i].begin(), "");
  }

  for(int i=0; i < strVector[0].size(); i++){
    if(strVector[0][i] == "氏名" || strVector[0][i] == "名前" || strVector[0][i] == "name"){
      nameColumnNumber = i;
    }else if(strVector[0][i] == "敬称" || strVector[0][i] == "title"){
      titleColumnNumber = i;
    }else if(strVector[0][i] == "住所" || strVector[0][i] == "自宅住所" || strVector[0][i] == "address"){
      addressColumnNumber = i;
    }else if(strVector[0][i] == "郵便番号" || strVector[0][i] == "自宅郵便番号" || strVector[0][i] == "postal code"){
      postalCodeColumnNumber = i;
    }else if(strVector[0][i] == "画像" || strVector[0][i] == "文面" || strVector[0][i] == "裏面" || strVector[0][i] == "picture" || strVector[0][i] == "image"){
      pictureColumnNumber = i;
      isPictureExist = true;
    }else if(strVector[0][i].find("連名") != std::string::npos){
      for(int j=0; j < 5; j++){
        if(strVector[0][i] == "連名"+std::to_string(j+1)){
          jointSignatureNameColumnNumbers[j] = i;
        }
      }
    }else if(strVector[0][i].find("敬称") != std::string::npos){
      for(int j=0; j < 5; j++){
        if(strVector[0][i] == "敬称"+std::to_string(j+1)){
          jointSignatureTitleColumnNumbers[j] = i;
        }
      }
    }
  }
  if(nameColumnNumber == -1){
    outputLog("ERROR", "名前の列がありません。 name column not found.");
    exit(EXIT_FAILURE);
  }
  if(titleColumnNumber == -1){
    outputLog("ERROR", "敬称の列がありません。 title column not found.");
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
  if(pictureColumnNumber == 0){
    outputLog("INFO ","画像(裏面)の列がありませんでした。 picture column not found.");
    isPictureExist = false;
  }

  for(int i=1; i <= NumberOfPeople; i++){
    std::string jointSignatureNames[5];
    std::string jointSignatureTitles[5];
    for(int j=0; j<5; j++){
      jointSignatureNames[j] = strVector[i][jointSignatureNameColumnNumbers[j]];
      jointSignatureTitles[j] = strVector[i][jointSignatureTitleColumnNumbers[j]];
    }
    peopleList.at(i-1).set(strVector[i].at(nameColumnNumber), strVector[i].at(titleColumnNumber), strVector[i].at(addressColumnNumber), strVector[i].at(postalCodeColumnNumber), strVector[i].at(pictureColumnNumber), jointSignatureNames, jointSignatureTitles);
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