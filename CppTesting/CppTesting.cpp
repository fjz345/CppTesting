#include <chrono>
#include <iostream>
#include <ranges>

#include "Dataset.h"
#include "Tests.h"
#include "TestScope.h"

using TestType = int64_t;
constexpr TestType NUM_TEST_ITERATIONS = 10000000000;

int main(int argc, char* argv[])
{
    std::vector<TestType> Numbers(NUM_TEST_ITERATIONS);
    std::vector<TestType> NumbersCopy;
    std::vector<bool> RandomRemoveMasks(NUM_TEST_ITERATIONS);
    {
        TEST_SCOPE("Setting up Numbers");

        //RandomizeVector(Numbers);
        //SaveVectorToFile(Numbers, "numbers.bin");

        LoadVectorFromFile(Numbers, "numbers_10000000000_1_to_1000000.bin", NUM_TEST_ITERATIONS);

        GenerateRandomMask(RandomRemoveMasks, NUM_TEST_ITERATIONS, 50, 0);
    }

    //RUN_TEST_FUNC(Test_Sum_For, TestType, volatile TestType, false, false);
    //RUN_TEST_FUNC(Test_Sum_For, TestType, volatile TestType, false, true);
    
    RunAll_Test_Sum_For(Numbers, RandomRemoveMasks); 
    RunAll_Test_Sum_Iter(Numbers, RandomRemoveMasks); 
}
