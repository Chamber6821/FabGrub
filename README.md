# Draft of the profile system for Factorio mods

# Functions:

- [ ] Users can work with the list of named profiles
    - [ ] User can create a new empty profile with name
    - [ ] User can delete exists profile by name
    - [ ] User can add a new mod to the profile
    - [ ] User can change added version range for mod
    - [ ] User can delete added mod from profile
    - [ ] User can export profile to text file
    - [ ] User can import profile from text file
    - [ ] User can see the profile name and mods in this profile
    - [ ] User can see the names of all existing profiles
    - [ ] User can load profile by name
        - [ ] If some profile already loaded, it must be unloaded
        - [ ] Load profile means that for each mod range in profile must be a selected compatible version
          and putted into folder `mods`
        - [ ] Concrete version of mod can require another mod to be installed,
          that requirement must be solved automatically (not manual)
        - [ ] If the profile contains incompatible mods, user must see that combination
- [ ] Mods must be downloaded from https://re146.dev/factorio/mods/
- [ ] Last used mods must be cached
    - [ ] User can clear full mod cache
    - [ ] User can clear mod cache for concrete profile

# How to build

Note: You should have installed `CMake` and `make`

Clone this repository, configure `configuration.mk` for yourself and run:

```
$ make app MODE=Release
```
