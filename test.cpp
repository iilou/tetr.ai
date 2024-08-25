// #include <iostream>





// int**** shapes = new int***[4]{
//     new int**[7]{
//         new int*[4]{ // I
//             new int[4]{0, 0, 0, 0},
//             new int[4]{1, 1, 1, 1},
//             new int[4]{0, 0, 0, 0},
//             new int[4]{0, 0, 0, 0}
//         },
//         new int*[3]{ // T
//             new int[3]{0, 1, 0},
//             new int[3]{1, 1, 1},
//             new int[3]{0, 0, 0}
//         },
//         new int*[3]{ // L
//             new int[3]{0, 0, 1},
//             new int[3]{1, 1, 1},
//             new int[3]{0, 0, 0}
//         },
//         new int*[3]{ // J
//             new int[3]{1, 0, 0},
//             new int[3]{1, 1, 1},
//             new int[3]{0, 0, 0}
//         },
//         new int*[2]{ // O
//             new int[2]{1, 1},
//             new int[2]{1, 1}
//         },
//         new int*[3]{ // S
//             new int[3]{0, 1, 1},
//             new int[3]{1, 1, 0},
//             new int[3]{0, 0, 0}
//         },
//         new int*[3]{ // Z
//             new int[3]{1, 1, 0},
//             new int[3]{0, 1, 1},
//             new int[3]{0, 0, 0}
//         }
//     },
//     new int**[7]{
//         new int*[4]{ // I
//             new int[4]{0, 0, 1, 0},
//             new int[4]{0, 0, 1, 0},
//             new int[4]{0, 0, 1, 0},
//             new int[4]{0, 0, 1, 0}
//         },
//         new int*[3]{ // T
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 1},
//             new int[3]{0, 1, 0}
//         },
//         new int*[3]{ // L
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 1}
//         },
//         new int*[3]{ // J
//             new int[3]{0, 1, 1},
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 0}
//         },
//         new int*[2]{ // O
//             new int[2]{1, 1},
//             new int[2]{1, 1}
//         },
//         new int*[3]{ // S
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 1},
//             new int[3]{0, 0, 1}
//         },
//         new int*[3]{ // Z
//             new int[3]{0, 0, 1},
//             new int[3]{0, 1, 1},
//             new int[3]{0, 1, 0}
//         }
//     },
//     new int**[7]{ // shape[2]
//         new int*[4]{ // I
//             new int[4]{0, 0, 0, 0},
//             new int[4]{0, 0, 0, 0},
//             new int[4]{1, 1, 1, 1},
//             new int[4]{0, 0, 0, 0}
//         },
//         new int*[3]{ // T
//             new int[3]{0, 0, 0},
//             new int[3]{1, 1, 1},
//             new int[3]{0, 1, 0}
//         },
//         new int*[3]{ // L
//             new int[3]{0, 0, 0},
//             new int[3]{1, 1, 1},
//             new int[3]{1, 0, 0}
//         },
//         new int*[3]{ // J
//             new int[3]{0, 0, 0},
//             new int[3]{1, 1, 1},
//             new int[3]{0, 0, 1}
//         },
//         new int*[2]{ // O
//             new int[2]{1, 1},
//             new int[2]{1, 1}
//         },
//         new int*[3]{ // S
//             new int[3]{0, 0, 0},
//             new int[3]{0, 1, 1},
//             new int[3]{1, 1, 0}
//         },
//         new int*[3]{ // Z
//             new int[3]{0, 0, 0},
//             new int[3]{1, 1, 0},
//             new int[3]{0, 1, 1}
//         }
//     },
//     new int**[7]{ // shape[3]
//         new int*[4]{ // I
//             new int[4]{0, 1, 0, 0},
//             new int[4]{0, 1, 0, 0},
//             new int[4]{0, 1, 0, 0},
//             new int[4]{0, 1, 0, 0}
//         },
//         new int*[3]{ // T
//             new int[3]{0, 1, 0},
//             new int[3]{1, 1, 0},
//             new int[3]{0, 1, 0}
//         },
//         new int*[3]{ // L
//             new int[3]{1, 1, 0},
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 0}
//         },
//         new int*[3]{ // J
//             new int[3]{0, 1, 0},
//             new int[3]{0, 1, 0},
//             new int[3]{1, 1, 0}
//         },
//         new int*[2]{ // O
//             new int[2]{1, 1},
//             new int[2]{1, 1}
//         },
//         new int*[3]{ // S
//             new int[3]{1, 0, 0},
//             new int[3]{1, 1, 0},
//             new int[3]{0, 1, 0}
//         },
//         new int*[3]{ // Z
//             new int[3]{0, 1, 0},
//             new int[3]{1, 1, 0},
//             new int[3]{1, 0, 0}
//         }
//     }
// };


// int* len = new int[7]{4, 3, 3, 3, 2, 3, 3};


// int main(){
//     for(int i = 0; i < 4; i++){
//         std::cout<<"new int*[7]{"<<std::endl;
//         for(int j = 0; j < 7; j++){
//             std::cout<<"new int[8]{";
//             for(int k = 0; k < len[j]; k++){
//                 for(int l = 0; l < len[j]; l++){
//                     if(shapes[i][j][k][l] == 1){
//                         std::cout<<k<<", "<<l<<", ";
//                     }
//                 }
//             }
//             std::cout<<"},"<<std::endl;
//         }
//         std::cout<<"},"<<std::endl;
//     }
// }