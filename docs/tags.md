---
layout: doc

title: All tags

permalink: 'https://blog.asumo.dev/tags.html'

prev: false
next: false

editLink: false
lastUpdated: false
---

<style>
.column-left{
  float: left;
  width: 50%;
  text-align: left;
}
.column-right{
  float: right;
  width: 50%;
  text-align: left;
}
.column-one{
  float: left;
  width: 100%;
  text-align: left;
}
</style>

<script lang="ts" setup>
    import { data as tags1 } from "./.vitepress/tags1.data"
    import { data as tags2 } from "./.vitepress/tags2.data"
    import PostCounter from "./.vitepress/components/PostCounter.vue"
</script>

# All tags

<br/>

<div class="column-left">

<ul style="list-style: none; padding-left: 0;">
    <li v-for="tagpage of tags1">
        <a :href="`${tagpage.url}`" class="font-semibold text-lg">
            <Badge type="tag" :text="tagpage.frontmatter.title" />
        </a>
        <span class="text-sm">
            (<PostCounter :tag="tagpage.url.replace('/tags/', '').replace('.html', '')"/>)
        </span>
    </li>
</ul>

</div>

<div class="column-right">

</div>

<ul style="list-style: none; padding-left: 0;">
    <li v-for="tagpage of tags2">
        <a :href="`${tagpage.url}`" class="font-semibold text-lg">
            <Badge type="tag" :text="tagpage.frontmatter.title" />
        </a>
        <span class="text-sm">
            (<PostCounter :tag="tagpage.url.replace('/tags/', '').replace('.html', '')"/>)
        </span>
    </li>
</ul>
