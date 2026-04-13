#include "ctime"
#include "iomanip"
#include "iostream"
#include "json.hpp"
#include "map"
#include "random"
#include "string"
#include "tuple"
#include "unordered_map"
#include "vector"
#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "sstream"

using nlohmann::json;
using std::map;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

struct com {
  vector<string> word;
  vector<string> chinese;
};
com readJson(string fileName, int start, int end) {
  // 读取单词表
  std::ifstream file(fileName);
  vector<string> word;
  vector<string> chinese;
  string line;
  int count = 0;
  while (count < end) {
    std::getline(file, line);
    if (count < start){
        count++;
        continue;
    }
    json data = json::parse(line);
    string key = data.begin().key();
    string value = data.begin().value();
    word.push_back(key);
    chinese.push_back(value);
    count++;
  }
  file.close();
  com res;
  res.word = word;
  res.chinese = chinese;
  return res;
}


int randomNum(int range) {
  // 获取随机索引
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<> distrib(0, range - 1);

  int rad = distrib(gen);

  return rad;
}

unordered_map<string, int> getCountWord(vector<string> word) {
  // 得到每日单词点认识的次数
  int n = word.size();
  unordered_map<string, int> res;
  for (int i = 0; i < n; i++) {
    res[word[i]] = 0;
  }
  return res;
}

void saveMemorizedJson(string fileName, string word, string chinese) {
  // 将已经学会的单词保存到json文件里
  vector<string> rememberWord;
  vector<string> rememberChinese;
  rememberWord.push_back(word);
  rememberChinese.push_back(chinese);
  std::ofstream outFile(fileName, std::ios::app);
  for (int i = 0; i < rememberChinese.size(); i++) {
    outFile << rememberWord[i] << " "<<rememberChinese[i]<< " ";
  }
  outFile.close();
}

void lineFeed(string fileName) {
  std::ofstream file(fileName, std::ios::app);
  file << "\n";
}

map<int, int> getForgettingCurve() {
  // 键：第几次复习
  // 值：距离上一次复习的间隔天数
  map<int, int> reviewSchedule = {
      {1, 1},  // 第1次：1天后 (24小时)
      {2, 2},  // 第2次：2天后
      {3, 4},  // 第3次：4天后
      {4, 7},  // 第4次：1周后
      {5, 15}, // 第5次：15天后
      {6, 30}, // 第6次：1个月后
      {7, 90}  // 第7次：3个月后 (进入长期记忆库)
  };
  return reviewSchedule;
}

int getDay(string fileName) {
  // 得到今天的学习天数
  std::ifstream file(fileName);
  int lineCount;
  string line;
  while (std::getline(file, line)) {
    lineCount++;
  }
  file.close();
  lineCount++;
  return lineCount;
}

template <typename T>
bool isArray(T* arr, int size, T target) {
    // 判断元素是否在数组内
    auto it = std::find(arr, arr + size, target);
    if (it != arr + size) {
        return true;
    }
    return false;
}

void getPreDayWord(string fileName, com& dayWord) {
    // // 将需要记忆的行与今天要记的单词融合
    // // 获得需要记忆单词是哪一行
    int today = getDay(fileName);
    int ForgettingArray[7] = {1, 2, 4, 7, 15, 30, 90};

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << fileName << std::endl;
        return;
    }

    // 1. 先统计总行数，这样才能准确计算“距离今天多少天”
    vector<string> allLines;
    string line;
    while (std::getline(file, line)) {
        if (!line.empty()) { // 忽略空行
            allLines.push_back(line);
        }
    }
    file.close();

    int totalLines = allLines.size();

    // 2. 遍历每一行
    for (int i = 0; i < totalLines; i++) {
        // 计算这一行距离今天的天数
        // 假设最后一行是昨天（距离1天），倒数第 totalLines 行是第一天
        // 距离 = 总行数 - 当前索引
        int daysAgo = totalLines - i;

        if (isArray(ForgettingArray, 7, daysAgo)) {
            string currentLine = allLines[i];
            std::stringstream ss(currentLine);
            string wordChinese;

            int judge = 0;
            while (ss >> wordChinese) {
                if (judge % 2 == 0) {
                    dayWord.word.push_back(wordChinese);
                }

                if (judge % 2 == 1) {
                    dayWord.chinese.push_back(wordChinese);
                }
                judge++;
            }
            
        }
    }

}

// vector<string> mixWord() {
//     // 将今天学的单词与以前遗忘的单词混合
//     com word = readJson("dictionary.json", 1, 100);
//     vector<int> preDay = getPreDay();
//     vector<string> todayWord = word.word;
//     vector<string> previousWord;
//     return previousWord;
// }

template <typename T>
vector<T> slice(const vector<T>& arr, int start, int end){
    // 对数组进行切片操作
    if (start < 0 || end > (int)arr.size() || end < start) std::cout<<"索引范围错误"<<std::endl;

    return vector<T>(arr.begin() + start, arr.begin() + end);
}


int main() {
    int needWrodCount = 5;
    int subRange = 2;

  vector<string> res;  // 学完的单词

  com day_word = readJson("dictionary.json", 1, needWrodCount);   // 今日学习单词库
  getPreDayWord("memorized.log", day_word);



  unordered_map<string, int> count_word = getCountWord(day_word.word);

  int choice, rad;
  int sub_start = 0;
  for (int sub_end = subRange; sub_end < needWrodCount; sub_end += subRange){
      vector<string> sub_day_word = slice(day_word.word, sub_start, sub_end);
      vector<string> sub_day_chinese = slice(day_word.chinese, sub_start, sub_end);

      while (!sub_day_word.empty()) {
        int currentSize = sub_day_word.size();
        if (currentSize == 0) break; // 防御性编程
        int rad = randomNum(currentSize); // randomNum 应该接受 [0, size) 范围
                                          
        string word = sub_day_word[rad];
        string chinese = sub_day_chinese[rad];
        std::cout << word << std::endl;
        std::cout << "1.认识\t2.不认识\t\t0.显示现在所学的单词数" << std::endl;
        std::cin >> choice;
        std::cout << chinese << std::endl;
        switch (choice) {}
        if (choice == 1) {
          count_word[word]++;
        }

          std::cout << std::endl;
          std::cout << std::endl;
        // if (choice == 3) {
        //   forget.push_back(word);
        //   std::cout << chinese << std::endl;
        //   std::cout << std::endl;
        //   std::cout << std::endl;
        // }

        if (choice == 3) {
            std::cout<< sub_day_word.size() << std::endl;
            for (int i = 0; i < sub_day_word.size(); i++) {
                std::cout<< sub_day_word[i] << std::endl;
            }
        }
        if (choice == 4) {
            std::cout<< day_word.word.size() << std::endl;
            for (int i = 0; i < day_word.word.size(); i++) {
                std::cout<< day_word.word[i] << std::endl;
            }
        }

        if (count_word[word] == 5) {
            // 1. 查找元素
            auto itWord = std::find(sub_day_word.begin(), sub_day_word.end(), word);
            auto itChinese = std::find(sub_day_chinese.begin(), sub_day_chinese.end(), chinese);

            if (itWord != sub_day_word.end()) {
                sub_day_word.erase(itWord); // 安全删除
                sub_day_chinese.erase(itChinese);
                res.push_back(word);

                saveMemorizedJson("memorized.log", word, chinese);
            } 
            else {
                std::cerr << "警告: 单词 '" << word << "' 计数已满但未在列表中找到！" << std::endl;
            }
        }

        if (choice == 0) {
            std::cout<< "现在已学的单词数为："<< res.size() <<std::endl;
        }

      }
      std::cout << "又学完一组单词！" << std::endl;
      sub_start += subRange;
  }
      std::cout << "单词学完了！" << std::endl;
      lineFeed("memorized.log");
}
