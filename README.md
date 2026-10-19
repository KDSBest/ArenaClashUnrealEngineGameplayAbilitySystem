# Arena Clash

The example project for the book **Gameplay Ability System in Unreal Engine 5.8: Build a Multiplayer Arena Game from Scratch** by Kevin Daniel Setiono, co-authored with Claude (Anthropic).

Arena Clash is a small networked arena game built with Unreal Engine's Gameplay Ability System (GAS). It has:

- Three heroes: **Warrior**, **Mage** and **Support**, each defined by a data asset with its own ability set.
- Abilities: melee attack, charge, dash with root motion, Fireball, Meteor Strike, Holy Bolt, heal and a barrier ultimate.
- Mana costs, tag-driven cooldowns, a damage execution with armor and critical hits, and status effects (stun, slow, burning, shields).
- Gameplay Cues for VFX, sound and camera shake.
- AI enemies with their own Ability System Component, driven by Behavior Trees.
- A **co-op** wave mode and a **team PvP** mode, with kills, assists and score.
- A ViewModel-based HUD with ability icons, cooldown overlays, damage numbers, a scoreboard and a kill feed.
- A main menu, sessions (Null Online Subsystem), a lobby with hero selection, and seamless travel to the arena.

## Following along with the book

The Git history mirrors the book: each section that changes the project has one commit, named `Chapter X.Y - <Section title>`. To see the project as it was at the end of a section, check out that commit:

```
git log --oneline
git checkout <commit>
```

Chapters 17 and 25 make no project changes, so they have no commits.

## Requirements

- Unreal Engine **5.8**
- Visual Studio 2022 (workload *Game development with C++*) or JetBrains Rider
- Windows 10/11 (the platform used in the book)

## Getting started

1. Clone the repository.
2. Right-click `ArenaClash/ArenaClash.uproject` and choose **Generate Visual Studio project files**.
3. Open `ArenaClash.sln`, select **Development Editor** / **Win64**, and build.
4. Open the project in the editor. The startup map is `L_Arena`.

### Playing

- **In the editor:** set *Number of Players* to 2 and *Net Mode* to *Play As Listen Server*, then press Play.
- **Full flow (menu, lobby, travel):** start two instances with `-game` (for example, right-click the `.uproject` and choose **Launch Game**). Host in the first, then use Find Games and join in the second. Pick heroes and start the match.
- Seamless travel in PIE needs the console command `net.AllowPIESeamlessTravel 1`.

### Controls

| Input | Action |
| --- | --- |
| WASD / Mouse | Move / look |
| LMB, RMB, Q, E, R | Abilities (depending on the hero) |
| Space | Jump |
| Left Shift | Dash |
| Tab (hold) | Scoreboard |
| H | Choose hero (lobby) |

## Project layout

```
ArenaClash/
  Source/ArenaClash/          C++: ASC setup, attribute set, abilities, damage execution,
                              game mode/state, player state, AI, wave spawner, ViewModel
  Content/ArenaClash/
    Abilities/                Gameplay abilities per hero (Warrior, Mage, Support, Common, Enemy)
    Effects/                  Gameplay effects (damage, costs, cooldowns, status)
    Heroes/                   Hero pawns and hero data assets / ability sets
    Enemies/                  Grunt and Caster enemies, Behavior Trees
    UI/                       HUD, ability bar, scoreboard, kill feed, menus, lobby
    Maps/                     L_MainMenu, L_Lobby, L_Transition, L_Arena
```

## License

The code written for this project is released under the [MIT License](LICENSE).

The project was created from the Unreal Engine Third Person template. Epic Games' template code and content (files with the "Copyright Epic Games, Inc." header and the template folders listed in `LICENSE`) and Unreal Engine itself remain subject to the [Unreal Engine EULA](https://www.unrealengine.com/eula/unreal).
