import { defineConfig } from "vite"
import solidPlugin from "vite-plugin-solid"
import { viteSingleFile } from "vite-plugin-singlefile"
import calory from "vite-plugin-calory"

export default defineConfig({
  plugins: [
    solidPlugin(),
    viteSingleFile(),
    calory(),
  ],
  server: {
    port: 3000,
  },
  build: {
    target: "esnext",
    assetsInlineLimit: Number.MAX_SAFE_INTEGER,
  },
})
