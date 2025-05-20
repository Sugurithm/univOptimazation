def lcs(S, T): 
    dp = [ [0] * (len(T) + 1) for _ in range(len(S) + 1)]

    for i in range(1, len(S) + 1):
        for j in range(1, len(T) + 1):
            if S[i - 1] == T[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    return dp[len(S)][len(T)]

if __name__ == "__main__":
    print("---INPUT---")
    S = input()
    T = input()

    lcs_length = lcs(S, T)

    print("---OUTPUT---")
    print(lcs(S, T))