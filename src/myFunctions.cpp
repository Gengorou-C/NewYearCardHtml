#include "myFunctions.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <regex>
#include <filesystem>

std::string getDateAndTimeStr() {
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << localTime->tm_year + 1900;
    // setw(),setfill()で0詰め
    s << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
    s << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    s << "T";
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
    s << std::setw(2) << std::setfill('0') << localTime->tm_min;
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    // std::stringにして値を返す
    return s.str();
}

int outputLog(std::string level, std::string content){
  std::ofstream log;
  log.open("./NewYearCardHtml.log", std::ios_base::app);
  std::cout <<  "[" << getDateAndTimeStr() << "] [" << level << "]" << content << '\n';
  log << "[" << getDateAndTimeStr() << "] [" << level << "]" << content << '\n'; 
  log.close();
  return 0;
}

int searchSeparatorPosition(std::string obj){
  if(obj[0]=='"'){
    int currentPosition=0;
    int separatorPosition=0;
    while(currentPosition < obj.length()){
        int counter=0;
        separatorPosition = obj.find_first_of(",\n\r", currentPosition);
        currentPosition=separatorPosition+1;
        if(separatorPosition == std::string::npos)return obj.length();
        for(int j=separatorPosition-1; 0 < j; j--){
            if(obj[j] == '"'){
                counter++;
            }else{
                break;
            }
        }
        if(counter%2 == 1)break;
    }
    return separatorPosition;
  }else{
    int separatorPosition=0;
    separatorPosition = obj.find_first_of(",\n\r");
    if(separatorPosition == std::string::npos)return obj.length();
    return separatorPosition;
  }
}

std::vector<std::vector<std::string>> csvTo2dVector(char* csvPathStr){
  int peopleCounter = 0;
  int separatorPosition;
  std::string strBuffer;
  std::string value;
  std::vector<std::vector<std::string>> result(1, std::vector<std::string>());

  std::ifstream ifs(csvPathStr);
  if(!ifs){
    outputLog("Error", "csvファイルの読み込みに失敗 could not load csv file");
    exit(EXIT_FAILURE);
  }
  std::getline(ifs, strBuffer, '\0');
  while(strBuffer != ""){
    separatorPosition = searchSeparatorPosition(strBuffer);
    value = strBuffer.substr(0, separatorPosition);
    if (value[0] == '"' && value[value.length() - 1] == '"'){
      value = value.substr(1, value.length() - 2);
      value = std::regex_replace(value, std::regex(R"("")"), R"(")");
    }
    result[peopleCounter].push_back(value);
    if(separatorPosition == strBuffer.length()){
      strBuffer = strBuffer.substr(separatorPosition);
    }else if(strBuffer[separatorPosition] == '\n'){
      peopleCounter++;
      result.push_back(std::vector<std::string>());
      strBuffer = strBuffer.substr(separatorPosition+1);
    }else if(strBuffer.substr(separatorPosition, 2) == "\r\n"){
      peopleCounter++;
      result.push_back(std::vector<std::string>());
      strBuffer = strBuffer.substr(separatorPosition+2);
    }else{
      strBuffer = strBuffer.substr(separatorPosition+1);
    }
    if(peopleCounter > 2147483646){
      outputLog("Error", "無限ループかも。 maybe infinite loop.");
      break;
    }
  }
  if(result[result.size()-1].size() == 0){
    result.pop_back();
  }
  return result;
}

std::string pathToBase64(std::string pathStr){
  std::filesystem::path path(pathStr);
  path.make_preferred();
  std::ifstream ifs(path, std::ios::in | std::ios::binary);
  if(!ifs){
    outputLog("Error", "画像の読み込みに失敗 could not load picture");
    outputLog("Error", "->target picture \"" + path.string() +'\"');
    exit(EXIT_FAILURE);
  }
  size_t fileSize = std::filesystem::file_size(path);
  char correspondenceTable[64] = {
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9',
    '+','/'
  };
  unsigned char buf[3];
  int intBuf;
  std::string result;

  for(size_t i=0; i < fileSize/3; i++){
    ifs.read((char*)buf, 3);
    intBuf = buf[0]*0x10000 + buf[1]*0x100 + buf[2];
    result.push_back(correspondenceTable[(intBuf >> 18) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >> 12) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >>  6) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >>  0) & 0b111111]);
  }

  int remainder = fileSize % 3;
  if(remainder == 1){
    ifs.read((char*)buf, remainder);
    intBuf = buf[0]*0b10000;
    result.push_back(correspondenceTable[(intBuf >> 6) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >> 0) & 0b111111]);
  }
  if(remainder == 2){
    ifs.read((char*)buf, remainder);
    intBuf = buf[0]*0b10000000000 + buf[1]*0b100;
    result.push_back(correspondenceTable[(intBuf >> 12) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >>  6) & 0b111111]);
    result.push_back(correspondenceTable[(intBuf >>  0) & 0b111111]);
  }

  while(result.length() % 4 != 0){
    result.push_back('=');
  }

  return result;
}

int generateHtml(addressBook book){
  std::ifstream css("./cssTemplate.txt");
  std::ifstream html("./htmlTemplate.txt");
  std::string outputDir = "./" + std::filesystem::path(book.getCsvPathStr()).stem().string() + "/" + getDateAndTimeStr();
  std::string outputFile = std::filesystem::path(book.getCsvPathStr()).stem().string() + ".html";
  std::filesystem::create_directories(outputDir);
  std::ofstream output(outputDir + "/" + outputFile);
  if(!css){
    outputLog("Error", "cssのテンプレートファイルの読み込みに失敗 could not load cssTemplate.txt");
    exit(EXIT_FAILURE);
  }
  if(!html){
    outputLog("Error", "htmlのテンプレートファイルの読み込みに失敗 could not load htmlTemplate.txt");
    exit(EXIT_FAILURE);
  }
  std::string cssTemplate;
  std::string imageTemplate;
  std::string htmlTemplate;
  std::getline(css, cssTemplate, '\0');
  std::getline(html, htmlTemplate, '\0');

  imageTemplate = R"(
.pic_{クラス名}{
  background-image: url(data:image/{画像形式};base64,{画像データ});
  background-size: 100% 100%;
}
)";

  std::string generatedHtml = R"(<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8" />
    <title></title>
    <style></style>
  </head>
  <body>
)";

  for(int i=1; i < book.getList().size(); i++){
    std::cout << "\r" << "宛名面 : " << std::to_string(i) + '/' + std::to_string(book.getList().size()-1) << "\r";
    std::string bufferStr = htmlTemplate;

    bufferStr = std::regex_replace(bufferStr, std::regex("\\{宛先氏名\\}"), book.getList()[i].getName());
    bufferStr = std::regex_replace(bufferStr, std::regex("\\{宛先住所\\}"), book.getList()[i].getAddress());
    for(int j=1; j < 8; j++){
      bufferStr = std::regex_replace(bufferStr, std::regex("\\{宛先郵便番号"+std::to_string(j)+"\\}"), book.getList()[i].getPostalCode().substr(j-1,1));
    }

    bufferStr = std::regex_replace(bufferStr, std::regex("\\{差出人氏名\\}"), book.getList()[0].getName());
    bufferStr = std::regex_replace(bufferStr, std::regex("\\{差出人住所\\}"), book.getList()[0].getAddress());
    for(int j=1; j < 8; j++){
      bufferStr = std::regex_replace(bufferStr, std::regex("\\{差出人郵便番号"+std::to_string(j)+"\\}"), book.getList()[0].getPostalCode().substr(j-1,1));
    }

    if(book.getList()[i].getPicturePathStr() != ""){
      std::string className;
      className = std::filesystem::path(book.getList()[i].getPicturePathStr()).stem().string();
      className = std::regex_replace(className, std::regex(" "), "_");
      bufferStr = std::regex_replace(bufferStr, std::regex("class=\"back\""), "class=\"back " + className + "\"");
    }
    generatedHtml = generatedHtml + bufferStr;
  }
  std::cout << "\r" << "宛名面 : " << std::to_string(book.getList().size()-1) + '/' + std::to_string(book.getList().size()-1) << "\n";
  
  int PictureProcessIndex = 1;
  for (const auto& [key, value] : book.getPictureList()) {
    std::cout << "\r" << "画像処理 : " << std::to_string(PictureProcessIndex) + '/' + std::to_string(book.getPictureList().size()) << "\r";
    std::string bufferStr = imageTemplate;
    std::string className;
    className = std::filesystem::path(key).stem().string();
    className = std::regex_replace(className, std::regex(" "), "_");
    bufferStr = std::regex_replace(bufferStr, std::regex("\\{クラス名\\}"), className);

    std::string extName;
    extName = std::filesystem::path(key).extension().string().erase(0, 1);
    bufferStr = std::regex_replace(bufferStr, std::regex("\\{画像形式\\}"), extName);

    bufferStr = std::regex_replace(bufferStr, std::regex("\\{画像データ\\}"), value);
    cssTemplate = cssTemplate + bufferStr;
    PictureProcessIndex++;
  }
  generatedHtml = std::regex_replace(generatedHtml, std::regex("</style>"), "\n"+cssTemplate+"\n    </style>");
  std::cout << "\r" << "画像処理 : " << std::to_string(book.getPictureList().size()) + '/' + std::to_string(book.getPictureList().size()) << "\n";

  generatedHtml = generatedHtml + R"(
  </body>
</html>)";
  output << generatedHtml << std::endl;
  output.close();
  outputLog(" log ", "完了 finish.\n");
  return 0;
}