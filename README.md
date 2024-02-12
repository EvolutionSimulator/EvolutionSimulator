# 🧠 EvolutionSimulator 🌍

👋 Welcome to the official repository of the **Evolution Simulator** project!

## 🌟 Overview

- 🧬 The project provides an easy way to simulate evolution through natural selection and genetic mutations.
- 🧠 The creatures use complex neural network brains with many different modules giving them information about the environment.
- 🐾 Starting out from random initialization, the organisms evolve highly complex behaviour like efficient food seeking, predation, cooperation, etc.
- 🛡️ None of the behavior is hard coded. All actions are guided through by the neural network that, through generations, adapt to survive in the environment.
- 🕹️ The software provides a way to configure your own environment factors and see which ones impact the evolution the most and in what ways.
- 🛠️ The simulation is based on a modified version of the NEAT algorithm (<a href="https://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf"> link to the original algorithm </a>).

## 💻 Installation guide

To build the project, all you need to do is open the `CMakeLists.txt` in the root folder (not the ones in `UI/` or `Engine/`) using Qt Creator. The Qt modules used in the UI are `QtWidgets` (usually installed by default) and `QtCharts`. 🛠️ If you don't have them, use the Qt Maintenance Tool to add them to your Qt installation. <ins>An internet connection is required during the initial configuration</ins> since some libraries (SFML and Google Test) are fetched from their git repositories.

The project is officially supported with **Qt 6.5.3** using the **GCC** compiler, on **Windows 10, MacOS, and Linux**. 🖥️ Clang also works, but you might need to install the OpenMP library first. The build system is CMake, so it's IDE agnostic, but you will need to link Qt libraries yourself if you plan on using any IDE other than Qt Creator.

## 📈 Contributors

<a href="https://github.com/EvolutionSimulator/EvolutionSimulator/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=EvolutionSimulator/EvolutionSimulator" />
</a>
