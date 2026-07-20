---
layout: doc

emoji: 🏊
title: LeetCode全部やる [Easy編①]

date: 2026-07-05
permalink: 'https://blog.asumo.dev/posts/18-LeetCode-Easy.html'

prev: false
next: false

tags:
  - post2026
  - ccpp

hidden: true
---

# LeetCode全部やる [Easy編①]

[<Badge type="tag" text="C/C++" />](../tags/ccpp)

## はじめに

解答はここ: https://github.com/asumo-1xts/isCatBot

本当に全部やれるのか？

## [1. Two Sum](https://leetcode.com/problems/two-sum/) 🥇

### 方針

1. `nums`を昇順にソート
2. `nums[0] + nums[nums.size() - 1]`と`target`を比較して、結果に応じて先頭または末尾のインデックスを中央へ寄せる。これを繰り返すと正解のペアが見つかる

### 気付き

- 配列の末尾を削除すると$O(1)$だが、先頭を削除すると全体を前に詰める手間が生じて$O(n)$になる。無理に配列をいじらず、インデックスの位置をずらしていくのが良い
- イテレータには向きがあり、`.base()`で反転する（振り返る）ことができる

## [9. Palindrome Number](https://leetcode.com/problems/palindrome-number/) 🥈

### 方針

実行速度と省メモリ性のどちらを優先するかによる。

### 気付き

- for文を使わず書けないか常に疑ってみる
- 数字なので、半分を超えたかどうかは数えずとも大小比較で判定できる

## [13. Roman to Integer](https://leetcode.com/problems/roman-to-integer/) 🥈

### 方針

先頭から見るか末尾から見るかをよく考える。

末尾から見れば、文字の組み合わせを考えずとも数の大小だけで判定できる。

## [14. Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix/) 🥇

### 方針

初め勘違いしていたが、Prefixなので先頭の共通部分だけを見れば良い。

文字一つ一つではなく文字列全体を比較して、合うまで長い方を`pop_back()`する。

## [20. Valid Parentheses](https://leetcode.com/problems/valid-parentheses/) 🥇

### 方針

新たにスタックを用意して文字を積んでいき、括弧が閉じたら`pop_back()`する。

### 気付き

文字列をfor文で走査するときは

```cpp
for (char c : s) {
  // cは文字列sの各文字
}
```

のような書き方ができる。

## [21. Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists/) 🥉

### 方針

なぜ謎の構造体を使うのか、普通に配列でやれば良いだろ！と思ったら、どうやらノードたちを繋ぎ合わせてくださいねという誘導だったらしい。

### 気付き

ポインタが数珠つなぎになっていると、先頭さえ押さえておけば後のノードの更新は自動で反映されるので面白い。

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};
```

上記の意味するところは

- 構造体`NodeList`
  - 整数`val`
  - 他の`NodeList`へのポインタ`next`
  - ↓
- 構造体`NodeList`
  - ...

ということらしく、ラスト3行は宣言時のテンプレートを示している。 

<br/>

---
