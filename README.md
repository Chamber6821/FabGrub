# Draft of the profile system for Factorio mods

# Hot to use

```shell
$ fabgrub run profile1
```

# What is profile?

Profile is JSON file that looks like that:

File `<factorio-location>/profiles/space.json`:

```json
{
  "space-exploration" : "^1.0.0",
  "another-mod" : "^0.0.0",
  "problem-mod" : "^2.0.0 <2.13.0"
}
```

Command for run this: `fabgrub run space`

# How to build

Note: You should have installed `CMake` and `make`

Clone this repository, configure `configuration.mk` for yourself and run:

```
$ make app
```

# Technically details

- User can load and start game with profile
    - If some profile already loaded, it will be unloaded
    - If the profile contains incompatible mods, user will see that combination
- Mods will be downloaded from https://re146.dev/factorio/mods/
- Last used mods will be cached in special folder

## What is profile loading?

Load profile means that for each mod range in profile must be a selected compatible version and putted into
folder `mods`

Concrete version of mod can require another mod to be installed,
that requirement must be solved automatically (not manual)
  
