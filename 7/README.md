# week7

## malloc.c

### 概要

mallocとfreeを実装したプログラム

best fitとfree list binで実装

#### best fit

十分な大きさの空き領域のうち、一番小さいものを選ぶようにする

##### 時間

Time[ms]

| challenge | simple_malloc | my_malloc|
| ---- | ---- | ---- |
| 1|26|27|
| 2|8|6|
| 3|234|36|
| 4|46627|1861|
| 5|36306|1392|

##### 利用率

Utilization[%]

| challenge | simple_malloc | my_malloc|
| ---- | ---- | ---- |
| 1|70|70|
| 2|40|39|
| 3|8|51|
| 4|16|72|
| 5|15|72|

#### free list bin

空き領域の大きさ別に5つのビンを用意する

- 32未満
- 32以上128未満
- 128以上512未満
- 512以上2048未満
- それ以外

要求される大きさを見たところ、4の倍数が多いように感じたので、32～4096の間の4の倍数で区切った

##### 時間

Time[ms]

| challenge | simple_malloc | my_malloc|
| ---- | ---- | ---- |
| 1|28|19|
| 2|9|9|
| 3|188|27|
| 4|45544|504|
| 5|35437|486|

##### 利用率

Utilization[%]

| challenge | simple_malloc | my_malloc|
| ---- | ---- | ---- |
| 1|70|70|
| 2|40|39|
| 3|8|51|
| 4|16|72|
| 5|15|72|

### 考察

free list binはbest fitと比べて速さの改善が見られたが、利用率は変わらなかった

空き領域同士をmargeするような操作を加えたら利用率が改善できたかもしれない