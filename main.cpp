#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#define MAX_ARRAY_SIZE 100000
#define MAX_ARRAY_ELEMENT_VALUE 10000

/*
 * Hey Bludutz, pentru ca stiu ca esti mai prost, am adaugat instructiuni detaliate despre cum sa adaugi algoritmi
 * la benchmark. Nu trebuie sa modifici decat ce iti indic eu, vectorul de sortat este generat automat (daca vrei
 * poti umbla la macro-urile de sus ca sa ii dai tweak). Pentru valori ale array size-ului mai mici sau egale cu 100,
 * vectorul o sa fie afisat in consola inainte de sortare si dupa, pentru debugging. Poti incepe citind instructiunile
 * de la linia 77.
 */

/*
Hey Pizderiu, pentru ca ti-a fost lene sa scrii cod safe si sa ne faci viata usoara cand Doomitran ne va cere sa testam
sortarile pe float, am facut eu cateva modificari si am scos warningurile. Vezi ca iti trebuie CXX >= 17, ca altfel trebuie
sa specifici la compile time tipul template-ului. Noapte buna, revin si cu alte imbunatatiri.
        P.S. : vezi ca mai sunt 2 warninguri, 2 de recursivitate.
*/

enum Algorithms
{
    SELECTION_SORT,
    QS_RANDOM,
    QS_FIRST,
    QS_LAST,
    QS_MEDIAN,
    MERGESORT,
    CYCLE_SORT
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

    /*
     * Bludutz, dupa ce ai terminat de implementat algoritmul de sortare (vezi mai jos), intoarce-te la aceasta functie
     * si urmeaza intocmai pasii pe care ti-i voi descrie.
     * Pasul 1: Du-te la enum Algorithms si adauga un enum nou care sa reprezinte algoritmul tau. Pasul 2 mai jos.
     */

    void time(Algorithms alg) {
        std::cout<<"Sorting array with "<<arr.size()<<" elements.\n";
        if (arr.size() <= 100)
            print_array(); // poate folosim std::vector<typename>::operator<<(2), cred ca ar fi mai clean
        auto time_start = std::chrono::high_resolution_clock::now();
        switch(alg) {
            // Pasul 2: Adauga un case exact asa cum am adaugat eu mai jos. Pasul 3 se va gasi in main.
            case QS_RANDOM:
                std::cout<<"Using quicksort algorithm with random pivot selection.\n";
                quicksort(0, arr.size() - 1, RANDOM);
                break;
            case QS_MEDIAN:
                std::cout<<"Using quicksort algorithm with median pivot selection.\n";
                quicksort(0, arr.size() - 1, MEDIAN);
                break;
            case QS_LAST:
                std::cout<<"Using quicksort algorithm with last pivot selection.\n";
                quicksort(0, arr.size() - 1, LAST);
                break;
            case QS_FIRST:
                std::cout<<"Using quicksort algorithm with first pivot selection.\n";
                quicksort(0, arr.size() - 1, FIRST);
                break;
            case SELECTION_SORT:
                std::cout<<"Using selection sort algorithm.\n";
                selection_sort();
                break;
            case MERGESORT:
                std::cout<<"Using mergesort algorithm.\n";
                arr = mergesort(0, arr.size() - 1);
                break;
            case CYCLE_SORT:
                std::cout << "Using cycle sort algorithm.\n";
                cycle_sort();
                break;
        }
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start);
        if(arr.size() <= 100) {
            print_array();
        }
        bool is_sorted = true;
        for (size_t i = 1; i < arr.size(); ++i)
        {
            if (arr.at(i) < arr.at(i - 1)) is_sorted = false;
        }
        std::cout << "Completed in: " << std::fixed << std::setprecision(6)
                  << 0.000001 * static_cast<double>(duration.count()) << " seconds.\n";
        std::cout << (is_sorted ? "Array is sorted correctly." : "Array is not sorted correctly.") << "\n\n";
        arr = copy;
    }
private:
    std::vector<T> arr;
    std::vector<T> copy;
    std::random_device dev;
    std::mt19937 rng;

    /*
     * Bludutz, in aceasta sectiune privata a clasei iti vei scrie tu algoritmii si functiile ajutatoare, daca ai
     * nevoie. Dupe ce termini de implementat, vezi functia "time" din sectiunea public.
     */

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
            int pivot = arr.at(cycle_start);
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
};

int main() {
    std::random_device dev;
    auto rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> arr_size_dist(10, MAX_ARRAY_SIZE);
    std::uniform_int_distribution<std::mt19937::result_type> arr_num_dist(0, MAX_ARRAY_ELEMENT_VALUE);

    std::vector<int> arr;
    unsigned int arr_size = arr_size_dist(rng);
    arr.reserve(arr_size);
    for(int i = 0; i < arr_size; i++) {
        arr.push_back(static_cast<int>(arr_num_dist(rng)));
    }

    // Pasul 3: cheama functia pasand enumul potrivit.

    Benchmark bm(arr);
    bm.time(MERGESORT);
    bm.time(SELECTION_SORT);
    bm.time(QS_RANDOM);
    bm.time(QS_MEDIAN);
    bm.time(QS_LAST);
    bm.time(QS_FIRST);
    bm.time(CYCLE_SORT);
    return 0;
}
