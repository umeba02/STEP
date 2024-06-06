#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;

// ソートの比較関数
// タイトルの長さ降順
bool comp_title_length(string s1, string s2)
{
    return (s1.length() > s2.length());
}

// ソートの比較関数
// ランクの高さ降順
bool comp_page_rank(pair<int, double> pair1, pair<int, double> pair2)
{
    return (pair1.second > pair2.second);
}

struct Wikipedia
{
    // IDでタイトルにアクセスできるvector
    vector<string> titles = {};
    // タイトルでIDにアクセスできるmap
    map<string, int> title_to_id;
    // IDで「そのIDの記事にリンクされている記事のID」のvectorにアクセスできるvector
    vector<vector<int>> links = {};
    // IDでページが存在するか確認できるvector
    vector<bool> is_valid_id = {};
    // ページ数
    int page_count = 0;

    // デバッグ用の関数
    // titlesとlinksの中身を全部プリントする
    void print_titles_and_links()
    {
        cout << "------------titles------------" << endl;
        for (int i = 0; i < titles.size(); i++)
        {
            cout << "i:" << i << " titles:" << titles[i] << endl;
        }

        cout << "------------links------------" << endl;
        for (int i = 0; i < links.size(); i++)
        {
            cout << "i:" << i << " { ";
            for (int j = 0; j < links[i].size(); j++)
            {
                cout << links[i][j];
                if (j != links[i].size() - 1)
                {
                    cout << ", ";
                }
            }
            cout << " }" << endl;
        }
        cout << "------------end------------" << endl;
    }

    Wikipedia(string pages_file_name, string links_file_name)
    {
        ifstream pages_file(pages_file_name); // ページファイル読む
        ifstream links_file(links_file_name); // リンクファイル読む
        string line, str;
        int id = -1, src = -1, dst;
        string title;

        // ページファイルを1行ずつ読む
        while (getline(pages_file, line))
        {
            // C++では文字列のsplitがないので...
            // sstreamに変換してもう一回getlineが使えるようにする
            stringstream line_ss{line};

            // 各行をIDとタイトルに切り分ける
            while (getline(line_ss, str, ' '))
            {
                if (id == -1)
                { // IDが初期値であれば
                    // IDを設定
                    id = stoi(str);
                }
                else
                { // IDが設定済みであれば
                    // タイトルを設定
                    title = str;
                }
            }

            // ID、タイトルのペアを作る
            pair<int, string> page_info = make_pair(id, title);

            // 【エラーが起こったので一旦コメントアウト】
            // IDがタイトルのvectorに含まれないかチェック(含まれなければOK)
            // stringのvectorでintを探すのが変な気もする？
            // assert(find(titles.begin(),titles.end(),id)==titles.end());

            // vectorの大きさよりidがでかかったら拡張していく
            if (titles.size() <= id)
            {
                titles.resize(id + 1);
            }

            if (links.size() <= id)
            {
                links.resize(id + 1);
            }

            if (is_valid_id.size() <= id)
            {
                is_valid_id.resize(id + 1);
            }

            // タイトルとリンク先を格納
            titles[id] = title;
            links[id] = {};

            // 「ページが紐づいているID」として記憶
            is_valid_id[id] = true;

            // マップに格納
            title_to_id[title] = id;

            // ページ数のカウントを増やす
            page_count++;

            // IDを初期値に戻す
            id = -1;
        }

        cout << "Finished reading " << pages_file_name << endl;
        // print_titles_and_links();

        // リンクファイルを一行ずつ読む
        while (getline(links_file, line))
        {
            // sstreamに変換してもう一回getlineが使えるようにする
            stringstream line_ss{line};

            // 各行をsrcとdstに分ける
            while (getline(line_ss, str, ' '))
            {
                if (src == -1)
                { // srcが未設定であれば
                    // srcを設定
                    src = stoi(str);
                }
                else
                { // srcが設定済みであれば
                    // dstを設定
                    dst = stoi(str);
                }
            }

            // srcのリンク先vectorの一番後ろにdstを追加
            links[src].push_back(dst);

            // srcを初期値に戻す
            src = -1;
        }
        cout << "Finished reading " << links_file_name << endl;
        // print_titles_and_links();
    }

    // いちばん長いタイトルを見つけて出力する関数
    void find_longest_titles()
    {
        // 文字数降順にソートされた新たなvectorを返したい
        // C++のsortは元vectorを変更してしまうので、コピー作ってsortする

        // ソート用にtitlesのコピーを作る
        vector<string> new_titles = {};
        copy(titles.begin(), titles.end(), back_inserter(new_titles));

        // コピーされたvectorを文字数降順にソートする
        sort(new_titles.begin(), new_titles.end(), comp_title_length);

        cout << "The longest titles are:" << endl;

        int count = 0;
        int index = 0;

        // 最大15個まで、長いタイトルを出力する
        while (count < 15 && index < new_titles.size())
        {
            //_が含まれていなければ出力？
            if (new_titles[index].find("_") == string::npos)
            {
                cout << new_titles[index] << endl;
                count++;
            }
            index++;
        }
    }

    // 最も被リンク数の多いページを見つけて出力する関数
    void find_most_linked_pages()
    {
        // リンク数をカウントするvector
        vector<int> link_count(titles.size());
        // 各リンク先を見ていく
        for (int i = 0; i < titles.size(); i++)
        {
            for (int j = 0; j < links[i].size(); j++)
            {
                // vectorの大きさがidよりでかかったら拡張していく
                if (link_count.size() <= links[i][j])
                {
                    link_count.resize(links[i][j]);
                }
                // リンク数のカウントを増やす
                link_count[links[i][j]]++;
            }
        }

        cout << "The most linked pages are:" << endl;
        // 最大値を取得
        int link_count_max = *max_element(link_count.begin(), link_count.end());

        for (int i = 0; i < link_count.size(); i++)
        {
            if (link_count[i] == link_count_max)
            {
                cout << titles[i] << " , " << link_count_max << endl;
            }
        }
    }

    // 最も短いパスを見つける関数
    // 今は距離だけが返るようになってるけど、経路も返ってきた方がいい？
    vector<string> find_shortest_path(string start, string goal)
    {
        // 最短経路を保持しておけばよさそう
        // とりあえずふつうにBFS書いてみる

        // IDとそこまでの最短距離を入れるキューを用意する
        queue<pair<int, int>> queue;

        map<int, vector<string>> path;

        // 到達済みのIDをTrue、それ以外をfalseで持っておく
        vector<bool> visited(titles.size());

        // スタート地点、ゴール地点のIDを取得
        int start_id = title_to_id[start];
        int goal_id = title_to_id[goal];

        // スタート時点を到達済みにしておく
        visited[start_id] = true;
        queue.push(make_pair(start_id, 0));
        path[start_id].push_back(titles[start_id]);

        // スタート地点から今いる点までの最短距離
        int dis = 0;

        // キューがなくなるまで回す
        while (!queue.empty())
        {
            // 先頭要素を見る
            int node = queue.front().first;

            if (node == goal_id)
            { // ゴールについたら
                // cout << queue.front().second << endl;
                return path[node]; // 経路を返す
            }

            // 今いる点の最短距離を更新
            //  if(queue.front().second<=dis){
            //      dis=queue.front().second;
            //  }

            // キューから取り出す
            queue.pop();
            dis++;
            // 今のノードに接続されているノードをひとつずつ見る
            for (int i = 0; i < links[node].size(); i++)
            {
                int child = links[node][i];
                // 未訪問であれば
                if (!visited[child])
                {
                    // 訪問済みに更新
                    visited[child] = true;

                    // キューに追加
                    queue.push(make_pair(child, dis));
                    path[child] = path[node];
                    path[child].push_back(titles[child]);
                }
            }
        }
        return {};
    }

    // ページランクの計算結果を返してくれる関数
    vector<double> calculate_page_rank()
    {
        // map使ってたけど遅すぎるのでvectorに戻します
        //  map<int, double> page_rank;
        //  map<int, double> new_page_rank;

        int max_id = titles.size();

        vector<double> page_rank(max_id);
        vector<double> new_page_rank(max_id);

        // ランクの初期化
        for (int id = 0; id < max_id; id++)
        {
            // タイトルと紐づいているIDであれば
            if (is_valid_id[id])
            {
                page_rank[id] = 1;
                new_page_rank[id] = 0;
            }
            else
            {
                page_rank[id] = -1;
                new_page_rank[id] = -1;
            }
        }

        double all_vertex_rank = 0;

        // ページランクを更新していく
        while (1)
        {
            // page_rankをひとつずつ見ていく
            for (int id = 0; id < max_id; id++)
            {
                // IDと紐づくページがなければスキップ
                if (!is_valid_id[id])
                {
                    continue;
                }
                // cout << "links[id].size():" << links[id].size() << endl;

                // 隣接ノードがある場合
                if (links[id].size() != 0)
                {
                    double adjacent_rank = 0.85 * page_rank[id] / links[id].size();

                    // 隣接頂点をひとつずつ見ていく
                    for (int i = 0; i < links[id].size(); i++)
                    {
                        // ページランクを更新する
                        new_page_rank[links[id][i]] += adjacent_rank;
                    }

                    // 全頂点にページランクを分配する
                    all_vertex_rank += 0.15 * page_rank[id] / page_count;
                }
                else
                { // 隣接ノードがない場合
                    all_vertex_rank += page_rank[id] / page_count;
                }
            }

            // 全頂点をひとつずつ見ていく
            for (int id = 0; id < max_id; id++)
            {
                // IDと紐づくページがなければスキップ
                if (!is_valid_id[id])
                {
                    continue;
                }
                // ページランクを更新する
                new_page_rank[id] += all_vertex_rank;
            }

            // 前のランクとの差を見ていく
            double sum = 0;

            // ページランクをひとつずつ見ていく
            for (int id = 0; id < max_id; id++)
            {
                // IDと紐づくページがなければスキップ
                if (!is_valid_id[id])
                {
                    continue;
                }

                sum += pow(new_page_rank[id] - page_rank[id], 2);
            }

            // 収束していたら終了
            if (sum < 0.01)
            {
                break;
            }
            else
            { // 収束していなければ
                // リセット
                for (int id = 0; id < max_id; id++)
                {
                    // IDと紐づくページがなければスキップ
                    if (!is_valid_id[id])
                    {
                        continue;
                    }

                    page_rank[id] = new_page_rank[id];
                    new_page_rank[id] = 0;
                    all_vertex_rank = 0;
                }
            }
        }

        // ページランクを返す
        return new_page_rank;
    }

    // ページランクの高い順にソートされたvectorを返す関数
    vector<pair<int, double>> sort_page_rank(vector<double> vec)
    {
        vector<pair<int, double>> sorted_by_rank = {};
        for (int i = 0; i < vec.size(); i++)
        {
            if (!is_valid_id[i])
            {
                continue;
            }

            sorted_by_rank.push_back(make_pair(i, vec[i]));
        }
        // ランクの高さ順にソート
        sort(sorted_by_rank.begin(), sorted_by_rank.end(), comp_page_rank);

        return sorted_by_rank;
    }

    // 最も重要度の高いページを求める関数
    void find_most_popular_pages()
    {
        // ページランクを取得
        vector<double> page_rank = calculate_page_rank();
        // 余分な要素を削除してソートする
        vector<pair<int, double>> sorted_by_rank = sort_page_rank(page_rank);

        cout << "The most popular pages are:" << endl;
        for (int i = 0; i < min(10, int(sorted_by_rank.size())); i++)
        {
            cout << titles[sorted_by_rank[i].first] << " " << sorted_by_rank[i].second << endl;
        }
    }
};

int main()
{
    Wikipedia *wikipedia = new Wikipedia("pages_medium.txt", "links_medium.txt");
    wikipedia->find_longest_titles();
    wikipedia->find_most_linked_pages();
    vector<string> result = wikipedia->find_shortest_path("サーターアンダギー", "東方紅魔郷");
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    wikipedia->find_most_popular_pages();
}