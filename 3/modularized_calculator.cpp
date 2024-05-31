#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <stack>
using namespace std;

// modularized_calculator.pyの内容をC++で書く

//各関数を表す数
double ABS = 1.0;
double INT = 2.0;
double ROUND = 3.0;

// token
struct token
{
    string type;
    double number;
};
double evaluate(vector<token> tokens);

// 整数・小数を読みこむ関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(読みとった数字のトークン、index)のペア
pair<token, int> read_number(string line, int index)
{
    double number = 0;

    // 数を読み込む
    while (index < line.length() && isdigit(line[index]))
    { // lineをまだ読み切っておらず、次の文字が数字であるとき
        // 各桁の数字を取得し、数に足す
        number = number * 10 + int(line[index] - '0');
        // 次の文字を見に行く
        index++;
    }

    // 小数点を読み込んだ時
    if (index < line.length() && line[index] == '.')
    {
        // 次の文字を見に行く
        index++;
        // 小数点以下の桁数に対応
        double decimal = 0.1;
        // 数を読み込む
        while (index < line.length() && isdigit(line[index]))
        {
            // 各桁の数字を取得し、数に足す
            number += int(line[index] - '0') * decimal;
            // 小数点以下の桁数を増やす
            decimal /= 10;
            // 次の文字を見に行く
            index++;
        }
    }
    // トークンを返す
    token result = {type : "NUMBER", number : number};
    return make_pair(result, index);
}

// ＋のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(＋のトークン、index)のペア
pair<token, int> read_plus(string line, int index)
{
    token result = {type : "PLUS", number : -1};
    return make_pair(result, index + 1);
}

// －のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(－のトークン、index)のペア
pair<token, int> read_minus(string line, int index)
{
    token result = {type : "MINUS", number : -1};
    return make_pair(result, index + 1);
}

// ×のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(×のトークン、index)のペア
pair<token, int> read_times(string line, int index)
{
    token result = {type : "TIMES", number : -1};
    return make_pair(result, index + 1);
}

// ÷のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(÷のトークン、index)のペア
pair<token, int> read_division(string line, int index)
{
    token result = {type : "DIVISION", number : -1};
    return make_pair(result, index + 1);
}

// )のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(")"のトークン、index)のペア
pair<token, int> read_right_bracket(string line, int index)
{
    token result = {type : "RIGHT_BRACKET", number : -1};
    return make_pair(result, index + 1);
}

// (のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：("("のトークン、index)のペア
pair<token, int> read_left_bracket(string line, int index)
{
    token result = {type : "LEFT_BRACKET", number : -1};
    return make_pair(result, index + 1);
}

// 関数のトークンを返す関数
// 入力：文字列、index(文字列内でindex番目を見ていることに対応)
// 返り値：(関数のトークン、index)のペア
pair<token, int> read_function(string line, int index)
{
    token result;
    if (line[index] == 'a' && line[index + 1] == 'b' && line[index + 2] == 's')
    {
        result = {type : "FUNCTION", number : ABS};
        index = index + 3;
    }
    else if (line[index] == 'i' && line[index + 1] == 'n' && line[index + 2] == 't')
    {
        result = {type : "FUNCTION", number : INT};
        index = index + 3;
    }
    else if (line[index] == 'r' && line[index + 1] == 'o' && line[index + 2] == 'u' && line[index + 3] == 'n' && line[index + 4] == 'd')
    {
        result = {type : "FUNCTION", number : ROUND};
        index = index + 5;
    }
    else
    {
        cout << "Invalid character found(x x): " << line[index] << endl;
        exit(1);
    }
    return make_pair(result, index);
}

// 文字列をトークンに分割する関数
// 入力：文字列
// 返り値：トークンのvector
vector<token> tokenize(string line)
{
    vector<token> tokens;
    int index = 0;

    pair<token, int> result;
    // 文字列を前から見ていく
    while (index < line.length())
    {
        if (isdigit(line[index]))
        { // 数字を読んだ場合
            result = read_number(line, index);
        }
        else if (line[index] == '+')
        { //+を読んだ場合
            result = read_plus(line, index);
        }
        else if (line[index] == '-')
        { //-を読んだ場合
            result = read_minus(line, index);
        }
        else if (line[index] == '*')
        { //*を読んだ場合
            result = read_times(line, index);
        }
        else if (line[index] == '/')
        { // /を読んだ場合
            result = read_division(line, index);
        }
        else if (line[index] == ')')
        { // (を読んだ場合
            result = read_right_bracket(line, index);
        }
        else if (line[index] == '(')
        { // (を読んだ場合
            result = read_left_bracket(line, index);
        }
        else if (isalpha(line[index]))
        {
            result = read_function(line, index);
        }
        else
        { // どれでもない場合
            cout << "Invalid character found(x x): " << line[index] << endl;
            exit(1);
        }
        // トークンをトークンvectorにpush
        tokens.push_back(result.first);
        // インデックスを更新
        index = result.second;
    }
    return tokens;
}

//()を計算してくれる関数
vector<token> evaluate_brackets(vector<token> tokens)
{
    int index = 0;
    int left = index;
    vector<token> new_tokens;
    // tokensを前から見ていく
    while (index < tokens.size())
    {
        // cout<<index<<endl;
        // トークンが)の場合
        if (tokens[index].type == "RIGHT_BRACKET")
        {
            //(を探しに行く
            left = index;
            while (left >= 0)
            {
                if (tokens[left].type == "LEFT_BRACKET")
                {
                    break;
                }
                left--;
            }

            //()の中身を取り出す
            copy(tokens.begin() + left + 1, tokens.begin() + index, back_inserter(new_tokens));
            double answer = evaluate(new_tokens);

            // tokensを更新する
            token new_token = {type : "NUMBER", number : answer};
            tokens.erase(tokens.begin() + left, tokens.begin() + index + 1);
            tokens.insert(tokens.begin() + left, new_token);
            index = left;
        }
        index++;
    }
    return tokens;
}

// 絶対値を計算する関数
// 入力：小数
// 返り値：入力の絶対値
double evaluate_abs(double num)
{
    return abs(num);
}

// 小数切り捨てを計算する関数
// 入力：小数
// 返り値：入力の小数切り捨て
double evaluate_int(double num)
{
    return int(num);
}

// 四捨五入を計算する関数
// 入力：小数
// 返り値：入力の四捨五入
double evaluate_round(double num)
{
    return round(num);
}

// functionを計算する関数
// 入力：トークンのvector
// 返り値：新しいトークンのvector
vector<token> evaluate_function(vector<token> tokens)
{
    int index = 0;
    int left = index;
    vector<token> new_tokens;
    // tokensを前から見ていく
    while (index < tokens.size())
    {
        if (tokens[index].type == "RIGHT_BRACKET")
        {
            left = index;
            //(を探しに行く
            while (left >= 0)
            {

                if (tokens[left].type == "LEFT_BRACKET") //(を見つけたらbreak
                {
                    break;
                }

                left--;
            }

            if (left > 0 && tokens[left - 1].type == "FUNCTION")
            { // 関数だった場合

                //()の中身を取り出す
                new_tokens.clear();
                copy(tokens.begin() + left + 1, tokens.begin() + index, back_inserter(new_tokens));
                double answer = evaluate(new_tokens);

                if (tokens[left - 1].number == ABS) // 関数の処理を実行
                {
                    answer = evaluate_abs(answer);
                }
                else if (tokens[left - 1].number == INT)
                {
                    answer = evaluate_int(answer);
                }
                else if (tokens[left - 1].number == ROUND)
                {
                    answer = evaluate_round(answer);
                }

                // tokensを更新する
                token new_token = {type : "NUMBER", number : answer};
                tokens.erase(tokens.begin() + left - 1, tokens.begin() + index + 1);
                tokens.insert(tokens.begin() + left - 1, new_token);

                index = left - 1;
            }
        }
        index++;
    }
    return tokens;
}

// かけざんわりざんをしてくれる関数
// 入力：トークンのvector
// 返り値：新しいトークン
vector<token> evaluate_times_division(vector<token> tokens)
{
    double answer = 0;
    int index = 1;

    // tokensを前から見ていく
    while (index < tokens.size())
    {
        // cout<<"index:"<<index<<endl;
        //  トークンが数字の場合
        if (tokens[index].type == "NUMBER")
        {

            if (tokens[index - 1].type == "TIMES")
            { // ひとつ前のトークンが*の場合
                answer = tokens[index - 2].number * tokens[index].number;
                // トークンから削除
                tokens.erase(tokens.begin() + index - 2, tokens.begin() + index + 1);
                // 新しいトークンを挿入
                token new_token = {type : "NUMBER", number : answer};
                tokens.insert(tokens.begin() + index - 2, new_token);
                index -= 2;
            }
            else if (tokens[index - 1].type == "DIVISION")
            { // ひとつ前のトークンが/の場合
                // 0除算のエラー
                if (tokens[index].number == 0)
                {
                    cout << "divide by zero!" << endl;
                    exit(1);
                }
                else
                {
                    answer = tokens[index - 2].number / tokens[index].number;
                    // トークンから削除
                    tokens.erase(tokens.begin() + index - 2, tokens.begin() + index + 1);
                    // 新しいトークンを挿入
                    token new_token = {type : "NUMBER", number : answer};
                    tokens.insert(tokens.begin() + index - 2, new_token);
                    index -= 2;
                }
            }
        }
        index++;
    }
    return tokens;
}

// たしざんひきざんをしてくれる関数
// 入力：トークンのvector
// 返り値：新しいトークン
double evaluate_plus_minus(vector<token> tokens)
{
    int index = 1;
    double answer = 0;
    // tokensを前から見ていく
    while (index < tokens.size())
    {
        // cout<<tokens[index].number<<endl;
        //  トークンが数字の場合
        if (tokens[index].type == "NUMBER")
        {

            if (tokens[index - 1].type == "PLUS")
            { // ひとつ前のトークンが+の場合
                answer += tokens[index].number;
            }
            else if (tokens[index - 1].type == "MINUS")
            { // ひとつ前のトークンが-の場合
                answer -= tokens[index].number;
            }
            else
            { // それ以外であればエラーを出す
                cout << "Invalid syntax: " << tokens[index - 1].type << " index-1:" << index - 1 << endl;
                exit(1);
            }
        }
        index++;
    }

    return answer;
}

// 計算結果を返してくれる関数
// 入力：トークンのvector
// 返り値：計算結果
double evaluate(vector<token> tokens)
{

    // 【TODO】デバッグ用の関数を作る
    // for (int i = 0; i < tokens.size(); i++)
    // {
    //     cout << "tokens.type: " << tokens[i].type << " tokens.number: " << tokens[i].number << endl;
    // }

    // 小数に対応するためdouble型
    double answer = 0;

    vector<token> new_tokens = evaluate_function(tokens);
    new_tokens = evaluate_brackets(new_tokens);

    // 一番最初に+を挿入することで最初の数字を足す
    token first_token = {type : "PLUS", number : -1};
    new_tokens.insert(new_tokens.begin(), first_token);

    new_tokens = evaluate_times_division(new_tokens);
    answer = evaluate_plus_minus(new_tokens);

    return answer;
}

//テスト用の関数
void test(string line, double expected_answer)
{
    // 文字列をトークンに分割
    vector<token> tokens = tokenize(line);
    // 計算する
    double actual_answer = evaluate(tokens);

    // eval関数が使えないので一旦想定解を自分で入力する形にする
    if (abs(actual_answer - expected_answer) < 1e-8)
    {
        cout << "PASS! (" << line << "=" << expected_answer << ")" << endl;
    }
    else
    {
        cout << "FAIL! (" << line << " should be " << expected_answer << " but was " << actual_answer << ")" << endl;
    }
}

//テスト用の関数
void run_test()
{
    cout << "==== Test started! ====" << endl;
    // 整数+整数
    test("1+2", 3);
    // 小数+小数
    test("3.0+2.0", 5);
    // 小数+小数-整数
    test("1.0+2.1-3", 0.1);
    // 整数*整数
    test("2*3", 6);
    // 小数*小数
    test("2.5*3.2", 8);
    // 整数*小数
    test("4*2.2", 8.8);
    // 整数/整数
    test("4/2", 2);
    // 小数/整数
    test("3.3*3", 9.9);
    // 小数/小数
    test("6.2/3.1", 2);
    // 四則演算ごちゃまぜ
    test("2+3*4-3", 11);
    test("7*4/2-10", 4);
    test("2.5*4-4/2", 8);

    //()つき
    test("5*(1+2+3)", 30);
    test("(10-2*4)/2", 1);
    test("(1+5)/2", 3);
    test("(6-2)/2", 2);
    test("2.4*5+(1.2+2.8)", 16);
    test("(3.4+5.2)/4.3+4", 6);

    //関数
    test("abs(-4)", 4);
    test("abs(3.2)", 3.2);
    test("int(-4)", -4);
    test("int(1.2)", 1);
    test("round(3.2)", 3);
    test("round(3.6)", 4);
    test("int(int(1)+int(3+4))", 8);
    test("abs(int(2.3)-round(3-1.5)*2)", 2);
    cout << "==== Test finished! ====" << endl;
}

int main()
{
    run_test();
    while (1)
    {
        cout << ">";
        // 入力を受け取る
        string line;
        cin >> line;
        // 文字列をトークンに分割
        vector<token> tokens = tokenize(line);
        // 計算する
        double answer = evaluate(tokens);

        cout << fixed << setprecision(5);
        cout << answer << endl;
    }
}
