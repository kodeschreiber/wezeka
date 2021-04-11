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

  - 
---

## Installation
Here's a simple layout diagram:  

![factor_diagram](factor.png)

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

Meta sections are structured as:
```
[entry name]
  url=
  tag=
  prefix=
  env=env1 ... envN
  exec=exec1 ... execN
  requires=req1 ... reqN
  noclean=true|false
  verbose=true|false
  freeze=true|false
```
Here are the rules to the CONF file:
1. `url` should specify the location to the target GIT repository. It is required.
2. `tag` must be a valid tag in that repository. The default is `master`
3. `requires` is a space-separated list of other resource names. They will be built first.
4. `env` is a space-separated list of section names in the `factor.conf`. Add entries to those sections; this will automatically export shell variables for your build scripts (no need to put 'export')
5. `prefix` is the final destination of your resource's build. It will given as the variable `PREFIX` in your build script. This value changes if `isolate` is defined. You may also use the `%git%` variable to specify the path your GIT project root.
6. `script` is the location of the buildscript to execute in the module's directory. The scipt must be set as execuatble. You may use the `%git%` syntax to specifiy the location.

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

There is one reserved section called `factor` used to define global properties for
your repository. It is not required, but can be used to override default values as
outlined below:
```
[factor]

```
---

## Build Variables
The following variables are injected to every buildscript's environment:

- PREFIX_CACHE - Path to the `<gitfolder>/.factor/<module>/cache` directory.
- PREFIX_BUILD - Path defined by the `prefix` variable in the factor.conf
- GIT_ROOT - Path to the parent git directories root.
