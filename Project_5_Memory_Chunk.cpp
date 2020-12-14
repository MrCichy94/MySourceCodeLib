#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <map>
using std::cout;


class Partition
{
    

public:  

    unsigned _noBlocks;
    unsigned _blockSize;
    unsigned _totalSize;
    
    std::map<char*, bool> _chunks;
    char* _pointer;

    Partition(unsigned blockSize, unsigned noBlocks) : _noBlocks(noBlocks), _blockSize(blockSize)
    {
        _totalSize = _blockSize * _noBlocks;
        
        _pointer = new char[_totalSize];
        
        createMap();
    }
    
    void createMap()
    {
        
        for(int i=0; i < _noBlocks; i++) {
            
            _chunks[_pointer+(_blockSize*i)] = false;
        }
    }
    
    void* getChunk()
    {
        
        for( auto& x : _chunks )
        {
            if (not(x.second))
            {
                x.second = true;
                return (void*)x.first;
            }
        }
        
        return nullptr;
    }
    
    
    
    ~Partition()
    {
        
    }
};

class Memory
{
public:
    virtual void* getMemory(size_t size, std::vector<Partition*>* partitions) = 0;
    
    virtual void freeMemory(void* e, std::vector<Partition*>* partitions) = 0;
    
    virtual ~Memory() = default;
    
};

class DefaultMemory : public Memory
{
    void* getMemory(size_t size, std::vector<Partition*>* partitions) {
        
        for(auto& p : *partitions)
        {
            if(p->_blockSize >= size){
                
                auto temp = p->getChunk();
                
                if(temp!=nullptr){
                    return temp;
                }
            
            }
        }
        std::cout << "Default Memory allocator" << std::endl;
        return nullptr;
        
    }
    
    void freeMemory(void* e, std::vector<Partition*>* partitions)
    {
        
        for(auto& p : *partitions)
        {
                
                auto temp = p->_chunks.find((char*)e); // iterator do pointera lub za ostatni element
                
                if(temp!=p->_chunks.end())
                    {
                        (*temp).second = false;
                    }
        }
        
        std::cout << "Default Memory deallocator" << std::endl;
    }
};

class FirstStopMemory : public Memory
{
    void* getMemory(size_t size, std::vector<Partition*>* partitions) {
        std::cout << "FirstStop Memory allocator" << std::endl;
    }
    
    void freeMemory(void* e, std::vector<Partition*>* partitions) {
        
    }
};


class Allocator
{
    std::unique_ptr<Memory> _memory;
    std::vector<Partition*> _partitions;
    
public:
    
    
    Allocator(std::unique_ptr<Memory> memory) : _memory(std::move(memory)) 
    {
        _partitions.push_back(new Partition(4, 32));
    }
    
    void* getMemory(size_t size)
    {
        return _memory->getMemory(size, &_partitions);
    }
    
    void freeMemory(void* e)
    {
        _memory->freeMemory(e, &_partitions);
    }
    
    void switchMemory(std::unique_ptr<Memory> memory)
    {
        //delete _memory;
       // _memory = std::move(memory);
    }
};

std::unique_ptr<Memory> mem = std::make_unique<DefaultMemory>();
Allocator allocator(std::move(mem));


class Object
{
public:
    
    void * operator new(size_t size)
    {
        std::cout << "Dummy new overloaded " << size << std::endl;
        return allocator.getMemory(size);
    }
    
    void * operator new[](size_t size)  
    {
        std::cout << "Dummy new[] overloaded " << size << std::endl;
    }
    
    void operator delete(void* e)
    {
        std::cout << "Dummy new overloaded " << std::endl;
        return allocator.freeMemory(e);
    }
    
    void operator delete[](void* e)
    {
        std::cout << "Dummy delete[]" << std::endl;
    }
};


class Small : public Object {
    char _size[4];
};

class Medium : public Object {
    char _size[32];
};

class Big : public Object {
    char _size[128];
};


int main()
{
        
    Small* maly = new Small;
   
   std::cout << &maly << std::endl;

   //Small* malya= new Small[5];
    
   // Medium* sredni = new Medium;
   // Big* duzy = new Big;

    delete maly;
    
    return 0;
}
