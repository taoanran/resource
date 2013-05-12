#!/bin/bash
set -e

JOBS=$1
if [[ $JOBS = "" ]]
then
    echo "usage: $0 jobs"
    echo "for example:"
    echo "$0 128"
    exit 1
fi

echo "########### premake kernel ###################"
pushd kernel
if 	[ -e .config ]
then
	echo  ".config exited "
else
	make godbox_ics_defconfig
fi
make -j $JOBS uImage
popd

echo "########### make hi*.ko ######################"
pushd device/hisilicon/godbox/driver/sdk
make -f mpi.mk -j $JOBS
popd

echo "########### update-api #######################"
make update-api -j $JOBS 2>&1 | tee update-api.log

echo "########### make image #######################"

rm -rf out/target/product/godbox/installed-files.txt
make -j $JOBS 2>&1 | tee mk.log

if [ -f out/target/product/godbox/installed-files.txt ];
then
	echo "ics compile success!"
else
        echo "ics compile fail!";
	exit 1;
fi

echo "########### make kernel ######################"
pushd kernel
make -j $JOBS uImage CONFIG_INITRAMFS_SOURCE="../out/target/product/godbox/root"
popd
cp -avf kernel/arch/arm/boot/uImage out/target/product/godbox/kernel

echo "########### make yaffs2 images ###############"
pushd out/target/product/godbox
./mksystemyaffs2
popd 

echo "########### make recovery image and update.zip ###############"
pushd kernel_recovery
./mkrecovery.sh
popd

echo ""
echo " OK! "
