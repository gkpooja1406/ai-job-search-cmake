pipeline {
    agent { label 'linux-agent' }

    options {
        timestamps()

        buildDiscarder(
            logRotator(
                numToKeepStr: '20',
                artifactNumToKeepStr: '20'
            )
        )
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
                    objcopy --version
                    file --version
                '''
            }
        }

        stage('Configure') {
            steps {
                echo 'Configuring CMake project...'

                sh '''
                    set -e

                    mkdir -p build
                    cmake -S . -B build
                '''
            }
        }

        stage('Build') {
            steps {
                echo 'Building AI Job Search project...'

                sh '''
                    set -e

                    cmake --build build
                '''
            }
        }

        stage('Test') {
            steps {
                echo 'Running unit tests...'

                sh '''
                    set -e

                    cd build
                    ctest --output-on-failure
                '''
            }
        }

        stage('Run') {
            steps {
                echo 'Running AI Job Search application...'

                sh '''
                    set -e

                    ./build/ai_job_search \
                        data/resume_skills.txt \
                        data/job_skills.txt \
                        | tee build/report.txt
                '''
            }
        }

        stage('Generate Artifacts') {
            steps {
                echo 'Generating ELF, BIN and HEX files...'

                sh '''
                    set -e

                    cp build/ai_job_search build/ai_job_search.elf

                    objcopy -O binary \
                        build/ai_job_search \
                        build/ai_job_search.bin

                    objcopy -O ihex \
                        build/ai_job_search \
                        build/ai_job_search.hex
                '''
            }
        }

        stage('Verify Artifacts') {
            steps {
                sh '''
                    set -e

                    echo "Generated files:"
                    ls -lh build/report.txt
                    ls -lh build/ai_job_search
                    ls -lh build/ai_job_search.elf
                    ls -lh build/ai_job_search.bin
                    ls -lh build/ai_job_search.hex

                    echo "File types:"
                    file build/report.txt
                    file build/ai_job_search
                    file build/ai_job_search.elf
                    file build/ai_job_search.bin
                    file build/ai_job_search.hex
                '''
            }
        }

        stage('Archive Artifacts') {
            steps {
                archiveArtifacts(
                    artifacts: 'build/report.txt,build/ai_job_search,build/ai_job_search.elf,build/ai_job_search.bin,build/ai_job_search.hex',
                    fingerprint: true,
                    onlyIfSuccessful: true
                )
            }
        }
    }

    post {
        success {
            echo 'Build, test, execution and artifact archiving completed successfully.'
        }

        failure {
            echo 'Pipeline failed. Check the failed stage in Console Output.'
        }

        always {
            echo "Job name: ${JOB_NAME}"
            echo "Build number: ${BUILD_NUMBER}"
            echo "Workspace: ${WORKSPACE}"
        }
    }
}
