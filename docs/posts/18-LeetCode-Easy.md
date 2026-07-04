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

## 1. Two Sum

### 方針

1. `nums`を昇順にソート
2. `nums[0] + nums[nums.size() - 1]`と`target`を比較して、結果に応じて先頭または末尾のインデックスを中央へ寄せる。これを繰り返すと正解のペアが見つかる

### 気付き

- 配列の末尾を削除すると$O(1)$だが、先頭を削除すると全体を前に詰める手間が生じて$O(n)$になる。無理に配列をいじらず、インデックスの位置をずらしていくのが良い
- イテレータには向きがあり、`.base()`で反転する（振り返る）ことができる

## 9. Palindrome Number

### 方針

実行速度と省メモリ性のどちらを優先するかによる。

### 気付き

- for文を使わず書けないか常に疑ってみる
- 数字なので、半分を超えたかどうかは数えずとも大小比較で判定できる

<br/>

---