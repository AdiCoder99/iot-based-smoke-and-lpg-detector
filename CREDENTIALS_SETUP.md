# Credentials Setup Guide

## ⚠️ IMPORTANT SECURITY NOTICE

This project uses sensitive credentials for WiFi and Firebase. **NEVER commit credentials to the repository.**

## Files Overview

- **`main_template.cpp`** - Template file with placeholder credentials (safe to share)
- **`main.cpp`** - Your working file with actual credentials (DO NOT COMMIT)
- **`.gitignore`** - Configured to prevent accidental commits of sensitive files

## How to Set Up Your Credentials

### Step 1: Copy Template to Working File

If you're starting fresh:
```bash
cp main_template.cpp main.cpp
```

### Step 2: Fill in WiFi Credentials

Open `main.cpp` and find these lines:
```cpp
const char* WIFI_SSID     = "YOUR_WIFI_SSID_HERE";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD_HERE";
```

Replace with your actual WiFi details:
```cpp
const char* WIFI_SSID     = "Your Network Name";
const char* WIFI_PASSWORD = "Your WiFi Password";
```

### Step 3: Set Up Firebase

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project or select existing one
3. Create a Realtime Database
4. Get your Project ID from the database URL
5. Get your Database Secret from Project Settings → Service Accounts → Database Secrets

### Step 4: Fill in Firebase Credentials

Find these lines in `main.cpp`:
```cpp
const char* FIREBASE_HOST    = "https://YOUR_PROJECT_ID.firebaseio.com";
const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET_HERE";
```

Replace with your actual Firebase details:
```cpp
const char* FIREBASE_HOST    = "https://mq2-sensor-project-default-rtdb.firebaseio.com";
const char* FIREBASE_API_KEY = "KDEEjIYOhAYjbI2kuFBimGbOMf7WMK9n246NvvNS";
```

### Step 5: Upload to Arduino IDE

- Open `main.cpp` (or `main_template.cpp` if you renamed it)
- Make sure you've filled in all credentials
- Upload to your ESP8266

## If Credentials Were Accidentally Exposed

If you accidentally committed credentials to GitHub:

### Immediate Actions:
1. **Regenerate Firebase API Key**
   - Firebase Console → Project Settings → Service Accounts → Database Secrets
   - Click the ⚠️ icon next to your key
   - Click "Regenerate" to create a new secret
   - Update your `main.cpp` with the new key

2. **Change WiFi Password**
   - Update your router settings
   - Update the new password in `main.cpp`

3. **Remove from Git History** (if already committed)
   ```bash
   # Rewrite history to remove credentials
   git filter-branch --force --index-filter \
   'git rm --cached --ignore-unmatch main.cpp' \
   --prune-empty --tag-name-filter cat -- --all
   
   git push origin --force --all
   ```

## Development vs Production

### During Development:
- Use credentials directly in code (keep locally only)
- Test mode Firebase rules: `".read": true, ".write": true`
- Use `client.setInsecure()` in code

### Before Production:
- Implement proper authentication system
- Use restrictive Firebase rules with authentication
- Remove `client.setInsecure()` and use proper SSL verification
- Consider using environment variables or a separate config file
- Never expose credentials in public repositories

## Best Practices

✅ **DO:**
- Keep credentials locally only
- Use `.gitignore` to prevent commits
- Regenerate keys if exposed
- Use template files for sharing
- Version control only template files
- Add credentials to `.gitignore` before first commit

❌ **DON'T:**
- Commit credentials to GitHub
- Share credentials in email or messaging
- Use the same credentials across projects
- Keep old exposed credentials active
- Hardcode credentials in production
- Push code with real credentials

## Using .gitignore

The `.gitignore` file is already configured to ignore:
- `main.cpp` (with your credentials)
- `config.h`, `secrets.h` files
- Firebase configuration files
- SSL certificates and keys
- `.env` files

If you create other credential files, add them to `.gitignore`:
```bash
echo "my_secrets.h" >> .gitignore
```

## Verify .gitignore is Working

Before committing:
```bash
# See what would be committed
git status

# Verify main.cpp is NOT listed
# Only main_template.cpp and README files should appear
```

## Quick Reference

| File | Status | Purpose |
|------|--------|---------|
| `main_template.cpp` | ✅ COMMIT | Template with placeholders |
| `main.cpp` | ❌ DO NOT COMMIT | Your working file with credentials |
| `README.md` | ✅ COMMIT | Project documentation |
| `.gitignore` | ✅ COMMIT | Ignore rules |
| `CREDENTIALS_SETUP.md` | ✅ COMMIT | This guide |

## Need Help?

If you have questions about securing credentials:
1. Check the `.gitignore` file
2. Review this guide again
3. Consult Firebase security documentation
4. Never share credentials in issues or discussions

---

**Remember: Credentials are like passwords - keep them private and secure!** 🔒
