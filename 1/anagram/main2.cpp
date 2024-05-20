#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;

// スコアの配列
int scores[26] = {1, 3, 2, 2, 1, 3, 3, 1, 1, 4, 4, 2, 2, 1, 1, 3, 4, 1, 1, 1, 2, 3, 3, 4, 3, 4}; 

//スコアを計算する関数
// 入力：文字列
// 出力：文字列のスコア
int calc_score(string S)
{
    int score = 0;
    for (int i = 0; i < S.length(); i++)
    {
        score+=scores[S[i] - 'a'];
    }
    return score;
}

//スコア順(降順)にソートするための関数
auto sort_score = [](string s1, string s2) -> bool
{
    return calc_score(s1) > calc_score(s2);
};

int main(int argc, char *argv[])
{

    ifstream input_file(argv[1]);     // 文字列ファイル読む
    ifstream words_file("words.txt"); // 辞書ファイル読む
    ofstream output_file(argv[2]);    // 出力ファイル指定
    vector<string> words_vec;         // 辞書の文字列をしまう

    int max, score;
    string S, word, ans;
    while (getline(words_file, word))
    { // 辞書の文字列をひとつずつ見ていく
        // 文字列収納
        words_vec.push_back(word);
    }
    // スコア順で辞書をソートする
    sort(words_vec.begin(), words_vec.end(), sort_score);

    // ソートを行ったあとに文字のカウントを行う
    vector<vector<int>> word_count(words_vec.size(), vector<int>(26)); // 文字のカウント

    // 文字列にあらわれた文字をカウント
    for (int i = 0; i < words_vec.size(); i++)
    {
        // 1文字ずつ見ていく
        for (int j = 0; j < words_vec[i].length(); j++)
        {
            int num = words_vec[i][j] - 'a';
            word_count[i][num]++;
        }
    }

    while (getline(input_file, S))
    { // 入力文字列をひとつずつ見ていく

        // 文字列にあらわれた文字をカウント
        vector<int> input_count(26);
        for (int i = 0; i < S.length(); i++)
        {
            input_count[S[i] - 'a']++;
        }

        bool flag=false;
        max = 0;
        for (int i = 0; i < words_vec.size(); i++)
        { // 辞書の文字列をひとつずつ見ていく

            score = 0;
            // 各文字の数を比較する
            for (int j = 0; j < 26; j++)
            {
                // スコア計算(スコア順にソートしたらこの部分はいらなくなる)
                // score += word_count[i][j] * scores[j];

                // 単語を作成できない場合はbreak
                if (word_count[i][j] > input_count[j])
                {
                    break;
                }
                else if (j == 25)//最後の文字まで到達した文字列が、最もスコアの高いアナグラムになる
                {
                    ans = words_vec[i];
                    max = score;
                    flag=true;
                }
            }

            if(flag){//最もスコアの高いアナグラムを見つけたら残りの辞書は見なくてよくなる
                flag=false;
                break;
            }
        }
        output_file << ans << endl;
    }

    return 0;
}