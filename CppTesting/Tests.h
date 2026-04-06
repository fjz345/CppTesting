#pragma once
#include <vector>
#include <cstdint>
#include <random>

#include "TestScope.h"

using TestType = int64_t;
using RandomRemoveType = uint8_t; // bool

template<bool Reverse, bool RandomRemove, typename AccType>
struct TestConfig
{
    static constexpr bool reverse = Reverse;
    static constexpr bool randomRemove = RandomRemove;
    using Acc = AccType;
};
using Configs = std::tuple<
    TestConfig<false, false, int64_t>,
    TestConfig<true, false, int64_t>,
    TestConfig<false, false, volatile int64_t>,
    TestConfig<true, false, volatile int64_t>
>;

template<template<typename, typename, bool, bool> typename Func, typename T, typename Config>
void RunTest(const std::vector<T>& Numbers, const std::vector<RandomRemoveType>& RandomRemoveMask, const char* Name)
{
    std::vector<T> NumbersCopy = Numbers;

    TEST_SCOPE(Name);

    if constexpr (Config::randomRemove)
    {
        auto result = Func<T, typename Config::Acc, Config::reverse, Config::randomRemove>(NumbersCopy, &RandomRemoveMask);
        std::cout << "Result: " << result << "\n";
    }
    else
    {
        auto result = Func<T, typename Config::Acc, Config::reverse, Config::randomRemove>(NumbersCopy);
        std::cout << "Result: " << result << "\n";
    }
}


inline void GenerateRandomMask(std::vector<RandomRemoveType> Masks, size_t N, int RemovePercent, uint32_t Seed)
{
    std::vector<RandomRemoveType> Mask(N, false);
    std::mt19937 rng(Seed);
    std::uniform_int_distribution<int> dist(1, 100);

    for (size_t i = 0; i < N; ++i)
        Mask[i] = (dist(rng) <= RemovePercent);
}

template<typename T>
__forceinline void EraseIfMasked(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& Mask, size_t i)
{
    if (Mask[i])
    {
        Numbers.erase(Numbers.begin() + i);
    }
}
template<typename T>
auto EraseIfMaskedIter(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& Mask, typename std::vector<T>::iterator it)
{
    size_t idx = std::distance(Numbers.begin(), it);
    if (Mask[idx])
    {
        return Numbers.erase(it);
    }
    else
    {
        return std::next(it);
    }
}
template<typename T>
auto EraseIfMaskedIter(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& Mask, typename std::vector<T>::reverse_iterator rit)
{
    auto it = rit.base();
    --it;               
    size_t idx = std::distance(Numbers.begin(), it);

    if (Mask[idx])
    {
        auto newIt = Numbers.erase(it);
        return std::reverse_iterator(newIt);
    }
    else
    {
        return std::next(rit);
    }
}
    
template<typename T, typename VolatileT, bool Reverse, bool RandomRemove>
__forceinline T Test_Sum_For(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& RandomRemoveMasks)
{
    volatile VolatileT Sum = 0;
    if constexpr (Reverse)
    {
        if constexpr (RandomRemove)
        {
            for (size_t i = Numbers.size(); i-- > 0; )
            {
                if (RandomRemoveMasks[i])
                    Numbers.erase(Numbers.begin() + i);
                else
                    Sum += Numbers[i];
            }
        }
        else
        {
            for (size_t i = Numbers.size(); i-- > 0; )
                Sum += Numbers[i];
        }
    }
    else
    {
        if constexpr (RandomRemove)
        {
            for (size_t i = 0; i < Numbers.size(); )
            {
                if (RandomRemoveMasks[i])
                    Numbers.erase(Numbers.begin() + i);
                else
                {
                    Sum += Numbers[i];
                    ++i;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < Numbers.size(); ++i)
                Sum += Numbers[i];
        }
    }
    return Sum;
}
template<typename T, typename VolatileT, bool Reverse, bool RandomRemove>
__forceinline T Test_Sum_Iter(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& RandomRemoveMasks)
{
    volatile VolatileT Sum = 0;

    if constexpr (Reverse)
    {
        if constexpr (RandomRemove)
        {
            for (size_t i = Numbers.size(); i-- > 0;)
            {
                if (RandomRemoveMasks[i])
                    Numbers.erase(Numbers.begin() + i);
                else
                    Sum += Numbers[i];
            }
        }
        else
        {
            for (size_t i = Numbers.size(); i-- > 0;)
                Sum += Numbers[i];
        }
    }
    else
    {
        if constexpr (RandomRemove)
        {
            for (size_t i = 0; i < Numbers.size(); )
            {
                if (RandomRemoveMasks[i])
                    Numbers.erase(Numbers.begin() + i);
                else
                {
                    Sum += Numbers[i];
                    ++i;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < Numbers.size(); ++i)
                Sum += Numbers[i];
        }
    }

    return Sum;
}

#define RUN_TEST_FUNC(FUNC, T, VolitileT, ReverseVal, RandomVal) \
do { \
NumbersCopy = Numbers; \
{ \
TEST_SCOPE(#FUNC "<" #T ", " #VolitileT ", " #ReverseVal ", " #RandomVal ">"); \
std::cout << "NumbersCopy.size() = " << NumbersCopy.size() << std::endl; \
auto result = FUNC<T, VolitileT, ReverseVal, RandomVal>(NumbersCopy, RandomRemoveMasks); \
std::cout << "Result: " << result << "\n"; \
} \
} while(0)
template<typename T>
__forceinline  void RunAll_Test_Sum_For(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& RandomRemoveMasks)
{
    std::vector<T> NumbersCopy;

    RUN_TEST_FUNC(Test_Sum_For, T, T, false, false);
    RUN_TEST_FUNC(Test_Sum_For, T, T, true,  false);
    RUN_TEST_FUNC(Test_Sum_For, T, T, false, true);
    RUN_TEST_FUNC(Test_Sum_For, T, T, true,  true);
    RUN_TEST_FUNC(Test_Sum_For, T, volatile T, false, false);
    RUN_TEST_FUNC(Test_Sum_For, T, volatile T, true,  false);
    RUN_TEST_FUNC(Test_Sum_For, T, volatile T, false, true);
    RUN_TEST_FUNC(Test_Sum_For, T, volatile T, true,  true);
}
template<typename T>
__forceinline  void RunAll_Test_Sum_Iter(std::vector<T>& Numbers, const std::vector<RandomRemoveType>& RandomRemoveMasks)
{
    std::vector<T> NumbersCopy;

    RUN_TEST_FUNC(Test_Sum_Iter, T, T, false, false);
    RUN_TEST_FUNC(Test_Sum_Iter, T, T, true,  false);
    RUN_TEST_FUNC(Test_Sum_Iter, T, T, false, true);
    RUN_TEST_FUNC(Test_Sum_Iter, T, T, true,  true);
    RUN_TEST_FUNC(Test_Sum_Iter, T, volatile T, false, false);
    RUN_TEST_FUNC(Test_Sum_Iter, T, volatile T, true,  false);
    RUN_TEST_FUNC(Test_Sum_Iter, T, volatile T, false, true);
    RUN_TEST_FUNC(Test_Sum_Iter, T, volatile T, true,  true);
}