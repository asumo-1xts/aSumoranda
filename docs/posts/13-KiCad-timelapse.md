---
layout: doc

emoji: 🕰️
title: KiCadで偽タイムラプスを生成

date: 2026-01-23
permalink: 'https://blog.asumo.dev/posts/13-KiCad-timelapse.html'

prev: false
next: false

tags:
  - post2026
  - kicad
  - python
---

# KiCadで偽タイムラプスを生成

[<Badge type="tag" text="KiCad" />](../tags/kicad)
[<Badge type="tag" text="Python" />](../tags/python)

## はじめに

ソースコードの書かれた過程をタイムラプス風に再生するOSS『[gitlogue](https://github.com/unhappychoice/gitlogue 'gitlogue')』を見かけて、KiCadの基板データで似たようなことをやりたくなりました。「kicad timelapse」でググったところマサチューセッツ大学の先生が[既にやっていた](https://blog.krastanov.org/2020/02/17/pcb-layout-timelapse/ 'Making a Timelapse of the Creation of a Printed Circuit Board in KiCad')のですが、彼の公開してくれているソースコードが（おそらくKiCadのバージョン違いなどもあり）うまく動かなかったため、私の方でもそれなりに頑張りました。

先に成果物↓を貼っておきます。

<ImageGroup
  :sources="[
    '/images/2026/13-01.gif'
  ]"
  type="big"
  caption="言うまでもなく、Arduino UNOの表側です"
/>

## 環境

- Debian 13 on WSL2
- KiCad 9.0.7
- Python 3.13.5

他の環境で試せていませんが、少なくともWindowsに関しては何とかなりそうに見えます。

## 方針

早めに断っておきますが、本記事の方法では**実際の作業工程に基づいたタイムラプスは生成できません**。冒頭でgitlogueを紹介してしまいましたがGitも関係ありません。代わりに、一切の迷いなく図面を完成させた世界線の、**偽のタイムラプスを捏造します**。

具体的には、KiCadには内部のC++をPythonでラップしたAPIが`pcbnew.py`として同梱されているので、これを叩いて任意の`.kicad_pcb`ファイル内を走査しつつ図面を出力していきます。

::: tip
むしろこの`pcbnew.py`をGUI化したものがKiCadのPCBエディターなのかも…？

Debian 13からaptでインストールした場合は`/usr/lib/python3/dist-packages/pnbnew.py`に、Windows 11からwingetでインストールした場合は`C:\Users\username\AppData\Local\Programs\KiCad\9.0\bin\Lib\site-packages\pcbnew.py`に入っています。
:::

### ワークフロー

1. `.kicad_pcb`ファイルを入力（このあと色々されますが変更は保存されないので無事です）
2. 塗りつぶし領域を一つ消去したうえで、基板をSVG画像にプロット
3. 塗りつぶし領域が無くなるまで2を繰り返す
4. 配線についても同様に2~3を行う
5. フットプリントについても同様に2~3を行う
6. 輪郭線についても同様に2~3を行う
7. ImageMagickでSVG画像をPNG画像に変換
8. PNG画像をFFmpegで逆順につなぎ合わせて出力
9. 適宜トリミングして完成

## 実行

### スクリプト

::: code-group
<<< @/snippets/2026/13-main.py{python} [main.py]
<<< @/snippets/2026/13-board_process.py{python} [board_process.py]
<<< @/snippets/2026/13-external_tools.py{python} [external_tools.py]
:::

[ワークフロー](#ワークフロー)のうち2~6を`board_process.py`の関数が、また7と8を`external_tools.py`の関数がそれぞれ担っています。

`main.py`の期待する引数は以下の通りです。

| 引数 | 説明                             | デフォルト         |
| ---- | -------------------------------- | ------------------ |
| #1   | レイヤー選択（`F_Cu` or `B_Cu`） | `F_Cu`             |
| #2   | 入力ファイルへの相対パス         | `sample.kicad_pcb` |
| #3   | 出力する動画ファイルの名前部分   | `sample`           |

### シェルコマンドの一例

```shell
apt install kicad imagemagick ffmpeg

python3 main.py B_Cu awesome.kicad_pcb awesome_timelapse
```

::: tip
実行の可否に関わらず、以下のような警告が大量に出力されると予想されます。

> swig/python detected a memory leak of type 'PCB_TRACK \*', no destructor found.

ターミナルでは最後にまとめて出力されるかもしれませんが、これは実際には`board_process.py`内の`remove_item()`で逐一発生しており、itemをremoveすることによって内部のC++からitemへのポインタがnullになってしまうことが原因です。今回は単発のスクリプト内での事象なので危険視せず放置してしまっています。

なお正攻法としてはitemをUser1など無関係なレイヤーに逃がすという手があるのですが、これを試してみると一部のitemがプロット上に残ってしまう（最終的に何も無い状態にならない）という謎の不具合が代わりに生じてしまったため、採用していません。
:::

::: tip
当然ですが、基板の規模によっては（特にSVG→PNGの処理に）膨大なリソースと時間を要します。筆者の割と強いデスクトップPCでも冒頭のArduino UNOの処理に8分ほどかかっているので、お手元の計算機の性能には十分に留意してください。
:::

## おわりに

無事にMP4動画とGIF画像が出力されたでしょうか？

そのままだと空白部分が大きすぎるのでクロップしたいところですが、GIF画像をアニメーションのままクロップする方法は意外と限られていて少し不便です。私は[EZgif](https://ezgif.com/ 'EZgif')を利用しました。

<br/>

---
