#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CITIES 100

typedef struct {
    double x, y;
} City;

void createCityList(City cities[], int n); // 都市生成
double calculateDistance(City a, City b); // 二点間の距離
void showStatus(int route[], double distances[][MAX_CITIES], int n); // ルートの状態を表示
void calculateDistancesMatrix(double distances[MAX_CITIES][MAX_CITIES], City cities[], int n); // 距離行列を計算
double calculateTotalDistance(int route[], double distances[][MAX_CITIES], int n); // ルートに従って距離を計算
void initRoute(int route[], int n); // 初期ルート生成
void swapInt(int *a, int *b); // 整数の交換
void twoOptSwap(int route[], int i, int j); // 2-optの部分区間を反転
void twoOpt(int route[], double distances[][MAX_CITIES], int n); // 2-opt 主処理

void twoOpt(int route[], double distances[][MAX_CITIES], int n) {
    /*
    2-OPT アルゴリズム
    都市を一筆書きするとき，コストが最小となるようなルートを探す．

    はじめ，適当なルートを生成する.
    その中のに変を選び，頂点を交換する.
        - A - B -        - A   B -
                    ->       X
        - C - D -        - C   D -
    もし，これがコストが低くなるなら，ルートを更新する.
    更新がなければ終了する.
    */
    int flag = 1; // 改善が必要かどうかのフラグ
    while (flag) {
        flag = 0;
        double bestDistance = calculateTotalDistance(route, distances, n);
        // 2つの都市間の部分区間を全て試す
        for (int i = 0; i < n; i++) {
            for (int k = i + 1; k < n; k++) {

                // 新しいルートをコピー
                int newRoute[MAX_CITIES];
                for (int m = 0; m < n; m++) {
                    newRoute[m] = route[m];
                }

                // 部分区間を反転
                twoOptSwap(newRoute, i, k);

                // 新しいルートの距離を計算
                double newDistance = calculateTotalDistance(newRoute, distances, n);

                // 改善されていればルートを更新
                if (newDistance < bestDistance) {
                    for (int m = 0; m < n; m++) {
                        route[m] = newRoute[m];
                    }
                    bestDistance = newDistance;
                    flag = 1;
                    break;
                }
            }
            if (flag) break;
        }
    }
}


int main() {
    int n = 10;  // 都市の数

    srand((unsigned int)time(NULL)); // 乱数のシードを設定
    City cities[MAX_CITIES];
    double dist[MAX_CITIES][MAX_CITIES];
    int route[MAX_CITIES];

    createCityList(cities, n); // 都市の座標をランダムに生成
    calculateDistancesMatrix(dist, cities, n); // 距離行列を計算
    initRoute(route, n); // ルートをランダムに生成

    printf("-----最初のルート------\n");
    showStatus(route, dist, n); // 最適化前
    twoOpt(route, dist, n); // 最適化
    printf("-----最適化後のルート------\n");
    showStatus(route, dist, n); // 最適化後

    return 0;
}


void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void createCityList(City cities[], int n) {
    for (int i = 0; i < n; i++) {
        cities[i].x = rand() % 100;
        cities[i].y = rand() % 100;
    }
}

double calculateDistance(City a, City b) { 
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void calculateDistancesMatrix(double distances[MAX_CITIES][MAX_CITIES], City cities[], int n) {
    /*
    距離行列を計算
    各地点間の距離を計算し距離行列に格納することで，距離を高速に計算することを目的とする．
    */
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double distance = calculateDistance(cities[i], cities[j]);
            distances[i][j] = distances[j][i] = distance;
        }
    }
}

double calculateTotalDistance(int route[], double distances[][MAX_CITIES], int n) {
    // ルートに従って距離を計算
    double totalDistance = 0;
    for (int i = 0; i < n; i++) {
        int start = route[i];
        int end = route[(i + 1) % n];
        totalDistance += distances[start][end];
    }
    return totalDistance;
}

void initRoute(int route[], int n) {
    // 初期のルートをランダムに生成
    for (int i = 0; i < n; i++) {
        route[i] = i;
    }
    for (int i = 0; i < n; i++) {
        int j = rand() % (n - i);
        swapInt(&route[i], &route[i + j]);
    }
}

void twoOptSwap(int route[], int i, int j) {
    // i から j までの部分区間を反転
    while (i < j) {
        swapInt(&route[i], &route[j]);
        i++;
        j--;
    }
}

void showStatus(int route[], double distances[][MAX_CITIES], int n) {
    double totalDistance = calculateTotalDistance(route, distances, n);
    printf("訪問順: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", route[i]);
    }
    printf("\n");
    printf("距離: %.2f\n", totalDistance);
}

