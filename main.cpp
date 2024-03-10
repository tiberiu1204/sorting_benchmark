#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

enum Algorithms {
    SELECTION_SORT,
    QUICKSORT
};

class Benchmark {
public:
    Benchmark(const std::vector<int> &arr) : arr(arr) {
        rng = std::mt19937(dev());
    }
    void print_array() const {
        for(const int &num : arr) {
            std::cout<<num<<" ";
        }
        std::cout<<"\n";
    }
    void time(Algorithms alg) {
        std::cout<<"Sorting array: ";
        print_array();
        auto time_start = std::chrono::high_resolution_clock::now();
        switch(alg) {
            case QUICKSORT:
                quicksort(0, arr.size() - 1);
                break;
            case SELECTION_SORT:
                selection_sort();
                break;
        }
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start);
        std::cout<<"Sorted array: ";
        print_array();
        std::cout<<"Completed in: "<<duration.count()<<" microseconds.\n";
    }
private:
    std::vector<int> arr;
    std::random_device dev;
    std::mt19937 rng;

    void quicksort(size_t start, size_t end) {
        if(start >= end) return;
        std::uniform_int_distribution<std::mt19937::result_type> dist(start,end);
        size_t pivot = dist(rng);
        std::swap(arr[pivot], arr[end]);
        size_t i = start, j = start;
        while(j < end && i < end) {
            if(arr[j] <= arr[end]) {
                std::swap(arr[j], arr[i++]);
            }
            j++;
        }
        std::swap(arr[end], arr[i]);
        if(i == 0) i++;
        quicksort(start, i - 1);
        quicksort(i + 1, end);
    }

    void selection_sort() {
        for(int i = 0; i < arr.size() - 1; i++) {
            int *min = &arr[i];
            for(int j = i + 1; j < arr.size(); j++) {
                if(*min > arr[j]) min = &arr[j];
            }
            std::swap(*min, arr[i]);
        }
    }
};

int main() {
    Benchmark bm({123, 2, 36, 46, 34, 23, 56, 347, 834, 1234, 123, 645, 834, 64, 2346, 74});
    bm.time(QUICKSORT);
    return 0;
}
