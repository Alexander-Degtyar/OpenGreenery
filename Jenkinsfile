pipeline {
    agent any

    stages {
        stage("Build") {
            steps {
                sh 'docker run --rm -v og-artifacts:/var/og-artifacts --name "og-build_${BRANCH_NAME}-${BUILD_NUMBER}" -e "BRANCH=jenkins-build" -e "COPYPATH=tests/tests" og-build'
            }
        }
        
        stage("Tests run") {
            steps {
                sh 'docker run --rm -v og-artifacts:/var/og-artifacts --name "og-run-tests_${BRANCH_NAME}-${BUILD_NUMBER}" og-run /var/og-artifacts/tests'
            }
        }
    }
}
