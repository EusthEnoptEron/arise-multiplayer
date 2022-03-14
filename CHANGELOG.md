# Changelog

## 1.0.3

- Implemented haptic feedback for all players (#8).
- Fixed a bug where the input direction was ignored in manual mode (#64).
- Some sorely needed refactoring to keep the code base manageable. Might have broken stuff.

## 1.0.2

- Added option to disable multiplayer camera. (`UseSinglePlayerCamera`)
- Added option to disable hit stop behavior. (`DisableHitStop`)
- Fixed issue where the player number would wiggle.

## 1.0.1

- Improved UI z-ordering. (UI should now show on the correct layer.)
- Added option to control the intensity of the camera shake effect. (`CameraShakeScale`)

## 1.0.0

- Added option to make camera ignore dead players. (`IgnoreDeadPlayers`)
- Added option to restrict boost attacks to P1. (`RestrictBoostAttacksToP1`)
- Added options to customize cursors and made them smaller by default.

## 1.0.0-rc.5

- Allow P2-P4 to change characters during battle without reordering.
- Added UI elements that show who controls which character.
- Added the possibility to restrict the usage of boost attacks.

## 1.0.0-rc.4

- Allow P2-P4 to quickly change targets. (#56)
- Fixed a crash that could occur when unplugging a controller during battle.

## 1.0.0-rc.3

- Fixed an issue where the multiplayer camera activated even though it was a single-player battle. (#55)
- Added a new option that allows making mouse & keyboard P1.

## 1.0.0-rc.2

- Fixed an issue where P2-P4 could control the direction of P1 attacks in manual mode. (#44)
- Fixed a crash pressing left bumper at the wrong time. (#52)

## 1.0.0-rc.1

- Made boost attack act predictably (fixes #15) (please tell me if you encounter any strange effects)
- Fixed colors for P3-P4 (fixes #49)
- Made it so that the HP bar is kept on the screen (fixes #23)

## 1.0.0-beta.4

- Made mod compatible with latest ToA patch (1.04)
- Fixed a bug that would lock the camera when switching back to single-player

## 1.0.0-beta.3

- Fixed an issue where P2-P4 would auto-guard and auto-step like crazy
- Fixed crashes in Spirit Temple
- Made it so that the multiplayer camera will only kick in when there are actual targets. (Mainly affects the spirit
  temple and at the beginning of boss fights.)

## 1.0.0-beta.2

- Added new options to control the pitch range of the camera
- Added a new option to allow changing characters on the fly outside of battles
- Removed config file from release archive. This way, you don't have to backup your local changes anymore.
- Stabilized camera transitions
- Improved loading behavior
- AI should now kick back in on changing charas (in most cases)
- Changing charas should be way more robust
- Stability improvements
- Fixed a bug where players had to join before the battle

## 1.0.0-beta

- Fixed camera
- Fix UI visibility during battle

## 1.0.0-alpha.4

- Mystic Artes should work for real
- Fixed retry screen
- Tinkered with camera transitions (WIP)

## 1.0.0-alpha.3

- Mystic Artes ~~should work~~ work partially
- Performance improvements

## 1.0.0-alpha.2

- All special abilities should work now
- ~~Mystic Artes should work now~~
- Counter edges should work
- Quick change target should work
- Completely changed the way input is handled (which probably creates new issues and solves other)

## 0.9f

- Stability improvements.

## 0.9e

- HOTFIX: Issue with the P1 cursor (#28)
- Make P-labels a little smaller
- Fixed a cause for random crashes for good  (hopefully) #17
- Added some new camera options for `MultiplayerMod.ini`. Most notably, you can now configure the FOV and the speed.
- Added P1 as a separate cursor. #3

## 0.9d

- Fixed a cause for random crashes for good  (hopefully) #17
- Added some new camera options for `MultiplayerMod.ini`. Most notably, you can now configure the FOV and the speed.
- Added P1 as a separate cursor. #3

## 0.9c

- Disabled multiplayer during tutorials. This should fix at least some of the issues there.

## 0.9b

- There's now an `MultiplayerMod.ini` file included that you can use to tweak the camera. You can edit the file while
  the game is running -- it will update on the fly.
- (Probably) fixed some clipping issues during cutscenes.
