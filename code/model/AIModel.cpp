#include <vector>
#include <numeric>
#include <iostream>

#include <cassert>

#include "AIModel.h"


// get default function
std::vector<int> get_default(){
    return std::vector<int>{
        rand() % 1999 - 999, // 0 - bumpiness
        rand() % 1999 - 999, // 1 - bumpinesssq
        rand() % 1999 - 999, // 2 - holes
        rand() % 1999 - 999, // 3 - holessq
        rand() % 1999 - 999, // 4 - caves
        rand() % 1999 - 999, // 5 - cavessq
        rand() % 1999 - 999, // 6 - hangs
        rand() % 1999 - 999, // 7 - hangssq
        rand() % 1999 - 999, // 8 - clear1
        rand() % 1999 - 999, // 9 - clear2
        rand() % 1999 - 999, // 10 - clear3
        rand() % 1999 - 999, // 11 - clear4
        rand() % 1999 - 999, // 12 - tsetup
        rand() % 1999 - 999, // 13 - tsetup2
        rand() % 1999 - 999, // 14 - tsetup3
        rand() % 1999 - 999, // 15 - tspin
        rand() % 1999 - 999, // 16 - tspin2
        rand() % 1999 - 999, // 17 - tspin3
        rand() % 1999 - 999, // 18 - allclear
        rand() % 1999 - 999, // 19 - height
        rand() % 1999 - 999, // 20 - heightsq
        rand() % 1999 - 999, // 21 - combo +1
        rand() % 1999 - 999, // 22 - combo +2
        rand() % 1999 - 999, // 23 - combo +3
        rand() % 1999 - 999, // 24 - combo +4
        rand() % 1999 - 999, // 25 - combo +5plus
        rand() % 1999 - 999, // 26 - b2b1
        rand() % 1999 - 999, // 27 - b2b2
        rand() % 1999 - 999, // 28 - b2b3plus 
        rand() % 1999 - 999, // 29 - wasted_t_piece
        rand() % 1999 - 999, // 30 - well 1
        rand() % 1999 - 999, // 31 - well 2
        rand() % 1999 - 999, // 32 - well 3
        rand() % 1999 - 999, // 33 - well 4
        rand() % 1999 - 999, // 34 - well 5
        rand() % 1999 - 999, // 35 - well 6
        rand() % 1999 - 999, // 36 - well 7
        rand() % 1999 - 999, // 37 - well 8
        rand() % 1999 - 999, // 38 - well 9
        rand() % 1999 - 999, // 39 - highheight
        rand() % 1999 - 999, // 40 - wasted i piece
    };
}

// get prebuilt function
// -172 -21 -627 -879 755 -648 -338 -315 791 329 -469 85 319 860 -20 967 434 -650 -361 -782 -186 70 116 546 256 853 -593 525 769 -307 401 611 -574 -163 624 727 584 504 289 -533
std::vector<int> get_prebuilt(){
    return std::vector<int>{
        -172, // 0 - bumpiness
        -21, // 1 - bumpinesssq
        -627, // 2 - holes
        -879, // 3 - holessq
        755, // 4 - caves
        -648, // 5 - cavessq
        -338, // 6 - hangs
        -315, // 7 - hangssq
        791, // 8 - clear1
        329, // 9 - clear2
        -469, // 10 - clear3
        85, // 11 - clear4
        319, // 12 - tsetup
        860, // 13 - tsetup2
        -20, // 14 - tsetup3
        967, // 15 - tspin
        434, // 16 - tspin2
        -650, // 17 - tspin3
        -361, // 18 - allclear
        -782, // 19 - height
        -186, // 20 - heightsq
        70, // 21 - combo +1
        116, // 22 - combo +2
        546, // 23 - combo +3
        256, // 24 - combo +4
        853, // 25 - combo +5plus
        -593, // 26 - b2b1
        525, // 27 - b2b2
        769, // 28 - b2b3plus 
        -307, // 29 - wasted t piece
        401, // 30 - well 1
        611, // 31 - well 2
        -574, // 32 - well 3
        -163, // 33 - well 4
        624, // 34 - well 5
        727, // 35 - well 6
        584, // 36 - well 7
        504, // 37 - well 8
        289, // 38 - well 9
        -533, // 39 - highheight
        -500, // 40 - wasted i piece
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

