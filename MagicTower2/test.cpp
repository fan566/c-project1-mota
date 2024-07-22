// #include <iostream>
// #include <string>
// #include <thread>
// #include <chrono>
// #include <future>
// #include <atomic>

// std::string timedCin(int timeout_seconds) {
//     std::string input;
//     std::atomic<bool> input_ready(false);

//     // 创建一个异步任务来处理输入
//     auto future = std::async(std::launch::async, [&]() {
//         std::getline(std::cin, input);
//         input_ready = true;
//     });

//     // 等待输入或超时
//     if (future.wait_for(std::chrono::seconds(timeout_seconds)) == std::future_status::timeout) {
//         std::cout << "\nInput timed out." << std::endl;
//         // 如果需要，可以在这里添加代码来清除输入缓冲区
//         return "";
//     }

//     // 如果输入准备好了，返回输入的内容
//     if (input_ready) {
//         return input;
//     }

//     return "";
// }

// int main() {
//     std::cout << "Enter something within 5 seconds: ";
//     std::string result = timedCin(5);

//     if (!result.empty()) {
//         std::cout << "You entered: " << result << std::endl;
//     } else {
//         std::cout << "No input received." << std::endl;
//     }

//     return 0;
// }