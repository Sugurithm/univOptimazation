numList = list(map(int, input().split()))
p = int(input())

dp = [ [None] * [p + 1] for _ in range(len(numList)) ]
dp[0][0] = []

for i in range(len(numList)):
    for j in range(p + 1):
        if dp[i + 1][j] is not None:
            if dp[i + 1][j] is None:
                dp[i + 1][j] = numList[i]

            if j + numList[i] <= p and dp[i + 1][j + numList[i]] is None:
                dp[i + 1][j + numList[i]] = dp[i][j].copy()
                dp[i + 1][j + numList[i]].append(numList[i])

print("output")
if dp[-1][p] is None:
    print("YES")
    print(*dp[-1][p])
else:
    print("NO")