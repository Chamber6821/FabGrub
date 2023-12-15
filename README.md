# FabGrub

It is console utility for automatically assembly mods for concrete gaming session for Factorio.

# Hot to use

```shell
$ fabgrub profile1
```

# What is profile?

Profile is JSON file that looks like that:

File `<factorio-location>/fabgrub/profiles/space.json`:

```json
{
  "requirements" : [
    "space-exploration",
    "another-mod >=0.0.0",
    "problem-mod <2.13.0 # implicit conflict with space-exploration"
  ]
}
```

Command for run this: `fabgrub space`

# How to build

Note: You should have installed `CMake`, `make` and some C++ compiler, for example `Clang`

Clone this repository and run:

```
$ make app
```

# Technically details

- Before load profile, FabGrub saves user `mods` folder inside `fabgrub` folder
- After close game, FabGrub restore user `mods` folder from `fabgrub`
- If the profile contains incompatible mods, user will see that combination
- Mods will be downloaded from https://re146.dev/factorio/mods/
- All mods will be cached in special folder

## What is profile loading?

Load profile means that for each mod range in profile must be a selected compatible version and putted into
folder `mods`

Concrete version of mod can require another mod to be installed,
that requirement must be solved automatically (not manual)
  
