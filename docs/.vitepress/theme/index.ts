import { h } from 'vue'
import DefaultTheme from 'vitepress/theme-without-fonts'
import type { Theme as ThemeConfig } from 'vitepress'
import { useData, useRoute } from 'vitepress'

import codeblocksFold from 'vitepress-plugin-codeblocks-fold'
import imageViewer from 'vitepress-plugin-image-viewer'
import vImageViewer from 'vitepress-plugin-image-viewer/lib/vImageViewer.vue'

import { ImageGroup } from '@miletorix/vitepress-image-group'
import '@miletorix/vitepress-image-group/style.css'

import CustomFeature from '../components/CustomFeature.vue'
import CustomBadge from '../components/CustomBadge.vue'
import Custom404 from './404.vue'

import { NolebaseHighlightTargetedHeading } from '@nolebase/vitepress-plugin-highlight-targeted-heading/client'
import '@nolebase/vitepress-plugin-highlight-targeted-heading/client/style.css'

import { NolebaseUnlazyImg } from '@nolebase/vitepress-plugin-thumbnail-hash/client'
import '@nolebase/vitepress-plugin-thumbnail-hash/client/style.css'

import '@nolebase/vitepress-plugin-enhanced-mark/client/style.css'

import './fonts.css'
import './colors.css'
import 'viewerjs/dist/viewer.min.css'
import 'vitepress-plugin-codeblocks-fold/style/index.css'
import 'virtual:group-icons.css'

const CustomTheme: ThemeConfig = {
  extends: DefaultTheme,
  Layout: () => {
    return h(DefaultTheme.Layout, null, {
      'layout-top': () => h(NolebaseHighlightTargetedHeading),
      'not-found': () => h(Custom404)
    })
  },
  enhanceApp: (ctx) => {
    ctx.app.component('vImageViewer', vImageViewer)
    ctx.app.component('ImageGroup', ImageGroup)
    ctx.app.component('CustomFeature', CustomFeature)
    ctx.app.component('CustomBadge', CustomBadge)
    ctx.app.component('NolebaseUnlazyImg', NolebaseUnlazyImg)
  },
  setup() {
    const route = useRoute()
    const { frontmatter } = useData()
    imageViewer(route)
    codeblocksFold({ route, frontmatter }, true, 500)
  }
}

export default CustomTheme
