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
