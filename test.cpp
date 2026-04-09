// #include <iostream>
// #include <chrono>
//
// int main() {
//     using namespace std::chrono;
//
//     // 获取当前时间点
//     auto now = system_clock::now();
//
//     // 截断到天（去除时分秒），转换为 year_month_day 格式
//     auto today = floor<days>(now);
//     year_month_day ymd{today};
//
//     std::cout << "当前日期: " 
//               << ymd.year() << "年" 
//               << static_cast<unsigned>(ymd.month()) << "月" 
//               << static_cast<unsigned>(ymd.day()) << "日" 
//               << std::endl;
//
//     return 0;
// }
