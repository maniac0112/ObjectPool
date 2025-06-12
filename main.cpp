#include <iostream>
#include <memory>
#include <stack> 
#include <thread>
#include "scoped_timer.cpp" 
#include <iomanip>
#include <optional>

class sample_object{
public:
    sample_object(int input = 0): val(input){
        //std::cout<<"parameterized/default ctor invoked \n"; 
    }

    sample_object(const sample_object& obj): val(obj.val){
        //std::cout<<"copy ctor invoked \n"; 
    } 

    sample_object(sample_object&& obj) noexcept: val(obj.val){
        //std::cout<<"move ctor invoked \n"; 
    } 

    ~sample_object(){
        //std::cout<<"dtor invoked \n"; 
    } 

    int get_val(){
        return val; 
    }
private:
    int val; 
};


template <typename T>
class ObjectPool; // 

// Custom deleter that returns the memory to the pool
template <typename T>
struct ObjectPoolDeleter {
    ObjectPool<T>* pool;

    // Custom deleter function
    void operator()(T* ptr) const {
        pool->release(ptr);  // Deallocate the memory back to the pool
    }
};


template <typename T> 
class ObjectPool{
public:
    ObjectPool (std::size_t n): native_block(malloc(n* sizeof(T))){
        T* T_ptr = reinterpret_cast<T*>(native_block); 
        while(n--){
            ::new (T_ptr) T(); //construct the object in the allocated memory using placement new
            pool.push(T_ptr);
            T_ptr++;
        }
    }

    std::shared_ptr<T> getInstance(){
        if (pool.empty()){
            std::cout<<"pool exhausted \n";
            return std::shared_ptr<T>(nullptr);
        }

        std::shared_ptr<T> ptr(pool.top(), ObjectPoolDeleter<T>{this});
        pool.pop();
        return ptr; 
    }

    void release(T* object){
        //std::cout<<"resetting so one more ctor call \n";
        ::new (object) T(); //placement new to call the default constructor again to reset the object 
        pool.push(object);
    }

    ~ObjectPool(){

        //We should wait for the objects to return to pool. If this goes out of scope without 
        //having all objects back, objects from the pool out in the wild would be freeed unexceptedly through free call
        
        T* T_ptr = reinterpret_cast<T*>(native_block); 
        while(!pool.empty()){
            auto p = pool.top(); 
            pool.pop();
            p->~T(); //call the destructor explicitly
        }
        free(native_block);
    }

private: 
    std::stack<T*> pool;
    void* native_block; 
};


void print(std::vector<int>& v){
    for (auto i: v){
        std::cout<<i<<", ";
    }
    std::cout<<"\n";    
}

void print(std::vector<std::chrono::duration<double, std::micro>>& v){
    for (auto i: v){
        //set precision to 6 decimal places
        std::cout<<std::fixed<<std::setprecision(6)<<i.count()<<", ";
    }
    std::cout<<"\n";    
}

int main(){
    std::vector<std::chrono::duration<double, std::micro>> experiments(10); 
    std::vector<std::chrono::duration<double, std::micro>> experiments_2(10); 
    std::vector<int> v1(100);
    std::vector<int> v2(100);
    int idx = 0;  
    for (int i=0; i<10; i++){
        //timer t(&experiments[i]);
        ObjectPool<sample_object> pool (1000);
        for (int i=0; i<100; i++){
            auto p = pool.getInstance();
            v1[idx++]+= p->get_val();
        }
    }

    for (int i=0; i<10; i++){
        //timer t(&experiments_2[i]);
        idx = 0; 
        for (int i=0; i<100; i++){
            auto p = new sample_object();
            v2[idx++] += p->get_val();
            delete p;
        }
    }

    print(experiments);
    print(experiments_2);
    print(v1);
    print(v2);     
}