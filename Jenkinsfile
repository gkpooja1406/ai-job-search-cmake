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

        stage('Generate Artifact Formats') {
            steps {
                echo 'Generating binary formats and reports...'

                sh '''
                    set -e

                    cd build

                    # Create the ELF binary format
                    cp ai_job_search ai_job_search.elf

                    # Create raw machine binary layout format
                    objcopy -O binary ai_job_search ai_job_search.bin

                    # Create Intel HEX configuration format
                    objcopy -O ihex ai_job_search ai_job_search.hex

                    # Generate the summary report text file
                    echo "AI Job Search Build Summary" > report.txt
                    echo "==========================" >> report.txt
                    echo "Build Date: $(date)" >> report.txt
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
