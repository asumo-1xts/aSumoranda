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
  typeof b === 'string'
    ? {
        text: b
      }
    : b
</script>

<template>
  <div class="custom-features">
    <!-- 制御ボタン -->
    <div class="controls">
      <button
        @click="toggleAutoPlay"
        class="play-pause-btn"
        :class="{
          'is-paused': !isAutoPlaying
        }"
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
            <div class="icon">
              {{ feature.icon }}
            </div>
            <div class="content">
              <a
                :href="feature.link"
                :target="feature.target"
                :rel="feature.rel"
                class="title-link"
              >
                <h3 class="title">
                  {{ feature.title }}
                </h3>
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
                      :class="{
                        'has-link': normalizeBadge(b).link
                      }"
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

<style>
@import '../theme/feature.css';
</style>
