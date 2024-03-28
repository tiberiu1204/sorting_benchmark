#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <bit>
#include <queue>

#define MAX_ARRAY_SIZE 100000001
#define MIN_ARRAY_SIZE 100000001
#define MAX_ARRAY_ELEMENT_VALUE 1000000000


enum Algorithms
{
    SELECTION_SORT,
    QS_RANDOM,
    QS_FIRST,
    QS_LAST,
    QS_MEDIAN,
    MERGESORT,
    CYCLE_SORT,
    SHELLSORT,
    RADIX_SORT_10,
    RADIX_SORT_BYTE,
    RADIX_SORT_2_16,
    HEAP_SORT,
    STL_SORT
};

enum QuicksortType {
    MEDIAN,
    RANDOM,
    FIRST,
    LAST
};

template<typename T>
class Benchmark
{
public:
    explicit Benchmark(const std::vector<T> &arr) : arr(arr), copy(arr), rng(dev())
    {}

    void print_array() const
    {
        std::cout << "Array is: ";
        for (const T &num: arr)
        {
            std::cout<<num<<" ";
        }
        std::cout<<"\n";
    }


    void time(Algorithms alg) {
        bool stopped = false;
        std::cout<<"Sorting array with "<<arr.size()<<" elements.\n";
        if (arr.size() <= 100)
            print_array(); // poate folosim std::vector<typename>::operator<<(2), cred ca ar fi mai clean
        auto time_start = std::chrono::high_resolution_clock::now();
        switch(alg) {
            // Pasul 2: Adauga un case exact asa cum am adaugat eu mai jos. Pasul 3 se va gasi in main.
            case QS_RANDOM:
                std::cout<<"Using quicksort algorithm with random pivot selection.\n";
                if (arr.size() <= 100000000) {
                    quicksort(0, arr.size() - 1, RANDOM);
                } else stopped = true;
                break;
            case QS_MEDIAN:
                std::cout<<"Using quicksort algorithm with median pivot selection.\n";
                if (arr.size() <= 100000000) {
                    quicksort(0, arr.size() - 1, MEDIAN);
                } else stopped = true;
                break;
            case QS_LAST:
                std::cout<<"Using quicksort algorithm with last pivot selection.\n";
                if (arr.size() <= 100000000) {
                    quicksort(0, arr.size() - 1, LAST);
                } else stopped = true;
                break;
            case QS_FIRST:
                std::cout<<"Using quicksort algorithm with first pivot selection.\n";
                if (arr.size() <= 100000000) {
                    quicksort(0, arr.size() - 1, FIRST);
                } else stopped = true;
                break;
            case SELECTION_SORT:
                std::cout<<"Using selection sort algorithm.\n";
                if (arr.size() <= 200000) {
                    selection_sort();
                } else stopped = true;
                break;
            case MERGESORT:
                std::cout<<"Using mergesort algorithm.\n";
                if (arr.size() <= 50000000) {
                    arr = mergesort(0, arr.size() - 1);
                } else stopped = true;
                break;
            case CYCLE_SORT:
                std::cout << "Using cycle sort algorithm.\n";
                if (arr.size() <= 80000) {
                    cycle_sort();
                } else stopped = true;
                break;
            case SHELLSORT:
                std::cout << "Using shellsort algorithm.\n";
                if (arr.size() <= 3000000) {
                    shellsort();
                } else stopped = true;
                break;
            case RADIX_SORT_10:
                std::cout << "Using radix sort with base 10 algorithm.\n";
                radix_sort(10);
                break;
            case RADIX_SORT_BYTE:
                std::cout << "Using radix sort by bytes algorithm.\n";
                radix_sort();
                break;
            case RADIX_SORT_2_16:
                std::cout << "Using radix sort with base 2^16 algorithm.\n";
                radix_sort(0x00010000);
                break;
            case HEAP_SORT:
                std::cout << "Using heap sort algorithm.\n";
                if (arr.size() <= 40000000) {
                    heap_sort();
                } else stopped = true;
                break;
            case STL_SORT:
                std::cout << "Using STL sort algorithm.\n";
                stl_sort();
                break;
        }
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start);
        if(arr.size() <= 100) {
            print_array();
        }
        if (!stopped) {
            std::cout << "Completed in: " << std::fixed << std::setprecision(6)
                      << 0.000001 * static_cast<double>(duration.count()) << " seconds.\n";
            std::cout << (std::is_sorted(std::begin(arr), std::end(arr)) ? "Array is sorted correctly."
                                                                         : "Array is not sorted correctly.") << "\n\n";
        } else std::cout << "Array is too large.\n\n";
        arr = copy;
    }
private:
    std::vector<T> arr;
    std::vector<T> copy;
    std::random_device dev;
    std::mt19937 rng;


    std::vector<T> merge(std::vector<T> v1, std::vector<T> v2)
    {
        std::vector<T> res_vec;
        size_t i = 0, j = 0;
        while(i < v1.size() && j < v2.size()) {
            if (v1.at(i) <= v2.at(j))
            {
                res_vec.push_back(v1.at(i++));
            }
            else {
                res_vec.push_back(v2.at(j++));
            }
        }
        while (i < v1.size()) res_vec.push_back(v1.at(i++));
        while (j < v2.size()) res_vec.push_back(v2.at(j++));
        return res_vec;
    }

    void quicksort(size_t start, size_t end, QuicksortType t) {
        if(start >= end) return;
        std::uniform_int_distribution<std::mt19937::result_type> dist(start,end);
        size_t pivot;
        switch(t) {
            case RANDOM:
                pivot = dist(rng);
                break;
            case MEDIAN:
                pivot = (start + end) / 2;
                break;
            case LAST:
                pivot = end;
                break;
            case FIRST:
                pivot = start;
                break;
        }
        std::swap(arr.at(pivot), arr.at(end));
        size_t i = start, j = start;
        while(j < end) {
            if (arr.at(j) <= arr.at(end))
            {
                std::swap(arr.at(j), arr.at(i++));
            }
            j++;
        }
        std::swap(arr.at(end), arr.at(i));
        if(i > 0) quicksort(start, i - 1, t);
        quicksort(i + 1, end, t);
    }

    void selection_sort() {
        for (size_t i = 0; i < arr.size() - 1; i++)
        {
            T *min = &arr.at(i);
            for (size_t j = i + 1; j < arr.size(); j++)
            {
                if (*min > arr.at(j)) min = &arr.at(j);
            }
            std::swap(*min, arr.at(i));
        }
    }

    std::vector<T> mergesort(size_t start, size_t end)
    {
        if(start == end) {
            return {arr.at(start)};
        }
        size_t mid = (start + end) / 2;
        std::vector<T> v1 = mergesort(start, mid);
        std::vector<T> v2 = mergesort(mid + 1, end);
        return merge(v1, v2);
    }

    void cycle_sort()
    {
        size_t max_cycle_start = arr.size() - 2, max_arr_index = arr.size() - 1;
        for (size_t cycle_start = 0; cycle_start <= max_cycle_start; ++cycle_start)
        {
            T pivot = arr.at(cycle_start);
            size_t position;
            do
            {
                position = cycle_start;
                for (size_t i = cycle_start + 1; i <= max_arr_index; ++i)
                {
                    if (arr.at(i) < pivot) position++;
                }
                if (position != cycle_start)
                {
                    while (arr.at(position) == pivot) position++;
                }
                std::swap(arr.at(position), pivot);
            } while (position != cycle_start);
        }
    }

    void shellsort()
    {
        size_t gaps[8] = {701, 301, 132, 57, 23, 10, 4, 1}; // Marcin Ciura experimental gapping
        for (const auto& gap : gaps)
        {
            for (size_t i = gap; i < arr.size(); ++i) {
                T temp = arr.at(i);
                size_t j;
                for (j = i; (j >= gap) && (arr.at(j - gap) > temp); j -= gap)
                {
                    arr.at(j) = arr.at(j - gap);
                }
                arr.at(j) = temp;
            }
        }
    }

    // NOTE: THIS SH*T IS IMPLEMENTATION DEFINED! SO IF THE NERDS THAT WRITE COMPILERS DECIDE THAT FLOATS SHOULD BE 3 BYTES AND INTS 53 BYTES, IT WILL FAIL WITH A SOMERSAULT!
    void radix_sort(unsigned radix = 256) //by default perform byte by byte sorting
    {
        // preprocessing step so the type of the data does not matter: flip all sign bits
        // also get the max element as unsigned for later
        if(!std::is_unsigned<T>::value)
        {
            for(auto& element : arr){
                reinterpret_cast<unsigned &>(element) ^= 0x80000000;
            }
        }
        unsigned max_element = 0;
        for (const auto &element : arr){
            max_element = std::max(std::bit_cast<unsigned>(element), max_element);
        }
        auto unsigned_max_element = std::bit_cast<unsigned>(max_element);
        std::vector<std::vector<T>> buckets(radix);

        if(radix == 256)
        {
            for (unsigned byte_index = 0; byte_index < 4; ++byte_index)
            {
                for (const auto &element : arr)
                {
                    buckets[(std::bit_cast<unsigned>(element)>>(byte_index*8))&0xff].emplace_back(element);
                }
                arr.clear();
                auto dest = std::back_inserter(arr);
                for (auto &bucket : buckets) {
                    arr.insert(std::end(arr), std::begin(bucket), std::end(bucket));
                    bucket.clear();
                }
            }
        } else if (radix == 0x00010000)
        {
            for (unsigned byte_pair_index = 0; byte_pair_index < 2; ++byte_pair_index)
            {
                for (const auto &element : arr)
                {
                    buckets[(std::bit_cast<unsigned>(element)>>(byte_pair_index*16))&0xffff].emplace_back(element);
                }
                arr.clear();
                auto dest = std::back_inserter(arr);
                for (auto &bucket : buckets) {
                    arr.insert(std::end(arr), std::begin(bucket), std::end(bucket));
                    bucket.clear();
                }
            }
        } else
        {
            for (unsigned long long order = 1; unsigned_max_element / order > 0; order *= radix)
            {
                for (const auto &element : arr)
                {
                    buckets[(std::bit_cast<unsigned>(element) / order) % radix].emplace_back(element);
                }
                arr.clear();
                auto dest = std::back_inserter(arr);
                for (auto &bucket : buckets)
                {
                    arr.insert(std::end(arr), std::begin(bucket), std::end(bucket));
                    bucket.clear();
                }
            }
        }

        // repair array
        if(!std::is_unsigned<T>::value)
        {
            for(auto& element : arr){
                reinterpret_cast<unsigned &>(element) ^= 0x80000000;;
            }
        }
    }

    void heap_sort()
    {
        std::priority_queue<T> heap;
        for (const auto &element : arr)
        {
            heap.push(element);
        }
        arr.clear();
        while(!heap.empty())
        {
            arr.emplace_back(heap.top());
            heap.pop();
        }
        std::reverse(arr.begin(), arr.end());
    }

    void stl_sort()
    {
        std::sort(arr.begin(), arr.end());
    }
};

int main() {
    std::random_device dev;
    auto rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> arr_size_dist(MIN_ARRAY_SIZE, MAX_ARRAY_SIZE);
    std::uniform_int_distribution<std::mt19937::result_type> arr_num_dist(0, MAX_ARRAY_ELEMENT_VALUE);

    std::vector<int> arr;
    unsigned int arr_size = arr_size_dist(rng);
    arr.reserve(arr_size);
    for(int i = 0; i < arr_size; i++) {
        arr.push_back(static_cast<int>(arr_num_dist(rng)));
    }


    Benchmark bm(arr);
    bm.time(MERGESORT); // 92424605 elements ~ 128s => stop at 50 million
    bm.time(SELECTION_SORT); // 100k elements ~ 30s => at over 200k elements abort sort (TLE, >60s runtime)
    bm.time(QS_RANDOM);
    bm.time(QS_MEDIAN); // Stop all quicksorts at 100 million => 60s exactly
    bm.time(QS_LAST);
    bm.time(QS_FIRST);
    bm.time(CYCLE_SORT); // 100k elements ~ 76s => at over 80(-ish)k elements abort sort (TLE, >60s runtime)
    bm.time(SHELLSORT);
    bm.time(RADIX_SORT_10); // this one seems to be the slowest among radix sorts
    bm.time(RADIX_SORT_BYTE); // this one (read in Vladut voice) "Trage da rupe scaunu"
    bm.time(RADIX_SORT_2_16); // interestingly good, ties with byte one (???) when elements <= 10000, loses afterward
    bm.time(HEAP_SORT);
    bm.time(STL_SORT);

    return 0;
}
