---
layout: doc

emoji: 🪽
title: チャットの誤送信を撲滅するツール

date: 2026-09-15
permalink: 'https://blog.asumo.dev/posts/19-GoE.html'

prev: false
next: false

tags:
  - appdev
  - webdev
---

# チャットの誤送信を撲滅するツール

## はじめに

表題のものを作ったので、主にロジック部分の実装について書き残しておきます。

ツールはGitHubのReleaseページから入手できます。

[The God of Enter](@:https://github.com/asumo-1xts/GoE)

### 作った動機

人間あるいはAI相手にチャットを入力しているとき、不意にEnterキーを押してしまって文の途中で誤送信すること、ありますよね（小林製薬）。

我々が等しく思い描いている挙動は、おそらくこうです。

| 操作       | 理想             | 現実              |
| ---------- | ---------------- | ----------------- |
| 変換を確定 | `Enter`          | `Enter`           |
| 改行       | `Enter`          | `Shift` + `Enter` |
| 送信       | `Ctrl` + `Enter` | `Enter`           |

Teams v2には「理想」がアプリ側のオプションとして[用意された](http://forest.watch.impress.co.jp/docs/serial/yajiuma/2097818.html '待望の［Enter］キーによる誤送信防止オプション、「Microsoft Teams」に展開される')にも関わらず、同じMicrosoft製のM365 Copilotには何故か用意されていません。他にはDiscordやCopilot等も軒並み「現実」なので、デスクトップ/Web問わず全部まとめて「理想」になるべきです。

同じ目的を持ったプロジェクトはいくつか試しましたが、残念ながら満足に動作するものに出会うことができませんでした。

## 環境

### OS

- Windows 11
  - ❌ MacOS [^1]

### 入力メソッド

- Microsoft IME
  - ❌ Google日本語入力
  - ❓ ATOK

## 方針

単純にキーバインドを次のように置き換えても、我々が日本語を使う限りはうまくいきません。

- `Enter` → `Ctrl` + `Enter`
- `Shift` + `Enter` → `Enter`

この状態でIMEをONにして日本語をタイプすると、皆さんもご存知の通り、タイプされた文字には下線が引かれて未確定な状態になります。ところがこのとき`Shift` + `Enter`では未確定な文字を確定できないので、Enterキーが完全に無効化されてしまいます。そこで本来の`Enter`を求めて送信用の`Ctrl` + `Enter`を恐る恐る押して確定することになる訳ですが、これでは今までと何も変わりません。

説明が長くなりましたが、上の表でも示したように、とにかく「変換を確定」するときだけはEnterキーはそのまま`Enter`として機能する必要があります。これを含めて所望の機能を実現するためには、以下の2点を常に監視できればよいです。

- IMEがONかOFFか
- 未確定な文字の有無

検索すると以下の記事が割とすぐヒットします。上記2点を達成するためのスニペットが記載されているのですが、残念ながらそのままではうまく動作しなかったので改良していきます。

[Autohotkey v2.0のIME制御用 関数群 IMEv2.ahk](@:https://qiita.com/kenichiro_ayaki/items/d55005df2787da725c6f)

### IMEがONかOFFかを取得する

これは上の記事の`IME_GET()`を使えばよいのですが、WebView2で描画されているデスクトップアプリ（Teams v2、M365 Copilot、…）に対応していません。そこで、そのような場合にはUIAutomation（UIA）ライブラリを用いることにします[^2]。

[UIA-v2](@:https://github.com/Descolada/UIA-v2)

::: code-group
<<< @/snippets/2026/19-GetIME.ahk{ahk2} [GetIME.ahk]
:::

### 未確定な文字の有無を取得する

同じく上の記事の`IME_GetConverting()`を使えれば良かったのですが、これは少なくとも私の環境では上手く動作しませんでした。そこで少し考えて、

`未確定な文字の有無 = IS_IME() && 直前に文字が打たれたかどうか`であることに気付きました。

### 未確定文字があるかどうかを取得する

IME_GetStatus()

<br>

[^1]: 私が会社の開発用PCとしてWindows PCしか支給されなかったため
[^2]: しれっとUIAライブラリが落ちているAutoHotKey、すごい