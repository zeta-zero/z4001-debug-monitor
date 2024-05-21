import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { createHtmlPlugin } from 'vite-plugin-html';

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [vue()],
    resolve: {
        alias: {
            echarts: 'https://cdnjs.cloudflare.com/ajax/libs/echarts/5.5.0/echarts.min.js'
        },
    },
});