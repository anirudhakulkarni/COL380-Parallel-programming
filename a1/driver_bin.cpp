#include <iostream>
#include <fstream>
#include <stdint.h>
#include <chrono>
#include <omp.h>
#include "psort.h"


void check_sorted(uint32_t *data, uint32_t n)
{
    for (uint32_t i = 0; i < n - 1; i++)
    {
        if (data[i] > data[i + 1])
        {
            std::cout << "Data is not sorted.\n";
            return;
        }
    }

    std::cout << "Data is sorted.\n";
    return;
}


void SortData(uint32_t *data, uint32_t n, int p, int n_threads)
{
    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp single
        ParallelSort(data, n, p);
    }
}


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Insufficient Arguments\n"; // <datafile> <n_threads>
        return 0;
    }

    auto beg = std::chrono::high_resolution_clock::now();
    std::cout << "Reading data from file" << std::endl;
    std::fstream fs(argv[1], std::ios::in | std::ios::binary);

    // Reading data
    uint32_t n, d = 0;
    int p, n_threads = atoi(argv[2]);
    fs.read((char *)&n, sizeof(n));
    fs.read((char *)&p, sizeof(p));
    uint32_t *data = new uint32_t[n];

    std::cout << "n_threads = " << n_threads << std::endl;
    std::cout << "N = " << n << " p = " << p << std::endl;

    for (d = 0; d < n; d++) {
        fs.read((char *)&data[d], sizeof(uint32_t));
    }

    fs.close();
    auto endd = std::chrono::high_resolution_clock::now();
    double durationn = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(endd - beg)).count());
    std::cout << "Time taken to read file of " << n << " elements with "
        << p << " buckets = " << durationn << "ms" << std::endl;

    // Sorting
    auto begin = std::chrono::high_resolution_clock::now();
    SortData(data, n, p, n_threads);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    double duration = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());

    check_sorted(data, n);
    std::cout << "Time taken for sorting " << n << " elements with "
        << p << " buckets = " << duration << "ms" << std::endl;

    // Clean-up
    delete data;

    return 0;
}