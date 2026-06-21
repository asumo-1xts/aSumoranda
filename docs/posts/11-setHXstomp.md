---
layout: doc

emoji: 👟
title: HX Stompの同時押しを無効化

date: 2025-11-23
permalink: "https://blog.asumo.dev/posts/11-setHXstomp.html"

prev: false
next: false

tags:
    - post2025
    - trouble
---

# HX Stompの同時押しを無効化

[<Badge type="tag" text="トラブルシュート" />](../tags/trouble)

## はじめに

最近になってHX Stompを買いました。出音に関してはまあこんなものかなという感じですが、そんなことよりもスナップショットモード時に同時押し（プリセット切り替え）を無効化できないのが致命的です。

<ImageGroup
  :sources="[
    '/images/2025/11-01.webp'
  ]"
  type="double"
  caption="爪先の尖った靴を履いていないギタリスト・ベーシストはそれなりに多い"
/>

直接的な無効化の方法はありませんが、既にRedditなどでも[言及されている通り](https://www.reddit.com/r/Line6Helix/comments/11gcxtz/comment/jaovxpk/?tl=ja&utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button "ダブルボタンプレス機能を無効にする")コマンドセンターからゴリ押せば実質的に無効化された状態にすることができます。

## 解決

1. スナップショットモードではなくストンプモードに入る
2. メニューを開く（`<PAGE`と`PAGE>`の同時押し）

<ImageGroup
  :sources="[
    '/images/2025/11-02.webp',
    '/images/2025/11-03.webp'
  ]"
  type="double"
/>

3. `Command Center`に入って上段ノブでStomp1を選択
4. `Command`を`Snpsht`に、そして`Press`を`SNAP1`に設定

<ImageGroup
  :sources="[
    '/images/2025/11-04.webp',
    '/images/2025/11-05.webp'
  ]"
  type="double"
/>

5. 必要に応じてSNAP2、SNAP3も同様に設定

<ImageGroup
  :sources="[
    '/images/2025/11-06.webp',
    '/images/2025/11-07.webp'
  ]"
  type="double"
/>

6. 完了

<ImageGroup
  :sources="[
    '/images/2025/11-08.webp'
  ]"
  type="double"
/>

## おわりに

こんな面倒なことユーザーにさせないでくれ～！

<br/>

---
