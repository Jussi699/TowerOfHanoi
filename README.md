# Tower of Hanoi (raylib C/C++)

Interactive 3D Tower of Hanoi game with particles, music, camera controls, and achievement tracking.

## 🛠️ Build Instructions

So far there is only one way to build a project

You may get an error related to **"Intelligent Application Managemen"**, in order to still launch the program, you can:    
``Windows Defender -> App & browser -> Intelligent Application Control service settings -> Disable``  
This is strongly **NOT RECOMMENDED**

№1 ---> Just run TowerOfHanoi.slnx and **Build** -> **F5**. If it doesn't work, go to №2.
   
№2 ---> Visual Studio Method: NuGet
2. **Install raylib via NuGet:**
1. `Visual Studio → Tools → NuGet Package Manager → Manage NuGet Packages for Solution`
2. `Browse → Search "raylib" → Install`
3. Go to Solution Properties -> C/C++ -> Additional Include Directories -> Paste `$(ProjectDir)src\modules;$(ProjectDir)src\components`
4. **Build → Build Solution** (Ctrl+Shift+B)
5. **F5** to run
   
## 🎮 Controls
| Key | Action |
|-----|--------|
| **1/2** | Difficulty (4/3 Towers) |
| **R** | Restart/Help |
| **W** | Undo |
| **M** | Music toggle |
| **V** | Last Score |
| **F11** | Fullscreen |
| **A/D** | Camera Rotation |
| **RMB** | Mouse Camera |

## ✨ Features
- 🎮 Two difficulty modes (3/4 towers)
- ✨ Victory particle animations
- 🏆 Star rating system (3★ <240s, 2★ <360s, 1★)
- 💾 Results saved to `Result/victory_results.txt`
- 🎵 Background music toggle
- 📱 Fullscreen (F11), undo (W), camera controls
- 🧪 Automated testing & logging
  
## Project Structure

```
Tower-Of-Hanoi/
├── Logs/
│   └── app.txt
│
├── Result/
│   └── victory_results.txt
│
├── Tower-Of-Hanoi.sln
│
├── src/
│   ├── components/
│   │   ├── BaseClass.h
│   │   ├── DiskStruct.h
│   │   ├── Logger.h
│   │   ├── MainGame.h
│   │   ├── Music.h
│   │   ├── Particles.h
│   │   ├── Test.h
│   │   └── Textures.h
│   │
│   ├── modules/
│   │   ├── Logger.cpp
│   │   ├── MainGame.cpp
│   │   ├── Music.cpp
│   │   ├── Particles.cpp
│   │   ├── Test.cpp
│   │   ├── Textures.cpp
│   │   └── main.cpp
│   │
│   ├── font/
│   │   └── Snowline.otf
│   │
│   ├── music/
│   │   └── Game.mp3
│   │
│   └── picture/
│       └── space.jpg
│
└── main.cpp
```

## 🎨 Assets

Picture:
"Designed by Davidzydd / Freepik" Licence: Freepik License

## 🚀 Quick Demo
- git clone https://github.com/Jussi699/Tower-Of-Hanoi
- cd Tower-Of-Hanoi
- Open Tower-Of-Hanoi.slnx in Visual Studio (Recommended: Visual Studio 2026)
- Build -> F5

## 🤝 Contributing
I would be glad to receive your assistance in improving the game.

## License
License: MIT
Copyright (c) 2026 [Korolov Artur]
