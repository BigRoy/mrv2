#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# mrv2
# Copyright Contributors to the mrv2 Project. All rights reserved.



extract_version()
{
    major=`cat cmake/version.cmake | grep -Po '(VERSION_MAJOR\s)\d' | sed -e 's/VERSION_MAJOR\s*//'`
    minor=`cat cmake/version.cmake | grep -Po '(VERSION_MINOR\s)\d' | sed -e 's/VERSION_MINOR\s*//'`
    patch=`cat cmake/version.cmake | grep -Po '(VERSION_PATCH\s)\d' | sed -e 's/VERSION_PATCH\s*//'`
    export mrv2_VERSION="${major}.${minor}.${patch}"
}


#
# SOME DEFINES
#
GIT_EXECUTABLE=git
DOCKER_EXECUTABLE=docker

extract_version


#
# Delete local tag if available
#
echo "Remove local tag v${mrv2_VERSION}"
${GIT_EXECUTABLE} tag -d v${mrv2_VERSION}

#
# Delete remote tag if available
#
echo "Remove remote tag v${mrv2_VERSION}"
${GIT_EXECUTABLE} push --delete origin v${mrv2_VERSION}



#
# Mark current repository with a new tag
#
echo "Create local tag v${mrv2_VERSION}"
${GIT_EXECUTABLE} tag v${mrv2_VERSION}

#
# Send new tag to repository
#
echo "Create remote tag v${mrv2_VERSION}"
${GIT_EXECUTABLE} push origin v${mrv2_VERSION}

#
# Remove the main images if present
#
echo "Remove rockylinux image..."
${DOCKER_EXECUTABLE} rmi rockylinux:8 --force

echo "Remove mrv2_builder image..."
${DOCKER_EXECUTABLE} rmi mrv2_builder:latest --force

echo "Run the ${DOCKER_EXECUTABLE} build..."
runme_docker.sh