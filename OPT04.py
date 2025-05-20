# 変種距離 Levenshtein Distance

S = "kitten"
T = "sitting"

# dp テーブル
dp = [ [0] * len(S) + 1 for _ in range(len(T) + 1)]

# 初期化（空文文字との比較）
for i in range(len(T) + 1):
    dp[i][0] = i
for j in range(len(S) + 1):
    dp[0][j] = j

# 漸化式に基づいてdpテーブルを埋める
for i in range(1, len(T) + 1):
    for j in range(1 , len(S) + 1):
        if T[i - 1] == S[j - 1]:
            dp[i][j] = min(
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] # 一致
            )
        else:
            dp[i][j] = min(
                dp[i - 1][j] + 1,       # 削除
                dp[i][j - 1] + 1,       # 挿入
                dp[i  - 1][j - 1] + 1   # 置換
            )

# 最小編集距離
print(dp[len(T)][len(S)])