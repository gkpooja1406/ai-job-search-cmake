pipeline {
    agent {
        label 'linux-agent'
    }

    options {
        timestamps()

        buildDiscarder(logRotator(
            numToKeepStr: '20',
            artifactNumToKeepStr: '20'
        ))
    }

    stages {

        stage('Checkout') {
            steps {
                echo 'Checking out source code...'
                checkout scm
            }
        }

        stage('Verify Tools') {
            steps {
                sh '''
                    set -e

                    echo "Checking required tools..."

                    cmake --version
                    gcc --version
                    make --version
                    objcopy --version || echo "GNU objcopy not found, make sure arm-none-eabi-objcopy or standard objcopy is available"
                '''
            }
        }

        stage('Configure CMake') {
            steps {
                echo 'Generating CMake build files...'

                sh '''
                    set -e

                    mkdir -p build
                    cmake -S . -B build
                '''
            }
        }

        stage('Build') {
            steps {
                echo 'Compiling AI Job Search project...'

                sh '''
                    set -e

                    cmake --build build
                '''
            }
        }

        stage('Generate Artifacts') {
            steps {
                echo 'Generating .bin, .elf, .hex, and report.txt files...'
                
                sh '''
                    set -e
                    
                    # Ensure we are working inside the build directory
                    cd build
                    
                    # 1. Create the .elf file (copying the main executable)
                    cp ai_job_search ai_job_search.elf
                    
                    # 2. Extract raw binary format from the ELF file
                    objcopy -O binary ai_job_search.elf ai_job_search.bin
                    
                    # 3. Extract Intel Hex format from the ELF file
                    objcopy -O ihex ai_job_search.elf ai_job_search.hex
                    
                    # 4. Generate the report.txt file summarizing the build
                    echo "Build Report" > report.txt
                    echo "==================" >> report.txt
                    echo "Build Timestamp: $(date)" >> report.txt
                    echo "Binary Sizes:" >> report.txt
                    ls -lh ai_job_search* >> report.txt
                '''
            }
        }

        stage('Test and Run in Parallel') {
            parallel {

                stage('Unit Test') {
                    steps {
                        echo 'Running Unit Tests...'

                        sh '''
                            set -e

                            cd build
                            ctest --output-on-failure
                        '''
                    }
                }

                stage('Run Application') {
                    steps {
                        echo 'Running AI Job Search application...'

                        sh '''
                            set -e

                            ./build/ai_job_search \
                            data/resume_skills.txt \
                            data/job_skills.txt \
                            | tee build/application-output.txt
                        '''
                    }
                }
            }
        }

        stage('Archive Artifacts') {
            steps {
                echo 'Archiving executable and formatted output files...'

                # Includes all exact artifact patterns shown in the image layout
                archiveArtifacts(
                    artifacts: 'build/ai_job_search, build/ai_job_search.bin, build/ai_job_search.elf, build/ai_job_search.hex, build/report.txt',
                    fingerprint: true,
                    onlyIfSuccessful: true
                )
            }
        }
    }

    post {
        success {
            echo 'AI Job Search CMake project built successfully.'
            echo 'Artifacts archived successfully.'
        }

        failure {
            echo 'Pipeline failed. Check Console Output.'
        }

        always {
            echo "Job Name: ${JOB_NAME}"
            echo "Build Number: ${BUILD_NUMBER}"
        }
    }
}
