#pragma once

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define TEST_SCOPE(name)                                      \
static constexpr const char CONCAT(_test_name_, __LINE__)[] = name; \
TestScope<CONCAT(_test_name_, __LINE__)> CONCAT(_scope_, __LINE__)
#include <chrono>
#include <iostream>

constexpr bool bShowTimestamp = false;
template<const char* Name>
struct TestScope
{
    TestScope()
    {
        start = std::chrono::steady_clock::now();
        std::cout << Name << " START =====================" << std::endl;
        auto seconds = std::chrono::duration<double>(start.time_since_epoch());
        if (bShowTimestamp)
        {
            std::cout << "Timestamp: " << seconds.count() << std::endl; 
        }
    }
    ~TestScope()
    {
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::micro> elapsed = end - start;

        constexpr bool bSeconds = true;
        if (bSeconds)
        {
            auto seconds = std::chrono::duration<double>(elapsed);
            std::cout << Name << " Finished:\n "
                      << seconds.count() << " seconds\n";
        }
        else
        {
            std::cout << Name << " Finished:\n " << elapsed.count() << " microseconds\n";
        }

        std::cout << Name << " END ========================" << std::endl;
    }

    std::chrono::steady_clock::time_point start;
};
