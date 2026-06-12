# 🛠 **Open-Source** [FunGun] EasyCheatDetector Mapper
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![GitHub forks](https://img.shields.io/github/forks/Ml-skibidist/ecd_sh1t?style=flat-square&color=blue)
![Status](https://img.shields.io/badge/Status-Experimental-orange?style=flat-square)
![Base](https://img.shields.io/badge/Base%20Version-ECD%201087%20Detects-blueviolet?style=flat-square)

[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)
[![Made with C++](https://img.shields.io/badge/Made%20with-C%2B%2B-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)]()
[![Platform Windows](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)]()

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&pause=1000&color=FFA500&center=true&vCenter=true&width=435&lines=%F0%9F%9B%A0%EF%B8%8F+EasyCheatDetector+Mapper;Reverse+Engineering+Toolkit;For+Educational+Purposes" alt="Typing SVG" />
</p>

> [!IMPORTANT]
> Это **тестовый форк** против проекта [FunGun EasyCheatDetector (ECD)](https://github.com/UnrealKaraulov/EasyCheatDetector).
>
> **Назначение:** тестирование, дебаггинг, реверс-инжиниринг, изучение работы сканера\мапперов\методов детекта.

## 📑 Содержание
- [🤔 Зачем этот форк?](#-зачем-этот-форк)
- [🤨 Что делает этот форк?](#-что-делает-форк)
- [⚙️ Что внутри?](#-что-внутри)
- [🎢 Режимы маппинга](#-режимы-маппинга)
- [📞 Дисклеймер](#-дисклеймер)

## 🤔 Зачем этот форк?
Этот форк нужен для понимания полной картины работы сканеров во всех играх.
### Основные цели форка
1. Этот форк нужен для **тестирования**, **дебаггинга**, **реверс-инжиниринга** и **изучение работы сканера\мапперов\методов детекта**, а так-же **анализа** сканера.
2. Понимание сути работы методов детекта, их обходов, а также эксплуатаций.
3. Также является форком оригинального проекта ecd_sh1t заточенного под игру **cs 1.6** - [Тык](https://github.com/exzereyddi/ecd_sh1t)

### Как этот форк помогает
| Цель | Как достигается |
|---|---|
| 🤖 **Изучение и анализ** | Каждый тип маппинга **(`UserDir`, `GameDir`, `Pattern`)** можно включать изолированно друг от друга и тестировать разные методы детекта  |
| 🧪 **Обходы и эксплуатации** | Добавляешь детект → смотришь, загорелся ли он в сканере. Добавляешь HWID -> смотришь, украл ли ты чью-то сессию в БД |
| 🎯 **Сигнатуры и паттерны** | Все детекты вынесены в массив строк, можно комментировать или редактировать без ограничений |
| 🛠️ **Кастомизация** | Легко добавить свои тестовые сигнатуры без правки логики |
---

## 🤨 Что делает этот форк?
Этот форк предоставляет возможность глубокого исследования всех сканеров, для всех видов игр. Вы можете смело его использовать против любой игры которая использует систему детектов, также добавлять свои новые тестовые методы и изучать работу сканеров.

Форк изначально написан под сканер **[[FunGun] EasyCheatDetector](https://github.com/UnrealKaraulov/EasyCheatDetector)** который сканирует систему на наличие следов от читов для разных игр. Делает это путём сканирование: **UserDir**, **GameDir**, **RegeditDir**, и другими методами. Делает это разными способами детекта: **asmjit**, **regkeys**, **jsonkeys**, **mappedMemory** и так-далее. Ниже предоставлены методы которые можно **абузить** для получения детекта в БД или скрытии от самого сканера.

### Методы детекта
| Категория | Примеры | Кол-во |
|---|---|---|
| 🔑 **Названия читов** | **NorAdrenaline**, **Midnight**, **Interium**, SultanHack, FluidAim, Zero, Valthrun, Aristois... | 100+ |
| 💬 **Классические сигнатуры** | `You are injected!`, `Cheat injected :3`, `Press INSERT to open settings` | 20+ |
| 🐞 **Отладочные PDB-пути** | `Release\AlterNative.pdb`, `NOT_HPP.pdb`, `xHancer.pdb` | 30+ |
| 🧩 **Функции меню** | `Menu/Aimbot - Fakelag`, `Knifebot`, `Bunnyhop`, `Edgebug`, `Strafe Helper` | 50+ |
| 📞 **Контакты авторов** | `t.me/animstate`, `vk.com/alternativehack`, `youtube.com/c/Sayuri7Mad` | 50+ |
| ⚙️ **Технические сигнатуры** | `aimbot.fov.distance.based`, `knifebot_bot_bypass`, `speedhack_active` | 300+ |
| 🎨 **OpenGL/DirectX хуки** | `glColor4f`, `wglSwapBuffers`, `R_GLStudioDrawPoints` | 100+ |
| 🎯 **Паттерны байткода** | `E8 ?? ?? ?? ?? 83 C4 04 8B 0D`, `48 8B 05 ?? ?? ?? ?? FF D0` | 50+ |
  
> Несмотря на наличие сигнатур для CS2, Minecraft и других игр, оригинал проверяет запущенную игру (`hl.exe` / `hw.dll`)!
> Поэтому реальных детектов на CS 1.6 — **ровно 131**!

## ⚙️ Что внутри?
Проект использует **три независимых методов маппера**, каждый из которых **оставляет в системе определённый след или метку** для сканера. Ниже предоставлены три независимых метода которые используются против **[[FunGun] EasyCheatDetector](https://github.com/UnrealKaraulov/EasyCheatDetector)**.

### 📁 `UserDirMapper`
Создаёт и удаляет папку-маркер в `%APPDATA%`.
### 📁 `GameDirMapper`
Создаёт и удаляет папку-маркер в относительном пути игры от любого модуля игры `gamemodule.dll`.
### 🚀 `PatternMapper`
Создаёт и удаляет метку-маркер в памяти игры `game.exe`.

## 🎢 Режимы маппинга
Проект позволяет гибко управлять тем, какие детекты и каким способом будут замаплены в систему. Это сделано для удобства изолированного тестирования независимых методов детекта сканеров.

### Управление категориями мапперов
Добавлены три основных типа маркеров которые можно включить и выключить:
```cpp
#define MAP_USERDIR_ENABLE(detect)
#define MAP_GAMEDIR_ENABLE(detect)
#define MAP_PATTERN_ENABLE(detect)

#define MAP_USERDIR_DISABLE(detect)
#define MAP_GAMEDIR_DISABLE(detect)
#define MAP_PATTERN_DISABLE(detect)
```

Пример их использования вы можете найти в **`dllmain.cpp`**:
```cpp
    // UserDir mapping
    for (const auto& cs16_ecdMapUserDirFileNames : Detects::cs16_ecdMapUserDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapUserDirFileNames)) {
            g_vecEcdMapUserDirFileName.push_back(cs16_ecdMapUserDirFileNames);
        }
    }
```
```cpp
    // GameDir mapping
    for (const auto& cs16_ecdMapGameDirFileName : Detects::cs16_ecdMapGameDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapGameDirFileName)) {
            g_vecEcdMapGameDirFileName.push_back(cs16_ecdMapGameDirFileName);
        }
    }
```
```cpp
    // Pattern mapping
    for (const auto& cs16_ecdMapPatterns : Detects::cs16_ecdMapPatterns) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapPatterns)) {
            g_vecEcdMapPattern.push_back(cs16_ecdMapPatterns);
        }
    }
```

## 📞 Дисклеймер
**Этот проект создан исключительно в образовательных, исследовательских и тестовых целях.**\
**Запрещено:**
  1. **Использовать на серверах любых игр против других игроков.**
  2. **Применять для обхода античитов или введения в заблуждение администраторов серверов.**
  3. **Выдавать результаты работы форка за реальное наличие читерского ПО у других пользователей.**

**Ты соглашаешься с тем, что:**
  1. **Автор форка не несёт ответственности за блокировки аккаунтов, **VAC-баны**, **HWID-баны**, **баны на серверах и любые другие санкции**, применённые к тебе или третьим лицам.**
  2. **Вся ответственность за использование этого ПО лежит исключительно на тебе.**
  3. **Ты используешь этот код на свой страх и риск.**

**Техническое предупреждение:**
  1. **DLL** **создаёт реальные следы, и в файловой системе, и в оперативной памяти процесса. Некорректное использование может привести к ложным срабатываниям анти-читов и сканеров.**
  2. **Независимые методы связанные с **RegeditDir** были удалены и не будут добавлены из-за возможных проблем и последствий с системой после неправильного использования.**
