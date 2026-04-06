#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <fstream>
#include <string>

template<typename T>
void RandomizeVector(std::vector<T>& numbers, T min = 0, T max = 1'000'000, uint64_t seed = 0)
{
    std::mt19937_64 gen(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<T> dist(min, max);

    for (auto& x : numbers)
        x = dist(gen);
}

template<typename T>
bool SaveVectorToFile(const std::vector<T>& numbers, const std::string& filename)
{
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    size_t size = numbers.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (!numbers.empty())
        out.write(reinterpret_cast<const char*>(numbers.data()), size * sizeof(T));

    return out.good();
}

template<typename T>
bool LoadVectorFromFile(std::vector<T>& numbers, const std::string& filename, size_t count)
{
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    size_t size = 0;
    if (!in.read(reinterpret_cast<char*>(&size), sizeof(size)))
        return false;

    size_t num_to_read = std::min(size, count);
    numbers.resize(num_to_read);

    if (num_to_read > 0)
        if (!in.read(reinterpret_cast<char*>(numbers.data()), num_to_read * sizeof(T)))
            return false;

    constexpr size_t NUM_PREVIEW_NUMBERS = 0;
    if constexpr (NUM_PREVIEW_NUMBERS > 0)
    {
        std::cout << "First " << NUM_PREVIEW_NUMBERS << " =====================" <<  std::endl;
        for (size_t i = 0; i < NUM_PREVIEW_NUMBERS && i < numbers.size(); ++i)
            std::cout << numbers[i] << '\n';
        std::cout << "=============================" <<  std::endl;
    }

    return true;
}
