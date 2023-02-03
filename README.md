# Midnight Harvest :moon: :tomato: :corn:

This is the repository of the game, Midnight Harvest. This repository is cloned from the Roche engine branch and continually receives updates to improve the use of the editor and to ensure tools are easier to use, and bugs are fixed.

## Getting Started

Please refer to the [wiki](https://github.com/Nine-Byte-Warriors/roche-engine/wiki) and technical documentation on the various uses of the Roche engine.

### Dependencies

To use the engine, the following prerequisites must be met.
* Windows 10+
* Visual Studio
* Git Version Control

The engine relies on the following libraries and APIs to function.
* DirectX 11
* XAudio2
* ImGui
* Assimp
* nlohmann

### Installing

To download a copy of the engine, select "Download ZIP" from the main code repository page, or create a fork of the project. More information on forking a GitHub respository can be found [here](https://www.youtube.com/watch?v=XTolZqmZq6s).

### Executing program

As the project settings have been modified to support the addition of the aforementioned libraries and APIs, there are no additional steps required to execute the application.

Running the application in "Debug" mode will enable the ImGui windows for development. Running the project in "Release" mode disables the ImGui windows improving performance and allowing the developer to interact with the game.

## Help

Refer to the [wiki](https://github.com/Nine-Byte-Warriors/roche-engine/wiki), and technical documentation for info regarding the uses of the engine.

If you are experiencing issues with the engine on your feature branch, or fork, be sure to update from the master branch to see if this resolve the issue. Assuming your branch is titled "feat/mybranch," use the following commands to merge the changes from the master branch.

```
git checkout feat/mybranch
git merge origin/master
git push origin feat/mybranch
```

With rebase, you can use the following commands.

```
git fetch
git rebase origin/master
```

Please reach out to a member of Nine Byte Warriors if you have any additional queries, concerns, or issues regarding the engine.

## Authors

_Engine Developers_
| Name                      | Profile                                                   |
| ------------------------- | --------------------------------------------------------- |
| Kyle Robinson             | [kyle-robinson](https://github.com/kyle-robinson)         |
| Juliusz Kaczmarek         | [juliusz-kaczmarek](https://github.com/juliusz-kaczmarek) |
| Maurice Thompson-Hamilton | [BlackRece](https://github.com/BlackRece)                 |
| Charlie Morris            | [charliemorris56](https://github.com/charliemorris56)     |
| Alvin Aggrey              | [Alvin Aggrey](https://github.com/AlvinAggrey)            |

_Game Developers_
| Name                      | Profile                                                   |
| ------------------------- | --------------------------------------------------------- |
| Will Bennett              | [WillBennett2](https://github.com/WillBennett2)           |
| James Barber              | [Tuffy137](https://github.com/Tuffy137)                   |
| Said Mozamil Sadat        | [Said-Sadat](https://github.com/Said-Sadat)               |
| Glen Nelson               | [glennelson1](https://github.com/glennelson1)             |
| Eleftherios Karakyritsis  | [lefk36](https://github.com/lefk36)                       |

## License

This project is licensed under the MIT License - see the LICENSE.txt file for details.
