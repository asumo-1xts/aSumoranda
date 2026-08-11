---
layout: doc

emoji: 🦓
title: Stripeで簡易なECサイトを実装

date: 2026-03-22
permalink: 'https://blog.asumo.dev/posts/15-Stripe-simpleEC.html'

prev: false
next: false

tags:
  - post2026
  - webdev
  - python
  - jsts
---

# Stripeで簡易なECサイトを実装

[<Badge type="tag" text="Web開発" />](../tags/webdev)
[<Badge type="tag" text="JS/TS" />](../tags/jsts)
[<Badge type="tag" text="Python" />](../tags/python)

## はじめに

[1x telescope 公式サイト](https://1xtelescope.com/ '1x telescope')の各製品ページにStripeの購入ボタンを設置し、他のECサイト等に飛ぶことなくダイレクトに商品を購入できるようにしました。

<ImageGroup
  :sources="[
    '/images/2026/15-01.webp',
    '/images/2026/15-02.webp'
  ]"
  type="double"
/>

そう珍しい方法でもないとは思いますが、

- Stripe Webhook
- Netlify Functions
- GitHub Actions

を連携して在庫の表示と更新までサーバーレスに実装できたので、その構成をざっくりまとめておきます。なお当該のGitHubリポジトリは流石にPrivateです、悪しからず。

::: details 個人的な思想の話
「公式サイトを運営しつつ決済のためにECサイトも利用する」というケースは2026年現在そう珍しいものでもないと思いますが、個人的に

- 製品ページがインターネット上に2つ存在することになる
- それらの管理が一元的でない
- 公式サイトからECサイトに飛ばされると購入時の体験があまり良くない

といった不満があり、公式サイトで直接買い物できるのが一番良いという思想を持っていました。しかしそのためには、一般的には（方法はどうあれ）サーバーを建てて管理する面倒が生じます[^1]。

そこで、何とかサーバーレスになんちゃってECサイトを実装できないか？と考えた結果、今回ご紹介するスタイルに辿り着いたという次第です。
:::

## 環境

- Netlifyの静的ホスティングサービスに[^2]
- サーバーサイドレンダリングなサイトを
- GitHub経由でデプロイしている

## 構想

諸々の動きは次の図の通りです。それぞれのステップについてソースコードなど貼りつつ説明していきます[^3]。

<ImageGroup
  :sources="[
    '/images/2026/15-03.webp'
  ]"
  type="big"
/>

## Stripe

### Webhook

#### 送信先を追加する

Stripeのダッシュボードから「設定」「Payments」と進んで、ページの一番下の「次のステップ」にある「Webhookを設定」クリックします。するとワークベンチとやらが下からニュッと現れるので、「＋送信先を追加する」をクリックして以下のように設定します。

| 項目                 | 値                                                             |
| -------------------- | -------------------------------------------------------------- |
| イベントのリッスン先 | 「お客様のアカウント」                                         |
| APIバージョン        | （デフォルト）                                                 |
| イベント             | `checkout.session.completed`                                   |
| 送信先のタイプ       | 「Webhookエンドポイント」[^4]                                  |
| 送信先名             | （任意に決める）                                               |
| エンドポイントURL    | `https://example.com/.netlify/`<br/>`functions/stripe-webhook` |

#### 署名キー

ここで「署名シークレット」なる`whsec_***...`みたいな秘密鍵が発行されるので、手元に控えておきます。

### APIキー

ダッシュボードに戻って、「開発者」「APIキー」「APIキーの管理」と進んで「制限付きのキー」を発行します。このとき権限は以下のように設定します。

| 項目                                   | 値                                                             |
| -------------------------------------- | -------------------------------------------------------------- |
| このAPIキーの使用方法                  | 「このキーを別の<br/>ウェブサイトに提供」                      |
| 名前                                   | （任意に決める）                                               |
| URL                                    | `https://example.com/.netlify/`<br/>`functions/stripe-webhook` |
| このキーに対する権限をカスタマイズする | ☑                                                              |

| 項目     | 権限     |
| -------- | -------- |
| Checkout | 読み取り |
| Webhook  | 読み取り |
| 他       | なし     |

## Netlify Functions

### サーバーレス関数

GitHubリポジトリの`netlify/functions/`ディレクトリに以下のファイルを入れておきます。心得のある人はPythonやC#で書いても良いみたいです。

::: code-group
<<< @/snippets/2026/15-stripe-webhook.js{javascript} [netlify/functions/stripe-webhook.js]
:::

コメントアウトから察せられるかと思いますが、Stripe上で決済が完了したとき

- 売れた商品の情報
- 購入者のメールアドレス

といった情報を受け取ってGitHub Actionsに投げる、中継所のような役割を担っています。

### GitHub PAT

GitHubのアカウントメニューから「Settings」「Developer settings」と進んで、Personal access token: PATを発行します。

| 項目              | 値                                                  |
| ----------------- | --------------------------------------------------- |
| 種類              | Fine-grained tokens                                 |
| Token name        | （任意に決める）                                    |
| ...               | ...                                                 |
| Repository access | Only select repository<br/>（当該リポジトリを選択） |
| Permissions       | ☑ Actions (Read and write)                          |

### 環境変数の登録

Netlifyにログインして、プロジェクトの「Environment Variables」に諸々の秘密鍵を登録します。

| 変数名                  | 中身                          |
| ----------------------- | ----------------------------- |
| `GITHUB_TOKEN`          | [先ほど](#github-pat)のPAT    |
| `STRIPE_WEBHOOK_SECRET` | [先ほど](#署名キー)の署名キー |
| `STRIPE_API_SECRET`     | [先ほど](#apiキー)のAPIキー   |

## GitHub Actions

ここまでで全部のサービスの連携が完了しました。あとはGitHub Actionsを`repository_dispatch`で発火させて、好きな動作をさせるだけです。

一例として、1x telescopeでは

- 在庫数の更新
- 購入者へのメール送信

をPythonスクリプトで実行しています。

::: code-group
<<< @/snippets/2026/15-stripe-webhook.yml{yaml} [.github/workflows/stripe-webhook.yml ~vscode-icons:folder-type-github~]
<<< @/snippets/2026/15-update_stock.py{python} [update_stock.py]
<<< @/snippets/2026/15-send_email.py{python} [send_email.py]
:::

::: tip
上の`update_stock.py`では在庫が0になると購入ボタンをコメントアウトするようにしています。しかしボタンから飛んだ先の決済リンクは生きているので、これを自動で無効化するためにStripeのダッシュボード上アプリとして[Stockify](https://marketplace.stripe.com/apps/stockify-manage-inventory 'Stockify - Manage Inventory')を併用しています。
:::

## おわりに

カート機能などを実装するとなると多分サーバーが必要になるのですが、複数種類をまとめて購入される方はあまり居ないため現状これで十分かなと思います。

説明することが多くて随分と駆け足になってしまいましたが、何らかのお役に立てば幸いです。

## 参考

[Netlify Functions を使って最速でサーバーを作る](@:https://qiita.com/hcrane/items/7f612685958be8cc2b03)

[GitHub Actions の repository_dispatch イベントを使ってリポジトリ間でリリースイベントを伝播させる](@:https://qiita.com/takat0-h0rikosh1/items/d464f4733ff81fda23fc)

[GitHub ActionsとPythonで、自動メール通知システムを実装してみた](@:https://qiita.com/nakano0328/items/6deed8e4a83100104802)

<br/>

[^1]: この世にはサーバーを建てて管理することに幸福を覚える人も一定数いるみたいです

[^2]: 言わずもがな、VercelやCloudFlareでも同様のノリで実装できそうです

[^3]: ？？？「_解説や応援が必要なら、私に任せてください！_」

[^4]: 初めAWSの方を使おうとして挫折しました。<br/>なおNetlify FunctionsはそのAWS（Lambda）をラップしたものらしい
