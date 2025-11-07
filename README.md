# From paper to plugin 

Audio Developer Conference 2025, Bristol

## Getting started

Due to the possibility of issues with Wifi at the conference we ask that, ahead of the workshop, you please have the following already installed. 

Please also follow the instructions for your platform in section 1 to install the Python dependencies.

- [Python3](https://www.python.org/downloads/)
- [CMake](https://cmake.org/download/)
- [JUCE](https://juce.com/download/)
- [Reaper](https://www.reaper.fm/download.php)

### For MacOS users 

XCode (this can be downloaded from the App Store)

### For Windows users 

Visual Studio 2019 / [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) + MSVC

When running the installer, choose to install the C/C++ development tools

### 1. Notebook

#### MacOS
```bash
  # navigate to the notebooks directory in the repo
  cd notebooks
  
  # create a local python environment
  python3 -m venv .venv
  
  # activate that environment in the current shell
  source .venv/bin/activate
  
  # install the required python packages
  pip install -r requirements.txt
  
  # run the notebook
  jupyter notebook
```

#### Windows (using PowerShell as admin)
```bash
  # navigate to the notebooks directory in the repo
  cd notebooks
  
  # create a local python environment
  python3 -m venv .venv
  
  # Allow script to be temporarily run
  Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
  
  # activate that environment in the current shell
  .\.venv\Scripts\Activate.ps1
  
    # install the required python packages
  pip install -r requirements.txt
  
  # run the notebook
  jupyter notebook --no-browser
  
  # Then copy the provided link into your browser
```

### 2.Configure CMake (from root folder)
```bash
  cd ../                                                     # Come back to root folder
  cmake -B build -S .                                        # Configure and generate CMake projects including lib and tests
```

### 3.Library and Tests
```bash
  cmake --build build --target MVMFilterTests --config Release # Build MVM filter library and tests
  cd tests
  python runTests.py                                           # Run tests
```

### 4.Profiling
```bash
  cd ../                                                     # Come back to root folder
  cmake --build build --target MVMFilterProfiling --config Release  # Build MVM filter profiling
  ./build/profiling/MVMFilterProfiling                              # Run profiling (MacOS)
  .\build\profiling\Release\MVMFilterProfiling.exe                  # Run profiling (Windows)
```

### 5.Plugin
1. Open plugin/MVMFilterPlugin.jucer
2. Open project in preferred IDE
3. Build target from IDE
   - `MVMFilter - All` to build everything: plugins are copied to `~/Library/Audio/Plug-Ins/`
     - For Windows, files are generated in `plugin\Builds\VisualStudio202x\x64\Debug/VST3` and need to be copied in `C:\Program Files(x86)\SteinBerg\VstPlugins`
     - Open preferred DAW and instantiate plugin.
   - `MVMFilter - Standalone plugin` to build the standalone version and open it
