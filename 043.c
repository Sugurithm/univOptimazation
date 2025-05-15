#include <stdio.h>

#define MAX 100

typedef struct {
    int index; // 元のアイテムのインデックス ( アルゴリズム内はratio で判断するため )
    int size; // サイズ
    int value; // 価値
    double ratio; // 価値/サイズ
} Item;

void swap(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// ボブルソート サイズあたりの価値の高い順に並べ替え
void bubbleSort(Item items[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (items[j].ratio < items[j + 1].ratio) {
                swap(&items[j], &items[j + 1]);
            }
        }
    }
}

int main(void) {
    int n, b;
    Item items[MAX]; // アイテムを保存する配列
    int z[MAX] = {0}; // items[i] を入れるかどうかを保存する配列

    printf("アイテムの数 ナップザックの容量\n");
    scanf("%d %d", &n, &b);

    printf("サイズ 価値\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &items[i].size, &items[i].value);
        items[i].ratio = (double)items[i].value / items[i].size;
        items[i].index = i;
    }

    bubbleSort(items, n);

    // グリーディ法
    int capacity = b;
    for (int i = 0; i < n; i++) {
        // 容量が余っているなら，単価が高い順に入れる
        if (items[i].size <= capacity) {
            z[items[i].index] = 1;
            capacity -= items[i].size;
        } else {
            z[items[i].index] = 0;
        }
    }

    // 結果出力
    for (int i = 0; i < n; i++) {
        printf("item %d : %d\n", i + 1, z[i]);
    }
    printf("\n");
}