pipeline {
  agent { label 'linux-agent' }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
            }
        }

        stage('Configure') {
            steps {
                sh '''
                    mkdir -p build
                    cmake -S . -B build
                '''
            }
        }

        stage('Build') {
            steps {
                sh 'cmake --build build'
            }
        }

        stage('Test') {
            steps {
                sh '''
                    cd build
                    ctest --output-on-failure
                '''
            }
        }

        stage('Run') {
            steps {
                sh '''
                    ./build/ai_job_search \
                    data/resume_skills.txt \
                    data/job_skills.txt
                '''
            }
        }
    }
}
