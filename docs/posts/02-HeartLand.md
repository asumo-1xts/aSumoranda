---
layout: doc

emoji: 🎛️
title: 理想のMIDIコントローラを自作

date: 2024-08-18
permalink: "https://blog.asumo.dev/posts/02-HeartLand.html"

prev: false
next: false

tags:
    - post2024
    - arduino
    - ccpp
    - midi
    - otherdiy
---

# 理想のMIDIコントローラを自作

[<Badge type="tag" text="その他の工作" />](../tags/otherdiy)
[<Badge type="tag" text="MIDI" />](../tags/midi)
[<Badge type="tag" text="Arduino" />](../tags/arduino)
[<Badge type="tag" text="C/C++" />](../tags/ccpp)

::: warning
正しい情報を記載するよう努めていますが、それはそれとして当方は一切の責任を負いかねます。

いかなる作業も自己責任で行っていただくようお願いいたします。
:::

## はじめに

Ableton LiveでDJをやりたいんですが、MIDIコントローラーの選択肢が少なすぎる！求める機能をそれなりに備えているのはAkai APC40 mk2ですが、高額な上に球数も少ないので故障が怖いです。海外には[セミオーダーできるところ](https://yaeltex.com/ "YAELTEX")もあるようですが、とても趣味に出せる金額ではありません…

仕方がないので自分で作ることにしました。

## 環境

- Windows 11
- Arduino IDE 2.3.3
- KiCad 8.0.2
- Ableton Live 12 Suite

## 構想

### 仕組み

ノブやフェーダやボタン、つまりは入力をたくさん用意したいので、単にArduinoボードを1台用意するだけでは端子が足りません。そこでArduinoを2台用意して『daughter』『mother』と名付け、以下のように役割を振りました。

<ImageGroup
  :sources="[
    '/images/2024/02-01.webp'
  ]"
  type="big"
/>

抽象度が高すぎて何のこっちゃという感じですが、ざっくり説明すると

#### daughter

- ノブ等からの入力を読み込んでMIDIメッセージを生成、motherへ送る
- 入力端子が多いのでとにかく沢山読み込む

#### mother

- ノブ等からの入力を読み込んでMIDIメッセージを生成、daughterから送られた分とマージしてPCへ送る
- daughterだけでは手の足りていないアナログ入力を主に読み込む

という感じで、そんなに難しいことはしていません。

マルチプレクサやシフトレジスタを使って入力を増やす方法もあるのですが、仕組みや回路を考えるのに少しばかり頭を使います。その点Arduinoなら直感的に繋いでコーディングするだけですし、ジェネリック品で良ければアリエクなどで割と安価に入手できます。

ちなみにdaughterとmotherをこの組み合わせで運用する場合、**仕様上Mega2560 ProMiniはPC側からUSBデバイスとして認識させることが出来ません**ので、必ずProMicroがmotherになる必要があります（一敗）。

### 外装

3Dプリンターやらアルミシャーシやら考えましたが、とりあえず基板を2枚重ねる方式を採用することにしました。回路が走る基板の上にスペーサをかませて、ガワとして同じ大きさの回路なし基板を乗せるやり方です。ゴミが入りやすいという最大の難点を無視すれば、そんなに悪くはないでしょう。

## 設計

### 回路

<ImageGroup
  :sources="[
    '/images/2024/02-02.webp'
  ]"
  type="big"
/>

細かい話ですが、C1とC2のパスコンは実際には各Arduinoへ一つずつ使っています。

なおArduinoのKICAD用シンボルは有志の方が公開しているものをお借りしました。本記事の末尾にまとめてリンクを貼っておきます。ダウンロードして自分のKiCadで使えるようになるまでに一苦労あった気がするのですが、忘れてしまいました…。

### ソースコード

[Control Surfaceなる神ライブラリ](https://github.com/tttapa/Control-Surface "Control Surface")のおかげで簡潔かつスムーズに書けました。`example`ディレクトリを見れば大体何でもできるようになっています。

個人的な工夫として、MIDIチャンネルを指定する箇所ではあえて同ライブラリの内部関数を用い、冒頭の変数で一括変更できるようにしました。

::: code-group
<<< @/snippets/2024/02-mother.cpp{cpp} [for Mother ~vscode-icons:file-type-cpp~]
<<< @/snippets/2024/02-daughter.cpp{cpp} [for Daughter ~vscode-icons:file-type-cpp~]
<<< @/snippets/2024/02-common.cpp{cpp} [Common functions ~vscode-icons:file-type-cpp~]
:::

mother側では、DAWからクロックを受けてLEDをメトロノーム的に光らせる機能を盛り込んでいます。詳細は[こちらの記事](./01-BPMfromDAW "ArduinoでDAWからBPMを取得")にて。

### 基板

普段エフェクターを作ったりするときは自分で配線まで考えるのですが、今回はちょっと大変だったので[自動配線ツール](https://freerouting.mihosoft.eu/ "Freerouting")を利用して電源ラインだけ手直ししました。本当はもっと拘るべきですが、まあ趣味のものなので最終的に問題なく動作していればOKとさせてください。

表側の基板（部品を載せないガワの方）にも両面GNDベタを施しておくと、強度がかなり上がってたわみにくくなります。これまた本当はノイズなどの影響を吟味すべきですが、とりあえず目先の実用性をとっています。

<ImageGroup
  :sources="[
    '/images/2024/02-03.webp',
    '/images/2024/02-04.webp'
  ]"
  type="double"
  caption="←2D | 3D→"
/>

ボタンはキーボード用のCherry MXのやつを採用することにしました。軸とキーキャップを選べて楽しいです。フェーダのフットプリントの選択に少し悩みましたが、KICADにデフォルトで入っている`Potentiometer_THT:Potentiometer_Bourns_PTA4543_Single_Slide`を使えば[秋月で売っているやつ](https://akizukidenshi.com/catalog/g/g109238/ "[109238]スライドボリューム(B10kΩ) SL4515N-B103L15CM")がそのまま使えました。

## 組み立て・完成

基板発注はJLCPCBで行いました。はんだ付けをして、ねじを締めて、諸々を取り付けて完成です。

<ImageGroup
  :sources="[
    '/images/2024/02-05.webp',
    '/images/2024/02-06.webp',
  ]"
  type="double"
/>

<ImageGroup
  :sources="[
    '/images/2024/02-07.webp',
    '/images/2024/02-08.webp'
  ]"
  type="double"
/>

製作途中の写真は撮り忘れました…。各部品の調達先をまとめておきます。

| 部品                 | 調達先            | 備考                                     |
| -------------------- | ----------------- | ---------------------------------------- |
| Arduino              | AliExpress        | セールを狙うとさらに安い                 |
| Cherry MX ボタン     | Amazon            | Gateron もあり                           |
| フェーダ             | 秋月              | 品質はめちゃくちゃ良いとは言えないかも   |
| ボリューム           | Tayda Erectronics | D シャフトで堅牢                         |
| ロータリーエンコーダ | Tayda Erectronics | 国内では見ない 20 クリックのものが買える |

## おわりに

完成してから数週間が経過しましたが、今のところ全機能きちんと使えています。

開発にあたって試作を繰り返す中でかなりの額を費やしたので最初から既製品を買っときゃ良かったじゃん！と思わないでもないですが、この先こいつが壊れても安価にもう一台もう一台と用意できるのは心強いです。上澄みとしてこの記事を残しておくので、皆さん軽率に理想のMIDIコンを作ってみてください！

## 参考

### KICAD 用シンボル/フットプリント

### ProMicro

[ProMicro](@:https://github.com/g200kg/kicad-lib-arduino)

### Mega2560

[Mega2560](@:https://github.com/Alarm-Siren/arduino-kicad-library)

<br/>

---
