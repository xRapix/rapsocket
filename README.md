# 🚀 RapSocket v1
A lightweight, header-only, RAW C++ network library built directly on top of the Windows HTTP Services (WinHTTP) API. No external dependencies, no complex setups, no modern standard library bloat if you don't want it. Just include and send.

## ✨ Features
📦 Header-Only: Just drop RapSocket.hpp into your project and you are good to go!

🔋 Zero Dependencies: No libcurl, no Boost.Asio, no CMake nightmares. Uses pure Windows Native API.

🛠️ RAW Programming: Custom high-performance URL parser built from scratch without standard string find utilities.

⚡ Ultra Lightweight: Perfect for low-level tools, game hooks, or independent loaders where executable size matters.

## 💻 Quick Start
### C++
```
#include "rapsocket.hpp"

int main() {
    // Drop the text and forget about the server's response!
    PostTextAndForget("YOUR_URL_HERE", "Hello from RapSocket!");
}
```
## 📜 License
MIT License - Feel free to use it in any project!

## 🗺️ Roadmap / Future Plans
Here is what's coming next in the future versions of RapSocket:

[x] v1.0.0 - Ultra-lightweight POST request without caring about the response (PostTextAndForget).

[ ] v1.1.0 - POST with response handling (PostTextAndListenClosely) – reading and buffer allocation for server response.

[ ] v2.0.0 - Full GET request support – fetching data from any server.

[ ] v3.0.0 - No-STD / Pure Native WinAPI version – completely removing <string> and modern C++ standard library dependencies for malware analysts and low-level enthusiasts (executable size < 100KB).

## ⚙️ Installation
Since RapSocket is a **header-only** library, setup takes exactly 5 seconds.

### Method 1: Manual
1. Download `rapsocket.hpp` and drop it into your project folder.

### Method 2: Terminal (Fastest)
Run one of these commands inside your project directory to download the header directly:

# Using curl
```
curl -O https://raw.githubusercontent.com/xRapix/RapSocket/main/rapsocket.hpp
```

# Using wget
```
wget https://raw.githubusercontent.com/xRapix/RapSocket/main/rapsocket.hpp
```
