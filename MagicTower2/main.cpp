
#if 1
#include"object.h"

int main(){
    srand(time(nullptr)); 
    Log l;
    Scence* sc =Scence::GetSingScence();
    shared_ptr<Scence> s(sc);
    Menu menu(s);
    l.attach(&menu);
    
    l.Display();
    while(true){
        menu.MenuDisplay();
        s->GUIRun();
    }
    // Log l;
    // // l.Register();
    // l.Login();
}

#endif

#if 0
#include"object.h"
int main(){


//   std::ofstream outfile("./文档/3.txt", std::ios::out);
//   if (outfile.is_open()) {
//       outfile << "发愤忘食的\n";
//       outfile.close();
//       std::cout << "文件写入成功！" << std::endl;
//   } else {
//       std::cerr << "无法打开文件！" << std::endl;
//   }

    // Tools::printFormattedOutput(60,0,{"fang","chuang","zheng"});
  return 0;
}

#endif

#if 0
#include <iostream>
#include <cstdlib>

#include <iostream>

class MyClass {
private:
    int value;

public:
    MyClass(int v) : value(v) {}

    void setValue(int v) {
        value = v;
    }

    int getValue() const {
        return value;
    }

    void printValue() const {
        std::cout << "Value: " << value << std::endl;
    }
};

int main() {
    MyClass obj(10);

    // 测试非常量对象
    obj.setValue(20);
    
    std::cout << "Non-const object value: " << obj.getValue() << std::endl;  // 输出：20

    // 测试常量对象
    const MyClass constObj(30);
    std::cout << "Const object value: " << constObj.getValue() << std::endl;  // 输出：30

    return 0;
}

#endif

#if 0
#include"object.h"
int main(){


    // Tools::printFormattedOutput(60,0,{"fang","chuang","zheng"});
    int i=0;
    for(int i=0;i<=100;i+=10){
        Tools::printProgressBar(80,'#',i,100);
        Tools::Delaymoment(1000);
    }
  return 0;
}

#endif