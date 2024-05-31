# week3

## modularized_calculator.cpp

### 概要

電卓のプログラム

#### 実行方法
コンパイルして実行すると、入力ができるようになります

(1+3)等好きな式を入力すると、計算結果が小数点以下5桁まで表示されます

#### 入力可能コマンド一覧

| コマンド | 効果 |
| ---- | ---- |
| + | たしざん |
| - | ひきざん |
| * | かけざん(たしざん・ひきざんより優先的に計算される) |
| / | わりざん(たしざん・ひきざんより優先的に計算される) |
| () | かっこ(他の演算子より優先的に計算される) |
| abs() | 絶対値 |
| int() | 小数切り捨て |
| round() | 四捨五入 |

### 仕様

#### 構造体

##### token

| 変数名 | 型 | 役割 |
| ---- | ---- | ---- |
| type | string | トークンのタイプを表す |
| number | double | 数字トークンの値や、関数トークンの関数の種類などを表す |

#### 関数一覧

| 関数名 | 入力 | 処理 | 返り値 |
| ---- | ---- | ---- | ---- |
| read_number | string、int | 整数・小数をトークン化する | pair<token, int> |
| read_plus | string、int | +をトークン化する | pair<token, int> |
| read_minus | string、int | -をトークン化する | pair<token, int> |
| read_times | string、int | *をトークン化する | pair<token, int> |
| read_division | string、int | /をトークン化する | pair<token, int> |
| read_right_bracket | string、int | )をトークン化する | pair<token, int> |
| read_left_bracket | string、int | (をトークン化する | pair<token, int> |
| read_function | string、int | 関数をトークン化する | pair<token, int> |
| tokenize | string | 文字列をトークンに分割する | vector<token> |
| evaluate_brackets | vector<token> | ()内を計算する | vector<token> |
| evaluate_abs | double | 絶対値を計算する | double |
| evaluate_int | double | 小数切り捨てを計算する | double |
| evaluate_round | double | 四捨五入を計算する | double |
| evaluate_function | vector<token> | 関数を計算する | vector<token> |
| evaluate_times_division | vector<token> | かけざん・わりざんを計算する | vector<token> |
| evaluate_plus_minus | vector<token> | たしざん・ひきざんを計算する | double |
| evaluate | vector<token> | 計算する | double |
| test | line、double | 計算結果が正しいかテストする関数 |  |
| run_test |  | いろんなテストケースを用いてテストする関数 |  |
| main |  | テストの実行や入力の受付を行う関数 | | 