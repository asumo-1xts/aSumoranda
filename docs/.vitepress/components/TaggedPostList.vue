<script lang="ts" setup>
import { data as posts } from '../posts.data'
const props = defineProps(['tag'])
const formatDate = (date: string) => date.slice(0, 10).replaceAll('-', '/')
let taggedPosts = posts.filter((page) =>
  page.frontmatter.tags
    .toString()
    .replaceAll(' ', '')
    .toLowerCase()
    .includes(props.tag)
)
</script>

<template>
  <ul style="list-style: none; padding-left: 0">
    <li v-for="post of taggedPosts">
      <span class="text-sm" style="font-family: myCodeFont">
        {{ formatDate(post.frontmatter.date) }}
        {{ post.frontmatter.emoji }}&nbsp;</span
      >
      <a :href="`${post.url}`" class="font-semibold text-lg"
        >{{ post.frontmatter.title }}
      </a>
    </li>
  </ul>
</template>

<style>
@import '../theme/fonts.css';
</style>
