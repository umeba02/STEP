#include <bits/stdc++.h>
using namespace std;
// solver_greedy.pyをC++で書いてみる

// 2-optsを限界までまわす
// 更新されたかどうかを毎回保存しておく
// 3optsも限界までまわす
// やきなまし法をやってみる

// 入力ファイルを読んで、各町の座標のvectorを返す関数
vector<pair<double, double>> read_input(string filename)
{
    // inputファイル読む
    ifstream input_file(filename);
    // 街の座標を格納するvector
    vector<pair<double, double>> cities = {};

    string line, str;
    double x, y;
    bool is_x = true; // x座標かどうかを判定するbool
    // 1行ずつ読む
    while (getline(input_file, line))
    {
        if (line == "x,y" || line == "")
        { // 1行目と最後は無視
            continue;
        }
        // sstreamに変換してもう一回getlineが使えるようにする
        stringstream line_ss{line};

        // 各行をx座標とy座標に分ける
        while (getline(line_ss, str, ','))
        {
            if (is_x)
            { // x座標を読み込む
                x = stod(str);
                is_x = false;
            }
            else
            { // y座標を読み込む
                y = stod(str);
                is_x = true;
            }
        }
        // 座標をpush
        cities.push_back(make_pair(x, y));
    }

    // for (int i = 0; i < cities.size(); i++)
    // {
    //     cout << cities[i].first<<" "<< cities[i].second<< endl;
    // }
    return cities;
}

// 経路をファイルに書き込む
void print_tour(vector<int> tour, string filename)
{
    ofstream output_file(filename);
    output_file << "index" << endl;
    for (int i = 0; i < tour.size(); i++)
    {
        output_file << tour[i] << endl;
    }
}

// 二点間の距離を返す関数
double distance(pair<double, double> city1, pair<double, double> city2)
{
    return sqrt(pow((city1.first - city2.first), 2) + pow((city1.second - city2.second), 2));
}

// 辺を入れ替える
vector<int> swap_edges(vector<int> tour, int a, int b, int c, int d)
{
    // abを入れ替えればOK？
    stack<int> stack;
    for (int i = 0; i < tour.size(); i++)
    {
        if (i >= c && i <= b)
        {
            stack.push(i);
        }
    }

    for (int i = 0; i < tour.size(); i++)
    {
        if (i >= c && i <= b)
        {
            tour[i] = stack.top();
            stack.pop();
        }
    }

    return tour;
}

// 2-opt
vector<int> two_opt(vector<int> tour, vector<vector<double>> dist)
{
    // 経路のサイズ(頂点数)
    int N = tour.size();
    // 経路が更新されたかどうか
    bool is_update = false;
    // とりあえず100回まわす
    int turns = 100;

    // 全頂点を順に見る
    for (int count = 0; count < turns; count++)
    {
        cout << "2-opt count:" << count << endl;
        for (int i = 0; i < N - 2; i++)
        {
            for (int j = i + 2; j < N; j++)
            {
                int a = i;
                int b = j;

                int c = (a + 1) % N;
                int d = (b + 1) % N;

                if (c == d)
                {
                    continue;
                }

                // 交換した方が短くなるのであれば交換する
                if (dist[tour[a]][tour[b]] + dist[tour[c]][tour[d]] < dist[tour[a]][tour[c]] + dist[tour[b]][tour[d]])
                {
                    // tour = swap_edges(tour, a, b, c, d);
                    reverse(tour.begin() + c, tour.begin() + d);
                    is_update = true;
                }
            }
        }

        if (!is_update)
        {
            break;
        }
        else
        {
            is_update = false;
        }
    }

    return tour;
}

// 二点間の差を返してくれる関数
pair<double, double> diff_pair(pair<int, int> p1, pair<int, int> p2)
{
    int a = p1.first - p2.first;
    int b = p1.second - p2.second;
    return make_pair(a, b);
}

// グラハムスキャン法で初期経路を生成
vector<int> graham_scan(vector<pair<double, double>> cities, vector<vector<double>> dist)
{
    // 初期位置を決める
    // y座標が最も大きく、その中で最もx座標が大きい頂点を初期位置にする
    int start = 0;
    for (int i = 1; i < cities.size(); i++)
    {
        if (cities[start].second < cities[i].second)
        {
            start = i;
        }
        else if (cities[start].second == cities[i].second && cities[start].first < cities[i].first)
        {
            start = i;
        }
    }

    // 経路
    vector<int> tour = {};

    // 起点から見て反時計回りになるような点のvector
    vector<pair<int, double>> counter_clockwise;
    // スタート地点を入れる
    counter_clockwise.push_back(make_pair(start, 0));

    // 角度を計算して格納する
    for (int i = 0; i < cities.size(); i++)
    {
        if (i != start)
        {
            // arctan、y、x
            double theta = atan2(cities[i].second - cities[start].second, cities[i].first - cities[start].first);
            counter_clockwise.push_back(make_pair(i, theta));
        }
    }

    // 点のvectorを反時計回りでソート
    // arctanが大きい順にならべる
    sort(counter_clockwise.begin() + 1, counter_clockwise.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         { return a.second > b.second; });

    // 経路を保存しておくdequeに最初の3点を入れる
    deque<int> deque_tour;
    deque_tour.push_back(counter_clockwise[0].first);
    deque_tour.push_back(counter_clockwise[1].first);
    deque_tour.push_back(counter_clockwise[2].first);

    // 残りの点は未到達の点一覧に入れる
    deque<int> deque_p;
    for (int i = 3; i < counter_clockwise.size(); i++)
    {
        deque_p.push_back(counter_clockwise[i].first);
    }

    // 経路の二番目を保持しておく
    int second = counter_clockwise[1].first;

    while (!deque_p.empty())
    {
        // 経路の2番目と1番目、piのなす角が180度以上(外積が負)なら
        pair<double, double> v1 = diff_pair(cities[deque_tour.back()], cities[second]);
        pair<double, double> v2 = diff_pair(cities[deque_p.front()], cities[second]);
        if (v1.first * v2.second - v1.second * v2.first < 0)
        { // 180度以上(外積が負)のとき
            // 二番目を更新
            second = deque_tour.back();
            // push
            deque_tour.push_back(deque_p.front());
            // 未到達点から削除
            deque_p.pop_front();
        }
        else
        { // 180度未満のとき
            // deque_tourの末尾を捨てる
            deque_tour.pop_back();

            // 二番目を更新
            int tmp = deque_tour.back();
            deque_tour.pop_back();
            second = deque_tour.back();
            deque_tour.push_back(tmp);
        }
    }
    int N = deque_tour.size();
    for (int i = 0; i < N; i++)
    {
        tour.push_back(deque_tour.front());
        deque_tour.pop_front();
    }

    // 経路を返す
    return tour;
}

double tri_length(double p1, double p2, double pnew, vector<vector<double>> dist)
{
    return dist[p1][pnew] + dist[p2][pnew] - dist[p1][p2];
}

// 挿入法
vector<int> insert(vector<pair<double, double>> cities, vector<vector<double>> dist, vector<int> tour)
{
    // 未到達の点をまとめる
    vector<int> unvisited = {};
    for (int i = 0; i < cities.size(); i++)
    {
        if (find(tour.begin(), tour.end(), i) == tour.end())
        {
            unvisited.push_back(i);
        }
    }
    cout << "create unvisited list:done" << endl;

    // 凸包の点と未到達の点を1つずつ比較する
    double min_length = dist[tour[0]][unvisited[0]] + dist[tour[1]][unvisited[0]] - dist[tour[0]][tour[1]];
    double L;

    // 挿入する場所、挿入する頂点のペアを保持
    pair<int, int> best_node;
    // 初期化
    best_node.first = 0;
    best_node.second = unvisited[0];

    // i番目の辺のj番目の点との距離を保存しておこう
    vector<vector<double>> lengths(tour.size(), vector<double>());
    for (int i = 0; i < tour.size(); i++)
    {
        cout << "i:" << i << endl;
        for (int j = 0; j < unvisited.size(); j++)
        {
            if (i == tour.size() - 1)
            {
                L = tri_length(tour[i], tour[0], unvisited[j], dist);
            }
            else
            {
                L = tri_length(tour[i], tour[i + 1], unvisited[j], dist);
            }
            lengths[i].push_back(L);
        }
    }
    cout << "create length list:done" << endl;

    while (!unvisited.empty())
    {
        cout << "unvisited size:" << unvisited.size() << endl;
        // いいてんをさがす
        for (int i = 0; i < tour.size(); i++)
        {
            for (int j = 0; j < unvisited.size(); j++)
            {
                L = lengths[i][j];

                if (L < min_length)
                {
                    min_length = L;
                    best_node.first = i;
                    best_node.second = j;
                }
            }
        }
        cout << "done:" << endl;

        // 経路にいれる
        tour.insert(tour.begin() + best_node.first, unvisited[best_node.second]);

        // 未到達の点集合から削除する
        unvisited.erase(unvisited.begin() + best_node.second);
        if (unvisited.empty())
        {
            break;
        }

        for (int i = 0; i < lengths.size(); i++)
        {
            lengths[i].erase(lengths[i].begin() + best_node.second);
        }

        // 新しい点との距離
        vector<double> new_vec;
        // あたらしい辺のインデックス
        int new_index;
        if (best_node.first == 0)
        {
            new_index = tour.size() - 1;
        }
        else
        {
            new_index = best_node.first - 1;
        }
        // あたらしい辺との距離のvectorつくる
        for (int j = 0; j < unvisited.size(); j++)
        {
            L = tri_length(tour[new_index], tour[best_node.first], unvisited[j], dist);
            new_vec.push_back(L);
        }

        // 距離一覧のvecにいれる
        lengths.insert(lengths.begin() + new_index, new_vec);

        // あたらしい辺2でもおなじことをやる
        // まず初期化
        new_vec.clear();
        if (best_node.first == tour.size())
        {
            new_index = 0;
        }
        else
        {
            new_index = best_node.first + 1;
        }

        // あたらしい辺との距離のvectorつくる
        for (int j = 0; j < unvisited.size(); j++)
        {
            L = tri_length(tour[best_node.first], tour[new_index], unvisited[j], dist);
            new_vec.push_back(L);
        }

        // 距離一覧のvecにいれる
        lengths.insert(lengths.begin() + new_index, new_vec);

        // 初期化
        min_length = dist[tour[0]][unvisited[0]] + dist[tour[1]][unvisited[0]] - dist[tour[0]][tour[1]];
        best_node.first = 0;
        best_node.second = unvisited[0];
    }
    return tour;
}

vector<int> solve(vector<pair<double, double>> cities)
{
    // まちの数を取得
    int N = cities.size();
    // 多分N×Nのlistを作っている
    //(pythonのlistの初期化がいまいちよくわからない)
    vector<vector<double>> dist(N, vector<double>(N, 0));

    // 各2点間の距離を取得し、格納する
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // 2点間の距離を取得
            dist[i][j] = distance(cities[i], cities[j]);
            dist[j][i] = dist[i][j];
        }
    }

    // グラハムスキャン法で凸包を生成
    vector<int> tour = graham_scan(cities, dist);
    cout << "graham_scan done" << endl;
    // 挿入法でなんかどうにかする
    tour = insert(cities, dist, tour);
    cout << "insert done" << endl;

    // // 現在地
    // int current_city = 0;
    // // スタート地点
    // int start_city = 0;

    // // 未到達のまちを保存しておく
    // set<int> unvisited_cities;
    // // 1以上N未満の整数をいれる
    // for (int i = 1; i < N; i++)
    // {
    //     unvisited_cities.insert(i);
    // }

    // while (!unvisited_cities.empty())
    // {
    //     // minつかって比較したいので一旦配列にさせてね
    //     vector<int> vec(unvisited_cities.begin(), unvisited_cities.end());
    //     int next_city = *min_element(vec.begin(), vec.end(), [&](int city1, int city2)
    //                                  { return dist[current_city][city1] < dist[current_city][city2]; });
    //     // 消す
    //     unvisited_cities.erase(next_city);
    //     // 足す
    //     tour.push_back(next_city);
    //     // 現在地かえる
    //     current_city = next_city;
    // }

    // 末尾にスタート地点を入れる
    // こうすることでパスだったものがサイクルになり、
    // 後処理をするときに楽になる(はず)
    //.push_back(start_city);

    // このあたりで初期経路を変えたい
    // 2-opts
    tour = two_opt(tour, dist);

    return tour;
}

int main(int argc, char *argv[])
{
    // 引数が2つより大きいか確認
    assert(argc > 2);

    vector<int> tour = solve(read_input(argv[1]));
    print_tour(tour, argv[2]);

    return 0;
}