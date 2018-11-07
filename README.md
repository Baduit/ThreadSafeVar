# ThreadSafeVar

## Description
Simple wrapper to create thread safe variable with a mutex.

## How to use it?
There are examples in the file __srd/main.cpp__

## Requirements
You need C++ 17 to make it work.
Well, it could work with C++ 14 i think if you replace
```C++
std::lock_guard lock(_mutex);
```
with
```C++
std::lock_guard<std::mutex> lock(_mutex);
```
but i wanted to use the C++ 17 fancy syntaxe, it is clearer, shorter and cooler.


## How to install it?
Simply add the file __src/ThreadSafeVar.hpp__ to your include path and inclue the file file in you project, this is a header-only library this is all it needs to work.


