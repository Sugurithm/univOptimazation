#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define INF 9999
#define MAX_ROUTES 100
#define MAX_ROUTE_LEN 20

int c[N + 1][N + 1]; // 配送コスト行列
int w[N + 1]; // 荷物の重さ（w[0]は配送センターなので0）
int W = 15; // 積載量上限

typedef struct {
    int start, end;
    int saving;
} Saving; // Saving : iとjを結びた場合の節約できた量

typedef struct {
    int nodes[MAX_ROUTE_LEN];
    int length;
    int total_weight;
} Route; // Route : 経路

Route routes[MAX_ROUTES]; // ルート
int route_count = 0;

Saving savings[N * N]; // セービング値
int saving_count = 0;


// 各地点の荷物の重さと配送コストを初期化
void initialize() {
    // 乱数シード
    srand(time(NULL));

    // 配送コスト行列を生成
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            if (i == j) c[i][j] = 0;
            else if (i < j) {
                int cost = rand() % 16 + 5; // 5〜20
                c[i][j] = c[j][i] = cost;
            }
        }
    }

    // 荷物重さ生成
    w[0] = 0;
    for (int i = 1; i <= N; i++) {
        w[i] = rand() % 10 + 1; // 1~10
    }

    // 初期状態では各地点を個別のルートとして扱う.
    for (int i = 1; i <= N; i++) {
        routes[route_count].nodes[0] = 0; // 出発地点（配送センター）
        routes[route_count].nodes[1] = i; // 行き先：顧客i
        routes[route_count].nodes[2] = 0; // 帰着地点（配送センター）
        routes[route_count].length = 3; // ルートの長さは３
        routes[route_count].total_weight = w[i]; // 荷物の重さ
        route_count++;
    }
}

// セービング値を計算
void calculate_savings() {
    /*
    iとjを結びた場合の節約できた量 = iとjを別々に配達した場合のコスト合計 − 直結したときのコスト
    （S_ij = c_0i + c_0j - c_ij）
    */
    saving_count = 0;
    for (int i = 1; i <= N; i++) {
        for (int j = i + 1; j <= N; j++) {
            int s = c[0][i] + c[0][j] - c[i][j];
            savings[saving_count++] = (Saving){i, j, s};
        }
    }
}

void swapSaving(Saving *a, Saving *b) {
    Saving temp = *a;
    *a = *b;
    *b = temp;
}

void sortSavings() {
    for (int i = 0; i < saving_count - 1; i++) {
        for (int j = 0; j < saving_count - i - 1; j++) {
            if (savings[j].saving < savings[j + 1].saving) {
                swapSaving(&savings[j], &savings[j + 1]);
            }
        }
    }
}

// セービング順に経路を結合
void merge_routes() {
    sortSavings(); // セービング値を降順にソート

    // 節約量を基に新しいルートを検討
    for (int s = 0; s < saving_count; s++) {
        int i = savings[s].start;
        int j = savings[s].end;

        // 顧客i, j を含むルートのインデックスを探す
        int ri = -1, rj = -1; 
        for (int r = 0; r < route_count; r++) {
            Route *route = &routes[r];

            // ルートの開始直後・終了直前（0->i->0）にあるか否かで判断
            if (route->nodes[1] == i && route->nodes[route->length - 2] == i)
                ri = r;
            if (route->nodes[1] == j && route->nodes[route->length - 2] == j)
                rj = r;
        }

        // 結合条件を満たすか確認
        // 条件１：両方の顧客がそれぞれ独立したルートに含まれている．
        if (ri != -1 && rj != -1 && ri != rj) {
            int combined_weight = routes[ri].total_weight + routes[rj].total_weight;
            // 条件２：結合した時に 積載量上限W を超えない．
            if (combined_weight <= W) { // 積載量上限を超えないなら統合
                /*
                rj(0 -> j1 ... jn -> 0)をri(0 -> i1 ... in -> 0)に統合する．
                1.  ri の終点0を削除
                2.  rj の終わりに rjのルート（始点と終点以外） を追加していく．
                3.  最後に 終点0 を追加
                4.  ri の総コストを更新
                5.  rj を削除
                */
                Route *a = &routes[ri];
                Route *b = &routes[rj];

                // 1
                a->length--;

                // 2
                for (int k = 1; k < b->length - 1; k++) {
                    a->nodes[a->length] = b->nodes[k];
                    a->length++;
                }

                // 3
                a->nodes[a->length] = 0;
                a->length++;

                // 4
                a->total_weight = combined_weight;

                // rj を削除
                for (int k = rj; k < route_count - 1; k++) {
                    routes[k] = routes[k + 1];
                }
                route_count--; // ルート数を1減らす
            }
        }
    }
}

// 結果の出力（ルートと総コスト）
void print_result() {
    int total_cost = 0;
    printf("配送ルート:\n");
    for (int r = 0; r < route_count; r++) {
        int cost = 0;
        printf("ルート%d: ", r + 1);
        for (int i = 0; i < routes[r].length; i++) {
            int node = routes[r].nodes[i];
            printf("%d ", node);
            if (i < routes[r].length - 1) {
                cost += c[routes[r].nodes[i]][routes[r].nodes[i + 1]];
            }
        }
        total_cost += cost;
        printf("(コスト: %d, 重さ: %d)\n", cost, routes[r].total_weight);
    }
    printf("総配送コスト: %d\n", total_cost);
}

int main() {
    // 手入力をする場合
    // int n;
    // scanf("%d", &n);
    
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         if (i == j) c[i][j] = 0;
    //         else if (i < j) {
    //             int cost;
    //             scanf("%d", &cost);
    //             c[i][j] = c[j][i] = cost;
    //         }
    //     }
    // }

    // w[0] = 0;
    // for (int i = 1; i <= n; i++) {
    //     int weight;
    //     scanf("%d", &weight);
    //     w[i] = weight;
    // }

    // // ルートの初期設定
    // for (int i = 1; i <= n; i++) {
    //     routes[route_count].nodes[0] = 0;
    //     routes[route_count].nodes[1] = i;
    //     routes[route_count].length = 2;
    //     routes[route_count].total_weight = w[i];
    //     route_count++;
    // }

    // ----------------------------------------------

    // 初期化
    initialize();

    // 処理前を出力
    print_result();

    // セービング法
    calculate_savings(); 
    merge_routes();

    // 処理後を出力
    print_result();

    return 0;
}