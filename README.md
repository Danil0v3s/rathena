<img src="doc/logo.png" align="right" height="90" />

# rAthena
![linux](https://img.shields.io/github/actions/workflow/status/Danil0v3s/rathena/build_servers_linux.yml?label=linux%20build&logo=linux) 
![macos](https://img.shields.io/github/actions/workflow/status/Danil0v3s/rathena/build_servers_macos.yml?label=macos%20build&logo=apple) 
![windows](https://img.shields.io/github/actions/workflow/status/Danil0v3s/rathena/build_servers_windows.yml?label=windows%20build&logo=visualstudio) 
![GitHub](https://img.shields.io/github/license/rathena/rathena.svg) 
![commit activity](https://img.shields.io/github/commit-activity/w/rathena/rathena) 
![GitHub repo size](https://img.shields.io/github/repo-size/rathena/rathena.svg)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/rathena/rathena)


> rAthena is a collaborative software development project revolving around the creation of a robust massively multiplayer online role playing game (MMORPG) server package. Written in C++, the program is very versatile and provides NPCs, warps and modifications. The project is jointly managed by a group of volunteers located around the world as well as a tremendous community providing QA and support. rAthena is a continuation of the eAthena project.

[Forum](https://rathena.org/board)|[Discord](https://rathena.org/discord)|[Wiki](https://github.com/rathena/rathena/wiki)|[FluxCP](https://github.com/rathena/FluxCP)|[Crowdfunding](https://rathena.org/board/crowdfunding/)|[Fork and Pull Request Q&A](https://rathena.org/board/topic/86913-pull-request-qa/)
--------|--------|--------|--------|--------|--------

### Table of Contents
1. [Prerequisites](#1-prerequisites)
2. [Installation](#2-installation)
3. [Troubleshooting](#3-troubleshooting)
4. [More Documentation](#4-more-documentation)
5. [How to Contribute](#5-how-to-contribute)
6. [License](#6-license)

## 1. Prerequisites
Before installing rAthena there are certain tools and applications you will need which
differs between the varying operating systems available.

### Hardware
Hardware Type | Minimum | Recommended
------|------|------
CPU | 1 Core | 2 Cores
RAM | 1 GB | 2 GB
Disk Space | 300 MB | 500 MB

### Operating System & Preferred Compiler
Operating System | Compiler
------|------
Linux / macOS | gcc 11+ or clang 14+, [CMake 3.21+](https://cmake.org/download/), [Ninja](https://ninja-build.org/) or Make
Windows | [MS Visual Studio 2022 or 2026](https://www.visualstudio.com/downloads/) with the "C++ CMake tools" component

All platforms build with CMake. Dependencies (MariaDB client, PCRE, zlib, yaml-cpp, rapidyaml, libconfig, cpp-httplib, nlohmann-json) are declared in `vcpkg.json` and fetched/built by [vcpkg](https://vcpkg.io) automatically on first configure; `git`, `curl`, `zip`, `unzip`, `tar` and `pkg-config` must be installed for that.

### Required Applications
Application | Name
------|------
Database | [MySQL 5 or newer](https://www.mysql.com/downloads/) / [MariaDB 5 or newer](https://downloads.mariadb.org/)
Git | [Windows](https://gitforwindows.org/) / [Linux](https://git-scm.com/download/linux)

### Optional Applications
Application | Name
------|------
Database | [MySQL Workbench 5 or newer](http://www.mysql.com/downloads/workbench/)

## 2. Installation 

### Full Installation Instructions
  * [Windows](https://github.com/rathena/rathena/wiki/Install-on-Windows)
  * [CentOS](https://github.com/rathena/rathena/wiki/Install-on-Centos)
  * [Debian](https://github.com/rathena/rathena/wiki/Install-on-Debian)
  * [FreeBSD](https://github.com/rathena/rathena/wiki/Install-on-FreeBSD)

### Building

```bash
cmake --preset dev            # configure: Ninja, Release, deps via vcpkg (first run takes a few minutes)
cmake --build --preset dev    # build everything; add --target map-server etc. to build less
```

Other presets (`cmake --list-presets`): `debug` (Ninja, Debug), `make` (Unix Makefiles, for machines without Ninja),
`msvs` / `msvs-2026` (Visual Studio 2022 / 2026, Windows only). Binaries land in the repository root as before.

Common configure options, passed as `-D<option>=<value>` after the preset:

| Option | Meaning |
|---|---|
| `ENABLE_PRERE=ON` | Pre-Renewal mode |
| `PACKETVER=20211103` | Client packet version |
| `ENABLE_VIP=ON` | VIP features |
| `MAXCONN=16384` | Maximum socket connections |
| `ENABLE_LTO=ON` | Link time optimization |
| `ENABLE_EXTRA_DEBUG_CODE=ON`, `ENABLE_EXTRA_BUILDBOT_CODE=ON` | Extra debug / CI checks |
| `ENABLE_WEB_SERVER=OFF` | Skip the web server |

If vcpkg is already installed on your machine, set `VCPKG_ROOT` and it is used instead of a local clone in `.vcpkg/`.

#### Visual Studio

Visual Studio 2022 and 2026 (with the "Desktop development with C++" workload) build CMake projects
natively: **File → Open → Folder…** on the clone, pick `msvs` / `msvs-2026` (or `dev` / `debug`) in the
configuration dropdown, and build/debug as usual. The first configure takes a few minutes while vcpkg
builds the dependencies.

If you prefer a classic solution file:

```powershell
cmake --preset msvs                                  # generates build-msvs\rAthena.sln (msvs-2026: rAthena.slnx)
cmake --build --preset msvs --config Debug           # or open the solution in Visual Studio
```

The solution is generated, not committed: it embeds absolute paths and the project lists are globbed,
so re-run the preset after adding or removing source files.

### Documentation

The reference documentation lives in [`doc/`](doc/) as Markdown and is published at
<https://danil0v3s.github.io/rathena/> on every push to `master`. Preview it locally with
`pip install 'mkdocs-material>=9,<10' && mkdocs serve`.

## 3. Troubleshooting

If you're having problems with starting your server, the first thing you should
do is check what's happening on your consoles. More often that not, all support issues
can be solved simply by looking at the error messages given. Check out the [wiki](https://github.com/rathena/rathena/wiki)
or [forums](https://rathena.org/board) if you need more support on troubleshooting.

## 4. More Documentation
rAthena has a large collection of help files and sample NPC scripts located in the /doc/
directory. These include detailed explanations of NPC script commands, atcommands (@),
group permissions, item bonuses, and packet structures, among many other topics. We
recommend that all users take the time to look over this directory before asking for
assistance elsewhere.

## 5. How to Contribute
Details on how to contribute to rAthena can be found in [CONTRIBUTING.md](https://github.com/rathena/rathena/blob/master/.github/CONTRIBUTING.md)!

## 6. License
Copyright (c) rAthena Development Team - Licensed under [GNU General Public License v3.0](https://github.com/rathena/rathena/blob/master/LICENSE)
