import { createContentLoader } from 'vitepress'

export default createContentLoader('tags/*.md', {
  includeSrc: false,
  transform(rawData) {
    return rawData
      .filter((page) => page.frontmatter.order <= 10)
      .sort(
        (a, b) =>
          +new Number(a.frontmatter.order) - +new Number(b.frontmatter.order)
      )
  }
})
