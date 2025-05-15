#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_CITIES 100
#define DEPOT 0


typedef struct {
    double x, y;
} City;

typedef struct {
    int i, j;
    double saving;
} Saving;

City cities[MAX_CITIES];
double distance[MAX_CITIES][MAX_CITIES];
int route[MAX_CITIES] = {0};

void createCityList(City cities[], int n); // 都市生成
double calculateDistance(City a, City b); // 二点間の距離
// void calculateDistancesMatrix(double distances[MAX_CITIES][MAX_CITIES], City cities[], int n); // 距離行列を計算
// void computeSavings(Saving savings[], int *count); // セービング計算
void swapSaving(Saving *a, Saving *b); // セービングの交換
void sortSavings(Saving savings[], int count); // セービングのソート
void applySavings(Saving savings[], int count); // セービング適用

void createCityList(City cities[], int n) {
    for (int i = 0; i < n; i++) {
        cities[i].x = rand() % 100;
        cities[i].y = rand() % 100;
    }
}

double calculateDistance(City a, City b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}


// void calculateDistancesMatrix(double distances[MAX_CITIES][MAX_CITIES], City cities[], int n) {
//     /*
//     距離行列を計算
//     各地点間の距離を計算し距離行列に格納することで，距離を高速に計算することを目的とする．
//     */
//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             double distance = calculateDistance(cities[i], cities[j]);
//             distances[i][j] = distances[j][i] = distance;
//         }
//     }
// }

void calculateDistancesMatrix(double distance[MAX_CITIES][MAX_CITIES], City cities[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = calculateDistance(cities[i], cities[j]);
            distance[i][j] = distance[j][i] = d;
        }
    }
}


// セービング計算
// void computeSavings(Saving savings[], int *count) {
//     *count = 0;
//     for (int i = 1; i < MAX_CITIES; i++) {
//         for (int j = i + 1; j < MAX_CITIES; j++) {
//             double s = distance[DEPOT][i] + distance[DEPOT][j] - distance[i][j];
//             savings[*count].i = i;
//             savings[*count].j = j;
//             savings[*count].saving = s;
//             (*count)++;
//         }
//     }
// }

void calculateSavings(Saving savings[], int *count, int n) {
    *count = 0;
    for (int i = 1; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double s = distance[DEPOT][i] + distance[DEPOT][j] - distance[i][j];
            savings[*count].i = i;
            savings[*count].j = j;
            savings[*count].saving = s;
            (*count)++;
        }
    }
}


void swapSaving(Saving *a, Saving *b) {
    Saving temp = *a;
    *a = *b;
    *b = temp;
}

void sortSavings(Saving savings[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (savings[i].saving < savings[j].saving) {
                swapSaving(&savings[i], &savings[j]);
            }
        }
    }
}

void applySavings(Saving savings[], int count) {
    for (int k = 0; k < count; k++) {
        int i = savings[k].i;
        int j = savings[k].j;
        if (route[i] == 0 && route[j] == 0) {
            printf("Depot → %d → %d → Depot (Saving: %.2f)\n", i, j, savings[k].saving);
            route[i] = 1;
            route[j] = 1;
        }
    }


    for (int i = 1; i < MAX_CITIES; i++) {
        if (route[i] == 0) {
            printf("Depot → %d → Depot (単独配送)\n", i);
        }
    }
}


int main(void) {
    srand((unsigned int)time(NULL)); // 乱数のシードを設定

    int n = 10;
    double distances[MAX_CITIES][MAX_CITIES];
    createCityList(cities, n);
    calculateDistancesMatrix(distances, cities, n);

    int count = 0;
    Saving saving[MAX_CITIES * MAX_CITIES];

    calculateSavings(saving, &count, n);
    sortSavings(saving, count);
    applySavings(saving, count);

    return 0;
}