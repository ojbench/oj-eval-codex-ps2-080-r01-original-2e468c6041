#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> head(n + 1, -1);
    vector<int> to(2 * m), nxt(2 * m), eid(2 * m);
    int idx = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        to[idx] = v; eid[idx] = i; nxt[idx] = head[u]; head[u] = idx++;
        to[idx] = u; eid[idx] = i; nxt[idx] = head[v]; head[v] = idx++;
    }

    vector<int> dfn(n + 1, 0), low(n + 1, 0), parent(n + 1, 0), parentEdge(n + 1, -1), it(n + 1, -1);
    vector<int> st;
    st.reserve(n);
    vector<int> bccVertices;
    bccVertices.reserve(n);
    vector<int> edgeStack;
    edgeStack.reserve(m);
    vector<char> ans(n + 1, 0);
    vector<int> compColor(n + 1, 0);
    int timer = 0;

    for (int s = 1; s <= n; ++s) {
        if (dfn[s]) continue;
        st.push_back(s);
        parent[s] = 0;
        parentEdge[s] = -1;
        while (!st.empty()) {
            int u = st.back();
            if (!dfn[u]) {
                dfn[u] = low[u] = ++timer;
                it[u] = head[u];
            }
            bool advanced = false;
            for (int &e = it[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (e == parentEdge[u]) continue;
                if (!dfn[v]) {
                    parent[v] = u;
                    parentEdge[v] = e ^ 1;
                    edgeStack.push_back(e);
                    st.push_back(v);
                    advanced = true;
                    break;
                }
                if (dfn[v] < dfn[u]) {
                    edgeStack.push_back(e);
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (advanced) continue;

            st.pop_back();
            int p = parent[u];
            if (p) {
                low[p] = min(low[p], low[u]);
                if (low[u] >= dfn[p]) {
                    bccVertices.clear();
                    vector<pair<int, int>> bccEdges;
                    int x;
                    do {
                        x = edgeStack.back();
                        edgeStack.pop_back();
                        int a = to[x ^ 1];
                        int b = to[x];
                        bccVertices.push_back(a);
                        bccVertices.push_back(b);
                        bccEdges.emplace_back(a, b);
                    } while (!(to[x ^ 1] == p && to[x] == u));

                    sort(bccVertices.begin(), bccVertices.end());
                    bccVertices.erase(unique(bccVertices.begin(), bccVertices.end()), bccVertices.end());
                    if ((int)bccVertices.size() >= 3) {
                        unordered_map<int, int> localId;
                        localId.reserve(bccVertices.size() * 2 + 1);
                        for (int i = 0; i < (int)bccVertices.size(); ++i) {
                            localId[bccVertices[i]] = i;
                        }
                        vector<vector<int>> localGraph(bccVertices.size());
                        for (auto [a, b] : bccEdges) {
                            int ia = localId[a];
                            int ib = localId[b];
                            localGraph[ia].push_back(ib);
                            localGraph[ib].push_back(ia);
                        }
                        queue<int> q;
                        for (int v : bccVertices) compColor[v] = -1;
                        compColor[bccVertices[0]] = 0;
                        q.push(0);
                        bool bip = true;
                        while (!q.empty() && bip) {
                            int cur = q.front();
                            q.pop();
                            for (int nxtVertex : localGraph[cur]) {
                                int realVertex = bccVertices[nxtVertex];
                                int curReal = bccVertices[cur];
                                if (compColor[realVertex] == -1) {
                                    compColor[realVertex] = compColor[curReal] ^ 1;
                                    q.push(nxtVertex);
                                } else if (compColor[realVertex] == compColor[curReal]) {
                                    bip = false;
                                    break;
                                }
                            }
                        }
                        if (!bip) {
                            for (int v : bccVertices) ans[v] = 1;
                        }
                    }
                }
            }
        }
    }

    int res = 0;
    for (int i = 1; i <= n; ++i) res += !ans[i];
    cout << res << '\n';
    return 0;
}
