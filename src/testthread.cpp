#include <iostream>
#include <unistd.h>
#include <thread>

void display(bool *status, std::string str){
    int y = 0;
    while(true){
        y++;
        if(y>5){
            break;
        }
        std::cout << str << std::endl;
        sleep(1);
    }
    *status = true;
}

int main(){
    bool status[] = {false, false}; 
    std::string str = "str";
    std::thread thread1(display, &status[0], str);
    std::thread thread2(display, &status[1], "hello mydear");
    thread1.detach();
    thread2.detach();
    while (true);
}