#include <chrono>
#include <iostream>
#include <ranges>

#include "Dataset.h"
#include "Tests.h"
#include "TestScope.h"

constexpr TestType NUM_TEST_ITERATIONS = 1'000'000'000;

int main(int argc, char* argv[])
{
    std::vector<TestType> Numbers(NUM_TEST_ITERATIONS);
    std::vector<TestType> NumbersCopy(NUM_TEST_ITERATIONS);
    std::vector<RandomRemoveType> RandomRemoveMasks(NUM_TEST_ITERATIONS);

    std::cout << "NUM_TEST_ITERATIONS = " << NUM_TEST_ITERATIONS << std::endl;
    {
        TEST_SCOPE("Numbers");

        //RandomizeVector(Numbers);
        //SaveVectorToFile(Numbers, "numbers_10000000000.bin");

        LoadVectorFromFile(Numbers, "numbers_10000000000.bin", NUM_TEST_ITERATIONS);
    }
    {
        TEST_SCOPE("RandomRemoveMasks");

        //GenerateRandomMask(RandomRemoveMasks, NUM_TEST_ITERATIONS, 50, 0);
        //SaveVectorToFile(RandomRemoveMasks, "random_remove_masks_10000000000.bin");

        LoadVectorFromFile(RandomRemoveMasks, "random_remove_masks_10000000000.bin", NUM_TEST_ITERATIONS);
    }

    //RUN_TEST_FUNC(Test_Sum_For, TestType, volatile TestType, false, false);
    //RUN_TEST_FUNC(Test_Sum_For, TestType, volatile TestType, false, true);

    //{
    //    TEST_SCOPE("TEST");
    //    std::cout << "Numbers.size() = " << Numbers.size() << std::endl;
    //    auto a = Test_Sum_For<TestType, volatile TestType, false, false>(Numbers, RandomRemoveMasks);
    //    std::cout << a << std::endl;
    //}
    
    RunAll_Test_Sum_For(Numbers, RandomRemoveMasks);
    RunAll_Test_Sum_Iter(Numbers, RandomRemoveMasks); 
}
