import path from 'node:path';
import { defineConfig, loadEnv } from 'vite';

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, '.', '');
  return {
    define: {
      'process.env.API_KEY': JSON.stringify(env.GEMINI_API_KEY),
      'process.env.GEMINI_API_KEY': JSON.stringify(env.GEMINI_API_KEY)
    },
    resolve: {
      alias: {
        '@': path.resolve(process.cwd(), '.'),
      }
    },
    server: {
      port:5173,
      open: true
    },
    build: {
      rollupOptions: {
        input: {
          main: path.resolve(process.cwd(), 'index.html')
        }
      }
    }
  };
});
