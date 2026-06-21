---
layout: doc

emoji: 🏋️
title: QMKでTAP機能を強引に無効化

date: 2026-06-21
permalink: "https://asumoranda.com/posts/17-QMK-reduce.html"

prev: false
next: false

tags:
    - post2026
    - trouble
    - ccpp

hidden: true
---

# QMKでTAP機能を強引に無効化

[<Badge type="tag" text="その他の工作" />](../tags/otherdiy)
[<Badge type="tag" text="トラブルシュート" />](../tags/trouble)
[<Badge type="tag" text="C/C++" />](../tags/ccpp)

## はじめに

QMKでキーボードのファームウェアを作るとき、容量を小さくするために使わない機能を無効化するのが一般的かと思います。ご多分に漏れず私も[ダイエットに勤しんでいた](./16-J2J)のですが、TAP機能（あるキーを素早く連続して押すことで別の動作をさせる機能）を無効化するべく[公式ドキュメント](https://docs.qmk.fm/squeezing_avr#config-h-settings)に従って

```cpp [config.h ~vscode-icons:file-type-cheader~]
#define NO_ACTION_TAPPING
```

したところコンパイルエラーが出ました。

色々と試して最終的にQMKのソースコードをローカルでいじって解決したので、その詳細を残しておきます。

## 環境

- Windows 11
- QMK 1.2.0

## 症状

```shell
Compiling: tmk_core/protocol/lufa/usb_util.c [OK]
Linking: .build/j2j_vial.elf [ERRORS]
C:/QMK_MSYS/opt/qmk/bin/../lib/gcc/avr/15.2.0/../../../../avr/bin/ld.exe:
C:\QMK_MSYS\tmp\cc0M5GPb.ltrans0.ltrans.o: in function L0':
<artificial>:(.text.process_record+0x1a): undefined reference to flow_tap_update_last_event'
collect2.exe: error: ld returned 1 exit status
make: *** [builddefs/common_rules.mk:272: .build/j2j_vial.elf]
エラー 1
```

エラーメッセージによると、どうやら`flow_tap_update_last_event()`なる関数が見つからないようです。この関数は[qmk_firmware](https://github.com/qmk/qmk_firmware)の`quantum/action_tapping.c`にて定義されています。

しかし、この`action_tapping.c`全体は`#ifndef NO_ACTION_TAPPING ... #endif`で囲まれており、また`action_tapping.h`を確認しても`flow_tap_update_last_event()`が外部に宣言されるための条件は満たされていないように見えます。つまり、謎です。

しばらく自分の`config.h`側から何とかできないかと格闘していましたが、面倒になったのでエラーの該当箇所をQMK側で直接削除することにしました。そもそも使わない機能なので、コンパイルが通ってしまえばこっちのものでしょう！

## 解決

3つのファイルを以下の通りに編集します。

::: code-group
<<< @/snippets/2026/17-action_tapping.c{c} [quantum/action_tapping.c ~vscode-icons:file-type-c~]
<<< @/snippets/2026/17-action_tapping.h{c} [quantum/action_tapping.h ~vscode-icons:file-type-cheader~]
<<< @/snippets/2026/17-action.c{c} [quantum/action.c ~vscode-icons:file-type-c~]
:::

その上で、自前のコードに改めて以下を追加します。ファームウェアの容量にはそれぞれが独立して影響します。

::: code-group
```cpp [config.h ~vscode-icons:file-type-cheader~]
#define NO_ACTION_TAPPING
```
```make [rules.mk ~vscode-icons:file-type-makefile~]
TAP_DANCE_ENABLE = no
```
:::

結果的に5172バイトも削減することができました！おかげでLEDアニメーションやMIDI機能を心おきなく実装できました。

## おわりに

使われていないコードを突き詰めて削除していけば、まだファームウェア軽量化の余地があるような気がします。それはそれとしてQMK側のソースコードをいじるのはあまりスマートではないようにも思うので、もっとスマートな方法があったら知りたいところです。
