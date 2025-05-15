

capacity = 14
value = [22, 36, 58, 41, 33, 27, 19, 45]
weight = [1, 2, 4, 3, 2, 1, 5, 4]
n = len(weight)

# 各セルに (価値の総和, 選択されたアイテムのリスト) を格納
dp = [[0] * (capacity + 1) for _ in range(n + 1)]

# knapSack DP
# 価値を最大化するためのDPテーブルを作成
for i in range(0, n):
    for w in range(0, capacity):
        # アイテムを選ぶ場合
        if weight[i] <= w:
            dp[i + 1][w] = max(dp[i][w], dp[i][w - weight[i]] + value[i])
        else:
            dp[i + 1][w] = dp[i][w]

