const stripe = require('stripe')(process.env.STRIPE_API_SECRET)
const GITHUB_OWNER = 'your-github-username' // GitHubのユーザー名
const GITHUB_REPO = 'your-repo-name' // GitHubの当該リポジトリ名

exports.handler = async (event) => {
  // POSTメソッド以外は拒否
  if (event.httpMethod !== 'POST') {
    return { statusCode: 405, body: 'Method Not Allowed' }
  }

  const sig = event.headers['stripe-signature']
  let stripeEvent

  try {
    // 署名（STRIPE_WEBHOOK_SECRET）を検証
    // Stripeからのリクエストであることを確認
    stripeEvent = stripe.webhooks.constructEvent(
      event.body,
      sig,
      process.env.STRIPE_WEBHOOK_SECRET
    )
  } catch (err) {
    console.error(`Webhook Error: ${err.message}`)
    return { statusCode: 400, body: `Webhook Error: ${err.message}` }
  }

  // 決済完了イベント（checkout.session.completed）の処理
  if (stripeEvent.type === 'checkout.session.completed') {
    const session = stripeEvent.data.object

    try {
      // STRIPE_API_SECRETの権限で、決済時の情報を取得
      const lineItems = await stripe.checkout.sessions.listLineItems(session.id)

      const itemsPurchased = lineItems.data.map((item) => ({
        name: item.description,
        quantity: item.quantity,
        sku: item.price.product // Stripeで設定した商品ID
      }))

      // GitHub Actions（Repository Dispatch）を起動
      const githubResponse = await fetch(
        `https://api.github.com/repos/${GITHUB_OWNER}/${GITHUB_REPO}/dispatches`,
        {
          method: 'POST',
          headers: {
            Authorization: `Bearer ${process.env.GITHUB_TOKEN}`,
            Accept: 'application/vnd.github.v3+json',
            'Content-Type': 'application/json',
            'User-Agent': 'Netlify-Functions'
          },
          body: JSON.stringify({
            event_type: 'stripe_payment_success',
            client_payload: {
              items: itemsPurchased,
              email: session.customer_details.email
            }
          })
        }
      )

      if (!githubResponse.ok) {
        const errorText = await githubResponse.text()
        throw new Error(
          `GitHub API error: ${githubResponse.status} ${errorText}`
        )
      }

      console.log('GitHub Actions triggered successfully')
    } catch (err) {
      console.error('Processing Error:', err)
      return { statusCode: 500, body: 'Internal Server Error' }
    }
  }

  // Stripeへの成功レスポンス
  return {
    statusCode: 200,
    body: JSON.stringify({ received: true })
  }
}
