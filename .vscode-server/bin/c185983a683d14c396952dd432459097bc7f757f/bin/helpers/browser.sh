#!/usr/bin/env sh
#
# Copyright (c) Microsoft Corporation. All rights reserved.
#
ROOT=$(dirname "$(dirname "$(dirname "$0")")")

APP_NAME="code"
VERSION="1.55.0"
COMMIT="c185983a683d14c396952dd432459097bc7f757f"
EXEC_NAME="code"
CLI_SCRIPT="$ROOT/out/vs/server/cli.js"
"$ROOT/node" "$CLI_SCRIPT" "$APP_NAME" "$VERSION" "$COMMIT" "$EXEC_NAME" "--openExternal" "$@"
