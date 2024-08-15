#include <vector>
#include <numeric>
#include <iostream>

#include <cassert>

#include "AIModel.h"


// get default function
std::vector<int> get_default(){
    return std::vector<int>{
        rand() % 1000 - 999, // 0 - bumpiness
        rand() % 1000 - 999, // 1 - bumpinesssq
        rand() % 1000 - 999, // 2 - holes
        rand() % 1000 - 999, // 3 - holessq
        rand() % 1000 - 999, // 4 - caves
        rand() % 1000 - 999, // 5 - cavessq
        rand() % 1000 - 999, // 6 - hangs
        rand() % 1000 - 999, // 7 - hangssq
        // -900, // 0 - bumpiness
        // -900, // 1 - bumpinesssq
        // -900, // 2 - holes
        // -900, // 3 - holessq
        // -900, // 4 - caves
        // -900, // 5 - cavessq
        // -900, // 6 - hangs
        // -900, // 7 - hangssq
        rand() % 1000, // 8 - clear1
        rand() % 1000, // 9 - clear2
        rand() % 1000, // 10 - clear3
        rand() % 1000, // 11 - clear4
        rand() % 1000, // 12 - tsetup
        rand() % 1000, // 13 - tsetup2
        rand() % 1000, // 14 - tsetup3
        rand() % 1000, // 15 - tspin
        rand() % 1000, // 16 - tspin2
        rand() % 1000, // 17 - tspin3
        rand() % 1000, // 18 - well
        rand() % 1000 - 999, // 19 - well2
        rand() % 500 + 500, // 20 - allclear
        rand() % 1000 - 999, // 21 - height
        rand() % 1000 - 999, // 22 - heightsq
        rand() % 1000, // 23 - combo +1
        rand() % 1000, // 24 - combo +2
        rand() % 1000, // 25 - combo +3
        rand() % 1000, // 26 - combo +4
        rand() % 1000, // 27 - combo +5plus
        rand() % 1000, // 28 - b2b1
        rand() % 1000, // 29 - b2b2
        rand() % 1000, // 30 - b2b3plus 
        rand() % 1000 - 700, // 31 - wasted t piece
    };
}

// get prebuilt function
// -873 -60 -175 -846 -359 -647 8 -812 276 -23 164 -157 858 710 90 823 746 558 272 20 876 -793 -999 229 390 356 476 636 192 308 415 -174
std::vector<int> get_prebuilt(){
    return std::vector<int>{
        -873, -60, -175, -846, -359, -647, 8, -812, 276, -23, 164, -157, 858, 710, 90, 823, 746, 558, 272, 20, 876, -793, -999, 229, 390, 356, 476, 636, 192, 308, 415, -174
    };
}

// crossover gene function
int crossover_gene(int v1, int v2){
    int r = rand() % 100;

    int v;

    if(r <= 41){
        v = v1;
    } else if(r <= 83){
        v = v2;
    } else if(r <= 98){
        v = (v1 + v2) / 2;
    } else {
        v = rand() % 1999 - 999;
    }

    v += rand() % 21 - 10;

    return std::max(-999, std::min(999, v));
}

// crossover function
std::vector<int> crossover(std::vector<int>& a, std::vector<int>& b){
    assert(a.size() == b.size());
    std::vector<int> v;
    for(int i = 0; i < a.size(); i++){
        v.push_back(crossover_gene(a[i], b[i]));
    }
    return v;
}

// fitness function
int fitness(std::vector<int>& genes, std::vector<int>& values){
    assert(genes.size() == values.size());  
    return std::inner_product(genes.begin(), genes.end(), values.begin(), 0);
}

