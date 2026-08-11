---
layout: doc

emoji: 📇
title: BibLaTeXで欧文と和文を混ぜる

date: 2025-06-10
permalink: 'https://blog.asumo.dev/posts/03-BibLaTeXJP.html'

prev: false
next: false

tags:
  - post2025
  - latex
---

# BibLaTeXで欧文と和文を混ぜる

[<Badge type="tag" text="LaTeX" />](../tags/latex)

## はじめに

BibLaTeXはBibTeXより新しく便利な部分もあるものの、日本語への対応状況が芳しくありません。この記事では、半ばごり押しで以下の項目を実装します。

|                    | 欧文の文献                                      | 和文の文献                                    |
| ------------------ | ----------------------------------------------- | --------------------------------------------- |
| 連名著者の省略     | $\textit{et al.}$                               | $\textrm{他}$                                 |
| 連名著者の区切り   | $\textrm{hoge, fuga,}$<br/>$\textrm{and piyo.}$ | $\textrm{hoge，fuga，}$<br/>$\textrm{piyo．}$ |
| 文献タイトル       | “$\textrm{Title}$”                              | $\textrm{『タイトル』}$                       |
| 会議・ジャーナル名 | $\textrm{In: Hoge}$<br/>$\textrm{Conference}$   | $\textrm{何とか会議}$<br/>（In: 無し）        |

## 環境

- Overleaf（texlive-fullでも同様に動きました）
- upLaTeX（LuaLaTeXだともう少し別な方法がありそう…）

## bibファイル

和文の文献だけ、最後に`langid = Japanese`を追加しておきます。ここが唯一の手動ポイントなのでどうにか自動化したいところですが、論文1本の中に含まれる和文の文献の数って（少なくとも理工系においては）たかが知れているという印象なので、まあ良いでしょう。

```tex [ref.bib ~vscode-icons:file-type-tex~]
@article{Europian2001,
  title   = {Europian title},
  author  = {Europian, Author and Europian, Editor},
  journal = {Europian journal},
  date    = 2001
}

@article{Europian2002,
  title   = {Europian title 2},
  author  = {Europian, Author and Europian, Editor and Europian, Director},
  journal = {Europian journal},
  date    = 2002
}

@article{Japanese2001,
  title     = {和文の文献},
  author    = {和文太郎 and 和文花子},
  journal   = {和文ジャーナル},
  date      = 2001,
  langid    = {Japanese} % [!code ++]
}

@article{Japanese2002,
  title     = {和文の文献 2},
  author    = {和文太郎 and 和文花子 and 和文次郎},
  journal   = {和文ジャーナル},
  date      = 2002,
  langid    = {Japanese} % [!code ++]
}
```

## プリアンブル

上で追加した`langid`によって文献ごとに欧文or和文を判別して処理を行います。

厄介なのは著者名省略の処理で、それ用のコマンドがbabelパッケージに依存？しているのに、babelパッケージが`japanese`をサポートしてくれていません。もっとも設定した言語そのものが文献リストに影響する訳ではなく、設定した言語ごとに特有の処理を行うというだけなので、`japanese`は便宜的に`british`で代用することにします。

::: code-group
<<< @/snippets/2025/03-preamble.tex{tex} [main.tex ~vscode-icons:file-type-tex~]
:::

## 本文

:::details おそらくlatexmkrcも必要
<<< @/snippets/2025/03-latexmkrc.pl{perl} [latexmkrc ~vscode-icons:file-type-perl~]
:::

::: code-group
<<< @/snippets/2025/03-document.tex{tex} [main.tex ~vscode-icons:file-type-tex~]
:::

## 結果

<ImageGroup
  :sources="[
    '/images/2025/03-01.webp',
  ]"
  type="big"
/>

## 文献番号のスタイルを変更する

ちょっとしたおまけです。論文の提出先によっては、以下のように文献番号を上付きにしたり括弧を変更したりといった変則的なスタイルが求められるかもしれません[^1]。

> $$
> \text{Europianらの研究もあれば}^{1)2)}\text{，}\text{和文らの研究もある}^{3)4)}\text{．}
> $$

このようなときはプリアンブルで以下のように追記し、本文中で`\cite`の代わりに`\autocite`を使うと良いです。BibLaTeX側の`style = numeric-comp`等もきちんと効きます。

::: code-group

```tex [main.tex ~vscode-icons:file-type-tex~]
\usepackage[
  autocite = superscript, % \cite -> \autocite [!code ++]
  backend = biber,
  style = numeric-comp
]{biblatex} % BibLaTeXパッケージ読み込み

\DeclareFieldFormat{labelnumber}{#1)} % 本文中での括弧を設定 [!code ++]
\DeclareFieldFormat{labelnumberwidth}{#1} % 末尾での括弧を設定 [!code ++]
```

:::

## おわりに

今のところ良い感じに使えています。

<br/>

[^1]: 一例として筆者の修士論文がそうでした
