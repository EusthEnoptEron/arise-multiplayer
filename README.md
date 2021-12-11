# Tales of Arise: Local Co-Op Mod

![Assets/Sample.jpg](Assets/Sample.jpg)

Mod that adds multiplayer to Tales of Arise

For progress, see <!-- [Milestones](https://github.com/EusthEnoptEron/arise-multiplayer/milestones) and--> the [Project page](https://github.com/EusthEnoptEron/arise-multiplayer/projects/1).

## Installing

1. Extract files to the game data folder.
2. Start the game.

During battle, controllers will automatically be assigned to the characters. See below for details on the order.

If `xinput1_3.dll` already exists because you installed another mod, skip the file and launch
UnrealEngineModLauncher.exe before starting the game.

## Disabling

1. Remove xinput1_3.dll

## Known Issues

- Might cause issues in some of the later dungeons / bosses
- Strike Attacks always use the P1 target

For a complete list, see [issues](https://github.com/EusthEnoptEron/arise-multiplayer/issues)

Please comment or add new issues if you encounter any that haven't been reported yet.

## Controller Order

- Player numbers are initially assigned in the order Steam reports them.
- When a controller disconnects, the player number is returned to the pool.
- When a controller connects, it is assigned the first free player number.

## Controls

### Overworld Controls

Outside of battle, all controllers are treated equally and can steer the main character and navigate through menus. This
mirrors the default behavior of the game and allows for taking turns without passing around a controller.

### Battle Controls

During battle, each player gets to control their own character. Make sure that P1 (= the flag) is assigned to the top
party member.

The other characters are assigned like so:

| Player | Party Member |
| ------ | ------------ |
| P2     | Left  |
| P3     | Right |
| P4     | Bottom |

## Customizing the Camera

The camera is still a work in progress. You can however tweak it to your liking by editing
a `MultiplayerMod.ini` file that you place at `...\steamapps\common\Tales of Arise\Arise\Binaries\Win64\MultiplayerMod.ini`. The folder already contains an example file -- simply rename it and delete the `.example` part.

**Hot-reloads when changes are made.**
 
## P2: "I want to walk around with *my* character outside of battles!"

If you're taking turns with who plays the base game, and you want to use your own character, set `AutoChangeCharas` to `1` in the config (see [Customizing the Camera](#customizing-the-camera)).

Now when you press "Reset Camera 2" (default is left bumper), you will immediately switch to the character you would control in battle.
