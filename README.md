# arise-multiplayer
Mod that adds multiplayer to Tales of Arise

For progress, see [Milestones.](https://github.com/EusthEnoptEron/arise-multiplayer/milestones)


## Controller Order

- Player numbers are initially assigned in the order Steam reports them.
- When a controller disconnects, the player number is returned to the pool.
- When a controller connects, it is assigned the first free player number.

## Controls
### Overworld Controls

Outside of battle, all controllers are treated equally and can steer the main character and navigate through menus.
This mirrors the default behavior of the game and allows for taking turns without passing around a controller.

### Battle Controls

During battle, each player gets to control their own character. Make sure that P1 is assigned the top party member.

```
    P1
 P2    P3
    P4
```
