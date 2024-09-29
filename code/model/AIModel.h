/*

from Piece import *;
import numpy as np;
import random;





def get_default():
    return np.array([
        random.randint(-999, 0), # 0 - bumpiness
        random.randint(-999, 0), # 1 - bumpinesssq
        random.randint(-999, 0), # 2 - holes
        random.randint(-999, 0), # 3 - holessq
        random.randint(-999, 0), # 4 - caves
        random.randint(-999, 0), # 5 - cavessq
        random.randint(-999, 0), # 6 - hangs
        random.randint(-999, 0), # 7 - hangssq
        random.randint(0, 999), # 8 - clear1
        random.randint(0, 999), # 9 - clear2
        random.randint(0, 999), # 10 - clear3
        random.randint(0, 999), # 11 - clear4
        random.randint(0, 999), # 12 - tsetup
        random.randint(0, 999), # 13 - tsetup2
        random.randint(0, 999), # 14 - tsetup3
        random.randint(0, 999), # 15 - tspin
        random.randint(0, 999), # 16 - tspin2
        random.randint(0, 999), # 17 - tspin3
        random.randint(0, 999), # 18 - well
        random.randint(-999, 0), # 19 - well2
        random.randint(500, 999), # 20 - allclear
        random.randint(-999, 0), # 21 - height
        random.randint(-999, 0)  # 22 - heightsq
    ])

def crossover_gene(v1, v2):
    rand = random.randint(0, 99)

    if rand <= 41:
        v = v1
    elif rand <= 83:
        v = v2
    elif rand <= 98:
        v = (v1 + v2) // 2
    else: 
        v = random.randint(-999, 999)
    
    v += random.randint(-10, 10)
    
    return max(-999, min(999, v))

def crossover(a, b):
    return np.array( [crossover_gene(a[i], b[i]) for i in range(len(a))] );

def fitness(genes, values):
    """
    :params: genes: np.array
    :params: values: np.array - 

    bumpiness, bumpinesssq,             2\n
    holes, holessq,                     4\n
    caves, cavessq,                     6\n
    hangs, hangssq,                     8\n
    clear1, clear2, clear3, clear4,     12\n
    tsetup, tsetup2, tsetup3,           15\n
    tspin, tspin2, tspin3,              18\n
    well, well2,                        20\n
    allclear                            21\n
    height, heightsq,                   23\n

    :return: fitness value 
    """
    return np.multiply(values, genes).sum()

def test_t_spin():
    return np.array([
        0, 0, 
        0, 0, 
        0, 0,
        0, 0,
        0, 0, 0, 0,
        1, 10, 100,
        0, 0, 0,
        0, 0,
        0,
        0, 0
        ])

 */

#ifndef AIMODEL_H
#define AIMODEL_H

#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

std::vector<int> get_default();
std::vector<int> get_prebuilt();
int crossover_gene(int v1, int v2);
std::vector<int> crossover(std::vector<int>& a, std::vector<int>& b);
int fitness(std::vector<int>& genes, std::vector<int>& values);

#endif // AIMODEL_H