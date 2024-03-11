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

enum Algorithms {
    SELECTION_SORT,
    QUICKSORT,
    MERGESORT
};

class Benchmark {
public:
    Benchmark(const std::vector<int> &arr) : arr(arr), copy(arr) {
        rng = std::mt19937(dev());
    }
    void print_array() const {
        std::cout<<"Array is: ";
        for(const int &num : arr) {
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
        if(arr.size() <= 100) print_array();
        auto time_start = std::chrono::high_resolution_clock::now();
        switch(alg) {
            // Pasul 2: Adauga un case exact asa cum am adaugat eu mai jos. Pasul 3 se va gasi in main.
            case QUICKSORT:
                std::cout<<"Using quicksort algorithm.\n";
                quicksort(0, arr.size() - 1);
                break;
            case SELECTION_SORT:
                std::cout<<"Using selection sort algorithm.\n";
                selection_sort();
                break;
            case MERGESORT:
                std::cout<<"Using mergesort algorithm.\n";
                arr = mergesort(0, arr.size() - 1);
                break;
        }
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start);
        if(arr.size() <= 100) {
            print_array();
        }
        std::cout<<"Completed in: "<<std::fixed<<std::setprecision(6)<<0.000001 * static_cast<double>(duration.count())<<" seconds.\n\n";
        arr = copy;
    }
private:
    std::vector<int> arr;
    std::vector<int> copy;
    std::random_device dev;
    std::mt19937 rng;

    /*
     * Bludutz, in aceasta sectiune privata a clasei iti vei scrie tu algoritmii si functiile ajutatoare, daca ai
     * nevoie. Dupe ce termini de implementat, vezi functia "time" din sectiunea public.
     */

    std::vector<int> merge(std::vector<int> v1, std::vector<int> v2) {
        std::vector<int> res_vec;
        int i = 0, j = 0;
        while(i < v1.size() && j < v2.size()) {
            if(v1[i] <= v2[j]) {
                res_vec.push_back(v1[i++]);
            }
            else {
                res_vec.push_back(v2[j++]);
            }
        }
        while(i < v1.size()) res_vec.push_back(v1[i++]);
        while(j < v2.size()) res_vec.push_back(v2[j++]);
        return res_vec;
    }

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

    std::vector<int> mergesort(size_t start, size_t end) {
        if(start == end) {
            return {arr[start]};
        }
        size_t mid = (start + end) / 2;
        std::vector<int> v1 = mergesort(start, mid);
        std::vector<int> v2 = mergesort(mid + 1, end);
        return merge(v1, v2);
    }
};

int main() {
    std::random_device dev;
    auto rng = std::mt19937(dev());
    std::uniform_int_distribution<std::mt19937::result_type> arr_size_dist(0, MAX_ARRAY_SIZE);
    std::uniform_int_distribution<std::mt19937::result_type> arr_num_dist(0, MAX_ARRAY_ELEMENT_VALUE);

    std::vector<int> arr;
    unsigned int arr_size = arr_size_dist(rng);
    for(int i = 0; i < arr_size; i++) {
        arr.push_back(static_cast<int>(arr_num_dist(rng)));
    }

    // Pasul 3: cheama functia pasand enumul potrivit.

    Benchmark bm(arr);
    bm.time(MERGESORT);
    bm.time(SELECTION_SORT);
    bm.time(QUICKSORT);
    return 0;
}
