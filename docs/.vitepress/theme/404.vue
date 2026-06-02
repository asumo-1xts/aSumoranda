<script setup>
import DefaultTheme from 'vitepress/theme'
const { Layout } = DefaultTheme
</script>

<template>
  <Layout>
    <template #not-found>
      <div class="not-found-container">
        <div class="illustration">
          <img src="/meow.webp" alt="404" />
        </div>
        <h1 class="code">󱄁 404</h1>
        <p class="message">Page Not Found.&emsp;<a href="/">Home</a></p>
      </div>
    </template>
  </Layout>
</template>

<style scoped>
@import './colors.css';
@import './fonts.css';

.not-found-container {
  padding: 25px 25px;
  text-align: center;
}

.code {
  font-family: 'myCodeFont', sans-serif;
  font-size: 100px;
  margin: 75px 0 0 0;
}

.message {
  font-family: 'myMainFont', sans-serif;
  font-size: 20px;
  margin: 75px 0 0 0;
  line-height: 2;
}
.message a {
  color: var(--vp-c-brand-1); /* 例: VitePressのブランドカラーを使用 */
  text-decoration: underline;
  transition: all 0.25s;
}
.message a:hover {
  color: var(--vp-c-brand-2); /* hover時にブランドのダークカラー */
}

.illustration {
  display: flex;
  justify-content: center;
  align-items: center;
}
.illustration img {
  max-width: 300px;
  width: 100%;
  height: auto;
}
</style>
