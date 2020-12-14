#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <vector>
#include <cassert>
using std::cout;
using std::endl;

#define TEST_CASE(n) std::cout << "TEST: " << n << '\n';
#define EXPECT(p) if (p) {} else std::cout << __LINE__ << ": FAILED: " << #p << '\n'
#define ASSERT(p) if (p) {} else return ((std::cout << __LINE__ << ":FAILED: " << #p << '\n'), -1)
#define EXPECT_THROW(p) { \
        bool throwed = false; try { p; } catch(...) { throwed = true; } \
        if (!throwed) std::cout << "unexpected no-throw: " << #p << '\n'; EXPECT(throwed); }
#define ASSERT_NO_THROW(p) \
        try { p; } catch(std::exception& ex) { \
        std::cout << "unexpected throw: " << ex.what() << '\n'; ASSERT(!#p); } 

std::atomic<int> counter {0};

template <class mType>
class ThreadSafeQueue{
    std::queue<mType> mQueue;
    std::mutex queueMutex;
public:
    ThreadSafeQueue () {}
    
    void addToQueue(mType message)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        mQueue.push(message);
    }
    
    mType popFromQueue(){
        std::lock_guard<std::mutex> lock(queueMutex);
        if(mQueue.empty())
            throw ("Queue is empty");

        mType task = std::move(mQueue.front());
        mQueue.pop();
        return task;
    }
    
    bool isEmpty(){
         std::lock_guard<std::mutex> lock(queueMutex);
         return mQueue.empty();
    }
};

template <class mType>
void executeTask(ThreadSafeQueue<mType>& queue){
    while(!queue.isEmpty()){
        counter += queue.popFromQueue()();
    }
}

template <class mType>
void putTaskToQueue(ThreadSafeQueue<mType>& queue, int counter){
    for(int i=0; i<counter; ++i)
        queue.addToQueue([](){return 1;});
}

class Workers{
    std::vector<std::thread> mThreads;
    ThreadSafeQueue<std::function<int()>> tasks;
    int mThreadsAmount;
public:
    Workers(unsigned int rowThread) :mThreads(rowThread) { }
    
    void runThreads()
    {
       for(auto i = 0, i < rowThread, i++)
            mThreads.emplace_back(std::thread<int()>([](){return 1;}));
    }
    
    void addTask()
};

// void shouldCreateQueue()
// {
//     ThreadSafeQueue 
// }

int main()
{
    TEST_CASE("shouldCreateQueue"){
        ThreadSafeQueue<int> testQueue;
        EXPECT(true);
    }
    
    TEST_CASE("shouldPushAndPop"){
        ThreadSafeQueue<int> testQueue;
        testQueue.addToQueue(5);
        EXPECT(5==testQueue.popFromQueue());
        EXPECT(testQueue.isEmpty());
        EXPECT_THROW(testQueue.popFromQueue());
    }
    
    TEST_CASE("should_be_accessed_from_two_threads"){
        ThreadSafeQueue<std::function<int()>> testQueue;
        for(int i=0; i<300000; ++i)
        {
            testQueue.addToQueue([](){return 1;});
        }
        std::thread first(executeTask<std::function<int()>>, std::ref(testQueue));
        std::thread second(executeTask<std::function<int()>>, std::ref(testQueue));
        
        first.join();
        second.join();
        EXPECT(counter == 300000);
        
    }
    
    TEST_CASE("should be added by two threads"){
        ThreadSafeQueue<std::function<int()>> testQueue;
        std::thread first(putTaskToQueue<std::function<int()>>, std::ref(testQueue), 150000);
        std::thread second(putTaskToQueue<std::function<int()>>, std::ref(testQueue), 150000);
        first.join();
        second.join();
        for(int i=0; i<300000; ++i)
        {
            testQueue.popFromQueue();
        }
        EXPECT(testQueue.isEmpty());
    }
    
    TEST_CASE("ShouldAddByOneThreadAndReadBySecondThread")
    {
        ThreadSafeQueue<std::function<int()>> testQueue;
        std::thread addingThread(putTaskToQueue<std::function<int()>>, std::ref(testQueue), 3000000);
        std::thread executingThread(executeTask<std::function<int()>>, std::ref(testQueue));
        addingThread.join();
        executingThread.join();
        EXPECT(testQueue.isEmpty());
    }
//   ASSERT_NO_THROW(Roman r1(1));
//   Roman r1(1);
//   EXPECT(r1.toInt() == 1);
//   EXPECT(r1.getValue() == "I");
    cout << "This is a debug message, threads: " << std::thread::hardware_concurrency() << endl;
    return 0;
}
