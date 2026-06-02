<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'

interface Badge {
  text: string
  link?: string
}

interface Feature {
  icon: string
  title: string
  link: string
  badges?: (string | Badge)[]
  target?: string
  rel?: string
  date: string
}

const props = defineProps<{
  features: Feature[]
}>()

const visibleCount = 4
const visibleIndices = ref<number[]>([])
const isAutoPlaying = ref(false) // デフォルトでは停止
let timer: number | null = null

const shuffle = () => {
  const all = props.features.map((_, i) => i)
  visibleIndices.value = all
    .sort(() => Math.random() - 0.5)
    .slice(0, visibleCount)
}

const startTimer = () => {
  if (timer) clearInterval(timer)
  timer = window.setInterval(shuffle, 100)
  isAutoPlaying.value = true
}

const stopTimer = () => {
  if (timer) {
    clearInterval(timer)
    timer = null
  }
  isAutoPlaying.value = false
}

const toggleAutoPlay = () => {
  if (isAutoPlaying.value) {
    stopTimer()
  } else {
    shuffle() // 切り替え時に一度シャッフル
    startTimer()
  }
}

const displayFeatures = computed(() => {
  if (visibleIndices.value.length === 0) return props.features
  return visibleIndices.value.map((i) => props.features[i])
})

onMounted(() => {
  shuffle()
})

onUnmounted(() => {
  stopTimer()
})

const normalizeBadge = (b: string | Badge): Badge =>
  typeof b === 'string' ? { text: b } : b
</script>

<template>
  <div class="custom-features">
    <!-- 制御ボタン -->
    <div class="controls">
      <button
        @click="toggleAutoPlay"
        class="play-pause-btn"
        :class="{ 'is-paused': !isAutoPlaying }"
      >
        <span class="btn-icon">{{
          isAutoPlaying ? '&nbsp;' : '&nbsp;'
        }}</span>
        <span class="btn-text">{{
          isAutoPlaying ? 'ストップ' : 'おみくじ'
        }}</span>
      </button>
    </div>

    <div class="container">
      <div class="items">
        <div
          v-for="feature in displayFeatures"
          :key="feature.title"
          class="item-wrapper"
        >
          <div class="item">
            <div class="icon">{{ feature.icon }}</div>
            <div class="content">
              <a
                :href="feature.link"
                :target="feature.target"
                :rel="feature.rel"
                class="title-link"
              >
                <h3 class="title">{{ feature.title }}</h3>
              </a>
              <div v-if="feature.badges?.length" class="meta-line">
                <div class="badge-list">
                  <template
                    v-for="b in feature.badges"
                    :key="typeof b === 'string' ? b : b.text"
                  >
                    <component
                      :is="normalizeBadge(b).link ? 'a' : 'span'"
                      :href="normalizeBadge(b).link"
                      class="VPBadge tag"
                      :class="{ 'has-link': normalizeBadge(b).link }"
                    >
                      {{ normalizeBadge(b).text }}
                    </component>
                  </template>
                </div>
              </div>
              <div class="date-line">
                <span class="date">{{
                  feature.date?.replace(/T.*/, '') || feature.date
                }}</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
@import '../theme/badge.css';

/* 制御ボタン */
.controls {
  position: sticky;
  display: flex;
  justify-content: flex-start;
  margin-bottom: 12px;
}
.play-pause-btn {
  font-family: 'myCodeFont', monospace;
  font-size: medium;
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 4px 12px;
  border-radius: 20px;
  border: 1px solid var(--vp-c-divider);
  background-color: var(--vp-c-bg);
  color: var(--vp-c-brand-1);
  transition: all 0.25s;
  cursor: pointer;
}
.play-pause-btn:hover {
  border-color: var(--vp-c-brand-1);
}
.play-pause-btn:not(.is-paused) {
  color: var(--vp-c-red-1);
}
.play-pause-btn:not(.is-paused):hover {
  border-color: var(--vp-c-red-1);
}

/* Features */
.custom-features {
  margin: 1.2rem 0;
  width: 100%;
}
.items {
  display: grid;
  gap: 12px;
  grid-template-columns: repeat(auto-fit, minmax(min(240px, 100%), 1fr));
}
.item {
  position: relative;
  display: flex;
  align-items: center;
  gap: 14px;
  border: 1px solid var(--vp-c-bg-soft);
  border-radius: 12px;
  padding: 12px 16px;
  background-color: var(--vp-c-bg-soft);
  transition: all 0.2s; /* ホバー時の色の変化などは残しておくと綺麗です */
  min-width: 0;
}
.item:hover {
  border-color: var(--vp-c-brand-1);
}
.icon {
  flex-shrink: 0;
  font-size: xx-large;
  line-height: 1;
}
.content {
  flex-grow: 1;
  min-width: 0;
  display: flex;
  flex-direction: column;
}
.title-link {
  text-decoration: none;
  color: inherit;
}
.title-link::after {
  content: '';
  position: absolute;
  top: 0;
  right: 0;
  bottom: 0;
  left: 0;
  z-index: 1;
}
.title {
  margin: 0 0 0 0.33rem !important;
  font-family: 'mySubFont', sans-serif;
  font-size: medium;
  font-weight: 600;
  line-height: 1.4;
  color: var(--vp-c-text-1);
  display: -webkit-box;
  line-clamp: 2;
  overflow: hidden;
  word-break: break-word;
}
.meta-line {
  display: flex;
  align-items: flex-start;
  position: relative;
  z-index: 2;
  pointer-events: none;
  min-height: 64px;
}
.badge-list {
  margin-top: 8px;
  display: flex;
  flex-wrap: wrap;
}
:deep(.VPBadge) {
  margin-top: 4px;
  text-decoration: none;
  pointer-events: none;
}
.VPBadge.has-link {
  pointer-events: auto;
}

/* Date */
.date-line {
  width: 100%;
  text-align: right;
  margin: 0 0.33rem 0 0 !important;
  font-size: small;
  color: var(--vp-c-text-2);
}
</style>
