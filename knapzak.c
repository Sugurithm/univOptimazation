#include <stdio.h>

#define MAX_N 100
#define MAX_CAPACITY 100

typedef struct {
    int value;
    int weight;
} Item;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapSack(int itemCnt, int capacity, Item items[]) {
    int dp[itemCnt + 1][capacity + 1];

    // Initialize
    for (int i = 0; i <= itemCnt; i++) {
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = 0;
        }
    }

    // DP fill
    for (int i = 0; i < itemCnt; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i].weight <= w) {
                dp[i + 1][w] = max(dp[i][w], dp[i][w - items[i].weight] + items[i].value);
            } else {
                dp[i + 1][w] = dp[i][w];
            }
        }
    }

    return dp[itemCnt][capacity];
}

int main(void) {
    int capacity, n;
    scanf("%d", &capacity);
    scanf("%d", &n);

    Item items[MAX_N];
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &items[i].value, &items[i].weight);
    }

    int maxValue = knapSack(n, capacity, items);
    printf("%d\n", maxValue);

    return 0;
}
