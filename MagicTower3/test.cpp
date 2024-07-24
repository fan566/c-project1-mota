// #if 0
// #include <iostream>
// #include <thread>
// #include <atomic>
// #include <chrono>

// // 使用 std::atomic 确保线程安全
// std::atomic<bool> should_exit(false);

// void infinite_loop() {
//     while (!should_exit) {
//         // 模拟一些工作
//         std::cout << "Working..." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
//     std::cout << "Loop ended." << std::endl;
// }

// void wait_for_exit_command() {
//     char input;
//     std::cout << "Enter 'q' to quit: ";
//     std::cin >> input;
//     if (input == 'q') {
//         should_exit = true;
//     }
// }

// int main() {
//     // 启动无限循环线程
//     std::thread work_thread(infinite_loop);

//     // 在主线程中等待退出命令
//     wait_for_exit_command();

//     // 等待工作线程结束
//     work_thread.join();

//     std::cout << "Program ended." << std::endl;
//     return 0;
// }
// #endif 

// #if 1
// #include <iostream>
// #include <thread>
// #include <atomic>
// #include <chrono>
// #include <future>

// std::atomic<bool> should_exit(false);

// void limited_loop(int max_iterations, int max_seconds) {
//     int iterations = 0;
//     auto start_time = std::chrono::steady_clock::now();

//     while (!should_exit) {
//         // 模拟一些工作
//         std::cout << "Working... Iteration: " << iterations + 1 << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));

//         // 检查迭代次数
//         if (++iterations >= max_iterations) {
//             std::cout << "Max iterations reached." << std::endl;
//             break;
//         }

//         // 检查是否超时
//         auto current_time = std::chrono::steady_clock::now();
//         auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
//         if (elapsed >= max_seconds) {
//             std::cout << "Time limit reached." << std::endl;
//             break;
//         }
//     }

//     should_exit = true;  // 确保另一个线程也会结束
//     std::cout << "Loop ended." << std::endl;
// }

// void wait_for_exit_command() {
//     while (!should_exit) {
//         std::cout << "Enter 'q' to quit: ";
//         char input;
//         std::cin >> input;
//         if (input == 'q' || should_exit) {
//             should_exit = true;
//             break;
//         }
//     }
// }

// int main() {
//     const int MAX_ITERATIONS = 10;
//     const int MAX_SECONDS = 15;

//     // 使用 std::async 来启动循环，这样我们可以等待它的完成
//     auto loop_future = std::async(std::launch::async, limited_loop, MAX_ITERATIONS, MAX_SECONDS);

//     // 在另一个线程中等待退出命令
//     std::thread input_thread(wait_for_exit_command);

//     // 等待循环结束或用户输入
//     loop_future.wait();

//     // 如果循环已经结束，确保输入线程也结束
//     should_exit = true;
//     input_thread.join();

//     std::cout << "Program ended." << std::endl;
//     return 0;
// }
// #endif



// #if 1
// #include<iostream>
// using namespace std;
// int main(){
//     cin.putback('q');
//     char c;
//     cin>>c;
//     std::cout<<c<<endl;

// }

// #endif