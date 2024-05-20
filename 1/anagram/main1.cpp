#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;

int main()
{

    ifstream input_file("test.txt");        // 文字列ファイル読む
    ifstream words_file("words.txt");       // 辞書ファイル読む
    vector<pair<string, string>> words_vec; // 辞書の中身をしまうvector
    string S;

    // 辞書の文字列のソート
    while (getline(words_file, S))
    { // 辞書の文字列をひとつずつ見ていく
        string tmp = S;
        // 文字列をソート
        sort(S.begin(), S.end());
        words_vec.push_back({S, tmp});
    }

    // 辞書そのもののソート
    sort(words_vec.begin(), words_vec.end());

    while (getline(input_file, S))
    { // 入力文字列をひとつずつ見ていく
        // 文字列をソート
        sort(S.begin(), S.end());

        // にぶたん
        int left = 0;
        int right = words_vec.size() - 1;

        while(left<=right){
            int mid=left+(right-left)/2;

            //みつかったらぬける
            // if(words_vec[mid]==S){
            //     cout << S<<" ";
            //     break;
            // }

            //ひだりにいく
            if(words_vec[mid].first>S){
                right=mid-1;
            }

            //みぎにいく
            //leftに正解の部分のいちばん大きいものの位置+1が残る
            if(words_vec[mid].first<=S){
                left=mid+1;
            }

        }

        // 正解の部分のいちばん大きいものの位置+1を記憶
        int max=left;
        // int max = upper_bound(words_vec.begin(), words_vec.end(), S) - words_vec.begin();
        // 正解の部分のいちばん小さいものの位置+1を記憶
        // int min = lower_bound(words_vec.begin(), words_vec.end(), S) - words_vec.begin();

        // リセット
        left = 0;
        right = words_vec.size() - 1;

        while(left<=right){
            int mid=left+(right-left)/2;

            //みつかったらぬける
            // if(words_vec[mid]==S){
            //     cout << S<<" ";
            //     break;
            // }

            //ひだりにいく
            //rightに正解の部分のいちばん小さいものの位置-1が残る
            if(words_vec[mid].first>=S){
                right=mid-1;
            }

            //みぎにいく
            if(words_vec[mid].first<S){
                left=mid+1;
            }

        }

        // アナグラムをすべて出力する
        for (int i = right+1; i < max; i++)
            {
                cout << words_vec[i].second << " ";
            }
    }

    cout << endl;

    return 0;
}