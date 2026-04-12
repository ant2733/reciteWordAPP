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
    // 将需要记忆的行与今天要记的单词融合
    // 获得需要记忆单词是哪一行
    vector<int> preDay;
    int today = getDay(fileName);
    com appendWord;
    int ForgettingArray[7] = {1, 2, 4, 7, 15, 30, 90};

    std::ifstream file(fileName);
    string line;
    int count = today;
    while (std::getline(file, line)) {
        count--;    // count就是距离该行距离今天的天数
        if (isArray(ForgettingArray, 7, count)) {
            vector<string> tokens;
            string wordChinese;
            int judge = 0;
            std::stringstream ss(line);
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

void init() {
  com day_word = readJson("dictionary.json", 1, 100);
  vector<string> sub_day_word = slice(day_word.word, 4, 8);

}

int main() {
  vector<string> familiar;
  vector<string> res;

  int row = sub_day_word.size();

  unordered_map<string, int> count_word = getCountWord(day_word.word);

  int choice, rad;
  int count = 0;
  rad = randomNum(row);
  while (!day_word.word.empty()) {
    string word = day_word.word[rad];
    string chinese = day_word.chinese[rad];
    std::cout << word << std::endl;
    std::cout << "1.认识\t2.模糊\t3.陌生" << std::endl;
    std::cin >> choice;
    std::cout << chinese << std::endl;
    switch (choice) {}
    if (choice == 1) {
      familiar.push_back(word);
      std::cout << std::endl;
      std::cout << std::endl;

      count_word[word]++;
    }
    // if (choice == 2) {
    //   vague.push_back(word);
    //   std::cout << chinese << std::endl;
    //   std::cout << std::endl;
    //   std::cout << std::endl;
    // }
    // if (choice == 3) {
    //   forget.push_back(word);
    //   std::cout << chinese << std::endl;
    //   std::cout << std::endl;
    //   std::cout << std::endl;
    // }

    if (count_word[word] == 5) {
        // 1. 查找元素
        auto it = std::find(day_word.word.begin(), day_word.word.end(), word);
        
        // 2. 【关键修复】检查是否真的找到了该元素
        if (it != day_word.word.end()) {
            day_word.word.erase(it); // 安全删除
            res.push_back(word);
            count++;
            
            saveMemorizedJson("memorized.log", word, chinese);
            
            // 注意：如果 row 依赖于 count，确保这里 row 的值是预期的
        } 
        else {
            std::cerr << "警告: 单词 '" << word << "' 计数已满但未在列表中找到！" << std::endl;
        }
    }

    rad = randomNum(row - count); 
  }
  lineFeed("memorized.log");
  std::cout << "单词学完了！" << std::endl;
}
