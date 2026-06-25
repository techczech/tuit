/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        bohemica: {
          blue: '#000099',
          yellow: '#ffcc00',
          darkRed: '#aa0000',
          darkGray: '#aaaaaa',
          lightGray: '#eeeeee',
          text: '#000000',
        }
      },
      fontFamily: {
        sans: ['Arial', 'Helvetica', 'sans-serif'],
      }
    },
  },
  plugins: [],
}
