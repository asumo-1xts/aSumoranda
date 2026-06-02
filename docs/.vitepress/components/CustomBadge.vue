<script setup lang="ts">
interface Props {
  text?: string
  type?: 'tag'
}
withDefaults(defineProps<Props>(), {
  type: 'tag'
})
</script>

<template>
  <span class="VPBadge" :class="type">
    <slot>{{ text }}</slot>
  </span>
</template>

<style>
@import '../theme/badge.css';
</style>
