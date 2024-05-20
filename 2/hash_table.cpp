#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;
// hash_table.pyの内容をC++でそれっぽくかいてみる

int prime_num[168] =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
     31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
     73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
     127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
     179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
     233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
     283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
     353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
     419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
     467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
     547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
     607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
     661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
     739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
     811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
     877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
     947, 953, 967, 971, 977, 983, 991, 997};

// hash関数
// 入力：文字列
// 返り値：ハッシュ
int calculate_hash(string key)
{
    int hash = 0;
    // hashを計算
    for (int i = 0; i < key.length(); i++)
    {
        // 各文字のアスキーコードを足していく
        hash += prime_num[key[i]] * (i + 1);
    }

    return hash;
}

// ハッシュテーブルの中のkeyとvalueをならべておく連結リスト
struct ItemListNode
{
    string key;         // 元の文字列
    int value;          // ハッシュ値のこと？
    ItemListNode *next; // 次のノード
};

// ハッシュテーブル
struct HashTable
{
    int bucket_size = 97;           // ハッシュテーブルのおおきさ
    int item_count = 0;             // ハッシュテーブルのアイテムの数？
    vector<ItemListNode *> buckets; // ハッシュテーブル

    // 初期化用のコンストラクタ
    HashTable()
    {
        // ハッシュテーブルの大きさを変更する
        buckets.resize(bucket_size);
        buckets.clear();
    }

    // ハッシュテーブルにアイテムを入れる関数
    // 入力：key文字列、値
    // 返り値：アイテムが追加されたらtrue、そうでなければfalse
    bool put(string key, int value)
    {
       
        // サイズ変更
        this->change_size();
        // サイズが適切かどうか確認する
        this->check_size();
        // ハッシュ値をもとに格納場所をきめる
        int bucket_index = calculate_hash(key) % this->bucket_size;
        cout <<"bucket_index:"<<bucket_index<<endl;

        // ハッシュテーブルから格納場所を取得
        ItemListNode *item = this->buckets[bucket_index];
        
        cout << "item:" << item << endl;

        // アイテムをひとつずつ見ていき、keyがかぶってないか確認する
        while (item)
        {
            // keyがみつかったらvalue更新して終了
            if (item->key == key)
            {
                item->value = value;
                return false;
            }
            else
            {
                // つぎの要素をみにいく
                item = item->next;
            }
        }
        
        // あたらしいノードをつくる
        ItemListNode *new_item = new ItemListNode;
        new_item->key = key;
        new_item->value = value;
        // いちばん手前にあたらしい要素を入れる
        new_item->next = this->buckets[bucket_index];

        // アイテムリスト更新
        this->buckets[bucket_index] = new_item;
        // アイテム数をふやす
        this->item_count++;
 
        return true;
    }

    // ハッシュテーブルからアイテムを取得する関数
    // 入力：文字列
    // 返り値：(valueもしくはNULL,ハッシュテーブル内でkeyがみつかったかどうかのbool)のペア
    pair<int, bool> get(string key)
    {
        // サイズが適切かどうか確認する
        this->check_size();
        // ハッシュ値をもとに格納場所を求める
        int bucket_index = calculate_hash(key) % this->bucket_size;
        // ハッシュテーブルから格納場所を取得
        ItemListNode *item = this->buckets[bucket_index];

        // アイテムをひとつずつ見ていき、keyがあるか確認する
        while (item)
        {
            // keyがみつかったらvalueとtrueを返す
            if (item->key == key)
            {
                return make_pair(item->value, true);
            }
            else
            {
                // つぎの要素をみにいく
                item = item->next;
            }
        }

        // keyみつからなかったら-1とfalseを返す
        return make_pair(-1, false);
    }

    // ハッシュテーブルからアイテムを削除する関数
    // 入力：文字列
    // 返り値：アイテムが削除できたらtrue、そうでなければfalse
    //  deleteはC++の予約語なので応急処置でdelete_にしてしまった
    bool delete_(string key)
    {
        // サイズ変更
        this->change_size();
        // サイズが適切かどうか確認する
        this->check_size();
        // ハッシュ値をもとに格納場所を求める
        int bucket_index = calculate_hash(key) % this->bucket_size;
        // ハッシュテーブルから格納場所を取得
        ItemListNode *item = this->buckets[bucket_index];
        // ひとつ前の要素を保持する
        ItemListNode *pre_item;

        // itemがNULLの場合
        if (item == NULL)
        {
            return false;
        }

        // 先頭を削除する場合
        if (item->key == key)
        {
            // 先頭を「今のアイテムの次の要素」にする
            this->buckets[bucket_index] = item->next;
            // アイテム数を減らす
            this->item_count--;
            return true;
        }
        else
        { // 先頭を削除しない場合
            // 「ひとつ前の要素」を保持
            pre_item = item;
            // 次の要素を見る
            item = item->next;
        }

        // アイテムをひとつずつ見ていき、keyがあったら削除する
        while (item)
        {
            if (item->key == key)
            { // keyが見つかった場合
                // 「手前のアイテムの次の要素」を「今のアイテムの次の要素」にする
                pre_item->next = item->next;
                // アイテム数を減らす
                this->item_count--;
                return true;
            }

            // ひとつ前の要素を保持しておく
            pre_item = item;
            // 次の要素を見る
            item = item->next;
        }
        return false;
    }

    // ハッシュテーブルのアイテム数を返してくれる関数
    // 出力：ハッシュテーブルのアイテム数
    int size()
    {
        return this->item_count;
    }

    // ハッシュテーブルが効率的な大きさかどうか確認する関数
    // ①ハッシュテーブルの大きさが100以下
    // ②アイテム数がハッシュテーブルの大きさの30%以上
    // ↑のどちらかの場合はOK
    void check_size()
    {
        assert(this->bucket_size < 100 || this->item_count >= this->bucket_size * 0.3);
    }

    //ハッシュテーブルのサイズを変える
    void change_size()
    {
        if (this->size() >= this->bucket_size * 0.7 ||
            (this->bucket_size>100 &&this->size() <= this->bucket_size * 0.3))
        {

            // 再ハッシュ
            // 新しいハッシュテーブルの大きさ
            int new_bucket_size;

            // ハッシュテーブルの大きさを変更
            if (this->size() >= this->bucket_size * 0.7)
            {
                new_bucket_size = this->bucket_size * 2+1;
            }
            else
            {
                new_bucket_size = this->bucket_size / 2;
            }

            // 新しいハッシュテーブルを作成
            vector<ItemListNode *> new_buckets;
            new_buckets.resize(new_bucket_size);
            new_buckets.clear();

            // 旧ハッシュテーブルの中身をみていく
            for (int i = 0; i < this->bucket_size; i++)
            {
                // アイテム
                ItemListNode *item = buckets[i];
                

                //アイテムをひとつずつ見て新しいハッシュテーブルに格納する
                while (item)
                {
                    // 新しいインデックス
                    int new_bucket_index = calculate_hash(item->key) % new_bucket_size;
                    //あたらしいハッシュテーブルのitem
                    ItemListNode *new_item= new_buckets[new_bucket_index];

                    // あたらしいノードをつくる
                    ItemListNode *new_item_node = new ItemListNode;
                    new_item_node->key = item->key;
                    new_item_node->value = item->value;
                    new_item_node->next=new_item;

                    //格納
                    new_buckets[new_bucket_index]=new_item_node;

                    //次の要素を見る
                    item=item->next;
                }
            }

            buckets.clear();
            buckets.shrink_to_fit();

            //更新
            this->bucket_size=new_bucket_size;
            this->buckets=new_buckets;
            cout <<"new:"<<this->bucket_size<<endl;
        }
    }
};

// ハッシュテーブルがうまく動作するかテストするための関数
void functional_test()
{
    HashTable *hash_table = new HashTable();
    assert(hash_table->put("aaa", 1) == true);
    assert(hash_table->get("aaa") == make_pair(1, true));
    assert(hash_table->size() == 1);

    assert(hash_table->put("bbb", 2) == true);
    assert(hash_table->put("ccc", 3) == true);
    assert(hash_table->put("ddd", 4) == true);
    assert(hash_table->get("aaa") == make_pair(1, true));
    assert(hash_table->get("bbb") == make_pair(2, true));
    assert(hash_table->get("ccc") == make_pair(3, true));
    assert(hash_table->get("ddd") == make_pair(4, true));
    assert(hash_table->get("a") == make_pair(-1, false));
    assert(hash_table->get("aa") == make_pair(-1, false));
    assert(hash_table->get("aaaa") == make_pair(-1, false));
    assert(hash_table->size() == 4);

    assert(hash_table->put("aaa", 11) == false);
    assert(hash_table->get("aaa") == make_pair(11, true));
    assert(hash_table->size() == 4);

    assert(hash_table->delete_("aaa") == true);
    assert(hash_table->get("aaa") == make_pair(-1, false));
    assert(hash_table->size() == 3);

    assert(hash_table->delete_("a") == false);
    assert(hash_table->delete_("aa") == false);
    assert(hash_table->delete_("aaa") == false);
    assert(hash_table->delete_("aaaa") == false);

    assert(hash_table->delete_("ddd") == true);
    assert(hash_table->delete_("ccc") == true);
    assert(hash_table->delete_("bbb") == true);
    assert(hash_table->get("aaa") == make_pair(-1, false));
    assert(hash_table->get("bbb") == make_pair(-1, false));
    assert(hash_table->get("ccc") == make_pair(-1, false));
    assert(hash_table->get("ddd") == make_pair(-1, false));
    assert(hash_table->size() == 0);

    assert(hash_table->put("abc", 1) == true);
    assert(hash_table->put("acb", 2) == true);
    assert(hash_table->put("bac", 3) == true);
    assert(hash_table->put("bca", 4) == true);
    assert(hash_table->put("cab", 5) == true);
    assert(hash_table->put("cba", 6) == true);
    assert(hash_table->get("abc") == make_pair(1, true));
    assert(hash_table->get("acb") == make_pair(2, true));
    assert(hash_table->get("bac") == make_pair(3, true));
    assert(hash_table->get("bca") == make_pair(4, true));
    assert(hash_table->get("cab") == make_pair(5, true));
    assert(hash_table->get("cba") == make_pair(6, true));
    assert(hash_table->size() == 6);

    assert(hash_table->delete_("abc") == true);
    assert(hash_table->delete_("cba") == true);
    assert(hash_table->delete_("bac") == true);
    assert(hash_table->delete_("bca") == true);
    assert(hash_table->delete_("acb") == true);
    assert(hash_table->delete_("cab") == true);
    assert(hash_table->size() == 0);
    cout << "Functional tests passed!" << endl;
}

//パフォーマンステスト
void performance_test()
{
    HashTable *hash_table = new HashTable();

    for (int i = 0; i < 100; i++)
    {
        // タイマースタート
        auto start = chrono::system_clock::now();
        // 乱数生成器
        mt19937 random_num(i);
        for (int j = 0; j < 10000; j++)
        {
            // 乱数を生成
            int rand = random_num() % 100000000;

            // ハッシュテーブルに格納
            cout << hash_table->put(to_string(rand), rand) << endl;
            cout << "j:" << j << endl;
        }

        // 乱数生成器
        mt19937 random_get_num(i);
        for (int j = 0; j < 10000; j++)
        {
            // 乱数を生成
            int rand = random_get_num() % 100000000;
            // ハッシュテーブルを検索
            hash_table->put(to_string(rand), rand);
        }

        auto end = chrono::system_clock::now();

        chrono::duration<double> time = end - start;

        cout << i << " " << time.count();
    }

    for (int i = 0; i < 100; i++)
    {
        mt19937 random_delete_num(i);
        for (int j = 0; j < 10000; j++)
        {
            // 乱数を生成
            int rand = random_delete_num() % 100000000;
            // ハッシュテーブルを検索
            hash_table->delete_(to_string(rand));
        }
    }
    assert(hash_table->size() == 0);
    cout << "Performance tests passed!" << endl;
}

int main()
{
    // functional_test();
    performance_test();
}