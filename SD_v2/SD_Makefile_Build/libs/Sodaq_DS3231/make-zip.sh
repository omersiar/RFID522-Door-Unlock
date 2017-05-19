#!/bin/sh
#
# Make a ZIP file for distribution / publishing.

#VERSION=$(git describe --abbrev=0 --tags 2> /dev/null)
VERSION=$(git describe --tags 2> /dev/null)
[ -z "${VERSION}" ] && { echo "ERROR: Cannot determine version. Did you do a git-tag?"; exit 1; }

FILES=$(git ls-files|grep -v .gitignore)
[ -z "${FILES}" ] && { echo "ERROR: No files to put in ZIP."; exit 1; }

DIRNAME="$(basename $(pwd))"
ZIPNAME="${DIRNAME}-${VERSION}"

TDIR=/tmp/$DIRNAME
[ -d "${TDIR}" ] && { echo "ERROR: Directory '${TDIR}' exists. Please remove."; exit 1; }

mkdir -p ${TDIR}
git ls-files |
grep -v .gitignore |
cpio -pmud ${TDIR}/
(cd /tmp && zip -r $ZIPNAME.zip $DIRNAME)
mv /tmp/$ZIPNAME.zip .
ln -sf -T $ZIPNAME.zip ${DIRNAME}.zip
rm -fr ${TDIR}
