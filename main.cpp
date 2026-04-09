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
#include <string>
#include <unordered_map>
#include <vector>

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

void saveMemorizedJson(string fileName, string word) {
  // 将已经学会的单词保存到json文件里
  vector<string> rememberWord;
  rememberWord.push_back(word);
  std::ofstream outFile(fileName, std::ios::app);
  for (string w : rememberWord) {
    outFile << w << " ";
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

vector<int> computeForgetDay() {
    // 算出每一天与今天相差几天
    int today = getDay("memorized.log");
    vector<int> res;
    int dayCount;
    for (int i = 1; i < today; i++){
        dayCount = today - i;
        res.push_back(dayCount);
    }
    return res;
}

vector<string> getPreWord(string fileName) {
    int row = 0;

}

vector<string> mixWord() {
    com word = readJson("dictionary.json", 1, 100);
    vector<string> todayWord = word.word;
    vector<string> previousWord;
}

template <typename T>
vector<T> slice(const vector<T>& arr, int start, int end){
    // 对数组进行切片操作
    if (start < 0 || end > arr.size() || end > start) std::cout<<"索引范围错误"<<std::endl;

    return vector<T>(arr.begin() + start, arr.begin() + end);
}

int main() {
  vector<string> familiar;
  vector<string> res;

  com day_word = readJson("dictionary.json", 1, 100);
  vector<string> sub_day_word = slice(day_word.word, 0, 10);
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
    switch (choice) {}
    if (choice == 1) {
      familiar.push_back(word);
      std::cout << chinese << std::endl;
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
            
            saveMemorizedJson("memorized.log", word);
            
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
