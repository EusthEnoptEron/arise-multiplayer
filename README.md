# arise-multiplayer
Mod that adds multiplayer to Tales of Arise

For progress, see [Milestones.](https://github.com/EusthEnoptEron/arise-multiplayer/milestones)

## Installing

1. Extract files to the game data folder.
2. Start the game. 

## Disabling

1. Remove the xinput1_3.dll

## Known Issues

- Tutorials are buggy. (#20)
- Changing characters during battle doesn't work well. (#5)
- Camera issues during strike attacks (#21)
- Special abilities only partly supported (#4)

For a complete list, see [issues](https://github.com/EusthEnoptEron/arise-multiplayer/issues)

## Controller Order

- Player numbers are initially assigned in the order Steam reports them.
- When a controller disconnects, the player number is returned to the pool.
- When a controller connects, it is assigned the first free player number.

## Controls
### Overworld Controls

Outside of battle, all controllers are treated equally and can steer the main character and navigate through menus.
This mirrors the default behavior of the game and allows for taking turns without passing around a controller.

### Battle Controls

During battle, each player gets to control their own character. Make sure that P1 (= the flag) is assigned to the top party member.

The other characters are assigned like so:

| Player | Party Member |
| ------ | ------------ |
| P2     | Left  |
| P3     | Right |
| P4     | Bottom |
