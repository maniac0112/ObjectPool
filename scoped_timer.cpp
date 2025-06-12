#include <iostream>
#include <chrono>
class timer{
public: 
    timer(): start_time(std::chrono::high_resolution_clock::now()){
    }

    ~timer(){
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        //cast this to microseconds 
        std::chrono::duration<double, std::micro> elapsed_time_micro = end_time - start_time; 
        //print the elapsed time
        std::cout<<"Elapsed time: "<< elapsed_time_micro.count() << " microseconds\n";

    }
private: 
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time; 
};