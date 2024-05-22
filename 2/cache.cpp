#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;
// cache.pyの内容をC++でそれっぽくかいてみる

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
        // 各文字に割り当てた素数とインデックスをかける
        hash += prime_num[key[i]] * (i + 1);
    }

    return hash;
}

// 時系列順にハッシュ値とURLを保存しておく双方向連結リスト
struct CacheListNode
{
    string URL;          // URL
    string value;        // データ
    CacheListNode *next; // 次のノード
    CacheListNode *prev; // 前のノード
};

// ハッシュテーブルに格納する連結リスト
struct ItemListNode
{
    string key;         // 元の文字列
    CacheListNode *p;   // 双方向連結リストの要素へのポインタ
    ItemListNode *next; // 次のノード
};

// ハッシュテーブル
struct HashTable
{
    int bucket_size = 10;           // ハッシュテーブルのおおきさ
    int item_count = 0;             // ハッシュテーブルのアイテムの数
    vector<ItemListNode *> buckets; // ハッシュテーブル

    // 初期化用のコンストラクタ
    HashTable()
    {
        // ハッシュテーブルの大きさを変更する
        buckets.resize(bucket_size);
        buckets.clear();
    }

    // ハッシュテーブルにアイテムを入れる関数
    // 入力：key文字列、ポインタ
    // 返り値：アイテムが追加されたらtrue、そうでなければfalse
    bool put(string key, CacheListNode *p)
    {

        // サイズ変更
        this->change_size();
        // サイズが適切かどうか確認する
        this->check_size();
        // ハッシュ値をもとに格納場所をきめる
        int bucket_index = calculate_hash(key) % this->bucket_size;

        // ハッシュテーブルから格納場所を取得
        ItemListNode *item = this->buckets[bucket_index];

        // アイテムをひとつずつ見ていき、keyがかぶってないか確認する
        while (item)
        {
            // keyがみつかったらvalue更新して終了
            if (item->key == key)
            {
                item->p = p;
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
        new_item->p = p;
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
    // 返り値：(双方向連結リストのポインタ,ハッシュテーブル内でkeyがみつかったかどうかのbool)のペア
    pair<CacheListNode *, bool> get(string key)
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
                return make_pair(item->p, true);
            }
            else
            {
                // つぎの要素をみにいく
                item = item->next;
            }
        }

        // keyがみつからなかったらてきとうなポインタとfalseを返す
        CacheListNode *dummy = new CacheListNode;
        return make_pair(dummy, false);
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

    // ハッシュテーブルのサイズを変える
    void change_size()
    {
        if (this->size() >= this->bucket_size * 0.7 ||
            (this->bucket_size > 100 && this->size() <= this->bucket_size * 0.3))
        {

            // 再ハッシュ
            // 新しいハッシュテーブルの大きさ
            int new_bucket_size;

            // ハッシュテーブルの大きさを変更
            if (this->size() >= this->bucket_size * 0.7)
            {
                new_bucket_size = this->bucket_size * 2 + 1;
            }
            else
            {
                new_bucket_size = this->bucket_size / 2;
            }

            // 新しいハッシュテーブルを作成
            vector<ItemListNode *> new_buckets;
            new_buckets.resize(new_bucket_size);
            // new_buckets.clear();

            // 旧ハッシュテーブルの中身をみていく
            for (int i = 0; i < this->bucket_size; i++)
            {
                // アイテム
                ItemListNode *item = buckets[i];

                // アイテムをひとつずつ見て新しいハッシュテーブルに格納する
                while (item)
                {
                    // 新しいインデックス
                    int new_bucket_index = calculate_hash(item->key) % new_bucket_size;
                    // あたらしいハッシュテーブルのitem
                    ItemListNode *new_item = new_buckets[new_bucket_index];

                    // あたらしいノードをつくる
                    ItemListNode *new_item_node = new ItemListNode;
                    new_item_node->key = item->key;
                    new_item_node->p = item->p;
                    new_item_node->next = new_item;

                    // 格納
                    new_buckets[new_bucket_index] = new_item_node;

                    // 次の要素を見る
                    item = item->next;
                }
            }

            // for(int i=0;i<new_bucket_size;i++){
            //     cout << new_buckets[i]<<endl;
            // }

            buckets.clear();
            buckets.shrink_to_fit();

            // 更新
            this->bucket_size = new_bucket_size;
            this->buckets = new_buckets;
        }
    }
};

// キャッシュ
struct Cache
{
    HashTable *hash_table = new HashTable(); // ハッシュテーブル
    CacheListNode *cache_new;                // 双方向連結リストの中でいちばん先頭の要素
    CacheListNode *cache_old = cache_new;    // 双方向連結リストの中でいちばん後ろの要素
    int Cache_size = 1;                      // キャッシュの大きさ

    // 初期化用コンストラクタ
    // 入力：キャッシュの大きさ
    Cache(int n)
    {
        // 引数でキャッシュの大きさを変更できるようにする
        Cache_size = n;
    }

    // 双方向連結リストのいちばん手前にデータを追加する関数
    // 入力；先頭に追加したい要素のポインタ
    void put_new(CacheListNode *p_new)
    {
        if (hash_table->item_count >= 1)
        { // アイテム数がひとつ以上の場合
            // あたらしい先頭の要素の次の要素を指定
            p_new->next = cache_new;

            // 元の先頭の前の要素を指定
            cache_new->prev = p_new;
        }

        // 先頭を更新
        cache_new = p_new;
    }

    // 双方向連結リストのいちばんうしろのデータを削除する関数
    void delete_old()
    {
        // あたらしいしんがりを取得
        CacheListNode *new_old = cache_old->prev;
        // しんがりの次の要素をNULLにする
        new_old->next = NULL;
        // しんがりを更新
        cache_old = new_old;
    }

    // 双方向連結リストの特定の要素を先頭に持ってくる関数
    // 入力：先頭に持っていきたい要素のポインタ
    void move_top(CacheListNode *p)
    {
        // 次の要素
        CacheListNode *next = p->next;
        // 手前の要素
        CacheListNode *prev = p->prev;

        if (prev == NULL)
        {
            // すでに先頭だったらなにもせず戻る
            return;
        }

        // やじるしをつけかえる

        prev->next = next;
        if (next != NULL)
        { // しんがり以外の場合
            next->prev = prev;
        }
        else
        { // しんがりの場合
            // しんがりを更新
            cache_old = prev;
        }

        // 先頭に持ってくる
        p->next = cache_new;
        cache_new->prev = p;
        p->prev = NULL;
        cache_new = p;
    }

    // ページにアクセスして、キャッシュにデータを格納する関数
    // 入力：URL、データ
    void access_page(string url, string contents)
    {
        // ハッシュテーブルに格納されているか確認
        CacheListNode *p = hash_table->get(url).first;
        bool result = hash_table->get(url).second;
        if (result)
        { // 格納されている場合
            // 双方向リストの一番先頭に持ってくる
            move_top(p);
        }
        else
        { // 格納されていない場合
            // キャッシュがまんぱいか見る
            if (hash_table->size() == Cache_size)
            { // まんぱいだった場合
                // 一番古い要素をハッシュテーブルから削除
                hash_table->delete_(cache_old->URL);
                // 双方向連結リストから一番古い要素を削除
                delete_old();
            }

            p = new CacheListNode;
            p->URL = url;
            p->value = contents;

            // 双方向連結リストの先頭に追加
            put_new(p);
            // ハッシュテーブルに追加
            hash_table->put(url, p);
        }
    }

    // キャッシュの中のURL一覧を返す関数
    // 出力：URL一覧のvector<string>
    vector<string> get_pages()
    {
        // 双方向リストの先頭
        CacheListNode *p = cache_new;
        vector<string> ans = {};
        // 双方向リストを前から見ていく
        while (p)
        {
            ans.push_back(p->URL);
            p = p->next;
        }
        return ans;
    }
};

// ふたつのvectorが等しいか確かめる関数
// なぜか==での比較を行うとエラーが出てしまうので比較用の関数を作ることで対処
// 入力：文字列が格納されたvectorふたつ
// 出力：2つの並びが完全に等しければtrue、そうでければfalse
bool check_pages(vector<string> vec1, vector<string> vec2)
{
    if (vec1.size() == vec2.size())
    {
        for (int i = 0; i < vec1.size(); i++)
        {
            if (vec1[i] != vec2[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

// キャッシュが動くかテストするための関数
void cache_test()
{
    Cache *cache = new Cache(4);

    assert(check_pages(cache->get_pages(), {}));

    cache->access_page("a.com", "AAA");
    assert(check_pages(cache->get_pages(), {"a.com"}));

    cache->access_page("b.com", "BBB");
    assert(check_pages(cache->get_pages(), {"b.com", "a.com"}));

    cache->access_page("c.com", "CCC");
    assert(check_pages(cache->get_pages(), {"c.com", "b.com", "a.com"}));

    cache->access_page("d.com", "DDD");
    assert(check_pages(cache->get_pages(), {"d.com", "c.com", "b.com", "a.com"}));

    cache->access_page("d.com", "DDD");
    assert(check_pages(cache->get_pages(), {"d.com", "c.com", "b.com", "a.com"}));

    cache->access_page("a.com", "AAA");
    assert(check_pages(cache->get_pages(), {"a.com", "d.com", "c.com", "b.com"}));

    cache->access_page("c.com", "CCC");
    assert(check_pages(cache->get_pages(), {"c.com", "a.com", "d.com", "b.com"}));

    cache->access_page("a.com", "AAA");
    assert(check_pages(cache->get_pages(), {"a.com", "c.com", "d.com", "b.com"}));

    cache->access_page("a.com", "AAA");
    assert(check_pages(cache->get_pages(), {"a.com", "c.com", "d.com", "b.com"}));

    cache->access_page("e.com", "EEE");
    assert(check_pages(cache->get_pages(), {"e.com", "a.com", "c.com", "d.com"}));

    cache->access_page("f.com", "FFF");
    assert(check_pages(cache->get_pages(), {"f.com", "e.com", "a.com", "c.com"}));

    cache->access_page("e.com", "EEE");
    assert(check_pages(cache->get_pages(), {"e.com", "f.com", "a.com", "c.com"}));

    cache->access_page("a.com", "AAA");
    assert(check_pages(cache->get_pages(), {"a.com", "e.com", "f.com", "c.com"}));

    cout << "Tests passed!" << endl;
}

int main()
{
    cache_test();
}