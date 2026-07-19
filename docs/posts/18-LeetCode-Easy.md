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

<br/>

---
