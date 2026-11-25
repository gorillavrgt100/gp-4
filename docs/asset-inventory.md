# GP 4 APK Asset Inventory

This inventory records names found in the user's supplied `GP V4 Update 3.1.apk` after unpacking its Unity data. It is an inspection record, not a claim that all assets have been exported.

## Source build

- APK: `GP V4 Update 3.1.apk` (~377 MB)
- Unity version: `2021.3.45f1`
- Android Unity IL2CPP build; no editable Windows project is included.

## Recovered serialized files

The unpacked data includes `globalgamemanagers`, `level0`, `level1`, `level2`, `resources.assets`, `sharedassets0.assets`, `sharedassets1.assets`, `sharedassets2.assets`, their associated `.resS` streams, and `Resources__unity_builtin_extra`.

## Verified identifiers

### Scenes and map objects
- `WELCOME TO GORILLA PURGATORY!`, `THE PURGATORY`, `CAVE`
- `ForestSpawn`, `CaveSpawn`, `CanyonSpawn`, `CitySpawn`, `MountainSpawn`
- `Spawn`, `GlobalSpawn`, `Map for gp`
- `rockwall2`, `minewall`, `minewallS`, `christmastree`
- `GorillaBody`, `Actual Gorilla`, `GorillaPlayer`, `LeftHandGorilla`, `RightHandGorilla`

### Enemies and gameplay
- `SCP096`, `DashyMonster`, `BlindMonster`, `MimicSpawner`
- `GorillaEnemySpawner`, `GorillaEnemyAI`, `EnemySpawner`, `EnemyController`
- `HalloweenGhostChaser`, `BossPandemonium`, `BossPhaseSync`
- `MonsterInstakill`, `RespawnManager`, `MonsterKillerCollider`

### SCP-096-related references
- `FractionalsSCP096BloodDisabled`
- `scp096 idle`, `scp096 chasing`, `scp096 enraging`, `scp096 triggered`, `scp096 kill`

### Audio and scare references
- `JUMPSCARE`, `jumpscare`, `jumpscareBanshee`, `pbbvjumpscare`
- `radar jumpscare`, `cosmeticjumpscare`, `start scare`
- `Spawn Audio`, `OnActivateSound`, `buzzer scare`, `explosion sound`
- `BossAnimAndAudioSync`, `SynchedMusicController`

### Materials, meshes, and textures
- `cavewall`, `cavewall1`, `Shotgun_Mat`
- `Shotgun_Mat_Base_Color`, `Shotgun_Mat_Normal_OpenGL`
- `AlienHand`, `AlienHand_AlbedoTransparency`, `AlienHand_Normal`
- `Gore Texture`, `BLOOD SKIN`, `Cobwebs`, `Material_58`
- `Shotgun_low`, `Pump_Shotgun_Game`, `Hands_1_low`, `Hands_1_low.001`
- `Combined Mesh (root: scene)` and numerous `pb_Mesh-*` objects

## Current Windows prototype

The existing source in this repository is a Win32/OpenGL prototype with procedural placeholder scenery and basic first-person controls. It does not yet import the original Unity assets. The README correctly identifies it as an early prototype, not a completed port.

## Remaining work

1. Export original models, textures, audio, prefabs, and scene data from the recovered Unity serialized files.
2. Validate the exported assets and dependencies.
3. Reconstruct the scenes, materials, player/enemy logic, and audio in a Windows-compatible project.
4. Build and test a Windows executable.

[AssetRipper](https://assetripper.github.io/AssetRipper/) is a candidate extraction tool; its official documentation lists Unity support from 3.5.0 through 6000.4.x. The current execution environment lacks AssetRipper and .NET/Mono and cannot download external binaries, so asset export and the final Windows EXE remain incomplete.

Names above are identifiers found in the supplied files. Their presence alone does not prove every associated asset is complete or exportable. Preserve the original APK unchanged.
