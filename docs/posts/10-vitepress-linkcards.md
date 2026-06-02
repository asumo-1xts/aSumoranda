---
layout: doc

emoji: 🎴
title: VitePressでリンクカードを貼る

date: 2025-11-06
permalink: 'https://blog.asumo.dev/posts/10-vitepress-linkcards.html'

prev: false
next: false

tags:
  - post2025
  - webdev
  - jsts
---

# VitePressでリンクカードを貼る

[<Badge type="tag" text="Web開発" />](../tags/webdev)
[<Badge type="tag" text="JS/TS" />](../tags/jsts)

::: info
二段階認証が上手くいかなくなってnpmjsから締め出されてしまい、`vitepress-linkcard`を更新できなくなりました。仕方なく`vitepress-linkcards`に移行し、それに合わせて本記事も更新しました。
:::

## はじめに

VitePress用のリンクカード生成プラグインをリリースしたので簡単に紹介します。

[GitHub](@:https://github.com/asumo-1xts/vitepress-linkcards)

リンクカードはこんな感じ↑です。v2になってホバーアニメーションが実装されました！後述しますが、カードの枠線や背景の色はオプションで自由に設定できます。

なお、このプラグインは[markdown-it-link-to-card](https://github.com/luckrya/markdown-it-link-to-card '@luckrya/markdown-it-link-to-card')をフォークして作られました。

## Getting started

### インストール

::: code-group

```sh [npm]
npm install -D vitepress-linkcards
```

```sh [yarn]
yarn add -D vitepress-linkcards
```

```sh [pnpm]
pnpm add -D vitepress-linkcards
```

:::

### 使い方

::: code-group

```ts [docs/.vitepress/config.ts]
import { defineConfig } from 'vitepress'
import { linkToCardPlugin } from 'vitepress-linkcards'
import type { LinkToCardPluginOptions } from 'vitepress-linkcards'

export default defineConfig({
  // ...
  markdown: {
    config: (md) => {
      md.use<LinkToCardPluginOptions>(linkToCardPlugin, {
        // target: "_self" // オプション（後述）
      })
    }
  }
  // ...
})
```

:::

URLに`@:`のプレフィクスを付けるとリンクカードが生成されます。

::: code-group

```md [*.md]
[example](@:https://example.com)
```

:::

## オプション

### Target

[使い方](#使い方)で見たように、リンクカードを踏んだときのリンクの開き方を指定できます。

- `_blank` (default)
- `_self`
- `_top`
- `_parent`

### Color theme

以下のカラーをカスタマイズできます。

- 枠線
- 背景
- 枠線（ホバー時）
- 背景（ホバー時）

デフォルトではホバーアニメーションは起こりません。

::: code-group

```css [docs/.vitepress/theme/custom.css]
/* homeレイアウトの"Features"に似せた設定 */

.vitepress-linkcards-container {
  border-color: #00000000 !important;
  background-color: var(--vp-c-bg-soft) !important;
}

.vitepress-linkcards-container:hover {
  border-color: var(--vp-c-brand-1) !important;
  background-color: var(--vp-c-bg-soft) !important;
}
```

```ts [docs/.vitepress/theme/index.ts]
import DefaultTheme from 'vitepress/theme-without-fonts'
import type { Theme as ThemeConfig } from 'vitepress'
import './custom.css'

const Theme: ThemeConfig = {
  extends: DefaultTheme
}

export default {
  ...Theme
}
```

:::

## その他の仕様

### `.linkcards_cache.json`

このファイルは自動的に生成され、解析されたメタデータをキャッシュします。必要に応じて編集できますが、gitignoreするとビルド時間が長くなります。

### `github.com`ドメインへの特殊な操作

URLのドメインが`github.com`のとき、titleとdescriptionの内容の重複を避けるべく以下に示すようなトリミングが実行されます。

|        | Title                                                                                     | Description                                                                                                                                 |
| ------ | ----------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| Before | GitHub - asumo-1xts/vitepress-linkcard: A VitePress plugin to generate a pretty linkcard. | A VitePress plugin to generate a pretty linkcard. Contribute to asumo-1xts/vitepress-linkcard development by creating an account on GitHub. |
| After  | asumo-1xts/vitepress-linkcard                                                             | A VitePress plugin to generate a pretty linkcard.                                                                                           |

## おわりに

なかなか良い感じだと思うので、良かったら使ってみてください！

<br/>

---
