---
layout: home

title: Home

hero:
  name: aSumoranda
  tagline: ｱｽﾓのﾒﾓﾗﾝﾀﾞ、ｱｽﾓﾗﾝﾀﾞ
  image:
    src: cover01.webp
    alt: aSumoranda

permalink: 'https://blog.asumo.dev/'
---

<script setup lang="ts">
import { data as posts } from '/.vitepress/posts.data'
import CustomFeature from '/.vitepress/components/CustomFeature.vue'

const tagMap: Record<string, string> = {
  appdev: 'アプリ開発',
  arduino: 'Arduino',
  ccpp: 'C/C++',
  environment: '環境構築',
  fixpedals: 'エフェクター修理',
  jsts: 'JS/TS',
  justnote: '雑記',
  kicad: 'KiCad',
  latex: 'LaTeX',
  makepedals: 'エフェクター自作',
  midi: 'MIDI',
  modpedals: 'エフェクター改造',
  otherdiy: 'その他の工作',
  pic: 'PIC',
  python: 'Python',
  trouble: 'トラブルシュート',
  webdev: 'Web開発',
}

const features = posts
  .map(p => ({
    icon: p.frontmatter.emoji,
    title: p.frontmatter.title,
    link: p.url,
    badges: (p.frontmatter.tags)
      .filter((t: string) => !t.startsWith('post'))
      .map((t: string) => ({
        text: tagMap[t] || t,
        link: `/tags/${t}`
      })),
    date: p.frontmatter.date
  }))
</script>

::: info
急いでお伝えしたいことは特にありません。
:::

---

<CustomFeature :features="features" />
