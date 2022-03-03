const cacheName = "cache1"; // Change value to force update

self.addEventListener("install", event => {
  // Kick out the old service worker
  self.skipWaiting();

  event.waitUntil(
    caches.open(cacheName).then(cache => {
      return cache.addAll([
        "/",
        "index.html", // Main HTML file
        "icon.png", // Logo
        "main.js", // Main Javascript file
        "manifest.json", // Manifest file
        "style.css", // Main CSS file
      ]);
    })
  );
});

self.addEventListener("activate", event => {
  // Delete any non-current cache
  event.waitUntil(
    caches.keys().then(keys => {
      Promise.all(
        keys.map(key => {
          if (![cacheName].includes(key)) {
            return caches.delete(key);
          }
        })
      );
    })
  );
});

// PWA offline support
self.addEventListener("fetch", event => {
  if (event.request.url === window.location.origin)
    event.respondWith(
      caches.match(event.request).then(response => {
        return response || fetch({ ...event.request });
      })
    );
});
