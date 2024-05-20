### ハッシュテーブルではなく木構造を使う理由

- ハッシュテーブルでは要素が増えれば増えるほど再ハッシュに時間がかかるが、木構造ではどれだけ要素が増えてもバランス木を使うことでO(logN)で終わらせることができるから
- ハッシュテーブルではあらかじめテーブルの領域を確保するのでメモリを食うが、木構造では要素の数分のメモリしか必要としないから
- ハッシュテーブルでは衝突が起こることでデータの検索、追加、削除の計算量がO(N)になる可能性があるが、木構造ではバランス木を用いることでデータの検索、追加、削除の計算量をO(logN)にすることができるから
- 木構造ではどんなデータ型でもハッシュ関数を考えることなく実装できるから
- 木構造の方が実装しやすいから