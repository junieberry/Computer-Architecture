#!/usr/bin/env sh
#
# Copyright (c) Microsoft Corporation. All rights reserved.
#
ROOT=$(dirname "$(dirname "$(dirname "$0")")")

APP_NAME="code"
VERSION="1.56.2"
COMMIT="054a9295330880ed74ceaedda236253b4f39a335"
EXEC_NAME="code"
CLI_SCRIPT="$ROOT/out/vs/server/cli.js"
"$ROOT/node" "$CLI_SCRIPT" "$APP_NAME" "$VERSION" "$COMMIT" "$EXEC_NAME" "--openExternal" "$@"
