#!/bin/sh
set -e

# Install tools for Debian-based(apt) systems.
# Usage: sudo ./init-tools.sh

# Required: .NET 5.0 SDK
#   https://dotnet.microsoft.com/download/dotnet/5.0

echo "Setup native binary toolchain ..."

apt update
apt install build-essential cmake ninja-build mono-devel -y

# `mono-devel` is required only running regression test. (net48 platform)
# See mono's APT repository for new version: https://www.mono-project.com/download/stable/#download-lin

chmod +x ./*.sh
