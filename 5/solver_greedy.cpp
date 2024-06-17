#include <bits/stdc++.h>
using namespace std;
// solver_greedy.pyをC++で書いてみる

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

//辺を入れ替える
vector<int> swap_edges(vector<int> tour,int a,int b,int c,int d){
    int x=d;
    int y=b;
    int N=tour.size();
    if(y-x>a + N-c){
        x=c;
        y=a+N;
    }
    while(x<y){
        swap(tour[x<N?x:x-N],tour[y<N?y:y-N]);
        x++;
        y--;
    }
    return tour;
}

//2-opt
//参考；https://github.com/firewood/topcoder/blob/master/algorithm/two_opt.cpp
//引用元はドキュメントにあとでちゃんとかきます...ごめんなさい.....................................
vector<int> two_opt(vector<int> tour, vector<vector<double>> dist)
{
    
    int N = tour.size();
    int turns=N*N;

    //乱数生成のための準備
    random_device seed;
    mt19937 rand(seed());
    
    for(int i=0;i<turns;i++){
        int a=rand()%N;
        int b=rand()%N;
        if(a==b){//同じ点だったらスキップ
            continue;
        }

        if(a>b){
            //bの方が大きい状態にしておく
            swap(a,b);
        }

        int c=(b+1)%N;
        int d=(a+1)%N;
        if(dist[tour[a]][tour[b]]+dist[tour[c]][tour[d]]<dist[tour[a]][tour[d]]+dist[tour[b]][tour[c]]){
            tour=swap_edges(tour,a,b,c,d);
        }
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
        for (int j = i; j < N; j++)
        {
            // 2点間の距離を取得
            dist[i][j] = distance(cities[i], cities[j]);
            dist[j][i] = dist[i][j];
        }
    }

    // 現在地
    int current_city = 0;
    // スタート地点
    int start_city = 0;

    // 未到達のまちを保存しておく
    set<int> unvisited_cities;
    // 1以上N未満の整数をいれる
    for (int i = 1; i < N; i++)
    {
        unvisited_cities.insert(i);
    }

    // 道のりを表している
    vector<int> tour = {current_city};

    while (!unvisited_cities.empty())
    {
        // minつかって比較したいので一旦配列にさせてね
        vector<int> vec(unvisited_cities.begin(), unvisited_cities.end());
        int next_city = *min_element(vec.begin(), vec.end(), [&](int city1, int city2)
                                     { return dist[current_city][city1] < dist[current_city][city2]; });
        // 消す
        unvisited_cities.erase(next_city);
        // 足す
        tour.push_back(next_city);
        // 現在地かえる
        current_city = next_city;
    }

    // 末尾にスタート地点を入れる
    // こうすることでパスだったものがサイクルになり、
    // 後処理をするときに楽になる(はず)
    //.push_back(start_city);

    // このあたりで初期経路を変えたい
    tour=two_opt(tour,dist);

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