# Networking By Zuzia - Web App

Progressive Web App version of the Contact Networking System with Firebase backend.

## Features

✨ **Full Feature Parity with C Version:**
- 🔍 Search contacts (by ID, name, hashtags, event)
- 📋 List contacts (full or brief)
- 🗑️ Delete contacts (with duplicate detection)
- ➕ Add new contacts with auto-formatting
- 📱 **PWA Support** - Install as native app
- ☁️ **Firebase Cloud Storage** - Access from anywhere
- 🔄 **Offline Support** - Works without internet

## Setup Instructions

### 1. Install Dependencies

```bash
cd web-app
npm install
```

### 2. Configure Firebase

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project (or use existing)
3. Enable **Firestore Database**:
   - Go to Firestore Database
   - Click "Create database"
   - Start in **test mode** (for development)
   - Choose a location

4. Get your Firebase config:
   - Go to Project Settings (⚙️ icon)
   - Scroll to "Your apps"
   - Click "Web" (</> icon)
   - Copy the `firebaseConfig` object

5. Update `src/services/firebase.js`:
   - Replace the `firebaseConfig` object with your own

### 3. Run Development Server

```bash
npm run dev
```

Open http://localhost:5173 in your browser

### 4. Build for Production

```bash
npm run build
```

### 5. Deploy to Firebase Hosting (Optional)

```bash
# Install Firebase CLI
npm install -g firebase-tools

# Login
firebase login

# Initialize
firebase init hosting

# Deploy
firebase deploy
```

## Project Structure

```
web-app/
├── src/
│   ├── services/
│   │   ├── firebase.js          # Firebase configuration
│   │   └── contactService.js    # Business logic (from C)
│   ├── utils/
│   │   └── formatters.js        # Text formatting utilities
│   ├── main.js                  # Main app logic
│   └── style.css                # Styling
├── public/                      # Static assets
├── index.html                   # Main HTML
├── package.json                 # Dependencies
└── vite.config.js              # Vite + PWA config
```

## Firebase Firestore Structure

```
contacts (collection)
  └── {auto-generated-id} (document)
      ├── id: number
      ├── name: string
      ├── surname: string
      ├── note: string
      ├── advantage: string
      ├── source_type: string ("event" | "contact" | "")
      ├── source_value: string
      └── hashtags: array<string>
```

## Auto-Formatting Features

Just like the C version:
- **Names & Surnames**: First letter capitalized, rest lowercase
- **Hashtags**: All lowercase
- **Source Values**: First letter capitalized

## PWA Features

- Install on any device (mobile, desktop)
- Offline support with service workers
- Fast loading with caching
- Native app-like experience

## Browser Support

- Chrome/Edge (recommended)
- Firefox
- Safari
- Any modern browser with PWA support

## Notes

⚠️ **Security**: The Firebase config uses test mode. For production:
1. Set up proper Firestore security rules
2. Enable authentication if needed
3. Restrict API keys

## License

MIT
