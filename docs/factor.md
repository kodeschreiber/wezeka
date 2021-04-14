# FACTOR
---
Factor is a build assistant tool designed to make organizing modules and their construction much easier.

When running the `wzk factor` command, you must be in the target directory containing your build files.

---

## Command Usage:
git factor OPERATION TARGET FLAGS

OPERATIONS:
  - build <entry> <optional: -f|--force>
      This performs a build on a specified entry. It checks to see if rebuilding is necessary
      (which can be overidden with --force). The associated repository is cloned (if specified)
      and tag checked-out (default is master). Using WZK IFIO, the cloned directory is
      executed with either the prespecified script or one that IFIO locates. This execution
      uses the specified environments (see WZK runenv) and endo-container (see WZK isolate).

  - refresh
      Using available FTR files in the project directory, repopulate the .factor directory
      with up-to-date files. Run this before using 'build' to ensure that scripts, environments,
      and expos are correct

  - reset
      Remove all repository caches, IFIO mnemonics, and endo-containers

  - clean
      Remove an enties resources from the .factor directory

  - conduct
      Run a defined `expo` section
---

## Installation
Factor only requires a few things:
1. One or more '<filename>.ftr' files in the git repository
2. Commands: python3, git

---

## FTR files
FTR files can be located anywhere in your git repository. FTR files are essentially
`ini` files. In fact, they are desinged the same way that GIT configs are.

There are five types of sections you may use:
  - entry
  - exec
  - env
  - endo
  - expo

Meta sections are structured as:
```
[entry name]
  url=
  tag=
  env=env1 ... envN
  exec=exec_ref
  endo=expo_ref
```
Here are the rules to the CONF file:
1. `url` should specify the location to the target GIT repository. It is required.
2. `tag` must be a valid tag in that repository. The default is `master`
3. `endo` specifies an isolated environment section by the given name
4. `env` is a space-separated list of section names in the `factor.conf`. Add entries to those sections; this will automatically export shell variables for your build scripts (no need to put 'export')
5. `exec` is the name of the section to execute in the module's directory.

Exec sections are free-form. They should be treated as parts to a script, later combined by reference in the `entry` section. Here's what one could look like:
```
[exec name]
#!/bin/bash

echo hello world
```

Env sections are structured as:
```
[env name]
PATH=/bin:/usr/bin
CUSTOM_VAR=foo
CC=/usr/bin/gcc
```
Note that you do not need to put the `export` keyword. This is done automatically.

Endo sections can have two keys:
```
venn=endo1 ... endo2
bind=dir1 ... dirN
```
Venn is a list of other defined endo sections
Bind is a list of directories to be Read-Only bind mounted to the isolated subroot

Expo sections are formatted as YAMLs. Each key is an entry name (`git factor build` will be ran on it).
This is designed so that you can list dependencies out for each entry. YAML anchors do work for this,
and are encouraged to reduce redundancy. In addtion. all enrties must end in a colon, even if they have
no dependencies


There is one reserved section called `factor` used to define global properties for
your repository. It is not required, but can be used to override default values as
outlined below:
```
[factor]

```
---

## Build Variables
The following variables are injected to every buildscript's environment:

- GIT_ROOT - Path to the parent git directories root.
- IFIO_PATH - This can be appended to, but in general, holds the directory where the exec sections are exported to.
