import { h, onMounted } from 'vue'
import { useData, useRoute } from 'vitepress'
import DefaultTheme from 'vitepress/theme-without-fonts'
import type { Theme as ThemeConfig } from 'vitepress'

// VitePress plugins
import codeblocksFold from 'vitepress-plugin-codeblocks-fold'
import imageViewer from 'vitepress-plugin-image-viewer'
import vImageViewer from 'vitepress-plugin-image-viewer/lib/vImageViewer.vue'

// Nolebase plugins
import { NolebaseHighlightTargetedHeading } from '@nolebase/vitepress-plugin-highlight-targeted-heading/client'
import { NolebaseUnlazyImg } from '@nolebase/vitepress-plugin-thumbnail-hash/client'

// Image group
import { ImageGroup } from '@miletorix/vitepress-image-group'

// Custom components
import CustomFeature from '../components/CustomFeature.vue'
import CustomBadge from '../components/CustomBadge.vue'
import Custom404 from './404.vue'

// Styles
import './fonts.css'
import './colors.css'

import '@nolebase/vitepress-plugin-highlight-targeted-heading/client/style.css'
import '@nolebase/vitepress-plugin-thumbnail-hash/client/style.css'
import '@nolebase/vitepress-plugin-enhanced-mark/client/style.css'

import '@miletorix/vitepress-image-group/style.css'
import 'viewerjs/dist/viewer.min.css'
import 'vitepress-plugin-codeblocks-fold/style/index.css'
import 'virtual:group-icons.css'

const analyticsId = 'G-SRVS9XNT7N'

function loadAnalytics() {
  const scriptId = 'google-analytics-script'
  if (document.getElementById(scriptId)) return

  const windowWithAnalytics = window as typeof window & {
    dataLayer: unknown[]
    gtag: (...args: unknown[]) => void
  }

  windowWithAnalytics.dataLayer = windowWithAnalytics.dataLayer || []
  windowWithAnalytics.gtag = (...args) => {
    windowWithAnalytics.dataLayer.push(args)
  }

  windowWithAnalytics.gtag('js', new Date())
  windowWithAnalytics.gtag('config', analyticsId, {
    page_path: `${location.pathname}${location.search}${location.hash}`
  })

  const script = document.createElement('script')
  script.id = scriptId
  script.async = true
  script.src = `https://www.googletagmanager.com/gtag/js?id=${analyticsId}`

  document.head.appendChild(script)
}

const CustomTheme: ThemeConfig = {
  extends: DefaultTheme,

  Layout: () =>
    h(DefaultTheme.Layout, null, {
      'layout-top': () => h(NolebaseHighlightTargetedHeading),
      'not-found': () => h(Custom404)
    }),

  enhanceApp: ({ app }) => {
    app.component('vImageViewer', vImageViewer)
    app.component('ImageGroup', ImageGroup)
    app.component('CustomFeature', CustomFeature)
    app.component('CustomBadge', CustomBadge)
    app.component('NolebaseUnlazyImg', NolebaseUnlazyImg)
  },

  setup() {
    const { frontmatter } = useData()
    const route = useRoute()

    imageViewer(route)
    codeblocksFold({ route, frontmatter })

    onMounted(() => {
      window.setTimeout(loadAnalytics, 2000)
    })
  }
}

export default CustomTheme
