#include <stdio.h>
#include <stdlib.h>

// 節約量の情報を格納する構造体
typedef struct {
    int i;          // 始点
    int j;          // 終点
    int saving;     // 節約量
} Saving;

// 経路の情報を格納する構造体
typedef struct {
    int* path;      // 経路の配列（地点のリスト）
    int length;     // 経路の長さ（地点数）
    int capacity;   // 現在の積載量
    int active;     // この経路が有効かどうか（1:有効, 0:無効）
} Route;

// 節約量を比較する関数（qsortで使用）
int compareSavings(const void* a, const void* b) {
    const Saving* s1 = (const Saving*)a;
    const Saving* s2 = (const Saving*)b;
    // 降順にソートするため、b - aを返す
    return s2->saving - s1->saving;
}

// セービング法を実装する関数
void savingsAlgorithm(int** costs, int* weights, int n, int max_capacity) {
    int i, j, k;
    int depot = 0;  // 配送センターは0番目とする（入力では1）
    
    // 1. 初期状態: 各地点ごとに1台の車両で配送
    Route* routes = (Route*)malloc((n - 1) * sizeof(Route));
    for (i = 0; i < n - 1; i++) {
        routes[i].path = (int*)malloc(3 * sizeof(int));
        routes[i].path[0] = depot;
        routes[i].path[1] = i + 1;
        routes[i].path[2] = depot;
        routes[i].length = 3;
        routes[i].capacity = weights[i + 1];
        routes[i].active = 1;
    }
    
    // 2. 節約量を計算し、降順にソート
    int num_savings = (n - 1) * (n - 2);  // 節約量の数
    Saving* savings = (Saving*)malloc(num_savings * sizeof(Saving));
    
    k = 0;
    for (i = 1; i < n; i++) {
        for (j = 1; j < n; j++) {
            if (i != j) {
                savings[k].i = i;
                savings[k].j = j;
                // 節約量の計算: S_ij = c_0i + c_j0 - c_ij
                savings[k].saving = costs[depot][i] + costs[j][depot] - costs[i][j];
                k++;
            }
        }
    }
    
    // 節約量を降順にソート
    qsort(savings, num_savings, sizeof(Saving), compareSavings);
    
    // 3. 節約量が大きい順に経路を結合
    for (k = 0; k < num_savings; k++) {
        int i_node = savings[k].i;
        int j_node = savings[k].j;
        int route_i = -1;
        int route_j = -1;
        
        // i_nodeで終わる経路とj_nodeで始まる経路を探す
        for (i = 0; i < n - 1; i++) {
            if (routes[i].active) {
                if (routes[i].path[routes[i].length - 2] == i_node) {
                    route_i = i;
                }
                if (routes[i].path[1] == j_node) {
                    route_j = i;
                }
            }
        }
        
        // 両方の経路が見つかり、かつ異なる経路である場合
        if (route_i != -1 && route_j != -1 && route_i != route_j) {
            // 結合後の積載量を計算
            int combined_capacity = routes[route_i].capacity + routes[route_j].capacity;
            
            // 積載量制約を満たす場合、経路を結合
            if (combined_capacity <= max_capacity) {
                int new_length = routes[route_i].length + routes[route_j].length - 2;
                int* new_path = (int*)malloc(new_length * sizeof(int));
                
                // 新しい経路を作成: [depot, ..., i, j, ..., depot]
                for (i = 0; i < routes[route_i].length - 1; i++) {
                    new_path[i] = routes[route_i].path[i];
                }
                for (j = 1; j < routes[route_j].length; j++) {
                    new_path[i + j - 1] = routes[route_j].path[j];
                }
                
                // 結合した経路を保存
                free(routes[route_i].path);
                routes[route_i].path = new_path;
                routes[route_i].length = new_length;
                routes[route_i].capacity = combined_capacity;
                
                // もう一方の経路を無効化
                routes[route_j].active = 0;
            }
        }
    }
    
    // 4. 結果を表示
    int total_cost = 0;
    printf("配送経路:\n");
    for (i = 0, k = 1; i < n - 1; i++) {
        if (routes[i].active) {
            printf("経路%d: ", k++);
            for (j = 0; j < routes[i].length; j++) {
                // 1ベースのインデックスに変換して表示
                printf("%d ", routes[i].path[j] + 1);
                
                // コストを計算
                if (j < routes[i].length - 1) {
                    total_cost += costs[routes[i].path[j]][routes[i].path[j + 1]];
                }
            }
            printf("(積載量: %d)\n", routes[i].capacity);
        }
    }
    printf("総配送コスト: %d\n", total_cost);
    
    // メモリ解放
    for (i = 0; i < n - 1; i++) {
        if (routes[i].active) {
            free(routes[i].path);
        }
    }
    free(routes);
    free(savings);
}

int main() {
    // テスト用のデータ
    int n = 5;  // 地点の数（配送センターを含む）
    
    // コスト行列（地点iから地点jへの配送コスト）
    // 注: 0ベースのインデックスを使用
    int costs_array[5][5] = {
        {0, 10, 20, 30, 25},  // 地点0（配送センター）からの配送コスト
        {10, 0, 15, 35, 20},
        {20, 15, 0, 18, 30},
        {30, 35, 18, 0, 12},
        {25, 20, 30, 12, 0}
    };
    
    // costs_arrayを二次元配列にポインタとして変換
    int** costs = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        costs[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            costs[i][j] = costs_array[i][j];
        }
    }
    
    // 各地点の荷物の重さ
    int weights[5] = {0, 4, 6, 5, 7};  // 地点0は配送センターなので重さ0
    
    // 車両の最大積載量
    int max_capacity = 15;
    
    // セービング法で解を求める
    savingsAlgorithm(costs, weights, n, max_capacity);
    
    // メモリ解放
    for (int i = 0; i < n; i++) {
        free(costs[i]);
    }
    free(costs);
    
    return 0;
}