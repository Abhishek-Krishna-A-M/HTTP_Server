# 🧠 C HTTP Server

A lightweight, modular HTTP server built in **C** — supporting static file hosting, JSON-based configuration, SPA mode, simple API routes, CORS, and hot reload.  
Ideal for learning **network programming**, **systems design**, and how real servers work under the hood.

---

## 🚀 Features

- ✅ Serve static websites (HTML, CSS, JS)
- ✅ SPA (Single Page App) support
- ✅ Basic REST API endpoints
- ✅ Configurable using `server.json`
- ✅ Multi-threaded request handling
- ✅ Simple logging system
- ✅ Hot reload (auto reloads config)
- ✅ CORS and custom headers support
- ✅ Lightweight & minimal dependencies

---

## ⚙️ Requirements

Make sure you have the following installed:

sudo pacman -S gcc make curl
sudo pacman -S cjson  # for JSON config parsing

If cjson is not available on your system, you can install it manually:
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
make && sudo make install

To build server :
gcc src/*.c main.c -lcjson -pthread -o myserver

## Configuration(server.json)
{
  "port": 8080,
  "spa_mode": true,
  "sites": [
    {
      "host": "localhost",
      "root": "www"
    },
    {
      "host": "mysite.local",
      "root": "sites/mysite"
    }
  ]
}

port → Port number where your server listens.

spa_mode → If true, unknown routes will fallback to index.html.

sites → Virtual hosts (multiple websites from same server).

host: The domain (like localhost or mysite.local)

root: Folder path containing that site’s files.

- To start server: ./myserver

## Port Forwarding

If you want to access your local server from another device:

Enable port forwarding in your router (TCP 8080 → your device IP)

Find your local IP: ip addr show

Access from another device:
http://<your-local-ip>:8080
