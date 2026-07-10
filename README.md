# AI Job Search CMake Project

This C application compares resume skills with job-required skills and calculates a match percentage.

## Project structure

```text
ai-job-search-cmake/
├── CMakeLists.txt
├── Jenkinsfile
├── README.md
├── .gitignore
├── data/
│   ├── resume_skills.txt
│   └── job_skills.txt
├── include/
│   └── job_matcher.h
├── src/
│   ├── main.c
│   └── job_matcher.c
└── tests/
    └── test_job_matcher.c
```

## Install tools

Ubuntu:

```bash
sudo apt update
sudo apt install -y build-essential cmake git
```

Amazon Linux 2023:

```bash
sudo dnf install -y gcc gcc-c++ make cmake git
```

## Build locally

```bash
cmake -S . -B build
cmake --build build
```

## Test

```bash
cd build
ctest --output-on-failure
cd ..
```

## Run

```bash
./build/ai_job_search data/resume_skills.txt data/job_skills.txt
```

## Jenkins setup

1. Push the project to GitHub or GitLab.
2. Open Jenkins and select **New Item**.
3. Enter `AI-Job-Search-CMake`.
4. Select **Pipeline**.
5. Under Pipeline, choose **Pipeline script from SCM**.
6. Select **Git**.
7. Enter the repository URL.
8. Add credentials if the repository is private.
9. Set Branch Specifier to `*/main`.
10. Set Script Path to `Jenkinsfile`.
11. Save and select **Build Now**.

## View previous output

Open:

```text
Jenkins Dashboard
→ AI-Job-Search-CMake
→ Build History
→ Select build number
→ Console Output
```

Each retained build also contains:

- `ai_job_search`
- `application-output.txt`

under **Artifacts**.

The Jenkinsfile does not use `rm -rf build`. It performs an incremental build.
