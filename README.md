# 🧪 EasyCheatDetector Mapper — Debug & Test Fork

![GitHub forks](https://img.shields.io/github/forks/Ml-skibidist/ecd_sh1t?style=flat-square&color=blue)
![GitHub License](https://img.shields.io/github/license/Ml-skibidist/ecd_sh1t?style=flat-square&color=green)
![Status](https://img.shields.io/badge/Status-Experimental-orange?style=flat-square)
![Base](https://img.shields.io/badge/Base%20Version-ECD%20131%20Detects-blueviolet?style=flat-square)

> [!IMPORTANT]
> Это **тестовый форк** против проекта [FunGun EasyCheatDetector (ECD)](https://github.com/UnrealKaraulov/EasyCheatDetector).
>
> **Назначение:** тестирование, дебаггинг, реверс-инжиниринг, изучение работы сканера и мапперов, изучение детектов.

---

## 📑 Содержание

- [🤔 Зачем этот форк?](#-зачем-этот-форк)
- [🔍 Что делает оригинал](#-что-делает-оригинал)
- [🧬 Что внутри (технические детали)](#-что-внутри-технические-детали)
- [⚙️ Режимы маппинга](#️-режимы-маппинга)
- [⚠️ Дисклеймер](#️-дисклеймер)
- [📄 Благодарность](#-благодарность)

---

## 🤔 Зачем этот форк?
Этот форк нужен для **изучения**, **анализа** и **реверсинга** сканера.
Понимание сути работы методов детекта, их обходов, а также эксплуатаций.
Также является форком оригинала ecd_sh1t - [Тык](https://github.com/exzereyddi/ecd_sh1t)

Как этот форк помогает:
| Цель | Как достигается |
|---|---|
| 🐛 **Изучение\анализ** | Каждый тип маппинга (`UserDir`, `GameDir`, `Pattern`) можно включать изолированно друг от друга и тестировать методы детекта  |
| 🧪 **Обходы\эксплуатации** | Добавляешь детект → смотришь, загорелся ли он в сканере. Добавляешь HWID -> смотришь, упала ли БД |
| 📖 **Сигнатуры\Паттерны** | Все детекты вынесены в массив строк, можно комментировать или редактировать без ограничений |
| 🛠️ **Кастомизация** | Легко добавить свои тестовые сигнатуры без правки логики |
---

## 🔍 Что делает оригинал
Оригинальнал **[FunGun] EasyCheatDetector** сканирует систему на наличие следов читов для CS 1.6 (и частично других игр).
Делает это путём сканирование: **UserDir**, **GameDir**, **RegeditDir**, и так-далее.
Делает это разными методами детекта: **asmjit**, **regkeys**, **jsonkeys**, и так-далее.

**Методы детекта (131+ шт.):**
<details>
<summary>📦 Развернуть список методов</summary>
  
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

</details>

> [!NOTE]
> Несмотря на наличие сигнатур для CS2, Minecraft и других игр, оригинал проверяет запущенную игру (`hl.exe` / `hw.dll`)!
> Поэтому реальных детектов на CS 1.6 — **ровно 131**!

---

## 🧬 Что внутри (технические детали)

Проект использует три независимых **маппера**, каждый из которых оставляет в системе определённый "след":
### 📁 `UserDirMapper`
Создаёт и удаляет папку-маркер в `%APPDATA%`.
### 📁 'GameDirMapper'
Создаёт и удаляет папку-маркер в относительном пути игры 'hw.dll'.
### 🚀 'PatternMapper'
Создаёт и удаляет метку-маркер в памяти игры 'hl.exe'.

## ⚙️ Режимы маппинга
Проект позволяет гибко управлять тем, какие детекты и каким способом будут замаплены в систему.
Это сделано для удобства изолированного тестирования конкретных методов детектов сканера.
### 1. Управление категориями мапперов
Добавлены три основных типа маркеров которые можно включить и выключить:
```cpp
#define MAP_USERDIR_ENABLE(detect)
#define MAP_GAMEDIR_ENABLE(detect)
#define MAP_PATTERN_ENABLE(detect)
```
Пример их использования в dllmain.cpp:
```cpp
    for (const auto& ecdMapUserDirFileName : Detects::ecdMapUserDirFileNames) {
        if (MAP_USERDIR_ENABLE(ecdMapUserDirFileName)) {
            g_vecEcdMapUserDirFileName.push_back(ecdMapUserDirFileName);
        }
    }
```
```cpp
    for (const auto& ecdMapGameDirFileName : Detects::ecdMapGameDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(ecdMapGameDirFileName)) {
            g_vecEcdMapGameDirFileName.push_back(ecdMapGameDirFileName);
        }
    }
```
```cpp
    for (const auto& ecdMapPattern : Detects::ecdMapPatterns) {
        if (MAP_PATTERN_ENABLE(ecdMapPattern)) {
            g_vecEcdMapPattern.push_back(ecdMapPattern);
        }
    }
```

---

[!CAUTION]
Внимание — прочти полностью.
Этот проект создан исключительно в образовательных, исследовательских и тестовых целях.
Запрещено:
    Использовать на серверах Counter-Strike 1.6 или любых других игр против других игроков.
    Применять для обхода античитов или введения в заблуждение администраторов серверов.
    Выдавать результаты работы форка за реальное наличие читерского ПО у других пользователей.

Ты соглашаешься с тем, что:
    Автор форка не несёт ответственности за блокировки аккаунтов, VAC-баны, HWID-баны, баны на серверах и любые другие санкции, применённые к тебе или третьим лицам.
    Вся ответственность за использование этого ПО лежит исключительно на тебе.
    Ты используешь этот код на свой страх и риск.

Техническое предупреждение:
    DLL создаёт реальные следы в файловой системе (%APPDATA%, папка с игрой) и в оперативной памяти процесса. Некорректное использование может привести к ложным срабатываниям античитов и повреждению репутации невиновных игроков.
    Методы связанные с метками по **RegeditDir** были удалены из-за возможных проблем и последствий после неправильного использования.
