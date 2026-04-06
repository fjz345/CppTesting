#pragma once

#include <random>
#include <vector>

#include <fstream>
#include <string>

void RandomizeVector(std::vector<int64_t>& numbers, int64_t min = 0, int64_t max = 1'000'000)
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dist(min, max);

    for (auto& x : numbers)
        x = dist(gen);
}

bool SaveVectorToFile(const std::vector<int64_t>& numbers, const std::string& filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    size_t size = numbers.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(numbers.data()), size * sizeof(int64_t));
    return true;
}


bool LoadVectorFromFile(std::vector<int64_t>& numbers, const std::string& filename, size_t count)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    size_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    size_t num_to_read = std::min(size, count);
    numbers.resize(num_to_read);
    in.read(reinterpret_cast<char*>(numbers.data()), num_to_read * sizeof(int64_t));

    constexpr auto NUM_PREIVEW_NUMERS = 0;
    if (NUM_PREIVEW_NUMERS >= 1)
    {
        std::cout << "First " << NUM_PREIVEW_NUMERS << " =====================" <<  std::endl;
        for (size_t i = 0; i < NUM_PREIVEW_NUMERS && i < numbers.size(); ++i)
            std::cout << numbers[i] << '\n';
        std::cout << "=============================" <<  std::endl;
    }


    return true;
}